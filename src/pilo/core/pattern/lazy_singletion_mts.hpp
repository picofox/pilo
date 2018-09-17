#include "core/coredefs.hpp"
#include "core/threading/recursive_mutex.hpp"

namespace pilo
{
    namespace core
    {
        namespace pattern
        {
            template< class Type >
            class lazy_singletion_mts
            {
            public:
                static Type*			instance();
                static Type*			instance_no_mts();
                static void				destory_instance();
                static Type*			instance_pointer();
                static void				set_instance_pointer(Type* p);

            protected:
                lazy_singletion_mts();
                ~lazy_singletion_mts();

            private:
                static Type*					                _instance;
                static ::pilo::core::threading::recursive_mutex	_mutex;

            }; // end lazy_singletion_mts

            template< class Type >
            Type* pilo::core::pattern::lazy_singletion_mts<Type>::instance_no_mts()
            {
                if (nullptr == _instance)
                {
                    _instance = static_cast<Type*>(new Type());
                }
                return _instance;
            }

            template< class Type >
            void pilo::core::pattern::lazy_singletion_mts<Type>::set_instance_pointer(Type* p)
            {
                M_ASSERT(nullptr != p);
                M_ASSERT(_instance == nullptr);
                _instance = p;
            }

            template< class Type >
            Type* pilo::core::pattern::lazy_singletion_mts<Type>::instance_pointer()
            {
                return _instance;
            }

            template< class Type >
            void pilo::core::pattern::lazy_singletion_mts<Type>::destory_instance()
            {
                if (nullptr == _instance)
                {
                    _mutex.lock();
                    {
                        MP_SAFE_DELETE(_instance);
                    }
                    _mutex.unlock();
                }
            }

            template< class Type >
            Type* pilo::core::pattern::lazy_singletion_mts<Type>::instance()
            {
                if (nullptr == _instance)
                {
                    _mutex.lock();
                    {                        
                        if (nullptr == _instance)
                        {
                            _instance = static_cast<Type*>(new Type());
                        }                        
                    }  
                    _mutex.unlock();

                }
                return _instance;
            }

            template< class Type >
            pilo::core::pattern::lazy_singletion_mts<Type>::~lazy_singletion_mts()
            {
                _instance = nullptr;
            }

            template< class Type >
            pilo::core::pattern::lazy_singletion_mts<Type>::lazy_singletion_mts()
            {
                _instance = nullptr;
            }

            template< class Type >
            Type* lazy_singletion_mts<Type>::_instance = NULL;

            template< class Type >
            ::pilo::core::threading::recursive_mutex lazy_singletion_mts<Type>::_mutex;



        }
    }
}