#include "directory_change_monitor.hpp"
#include "core/fs/fs_util.hpp"
#include "core/threading/advanced_mutex_locker.hpp"
#include "core/threading/mutex_locker.hpp"
#include "core/string/string_util.hpp"
#include "core/process/process_util.hpp"
#include "core/io/format_output.hpp"
#include "core/fs/path.hpp"

namespace pilo
{
    namespace core
    {
        namespace fs
        {

#ifdef WINDOWS
            pilo::i32_t directory_change_monitor::on_run()
            {
                DWORD numBytes = 0;
                LPOVERLAPPED lpOverlapped;
                directory_monitor_task* task_ptr = nullptr;

                do
                {
                    // Retrieve the directory info for this directory
                    // through the io port's completion key
                    BOOL b_io_comport_ok = GetQueuedCompletionStatus(m_completion_port,
                        &numBytes,
                        (PULONG_PTR)&task_ptr,//<-- completion Key
                        &lpOverlapped,
                        INFINITE);

                    
                    if (!b_io_comport_ok)
                    {
                        if (task_ptr == nullptr)
                        {
                            M_ASSERT(false);
                        }
                        if (task_ptr != nullptr && task_ptr->m_dir_handle != INVALID_HANDLE_VALUE)
                        {
                            M_ASSERT(false);
                        }
                    }

                    if (task_ptr != nullptr)
                    {
                        ::pilo::core::fs::directory_monitor_task::EnumTaskState eState;

                        task_ptr->m_mutex.lock();
                        {
                            eState = task_ptr->m_state;
                        }
                        task_ptr->m_mutex.unlock();

                        switch (eState)
                        {
                            case pilo::core::fs::directory_monitor_task::eTS_StartMonitoring:
                            {
                                if (!ReadDirectoryChangesW(task_ptr->m_dir_handle,
                                    task_ptr->m_buffer,//<--FILE_NOTIFY_INFORMATION records are put into this buffer
                                    MC_READ_DIR_CHANGE_BUFFER_SIZE,
                                    task_ptr->m_monitor_sub_dir,
                                    task_ptr->m_filter_flags,
                                    &task_ptr->m_buffer_lenth,//this var not set when using asynchronous mechanisms...
                                    &task_ptr->m_overlapped,
                                    NULL))//no completion routine!
                                {
                                    task_ptr->m_error_code = ::pilo::EC_READ_DIR_CHANGE_ERROR;
                                    this->on_monitor_started(task_ptr->m_path, ::pilo::EC_READ_DIR_CHANGE_ERROR);
                                }
                                else
                                {
                                    task_ptr->m_state = ::pilo::core::fs::directory_monitor_task::eTS_Normal;
                                    task_ptr->m_error_code = ::pilo::EC_OK;
                                    this->on_monitor_started(task_ptr->m_path, ::pilo::EC_OK);
                                    task_ptr->signal_event();
                                }
                            }
                                break;

                            case pilo::core::fs::directory_monitor_task::eTS_Normal:
                            {
                                unsigned int solve_offset = 0UL;                                
                                _win32_directory_change_notification noti(task_ptr->m_buffer, MC_READ_DIR_CHANGE_BUFFER_SIZE);

                                if (noti.valid())
                                {
                                    do
                                    {
                                        unsigned int last_action = 0;
                                        solve_offset = 0UL;

                                        switch (noti.current_action())
                                        {
                                        case FILE_ACTION_ADDED:
                                        {
                                            char mb_buffer[MC_PATH_MAX] = { 0 };
                                            size_t n_sz = noti.get_filename(mb_buffer, sizeof(mb_buffer));
                                            this->on_file_added(task_ptr->m_path, mb_buffer, n_sz);
                                        }
                                            break;
                                        case FILE_ACTION_REMOVED:
                                        {
                                            char mb_buffer[MC_PATH_MAX] = { 0 };
                                            size_t n_sz = noti.get_filename(mb_buffer, sizeof(mb_buffer));
                                            this->on_file_removed(task_ptr->m_path, mb_buffer, n_sz);
                                        }
                                            break;

                                        case FILE_ACTION_MODIFIED:
                                        {
                                            char mb_buffer[MC_PATH_MAX] = { 0 };
                                            size_t n_sz = noti.get_filename(mb_buffer, sizeof(mb_buffer));
                                            this->on_file_modified(task_ptr->m_path, mb_buffer, n_sz);
                                        }
                                            break;

                                        case FILE_ACTION_RENAMED_OLD_NAME:
                                        {
                                            char mb_buffer[MC_PATH_MAX] = { 0 };
                                            size_t n_sz = noti.get_filename(mb_buffer, sizeof(mb_buffer));

                                            if (noti.solve_next())
                                            {
                                                M_ASSERT(noti.current_action() == FILE_ACTION_RENAMED_NEW_NAME);
                                                if (noti.current_action() == FILE_ACTION_RENAMED_NEW_NAME)
                                                {
                                                    char mb_neo_buffer[MC_PATH_MAX] = { 0 };
                                                    size_t n_nsz = noti.get_filename(mb_neo_buffer, sizeof(mb_neo_buffer));
                                                    this->on_file_renamed(task_ptr->m_path, mb_buffer, n_sz, mb_neo_buffer, n_nsz);
                                                }
                                            }
                                            else
                                            {
                                                unsigned int ret_offset = noti.move_record_to_beginning();
                                                if (ret_offset == (unsigned int)-1)
                                                {
                                                    this->on_file_renamed(task_ptr->m_path, mb_buffer, n_sz, nullptr, 0);
                                                }
                                                else
                                                {
                                                    solve_offset = ret_offset;
                                                }
                                            }
                                        }
                                            break;

                                        case FILE_ACTION_RENAMED_NEW_NAME:
                                        {
                                            M_ASSERT(last_action == FILE_ACTION_RENAMED_OLD_NAME);
                                        }
                                            break;

                                        default:
                                            break;
                                        } // end of internal switch block

                                        last_action = noti.current_action();

                                    } while (noti.solve_next());

                                }  //end of normal                 

                                if (!ReadDirectoryChangesW(task_ptr->m_dir_handle,
                                    task_ptr->m_buffer + solve_offset,//<--FILE_NOTIFY_INFORMATION records are put into this buffer 
                                    MC_READ_DIR_CHANGE_BUFFER_SIZE - solve_offset,
                                    task_ptr->m_monitor_sub_dir,
                                    task_ptr->m_filter_flags,
                                    &task_ptr->m_buffer_lenth,//this var not set when using asynchronous mechanisms...
                                    &task_ptr->m_overlapped,
                                    NULL))//no completion routine!
                                {
                                    int nOldThreadPriority = GetThreadPriority(GetCurrentThread());
                                    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
                                    try{
                                        task_ptr->m_error_code = ::pilo::EC_READ_DIR_CHANGE_ERROR;
                                        this->on_error(task_ptr->m_path, task_ptr->m_error_code, GetLastError());

                                        //Cancel the problematic task
                                        _cancel_task_on_error(task_ptr);                       

                                    }
                                    catch (...)
                                    {
                                        //just in case of exception, this thread will be set back to 
                                        //normal priority.
                                    }
                                    //
                                    //	Set the thread priority back to normal.
                                    //
                                    SetThreadPriority(GetCurrentThread(), nOldThreadPriority);
                                }


                            }
                                break;

                            case pilo::core::fs::directory_monitor_task::eTS_Stop:
                            {
                                if (task_ptr->m_dir_handle != INVALID_HANDLE_VALUE)
                                {
                                    task_ptr->close_dir_handle();
                                    task_ptr->m_state = ::pilo::core::fs::directory_monitor_task::eTS_Stopping;
                                    this->on_monitor_stopped(task_ptr->m_path, ::pilo::EC_OK);
                                }
                                else
                                {
                                    this->on_monitor_stopped(task_ptr->m_path, ::pilo::EC_INVALID_OBJECT);
                                    task_ptr->signal_event();                                    
                                }
                            }
                                break;

                            case pilo::core::fs::directory_monitor_task::eTS_Stopping:
                            {
                                if (task_ptr->m_dir_handle == INVALID_HANDLE_VALUE)
                                {
                                    task_ptr->signal_event();
                                }
                                else
                                {
                                     task_ptr->close_dir_handle();
                                }
                            }


                            break;

                            default:
                                break;
                        }

                    }                        
                    else
                    {
                        return ::pilo::EC_USER_CANCELED;
                    }
             

                } //end of do
                while (true);


                return ::pilo::EC_OK;
            }

            void directory_change_monitor::dump_console()
            {
                ::pilo::core::threading::advanced_mutex_locker<::pilo::core::threading::recursive_mutex> locker(_m_tasks_mutex);

                ::pilo::core::io::console_format_output("dump tasks:\n");
                directory_monitor_task * p = _m_tasks.front();
                for (; p != _m_tasks.end(); p = _m_tasks.next(p))
                {
                    ::pilo::core::io::console_format_output("\t%s\n", p->to_string().c_str());
                }

            }

            directory_change_monitor::directory_change_monitor()
            {
                m_completion_port = NULL;
                
#ifdef WINDOWS
                ::pilo::error_number_t error = pilo::EC_OK;
                for (int i = ::pilo::core::process::process_util::ePPN_Backup; i < ::pilo::core::process::process_util::ePPN_Count; i++)
                {
                    error = ::pilo::core::process::process_util::set_current_process_privilege((::pilo::core::process::process_util::EnumProcessPrivilegeName)i, true);
                    if (error != ::pilo::EC_OK)
                    {
                        M_ASSERT(false);
                    }
                }                   

#endif
            }

            directory_change_monitor::~directory_change_monitor()
            {
                if (!_m_is_detached)
                {
                    _shutdown_service_no_mts();
                }
                else
                {
                    this->kill();                    

                    if (m_completion_port == NULL)
                    {
                        ::CloseHandle(m_completion_port);
                        m_completion_port = NULL;
                    }
                }

                directory_monitor_task * p = nullptr;
                while ((p = _m_tasks.pop_front()) != nullptr)
                {
                    MP_SAFE_DELETE(p);
                }
            }

            ::pilo::error_number_t directory_change_monitor::start_task(const char* dirname, ::pilo::i32_t filter_flags, bool monitor_sub_dir /*= false*/)
            {
                M_UNUSED(monitor_sub_dir);


                M_ASSERT(filter_flags != 0);
                MP_CHECK_EMPTY_CSTR_RET(dirname, ::pilo::EC_NULL_PARAM);
                MP_CHECK_EQUAL_VALUE_RET(filter_flags, 0, ::pilo::EC_OUT_OF_RANGE);

                ::pilo::core::fs::path_string<0> dirpath;
                if (!dirpath.assign_path_string(dirname))
                {
                    return ::pilo::EC_INVALID_PATH;
                }

                if (dirpath.to_absolute(false, true) != ::pilo::EC_OK)
                {
                    return ::pilo::EC_INVALID_PATH;
                }

                //ensure this is valid dir
                if (::pilo::core::fs::fs_util::calculate_type(dirpath.c_str()) != ::pilo::core::fs::fs_util::eFSNT_Directory)
                { 
                    return ::pilo::EC_DIR_NOT_EXIST;
                }                

                directory_monitor_task* task_ptr = new directory_monitor_task;
                task_ptr->m_monitor_sub_dir = monitor_sub_dir;
                task_ptr->m_filter_flags = filter_flags;
                task_ptr->m_state = directory_monitor_task::eTS_NotSet;
                ::pilo::core::string::string_util::copy(task_ptr->m_path, MC_PATH_MAX, dirpath.c_str(), MC_INVALID_SIZE);

                //create dir handle to monitor on
                task_ptr->m_dir_handle = CreateFile(task_ptr->m_path,
                    FILE_LIST_DIRECTORY,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,//| FILE_SHARE_DELETE, <-- removing FILE_SHARE_DELETE prevents the user or someone else from renaming or deleting the watched directory. This is a good thing to prevent.
                    NULL, //security attributes
                    OPEN_EXISTING,
                    FILE_FLAG_BACKUP_SEMANTICS | //<- the required priviliges for this flag are: SE_BACKUP_NAME and SE_RESTORE_NAME.  CPrivilegeEnabler takes care of that.
                    FILE_FLAG_OVERLAPPED, //OVERLAPPED!
                    NULL);
                if (task_ptr->m_dir_handle == INVALID_HANDLE_VALUE)
                {
                    MP_SAFE_DELETE(task_ptr);
                    return MAKE_SYSERR(::pilo::EC_CREATE_DIR_ERROR);
                }
                
                {   // need protect by mutex
                    ::pilo::core::threading::advanced_mutex_locker<::pilo::core::threading::recursive_mutex> locker(_m_tasks_mutex);

                    if (nullptr != _find_task_no_mts(task_ptr->m_path))
                    {
                        MP_SAFE_DELETE(task_ptr);
                        return ::pilo::EC_DIR_ALREAD_EXIST;
                    }
                    
                    //working asyn
                    m_completion_port = CreateIoCompletionPort(task_ptr->m_dir_handle,
                        m_completion_port, //if m_hCompPort is NULL, hDir is associated with a NEW completion port,
                        //if m_hCompPort is NON-NULL, hDir is associated with the existing completion port that the handle m_hCompPort references
                        (ULONG_PTR)task_ptr, //the completion 'key'... this ptr is returned from GetQueuedCompletionStatus() when one of the events in the dwChangesToWatchFor filter takes place
                        0);
                    if (m_completion_port == NULL)
                    {
                        MP_SAFE_DELETE(task_ptr);
                        return MAKE_SYSERR(::pilo::EC_CREATE_DIR_ERROR);
                    }                    
                }    

                //check if the monitor thread has wake up.
                if (this->state() != ::pilo::core::threading::eRunning)
                {
                    ::pilo::error_number_t exec_ok = this->execute();
                    if (exec_ok != ::pilo::EC_OK)
                    {
                        MP_SAFE_DELETE(task_ptr);
                        return ::pilo::EC_START_THREAD_FAILED;
                    }
                }

                {
                    ::pilo::core::threading::advanced_mutex_locker<::pilo::core::threading::recursive_mutex> locker(task_ptr->m_mutex);
                    task_ptr->m_state = directory_monitor_task::eTS_StartMonitoring; 
                    PostQueuedCompletionStatus(m_completion_port, sizeof(task_ptr), (ULONG_PTR) task_ptr, &task_ptr->m_overlapped);
                } 

                DWORD dwWait = 0;
                do{
                    dwWait = task_ptr->wait_event();
                    if (dwWait != WAIT_OBJECT_0)
                    {
                        this->on_error(task_ptr->m_path, ::pilo::EC_WAIT_MONITOR_DIR_TASK_START_ERROR, GetLastError());
                    }
                } while (dwWait != WAIT_OBJECT_0);
                
                task_ptr->reset_startstrop_event();

                if (task_ptr->m_error_code != ::pilo::EC_OK)
                {
                    return pilo::EC_READ_DIR_CHANGE_ERROR;
                }

                {   // need protect by mutex
                    ::pilo::core::threading::advanced_mutex_locker<::pilo::core::threading::recursive_mutex> locker(_m_tasks_mutex);
                    _m_tasks.push_back(task_ptr);
                }

                return ::pilo::EC_OK;
            }            

            pilo::error_number_t directory_change_monitor::stop_task(const char* dirname)
            {
                MP_CHECK_EMPTY_CSTR_RET(dirname, ::pilo::EC_NULL_PARAM);
                if (m_completion_port == NULL) return ::pilo::EC_NONSENSE_OPERATION;

                directory_monitor_task* task_ptr = nullptr;

                ::pilo::core::fs::path_string<0> dirpath;
                if (!dirpath.assign_path_string(dirname))
                {
                    return ::pilo::EC_INVALID_PATH;
                }

                if (dirpath.to_absolute(false, true) != ::pilo::EC_OK)
                {
                    return ::pilo::EC_INVALID_PATH;
                }

                {
                    ::pilo::core::threading::mutex_locker<::pilo::core::threading::recursive_mutex> locker(_m_tasks_mutex);
                    task_ptr = _find_task_no_mts(dirpath.c_str());
                    if (task_ptr == nullptr)
                    {
                        return ::pilo::EC_DIR_NOT_EXIST;
                    }

                    ::pilo::error_number_t ret = _stop_task_no_mts(task_ptr);   
                    if (ret == ::pilo::EC_OK)
                    {
                        _delete_task_no_mts(task_ptr);
                        MP_SAFE_DELETE(task_ptr);
                        return ret;
                    }
                    
                    
                } 

                return ::pilo::EC_UNDEFINED;

            }

            pilo::error_number_t directory_change_monitor::shutdown_service(size_t timeout_ticks)
            {
                MP_CHECK_EQUAL_VALUE_RET(_m_thread_handle, NULL, ::pilo::EC_NULL_PARAM);
                ::pilo::core::threading::advanced_mutex_locker<::pilo::core::threading::recursive_mutex> locker(_m_tasks_mutex);
                return _shutdown_service_no_mts(timeout_ticks);

            }

            pilo::error_number_t directory_change_monitor::_shutdown_service_no_mts(size_t timeout_ticks)
            {
                MP_CHECK_EQUAL_VALUE_RET(_m_thread_handle, NULL, ::pilo::EC_NULL_PARAM);

                directory_monitor_task * task_ptr = nullptr;
                while ((task_ptr = _m_tasks.pop_front()) != nullptr)
                {
                    if (_stop_task_no_mts(task_ptr) == ::pilo::EC_OK)
                    {
                        MP_SAFE_DELETE(task_ptr);
                    }
                }

                pilo::error_number_t rc = ::pilo::EC_OK;
                PostQueuedCompletionStatus(m_completion_port, 0, 0, NULL);
                ::pilo::error_number_t wait_ret = this->wait(timeout_ticks);
                if (wait_ret != ::pilo::EC_OK)
                {
                    this->kill();
                    rc = ::pilo::EC_USER_ABORTED;
                }

                if (m_completion_port == NULL)
                {
                    ::CloseHandle(m_completion_port);
                    m_completion_port = NULL;
                }

                return rc;
            }

            directory_monitor_task* directory_change_monitor::_find_task_no_mts(const char* dirname)
            {
                directory_monitor_task * p = _m_tasks.front();
                for (; p != _m_tasks.end(); p = _m_tasks.next(p))
                {
                    if (::pilo::core::string::string_util::binary_compare(dirname, p->m_path) == 0)
                    {
                        return p;
                    }
                }

                return nullptr;
            }

            void directory_change_monitor::_delete_task_no_mts(directory_monitor_task* task_ptr)
            {
                _m_tasks.erase(task_ptr);
            }            

            ::pilo::error_number_t directory_change_monitor::_stop_task_no_mts(directory_monitor_task* task_ptr)
            {
                MP_CHECK_NULL_PTR_RET(task_ptr, ::pilo::EC_NULL_PARAM);

                BOOL bRetVal = FALSE;
                {
                    ::pilo::core::threading::mutex_locker<::pilo::core::threading::recursive_mutex> locker(task_ptr->m_mutex);
                    task_ptr->m_state = directory_monitor_task::eTS_Stop;
                    bRetVal = PostQueuedCompletionStatus(m_completion_port,
                                                         sizeof(task_ptr), (ULONG_PTR)task_ptr, &task_ptr->m_overlapped);
                    
                }

                if (!bRetVal)
                {
                    return ::pilo::EC_READ_DIR_CHANGE_ERROR;
                }

                DWORD dwWait = 0;
                do{
                    dwWait = task_ptr->wait_event();
                    if (dwWait != WAIT_OBJECT_0)
                    {
                        this->on_error(task_ptr->m_path, ::pilo::EC_WAIT_MONITOR_DIR_TASK_STOP_ERROR, GetLastError());
                    }
                } while (dwWait != WAIT_OBJECT_0);

                task_ptr->reset_startstrop_event();

                if (dwWait == WAIT_OBJECT_0)
                {
                    ::pilo::core::threading::mutex_locker<::pilo::core::threading::recursive_mutex> locker(task_ptr->m_mutex);         
                    return ::pilo::EC_OK;
                }

                return ::pilo::EC_READ_DIR_CHANGE_ERROR;                

            }

            ::pilo::error_number_t directory_change_monitor::_cancel_task_on_error(directory_monitor_task* task_ptr)
            {
                MP_CHECK_NULL_PTR_RET(task_ptr, ::pilo::EC_NULL_PARAM);

                ::pilo::core::threading::advanced_mutex_locker<::pilo::core::threading::recursive_mutex> locker(_m_tasks_mutex);

                _delete_task_no_mts(task_ptr);
                MP_SAFE_DELETE(task_ptr);

                return ::pilo::EC_OK;
            }

#else

#endif

        }
    }
}