#include "core/coredefs.hpp"
#include "core/threading/basic_thread.hpp"
#include "core/threading/recursive_mutex.hpp"
#include "core/container/singly_linked_selflist.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/threading/sync_condition_variable_any.hpp"
#include <vector>

#define MC_DCM_FILENAME_CHANGE  (FILE_NOTIFY_CHANGE_FILE_NAME)
#define MC_DCM_FILECONTENT_CHANGE (FILE_NOTIFY_CHANGE_LAST_WRITE)


#define MC_DCM_ALL (MC_DCM_FILENAME_CHANGE |  MC_DCM_FILECONTENT_CHANGE)

//#define MC_DCM_MAX_WAIT_ERROR_COUNT (128)



namespace pilo
{
    namespace core
    {
        namespace fs
        {
            class directory_change_monitor;

#ifdef WINDOWS

            class _win32_directory_change_notification
            {
            public:
                _win32_directory_change_notification(char* buffer_ptr, unsigned int len)
                    : m_buffer_ptr(buffer_ptr), m_buffer_length(len)
                {
                    m_record_ptr = (PFILE_NOTIFY_INFORMATION) m_buffer_ptr;
                }

                ~_win32_directory_change_notification()
                {
                    m_buffer_ptr = nullptr;
                    m_buffer_length = 0;
                    m_record_ptr = nullptr;
                }

                unsigned int move_record_to_beginning()
                {
                    if (!valid()) return ((unsigned int) -1);

                    unsigned int ret_size = sizeof(FILE_NOTIFY_INFORMATION);
                    WCHAR FileName[1];//same as is defined for FILE_NOTIFY_INFORMATION::FileName
                    UNREFERENCED_PARAMETER(FileName);
                    ret_size -= sizeof(FileName);
                    //and replace it w/ value of FILE_NOTIFY_INFORMATION::FileNameLength
                    ret_size += m_record_ptr->FileNameLength;

                    M_ASSERT(((char*)m_record_ptr + ret_size) <= m_buffer_ptr + m_buffer_length);
                    if (((char*)m_record_ptr + ret_size) > m_buffer_ptr + m_buffer_length)
                    {
                        return (unsigned int) -1;
                    }

                    M_ASSERT((void*)m_buffer_ptr != (void*)m_record_ptr);//if this is the case, your buffer is way too small
                    if ((void*)m_buffer_ptr != (void*)m_record_ptr)
                    {
                        return (unsigned int) -1;
                    }

                    if ((void*)m_buffer_ptr != (void*)m_record_ptr)
                    {//copy the m_pCurrentRecord to the beginning of m_pBuffer
                        __try{
                            memmove(m_buffer_ptr, m_record_ptr, ret_size);
                            return ret_size;
                        }
                        __except (EXCEPTION_EXECUTE_HANDLER)
                        {
                            return (unsigned int)-1;
                        }
                    }

                    return (unsigned int)-1;

                }

                bool valid() const
                {
                    if (m_buffer_ptr == nullptr || m_buffer_length <= 0) return false;
                    if (m_record_ptr == nullptr) return false;
                    return true;
                }

                unsigned int current_action()
                {
                    M_ASSERT(m_record_ptr);
                    if (!valid()) return 0UL;

                    if (m_record_ptr)
                        return m_record_ptr->Action;
                    return 0UL;
                }

                size_t get_filename(char* mb_buffer, int len) const
                {
                    if (! valid())
                    {
                        return 0;
                    }
             
                    ::WideCharToMultiByte(CP_ACP, 0, 
                        m_record_ptr->FileName, 
                        m_record_ptr->FileNameLength, 
                        mb_buffer, len, NULL, NULL);

                    return m_record_ptr->FileNameLength/2;
                }

                bool solve_next()
                {
                    if (m_record_ptr
                        &&	m_record_ptr->NextEntryOffset != 0UL)//is there another record after this one?
                    {
                        //set the current record to point to the 'next' record
                        PFILE_NOTIFY_INFORMATION pOld = m_record_ptr;
                        m_record_ptr = (PFILE_NOTIFY_INFORMATION)((LPBYTE)m_record_ptr + m_record_ptr->NextEntryOffset);

                        M_ASSERT((unsigned int)((const char*)m_record_ptr - m_buffer_ptr) < m_buffer_length);//make sure we haven't gone too far

                        if ((unsigned int)((char*)m_record_ptr - m_buffer_ptr) > m_buffer_length)
                        {
                            //we've gone too far.... this data is hosed.
                            //
                            // This sometimes happens if the watched directory becomes deleted... remove the FILE_SHARE_DELETE flag when using CreateFile() to get the handle to the directory...
                            m_record_ptr = pOld;
                        }

                        return (bool)(m_record_ptr != pOld);
                    }
                    return false;
                }

            protected:
                char * m_buffer_ptr;//<--all of the FILE_NOTIFY_INFORMATION records 'live' in the buffer this points to...
                unsigned int  m_buffer_length;
                PFILE_NOTIFY_INFORMATION m_record_ptr;//this points to the current FILE_NOTIFY_INFORMATION record in m_pBuffer
            };

            class directory_monitor_task
            {
            public:
                typedef ::pilo::core::string::fixed_astring<MC_PATH_MAX + 256> format_string_type;

            public:
                enum EnumTaskState
                {
                    eTS_NotSet,
                    eTS_StartMonitoring,
                    eTS_Stop,
                    eTS_Stopping,
                    eTS_Stopped,
                    eTS_Normal,
                };

            public:
                directory_monitor_task()
                {
                    m_path[0] = 0;
                    m_filter_flags = 0;
                    m_monitor_sub_dir = false;
                    m_dir_handle = INVALID_HANDLE_VALUE;
                    m_buffer_lenth = 0;
                    m_state = eTS_NotSet;
                    m_error_code = 0;
                    m_startstop_event = CreateEvent(NULL, TRUE, FALSE, NULL);
                }
                ~directory_monitor_task()
                {
                    if (m_startstop_event != NULL)
                    {
                        ::CloseHandle(m_startstop_event);
                        m_startstop_event = NULL;
                    }

                    if (m_dir_handle != INVALID_HANDLE_VALUE)
                    {
                        ::CloseHandle(m_dir_handle);
                        m_dir_handle = INVALID_HANDLE_VALUE;
                    }
                }

                DWORD wait_event(DWORD timeout = 10 * 1000)
                {
                    return WaitForSingleObject(m_startstop_event, timeout);
                }

                void signal_event()
                {
                    SetEvent(m_startstop_event);
                }

                BOOL close_dir_handle()
                {
                    BOOL b = FALSE;
                    if (m_dir_handle != INVALID_HANDLE_VALUE)
                    {
                        b = ::CloseHandle(m_dir_handle);
                        m_dir_handle = INVALID_HANDLE_VALUE;
                    }
                    return b;
                }

                void reset_startstrop_event()
                {
                    ResetEvent(m_startstop_event);
                }

                bool valid() const
                {
                    if (m_dir_handle == INVALID_HANDLE_VALUE)
                    {
                        return false;
                    }

                    return true;
                }

                ::pilo::i32_t                                       m_filter_flags;
                bool                                                m_monitor_sub_dir;
                HANDLE                                              m_dir_handle;
                HANDLE                                              m_startstop_event;
                char                                                m_path[MC_PATH_MAX];
                OVERLAPPED                                          m_overlapped;
                EnumTaskState                                       m_state;
                ::pilo::core::threading::recursive_mutex            m_mutex;
                char                                                m_buffer[MC_READ_DIR_CHANGE_BUFFER_SIZE];
                DWORD                                               m_buffer_lenth;
                ::pilo::error_number_t                              m_error_code;
                directory_monitor_task*                             m_next;

                format_string_type to_string() const
                {
                    format_string_type str;
                    str.format("task_%p f=(%x) fd=(%d) st=(%d) f=<%s>", this, m_filter_flags, m_dir_handle, m_state, m_path);
                    return str;
                }

            };

            class directory_change_monitor : public ::pilo::core::threading::basic_thread
            {
                public:
                friend class directory_monitor_task;
                public:
                directory_change_monitor();
                virtual ~directory_change_monitor();

                pilo::error_number_t start_task(const char* dirname,
                                                ::pilo::i32_t filter_flags,
                                                bool monitor_sub_dir = false);


                pilo::error_number_t stop_task(const char* dirname);

                pilo::error_number_t shutdown_service(size_t timeout_ticks = MC_INVALID_SIZE);


                virtual i32_t on_run();
                void dump_console();

                public: //mothods to overload
                virtual i32_t on_error(const char* dir_pathname, ::pilo::error_number_t err_code, ::pilo::os_error_number_t os_err_code) { M_UNUSED(dir_pathname) M_UNUSED(err_code); M_UNUSED(os_err_code); return ::pilo::EC_OK; }
                virtual i32_t on_monitor_started(const char* dir_pathname, ::pilo::error_number_t err_code) { M_UNUSED(dir_pathname); M_UNUSED(err_code); return ::pilo::EC_OK; }
                virtual i32_t on_monitor_stopped(const char* dir_pathname, ::pilo::error_number_t err_code) { M_UNUSED(dir_pathname); M_UNUSED(err_code); return ::pilo::EC_OK; }
                virtual i32_t on_file_added(const char* dir_pathname, const char* filename_ptr, size_t filename_length) { M_UNUSED(dir_pathname) M_UNUSED(filename_ptr); M_UNUSED(filename_length); return ::pilo::EC_OK; }
                virtual i32_t on_file_removed(const char* dir_pathname, const char* filename_ptr, size_t filename_length) { M_UNUSED(dir_pathname) M_UNUSED(filename_ptr); M_UNUSED(filename_length); return ::pilo::EC_OK; }
                virtual i32_t on_file_modified(const char* dir_pathname, const char* filename_ptr, size_t filename_length) { M_UNUSED(dir_pathname) M_UNUSED(filename_ptr); M_UNUSED(filename_length); return ::pilo::EC_OK; }
                virtual i32_t on_file_renamed(const char* dir_pathname, const char* old_filename_ptr, size_t old_filename_length, const char* neo_filename_ptr, size_t neo_filename_length) { M_UNUSED(dir_pathname) M_UNUSED(old_filename_ptr); M_UNUSED(old_filename_length); M_UNUSED(neo_filename_ptr); M_UNUSED(neo_filename_length);  return ::pilo::EC_OK; }
                protected:
                mutable ::pilo::core::threading::recursive_mutex                        _m_tasks_mutex;
                ::pilo::core::container::singly_linked_selflist<directory_monitor_task> _m_tasks;
                HANDLE                                                                  m_completion_port;	//io completion port                

                protected: //methods
                directory_monitor_task* _find_task_no_mts(const char* dirname);
                void _delete_task_no_mts(directory_monitor_task* task_ptr);
                ::pilo::error_number_t _stop_task_no_mts(directory_monitor_task* task_ptr);
                pilo::error_number_t _shutdown_service_no_mts(size_t timeout_ticks = MC_INVALID_SIZE);
                ::pilo::error_number_t _cancel_task_on_error(directory_monitor_task* task_ptr);
            };
#else

#endif     
            
        }
    }
}