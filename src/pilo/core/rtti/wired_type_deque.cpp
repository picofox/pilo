#include "../../pilo.hpp"
#include "./wired_type.hpp"
#include "../string/string_operation.hpp"
#include <vector>
#include <deque>
#include "wired_type_deque.hpp"
#include "../../tlv.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			typedef bool(*handle_deque_compare_func_type)(const char* a, const char* b, ::pilo::err_t* err);
			static handle_deque_compare_func_type stc_handle_deque_compare_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },
				[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return deque_equals((const std::deque<::pilo::i8_t>*) a_data, (const std::deque<::pilo::i8_t>*) b_data, err); },//1
				[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return deque_equals((const std::deque<::pilo::u8_t>*) a_data, (const std::deque<::pilo::u8_t>*) b_data, err); },//2
				[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return deque_equals((const std::deque<::pilo::i16_t>*) a_data, (const std::deque<::pilo::i16_t>*) b_data, err); },//3
				[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return deque_equals((const std::deque<::pilo::u16_t>*) a_data, (const std::deque<::pilo::u16_t>*) b_data, err); },//4
				[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return deque_equals((const std::deque<::pilo::i32_t>*) a_data, (const std::deque<::pilo::i32_t>*) b_data, err); },//5
				[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return deque_equals((const std::deque<::pilo::u32_t>*) a_data, (const std::deque<::pilo::u32_t>*) b_data, err); },//6
				[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return deque_equals((const std::deque<::pilo::i64_t>*) a_data, (const std::deque<::pilo::i64_t>*) b_data, err); },//7
				[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return deque_equals((const std::deque<::pilo::u64_t>*) a_data, (const std::deque<::pilo::u64_t>*) b_data, err); },//8
				[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return deque_equals((const std::deque<bool>*) a_data, (const std::deque<bool>*) b_data, err); },//10
				[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return deque_equals((const std::deque<::pilo::f32_t>*) a_data, (const std::deque<::pilo::f32_t>*) b_data, err); },//10
				[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return deque_equals((const std::deque<::pilo::f64_t>*) a_data, (const std::deque<::pilo::f64_t>*) b_data, err); },//11
				[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return deque_equals((const std::deque<char*>*) a_data, (const std::deque<char*>*) b_data, err); },//12
				[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return deque_equals((const std::deque<std::string>*) a_data, (const std::deque<std::string>*) b_data, err); },//13
				[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return deque_equals((const std::deque<::pilo::tlv*>*) a_data, (const std::deque<::pilo::tlv*>*) b_data, err); },//14
			};


			typedef char* (*handle_deque_clone_func_type)(char* deque_char_ptr, ::pilo::err_t* err);
			static handle_deque_clone_func_type stc_handle_deque_clone_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char*, ::pilo::err_t* err) -> char* {if (err != nullptr) *err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE); return nullptr; },
				[](char* deque_char_ptr, ::pilo::err_t* err) -> char* { return (char*)clone_deque((std::deque<::pilo::i8_t>*) deque_char_ptr, err); },
				[](char* deque_char_ptr, ::pilo::err_t* err) -> char* { return (char*)clone_deque((std::deque<::pilo::u8_t>*) deque_char_ptr, err); },
				[](char* deque_char_ptr, ::pilo::err_t* err) -> char* { return (char*)clone_deque((std::deque<::pilo::i16_t>*) deque_char_ptr, err); },
				[](char* deque_char_ptr, ::pilo::err_t* err) -> char* { return (char*)clone_deque((std::deque<::pilo::u16_t>*) deque_char_ptr, err); },
				[](char* deque_char_ptr, ::pilo::err_t* err) -> char* { return (char*)clone_deque((std::deque<::pilo::i32_t>*) deque_char_ptr, err); },
				[](char* deque_char_ptr, ::pilo::err_t* err) -> char* { return (char*)clone_deque((std::deque<::pilo::u32_t>*) deque_char_ptr, err); },
				[](char* deque_char_ptr, ::pilo::err_t* err) -> char* { return (char*)clone_deque((std::deque<::pilo::i64_t>*) deque_char_ptr, err); },
				[](char* deque_char_ptr, ::pilo::err_t* err) -> char* { return (char*)clone_deque((std::deque<::pilo::u64_t>*) deque_char_ptr, err); },
				[](char* deque_char_ptr, ::pilo::err_t* err) -> char* { return (char*)clone_deque((std::deque<bool>*) deque_char_ptr, err); },
				[](char* deque_char_ptr, ::pilo::err_t* err) -> char* { return (char*)clone_deque((std::deque<::pilo::f32_t>*) deque_char_ptr, err); },
				[](char* deque_char_ptr, ::pilo::err_t* err) -> char* { return (char*)clone_deque((std::deque<::pilo::f64_t>*) deque_char_ptr, err); }, //11
				[](char* deque_char_ptr, ::pilo::err_t* err) -> char* { return (char*)clone_deque((std::deque<char*>*) deque_char_ptr, err); },
				[](char* deque_char_ptr, ::pilo::err_t* err) -> char* { return (char*)clone_deque((std::deque<std::string>*) deque_char_ptr, err); },
				[](char* deque_char_ptr, ::pilo::err_t* err) -> char* { return (char*)clone_deque((std::deque<::pilo::tlv*>*) deque_char_ptr, err); },

			};

			typedef ::pilo::err_t(*handle_deque_delete_func_type)(char** deque_char_pptr);
			static handle_deque_delete_func_type stc_handle_deque_delete_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char**) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char** deque_char_pptr) -> ::pilo::err_t	{ destroy_deque((std::deque<::pilo::i8_t>**) deque_char_pptr);return PILO_OK;}, //i8
				[](char** deque_char_pptr) -> ::pilo::err_t { destroy_deque((std::deque<::pilo::u8_t>**) deque_char_pptr); return PILO_OK; }, //u8 2
				[](char** deque_char_pptr) -> ::pilo::err_t { destroy_deque((std::deque<::pilo::i16_t>**) deque_char_pptr); return PILO_OK; }, //i16 3
				[](char** deque_char_pptr) -> ::pilo::err_t { destroy_deque((std::deque<::pilo::u16_t>**) deque_char_pptr); return PILO_OK; }, //u16 4
				[](char** deque_char_pptr) -> ::pilo::err_t { destroy_deque((std::deque<::pilo::i32_t>**) deque_char_pptr); return PILO_OK; }, //i32 5
				[](char** deque_char_pptr) -> ::pilo::err_t { destroy_deque((std::deque<::pilo::u32_t>**) deque_char_pptr); return PILO_OK; }, //u32 6
				[](char** deque_char_pptr) -> ::pilo::err_t { destroy_deque((std::deque<::pilo::i64_t>**) deque_char_pptr); return PILO_OK; }, //i64 7
				[](char** deque_char_pptr) -> ::pilo::err_t { destroy_deque((std::deque<::pilo::u64_t>**) deque_char_pptr); return PILO_OK; }, //u64 8
				[](char** deque_char_pptr) -> ::pilo::err_t { destroy_deque((std::deque<bool>**) deque_char_pptr); return PILO_OK; }, //bool 9
				[](char** deque_char_pptr) -> ::pilo::err_t { destroy_deque((std::deque<::pilo::f32_t>**) deque_char_pptr); return PILO_OK; }, //f32 10
				[](char** deque_char_pptr) -> ::pilo::err_t { destroy_deque((std::deque<::pilo::f64_t>**) deque_char_pptr); return PILO_OK; }, //f64 11
				[](char** deque_char_pptr) -> ::pilo::err_t { destroy_deque((std::deque<char*>**) deque_char_pptr); return PILO_OK; }, //bytes 12
				[](char** deque_char_pptr) -> ::pilo::err_t { destroy_deque((std::deque<std::string>**) deque_char_pptr); return PILO_OK; }, //str 13
				[](char** deque_char_pptr) -> ::pilo::err_t { destroy_deque((std::deque<::pilo::tlv*>**) deque_char_pptr); return PILO_OK; }, //tlv 14
			};

			void clear_deque(std::deque<char*>* deq, bool compact)
			{
				if (deq != nullptr)
				{
					for (::pilo::i64_t idx = 0; idx < (::pilo::i64_t)(deq)->size(); idx++)
					{
						if ((deq)->at(idx) != nullptr)
						{
							PMF_HEAP_FREE((deq)->at(idx));
							(deq)->at(idx) = nullptr;
						}
					}
					(deq)->clear();
					if (compact)
						(deq)->shrink_to_fit();
				}
			}

			void clear_deque(std::deque<::pilo::tlv*>* deq, bool compact)
			{
				if (deq != nullptr)
				{
					for (::pilo::i64_t idx = 0; idx < (::pilo::i64_t) (deq)->size(); idx++)
					{
						if ((deq)->at(idx) != nullptr)
						{
							::pilo::tlv::deallocate(deq->at(idx));
							(deq)->at(idx) = nullptr;
						}
					}
					(deq)->clear();
					if (compact)
						(deq)->shrink_to_fit();
				}
			}
			::pilo::err_t destroy_deque(::pilo::u16_t vt, char** data_pptr)
			{
				if (vt >= ::pilo::core::rtti::wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return 0;
				}

				return stc_handle_deque_delete_func[vt](data_pptr);
			}
			char* create_deque(::pilo::u16_t valtype, ::pilo::err_t* err)
			{
				char* data = nullptr;

				if (valtype == ::pilo::core::rtti::wired_type::value_type_tlv)
					data = (char*) new std::deque<::pilo::tlv*>();
				else if (valtype == ::pilo::core::rtti::wired_type::value_type_str)
					data = (char*) new std::deque<std::string>();
				else if (valtype == ::pilo::core::rtti::wired_type::value_type_i64)
					data = (char*) new std::deque<::pilo::i64_t>();
				else if (valtype == ::pilo::core::rtti::wired_type::value_type_u64)
					data = (char*) new std::deque<::pilo::u64_t>();
				else if (valtype == ::pilo::core::rtti::wired_type::value_type_i32)
					data = (char*) new std::deque<::pilo::i32_t>();
				else if (valtype == ::pilo::core::rtti::wired_type::value_type_u32)
					data = (char*) new std::deque<::pilo::u32_t>();
				else if (valtype == ::pilo::core::rtti::wired_type::value_type_i16)
					data = (char*) new std::deque<::pilo::i16_t>();
				else if (valtype == ::pilo::core::rtti::wired_type::value_type_u16)
					data = (char*) new std::deque<::pilo::u16_t>();
				else if (valtype == ::pilo::core::rtti::wired_type::value_type_i8)				
					data = (char*) new std::deque<::pilo::i8_t>();
				else if (valtype == ::pilo::core::rtti::wired_type::value_type_u8)
					data = (char*) new std::deque<::pilo::u8_t>();
				else if (valtype == ::pilo::core::rtti::wired_type::value_type_f32)
					data = (char*) new std::deque<::pilo::f32_t>();
				else if (valtype == ::pilo::core::rtti::wired_type::value_type_f64)
					data = (char*) new std::deque<::pilo::f64_t>();
				else if (valtype == ::pilo::core::rtti::wired_type::value_type_boolean)
					data = (char*) new std::deque<bool>();
				else if (valtype == ::pilo::core::rtti::wired_type::value_type_bytes)
					data = (char*) new std::deque<char*>();
				else
				{
					::pilo::set_if_ptr_is_not_null(err, ::pilo::mk_perr(PERR_MIS_DATA_TYPE));
					return nullptr;
				}

				if (data == nullptr)
				{
					::pilo::set_if_ptr_is_not_null(err, ::pilo::mk_perr( PERR_INSUF_HEAP));
					return nullptr;
				}
				
				return data;				
			}


			std::deque<char*>* create_deque(const std::initializer_list<const char*>& list, bool adopt, ::pilo::err_t* errp)
			{
				PMC_UNUSED(adopt);
				std::deque<char*>* deq = new std::deque<char*>;
				if (deq == nullptr)
				{
					::pilo::set_if_ptr_is_not_null(errp, ::pilo::mk_perr( PERR_INSUF_HEAP));
					return nullptr;
				}
				char* tmp = nullptr;
				for (const char* val : list)
				{
					if (val == nullptr)
					{
						deq->push_back(nullptr);
					}
					else
					{
						tmp = ::pilo::core::string::duplicate(val, -1);
						if (tmp == nullptr)
						{
							destroy_deque(&deq);
						}
						deq->push_back(tmp);
					}

				}
				return deq;
			}

			std::deque<char*>* create_deque(const std::initializer_list<char*>& list, bool adopt, ::pilo::err_t* errp)
			{
				std::deque<char*>* deq = new std::deque<char*>;
				if (deq == nullptr)
				{
					::pilo::set_if_ptr_is_not_null(errp, ::pilo::mk_perr( PERR_INSUF_HEAP));
					return nullptr;
				}
				char* tmp = nullptr;
				for (char* val : list)
				{
					if (val == nullptr)
					{
						deq->push_back(nullptr);
					}
					else
					{
						if (adopt)
						{
							deq->push_back(val);
						}
						else
						{
							tmp = ::pilo::core::string::duplicate(val, -1);
							if (tmp == nullptr)
							{
								destroy_deque(&deq);
							}
							deq->push_back(tmp);
						}
					}
				}
				return deq;
			}


			std::deque<::pilo::tlv*>* create_deque(const std::initializer_list<::pilo::tlv*>& list, bool adopt, ::pilo::err_t* errp)
			{
				std::deque<::pilo::tlv*>* deq = new std::deque<::pilo::tlv*>;
				if (deq == nullptr)
				{
					::pilo::set_if_ptr_is_not_null(errp, ::pilo::mk_perr( PERR_INSUF_HEAP));
					return nullptr;
				}
				::pilo::tlv* tmp = nullptr;
				for (::pilo::tlv* val : list)
				{
					if (val == nullptr)
					{
						deq->push_back(nullptr);
					}
					else
					{
						if (adopt)
						{
							deq->push_back(val);
						}
						else
						{
							tmp = val->clone();
							if (tmp == nullptr)
							{
								destroy_deque(&deq);
							}
							deq->push_back(tmp);
						}
					}
				}
				return deq;
			}

			std::deque<char*>* clone_deque(std::deque<char*>* src, ::pilo::err_t* err)
			{
				if (src == nullptr)
					return nullptr;
				typename std::deque<char*>* cloned_deque = new std::deque<char*>;
				if (cloned_deque == nullptr)
				{
					if (err != nullptr)
						*err = ::pilo::mk_perr( PERR_INSUF_HEAP);
					return nullptr;
				}

				typename std::deque<char*>::const_iterator it = src->cbegin();
				for (; it != src->cend(); it++)
				{
					if (*it == nullptr)
					{
						cloned_deque->push_back(nullptr);
					}
					else
					{
						char* tmp_ptr = (char*) ::pilo::core::string::duplicate(*it, -1);
						if (tmp_ptr == nullptr)
						{
							if (err != nullptr)
								*err = ::pilo::mk_perr( PERR_INSUF_HEAP);
							destroy_deque((std::deque<char*>**) & cloned_deque);
							return nullptr;
						}
						cloned_deque->push_back(tmp_ptr);
					}
				}
				return cloned_deque;
			}

			std::deque<char*>* clone_deque(std::deque<const char*>* src, ::pilo::err_t* err)
			{
				if (src == nullptr)
					return nullptr;
				typename std::deque<char*>* cloned_deque = new std::deque<char*>;
				if (cloned_deque == nullptr)
				{
					if (err != nullptr)
						*err = ::pilo::mk_perr( PERR_INSUF_HEAP);
					return nullptr;
				}

				typename std::deque<const char*>::const_iterator it = src->cbegin();
				for (; it != src->cend(); it++)
				{
					if (*it == nullptr)
					{
						cloned_deque->push_back(nullptr);
					}
					else
					{
						char* tmp_ptr = (char*) ::pilo::core::string::duplicate(*it, -1);
						if (tmp_ptr == nullptr)
						{
							if (err != nullptr)
								*err = ::pilo::mk_perr( PERR_INSUF_HEAP);
							destroy_deque((std::deque<char*>**) & cloned_deque);
							return nullptr;
						}
						cloned_deque->push_back(tmp_ptr);
					}
				}
				return cloned_deque;
			}

			std::deque<::pilo::tlv*>* clone_deque(std::deque<::pilo::tlv*>* src, ::pilo::err_t* err)
			{
				if (src == nullptr)
					return nullptr;
				typename std::deque<::pilo::tlv*>* cloned_deque = new std::deque<::pilo::tlv*>;
				if (cloned_deque == nullptr)
				{
					if (err != nullptr)
						*err = ::pilo::mk_perr( PERR_INSUF_HEAP);
					return nullptr;
				}

				typename std::deque<::pilo::tlv*>::const_iterator it = src->cbegin();
				for (; it != src->cend(); it++)
				{
					if (*it == nullptr)
					{
						cloned_deque->push_back(nullptr);
					}
					else
					{
						::pilo::tlv* cloned_tlv = (*it)->clone();
						if (cloned_tlv == nullptr)
						{
							if (err != nullptr)
								*err = ::pilo::mk_perr( PERR_INSUF_HEAP);
							destroy_deque((std::deque<::pilo::tlv*>**) &cloned_deque);
							return nullptr;
						}
						cloned_deque->push_back(cloned_tlv);
					}
				}
				return cloned_deque;
			}

			char* clone_deque(::pilo::u16_t vt, char* src, pilo::err_t* err)
			{
				if (vt >= ::pilo::core::rtti::wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return nullptr;
				}

				return stc_handle_deque_clone_func[vt](src, err);
			}


			bool deque_equals(const std::deque<char*>* a, const std::deque<char*>* b, ::pilo::err_t*)
			{
				if (a->size() != b->size())
					return false;
				typename std::deque<char*>::const_iterator cit_a = a->cbegin();
				typename std::deque<char*>::const_iterator cit_b = b->cbegin();
				for (; cit_a != a->cend(); cit_a++, cit_b++)
				{
					if (::pilo::core::string::strict_compare(*cit_a, 0, *cit_b, 0, -1) != 0)
					{
						return false;
					}
				}
				return true;
			}

			bool deque_equals(const std::deque<const char*>* a, const std::deque<char*>* b, ::pilo::err_t*)
			{
				if (a->size() != b->size())
					return false;
				typename std::deque<const char*>::const_iterator cit_a = a->cbegin();
				typename std::deque<char*>::const_iterator cit_b = b->cbegin();
				for (; cit_a != a->cend(); cit_a++, cit_b++)
				{
					if (::pilo::core::string::strict_compare(*cit_a, 0, *cit_b, 0, -1) != 0)
					{
						return false;
					}
				}
				return true;
			}

			bool deque_equals(const std::deque< char*>* a, const std::deque<const char*>* b, ::pilo::err_t*)
			{
				if (a->size() != b->size())
					return false;
				typename std::deque< char*>::const_iterator cit_a = a->cbegin();
				typename std::deque<const char*>::const_iterator cit_b = b->cbegin();
				for (; cit_a != a->cend(); cit_a++, cit_b++)
				{
					if (::pilo::core::string::strict_compare(*cit_a, 0, *cit_b, 0, -1) != 0)
					{
						return false;
					}
				}
				return true;
			}


			bool deque_equals(const std::deque<const char*>* a, const std::deque<const char*>* b, ::pilo::err_t*)
			{
				if (a->size() != b->size())
					return false;
				typename std::deque<const char*>::const_iterator cit_a = a->cbegin();
				typename std::deque<const char*>::const_iterator cit_b = b->cbegin();
				for (; cit_a != a->cend(); cit_a++, cit_b++)
				{
					if (::pilo::core::string::strict_compare(*cit_a, 0, *cit_b, 0, -1) != 0)
					{
						return false;
					}
				}
				return true;
			}


			bool deque_equals(const std::deque<::pilo::tlv*>* a, const std::deque<::pilo::tlv*>* b, ::pilo::err_t*)
			{

				if (a->size() != b->size())
					return false;
				typename std::deque<::pilo::tlv*>::const_iterator cit_a = a->cbegin();
				typename std::deque<::pilo::tlv*>::const_iterator cit_b = b->cbegin();
				for (; cit_a != a->cend(); cit_a++, cit_b++)
				{
					if (*cit_a == nullptr &&  *cit_b == nullptr)
					{

					}
					else if (*cit_a != nullptr && *cit_b != nullptr)
					{
						if (!(*cit_a)->equals_to(*cit_b))
						{
							return false;
						}
					}
					else
					{
						return false;
					}
				}
				return true;
			}

			bool deque_equals(const std::deque<const ::pilo::tlv*>* a, const std::deque<::pilo::tlv*>* b, ::pilo::err_t*)
			{
				if (a->size() != b->size())
					return false;
				typename std::deque<const ::pilo::tlv*>::const_iterator cit_a = a->cbegin();
				typename std::deque<::pilo::tlv*>::const_iterator cit_b = b->cbegin();
				for (; cit_a != a->cend(); cit_a++, cit_b++)
				{
					if (*cit_a == nullptr && *cit_b == nullptr)
					{

					}
					else if (*cit_a != nullptr && *cit_b != nullptr)
					{
						if (!(*cit_a)->equals_to(*cit_b))
						{
							return false;
						}
					}
					else
					{
						return false;
					}
				}
				return true;
			}

			bool deque_equals(const std::deque<::pilo::tlv*>* a, const std::deque<const ::pilo::tlv*>* b, ::pilo::err_t*)
			{
				if (a->size() != b->size())
					return false;
				typename std::deque<::pilo::tlv*>::const_iterator cit_a = a->cbegin();
				typename std::deque<const ::pilo::tlv*>::const_iterator cit_b = b->cbegin();
				for (; cit_a != a->cend(); cit_a++, cit_b++)
				{
					if (*cit_a == nullptr && *cit_b == nullptr)
					{

					}
					else if (*cit_a != nullptr && *cit_b != nullptr)
					{
						if (!(*cit_a)->equals_to(*cit_b))
						{
							return false;
						}
					}
					else
					{
						return false;
					}
				}
				return true;
			}

			bool deque_equals(const std::deque<const ::pilo::tlv*>* a, const std::deque<const ::pilo::tlv*>* b, ::pilo::err_t*)
			{
				if (a->size() != b->size())
					return false;
				typename std::deque<const ::pilo::tlv*>::const_iterator cit_a = a->cbegin();
				typename std::deque<const ::pilo::tlv*>::const_iterator cit_b = b->cbegin();
				for (; cit_a != a->cend(); cit_a++, cit_b++)
				{
					if (*cit_a == nullptr && *cit_b == nullptr)
					{

					}
					else if (*cit_a != nullptr && *cit_b != nullptr)
					{
						if (!(*cit_a)->equals_to(*cit_b))
						{
							return false;
						}
					}
					else
					{
						return false;
					}
				}
				return true;
			}
			bool deque_equals(::pilo::u16_t a_val_type, const char* a_data, ::pilo::u16_t b_val_type, const char* b_data, ::pilo::err_t* err)
			{
				if (a_val_type != b_val_type) return false;
				if (a_data == b_data) return true;
				if (a_data == nullptr || b_data == nullptr) return false;

				if (a_val_type >= ::pilo::core::rtti::wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return false;
				}

				return stc_handle_deque_compare_func[a_val_type](a_data, b_data, err);

			}


			typedef ::pilo::err_t(*handler_deque_pushback_i8_func_type)(char* dq, ::pilo::i8_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushback_i8_func_type stc_handler_deque_pushback_i8_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , ::pilo::i8_t, ::pilo::i32_t , bool ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_back(value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_back((::pilo::u8_t) value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_back((::pilo::i16_t) value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_back((::pilo::u16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_back((::pilo::i32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_back((::pilo::u32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_back((::pilo::i64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_back((::pilo::u64_t)value); 
					return PILO_OK;
				},
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->push_back(value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_back((::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_back((::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(8);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 8, value);
					reinterpret_cast<std::deque<char*>*>(dq)->push_back(ret);
					return PILO_OK;
				},//12

				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[8] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->push_back(std::string(buffer));
					return PILO_OK;
				},//13
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool ) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_back(t);
						return PILO_OK;
					
				},//14


			};

			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, ::pilo::i8_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushback_i8_func[vt](dq, value, len, adopt);
			}

			typedef ::pilo::err_t(*handler_deque_pushback_u8_func_type)(char* dq, ::pilo::u8_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushback_u8_func_type stc_handler_deque_pushback_u8_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , ::pilo::u8_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_back((::pilo::i8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_back(value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_back((::pilo::i16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_back((::pilo::u16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_back((::pilo::i32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_back((::pilo::u32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_back((::pilo::i64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_back((::pilo::u64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->push_back(value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_back((::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_back((::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(8);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 8, value);
					reinterpret_cast<std::deque<char*>*>(dq)->push_back(ret);
					return PILO_OK;
				},//12

				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[8] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->push_back(std::string(buffer));
					return PILO_OK;
				},//13
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_back(t);
						return PILO_OK;

				},//14
			};
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, ::pilo::u8_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushback_u8_func[vt](dq, value, len, adopt);
			}


			typedef ::pilo::err_t(*handler_deque_pushback_i16_func_type)(char* dq, ::pilo::i16_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushback_i16_func_type stc_handler_deque_pushback_i16_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , ::pilo::i16_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_back((::pilo::i8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_back((::pilo::u8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_back(value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_back((::pilo::u16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_back((::pilo::i32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_back((::pilo::u32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_back((::pilo::i64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_back((::pilo::u64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->push_back(value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_back((::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_back((::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(16);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 16, value);
					reinterpret_cast<std::deque<char*>*>(dq)->push_back(ret);
					return PILO_OK;
				},//12

				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[16] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->push_back(std::string(buffer));
					return PILO_OK;
				},//13
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_back(t);
						return PILO_OK;

				},//14
			};
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, ::pilo::i16_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushback_i16_func[vt](dq, value, len, adopt);
			}

			typedef ::pilo::err_t(*handler_deque_pushback_u16_func_type)(char* dq, ::pilo::u16_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushback_u16_func_type stc_handler_deque_pushback_u16_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , ::pilo::u16_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_back((::pilo::i8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_back((::pilo::u8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_back((::pilo::i16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_back(value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_back((::pilo::i32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_back((::pilo::u32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_back((::pilo::i64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_back((::pilo::u64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->push_back(value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_back((::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_back((::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(16);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 16, value);
					reinterpret_cast<std::deque<char*>*>(dq)->push_back(ret);
					return PILO_OK;
				},//12

				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[16] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->push_back(std::string(buffer));
					return PILO_OK;
				},//13
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_back(t);
						return PILO_OK;

				},//14
			};
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, ::pilo::u16_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushback_u16_func[vt](dq, value, len, adopt);
			}


			typedef ::pilo::err_t(*handler_deque_pushback_i32_func_type)(char* dq, ::pilo::i32_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushback_i32_func_type stc_handler_deque_pushback_i32_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , ::pilo::i32_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_back((::pilo::i8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_back((::pilo::u8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_back((::pilo::i16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_back((::pilo::u16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_back(value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_back((::pilo::u32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_back((::pilo::i64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_back((::pilo::u64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->push_back(value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_back((::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_back((::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(16);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 16, value);
					reinterpret_cast<std::deque<char*>*>(dq)->push_back(ret);
					return PILO_OK;
				},//12

				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[16] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->push_back(std::string(buffer));
					return PILO_OK;
				},//13
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_back(t);
						return PILO_OK;

				},//14
			};
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, ::pilo::i32_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushback_i32_func[vt](dq, value, len, adopt);
			}


			typedef ::pilo::err_t(*handler_deque_pushback_u32_func_type)(char* dq, ::pilo::u32_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushback_u32_func_type stc_handler_deque_pushback_u32_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , ::pilo::u32_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_back((::pilo::i8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_back((::pilo::u8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_back((::pilo::i16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_back((::pilo::u16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_back((::pilo::i32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_back(value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_back((::pilo::i64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_back((::pilo::u64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->push_back(value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_back((::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_back((::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(16);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 16, value);
					reinterpret_cast<std::deque<char*>*>(dq)->push_back(ret);
					return PILO_OK;
				},//12

				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[16] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->push_back(std::string(buffer));
					return PILO_OK;
				},//13
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_back(t);
						return PILO_OK;

				},//14
			};
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, ::pilo::u32_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushback_u32_func[vt](dq, value, len, adopt);
			}


			typedef ::pilo::err_t(*handler_deque_pushback_i64_func_type)(char* dq, ::pilo::i64_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushback_i64_func_type stc_handler_deque_pushback_i64_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , ::pilo::i64_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_back((::pilo::i8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_back((::pilo::u8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_back((::pilo::i16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_back((::pilo::u16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_back((::pilo::i32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_back((::pilo::u32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_back(value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_back((::pilo::u64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->push_back(value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_back((::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_back((::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(32);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 32, value);
					reinterpret_cast<std::deque<char*>*>(dq)->push_back(ret);
					return PILO_OK;
				},//12

				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[32] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->push_back(std::string(buffer));
					return PILO_OK;
				},//13
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_back(t);
						return PILO_OK;

				},//14				
			};
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, ::pilo::i64_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushback_i64_func[vt](dq, value, len, adopt);
			}

			typedef ::pilo::err_t(*handler_deque_pushback_u64_func_type)(char* dq, ::pilo::u64_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushback_u64_func_type stc_handler_deque_pushback_u64_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , ::pilo::u64_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_back((::pilo::i8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_back((::pilo::u8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_back((::pilo::i16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_back((::pilo::u16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_back((::pilo::i32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_back((::pilo::u32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_back((::pilo::i64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_back(value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->push_back(value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_back((::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_back((::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(32);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 32, value);
					reinterpret_cast<std::deque<char*>*>(dq)->push_back(ret);
					return PILO_OK;
				},//12

				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[32] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->push_back(std::string(buffer));
					return PILO_OK;
				},//13
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_back(t);
						return PILO_OK;

				},//14				
			};
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, ::pilo::u64_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushback_u64_func[vt](dq, value, len, adopt);
			}


			typedef ::pilo::err_t(*handler_deque_pushback_bool_func_type)(char* dq, bool, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushback_bool_func_type stc_handler_deque_pushback_bool_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , bool, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_back((::pilo::i8_t)value);
					return PILO_OK;
				},
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_back((::pilo::u8_t)value);
					return PILO_OK;
				},
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_back((::pilo::i16_t)value);
					return PILO_OK;
				},
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_back((::pilo::u16_t)value);
					return PILO_OK;
				},
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_back((::pilo::i32_t)value);
					return PILO_OK;
				},
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_back((::pilo::u32_t)value);
					return PILO_OK;
				},
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_back((::pilo::i64_t)value);
					return PILO_OK;
				},
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_back((::pilo::u64_t)value);
					return PILO_OK;
				},
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->push_back(value);
					return PILO_OK;
				},//9
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_back((::pilo::f32_t)value ? 1.0f : 0.0f);
					return PILO_OK;
				},//10
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_back((::pilo::f64_t) value ? 1.0 : 0.0);
					return PILO_OK;
				},//11
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = nullptr;					
					if (value)
						ret = ::pilo::core::string::duplicate("true", 4);
					else					
						ret = ::pilo::core::string::duplicate("false", 5);					
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<char*>*>(dq)->push_back(ret);
					return PILO_OK;
				},//12

				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					if (value)
						reinterpret_cast<std::deque<std::string>*>(dq)->push_back(std::string("true"));
					else
						reinterpret_cast<std::deque<std::string>*>(dq)->push_back(std::string("false"));
					return PILO_OK;
				},//13
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_back(t);
						return PILO_OK;

				},//14				
			};
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, bool value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushback_bool_func[vt](dq, value, len, adopt);
			}


			typedef ::pilo::err_t(*handler_deque_pushback_f32_func_type)(char* dq, ::pilo::f32_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushback_f32_func_type stc_handler_deque_pushback_f32_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , ::pilo::f32_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_back((::pilo::i8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_back((::pilo::u8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_back((::pilo::i16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_back((::pilo::u16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_back((::pilo::i32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_back((::pilo::u32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_back((::pilo::i64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_back((::pilo::u64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->push_back(value == 0.0f ? false : true);
					return PILO_OK;
				},//9
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_back(value);
					return PILO_OK;
				},//10
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_back((::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(32);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 32, value);
					reinterpret_cast<std::deque<char*>*>(dq)->push_back(ret);
					return PILO_OK;
				},//12

				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[32] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->push_back(std::string(buffer));
					return PILO_OK;
				},//13
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_back(t);
						return PILO_OK;

				},//14
			};
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, ::pilo::f32_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushback_f32_func[vt](dq, value, len, adopt);
			}

			typedef ::pilo::err_t(*handler_deque_pushback_f64_func_type)(char* dq, ::pilo::f64_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushback_f64_func_type stc_handler_deque_pushback_f64_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , ::pilo::f64_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_back((::pilo::i8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_back((::pilo::u8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_back((::pilo::i16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_back((::pilo::u16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_back((::pilo::i32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_back((::pilo::u32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_back((::pilo::i64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_back((::pilo::u64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->push_back(value == 0.0 ? false : true);
					return PILO_OK;
				},//9
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_back((::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_back(value);
					return PILO_OK;
				},//11
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(32);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 32, value);
					reinterpret_cast<std::deque<char*>*>(dq)->push_back(ret);
					return PILO_OK;
				},//12

				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[32] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->push_back(std::string(buffer));
					return PILO_OK;
				},//13
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_back(t);
						return PILO_OK;

				},//14				
			};
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, ::pilo::f64_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushback_f64_func[vt](dq, value, len, adopt);
			}

			typedef ::pilo::err_t(*handler_deque_pushback_ccharptr_func_type)(char* dq, const char*, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushback_ccharptr_func_type stc_handler_deque_pushback_ccharptr_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , const char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_back((::pilo::i8_t)v);
					return PILO_OK;
				},//1
				[](char* dq, const char* value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_back((::pilo::u8_t)v);
					return PILO_OK;
				},//2
				[](char* dq, const char* value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_back((::pilo::i16_t)v);
					return PILO_OK;
				},//3
				[](char* dq, const char* value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_back((::pilo::u16_t)v);
					return PILO_OK;
				},//4
				[](char* dq, const char* value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_back(v);
					return PILO_OK;
				},//5
				[](char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_back((::pilo::u32_t)v);
					return PILO_OK;
				},//6
				[](char* dq, const char* value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i64_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_back(v);
					return PILO_OK;
				},//7
				[](char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::i64_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_back((::pilo::u64_t)v);
					return PILO_OK;
				},//8
				[](char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					bool v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_bool(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<bool>*>(dq)->push_back(v);
					return PILO_OK;
				},//9
				[](char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::f32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_back(v);
					return PILO_OK;
				},//10
				[](char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::f64_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_back(v);
					return PILO_OK;
				},//11
				[](char* dq, const char* value, ::pilo::i32_t len, bool ) -> ::pilo::err_t
				{
					if (value == nullptr)
					{
						reinterpret_cast<std::deque<char*>*>(dq)->push_back(nullptr);
					}
					else
					{
						char* ret = ::pilo::core::string::duplicate(value, len);
						if (ret == nullptr)
							return ::pilo::mk_perr( PERR_INSUF_HEAP);
						reinterpret_cast<std::deque<char*>*>(dq)->push_back(ret);
					}
					return PILO_OK;
				},//12
				[](char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					if (value == nullptr)
					{
						reinterpret_cast<std::deque<std::string>*>(dq)->push_back(std::string());
					}
					else
					{						
						if (len >= 0)
							reinterpret_cast<std::deque<std::string>*>(dq)->push_back(std::string(value, len));
						else
							reinterpret_cast<std::deque<std::string>*>(dq)->push_back(std::string(value));
					}
					return PILO_OK;
				},//13
				[](char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value, len, false, false);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_back(t);
					return PILO_OK;

				},//14				
			};
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, const char* value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushback_ccharptr_func[vt](dq, value, len, adopt);				
			}


			typedef ::pilo::err_t(*handler_deque_pushback_stdstr_func_type)(char* dq, const std::string&, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushback_stdstr_func_type stc_handler_deque_pushback_stdstr_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , const std::string &, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, const std::string & value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_back((::pilo::i8_t)v);
					return PILO_OK;
				},//1
				[](char* dq, const std::string & value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_back((::pilo::u8_t)v);
					return PILO_OK;
				},//2
				[](char* dq, const std::string & value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_back((::pilo::i16_t)v);
					return PILO_OK;
				},//3
				[](char* dq, const std::string & value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_back((::pilo::u16_t)v);
					return PILO_OK;
				},//4
				[](char* dq, const std::string & value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_back(v);
					return PILO_OK;
				},//5
				[](char* dq, const std::string & value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_back((::pilo::u32_t)v);
					return PILO_OK;
				},//6
				[](char* dq, const std::string & value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i64_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_back(v);
					return PILO_OK;
				},//7
				[](char* dq, const std::string & value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::i64_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_back((::pilo::u64_t)v);
					return PILO_OK;
				},//8
				[](char* dq, const std::string & value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					bool v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_bool(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<bool>*>(dq)->push_back(v);
					return PILO_OK;
				},//9
				[](char* dq, const std::string & value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::f32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_back(v);
					return PILO_OK;
				},//10
				[](char* dq, const std::string & value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::f64_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_back(v);
					return PILO_OK;
				},//11
				[](char* dq, const std::string & value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = ::pilo::core::string::duplicate(value.c_str(), value.size());
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<char*>*>(dq)->push_back(ret);
					return PILO_OK;
				},//12
				[](char* dq, const std::string & value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<std::string>*>(dq)->push_back(std::string(value));
					return PILO_OK;
				},//13
				[](char* dq, const std::string & value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_back(t);
					return PILO_OK;

				},//14				
			};
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, const std::string& value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushback_stdstr_func[vt](dq, value, len, adopt);
			}


			typedef ::pilo::err_t(*handler_deque_pushback_tlv_func_type)(char* dq, ::pilo::tlv*, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushback_tlv_func_type stc_handler_deque_pushback_tlv_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , ::pilo::tlv*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_back(0);
					else						
						reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_back(value->as_i8(&err));					
					return err;
				},
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_back(0);
					else
						reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_back(value->as_u8(&err));
					return err;
				},
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_back(0);
					else
						reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_back(value->as_i16(&err));
					return err;
				},
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_back(0);
					else
						reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_back(value->as_u16(&err));
					return err;
				},
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_back(0);
					else
						reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_back(value->as_i32(&err));
					return err;
				},
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_back(0);
					else
						reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_back(value->as_u32(&err));
					return err;
				},
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_back(0);
					else
						reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_back(value->as_i64(&err));
					return err;
				},
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_back(0);
					else
						reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_back(value->as_u64(&err));
					return err;
				},
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<bool>*>(dq)->push_back(false);
					else
						reinterpret_cast<std::deque<bool>*>(dq)->push_back(value->as_bool(&err));
					return err;
				},//9
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_back(0.0f);
					else
						reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_back(value->as_f32(&err));
					return err;
				},//10
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_back(0.0);
					else
						reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_back(value->as_f64(&err));
					return err;
				},//11
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<char*>*>(dq)->push_back(nullptr);
					else
						reinterpret_cast<std::deque<char*>*>(dq)->push_back(value->as_cstr_dup(&err, nullptr));
					return err;
				},//12
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<std::string>*>(dq)->push_back(std::string());
					else
					{
						std::string dupped_str = value->as_str(&err);						
						reinterpret_cast<std::deque<std::string>*>(dq)->push_back(dupped_str);
						return err;
					}
						
					return err;
				},//13
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool adopt) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_back(nullptr);
					else
					{
						if (adopt)
						{
							reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_back(value);
						}
						else
						{
							::pilo::tlv* dupped = value->clone();
							if (dupped == nullptr)
							{
								return ::pilo::mk_perr( PERR_INSUF_HEAP);
							}
							reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_back(dupped);
						}
					}						
					return err;

				},//14
			};
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, ::pilo::tlv* value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushback_tlv_func[vt](dq, value, len, adopt);
			}


			//push front
			typedef ::pilo::err_t(*handler_deque_pushfront_i8_func_type)(char* dq, ::pilo::i8_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushfront_i8_func_type stc_handler_deque_pushfront_i8_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , ::pilo::i8_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_front(value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_front((::pilo::u8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_front((::pilo::i16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_front((::pilo::u16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_front((::pilo::i32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_front((::pilo::u32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_front((::pilo::i64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_front((::pilo::u64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->push_front(value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_front((::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_front((::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(8);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 8, value);
					reinterpret_cast<std::deque<char*>*>(dq)->push_front(ret);
					return PILO_OK;
				},//12

				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[8] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->push_front(std::string(buffer));
					return PILO_OK;
				},//13
				[](char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_front(t);
						return PILO_OK;

				},//14


			};

			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, ::pilo::i8_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushfront_i8_func[vt](dq, value, len, adopt);
			}

			typedef ::pilo::err_t(*handler_deque_pushfront_u8_func_type)(char* dq, ::pilo::u8_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushfront_u8_func_type stc_handler_deque_pushfront_u8_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , ::pilo::u8_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_front((::pilo::i8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_front(value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_front((::pilo::i16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_front((::pilo::u16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_front((::pilo::i32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_front((::pilo::u32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_front((::pilo::i64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_front((::pilo::u64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->push_front(value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_front((::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_front((::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(8);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 8, value);
					reinterpret_cast<std::deque<char*>*>(dq)->push_front(ret);
					return PILO_OK;
				},//12

				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[8] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->push_front(std::string(buffer));
					return PILO_OK;
				},//13
				[](char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_front(t);
						return PILO_OK;

				},//14
			};
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, ::pilo::u8_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushfront_u8_func[vt](dq, value, len, adopt);
			}


			typedef ::pilo::err_t(*handler_deque_pushfront_i16_func_type)(char* dq, ::pilo::i16_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushfront_i16_func_type stc_handler_deque_pushfront_i16_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , ::pilo::i16_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_front((::pilo::i8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_front((::pilo::u8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_front(value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_front((::pilo::u16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_front((::pilo::i32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_front((::pilo::u32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_front((::pilo::i64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_front((::pilo::u64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->push_front(value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_front((::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_front((::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(16);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 16, value);
					reinterpret_cast<std::deque<char*>*>(dq)->push_front(ret);
					return PILO_OK;
				},//12

				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[16] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->push_front(std::string(buffer));
					return PILO_OK;
				},//13
				[](char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_front(t);
						return PILO_OK;

				},//14
			};
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, ::pilo::i16_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushfront_i16_func[vt](dq, value, len, adopt);
			}

			typedef ::pilo::err_t(*handler_deque_pushfront_u16_func_type)(char* dq, ::pilo::u16_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushfront_u16_func_type stc_handler_deque_pushfront_u16_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , ::pilo::u16_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_front((::pilo::i8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_front((::pilo::u8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_front((::pilo::i16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_front(value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_front((::pilo::i32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_front((::pilo::u32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_front((::pilo::i64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_front((::pilo::u64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->push_front(value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_front((::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_front((::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(16);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 16, value);
					reinterpret_cast<std::deque<char*>*>(dq)->push_front(ret);
					return PILO_OK;
				},//12

				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[16] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->push_front(std::string(buffer));
					return PILO_OK;
				},//13
				[](char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_front(t);
						return PILO_OK;

				},//14
			};
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, ::pilo::u16_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushfront_u16_func[vt](dq, value, len, adopt);
			}


			typedef ::pilo::err_t(*handler_deque_pushfront_i32_func_type)(char* dq, ::pilo::i32_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushfront_i32_func_type stc_handler_deque_pushfront_i32_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , ::pilo::i32_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_front((::pilo::i8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_front((::pilo::u8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_front((::pilo::i16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_front((::pilo::u16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_front(value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_front((::pilo::u32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_front((::pilo::i64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_front((::pilo::u64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->push_front(value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_front((::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_front((::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(16);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 16, value);
					reinterpret_cast<std::deque<char*>*>(dq)->push_front(ret);
					return PILO_OK;
				},//12

				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[16] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->push_front(std::string(buffer));
					return PILO_OK;
				},//13
				[](char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_front(t);
						return PILO_OK;

				},//14
			};
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, ::pilo::i32_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushfront_i32_func[vt](dq, value, len, adopt);
			}


			typedef ::pilo::err_t(*handler_deque_pushfront_u32_func_type)(char* dq, ::pilo::u32_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushfront_u32_func_type stc_handler_deque_pushfront_u32_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , ::pilo::u32_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_front((::pilo::i8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_front((::pilo::u8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_front((::pilo::i16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_front((::pilo::u16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_front((::pilo::i32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_front(value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_front((::pilo::i64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_front((::pilo::u64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->push_front(value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_front((::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_front((::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(16);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 16, value);
					reinterpret_cast<std::deque<char*>*>(dq)->push_front(ret);
					return PILO_OK;
				},//12

				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[16] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->push_front(std::string(buffer));
					return PILO_OK;
				},//13
				[](char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_front(t);
						return PILO_OK;

				},//14
			};
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, ::pilo::u32_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushfront_u32_func[vt](dq, value, len, adopt);
			}


			typedef ::pilo::err_t(*handler_deque_pushfront_i64_func_type)(char* dq, ::pilo::i64_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushfront_i64_func_type stc_handler_deque_pushfront_i64_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , ::pilo::i64_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_front((::pilo::i8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_front((::pilo::u8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_front((::pilo::i16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_front((::pilo::u16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_front((::pilo::i32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_front((::pilo::u32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_front(value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_front((::pilo::u64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->push_front(value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_front((::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_front((::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(32);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 32, value);
					reinterpret_cast<std::deque<char*>*>(dq)->push_front(ret);
					return PILO_OK;
				},//12

				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[32] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->push_front(std::string(buffer));
					return PILO_OK;
				},//13
				[](char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_front(t);
						return PILO_OK;

				},//14				
			};
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, ::pilo::i64_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushfront_i64_func[vt](dq, value, len, adopt);
			}

			typedef ::pilo::err_t(*handler_deque_pushfront_u64_func_type)(char* dq, ::pilo::u64_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushfront_u64_func_type stc_handler_deque_pushfront_u64_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , ::pilo::u64_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_front((::pilo::i8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_front((::pilo::u8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_front((::pilo::i16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_front((::pilo::u16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_front((::pilo::i32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_front((::pilo::u32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_front((::pilo::i64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_front(value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->push_front(value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_front((::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_front((::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(32);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 32, value);
					reinterpret_cast<std::deque<char*>*>(dq)->push_front(ret);
					return PILO_OK;
				},//12

				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[32] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->push_front(std::string(buffer));
					return PILO_OK;
				},//13
				[](char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_front(t);
						return PILO_OK;

				},//14				
			};
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, ::pilo::u64_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushfront_u64_func[vt](dq, value, len, adopt);
			}


			typedef ::pilo::err_t(*handler_deque_pushfront_bool_func_type)(char* dq, bool, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushfront_bool_func_type stc_handler_deque_pushfront_bool_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , bool, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_front((::pilo::i8_t)value);
					return PILO_OK;
				},
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_front((::pilo::u8_t)value);
					return PILO_OK;
				},
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_front((::pilo::i16_t)value);
					return PILO_OK;
				},
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_front((::pilo::u16_t)value);
					return PILO_OK;
				},
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_front((::pilo::i32_t)value);
					return PILO_OK;
				},
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_front((::pilo::u32_t)value);
					return PILO_OK;
				},
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_front((::pilo::i64_t)value);
					return PILO_OK;
				},
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_front((::pilo::u64_t)value);
					return PILO_OK;
				},
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->push_front(value);
					return PILO_OK;
				},//9
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_front((::pilo::f32_t)value ? 1.0f : 0.0f);
					return PILO_OK;
				},//10
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_front((::pilo::f64_t)value ? 1.0 : 0.0);
					return PILO_OK;
				},//11
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = nullptr;
					if (value)
						ret = ::pilo::core::string::duplicate("true", 4);
					else
						ret = ::pilo::core::string::duplicate("false", 5);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<char*>*>(dq)->push_front(ret);
					return PILO_OK;
				},//12

				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					if (value)
						reinterpret_cast<std::deque<std::string>*>(dq)->push_front(std::string("true"));
					else
						reinterpret_cast<std::deque<std::string>*>(dq)->push_front(std::string("false"));
					return PILO_OK;
				},//13
				[](char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_front(t);
						return PILO_OK;

				},//14				
			};
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, bool value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushfront_bool_func[vt](dq, value, len, adopt);
			}


			typedef ::pilo::err_t(*handler_deque_pushfront_f32_func_type)(char* dq, ::pilo::f32_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushfront_f32_func_type stc_handler_deque_pushfront_f32_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , ::pilo::f32_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_front((::pilo::i8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_front((::pilo::u8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_front((::pilo::i16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_front((::pilo::u16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_front((::pilo::i32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_front((::pilo::u32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_front((::pilo::i64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_front((::pilo::u64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->push_front(value == 0.0f ? false : true);
					return PILO_OK;
				},//9
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_front(value);
					return PILO_OK;
				},//10
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_front((::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(32);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 32, value);
					reinterpret_cast<std::deque<char*>*>(dq)->push_front(ret);
					return PILO_OK;
				},//12

				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[32] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->push_front(std::string(buffer));
					return PILO_OK;
				},//13
				[](char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_front(t);
						return PILO_OK;

				},//14
			};
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, ::pilo::f32_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushfront_f32_func[vt](dq, value, len, adopt);
			}

			typedef ::pilo::err_t(*handler_deque_pushfront_f64_func_type)(char* dq, ::pilo::f64_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushfront_f64_func_type stc_handler_deque_pushfront_f64_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , ::pilo::f64_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_front((::pilo::i8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_front((::pilo::u8_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_front((::pilo::i16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_front((::pilo::u16_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_front((::pilo::i32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_front((::pilo::u32_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_front((::pilo::i64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_front((::pilo::u64_t)value);
					return PILO_OK;
				},
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->push_front(value == 0.0 ? false : true);
					return PILO_OK;
				},//9
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_front((::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_front(value);
					return PILO_OK;
				},//11
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(32);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 32, value);
					reinterpret_cast<std::deque<char*>*>(dq)->push_front(ret);
					return PILO_OK;
				},//12

				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[32] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->push_front(std::string(buffer));
					return PILO_OK;
				},//13
				[](char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_front(t);
						return PILO_OK;

				},//14				
			};
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, ::pilo::f64_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushfront_f64_func[vt](dq, value, len, adopt);
			}

			typedef ::pilo::err_t(*handler_deque_pushfront_ccharptr_func_type)(char* dq, const char*, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushfront_ccharptr_func_type stc_handler_deque_pushfront_ccharptr_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , const char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_front((::pilo::i8_t)v);
					return PILO_OK;
				},//1
				[](char* dq, const char* value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_front((::pilo::u8_t)v);
					return PILO_OK;
				},//2
				[](char* dq, const char* value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_front((::pilo::i16_t)v);
					return PILO_OK;
				},//3
				[](char* dq, const char* value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_front((::pilo::u16_t)v);
					return PILO_OK;
				},//4
				[](char* dq, const char* value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_front(v);
					return PILO_OK;
				},//5
				[](char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_front((::pilo::u32_t)v);
					return PILO_OK;
				},//6
				[](char* dq, const char* value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i64_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_front(v);
					return PILO_OK;
				},//7
				[](char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::i64_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_front((::pilo::u64_t)v);
					return PILO_OK;
				},//8
				[](char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					bool v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_bool(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<bool>*>(dq)->push_front(v);
					return PILO_OK;
				},//9
				[](char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::f32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_front(v);
					return PILO_OK;
				},//10
				[](char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::f64_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_front(v);
					return PILO_OK;
				},//11
				[](char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					if (value == nullptr)
					{
						reinterpret_cast<std::deque<char*>*>(dq)->push_front(nullptr);
					}
					else
					{
						char* ret = ::pilo::core::string::duplicate(value, len);
						if (ret == nullptr)
							return ::pilo::mk_perr( PERR_INSUF_HEAP);
						reinterpret_cast<std::deque<char*>*>(dq)->push_front(ret);
					}
					return PILO_OK;
				},//12
				[](char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					if (value == nullptr)
					{
						reinterpret_cast<std::deque<std::string>*>(dq)->push_front(std::string());
					}
					else
					{
						if (len >= 0)
							reinterpret_cast<std::deque<std::string>*>(dq)->push_front(std::string(value, len));
						else
							reinterpret_cast<std::deque<std::string>*>(dq)->push_front(std::string(value));
					}
					return PILO_OK;
				},//13
				[](char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value, len, false, false);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_front(t);
					return PILO_OK;

				},//14				
			};
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, const char* value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushfront_ccharptr_func[vt](dq, value, len, adopt);
			}


			typedef ::pilo::err_t(*handler_deque_pushfront_stdstr_func_type)(char* dq, const std::string&, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushfront_stdstr_func_type stc_handler_deque_pushfront_stdstr_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , const std::string&, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, const std::string& value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_front((::pilo::i8_t)v);
					return PILO_OK;
				},//1
				[](char* dq, const std::string& value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_front((::pilo::u8_t)v);
					return PILO_OK;
				},//2
				[](char* dq, const std::string& value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_front((::pilo::i16_t)v);
					return PILO_OK;
				},//3
				[](char* dq, const std::string& value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_front((::pilo::u16_t)v);
					return PILO_OK;
				},//4
				[](char* dq, const std::string& value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_front(v);
					return PILO_OK;
				},//5
				[](char* dq, const std::string& value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_front((::pilo::u32_t)v);
					return PILO_OK;
				},//6
				[](char* dq, const std::string& value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i64_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_front(v);
					return PILO_OK;
				},//7
				[](char* dq, const std::string& value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::i64_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_front((::pilo::u64_t)v);
					return PILO_OK;
				},//8
				[](char* dq, const std::string& value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					bool v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_bool(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<bool>*>(dq)->push_front(v);
					return PILO_OK;
				},//9
				[](char* dq, const std::string& value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::f32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_front(v);
					return PILO_OK;
				},//10
				[](char* dq, const std::string& value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::f64_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_front(v);
					return PILO_OK;
				},//11
				[](char* dq, const std::string& value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = ::pilo::core::string::duplicate(value.c_str(), value.size());
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<char*>*>(dq)->push_front(ret);
					return PILO_OK;
				},//12
				[](char* dq, const std::string& value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<std::string>*>(dq)->push_front(std::string(value));
					return PILO_OK;
				},//13
				[](char* dq, const std::string& value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_front(t);
					return PILO_OK;

				},//14				
			};
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, const std::string& value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushfront_stdstr_func[vt](dq, value, len, adopt);
			}


			typedef ::pilo::err_t(*handler_deque_pushfront_tlv_func_type)(char* dq, ::pilo::tlv*, ::pilo::i32_t len, bool adopt);
			static handler_deque_pushfront_tlv_func_type stc_handler_deque_pushfront_tlv_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](char* , ::pilo::tlv*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_front(0);
					else
						reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->push_front(value->as_i8(&err));
					return err;
				},
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_front(0);
					else
						reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->push_front(value->as_u8(&err));
					return err;
				},
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_front(0);
					else
						reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->push_front(value->as_i16(&err));
					return err;
				},
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_front(0);
					else
						reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->push_front(value->as_u16(&err));
					return err;
				},
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_front(0);
					else
						reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->push_front(value->as_i32(&err));
					return err;
				},
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_front(0);
					else
						reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->push_front(value->as_u32(&err));
					return err;
				},
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_front(0);
					else
						reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->push_front(value->as_i64(&err));
					return err;
				},
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_front(0);
					else
						reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->push_front(value->as_u64(&err));
					return err;
				},
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<bool>*>(dq)->push_front(false);
					else
						reinterpret_cast<std::deque<bool>*>(dq)->push_front(value->as_bool(&err));
					return err;
				},//9
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_front(0.0f);
					else
						reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->push_front(value->as_f32(&err));
					return err;
				},//10
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_front(0.0);
					else
						reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->push_front(value->as_f64(&err));
					return err;
				},//11
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<char*>*>(dq)->push_front(nullptr);
					else
						reinterpret_cast<std::deque<char*>*>(dq)->push_front(value->as_cstr_dup(&err, nullptr));
					return err;
				},//12
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<std::string>*>(dq)->push_front(std::string());
					else
					{
						std::string dupped_str = value->as_str(&err);
						reinterpret_cast<std::deque<std::string>*>(dq)->push_front(dupped_str);
						return err;
					}

					return err;
				},//13
				[](char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool adopt) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_front(nullptr);
					else
					{
						if (adopt)
						{
							reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_front(value);
						}
						else
						{
							::pilo::tlv* dupped = value->clone();
							if (dupped == nullptr)
							{
								return ::pilo::mk_perr( PERR_INSUF_HEAP);
							}
							reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->push_front(dupped);
						}
					}
					return err;

				},//14
			};
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, ::pilo::tlv* value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_pushfront_tlv_func[vt](dq, value, len, adopt);
			}

			//insert
			typedef ::pilo::err_t(*handler_deque_insert_i8_func_type)(::pilo::i64_t idx, char* dq, ::pilo::i8_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_insert_i8_func_type stc_handler_deque_insert_i8_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](::pilo::i64_t, char* , ::pilo::i8_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](::pilo::i64_t idx, char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i8_t>::size_type>(idx),value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx),(::pilo::u8_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i16_t>::size_type>(idx), (::pilo::i16_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx), (::pilo::u16_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i32_t>::size_type>(idx), (::pilo::i32_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u32_t>::size_type>(idx), (::pilo::u32_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i64_t>::size_type>(idx), (::pilo::i64_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u64_t>::size_type>(idx), (::pilo::u64_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->insert(reinterpret_cast<std::deque<bool>*>(dq)->begin() + static_cast<std::deque<bool>::size_type>(idx),  value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](::pilo::i64_t idx, char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f32_t>::size_type>(idx), (::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](::pilo::i64_t idx, char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f64_t>::size_type>(idx), (::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](::pilo::i64_t idx, char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(8);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 8, value);
					reinterpret_cast<std::deque<char*>*>(dq)->insert(reinterpret_cast<std::deque<char*>*>(dq)->begin() + static_cast<std::deque<char*>::size_type>(idx), (char*)ret);
					return PILO_OK;
				},//12

				[](::pilo::i64_t idx, char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[8] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->insert(reinterpret_cast<std::deque<std::string>*>(dq)->begin() + static_cast<std::deque<std::string>::size_type>(idx), std::string(buffer));
					return PILO_OK;
				},//13
				[](::pilo::i64_t idx, char* dq, ::pilo::i8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->begin() + static_cast<std::deque<::pilo::tlv*>::size_type>(idx), t);
						return PILO_OK;

				},//14
			};
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, ::pilo::i8_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_insert_i8_func[vt](idx, dq, value, len, adopt);
			}

			typedef ::pilo::err_t(*handler_deque_insert_u8_func_type)(::pilo::i64_t idx, char* dq, ::pilo::u8_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_insert_u8_func_type stc_handler_deque_insert_u8_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](::pilo::i64_t, char* , ::pilo::u8_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](::pilo::i64_t idx, char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i8_t>::size_type>(idx),(::pilo::i8_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx),value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i16_t>::size_type>(idx), (::pilo::i16_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx), (::pilo::u16_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i32_t>::size_type>(idx), (::pilo::i32_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u32_t>::size_type>(idx), (::pilo::u32_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i64_t>::size_type>(idx), (::pilo::i64_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u64_t>::size_type>(idx), (::pilo::u64_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->insert(reinterpret_cast<std::deque<bool>*>(dq)->begin() + static_cast<std::deque<bool>::size_type>(idx),  value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](::pilo::i64_t idx, char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f32_t>::size_type>(idx), (::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](::pilo::i64_t idx, char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f64_t>::size_type>(idx), (::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](::pilo::i64_t idx, char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(8);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 8, value);
					reinterpret_cast<std::deque<char*>*>(dq)->insert(reinterpret_cast<std::deque<char*>*>(dq)->begin() + static_cast<std::deque<char*>::size_type>(idx), (char*)ret);
					return PILO_OK;
				},//12

				[](::pilo::i64_t idx, char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[8] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->insert(reinterpret_cast<std::deque<std::string>*>(dq)->begin() + static_cast<std::deque<std::string>::size_type>(idx), std::string(buffer));
					return PILO_OK;
				},//13
				[](::pilo::i64_t idx, char* dq, ::pilo::u8_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->begin() + static_cast<std::deque<::pilo::tlv*>::size_type>(idx), t);
						return PILO_OK;

				},//14
			};
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, ::pilo::u8_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_insert_u8_func[vt](idx, dq, value, len, adopt);
			}

			typedef ::pilo::err_t(*handler_deque_insert_i16_func_type)(::pilo::i64_t idx, char* dq, ::pilo::i16_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_insert_i16_func_type stc_handler_deque_insert_i16_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](::pilo::i64_t, char* , ::pilo::i16_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](::pilo::i64_t idx, char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i8_t>::size_type>(idx),(::pilo::u8_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx),(::pilo::u8_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i16_t>::size_type>(idx), value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx), (::pilo::u16_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i32_t>::size_type>(idx), (::pilo::i32_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u32_t>::size_type>(idx), (::pilo::u32_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i64_t>::size_type>(idx), (::pilo::i64_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u64_t>::size_type>(idx), (::pilo::u64_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->insert(reinterpret_cast<std::deque<bool>*>(dq)->begin() + static_cast<std::deque<bool>::size_type>(idx),  value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](::pilo::i64_t idx, char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f32_t>::size_type>(idx), (::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](::pilo::i64_t idx, char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f64_t>::size_type>(idx), (::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](::pilo::i64_t idx, char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(16);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 16, value);
					reinterpret_cast<std::deque<char*>*>(dq)->insert(reinterpret_cast<std::deque<char*>*>(dq)->begin() + static_cast<std::deque<char*>::size_type>(idx), (char*)ret);
					return PILO_OK;
				},//12

				[](::pilo::i64_t idx, char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[16] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->insert(reinterpret_cast<std::deque<std::string>*>(dq)->begin() + static_cast<std::deque<std::string>::size_type>(idx), std::string(buffer));
					return PILO_OK;
				},//13
				[](::pilo::i64_t idx, char* dq, ::pilo::i16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->begin() + static_cast<std::deque<::pilo::tlv*>::size_type>(idx), t);
						return PILO_OK;

				},//14
			};
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, ::pilo::i16_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_insert_i16_func[vt](idx, dq, value, len, adopt);
			}

			typedef ::pilo::err_t(*handler_deque_insert_u16_func_type)(::pilo::i64_t idx, char* dq, ::pilo::u16_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_insert_u16_func_type stc_handler_deque_insert_u16_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](::pilo::i64_t, char* , ::pilo::u16_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](::pilo::i64_t idx, char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i8_t>::size_type>(idx),(::pilo::u8_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx),(::pilo::u8_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i16_t>::size_type>(idx), (::pilo::i16_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx), value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i32_t>::size_type>(idx), (::pilo::i32_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u32_t>::size_type>(idx), (::pilo::u32_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i64_t>::size_type>(idx), (::pilo::i64_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u64_t>::size_type>(idx), (::pilo::u64_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->insert(reinterpret_cast<std::deque<bool>*>(dq)->begin() + static_cast<std::deque<bool>::size_type>(idx),  value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](::pilo::i64_t idx, char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f32_t>::size_type>(idx), (::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](::pilo::i64_t idx, char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f64_t>::size_type>(idx), (::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](::pilo::i64_t idx, char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(16);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 16, value);
					reinterpret_cast<std::deque<char*>*>(dq)->insert(reinterpret_cast<std::deque<char*>*>(dq)->begin() + static_cast<std::deque<char*>::size_type>(idx), (char*)ret);
					return PILO_OK;
				},//12

				[](::pilo::i64_t idx, char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[16] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->insert(reinterpret_cast<std::deque<std::string>*>(dq)->begin() + static_cast<std::deque<std::string>::size_type>(idx), std::string(buffer));
					return PILO_OK;
				},//13
				[](::pilo::i64_t idx, char* dq, ::pilo::u16_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->begin() + static_cast<std::deque<::pilo::tlv*>::size_type>(idx), t);
						return PILO_OK;

				},//14
			};
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, ::pilo::u16_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_insert_u16_func[vt](idx, dq, value, len, adopt);
			}

			typedef ::pilo::err_t(*handler_deque_insert_i32_func_type)(::pilo::i64_t idx, char* dq, ::pilo::i32_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_insert_i32_func_type stc_handler_deque_insert_i32_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](::pilo::i64_t, char* , ::pilo::i32_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](::pilo::i64_t idx, char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i8_t>::size_type>(idx),(::pilo::u8_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx),(::pilo::u8_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i16_t>::size_type>(idx), (::pilo::i16_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx), (::pilo::u16_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i32_t>::size_type>(idx), (::pilo::i32_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u32_t>::size_type>(idx), (::pilo::u32_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i64_t>::size_type>(idx), (::pilo::i64_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u64_t>::size_type>(idx), (::pilo::u64_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->insert(reinterpret_cast<std::deque<bool>*>(dq)->begin() + static_cast<std::deque<bool>::size_type>(idx),  value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](::pilo::i64_t idx, char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f32_t>::size_type>(idx), (::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](::pilo::i64_t idx, char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f64_t>::size_type>(idx), (::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](::pilo::i64_t idx, char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(16);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 16, value);
					reinterpret_cast<std::deque<char*>*>(dq)->insert(reinterpret_cast<std::deque<char*>*>(dq)->begin() + static_cast<std::deque<char*>::size_type>(idx), (char*)ret);
					return PILO_OK;
				},//12

				[](::pilo::i64_t idx, char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[16] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->insert(reinterpret_cast<std::deque<std::string>*>(dq)->begin() + static_cast<std::deque<std::string>::size_type>(idx), std::string(buffer));
					return PILO_OK;
				},//13
				[](::pilo::i64_t idx, char* dq, ::pilo::i32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->begin() + static_cast<std::deque<::pilo::tlv*>::size_type>(idx), t);
						return PILO_OK;

				},//14
			};
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, ::pilo::i32_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_insert_i32_func[vt](idx, dq, value, len, adopt);
			}

			typedef ::pilo::err_t(*handler_deque_insert_u32_func_type)(::pilo::i64_t idx, char* dq, ::pilo::u32_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_insert_u32_func_type stc_handler_deque_insert_u32_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](::pilo::i64_t, char* , ::pilo::u32_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](::pilo::i64_t idx, char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i8_t>::size_type>(idx),(::pilo::u8_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx),(::pilo::u8_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i16_t>::size_type>(idx), (::pilo::i16_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx), (::pilo::u16_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i32_t>::size_type>(idx), (::pilo::i32_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u32_t>::size_type>(idx), (::pilo::u32_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i64_t>::size_type>(idx), (::pilo::i64_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u64_t>::size_type>(idx), (::pilo::u64_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->insert(reinterpret_cast<std::deque<bool>*>(dq)->begin() + static_cast<std::deque<bool>::size_type>(idx),  value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](::pilo::i64_t idx, char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f32_t>::size_type>(idx), (::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](::pilo::i64_t idx, char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f64_t>::size_type>(idx), (::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](::pilo::i64_t idx, char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(16);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 16, value);
					reinterpret_cast<std::deque<char*>*>(dq)->insert(reinterpret_cast<std::deque<char*>*>(dq)->begin() + static_cast<std::deque<char*>::size_type>(idx), (char*)ret);
					return PILO_OK;
				},//12

				[](::pilo::i64_t idx, char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[16] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->insert(reinterpret_cast<std::deque<std::string>*>(dq)->begin() + static_cast<std::deque<std::string>::size_type>(idx), std::string(buffer));
					return PILO_OK;
				},//13
				[](::pilo::i64_t idx, char* dq, ::pilo::u32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->begin() + static_cast<std::deque<::pilo::tlv*>::size_type>(idx), t);
						return PILO_OK;

				},//14
			};
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, ::pilo::u32_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_insert_u32_func[vt](idx, dq, value, len, adopt);
			}

			typedef ::pilo::err_t(*handler_deque_insert_i64_func_type)(::pilo::i64_t idx, char* dq, ::pilo::i64_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_insert_i64_func_type stc_handler_deque_insert_i64_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](::pilo::i64_t, char* , ::pilo::i64_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](::pilo::i64_t idx, char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i8_t>::size_type>(idx),(::pilo::u8_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx),(::pilo::u8_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i16_t>::size_type>(idx), (::pilo::i16_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx), (::pilo::u16_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i32_t>::size_type>(idx), (::pilo::i32_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u32_t>::size_type>(idx), (::pilo::u32_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i64_t>::size_type>(idx), (::pilo::i64_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u64_t>::size_type>(idx), (::pilo::u64_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->insert(reinterpret_cast<std::deque<bool>*>(dq)->begin() + static_cast<std::deque<bool>::size_type>(idx),  value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](::pilo::i64_t idx, char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f32_t>::size_type>(idx), (::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](::pilo::i64_t idx, char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f64_t>::size_type>(idx), (::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](::pilo::i64_t idx, char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(32);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 32, value);
					reinterpret_cast<std::deque<char*>*>(dq)->insert(reinterpret_cast<std::deque<char*>*>(dq)->begin() + static_cast<std::deque<char*>::size_type>(idx), (char*)ret);
					return PILO_OK;
				},//12

				[](::pilo::i64_t idx, char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[32] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->insert(reinterpret_cast<std::deque<std::string>*>(dq)->begin() + static_cast<std::deque<std::string>::size_type>(idx), std::string(buffer));
					return PILO_OK;
				},//13
				[](::pilo::i64_t idx, char* dq, ::pilo::i64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->begin() + static_cast<std::deque<::pilo::tlv*>::size_type>(idx), t);
						return PILO_OK;

				},//14
			};
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, ::pilo::i64_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_insert_i64_func[vt](idx, dq, value, len, adopt);
			}

			typedef ::pilo::err_t(*handler_deque_insert_u64_func_type)(::pilo::i64_t idx, char* dq, ::pilo::u64_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_insert_u64_func_type stc_handler_deque_insert_u64_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](::pilo::i64_t, char* , ::pilo::u64_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](::pilo::i64_t idx, char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i8_t>::size_type>(idx),(::pilo::u8_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx),(::pilo::u8_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i16_t>::size_type>(idx), (::pilo::i16_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx), (::pilo::u16_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i32_t>::size_type>(idx), (::pilo::i32_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u32_t>::size_type>(idx), (::pilo::u32_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i64_t>::size_type>(idx), (::pilo::i64_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u64_t>::size_type>(idx), (::pilo::u64_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->insert(reinterpret_cast<std::deque<bool>*>(dq)->begin() + static_cast<std::deque<bool>::size_type>(idx),  value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](::pilo::i64_t idx, char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f32_t>::size_type>(idx), (::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](::pilo::i64_t idx, char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f64_t>::size_type>(idx), (::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](::pilo::i64_t idx, char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(32);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 32, value);
					reinterpret_cast<std::deque<char*>*>(dq)->insert(reinterpret_cast<std::deque<char*>*>(dq)->begin() + static_cast<std::deque<char*>::size_type>(idx), (char*)ret);
					return PILO_OK;
				},//12

				[](::pilo::i64_t idx, char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[32] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->insert(reinterpret_cast<std::deque<std::string>*>(dq)->begin() + static_cast<std::deque<std::string>::size_type>(idx), std::string(buffer));
					return PILO_OK;
				},//13
				[](::pilo::i64_t idx, char* dq, ::pilo::u64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->begin() + static_cast<std::deque<::pilo::tlv*>::size_type>(idx), t);
						return PILO_OK;

				},//14
			};
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, ::pilo::u64_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_insert_u64_func[vt](idx, dq, value, len, adopt);
			}


			typedef ::pilo::err_t(*handler_deque_insert_bool_func_type)(::pilo::i64_t idx, char* dq, bool, ::pilo::i32_t len, bool adopt);
			static handler_deque_insert_bool_func_type stc_handler_deque_insert_bool_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](::pilo::i64_t, char* , bool, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](::pilo::i64_t idx, char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i8_t>::size_type>(idx), (::pilo::i8_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx), (::pilo::u8_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i16_t>::size_type>(idx), (::pilo::i16_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u16_t>::size_type>(idx), (::pilo::u16_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i32_t>::size_type>(idx), (::pilo::i32_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u32_t>::size_type>(idx), (::pilo::u32_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i64_t>::size_type>(idx), (::pilo::i64_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u64_t>::size_type>(idx), (::pilo::i64_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->insert(reinterpret_cast<std::deque<bool>*>(dq)->begin() + static_cast<std::deque<bool>::size_type>(idx), (::pilo::i64_t)value);
					return PILO_OK;
				},//9
				[](::pilo::i64_t idx, char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f32_t>::size_type>(idx), (::pilo::f32_t)value ? 1.0f : 0.0f);
					return PILO_OK;
				},//10
				[](::pilo::i64_t idx, char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f64_t>::size_type>(idx), (::pilo::f64_t) value ? 1.0 : 0.0);
					return PILO_OK;
				},//11
				[](::pilo::i64_t idx, char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = nullptr;
					if (value)
						ret = ::pilo::core::string::duplicate("true", 4);
					else
						ret = ::pilo::core::string::duplicate("false", 5);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<char*>*>(dq)->insert(reinterpret_cast<std::deque<char*>*>(dq)->begin() + static_cast<std::deque<char*>::size_type>(idx), (char*)ret);
					return PILO_OK;
				},//12

				[](::pilo::i64_t idx, char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					if (value)	
						reinterpret_cast<std::deque<std::string>*>(dq)->insert(reinterpret_cast<std::deque<std::string>*>(dq)->begin() + static_cast<std::deque<std::string>::size_type>(idx), std::string("true"));
					else
						reinterpret_cast<std::deque<std::string>*>(dq)->insert(reinterpret_cast<std::deque<std::string>*>(dq)->begin() + static_cast<std::deque<std::string>::size_type>(idx), std::string("false"));
					return PILO_OK;
				},//13
				[](::pilo::i64_t idx, char* dq, bool value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->begin() + static_cast<std::deque<::pilo::tlv*>::size_type>(idx), t);
					return PILO_OK;

				},//14				
			};
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, bool value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_insert_bool_func[vt](idx, dq, value, len, adopt);
			}

			typedef ::pilo::err_t(*handler_deque_insert_f32_func_type)(::pilo::i64_t idx, char* dq, ::pilo::f32_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_insert_f32_func_type stc_handler_deque_insert_f32_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](::pilo::i64_t, char* , ::pilo::f32_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](::pilo::i64_t idx, char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i8_t>::size_type>(idx),(::pilo::u8_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx),(::pilo::u8_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i16_t>::size_type>(idx), (::pilo::i16_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx), (::pilo::u16_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i32_t>::size_type>(idx), (::pilo::i32_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u32_t>::size_type>(idx), (::pilo::u32_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i64_t>::size_type>(idx), (::pilo::i64_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u64_t>::size_type>(idx), (::pilo::u64_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->insert(reinterpret_cast<std::deque<bool>*>(dq)->begin() + static_cast<std::deque<bool>::size_type>(idx),  value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](::pilo::i64_t idx, char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f32_t>::size_type>(idx), (::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](::pilo::i64_t idx, char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f64_t>::size_type>(idx), (::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](::pilo::i64_t idx, char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(32);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 32, value);
					reinterpret_cast<std::deque<char*>*>(dq)->insert(reinterpret_cast<std::deque<char*>*>(dq)->begin() + static_cast<std::deque<char*>::size_type>(idx), (char*)ret);
					return PILO_OK;
				},//12
				[](::pilo::i64_t idx, char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[32] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->insert(reinterpret_cast<std::deque<std::string>*>(dq)->begin() + static_cast<std::deque<std::string>::size_type>(idx), std::string(buffer));
					return PILO_OK;
				},//13
				[](::pilo::i64_t idx, char* dq, ::pilo::f32_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->begin() + static_cast<std::deque<::pilo::tlv*>::size_type>(idx), t);
						return PILO_OK;

				},//14
			};
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, ::pilo::f32_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_insert_f32_func[vt](idx, dq, value, len, adopt);
			}

			typedef ::pilo::err_t(*handler_deque_insert_f64_func_type)(::pilo::i64_t idx, char* dq, ::pilo::f64_t, ::pilo::i32_t len, bool adopt);
			static handler_deque_insert_f64_func_type stc_handler_deque_insert_f64_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](::pilo::i64_t, char* , ::pilo::f64_t, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](::pilo::i64_t idx, char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i8_t>::size_type>(idx),(::pilo::u8_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx),(::pilo::u8_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i16_t>::size_type>(idx), (::pilo::i16_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx), (::pilo::u16_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i32_t>::size_type>(idx), (::pilo::i32_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u32_t>::size_type>(idx), (::pilo::u32_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i64_t>::size_type>(idx), (::pilo::i64_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u64_t>::size_type>(idx), (::pilo::u64_t)value);
					return PILO_OK;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<bool>*>(dq)->insert(reinterpret_cast<std::deque<bool>*>(dq)->begin() + static_cast<std::deque<bool>::size_type>(idx),  value == 0 ? false : true);
					return PILO_OK;
				},//9
				[](::pilo::i64_t idx, char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f32_t>::size_type>(idx), (::pilo::f32_t)value);
					return PILO_OK;
				},//10
				[](::pilo::i64_t idx, char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f64_t>::size_type>(idx), (::pilo::f64_t)value);
					return PILO_OK;
				},//11
				[](::pilo::i64_t idx, char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = (char*)PMF_HEAP_MALLOC(32);
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					::pilo::core::string::number_to_string(ret, 32, value);
					reinterpret_cast<std::deque<char*>*>(dq)->insert(reinterpret_cast<std::deque<char*>*>(dq)->begin() + static_cast<std::deque<char*>::size_type>(idx), (char*)ret);
					return PILO_OK;
				},//12
				[](::pilo::i64_t idx, char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char buffer[32] = {0};
					::pilo::core::string::number_to_string(buffer, sizeof(buffer), value);
					reinterpret_cast<std::deque<std::string>*>(dq)->insert(reinterpret_cast<std::deque<std::string>*>(dq)->begin() + static_cast<std::deque<std::string>::size_type>(idx), std::string(buffer));
					return PILO_OK;
				},//13
				[](::pilo::i64_t idx, char* dq, ::pilo::f64_t value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->begin() + static_cast<std::deque<::pilo::tlv*>::size_type>(idx), t);
						return PILO_OK;

				},//14
			};
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, ::pilo::f64_t value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_insert_f64_func[vt](idx, dq, value, len, adopt);
			}



			typedef ::pilo::err_t(*handler_deque_insert_ccharptr_func_type)(::pilo::i64_t idx, char* dq, const char*, ::pilo::i32_t len, bool adopt);
			static handler_deque_insert_ccharptr_func_type stc_handler_deque_insert_ccharptr_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](::pilo::i64_t, char* , const char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](::pilo::i64_t idx, char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i8_t>::size_type>(idx), (::pilo::i8_t)v);
					return PILO_OK;
				},//1
				[](::pilo::i64_t idx, char* dq, const char* value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx), (::pilo::u8_t)v);
					return PILO_OK;
				},//2
				[](::pilo::i64_t idx, char* dq, const char* value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i16_t>::size_type>(idx), (::pilo::i16_t)v);
					return PILO_OK;
				},//3
				[](::pilo::i64_t idx, char* dq, const char* value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u16_t>::size_type>(idx), (::pilo::u16_t)v);
					return PILO_OK;
				},//4
				[](::pilo::i64_t idx, char* dq, const char* value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i32_t>::size_type>(idx), (::pilo::i32_t)v);
					return PILO_OK;
				},//5
				[](::pilo::i64_t idx, char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u32_t>::size_type>(idx), (::pilo::u32_t)v);
					return PILO_OK;
				},//6
				[](::pilo::i64_t idx, char* dq, const char* value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i64_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i64_t>::size_type>(idx), (::pilo::i64_t)v);
					return PILO_OK;
				},//7
				[](::pilo::i64_t idx, char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::i64_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u64_t>::size_type>(idx), (::pilo::u64_t)v);
					return PILO_OK;
				},//8
				[](::pilo::i64_t idx, char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					bool v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_bool(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<bool>*>(dq)->insert(reinterpret_cast<std::deque<bool>*>(dq)->begin() + static_cast<std::deque<bool>::size_type>(idx), v);
					return PILO_OK;
				},//9
				[](::pilo::i64_t idx, char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::f32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f32_t>::size_type>(idx), v);
					return PILO_OK;
				},//10
				[](::pilo::i64_t idx, char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::f64_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value, len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f64_t>::size_type>(idx), v);
					return PILO_OK;
				},//11
				[](::pilo::i64_t idx, char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					if (value == nullptr)
					{
						reinterpret_cast<std::deque<char*>*>(dq)->insert(reinterpret_cast<std::deque<char*>*>(dq)->begin() + static_cast<std::deque<char*>::size_type>(idx), nullptr);
					}
					else
					{
						char* ret = ::pilo::core::string::duplicate(value, len);
						if (ret == nullptr)
							return ::pilo::mk_perr( PERR_INSUF_HEAP);
						reinterpret_cast<std::deque<char*>*>(dq)->insert(reinterpret_cast<std::deque<char*>*>(dq)->begin() + static_cast<std::deque<char*>::size_type>(idx), (char*)ret);
					}
					return PILO_OK;
				},//12
				[](::pilo::i64_t idx, char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					if (value == nullptr)
					{
						reinterpret_cast<std::deque<std::string>*>(dq)->insert(reinterpret_cast<std::deque<std::string>*>(dq)->begin() + static_cast<std::deque<std::string>::size_type>(idx), std::string());
					}
					else
					{
						if (len >= 0)
							reinterpret_cast<std::deque<std::string>*>(dq)->insert(reinterpret_cast<std::deque<std::string>*>(dq)->begin() + static_cast<std::deque<std::string>::size_type>(idx), std::string(value, len));
						else
							reinterpret_cast<std::deque<std::string>*>(dq)->insert(reinterpret_cast<std::deque<std::string>*>(dq)->begin() + static_cast<std::deque<std::string>::size_type>(idx), std::string(value));

					}
					return PILO_OK;
				},//13
				[](::pilo::i64_t idx, char* dq, const char* value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value, len, false, false);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->begin() + static_cast<std::deque<::pilo::tlv*>::size_type>(idx), t);

					return PILO_OK;

				},//14				
			};
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, const char* value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_insert_ccharptr_func[vt](idx, dq, value, len, adopt);
			}

			typedef ::pilo::err_t(*handler_deque_insert_stdstr_func_type)(::pilo::i64_t idx, char* dq, const std::string&, ::pilo::i32_t len, bool adopt);
			static handler_deque_insert_stdstr_func_type stc_handler_deque_insert_stdstr_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](::pilo::i64_t, char* , const std::string&, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](::pilo::i64_t idx, char* dq, const std::string& value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i8_t>::size_type>(idx), (::pilo::i8_t)v);
					return PILO_OK;
				},//1
				[](::pilo::i64_t idx, char* dq, const std::string& value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx), (::pilo::u8_t)v);
					return PILO_OK;
				},//2
				[](::pilo::i64_t idx, char* dq, const std::string& value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i16_t>::size_type>(idx), (::pilo::i16_t)v);
					return PILO_OK;
				},//3
				[](::pilo::i64_t idx, char* dq, const std::string& value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u16_t>::size_type>(idx), (::pilo::u16_t)v);
					return PILO_OK;
				},//4
				[](::pilo::i64_t idx, char* dq, const std::string& value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i32_t>::size_type>(idx), (::pilo::i32_t)v);
					return PILO_OK;
				},//5
				[](::pilo::i64_t idx, char* dq, const std::string& value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::i32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u32_t>::size_type>(idx), (::pilo::u32_t)v);
					return PILO_OK;
				},//6
				[](::pilo::i64_t idx, char* dq, const std::string& value, ::pilo::i32_t len , bool) -> ::pilo::err_t
				{
					::pilo::i64_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i64_t>::size_type>(idx), (::pilo::i64_t)v);
					return PILO_OK;
				},//7
				[](::pilo::i64_t idx, char* dq, const std::string& value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::i64_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u64_t>::size_type>(idx), (::pilo::u64_t)v);
					return PILO_OK;
				},//8
				[](::pilo::i64_t idx, char* dq, const std::string& value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					bool v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_bool(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<bool>*>(dq)->insert(reinterpret_cast<std::deque<bool>*>(dq)->begin() + static_cast<std::deque<bool>::size_type>(idx), v);
					return PILO_OK;
				},//9
				[](::pilo::i64_t idx, char* dq, const std::string& value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::f32_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f32_t>::size_type>(idx), v);

					return PILO_OK;
				},//10
				[](::pilo::i64_t idx, char* dq, const std::string& value, ::pilo::i32_t len, bool) -> ::pilo::err_t
				{
					::pilo::f64_t v = 0;
					::pilo::err_t err = ::pilo::core::string::string_to_number(v, value.c_str(), len);
					if (err != PILO_OK)
					{
						return err;
					}
					reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f64_t>::size_type>(idx), v);
					return PILO_OK;
				},//11
				[](::pilo::i64_t idx, char* dq, const std::string& value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					char* ret = ::pilo::core::string::duplicate(value.c_str(), value.size());
					if (ret == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<char*>*>(dq)->insert(reinterpret_cast<std::deque<char*>*>(dq)->begin() + static_cast<std::deque<char*>::size_type>(idx), (char*)ret);
					return PILO_OK;
				},//12
				[](::pilo::i64_t idx, char* dq, const std::string& value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					reinterpret_cast<std::deque<std::string>*>(dq)->insert(reinterpret_cast<std::deque<std::string>*>(dq)->begin() + static_cast<std::deque<std::string>::size_type>(idx), std::string(value));
					return PILO_OK;
				},//13
				[](::pilo::i64_t idx, char* dq, const std::string& value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::tlv* t = ::pilo::tlv::allocate_single(value);
					if (t == nullptr)
						return ::pilo::mk_perr( PERR_INSUF_HEAP);
					reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->begin() + static_cast<std::deque<::pilo::tlv*>::size_type>(idx), t);
					return PILO_OK;

				},//14				
			};
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, const std::string& value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_insert_stdstr_func[vt](idx, dq, value, len, adopt);
			}

			typedef ::pilo::err_t(*handler_deque_insert_tlv_func_type)(::pilo::i64_t idx, char* dq, ::pilo::tlv*, ::pilo::i32_t len, bool adopt);
			static handler_deque_insert_tlv_func_type stc_handler_deque_insert_tlv_func[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				[](::pilo::i64_t, char* , ::pilo::tlv*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);  },
				[](::pilo::i64_t idx, char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i8_t>::size_type>(idx), 0);
					else
						reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i8_t>::size_type>(idx), value->as_i8(&err));
					return err;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx), 0);
					else
						reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u8_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u8_t>::size_type>(idx), value->as_u8(&err));
					return err;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i16_t>::size_type>(idx), 0);
					else
						reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i16_t>::size_type>(idx), value->as_i16(&err));
					return err;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u16_t>::size_type>(idx), 0);
					else
						reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u16_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u16_t>::size_type>(idx), value->as_u16(&err));
					return err;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i32_t>::size_type>(idx), 0);
					else
						reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i32_t>::size_type>(idx), value->as_i32(&err));
					return err;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u32_t>::size_type>(idx), 0);
					else
						reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u32_t>::size_type>(idx), value->as_u32(&err));
					return err;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i64_t>::size_type>(idx), 0);
					else
						reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::i64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::i64_t>::size_type>(idx), value->as_i64(&err));
					return err;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u64_t>::size_type>(idx), 0);
					else
						reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::u64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::u64_t>::size_type>(idx), value->as_u64(&err));
					return err;
				},
				[](::pilo::i64_t idx, char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<bool>*>(dq)->insert(reinterpret_cast<std::deque<bool>*>(dq)->begin() + static_cast<std::deque<bool>::size_type>(idx), false);
					else
						reinterpret_cast<std::deque<bool>*>(dq)->insert(reinterpret_cast<std::deque<bool>*>(dq)->begin() + static_cast<std::deque<bool>::size_type>(idx), value->as_bool(&err));
					return err;
				},//9
				[](::pilo::i64_t idx, char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f32_t>::size_type>(idx), 0);
					else
						reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f32_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f32_t>::size_type>(idx), value->as_f32(&err));
					return err;
				},//10
				[](::pilo::i64_t idx, char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f64_t>::size_type>(idx), 0);
					else
						reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::f64_t>*>(dq)->begin() + static_cast<std::deque<::pilo::f64_t>::size_type>(idx), value->as_f64(&err));
					return err;
				},//11
				[](::pilo::i64_t idx, char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<char*>*>(dq)->insert(reinterpret_cast<std::deque<char*>*>(dq)->begin() + static_cast<std::deque<char*>::size_type>(idx), nullptr);
					else
					{
						reinterpret_cast<std::deque<char*>*>(dq)->insert(reinterpret_cast<std::deque<char*>*>(dq)->begin() + static_cast<std::deque<char*>::size_type>(idx), value->as_cstr_dup(&err, nullptr));
					}
						
					return err;
				},//12
				[](::pilo::i64_t idx, char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<std::string>*>(dq)->insert(reinterpret_cast<std::deque<std::string>*>(dq)->begin() + static_cast<std::deque<std::string>::size_type>(idx), std::string());
					else
					{
						std::string dupped_str = value->as_str(&err);
						reinterpret_cast<std::deque<std::string>*>(dq)->insert(reinterpret_cast<std::deque<std::string>*>(dq)->begin() + static_cast<std::deque<std::string>::size_type>(idx), dupped_str);
						return err;
					}

					return err;
				},//13
				[](::pilo::i64_t idx, char* dq, ::pilo::tlv* value, ::pilo::i32_t , bool adopt) -> ::pilo::err_t
				{
					::pilo::err_t err = PILO_OK;
					if (value == nullptr)
						reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->begin() + static_cast<std::deque<::pilo::tlv*>::size_type>(idx), nullptr);
					else
					{
						if (adopt)
						{
							reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->begin() + static_cast<std::deque<::pilo::tlv*>::size_type>(idx), value);
						}
						else
						{
							::pilo::tlv* dupped = value->clone();
							if (dupped == nullptr)
							{
								return ::pilo::mk_perr( PERR_INSUF_HEAP);
							}
							reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->insert(reinterpret_cast<std::deque<::pilo::tlv*>*>(dq)->begin() + static_cast<std::deque<::pilo::tlv*>::size_type>(idx), dupped);
						}
					}
					return err;

				},//14
			};
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, ::pilo::tlv* value, ::pilo::i32_t len, bool adopt)
			{
				PMC_UNUSED(len);
				PMC_UNUSED(adopt);
				if (vt >= wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
				}
				return stc_handler_deque_insert_tlv_func[vt](idx, dq, value, len, adopt);
			}

		}
	}
}