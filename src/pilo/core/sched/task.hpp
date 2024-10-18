#ifndef _pilo_core_sched_task_hpp_
#define _pilo_core_sched_task_hpp_

#include "../../pilo.hpp"

namespace pilo
{
    namespace core
    {
        namespace sched
        {
            class task;
            typedef void (*task_func_type)(::pilo::core::sched::task* task_ptr);
            typedef void (*task_destructor_func_type)(::pilo::core::sched::task* task_ptr);

            class task
            {
            public:
                task() :_func(nullptr), _object(nullptr), _param(nullptr), _context(nullptr), _dtor(nullptr) {}
                ~task()
                {
                    if (nullptr != _dtor)
                        _dtor(this);
                }

                inline void operator()()
                {
                    PMC_ASSERT(_func != nullptr);
                    _func(this);
                }

                inline void set(task_func_type f_func, void* obj, void* param, void* ctx, task_destructor_func_type dtor)
                {
                    _func = f_func;
                    _object = obj;
                    _param = param;
                    _context = ctx;
                    _dtor = dtor;
                }

                inline void reset()
                {
                    if (nullptr != _dtor)
                        _dtor(this);
                }

                inline void exec()
                {
                    PMC_ASSERT(_func != nullptr);
                    _func(this);
                }

                inline void* object() { return _object; }
                inline void* param() { return _param; }
                inline task_func_type func() { return _func; }
                inline task_destructor_func_type dtor() { return _dtor;  }

                template<typename CTX>
                inline CTX* context() { return (CTX*)_context; }

            private:
                task_func_type                      _func;
                void* _object;
                void* _param;
                void* _context;
                task_destructor_func_type           _dtor;
            };
        }
    }
}

#endif // !_pilo_core_threading_thread_pool_interface_hpp_
