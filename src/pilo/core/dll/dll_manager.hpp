#pragma once
#include "core/coredefs.hpp"
#include "dynamic_link_library.hpp"
#include "core/fs/path_string.hpp"
#include "core/threading/mutex_locker.hpp"
#include "core/threading//dummy_mutex.hpp"
#include <map>

namespace pilo
{
    namespace core
    {
        namespace dll
        {
            template<typename _Lock = pilo::core::threading::dummy_mutex>
            class dll_manager
            {
            public:
                typedef _Lock lock_type;

                dll_manager() {}
                ~dll_manager()
                {
                    std::map<std::string, dynamic_link_library*>::iterator it = m_dlls.begin();
                    for (; it != m_dlls.end(); it ++)
                    {
                        if (it->second != nullptr)
                        {
                            it->second.unload();
                            delete it->second;
                        }
                    }

                    m_dlls.clear();
                }

                dynamic_link_library* load(const char* name, ::pilo::u32_t flags)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);

                    if (name == nullptr) return nullptr;

                    std::string str = name;
                    std::map<std::string, dynamic_link_library*>::iterator it = m_dlls.end();
                    it = m_dlls.find(str);
                    if (it != m_dlls.end())
                    {
                        it->second->inc_ref_count();
                        return it->second;
                    }
                    
                    dynamic_link_library* dll_ptr = new dynamic_link_library;
                    if (::pilo::EC_OK != dll_ptr->load(name, flags))
                    {
                        return nullptr;
                    }
                    dll_ptr->inc_ref_count();
                    m_dlls.insert(std::pair<std::string, dynamic_link_library*>(str, dll_ptr));
                    return dll_ptr;
                }

                void unload(const char* name)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);

                    if (name == nullptr) return;
                    std::string str = name;

                    std::map<std::string, dynamic_link_library*>::iterator it = m_dlls.end();
                    it = m_dlls.find(str);
                    if (it == m_dlls.end())
                    {
                        return;
                    }

                    if (it->second->ref_count() == 0)
                    {
                        it->second->unload();
                        m_dlls.erase(it);
                        return;
                    }
                    else
                    {
                        it->second->dec_ref_count();
                        if (it->second->ref_count() == 0)
                        {
                            it->second->unload();
                            m_dlls.erase(it);
                            return;
                        }
                    }

                    return;
                }

                void unload(dynamic_link_library* dll)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);

                    if (name == nullptr) return;
                    
                    std::map<std::string, dynamic_link_library*>::iterator it = m_dlls.begin();
                    for (; it != m_dlls.end(); it ++)
                    {
                        if (it->second == dll)
                        {
                            if (it->second->ref_count() == 0)
                            {
                                it->second->unload();
                                m_dlls.erase(it);
                                return;
                            }
                            else
                            {
                                it->second->dec_ref_count();
                                if (it->second->ref_count() == 0)
                                {
                                    it->second->unload();
                                    m_dlls.erase(it);
                                    return;
                                }
                            }

                            return;
                        }
                    }
                }


            private:
                M_DISABLE_COPY(dll_manager);
                std::map<std::string, dynamic_link_library*> m_dlls;
                lock_type _m_lock;
            };
        }
    }
}

