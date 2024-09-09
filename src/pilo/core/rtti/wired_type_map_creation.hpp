#pragma once

#include "../../pilo.hpp"
#include "../string/string_operation.hpp"
#include <map>
#include <algorithm>
#include "./wired_type_deleter.hpp"
#include <type_traits>

namespace pilo
{
	class tlv;
	namespace core
	{
		namespace rtti
		{



			template<typename TA_KEY, typename TA_VALUE> inline ::pilo::err_t map_ptr_insert(char* &map_char_ptr
				, std::initializer_list<std::pair<TA_KEY, TA_VALUE>> list, bool is_force)
			{
				std::map<TA_KEY, TA_VALUE>* &map_ptr = (std::map<TA_KEY, TA_VALUE>*&) map_char_ptr;
				if (map_ptr == nullptr)
				{
					map_ptr = new std::map<TA_KEY, TA_VALUE>;
					if (map_ptr == nullptr)
						return ::pilo::mk_perr(PERR_NULL_PARAM);
				}
				for (std::pair<TA_KEY, TA_VALUE> kvpair : list)
				{
					typename std::map<TA_KEY, TA_VALUE>::iterator it = map_ptr->find(kvpair.first);
					if (it != map_ptr->end())
					{
						if (! is_force)
						{
							return ::pilo::mk_perr(PERR_EXIST);
						}
						map_ptr->erase(it);
					}
					map_ptr->insert(kvpair);
				}
				return PILO_OK;
			}

			template<typename TA_KEY> inline ::pilo::err_t map_ptr_insert(char* &map_char_ptr, std::initializer_list<std::pair<TA_KEY, const char*>> list, bool is_force)
			{
				std::map<TA_KEY, char*>*& map_ptr = (std::map<TA_KEY, char*>*&) map_char_ptr;
				if (map_ptr == nullptr)
				{
					map_ptr = new std::map<TA_KEY, char*>;
					if (map_ptr == nullptr)
						return ::pilo::mk_perr(PERR_NULL_PARAM);

				}
				for (std::pair<TA_KEY, const char*> kvpair : list)
				{
					typename std::map<TA_KEY, char*>::iterator it = map_ptr->find(kvpair.first);
					if (it != map_ptr->end())
					{
						if (!is_force)
						{
							return ::pilo::mk_perr(PERR_EXIST);
						}
						map_ptr->erase(it);
					}

					char* tmp = ::pilo::core::string::duplicate(kvpair.second, -1);
					if (tmp == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					map_ptr->insert(std::pair<TA_KEY, char*>(kvpair.first, tmp));					
				}
				return PILO_OK;;
			}


			template<typename TA_KEY> inline ::pilo::err_t map_ptr_insert(char* &map_char_ptr, std::initializer_list<std::pair<TA_KEY, char*>> list, bool adopt, bool is_force)
			{
				std::map<TA_KEY, char*>*& map_ptr = (std::map<TA_KEY, char*>*&) map_char_ptr;
				if (map_ptr == nullptr)
				{
					map_ptr = new std::map<TA_KEY, char*>;
					if (map_ptr == nullptr)
						return ::pilo::mk_perr(PERR_NULL_PARAM);
				}
				for (std::pair<TA_KEY, char*> kvpair : list)
				{
					typename std::map<TA_KEY, char*>::iterator it = map_ptr->find(kvpair->first);
					if (it != map_ptr->end())
					{
						if (!is_force)
						{
							return ::pilo::mk_perr(PERR_EXIST);
						}
						map_ptr->erase(it);
					}
					if (adopt)
						map_ptr->insert(kvpair);
					else
					{
						char* tmp = ::pilo::core::string::duplicate(kvpair->second, -1);
						if (tmp == nullptr)
							return ::pilo::mk_perr( PERR_INSUF_HEAP);
						map_ptr->insert(std::pair<TA_KEY, char*>(kvpair.first, tmp));
					}
				}
				return PILO_OK;;
			}

			template<typename TA_KEY> inline ::pilo::err_t map_ptr_insert(char* &map_char_ptr, std::initializer_list<std::pair<TA_KEY, ::pilo::tlv*>> list, bool adopt, bool is_force)
			{
				std::map<TA_KEY, ::pilo::tlv*>*& map_ptr = (std::map<TA_KEY, ::pilo::tlv*>*&) map_char_ptr;
				if (map_ptr == nullptr)
				{
					map_ptr = new std::map<TA_KEY, ::pilo::tlv*>;
					if (map_ptr == nullptr)
						return ::pilo::mk_perr(PERR_NULL_PARAM);
				}
				for (std::pair<TA_KEY, ::pilo::tlv*> kvpair : list)
				{
					typename std::map<TA_KEY, ::pilo::tlv*>::iterator it = map_ptr->find(kvpair.first);
					if (it != map_ptr->end())
					{
						if (!is_force)
						{
							return ::pilo::mk_perr(PERR_EXIST);
						}
						map_ptr->erase(it);
					}
					if (adopt)
						map_ptr->insert(kvpair);
					else
					{
						::pilo::tlv* tmp = kvpair.second->clone();
						if (tmp == nullptr)
							return ::pilo::mk_perr( PERR_INSUF_HEAP);
						map_ptr->insert(std::pair<TA_KEY, char*>(kvpair.first, tmp));
					}
				}
				return PILO_OK;;
			}

			template<typename TA_KEY, typename TA_VALUE> inline ::pilo::err_t map_ptr_insert(std::map<TA_KEY, TA_VALUE> * map_ptr, const TA_KEY& key, const TA_VALUE& value, bool is_force)
			{
				typename std::map<TA_KEY, TA_VALUE>::iterator it = map_ptr->find(key);
				if (it == map_ptr->end())
				{
					map_ptr->insert(std::pair<TA_KEY, TA_VALUE>(key, value));
					return PILO_OK;
				}

				if (!is_force)
				{
					return ::pilo::mk_perr(PERR_EXIST);
				}
				else
				{
					map_ptr->erase(it);
					map_ptr->insert(std::pair<TA_KEY, TA_VALUE>(key, value));
					return PILO_OK;
				}
			}


			template<typename TA_KEY, typename TA_VALUE> inline ::pilo::err_t map_ptr_insert(char* &map_char_ptr, const TA_KEY& key, const TA_VALUE& value, bool is_force)
			{
				std::map<TA_KEY, TA_VALUE>*& map_ptr = (std::map<TA_KEY, TA_VALUE>*&) map_char_ptr;
				if (map_ptr == nullptr)
				{
					map_ptr = new std::map<TA_KEY, TA_VALUE>;
					if (map_ptr == nullptr)
						return ::pilo::mk_perr(PERR_NULL_PARAM);
				}
				typename std::map<TA_KEY, TA_VALUE>::iterator it = map_ptr->find(key);
				if (it == map_ptr->end())
				{
					map_ptr->insert(std::pair<TA_KEY, TA_VALUE>(key, value));
					return PILO_OK;
				}

				if (! is_force)
				{
					return ::pilo::mk_perr(PERR_EXIST);
				}
				else
				{
					map_ptr->erase(it);
					map_ptr->insert(std::pair<TA_KEY, TA_VALUE>(key, value));
					return PILO_OK;
				}
			}

			template<typename TA_KEY> inline ::pilo::err_t map_ptr_insert(char* &map_char_ptr, const TA_KEY& key, const char* value, bool is_adopt, bool is_force)
			{
				PMC_UNUSED(is_adopt);
				std::map<TA_KEY, char*>*& map_ptr = (std::map<TA_KEY, char*>*&) map_char_ptr;
				if (map_ptr == nullptr)
				{
					map_ptr = new std::map<TA_KEY, char*>;
					if (map_ptr == nullptr)
						return ::pilo::mk_perr(PERR_NULL_PARAM);
				}
				typename std::map<TA_KEY, char*>::iterator it = map_ptr->find(key);
				if (it != map_ptr->end())
				{
					if (!is_force)
					{
						return ::pilo::mk_perr(PERR_EXIST);
					}
					else
					{
						map_ptr->erase(it);
					}
				}

				if (value == nullptr)
				{
					map_ptr->insert(std::pair<TA_KEY, char*>(key, nullptr));
					return PILO_OK;
				}
				else
				{
					auto ptr = ::pilo::core::string::duplicate(value, -1);
					if (ptr == nullptr)
					{
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					}
					map_ptr->insert(std::pair<TA_KEY, char*>(key, ptr));
				}

				return PILO_OK;
			}

			template<typename TA_KEY> inline ::pilo::err_t map_ptr_insert(char* &map_char_ptr, const TA_KEY& key, char* value, bool is_adopt, bool is_force)
			{
				std::map<TA_KEY, char*>*& map_ptr = (std::map<TA_KEY, char*>*&) map_char_ptr;
				if (map_ptr == nullptr)
				{
					map_ptr = new std::map<TA_KEY, char*>;
					if (map_ptr == nullptr)
						return ::pilo::mk_perr(PERR_NULL_PARAM);
				}
				typename std::map<TA_KEY, char*>::iterator it = map_ptr->find(key);
				if (it != map_ptr->end())
				{
					if (!is_force)
					{
						return ::pilo::mk_perr(PERR_EXIST);
					}
					else
					{
						map_ptr->erase(it);
					}
				}

				if (value == nullptr)
				{
					map_ptr->insert(std::pair<TA_KEY, char*>(key, nullptr));
					return PILO_OK;
				}
				else
				{
					if (!is_adopt)
					{
						auto ptr = ::pilo::core::string::duplicate(value, -1);
						if (ptr == nullptr)
						{
							return ::pilo::mk_perr( PERR_INSUF_HEAP);
						}
						map_ptr->insert(std::pair<TA_KEY, char*>(key, ptr));
					}
					else
					{
						map_ptr->insert(std::pair<TA_KEY, char*>(key, (char*)value));
					}
				}

				return PILO_OK;
			}
			
			char* create_map(::pilo::u8_t keytype, ::pilo::u16_t valtype, ::pilo::err_t*);
			template<typename TA_KEY, typename TA_VALUE> inline std::map<TA_KEY, TA_VALUE> * create_map(std::initializer_list<std::pair<TA_KEY, TA_VALUE>> list, ::pilo::err_t *)
			{
				std::map<TA_KEY, TA_VALUE>* ret = new std::map<TA_KEY, TA_VALUE>();
				
				for (std::pair<TA_KEY, TA_VALUE> kvpair : list)
				{
					ret->insert(kvpair);
				}
				return ret;
			}
			template<typename TA_KEY, typename TA_VALUE> inline std::map<TA_KEY, TA_VALUE>* create_map(::pilo::err_t*)
			{
				std::map<TA_KEY, TA_VALUE>* ret = new std::map<TA_KEY, TA_VALUE>();
				return ret;
			}

			template<typename TA_KEY, typename TA_VALUE> inline ::pilo::err_t delete_map(std::map<TA_KEY, TA_VALUE>* map_ptr)
			{
				delete map_ptr;
				return PILO_OK;
			}
			template<typename TA_KEY> inline ::pilo::err_t delete_map(std::map<TA_KEY, char*>* map_ptr)
			{
				typename std::map<TA_KEY, char*>::iterator it = map_ptr->begin();
				for (; it != map_ptr->end(); it++)
				{
					if (it->second != nullptr)
					{
						PMF_HEAP_FREE(it->second);
					}
				}
				delete map_ptr;
				return PILO_OK;
			}
			template<typename TA_KEY> inline ::pilo::err_t delete_map(std::map<TA_KEY, ::pilo::tlv*>* map_ptr)
			{
				typename std::map<TA_KEY, ::pilo::tlv*>::iterator it = map_ptr->begin();
				for (; it != map_ptr->end(); it++)
				{
					if (it->second != nullptr)
					{
						delete_tlv(it->second);
					}
				}
				delete map_ptr;
				return PILO_OK;
			}
			::pilo::err_t delete_map(::pilo::u8_t keytype, ::pilo::u16_t valtype, char* map_char_ptr);

			template<typename TA_KEY, typename TA_VALUE> 
			inline std::map<TA_KEY, TA_VALUE>* clone_map(std::map<TA_KEY, TA_VALUE>* src, ::pilo::err_t* err)
			{
				if (src == nullptr)
					return nullptr;

				typename std::map<TA_KEY, TA_VALUE>* cloned_map = new std::map<TA_KEY, TA_VALUE>(*src);
				if (cloned_map == nullptr)
				{
					if (err != nullptr)
						*err = ::pilo::mk_perr( PERR_INSUF_HEAP);
					return nullptr;
				}
				return cloned_map;
			}

			template<typename TA_KEY>
			inline std::map<TA_KEY, char*>* clone_map(std::map<TA_KEY, char*>* src, ::pilo::err_t* err)
			{
				if (src == nullptr)
					return nullptr;

				typename std::map<TA_KEY, char*>* cloned_map = new std::map<TA_KEY, char*>();
				if (cloned_map == nullptr)
				{
					if (err != nullptr)
						*err = ::pilo::mk_perr( PERR_INSUF_HEAP);
					return nullptr;
				}

				typename std::map<TA_KEY, char*>::const_iterator it = src->cbegin();
				for (; it != src->cend(); it++)
				{
					if (it->second != nullptr)
					{
						char* tmp_ptr = (char*) ::pilo::core::string::duplicate(it->second, -1);
						if (tmp_ptr == nullptr)
						{
							if (err != nullptr)
								*err = ::pilo::mk_perr( PERR_INSUF_HEAP);
							return nullptr;
						}
						cloned_map->insert(std::pair<TA_KEY, char*>(it->first, tmp_ptr));
					}
					else
					{
						cloned_map->insert(std::pair<TA_KEY, char*>(it->first, nullptr));
					}
				}

				return cloned_map;
			}

			template<typename TA_KEY>
			inline std::map<TA_KEY, char*>* clone_map(std::map<TA_KEY, const char*>* src, ::pilo::err_t* err)
			{
				if (src == nullptr)
					return nullptr;

				typename std::map<TA_KEY, char*>* cloned_map = new std::map<TA_KEY, char*>();
				if (cloned_map == nullptr)
				{
					if (err != nullptr)
						*err = ::pilo::mk_perr( PERR_INSUF_HEAP);
					return nullptr;
				}

				typename std::map<TA_KEY, const char*>::const_iterator it = src->cbegin();
				for (; it != src->cend(); it++)
				{
					if (it->second != nullptr)
					{
						char* tmp_ptr = (char*) ::pilo::core::string::duplicate(it->second, -1);
						if (tmp_ptr == nullptr)
						{
							if (err != nullptr)
								*err = ::pilo::mk_perr( PERR_INSUF_HEAP);
							return nullptr;
						}
						cloned_map->insert(std::pair<TA_KEY, char*>(it->first, tmp_ptr));
					}
					else
					{
						cloned_map->insert(std::pair<TA_KEY, char*>(it->first, nullptr));
					}
				}

				return cloned_map;
			}

			


			template<typename TA_KEY>
			inline std::map<TA_KEY, ::pilo::tlv*>* clone_map(std::map<TA_KEY, ::pilo::tlv*>* src, ::pilo::err_t* err)
			{
				if (src == nullptr)
					return nullptr;

				typename std::map<TA_KEY, ::pilo::tlv*>* cloned_map = new std::map<TA_KEY, ::pilo::tlv*>();
				if (cloned_map == nullptr)
				{
					if (err != nullptr)
						*err = ::pilo::mk_perr( PERR_INSUF_HEAP);
					return nullptr;
				}

				typename std::map<TA_KEY, ::pilo::tlv*>::const_iterator it = src->cbegin();
				for (; it != src->cend(); it++)
				{
					if (it->second != nullptr)
					{
						::pilo::tlv* tmp_tlv = it->second->clone();
						if (tmp_tlv == nullptr)
						{
							if (err != nullptr)
								*err = ::pilo::mk_perr(PERR_OBJECT_CLONE_FAIL);
							return nullptr;
						}
                        cloned_map->insert(std::pair<TA_KEY, ::pilo::tlv*>(it->first, tmp_tlv));
					}
					else
					{
                        cloned_map->insert(std::pair<TA_KEY, ::pilo::tlv*>(it->first, nullptr));
					}
				}

				return cloned_map;
			}

			char* clone_map(::pilo::u8_t keytype, ::pilo::u16_t valtype, char* map_char_ptr, pilo::err_t* err);

		}
	}
}