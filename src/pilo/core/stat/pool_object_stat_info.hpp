#ifndef __pilo_core_stat_object_stat_
#define __pilo_core_stat_object_stat_

#include <string>
#include <map>
#include <functional>
#include "../../pilo.hpp"
#include "../string/string_operation.hpp"

namespace pilo
{
    namespace core
    {
        namespace stat
        {
            class pool_object_stat_manager
            {
            public:
                enum class pool_object_key_code : pilo::i16_t
                {
                    key_tlv = -1,
                    linked_buffer_node_4k = -2,
                };

            public:
                class stat_item;
                typedef ::pilo::err_t (*pool_object_stat_update_func_type)(pool_object_key_code key_code, stat_item * si);
                class stat_item
                {
                public:
                    stat_item() : _total_count(0), _free_count(0), _unit_size(0), _updater(0), _name("*") {}
                    stat_item(::pilo::i64_t unit_size, pool_object_stat_update_func_type updater, const char* name)
                        : _total_count(0), _free_count(0), _unit_size(unit_size), _updater(updater), _name(name) {}

                public:
                    inline void set(::pilo::i64_t total, ::pilo::i64_t avail)
                    {
                        _total_count = total;
                        _free_count = avail;
                    }
                    inline const std::string& name() const { return _name;  }
                    inline ::pilo::i64_t total_count() const { return _total_count; }
                    inline ::pilo::i64_t unit_size() const { return _unit_size; }
                    inline ::pilo::i64_t object_size() const { return _unit_size - (::pilo::i64_t)sizeof(void*); }
                    inline ::pilo::i64_t free_count() const { return _free_count; }
                    inline ::pilo::i64_t used_count() const { return _total_count - _free_count; }
                    inline ::pilo::i64_t total_bytes() const { return _total_count * static_cast<::pilo::i64_t>(_unit_size); }
                    inline ::pilo::i64_t free_bytes() const { return _free_count * static_cast<::pilo::i64_t>(_unit_size); }
                    inline ::pilo::i64_t used_bytes() const { return used_count() * static_cast<::pilo::i64_t>(_unit_size); }
                    inline ::pilo::err_t update(pool_object_key_code key_code)
                    { 
                        if (_updater != nullptr)
                        {
                            return _updater(key_code, this);
                        }
                        return ::pilo::mk_perr( PERR_NON_EXIST);
                    }
                    inline void to_string(pool_object_key_code key_code, std::stringstream& ss) const
                    {
                        char total_bytes_cstr[32] = { 0 };
                        char free_bytes_cstr[32] = { 0 };
                        char used_bytes_cstr[32] = { 0 };
                        ::pilo::core::string::size_to_cstr(total_bytes_cstr, 32, total_bytes(), 2, "B");
                        ::pilo::core::string::size_to_cstr(free_bytes_cstr, 32, free_bytes(), 2, "B");
                        ::pilo::core::string::size_to_cstr(used_bytes_cstr, 32, used_bytes(), 2, "B");

                        ss << "POBJ-" << _name << " (" << (::pilo::i16_t)key_code << ") -> "
                            << "size:" << _unit_size << " (" << object_size() << "), "
                            << "total:" << _total_count << " (" << total_bytes_cstr << "), "
                            << "freed:" << _free_count << " (" << free_bytes_cstr << "), "
                            << "used:" << used_count() << " (" << used_bytes_cstr << ")"
                            << std::endl;
                    }
                    


                private:
                    ::pilo::i64_t _total_count;
                    ::pilo::i64_t _free_count;
                    ::pilo::i64_t _unit_size;
                    pool_object_stat_update_func_type _updater;
                    std::string _name;
                };

            public:
                ::pilo::err_t register_item(pool_object_key_code key_code, ::pilo::i64_t object_size, pool_object_stat_update_func_type updater, const char* name)
                {
                    stat_item si(object_size + (::pilo::i64_t)(sizeof(void*)), updater, name);
                    std::map<pool_object_key_code, stat_item>::const_iterator cit = _items.find(key_code);
                    if (cit != _items.cend())
                    {
                        return ::pilo::mk_perr(PERR_EXIST);
                    }
                    _items.insert(std::pair<pool_object_key_code, stat_item>(key_code, si));
                    return PILO_OK;
                }

                ::pilo::err_t unregister_item(pool_object_key_code key_code)
                {
                    std::map<pool_object_key_code, stat_item>::const_iterator cit = _items.find(key_code);
                    if (cit != _items.cend())
                    {
                        _items.erase(cit);
                        return PILO_OK;
                    }
                    return ::pilo::mk_perr( PERR_NON_EXIST);
                }

                const stat_item& get(pool_object_key_code key_code, bool update) 
                {
                    std::map<pool_object_key_code, stat_item>::iterator it = _items.find(key_code);
                    if (it == _items.cend())
                    {
                        return ::pilo::_type_default_value_<stat_item>::get();
                    }

                    if (update)
                    {
                        (it->second).update(it->first);
                    }
                    return it->second;
                }                                

                inline void update_all()
                {
                    std::map<pool_object_key_code, stat_item>::iterator it = _items.begin();
                    for (; it != _items.end(); it++)
                    {
                        it->second.update(it->first);
                    }    
                }

                inline std::string to_string() const
                {
                    std::stringstream ss;
                    std::map<pool_object_key_code, stat_item>::const_iterator cit = _items.cbegin();
                    for ( ; cit != _items.cend(); cit++)
                    {                        
                        cit->second.to_string(cit->first, ss);
                    }
                    return ss.str();
                }

                inline std::string to_updated_string()
                {
                    update_all();
                    return to_string();
                }


            private:
                std::map<pool_object_key_code, stat_item> _items;
            };
        }
    }
}

#endif //__pilo_core_stat_object_stat_