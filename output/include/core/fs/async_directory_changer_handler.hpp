#pragma once
#include "core/coredefs.hpp"
namespace pilo
{
    namespace core
    {
        namespace fs
        {
            class directory_change_notification;
            class async_directory_changer_handler;

            class async_notifier
            {
            public:
                virtual ~async_notifier() {}
                virtual void post_notification(directory_change_notification * n_ptr) = 0;
            };

            class directory_change_notification
            {
            public:
                directory_change_notification();
            public:
                explicit directory_change_notification(async_directory_changer_handler * pDelayedHandler, ::pilo::u32_t dwPartialPathOffset);
                ~directory_change_notification();

                void on_file_added(const char* szFileName);
                void on_file_removed(const char* szFileName);
                void on_file_renamed(const char* szOldName, const char* szNewName);
                void on_file_modified(const char* szFileName);
                void on_read_directory_change_error(::pilo::u32_t dwError, const char* szDirectoryName);
                void on_monitor_started(::pilo::u32_t dwError, const char* szDirectoryName);
                void on_monitor_stopped(const char* szDirectoryName);

                void dispatch();

                enum EnumFunctionToDispatch{
                    eFTD_Invalid = -1,
                    eFTD_FileAdded = FILE_ACTION_ADDED,
                    eFTD_FileRemoved = FILE_ACTION_REMOVED,
                    eFTD_FileModified = FILE_ACTION_MODIFIED,
                    eFTD_FileRenamed = FILE_ACTION_RENAMED_OLD_NAME,
                    eFTD_ReadDirChangeError,
                    eFTD_MonitorStarted,
                    eOn_MonitorStopped
                };

            protected:
                void post_notification();

            private:
                friend class async_directory_changer_handler;
                async_directory_changer_handler * m_async_change_handler;

                EnumFunctionToDispatch m_function_to_dispatch;
                //Notification Data:

                //m_filename_param_1 is the szFileName parameter to On_FileAdded(),On_FileRemoved,On_FileModified(), 
                //and is szOldFileName to On_FileNameChanged(). 
                //    Is also strDirectoryName to On_ReadDirectoryChangesError(), On_WatchStarted(), and On_WatchStopped()
                const char *	m_filename_param_1;

                //m_filename_param_2 is the szNewFileName parameter to On_FileNameChanged()
                const char *	m_filename_param_2;

                //m_error is the dwError parameter to On_WatchStarted(), and On_ReadDirectoryChangesError()
                ::pilo::u32_t   m_error;	 
                
                //helps support FILTERS_CHECK_PARTIAL_PATH...not passed to any functions 
                //other than may be used during tests in CDelayedDirectoryChangeHandler::NotifyClientOfFileChange()
                ::pilo::u32_t m_partial_path_offset;

                friend class directory_change_notification;
                friend class directory_change_monitor;
                friend ::pilo::u32_t get_path_offset_by_filters(directory_change_notification*, ::pilo::u32_t);//a friend function

            };

            class async_directory_changer_handler
            {

            };

        }
    }
}