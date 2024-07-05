#pragma once

#include "../../pilo.hpp"
#include "../string/string_operation.hpp"
#include <deque>
#include <algorithm>
#include "./wired_type_deleter.hpp"

namespace pilo
{
	class tlv;
	namespace core
	{
		namespace rtti
		{

			template<typename TA_VALUE>	inline void clear_deque(std::deque<TA_VALUE>* deq, bool compact)
			{
				if (deq != nullptr)
				{
					(deq)->clear();
					if (compact)
						(deq)->shrink_to_fit();
				}
			}
			void clear_deque(std::deque<char*>* deq, bool compact);
			void clear_deque(std::deque<::pilo::tlv*>* deq, bool compact);
			

			::pilo::err_t destroy_deque(::pilo::u16_t vt, char** data_pptr);
			template<typename TA_VALUE>	inline void destroy_deque(std::deque<TA_VALUE>** deq)
			{
				if (deq != nullptr && *deq != nullptr)
				{
					clear_deque(*deq, true);
					delete* deq;
					*deq = nullptr;
				}				
			}


			char* create_deque(::pilo::u16_t valtype, ::pilo::err_t*);
			template<typename TA_VALUE> inline std::deque<TA_VALUE>* create_deque(const std::initializer_list<TA_VALUE> & list, bool adopt, ::pilo::err_t* errp)
			{
				PMC_UNUSED(adopt);
				std::deque<TA_VALUE>* rac = new std::deque<TA_VALUE>;
				if (rac == nullptr)
				{
					::pilo::set_if_ptr_is_not_null(errp, ::pilo::mk_perr( PERR_INSUF_HEAP));
					return nullptr;
				}
				for (TA_VALUE val : list)
				{
					rac->push_back(val);
				}
				return rac;
			}	
			std::deque<char*>* create_deque(const std::initializer_list<const char*>& list, bool adopt, ::pilo::err_t* errp);
			std::deque<char*>* create_deque(const std::initializer_list<char*>& list, bool adopt, ::pilo::err_t* errp);
			std::deque<::pilo::tlv*>* create_deque(const std::initializer_list<::pilo::tlv*>& list, bool adopt, ::pilo::err_t* errp);
			


			template<typename TA_VALUE>	inline std::deque<TA_VALUE>* clone_deque(std::deque<TA_VALUE>* src, ::pilo::err_t* err)
			{
				if (src == nullptr)
					return nullptr;
				typename std::deque<TA_VALUE>* cloned_deque = new std::deque<TA_VALUE>(*src);
				if (cloned_deque == nullptr)
				{
					if (err != nullptr)
						*err = ::pilo::mk_perr( PERR_INSUF_HEAP);
					destroy_deque((std::deque<TA_VALUE>**) &cloned_deque);
					return nullptr;
				}
				return cloned_deque;
			}

			std::deque<char*>* clone_deque(std::deque<char*>* src, ::pilo::err_t* err);
			std::deque<char*>* clone_deque(std::deque<const char*>* src, ::pilo::err_t* err);
			std::deque<::pilo::tlv*>* clone_deque(std::deque<::pilo::tlv*>* src, ::pilo::err_t* err);
			char* clone_deque(::pilo::u16_t vt, char*, pilo::err_t*);


			template <typename TA_VALUE> inline bool deque_equals(const std::deque<TA_VALUE>* a, const std::deque<TA_VALUE>* b, ::pilo::err_t*)
			{
				if (a->size() != b->size())
					return false;
				typename std::deque<TA_VALUE>::const_iterator cit_a = a->cbegin();
				typename std::deque<TA_VALUE>::const_iterator cit_b = b->cbegin();
				for (; cit_a != a->cend(); cit_a++, cit_b++)
				{
					if (*cit_a != *cit_b)
					{
						return false;
					}
				}
				return true;
			}
			bool deque_equals(const std::deque<char*>* a, const std::deque<char*>* b, ::pilo::err_t*);
			bool deque_equals(const std::deque<const char*>* a, const std::deque<char*>* b, ::pilo::err_t*);
			bool deque_equals(const std::deque<char*>* a, const std::deque<const char*>* b, ::pilo::err_t*);
			bool deque_equals(const std::deque<const char*>* a, const std::deque<const char*>* b, ::pilo::err_t*);
			bool deque_equals(const std::deque<::pilo::tlv*>* a, const std::deque<::pilo::tlv*>* b, ::pilo::err_t*);
			bool deque_equals(const std::deque<const ::pilo::tlv*>* a, const std::deque<::pilo::tlv*>* b, ::pilo::err_t*);
			bool deque_equals(const std::deque<::pilo::tlv*>* a, const std::deque<const ::pilo::tlv*>* b, ::pilo::err_t*);
			bool deque_equals(const std::deque<const ::pilo::tlv*>* a, const std::deque<const ::pilo::tlv*>* b, ::pilo::err_t*);
			bool deque_equals(::pilo::u16_t a_val_type, const char* a_data, ::pilo::u16_t b_val_type, const char* b_data, ::pilo::err_t* err);


			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, ::pilo::i8_t, ::pilo::i32_t len, bool adopt);//1
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, ::pilo::u8_t, ::pilo::i32_t len, bool adopt);//2
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, ::pilo::i16_t, ::pilo::i32_t len, bool adopt);//3
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, ::pilo::u16_t, ::pilo::i32_t len, bool adopt);//4
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, ::pilo::i32_t, ::pilo::i32_t len, bool adopt);//5
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, ::pilo::u32_t, ::pilo::i32_t len, bool adopt);//6
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, ::pilo::i64_t, ::pilo::i32_t len, bool adopt);//7
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, ::pilo::u64_t, ::pilo::i32_t len, bool adopt);//8
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, bool, ::pilo::i32_t len, bool adopt);//9
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, ::pilo::f32_t, ::pilo::i32_t len, bool adopt);//10
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, ::pilo::f64_t, ::pilo::i32_t len, bool adopt);//11
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, const char*, ::pilo::i32_t len, bool adopt);//12
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, const std::string&, ::pilo::i32_t len, bool adopt);//13
			::pilo::err_t deque_push_back(::pilo::u16_t vt, char* dq, ::pilo::tlv*, ::pilo::i32_t len, bool adopt);//14

			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, ::pilo::i8_t, ::pilo::i32_t len, bool adopt);//1
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, ::pilo::u8_t, ::pilo::i32_t len, bool adopt);//2
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, ::pilo::i16_t, ::pilo::i32_t len, bool adopt);//3
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, ::pilo::u16_t, ::pilo::i32_t len, bool adopt);//4
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, ::pilo::i32_t, ::pilo::i32_t len, bool adopt);//5
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, ::pilo::u32_t, ::pilo::i32_t len, bool adopt);//6
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, ::pilo::i64_t, ::pilo::i32_t len, bool adopt);//7
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, ::pilo::u64_t, ::pilo::i32_t len, bool adopt);//8
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, bool, ::pilo::i32_t len, bool adopt);//9
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, ::pilo::f32_t, ::pilo::i32_t len, bool adopt);//10
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, ::pilo::f64_t, ::pilo::i32_t len, bool adopt);//11
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, const char*, ::pilo::i32_t len, bool adopt);//12
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, const std::string&, ::pilo::i32_t len, bool adopt);//13
			::pilo::err_t deque_push_front(::pilo::u16_t vt, char* dq, ::pilo::tlv*, ::pilo::i32_t len, bool adopt);//14


			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, ::pilo::i8_t, ::pilo::i32_t len, bool adopt);//1
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, ::pilo::u8_t, ::pilo::i32_t len, bool adopt);//2
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, ::pilo::i16_t, ::pilo::i32_t len, bool adopt);//3
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, ::pilo::u16_t, ::pilo::i32_t len, bool adopt);//4
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, ::pilo::i32_t, ::pilo::i32_t len, bool adopt);//5
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, ::pilo::u32_t, ::pilo::i32_t len, bool adopt);//6
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, ::pilo::i64_t, ::pilo::i32_t len, bool adopt);//7
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, ::pilo::u64_t, ::pilo::i32_t len, bool adopt);//8
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, bool, ::pilo::i32_t len, bool adopt);//9
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, ::pilo::f32_t, ::pilo::i32_t len, bool adopt);//10
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, ::pilo::f64_t, ::pilo::i32_t len, bool adopt);//11
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, const char*, ::pilo::i32_t len, bool adopt);//12
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, const std::string&, ::pilo::i32_t len, bool adopt);//13
			::pilo::err_t deque_insert(::pilo::u16_t vt, ::pilo::i64_t idx, char* dq, ::pilo::tlv*, ::pilo::i32_t len, bool adopt);//14

		}
	}
}