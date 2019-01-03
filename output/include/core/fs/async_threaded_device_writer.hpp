#pragma once
#include "core/coredefs.hpp"
#include "core/threading/basic_thread.hpp"
#include "core/threading/read_write_lock.hpp"
#include "core/threading/rw_mutex_r_locker.hpp"
#include "core/threading/rw_mutex_w_locker.hpp"

#define MB_IO_DEV_OPEN_FLAG_APPEND    (1<<0)
#define MB_IO_DEV_OPEN_SYNC           (1<<1)
#define MB_IO_DEV_OPEN_NO_OS_CACHE    (1<<2)
#define MB_IO_DEV_OPEN_REOPEN         (1<<3)



#define MB_IO_DEV_INIT_FLAG_AUTO_CREATE         (1<<0)
#define MB_IO_DEV_INIT_FLAG_FORCE_DELETE_DIR    (1<<1)
#define MB_IO_DEV_INIT_FLAG_FORCE_DELETE_FILE   (1<<2)
#define MB_IO_DEV_INIT_FLAG_AUTO_DELETE         (1<<3)
#define MB_IO_DEV_INIT_FLAG_AUTO_DELETE_ON_FINALIZE         (1<<4)


namespace pilo
{
    namespace core
    {
        namespace fs
        {

            class __pilointernal_async_threaded_device_writer_thread : public ::pilo::core::threading::basic_thread
            {
            public:
                typedef ::pilo::core::threading::read_write_lock lock_type;

            public:
                __pilointernal_async_threaded_device_writer_thread()
                {
                    _m_lock_ptr = nullptr;
                }

                ~__pilointernal_async_threaded_device_writer_thread()
                {

                }

                

                virtual pilo::i32_t on_run();

                lock_type* _m_lock_ptr;


            };

            template<typedef _DEV_TYPE>
            class async_threaded_device_writer
            {
            public:
                typedef _DEV_TYPE device_type;
                typedef ::pilo::core::threading::read_write_lock lock_type;

            public:
                ::pilo::error_number_t request_data_enqueue(void* buffer, size_t len, bool flush_now);
                ::pilo::error_number_t request_data_flush();
                ::pilo::error_number_t stop_service();

            protected:
                device_type                 _m_dev;
                lock_type                   _m_lock;
                __pilointernal_async_threaded_device_writer_thread _m_writer;

            private:
                M_DISABLE_COPY  
            };
        }
    }
}
