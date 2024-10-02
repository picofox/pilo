#ifndef _pilo_core_threading_task_hpp_
#define _pilo_core_threading_task_hpp_

#include    "./tlv.hpp"


#define PMB_TASK_DEFAULT_FLAG         ((::pilo::u8_t) 0x0)
#define PMB_KEEP_TASK_AFTER_INVOKE    ((::pilo::u8_t) 0x1)
#define PMB_KEEP_TLVP_ON_TASK_DEL     ((::pilo::u8_t) 0x2)


namespace pilo
{

    typedef void (*thread_callback_func_type)(void* gen_obj_ptr, ::pilo::tlv* in_param);
    typedef void(*object_dealloc_func_type)(void* gen_obj_ptr);

    class task
    {
    public:
        task() :_func(nullptr),_object(nullptr), _param(nullptr), _object_dealloc(nullptr){}
        ~task()
        {
            reset();
        }

        inline void operator()() const
        {
            exec();
        }

        inline void set(thread_callback_func_type f_func, void * obj, ::pilo::tlv* param, object_dealloc_func_type d_func, ::pilo::i8_t flag = PMB_TASK_DEFAULT_FLAG)
        {
            _func = f_func;
            _object = obj;
            _param = param;
            _object_dealloc = d_func;
            _flags.set(flag);
        }

        inline void reset()
        {
            if (_object != nullptr && _object_dealloc != nullptr) {
                _object_dealloc(_object);
            }
            if (_param != nullptr) {
                if (! _flags.test_value(PMB_KEEP_TLVP_ON_TASK_DEL)) {
                    ::pilo::tlv::deallocate(_param);
                }
            }
        }

        inline void exec() const
        {
            PMC_ASSERT(_func != nullptr);
            _func(_object, _param);
        }

        inline bool keep_task_after_invoke() const { return _flags.test_value(PMB_KEEP_TASK_AFTER_INVOKE);  }
        inline bool keep_tlvp_on_task_del() const { return _flags.test_value(PMB_KEEP_TASK_AFTER_INVOKE); }
        inline void set_keep_task_after_invoke(bool v) 
        {
            if (v)
                _flags.mark_value(PMB_KEEP_TASK_AFTER_INVOKE);
            else
                _flags.clear_value(PMB_KEEP_TASK_AFTER_INVOKE);
        }
        inline void set_keep_tlvp_on_task_del(bool v)
        {
            if (v)
                _flags.mark_value(PMB_KEEP_TLVP_ON_TASK_DEL);
            else
                _flags.clear_value(PMB_KEEP_TLVP_ON_TASK_DEL);
        }


    private:
        thread_callback_func_type           _func;
        void*                               _object;
        ::pilo::tlv*                        _param;
        object_dealloc_func_type            _object_dealloc;
        ::pilo::bit_flag<::pilo::u8_t>      _flags;
    };
   
    
}

#endif // !_pilo_core_threading_thread_pool_interface_hpp_
