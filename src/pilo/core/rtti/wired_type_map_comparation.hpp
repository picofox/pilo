#pragma once

#include "../../pilo.hpp"
#include "../string/string_operation.hpp"
#include <map>

namespace pilo
{
	class tlv;
	namespace core
	{
		namespace rtti
		{
		
			bool map_equals(::pilo::u8_t a_key_type, ::pilo::u16_t a_val_type, const char* a_data
				, ::pilo::u8_t b_key_type, ::pilo::u16_t b_val_type, const char* b_data, ::pilo::err_t * err);

			bool map_equals(::pilo::u8_t a_key_type, ::pilo::u16_t a_val_type, const char* a_data, const char* b_data, ::pilo::err_t* err);

			template <typename TA_KET, typename TA_VALUE>
			inline bool map_equals(const std::map<TA_KET, TA_VALUE>* a, const std::map<TA_KET, TA_VALUE>* b, ::pilo::err_t*)
			{
				if (a->size() != b->size())
					return false;
				typename std::map<TA_KET, TA_VALUE>::const_iterator cit_a = a->cbegin();
				typename std::map<TA_KET, TA_VALUE>::const_iterator cit_b = b->cbegin();
				for (; cit_a != a->cend(); cit_a++, cit_b++)
				{
					if (cit_a->second != cit_b->second)
					{
						return false;
					}
				}
				return true;
			}

			template <typename TA_KET>
			inline bool map_equals(const std::map<TA_KET, char*>* a, const std::map<TA_KET, char*>* b, ::pilo::err_t* )
			{
				if (a->size() != b->size())
					return false;
				typename std::map<TA_KET, char*>::const_iterator cit_a = a->cbegin();
				typename std::map<TA_KET, char*>::const_iterator cit_b = b->cbegin();
				for (; cit_a != a->cend(); cit_a++, cit_b++)
				{
					if (::pilo::core::string::strict_compare(cit_a->second, 0, cit_b->second,0, -1) != 0)
					{
						return false;
					}
				}
				return true;
			}

			template <typename TA_KET>
			inline bool map_equals(const std::map<TA_KET, const char*>* a, const std::map<TA_KET, const char*>* b, ::pilo::err_t*)
			{
				if (a->size() != b->size())
					return false;
				typename std::map<TA_KET, const char*>::const_iterator cit_a = a->cbegin();
				typename std::map<TA_KET, const char*>::const_iterator cit_b = b->cbegin();
				for (; cit_a != a->cend(); cit_a++, cit_b++)
				{
					if (::pilo::core::string::strict_compare(cit_a->second, 0, cit_b->second, 0, -1) != 0)
					{
						return false;
					}
				}
				return true;
			}

			template <typename TA_KET>
			inline bool map_equals(const std::map<TA_KET, const char*>* a, const std::map<TA_KET, char*>* b, ::pilo::err_t*)
			{
				if (a->size() != b->size())
					return false;
				typename std::map<TA_KET, const char*>::const_iterator cit_a = a->cbegin();
				typename std::map<TA_KET, char*>::const_iterator cit_b = b->cbegin();
				for (; cit_a != a->cend(); cit_a++, cit_b++)
				{
					if (::pilo::core::string::strict_compare(cit_a->second, 0, cit_b->second, 0, -1) != 0)
					{
						return false;
					}
				}
				return true;
			}

			template <typename TA_KET>
			inline bool map_equals(const std::map<TA_KET, char*>* a, const std::map<TA_KET, const char*>* b, ::pilo::err_t*)
			{
				if (a->size() != b->size())
					return false;
				typename std::map<TA_KET, char*>::const_iterator cit_a = a->cbegin();
				typename std::map<TA_KET, const char*>::const_iterator cit_b = b->cbegin();
				for (; cit_a != a->cend(); cit_a++, cit_b++)
				{
					if (::pilo::core::string::strict_compare(cit_a->second, 0, cit_b->second, 0, -1) != 0)
					{
						return false;
					}
				}
				return true;
			}

			template <typename TA_KET>
			inline bool map_equals(const std::map<TA_KET, ::pilo::tlv*>* a, const std::map<TA_KET, ::pilo::tlv*>* b, ::pilo::err_t* )
			{
				if (a->size() != b->size())
					return false;
				typename std::map<TA_KET, ::pilo::tlv*>::const_iterator cit_a = a->cbegin();
				typename std::map<TA_KET, ::pilo::tlv*>::const_iterator cit_b = b->cbegin();
				for (; cit_a != a->cend(); cit_a++, cit_b++)
				{
					if (cit_a->second == nullptr && cit_b->second == nullptr)
					{

					}
					else if (cit_a->second != nullptr && cit_b->second != nullptr)
					{
						if (!cit_a->second->equals_to(cit_b->second))
						{
							return false;
						}
					}
					else
						return false;
				}
				return true;
			}

			template <typename TA_KET>
			inline bool map_equals(const std::map<TA_KET, const ::pilo::tlv*>* a, const std::map<TA_KET, const ::pilo::tlv*>* b, ::pilo::err_t*)
			{
				if (a->size() != b->size())
					return false;
				typename std::map<TA_KET, const ::pilo::tlv*>::const_iterator cit_a = a->cbegin();
				typename std::map<TA_KET, const ::pilo::tlv*>::const_iterator cit_b = b->cbegin();
				for (; cit_a != a->cend(); cit_a++, cit_b++)
				{
					if (cit_a->second == nullptr && cit_b->second == nullptr)
					{

					}
					else if (cit_a->second != nullptr && cit_b->second != nullptr)
					{
						if (!cit_a->second->equals_to(cit_b->second))
						{
							return false;
						}
					}
					else
						return false;
				}
				return true;
			}

			template <typename TA_KET>
			inline bool map_equals(const std::map<TA_KET, const ::pilo::tlv*>* a, const std::map<TA_KET, ::pilo::tlv*>* b, ::pilo::err_t*)
			{
				if (a->size() != b->size())
					return false;
				typename std::map<TA_KET, const ::pilo::tlv*>::const_iterator cit_a = a->cbegin();
				typename std::map<TA_KET, ::pilo::tlv*>::const_iterator cit_b = b->cbegin();
				for (; cit_a != a->cend(); cit_a++, cit_b++)
				{
					if (cit_a->second == nullptr && cit_b->second == nullptr)
					{

					}
					else if (cit_a->second != nullptr && cit_b->second != nullptr)
					{
						if (!cit_a->second->equals_to(cit_b->second))
						{
							return false;
						}
					}
					else
						return false;
				}
				return true;
			}

			template <typename TA_KET>
			inline bool map_equals(const std::map<TA_KET,  ::pilo::tlv*>* a, const std::map<TA_KET, const ::pilo::tlv*>* b, ::pilo::err_t*)
			{
				if (a->size() != b->size())
					return false;
				typename std::map<TA_KET, ::pilo::tlv*>::const_iterator cit_a = a->cbegin();
				typename std::map<TA_KET, const ::pilo::tlv*>::const_iterator cit_b = b->cbegin();
				for (; cit_a != a->cend(); cit_a++, cit_b++)
				{
					if (cit_a->second == nullptr && cit_b->second == nullptr)
					{

					}
					else if (cit_a->second != nullptr && cit_b->second != nullptr)
					{
						if(!cit_a->second->equals_to(cit_b->second))
						{
							return false;
						}
					}
					else
						return false;
				}
				return true;
			}
		}
	}
}