#ifndef _pilo_task_hpp_
#define _pilo_task_hpp_

#include    "./tlv.hpp"


namespace pilo
{
    class task;
    typedef void (*task_func_type)(::pilo::task *task_ptr);
    typedef void (*task_destructor_func_type)(::pilo::task* task_ptr);

    class task
    {
    public:
        task() :_func(nullptr),_object(nullptr), _param(nullptr), _dtor(nullptr) {}
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

        inline void set(task_func_type f_func, void * obj, ::pilo::tlv* param, task_destructor_func_type dtor)
        {
            _func = f_func;
            _object = obj;
            _param = param;
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

        inline void* object() { return _object;  }
        inline ::pilo::tlv* param() { return _param;  }

    private:
        task_func_type                      _func;
        void*                               _object;
        ::pilo::tlv*                        _param;
        task_destructor_func_type           _dtor;
    };
   
    
}

#endif // !_pilo_core_threading_thread_pool_interface_hpp_
