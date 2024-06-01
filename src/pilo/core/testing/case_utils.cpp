#include "./case_utils.hpp"
#include "../datetime/timestamp.hpp"
#include "../rtti/wired_type_map_creation.hpp"

namespace pilo
{
	namespace core
	{
		namespace testing
		{
            ::pilo::i32_t create_random_memory_sample(std::string& str, ::pilo::i32_t beg_size, ::pilo::i32_t end_size)
            {
                int b = beg_size + rand() % (end_size - beg_size + 1);
                str.clear();
                str.resize(b);
                for (::pilo::i32_t i = 0; i < b; i++)
                {
                    str[i] = i % (127);
                }
                return b;
            }

            char* create_random_memory_sample(char* buffer, ::pilo::i32_t capa, ::pilo::i32_t & rlen, ::pilo::i32_t beg_size, ::pilo::i32_t end_size)
            {
                char* ret = buffer;
                int b = beg_size + rand() % (end_size - beg_size + 1);
                if (b < 1)
                {
                    rlen = 0;
                    return nullptr;
                }
                    
                if (capa < b)
                {
                    ret = (char*)PMF_HEAP_MALLOC(b);
                }                
                rlen = b;
                for (::pilo::i32_t i = 0; i < b; i++)
                {
                    ret[i] = i % (127);
                }
                return ret;
            }

            ::pilo::tlv* create_random_array_tlv(::pilo::i32_t level)
            {
                PMC_UNUSED(level);
                ::pilo::tlv* ret = ::pilo::tlv::allocate();
                if (ret == nullptr)
                {
                    PMC_ASSERT(false);
                    return nullptr;
                }
                ::pilo::err_t err = ret->set_array_type(::pilo::core::rtti::wired_type::value_type_tlv);
                if (err != PILO_OK)
                {
                    return nullptr;
                }

                ret->push_back_array_element<::pilo::i8_t>({ PILO_INT8_MIN , -100,  0 , 100,  PILO_INT8_MAX}, &err); //0
                if (err != PILO_OK)
                {
                    return nullptr;
                }
                ret->push_back_array_element<::pilo::u8_t>({ 0 , 1,2,100, PILO_UINT8_MAX }, &err); //1
                if (err != PILO_OK)
                {
                    return nullptr;
                }
                ret->push_back_array_element<::pilo::i16_t>({ PILO_INT16_MIN , -100,  0 , 100,  PILO_INT16_MAX }, &err); //2
                if (err != PILO_OK)
                {
                    return nullptr;
                }
                ret->push_back_array_element<::pilo::u16_t>({ 0 , 1,2,100, PILO_UINT16_MAX }, &err); //3
                if (err != PILO_OK)
                {
                    return nullptr;
                }
                ret->push_back_array_element<::pilo::i32_t>({ PILO_INT32_MIN , -100,  0 , 100,  PILO_INT32_MAX }, &err); //4
                if (err != PILO_OK)
                {
                    return nullptr;
                }
                ret->push_back_array_element<::pilo::u32_t>({ 0 , 1,2,100, PILO_UINT32_MAX }, &err); //5
                if (err != PILO_OK)
                {
                    return nullptr;
                }
                ret->push_back_array_element<::pilo::i64_t>({ PILO_INT64_MIN , -100,  0 , 100,  PILO_INT64_MAX }, &err); //6
                if (err != PILO_OK)
                {
                    return nullptr;
                }
                ret->push_back_array_element<::pilo::u64_t>({ 0 , 1,2,100, PILO_UINT64_MAX }, &err); //7
                if (err != PILO_OK)
                {
                    return nullptr;
                }
                ret->push_back_array_element<bool>({ false, true, false, true }, &err); //8
                if (err != PILO_OK)
                {
                    return nullptr;
                }



                ret->push_back_array_element<::pilo::f32_t>({ 0.0f, 1.0f, 2.0f, -5.0f }, &err); //9
                if (err != PILO_OK)
                {
                    return nullptr;
                }
                ret->push_back_array_element<::pilo::f64_t>({ 0.0, 1.0, 2.0, -5.0 }, &err); //10
                if (err != PILO_OK)
                {
                    return nullptr;
                }
                auto tmp_arr = ret->push_back_array_element<const char*>({"fox1", "fox2", "fox3", "fox4", "cat0"}, &err); //11
                if (err != PILO_OK)
                {
                    return nullptr;
                }
                tmp_arr->push_back("gorila_1");

                ::pilo::tlv* tmp_arr_tlv = ret->push_back_array_element<::pilo::tlv*>({}, &err); //12
                if (err != PILO_OK)
                {
                    return nullptr;
                }
                ::pilo::i32_t tcnt = std::rand() % 5;
                for (::pilo::i32_t i = 0; i < tcnt; i++)
                {
                    if (level > 0)
                    { 
                        if (level == 1)
                        {
                            ::pilo::tlv* t0_tlv = create_random_dict_tlv(::pilo::core::rtti::wired_type::key_type_na, ::pilo::core::rtti::wired_type::value_type_na, level - 1);
                            if (t0_tlv == nullptr)
                            {
                                PMC_ASSERT(false);
                                return nullptr;
                            }
                            tmp_arr_tlv->push_back(t0_tlv, -1, true);

                        }
                        else
                        {
                            ::pilo::tlv* t0_tlv = create_random_array_tlv(level - 1);
                            tmp_arr_tlv->push_back(t0_tlv, -1, true);
                        }                        

                    }
                    else
                    {
                        ::pilo::tlv* t0_tlv = create_random_single_tlv();
                        if (t0_tlv->wrapper_type() == 0)
                        {
                            printf("\n");
                        }
                        tmp_arr_tlv->push_back(t0_tlv, -1, true);
                    }
                    
                }

                ::pilo::tlv* tmp_tlv = ret->push_back_array_element<std::string>({}, &err); //13
                if (err != PILO_OK)
                {
                    return nullptr;
                }
                for (int i = 0; i < 17; i++)
                {
                    std::string str;
                    create_random_memory_sample(str, 0, 257);
                    tmp_tlv->push_back(str);
                }

                return ret;
            }

            ::pilo::tlv* create_random_dict_tlv(::pilo::u8_t kt, ::pilo::u16_t vt, ::pilo::i32_t level)
            {
                ::pilo::tlv* t = ::pilo::tlv::allocate();
                if (t == nullptr) return t;

                if (kt == ::pilo::core::rtti::wired_type::key_type_na || kt > ::pilo::core::rtti::wired_type::key_type_str)
                {
                    kt = rand() % (::pilo::core::rtti::wired_type::key_type_count - 1) + 1;
                }
                if (vt == ::pilo::core::rtti::wired_type::value_type_na || vt >= ::pilo::core::rtti::wired_type::value_type_intrincs_count)
                {
                    vt = rand() % (::pilo::core::rtti::wired_type::value_type_intrincs_count - 1) + 1;
                }

                PMC_ASSERT(kt != ::pilo::core::rtti::wired_type::key_type_na);
                PMC_ASSERT(vt != ::pilo::core::rtti::wired_type::value_type_na);
                ::pilo::err_t err = t->set_dict_type(kt, vt);
                if (err != PILO_OK)
                    return nullptr;

                if (kt == ::pilo::core::rtti::wired_type::key_type_i8)
                {
                    if (vt == ::pilo::core::rtti::wired_type::value_type_i8)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i8_t, ::pilo::i8_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MIN, (::pilo::i8_t)PILO_INT8_MIN),
                                ::std::make_pair((::pilo::i8_t)-1, (::pilo::i8_t)-100),
                                ::std::make_pair((::pilo::i8_t)0, (::pilo::i8_t)0),
                                ::std::make_pair((::pilo::i8_t)1, (::pilo::i8_t)100),
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MAX, (::pilo::i8_t)PILO_INT8_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u8)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i8_t, ::pilo::u8_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MIN, (::pilo::u8_t)0),
                                ::std::make_pair((::pilo::i8_t)-1, (::pilo::u8_t)100),
                                ::std::make_pair((::pilo::i8_t)0, (::pilo::u8_t)0),
                                ::std::make_pair((::pilo::i8_t)2, (::pilo::u8_t)200),
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MAX, (::pilo::u8_t)PILO_UINT8_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i16)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i8_t, ::pilo::i16_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MIN, (::pilo::i16_t)PILO_INT16_MIN),
                                ::std::make_pair((::pilo::i8_t)-1, (::pilo::i16_t)-100),
                                ::std::make_pair((::pilo::i8_t)0, (::pilo::i16_t)0),
                                ::std::make_pair((::pilo::i8_t)1, (::pilo::i16_t)100),
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MAX, (::pilo::i16_t)PILO_INT16_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u16)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i8_t, ::pilo::u16_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MIN, (::pilo::u16_t)0),
                                ::std::make_pair((::pilo::i8_t)-1, (::pilo::u16_t)10000),
                                ::std::make_pair((::pilo::i8_t)0, (::pilo::u16_t)0),
                                ::std::make_pair((::pilo::i8_t)1, (::pilo::u16_t)20000),
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MAX, (::pilo::u16_t)PILO_UINT16_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i8_t, ::pilo::i32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MIN, (::pilo::i32_t)PILO_INT32_MIN),
                                ::std::make_pair((::pilo::i8_t)-1, (::pilo::i32_t)-100),
                                ::std::make_pair((::pilo::i8_t)0, (::pilo::i32_t)0),
                                ::std::make_pair((::pilo::i8_t)1, (::pilo::i32_t)100),
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MAX, (::pilo::i32_t)PILO_INT32_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i8_t, ::pilo::u32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MIN, (::pilo::u32_t)0),
                                ::std::make_pair((::pilo::i8_t)-1, (::pilo::u32_t)10000),
                                ::std::make_pair((::pilo::i8_t)0, (::pilo::u32_t)0),
                                ::std::make_pair((::pilo::i8_t)1, (::pilo::u32_t)20000),
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MAX, (::pilo::u32_t)PILO_UINT32_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i8_t, ::pilo::i64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MIN, (::pilo::i64_t)PILO_INT64_MIN),
                                ::std::make_pair((::pilo::i8_t)-1, (::pilo::i64_t)-100),
                                ::std::make_pair((::pilo::i8_t)0, (::pilo::i64_t)0),
                                ::std::make_pair((::pilo::i8_t)1, (::pilo::i64_t)100),
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MAX, (::pilo::i64_t)PILO_INT64_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i8_t, ::pilo::u64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MIN, (::pilo::u64_t)0),
                                ::std::make_pair((::pilo::i8_t)-1, (::pilo::u64_t)100000),
                                ::std::make_pair((::pilo::i8_t)0, (::pilo::u64_t)0),
                                ::std::make_pair((::pilo::i8_t)1, (::pilo::u64_t)200000),
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MAX, (::pilo::u64_t)PILO_UINT64_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_boolean)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i8_t, bool>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MIN, (bool)false),
                                ::std::make_pair((::pilo::i8_t)-1, (bool)true),
                                ::std::make_pair((::pilo::i8_t)0, (bool)false),
                                ::std::make_pair((::pilo::i8_t)1, (bool)true),
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MAX, (bool)false),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_f32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i8_t, ::pilo::f32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MIN, (::pilo::f32_t) 1.0f),
                                ::std::make_pair((::pilo::i8_t)-1, (::pilo::f32_t) 2.0f),
                                ::std::make_pair((::pilo::i8_t)0, (::pilo::f32_t) 3.0f),
                                ::std::make_pair((::pilo::i8_t)1, (::pilo::f32_t) 4.0f),
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MAX, (::pilo::f32_t) 5.0f),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_f64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i8_t, ::pilo::f64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MIN, (::pilo::f64_t)11.0),
                                ::std::make_pair((::pilo::i8_t)-1, (::pilo::f64_t)12.0),
                                ::std::make_pair((::pilo::i8_t)0, (::pilo::f64_t)13.0),
                                ::std::make_pair((::pilo::i8_t)1, (::pilo::f64_t)14.0),
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MAX, (::pilo::f64_t)15.0),
                            }, false);

                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_bytes)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i8_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MIN, "0123456789"),
                                ::std::make_pair((::pilo::i8_t)-1, "abcdefg"),
                                ::std::make_pair((::pilo::i8_t)0, "tessakkkaz"),
                                ::std::make_pair((::pilo::i8_t)1, "supermario"),
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MAX, "xenoblade"),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_str)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i8_t, std::string>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MIN, std::string("fox1")),
                                ::std::make_pair((::pilo::i8_t)-1, std::string("fox2")),
                                ::std::make_pair((::pilo::i8_t)0, std::string("fox3")),
                                ::std::make_pair((::pilo::i8_t)1, std::string("fox4")),
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MAX, std::string("fox5")),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_tlv)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i8_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MIN, ::pilo::tlv::allocate_single(1.2f)),
                                ::std::make_pair((::pilo::i8_t)-1, ::pilo::tlv::allocate_single((::pilo::u64_t)100010)),
                                ::std::make_pair((::pilo::i8_t)0, ::pilo::tlv::allocate_single(std::string("single variable"))),
                                ::std::make_pair((::pilo::i8_t)1, ::pilo::tlv::allocate_single(true)),
                                ::std::make_pair((::pilo::i8_t)PILO_INT8_MAX, ::pilo::tlv::allocate_single("bytes value", -1, true, false)),
                            }, false);
                    }
                    else
                    {
                        PMC_ASSERT(false);
                    }
                
                    t->size_ref() = 5;
                    return t;
                }
                else if (kt == ::pilo::core::rtti::wired_type::key_type_u8)
                {
                    if (vt == ::pilo::core::rtti::wired_type::value_type_i8)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u8_t, ::pilo::i8_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u8_t)0, (::pilo::i8_t)PILO_INT8_MIN),
                                ::std::make_pair((::pilo::u8_t)1, (::pilo::i8_t)-100),
                                ::std::make_pair((::pilo::u8_t)2, (::pilo::i8_t)0),
                                ::std::make_pair((::pilo::u8_t)3, (::pilo::i8_t)100),
                                ::std::make_pair((::pilo::u8_t)PILO_UINT8_MAX, (::pilo::i8_t)PILO_UINT8_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u8)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u8_t, ::pilo::u8_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u8_t)0, (::pilo::u8_t)0),
                                ::std::make_pair((::pilo::u8_t)1, (::pilo::u8_t)100),
                                ::std::make_pair((::pilo::u8_t)2, (::pilo::u8_t)0),
                                ::std::make_pair((::pilo::u8_t)3, (::pilo::u8_t)200),
                                ::std::make_pair((::pilo::u8_t)PILO_UINT8_MAX, (::pilo::u8_t)PILO_UINT8_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i16)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u8_t, ::pilo::i16_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u8_t)0, (::pilo::i16_t)PILO_INT16_MIN),
                                ::std::make_pair((::pilo::u8_t)1, (::pilo::i16_t)-100),
                                ::std::make_pair((::pilo::u8_t)2, (::pilo::i16_t)0),
                                ::std::make_pair((::pilo::u8_t)3, (::pilo::i16_t)100),
                                ::std::make_pair((::pilo::u8_t)PILO_UINT8_MAX, (::pilo::i16_t)PILO_INT16_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u16)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u8_t, ::pilo::u16_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u8_t)0, (::pilo::u16_t)0),
                                ::std::make_pair((::pilo::u8_t)1, (::pilo::u16_t)10000),
                                ::std::make_pair((::pilo::u8_t)2, (::pilo::u16_t)0),
                                ::std::make_pair((::pilo::u8_t)3, (::pilo::u16_t)20000),
                                ::std::make_pair((::pilo::u8_t)PILO_UINT8_MAX, (::pilo::u16_t)PILO_UINT16_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u8_t, ::pilo::i32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u8_t)0, (::pilo::i32_t)PILO_INT32_MIN),
                                ::std::make_pair((::pilo::u8_t)1, (::pilo::i32_t)-100),
                                ::std::make_pair((::pilo::u8_t)2, (::pilo::i32_t)0),
                                ::std::make_pair((::pilo::u8_t)3, (::pilo::i32_t)100),
                                ::std::make_pair((::pilo::u8_t)PILO_UINT8_MAX, (::pilo::i32_t)PILO_INT32_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u8_t, ::pilo::u32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u8_t)0, (::pilo::u32_t)0),
                                ::std::make_pair((::pilo::u8_t)1, (::pilo::u32_t)10000),
                                ::std::make_pair((::pilo::u8_t)2, (::pilo::u32_t)0),
                                ::std::make_pair((::pilo::u8_t)3, (::pilo::u32_t)20000),
                                ::std::make_pair((::pilo::u8_t)PILO_UINT8_MAX, (::pilo::u32_t)PILO_UINT32_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u8_t, ::pilo::i64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u8_t)0, (::pilo::i64_t)PILO_INT64_MIN),
                                ::std::make_pair((::pilo::u8_t)1, (::pilo::i64_t)-100),
                                ::std::make_pair((::pilo::u8_t)2, (::pilo::i64_t)0),
                                ::std::make_pair((::pilo::u8_t)3, (::pilo::i64_t)100),
                                ::std::make_pair((::pilo::u8_t)PILO_UINT8_MAX, (::pilo::i64_t)PILO_INT64_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u8_t, ::pilo::u64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u8_t)0, (::pilo::u64_t)0),
                                ::std::make_pair((::pilo::u8_t)1, (::pilo::u64_t)100000),
                                ::std::make_pair((::pilo::u8_t)2, (::pilo::u64_t)0),
                                ::std::make_pair((::pilo::u8_t)3, (::pilo::u64_t)200000),
                                ::std::make_pair((::pilo::u8_t)PILO_UINT8_MAX, (::pilo::u64_t)PILO_UINT64_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_boolean)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u8_t, bool>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u8_t)0, (bool)false),
                                ::std::make_pair((::pilo::u8_t)1, (bool)true),
                                ::std::make_pair((::pilo::u8_t)2, (bool)false),
                                ::std::make_pair((::pilo::u8_t)3, (bool)true),
                                ::std::make_pair((::pilo::u8_t)PILO_UINT8_MAX, (bool)false),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_f32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u8_t, ::pilo::f32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u8_t)0, (::pilo::f32_t)1.0f),
                                ::std::make_pair((::pilo::u8_t)1, (::pilo::f32_t)2.0f),
                                ::std::make_pair((::pilo::u8_t)2, (::pilo::f32_t)3.0f),
                                ::std::make_pair((::pilo::u8_t)3, (::pilo::f32_t)4.0f),
                                ::std::make_pair((::pilo::u8_t)PILO_UINT8_MAX, (::pilo::f32_t)5.0f),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_f64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u8_t, ::pilo::f64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u8_t)0, (::pilo::f64_t)11.0),
                                ::std::make_pair((::pilo::u8_t)1, (::pilo::f64_t)12.0),
                                ::std::make_pair((::pilo::u8_t)2, (::pilo::f64_t)13.0),
                                ::std::make_pair((::pilo::u8_t)3, (::pilo::f64_t)14.0),
                                ::std::make_pair((::pilo::u8_t)PILO_UINT8_MAX, (::pilo::f64_t)15.0),
                            }, false);

                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_bytes)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u8_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u8_t)0, "0123456789"),
                                ::std::make_pair((::pilo::u8_t)1, "abcdefg"),
                                ::std::make_pair((::pilo::u8_t)2, "tessakkkaz"),
                                ::std::make_pair((::pilo::u8_t)3, "supermario"),
                                ::std::make_pair((::pilo::u8_t)PILO_UINT8_MAX, "xenoblade"),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_str)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u8_t, std::string>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u8_t)0, std::string("fox1")),
                                ::std::make_pair((::pilo::u8_t)1, std::string("fox2")),
                                ::std::make_pair((::pilo::u8_t)2, std::string("fox3")),
                                ::std::make_pair((::pilo::u8_t)3, std::string("fox4")),
                                ::std::make_pair((::pilo::u8_t)PILO_UINT8_MAX, std::string("fox5")),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_tlv)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u8_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u8_t)0, ::pilo::tlv::allocate_single(1.2f)),
                                ::std::make_pair((::pilo::u8_t)1, ::pilo::tlv::allocate_single((::pilo::u64_t)100010)),
                                ::std::make_pair((::pilo::u8_t)2, ::pilo::tlv::allocate_single(std::string("single variable"))),
                                ::std::make_pair((::pilo::u8_t)3, ::pilo::tlv::allocate_single(true)),
                                ::std::make_pair((::pilo::u8_t)PILO_UINT8_MAX, ::pilo::tlv::allocate_single("bytes value", -1, true, false)),
                            }, false);
                    }
                    else
                    {
                        PMC_ASSERT(false);
                    }

                    t->size_ref() = 5;
                    return t;
                }
                else if (kt == ::pilo::core::rtti::wired_type::key_type_i16)
                {
                    if (vt == ::pilo::core::rtti::wired_type::value_type_i8)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i16_t, ::pilo::i8_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MIN, (::pilo::i8_t)PILO_INT8_MIN),
                                ::std::make_pair((::pilo::i16_t)-1, (::pilo::i8_t)-100),
                                ::std::make_pair((::pilo::i16_t)0, (::pilo::i8_t)0),
                                ::std::make_pair((::pilo::i16_t)1, (::pilo::i8_t)100),
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MAX, (::pilo::i8_t)PILO_INT8_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u8)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i16_t, ::pilo::u8_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MIN, (::pilo::u8_t)0),
                                ::std::make_pair((::pilo::i16_t)-1, (::pilo::u8_t)100),
                                ::std::make_pair((::pilo::i16_t)0, (::pilo::u8_t)0),
                                ::std::make_pair((::pilo::i16_t)2, (::pilo::u8_t)200),
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MAX, (::pilo::u8_t)PILO_UINT8_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i16)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i16_t, ::pilo::i16_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MIN, (::pilo::i16_t)PILO_INT16_MIN),
                                ::std::make_pair((::pilo::i16_t)-1, (::pilo::i16_t)-100),
                                ::std::make_pair((::pilo::i16_t)0, (::pilo::i16_t)0),
                                ::std::make_pair((::pilo::i16_t)1, (::pilo::i16_t)100),
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MAX, (::pilo::i16_t)PILO_INT16_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u16)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i16_t, ::pilo::u16_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MIN, (::pilo::u16_t)0),
                                ::std::make_pair((::pilo::i16_t)-1, (::pilo::u16_t)10000),
                                ::std::make_pair((::pilo::i16_t)0, (::pilo::u16_t)0),
                                ::std::make_pair((::pilo::i16_t)1, (::pilo::u16_t)20000),
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MAX, (::pilo::u16_t)PILO_UINT16_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i16_t, ::pilo::i32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MIN, (::pilo::i32_t)PILO_INT32_MIN),
                                ::std::make_pair((::pilo::i16_t)-1, (::pilo::i32_t)-100),
                                ::std::make_pair((::pilo::i16_t)0, (::pilo::i32_t)0),
                                ::std::make_pair((::pilo::i16_t)1, (::pilo::i32_t)100),
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MAX, (::pilo::i32_t)PILO_INT32_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i16_t, ::pilo::u32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MIN, (::pilo::u32_t)0),
                                ::std::make_pair((::pilo::i16_t)-1, (::pilo::u32_t)10000),
                                ::std::make_pair((::pilo::i16_t)0, (::pilo::u32_t)0),
                                ::std::make_pair((::pilo::i16_t)1, (::pilo::u32_t)20000),
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MAX, (::pilo::u32_t)PILO_UINT32_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i16_t, ::pilo::i64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MIN, (::pilo::i64_t)PILO_INT64_MIN),
                                ::std::make_pair((::pilo::i16_t)-1, (::pilo::i64_t)-100),
                                ::std::make_pair((::pilo::i16_t)0, (::pilo::i64_t)0),
                                ::std::make_pair((::pilo::i16_t)1, (::pilo::i64_t)100),
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MAX, (::pilo::i64_t)PILO_INT64_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i16_t, ::pilo::u64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MIN, (::pilo::u64_t)0),
                                ::std::make_pair((::pilo::i16_t)-1, (::pilo::u64_t)100000),
                                ::std::make_pair((::pilo::i16_t)0, (::pilo::u64_t)0),
                                ::std::make_pair((::pilo::i16_t)1, (::pilo::u64_t)200000),
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MAX, (::pilo::u64_t)PILO_UINT64_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_boolean)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i16_t, bool>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MIN, (bool)false),
                                ::std::make_pair((::pilo::i16_t)-1, (bool)true),
                                ::std::make_pair((::pilo::i16_t)0, (bool)false),
                                ::std::make_pair((::pilo::i16_t)1, (bool)true),
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MAX, (bool)false),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_f32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i16_t, ::pilo::f32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MIN, (::pilo::f32_t)1.0f),
                                ::std::make_pair((::pilo::i16_t)-1, (::pilo::f32_t)2.0f),
                                ::std::make_pair((::pilo::i16_t)0, (::pilo::f32_t)3.0f),
                                ::std::make_pair((::pilo::i16_t)1, (::pilo::f32_t)4.0f),
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MAX, (::pilo::f32_t)5.0f),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_f64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i16_t, ::pilo::f64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MIN, (::pilo::f64_t)11.0),
                                ::std::make_pair((::pilo::i16_t)-1, (::pilo::f64_t)12.0),
                                ::std::make_pair((::pilo::i16_t)0, (::pilo::f64_t)13.0),
                                ::std::make_pair((::pilo::i16_t)1, (::pilo::f64_t)14.0),
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MAX, (::pilo::f64_t)15.0),
                            }, false);

                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_bytes)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i16_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MIN, "0123456789"),
                                ::std::make_pair((::pilo::i16_t)-1, "abcdefg"),
                                ::std::make_pair((::pilo::i16_t)0, "tessakkkaz"),
                                ::std::make_pair((::pilo::i16_t)1, "supermario"),
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MAX, "xenoblade"),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_str)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i16_t, std::string>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MIN, std::string("fox1")),
                                ::std::make_pair((::pilo::i16_t)-1, std::string("fox2")),
                                ::std::make_pair((::pilo::i16_t)0, std::string("fox3")),
                                ::std::make_pair((::pilo::i16_t)1, std::string("fox4")),
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MAX, std::string("fox5")),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_tlv)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i16_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MIN, ::pilo::tlv::allocate_single(1.2f)),
                                ::std::make_pair((::pilo::i16_t)-1, ::pilo::tlv::allocate_single((::pilo::u64_t)100010)),
                                ::std::make_pair((::pilo::i16_t)0, ::pilo::tlv::allocate_single(std::string("single variable"))),
                                ::std::make_pair((::pilo::i16_t)1, ::pilo::tlv::allocate_single(true)),
                                ::std::make_pair((::pilo::i16_t)PILO_INT16_MAX, ::pilo::tlv::allocate_single("bytes value", -1, true, false)),
                            }, false);
                    }
                    else
                    {
                        PMC_ASSERT(false);
                    }

                    t->size_ref() = 5;
                    return t;
                }
                else if (kt == ::pilo::core::rtti::wired_type::key_type_u16)
                {
                    if (vt == ::pilo::core::rtti::wired_type::value_type_i8)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u16_t, ::pilo::i8_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u16_t)0, (::pilo::i8_t)PILO_INT8_MIN),
                                ::std::make_pair((::pilo::u16_t)1, (::pilo::i8_t)-100),
                                ::std::make_pair((::pilo::u16_t)2, (::pilo::i8_t)0),
                                ::std::make_pair((::pilo::u16_t)3, (::pilo::i8_t)100),
                                ::std::make_pair((::pilo::u16_t)PILO_UINT16_MAX, (::pilo::i8_t)PILO_INT8_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u8)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u16_t, ::pilo::u8_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u16_t)0, (::pilo::u8_t)0),
                                ::std::make_pair((::pilo::u16_t)1, (::pilo::u8_t)100),
                                ::std::make_pair((::pilo::u16_t)2, (::pilo::u8_t)0),
                                ::std::make_pair((::pilo::u16_t)3, (::pilo::u8_t)200),
                                ::std::make_pair((::pilo::u16_t)PILO_UINT16_MAX, (::pilo::u8_t)PILO_UINT8_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i16)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u16_t, ::pilo::i16_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u16_t)0, (::pilo::i16_t)PILO_INT16_MIN),
                                ::std::make_pair((::pilo::u16_t)1, (::pilo::i16_t)-100),
                                ::std::make_pair((::pilo::u16_t)2, (::pilo::i16_t)0),
                                ::std::make_pair((::pilo::u16_t)3, (::pilo::i16_t)100),
                                ::std::make_pair((::pilo::u16_t)PILO_UINT16_MAX, (::pilo::i16_t)PILO_INT16_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u16)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u16_t, ::pilo::u16_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u16_t)0, (::pilo::u16_t)0),
                                ::std::make_pair((::pilo::u16_t)1, (::pilo::u16_t)10000),
                                ::std::make_pair((::pilo::u16_t)2, (::pilo::u16_t)0),
                                ::std::make_pair((::pilo::u16_t)3, (::pilo::u16_t)20000),
                                ::std::make_pair((::pilo::u16_t)PILO_UINT16_MAX, (::pilo::u16_t)PILO_UINT16_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u16_t, ::pilo::i32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u16_t)0, (::pilo::i32_t)PILO_INT32_MIN),
                                ::std::make_pair((::pilo::u16_t)1, (::pilo::i32_t)-100),
                                ::std::make_pair((::pilo::u16_t)2, (::pilo::i32_t)0),
                                ::std::make_pair((::pilo::u16_t)3, (::pilo::i32_t)100),
                                ::std::make_pair((::pilo::u16_t)PILO_UINT16_MAX, (::pilo::i32_t)PILO_INT32_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u16_t, ::pilo::u32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u16_t)0, (::pilo::u32_t)0),
                                ::std::make_pair((::pilo::u16_t)1, (::pilo::u32_t)10000),
                                ::std::make_pair((::pilo::u16_t)2, (::pilo::u32_t)0),
                                ::std::make_pair((::pilo::u16_t)3, (::pilo::u32_t)20000),
                                ::std::make_pair((::pilo::u16_t)PILO_UINT16_MAX, (::pilo::u32_t)PILO_UINT32_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u16_t, ::pilo::i64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u16_t)0, (::pilo::i64_t)PILO_INT64_MIN),
                                ::std::make_pair((::pilo::u16_t)1, (::pilo::i64_t)-100),
                                ::std::make_pair((::pilo::u16_t)2, (::pilo::i64_t)0),
                                ::std::make_pair((::pilo::u16_t)3, (::pilo::i64_t)100),
                                ::std::make_pair((::pilo::u16_t)PILO_UINT16_MAX, (::pilo::i64_t)PILO_INT64_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u16_t, ::pilo::u64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u16_t)0, (::pilo::u64_t)0),
                                ::std::make_pair((::pilo::u16_t)1, (::pilo::u64_t)100000),
                                ::std::make_pair((::pilo::u16_t)2, (::pilo::u64_t)0),
                                ::std::make_pair((::pilo::u16_t)3, (::pilo::u64_t)200000),
                                ::std::make_pair((::pilo::u16_t)PILO_UINT16_MAX, (::pilo::u64_t)PILO_UINT64_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_boolean)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u16_t, bool>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u16_t)0, (bool)false),
                                ::std::make_pair((::pilo::u16_t)1, (bool)true),
                                ::std::make_pair((::pilo::u16_t)2, (bool)false),
                                ::std::make_pair((::pilo::u16_t)3, (bool)true),
                                ::std::make_pair((::pilo::u16_t)PILO_UINT16_MAX, (bool)false),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_f32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u16_t, ::pilo::f32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u16_t)0, (::pilo::f32_t)1.0f),
                                ::std::make_pair((::pilo::u16_t)1, (::pilo::f32_t)2.0f),
                                ::std::make_pair((::pilo::u16_t)2, (::pilo::f32_t)3.0f),
                                ::std::make_pair((::pilo::u16_t)3, (::pilo::f32_t)4.0f),
                                ::std::make_pair((::pilo::u16_t)PILO_UINT16_MAX, (::pilo::f32_t)5.0f),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_f64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u16_t, ::pilo::f64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u16_t)0, (::pilo::f64_t)11.0),
                                ::std::make_pair((::pilo::u16_t)1, (::pilo::f64_t)12.0),
                                ::std::make_pair((::pilo::u16_t)2, (::pilo::f64_t)13.0),
                                ::std::make_pair((::pilo::u16_t)3, (::pilo::f64_t)14.0),
                                ::std::make_pair((::pilo::u16_t)PILO_UINT16_MAX, (::pilo::f64_t)15.0),
                            }, false);

                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_bytes)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u16_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u16_t)0, "0123456789"),
                                ::std::make_pair((::pilo::u16_t)1, "abcdefg"),
                                ::std::make_pair((::pilo::u16_t)2, "tessakkkaz"),
                                ::std::make_pair((::pilo::u16_t)3, "supermario"),
                                ::std::make_pair((::pilo::u16_t)PILO_UINT16_MAX, "xenoblade"),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_str)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u16_t, std::string>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u16_t)0, std::string("fox1")),
                                ::std::make_pair((::pilo::u16_t)1, std::string("fox2")),
                                ::std::make_pair((::pilo::u16_t)2, std::string("fox3")),
                                ::std::make_pair((::pilo::u16_t)3, std::string("fox4")),
                                ::std::make_pair((::pilo::u16_t)PILO_UINT16_MAX, std::string("fox5")),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_tlv)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u16_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u16_t)0, ::pilo::tlv::allocate_single(1.2f)),
                                ::std::make_pair((::pilo::u16_t)1, ::pilo::tlv::allocate_single((::pilo::u64_t)100010)),
                                ::std::make_pair((::pilo::u16_t)2, ::pilo::tlv::allocate_single(std::string("single variable"))),
                                ::std::make_pair((::pilo::u16_t)3, ::pilo::tlv::allocate_single(true)),
                                ::std::make_pair((::pilo::u16_t)PILO_UINT16_MAX, ::pilo::tlv::allocate_single("bytes value", -1, true, false)),
                            }, false);
                    }
                    else
                    {
                        PMC_ASSERT(false);
                    }

                    t->size_ref() = 5;
                    return t;
                }
                else if (kt == ::pilo::core::rtti::wired_type::key_type_i32)
                {
                    if (vt == ::pilo::core::rtti::wired_type::value_type_i8)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i32_t, ::pilo::i8_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MIN, (::pilo::i8_t)PILO_INT8_MIN),
                                ::std::make_pair((::pilo::i32_t)-1, (::pilo::i8_t)-100),
                                ::std::make_pair((::pilo::i32_t)0, (::pilo::i8_t)0),
                                ::std::make_pair((::pilo::i32_t)1, (::pilo::i8_t)100),
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MAX, (::pilo::i8_t)PILO_INT8_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u8)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i32_t, ::pilo::u8_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MIN, (::pilo::u8_t)0),
                                ::std::make_pair((::pilo::i32_t)-1, (::pilo::u8_t)100),
                                ::std::make_pair((::pilo::i32_t)0, (::pilo::u8_t)0),
                                ::std::make_pair((::pilo::i32_t)2, (::pilo::u8_t)200),
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MAX, (::pilo::u8_t)PILO_UINT8_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i16)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i32_t, ::pilo::i16_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MIN, (::pilo::i16_t)PILO_INT16_MIN),
                                ::std::make_pair((::pilo::i32_t)-1, (::pilo::i16_t)-100),
                                ::std::make_pair((::pilo::i32_t)0, (::pilo::i16_t)0),
                                ::std::make_pair((::pilo::i32_t)1, (::pilo::i16_t)100),
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MAX, (::pilo::i16_t)PILO_INT16_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u16)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i32_t, ::pilo::u16_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MIN, (::pilo::u16_t)0),
                                ::std::make_pair((::pilo::i32_t)-1, (::pilo::u16_t)10000),
                                ::std::make_pair((::pilo::i32_t)0, (::pilo::u16_t)0),
                                ::std::make_pair((::pilo::i32_t)1, (::pilo::u16_t)20000),
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MAX, (::pilo::u16_t)PILO_UINT16_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i32_t, ::pilo::i32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MIN, (::pilo::i32_t)PILO_INT32_MIN),
                                ::std::make_pair((::pilo::i32_t)-1, (::pilo::i32_t)-100),
                                ::std::make_pair((::pilo::i32_t)0, (::pilo::i32_t)0),
                                ::std::make_pair((::pilo::i32_t)1, (::pilo::i32_t)100),
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MAX, (::pilo::i32_t)PILO_INT32_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i32_t, ::pilo::u32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MIN, (::pilo::u32_t)0),
                                ::std::make_pair((::pilo::i32_t)-1, (::pilo::u32_t)10000),
                                ::std::make_pair((::pilo::i32_t)0, (::pilo::u32_t)0),
                                ::std::make_pair((::pilo::i32_t)1, (::pilo::u32_t)20000),
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MAX, (::pilo::u32_t)PILO_UINT32_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i32_t, ::pilo::i64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MIN, (::pilo::i64_t)PILO_INT64_MIN),
                                ::std::make_pair((::pilo::i32_t)-1, (::pilo::i64_t)-100),
                                ::std::make_pair((::pilo::i32_t)0, (::pilo::i64_t)0),
                                ::std::make_pair((::pilo::i32_t)1, (::pilo::i64_t)100),
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MAX, (::pilo::i64_t)PILO_INT64_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i32_t, ::pilo::u64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MIN, (::pilo::u64_t)0),
                                ::std::make_pair((::pilo::i32_t)-1, (::pilo::u64_t)100000),
                                ::std::make_pair((::pilo::i32_t)0, (::pilo::u64_t)0),
                                ::std::make_pair((::pilo::i32_t)1, (::pilo::u64_t)200000),
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MAX, (::pilo::u64_t)PILO_UINT64_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_boolean)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i32_t, bool>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MIN, (bool)false),
                                ::std::make_pair((::pilo::i32_t)-1, (bool)true),
                                ::std::make_pair((::pilo::i32_t)0, (bool)false),
                                ::std::make_pair((::pilo::i32_t)1, (bool)true),
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MAX, (bool)false),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_f32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i32_t, ::pilo::f32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MIN, (::pilo::f32_t)1.0f),
                                ::std::make_pair((::pilo::i32_t)-1, (::pilo::f32_t)2.0f),
                                ::std::make_pair((::pilo::i32_t)0, (::pilo::f32_t)3.0f),
                                ::std::make_pair((::pilo::i32_t)1, (::pilo::f32_t)4.0f),
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MAX, (::pilo::f32_t)5.0f),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_f64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i32_t, ::pilo::f64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MIN, (::pilo::f64_t)11.0),
                                ::std::make_pair((::pilo::i32_t)-1, (::pilo::f64_t)12.0),
                                ::std::make_pair((::pilo::i32_t)0, (::pilo::f64_t)13.0),
                                ::std::make_pair((::pilo::i32_t)1, (::pilo::f64_t)14.0),
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MAX, (::pilo::f64_t)15.0),
                            }, false);

                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_bytes)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MIN, "0123456789"),
                                ::std::make_pair((::pilo::i32_t)-1, "abcdefg"),
                                ::std::make_pair((::pilo::i32_t)0, "tessakkkaz"),
                                ::std::make_pair((::pilo::i32_t)1, "supermario"),
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MAX, "xenoblade"),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_str)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i32_t, std::string>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MIN, std::string("fox1")),
                                ::std::make_pair((::pilo::i32_t)-1, std::string("fox2")),
                                ::std::make_pair((::pilo::i32_t)0, std::string("fox3")),
                                ::std::make_pair((::pilo::i32_t)1, std::string("fox4")),
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MAX, std::string("fox5")),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_tlv)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MIN, ::pilo::tlv::allocate_single(1.2f)),
                                ::std::make_pair((::pilo::i32_t)-1, ::pilo::tlv::allocate_single((::pilo::u64_t)100010)),
                                ::std::make_pair((::pilo::i32_t)0, ::pilo::tlv::allocate_single(std::string("single variable"))),
                                ::std::make_pair((::pilo::i32_t)1, ::pilo::tlv::allocate_single(true)),
                                ::std::make_pair((::pilo::i32_t)PILO_INT32_MAX, ::pilo::tlv::allocate_single("bytes value", -1, true, false)),
                            }, false);
                    }
                    else
                    {
                        PMC_ASSERT(false);
                    }

                    t->size_ref() = 5;
                    return t;
                }
                else if (kt == ::pilo::core::rtti::wired_type::key_type_u32)
                {
                    if (vt == ::pilo::core::rtti::wired_type::value_type_i8)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u32_t, ::pilo::i8_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u32_t)0, (::pilo::i8_t)PILO_INT8_MIN),
                                ::std::make_pair((::pilo::u32_t)1, (::pilo::i8_t)-100),
                                ::std::make_pair((::pilo::u32_t)2, (::pilo::i8_t)0),
                                ::std::make_pair((::pilo::u32_t)3, (::pilo::i8_t)100),
                                ::std::make_pair((::pilo::u32_t)PILO_UINT32_MAX, (::pilo::i8_t)PILO_INT8_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u8)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u32_t, ::pilo::u8_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u32_t)0, (::pilo::u8_t)0),
                                ::std::make_pair((::pilo::u32_t)1, (::pilo::u8_t)100),
                                ::std::make_pair((::pilo::u32_t)2, (::pilo::u8_t)0),
                                ::std::make_pair((::pilo::u32_t)3, (::pilo::u8_t)200),
                                ::std::make_pair((::pilo::u32_t)PILO_UINT32_MAX, (::pilo::u8_t)PILO_UINT8_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i16)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u32_t, ::pilo::i16_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u32_t)0, (::pilo::i16_t)PILO_INT16_MIN),
                                ::std::make_pair((::pilo::u32_t)1, (::pilo::i16_t)-100),
                                ::std::make_pair((::pilo::u32_t)2, (::pilo::i16_t)0),
                                ::std::make_pair((::pilo::u32_t)3, (::pilo::i16_t)100),
                                ::std::make_pair((::pilo::u32_t)PILO_UINT32_MAX, (::pilo::i16_t)PILO_INT16_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u16)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u32_t, ::pilo::u16_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u32_t)0, (::pilo::u16_t)0),
                                ::std::make_pair((::pilo::u32_t)1, (::pilo::u16_t)10000),
                                ::std::make_pair((::pilo::u32_t)2, (::pilo::u16_t)0),
                                ::std::make_pair((::pilo::u32_t)3, (::pilo::u16_t)20000),
                                ::std::make_pair((::pilo::u32_t)PILO_UINT32_MAX, (::pilo::u16_t)PILO_UINT16_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u32_t, ::pilo::i32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u32_t)0, (::pilo::i32_t)PILO_INT32_MIN),
                                ::std::make_pair((::pilo::u32_t)1, (::pilo::i32_t)-100),
                                ::std::make_pair((::pilo::u32_t)2, (::pilo::i32_t)0),
                                ::std::make_pair((::pilo::u32_t)3, (::pilo::i32_t)100),
                                ::std::make_pair((::pilo::u32_t)PILO_UINT32_MAX, (::pilo::i32_t)PILO_INT32_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u32_t, ::pilo::u32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u32_t)0, (::pilo::u32_t)0),
                                ::std::make_pair((::pilo::u32_t)1, (::pilo::u32_t)10000),
                                ::std::make_pair((::pilo::u32_t)2, (::pilo::u32_t)0),
                                ::std::make_pair((::pilo::u32_t)3, (::pilo::u32_t)20000),
                                ::std::make_pair((::pilo::u32_t)PILO_UINT32_MAX, (::pilo::u32_t)PILO_UINT32_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u32_t, ::pilo::i64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u32_t)0, (::pilo::i64_t)PILO_INT64_MIN),
                                ::std::make_pair((::pilo::u32_t)1, (::pilo::i64_t)-100),
                                ::std::make_pair((::pilo::u32_t)2, (::pilo::i64_t)0),
                                ::std::make_pair((::pilo::u32_t)3, (::pilo::i64_t)100),
                                ::std::make_pair((::pilo::u32_t)PILO_UINT32_MAX, (::pilo::i64_t)PILO_INT64_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u32_t, ::pilo::u64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u32_t)0, (::pilo::u64_t)0),
                                ::std::make_pair((::pilo::u32_t)1, (::pilo::u64_t)100000),
                                ::std::make_pair((::pilo::u32_t)2, (::pilo::u64_t)0),
                                ::std::make_pair((::pilo::u32_t)3, (::pilo::u64_t)200000),
                                ::std::make_pair((::pilo::u32_t)PILO_UINT32_MAX, (::pilo::u64_t)PILO_UINT64_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_boolean)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u32_t, bool>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u32_t)0, (bool)false),
                                ::std::make_pair((::pilo::u32_t)1, (bool)true),
                                ::std::make_pair((::pilo::u32_t)2, (bool)false),
                                ::std::make_pair((::pilo::u32_t)3, (bool)true),
                                ::std::make_pair((::pilo::u32_t)PILO_UINT32_MAX, (bool)false),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_f32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u32_t, ::pilo::f32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u32_t)0, (::pilo::f32_t)1.0f),
                                ::std::make_pair((::pilo::u32_t)1, (::pilo::f32_t)2.0f),
                                ::std::make_pair((::pilo::u32_t)2, (::pilo::f32_t)3.0f),
                                ::std::make_pair((::pilo::u32_t)3, (::pilo::f32_t)4.0f),
                                ::std::make_pair((::pilo::u32_t)PILO_UINT32_MAX, (::pilo::f32_t)5.0f),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_f64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u32_t, ::pilo::f64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u32_t)0, (::pilo::f64_t)11.0),
                                ::std::make_pair((::pilo::u32_t)1, (::pilo::f64_t)12.0),
                                ::std::make_pair((::pilo::u32_t)2, (::pilo::f64_t)13.0),
                                ::std::make_pair((::pilo::u32_t)3, (::pilo::f64_t)14.0),
                                ::std::make_pair((::pilo::u32_t)PILO_UINT32_MAX, (::pilo::f64_t)15.0),
                            }, false);

                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_bytes)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u32_t)0, "0123456789"),
                                ::std::make_pair((::pilo::u32_t)1, "abcdefg"),
                                ::std::make_pair((::pilo::u32_t)2, "tessakkkaz"),
                                ::std::make_pair((::pilo::u32_t)3, "supermario"),
                                ::std::make_pair((::pilo::u32_t)PILO_UINT32_MAX, "xenoblade"),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_str)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u32_t, std::string>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u32_t)0, std::string("fox1")),
                                ::std::make_pair((::pilo::u32_t)1, std::string("fox2")),
                                ::std::make_pair((::pilo::u32_t)2, std::string("fox3")),
                                ::std::make_pair((::pilo::u32_t)3, std::string("fox4")),
                                ::std::make_pair((::pilo::u32_t)PILO_UINT32_MAX, std::string("fox5")),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_tlv)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u32_t)0, ::pilo::tlv::allocate_single(1.2f)),
                                ::std::make_pair((::pilo::u32_t)1, ::pilo::tlv::allocate_single((::pilo::u64_t)100010)),
                                ::std::make_pair((::pilo::u32_t)2, ::pilo::tlv::allocate_single(std::string("single variable"))),
                                ::std::make_pair((::pilo::u32_t)3, ::pilo::tlv::allocate_single(true)),
                                ::std::make_pair((::pilo::u32_t)PILO_UINT32_MAX, ::pilo::tlv::allocate_single("bytes value", -1, true, false)),
                            }, false);
                    }
                    else
                    {
                        PMC_ASSERT(false);
                    }

                    t->size_ref() = 5;
                    return t;
                }
                else if (kt == ::pilo::core::rtti::wired_type::key_type_i64)
                {
                    if (vt == ::pilo::core::rtti::wired_type::value_type_i8)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i64_t, ::pilo::i8_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MIN, (::pilo::i8_t)PILO_INT8_MIN),
                                ::std::make_pair((::pilo::i64_t)-1, (::pilo::i8_t)-100),
                                ::std::make_pair((::pilo::i64_t)0, (::pilo::i8_t)0),
                                ::std::make_pair((::pilo::i64_t)1, (::pilo::i8_t)100),
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MAX, (::pilo::i8_t)PILO_INT8_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u8)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i64_t, ::pilo::u8_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MIN, (::pilo::u8_t)0),
                                ::std::make_pair((::pilo::i64_t)-1, (::pilo::u8_t)100),
                                ::std::make_pair((::pilo::i64_t)0, (::pilo::u8_t)0),
                                ::std::make_pair((::pilo::i64_t)2, (::pilo::u8_t)200),
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MAX, (::pilo::u8_t)PILO_UINT8_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i16)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i64_t, ::pilo::i16_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MIN, (::pilo::i16_t)PILO_INT16_MIN),
                                ::std::make_pair((::pilo::i64_t)-1, (::pilo::i16_t)-100),
                                ::std::make_pair((::pilo::i64_t)0, (::pilo::i16_t)0),
                                ::std::make_pair((::pilo::i64_t)1, (::pilo::i16_t)100),
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MAX, (::pilo::i16_t)PILO_INT16_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u16)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i64_t, ::pilo::u16_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MIN, (::pilo::u16_t)0),
                                ::std::make_pair((::pilo::i64_t)-1, (::pilo::u16_t)10000),
                                ::std::make_pair((::pilo::i64_t)0, (::pilo::u16_t)0),
                                ::std::make_pair((::pilo::i64_t)1, (::pilo::u16_t)20000),
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MAX, (::pilo::u16_t)PILO_UINT16_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i64_t, ::pilo::i32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MIN, (::pilo::i32_t)PILO_INT32_MIN),
                                ::std::make_pair((::pilo::i64_t)-1, (::pilo::i32_t)-100),
                                ::std::make_pair((::pilo::i64_t)0, (::pilo::i32_t)0),
                                ::std::make_pair((::pilo::i64_t)1, (::pilo::i32_t)100),
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MAX, (::pilo::i32_t)PILO_INT32_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i64_t, ::pilo::u32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MIN, (::pilo::u32_t)0),
                                ::std::make_pair((::pilo::i64_t)-1, (::pilo::u32_t)10000),
                                ::std::make_pair((::pilo::i64_t)0, (::pilo::u32_t)0),
                                ::std::make_pair((::pilo::i64_t)1, (::pilo::u32_t)20000),
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MAX, (::pilo::u32_t)PILO_UINT32_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i64_t, ::pilo::i64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MIN, (::pilo::i64_t)PILO_INT64_MIN),
                                ::std::make_pair((::pilo::i64_t)-1, (::pilo::i64_t)-100),
                                ::std::make_pair((::pilo::i64_t)0, (::pilo::i64_t)0),
                                ::std::make_pair((::pilo::i64_t)1, (::pilo::i64_t)100),
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MAX, (::pilo::i64_t)PILO_INT64_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i64_t, ::pilo::u64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MIN, (::pilo::u64_t)0),
                                ::std::make_pair((::pilo::i64_t)-1, (::pilo::u64_t)100000),
                                ::std::make_pair((::pilo::i64_t)0, (::pilo::u64_t)0),
                                ::std::make_pair((::pilo::i64_t)1, (::pilo::u64_t)200000),
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MAX, (::pilo::u64_t)PILO_UINT64_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_boolean)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i64_t, bool>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MIN, (bool)false),
                                ::std::make_pair((::pilo::i64_t)-1, (bool)true),
                                ::std::make_pair((::pilo::i64_t)0, (bool)false),
                                ::std::make_pair((::pilo::i64_t)1, (bool)true),
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MAX, (bool)false),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_f32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i64_t, ::pilo::f32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MIN, (::pilo::f32_t)1.0f),
                                ::std::make_pair((::pilo::i64_t)-1, (::pilo::f32_t)2.0f),
                                ::std::make_pair((::pilo::i64_t)0, (::pilo::f32_t)3.0f),
                                ::std::make_pair((::pilo::i64_t)1, (::pilo::f32_t)4.0f),
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MAX, (::pilo::f32_t)5.0f),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_f64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i64_t, ::pilo::f64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MIN, (::pilo::f64_t)11.0),
                                ::std::make_pair((::pilo::i64_t)-1, (::pilo::f64_t)12.0),
                                ::std::make_pair((::pilo::i64_t)0, (::pilo::f64_t)13.0),
                                ::std::make_pair((::pilo::i64_t)1, (::pilo::f64_t)14.0),
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MAX, (::pilo::f64_t)15.0),
                            }, false);

                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_bytes)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MIN, "0123456789"),
                                ::std::make_pair((::pilo::i64_t)-1, "abcdefg"),
                                ::std::make_pair((::pilo::i64_t)0, "tessakkkaz"),
                                ::std::make_pair((::pilo::i64_t)1, "supermario"),
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MAX, "xenoblade"),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_str)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::i64_t, std::string>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MIN, std::string("fox1")),
                                ::std::make_pair((::pilo::i64_t)-1, std::string("fox2")),
                                ::std::make_pair((::pilo::i64_t)0, std::string("fox3")),
                                ::std::make_pair((::pilo::i64_t)1, std::string("fox4")),
                                ::std::make_pair((::pilo::i64_t)PILO_INT64_MAX, std::string("fox5")),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_tlv)
                    {
                        if (level < 1)
                        {
                            ::pilo::core::rtti::map_ptr_insert<::pilo::i64_t>(t->daynamic_data_ref(),
                                {
                                    ::std::make_pair((::pilo::i64_t)PILO_INT64_MIN, ::pilo::tlv::allocate_single(1.2f)),
                                    ::std::make_pair((::pilo::i64_t)-1, ::pilo::tlv::allocate_single((::pilo::u64_t)100010)),
                                    ::std::make_pair((::pilo::i64_t)0, ::pilo::tlv::allocate_single(std::string("single variable"))),
                                    ::std::make_pair((::pilo::i64_t)1, ::pilo::tlv::allocate_single(true)),
                                    ::std::make_pair((::pilo::i64_t)PILO_INT64_MAX, ::pilo::tlv::allocate_single("bytes value", -1, true, false)),
                                }, false);
                        }
                        else
                        {
                            ::pilo::tlv* arr_t = create_random_array_tlv(level - 1);
                            if (arr_t == nullptr)
                            {
                                return nullptr;
                            }
                            ::pilo::core::rtti::map_ptr_insert<::pilo::i64_t>(t->daynamic_data_ref(),
                                {
                                    ::std::make_pair((::pilo::i64_t)0, arr_t),
                                }, false);

                            t->size_ref() = 1;
                            return t;
                        }
                        
                    }
                    else
                    {
                        PMC_ASSERT(false);
                    }

                    t->size_ref() = 5;
                    return t;
                }
                else if (kt == ::pilo::core::rtti::wired_type::key_type_u64)
                {
                    if (vt == ::pilo::core::rtti::wired_type::value_type_i8)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u64_t, ::pilo::i8_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u64_t)0, (::pilo::i8_t)PILO_INT8_MIN),
                                ::std::make_pair((::pilo::u64_t)1, (::pilo::i8_t)-100),
                                ::std::make_pair((::pilo::u64_t)2, (::pilo::i8_t)0),
                                ::std::make_pair((::pilo::u64_t)3, (::pilo::i8_t)100),
                                ::std::make_pair((::pilo::u64_t)PILO_UINT64_MAX, (::pilo::i8_t)PILO_INT8_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u8)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u64_t, ::pilo::u8_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u64_t)0, (::pilo::u8_t)0),
                                ::std::make_pair((::pilo::u64_t)1, (::pilo::u8_t)100),
                                ::std::make_pair((::pilo::u64_t)2, (::pilo::u8_t)0),
                                ::std::make_pair((::pilo::u64_t)3, (::pilo::u8_t)200),
                                ::std::make_pair((::pilo::u64_t)PILO_UINT64_MAX, (::pilo::u8_t)PILO_UINT8_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i16)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u64_t, ::pilo::i16_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u64_t)0, (::pilo::i16_t)PILO_INT16_MIN),
                                ::std::make_pair((::pilo::u64_t)1, (::pilo::i16_t)-100),
                                ::std::make_pair((::pilo::u64_t)2, (::pilo::i16_t)0),
                                ::std::make_pair((::pilo::u64_t)3, (::pilo::i16_t)100),
                                ::std::make_pair((::pilo::u64_t)PILO_UINT64_MAX, (::pilo::i16_t)PILO_INT16_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u16)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u64_t, ::pilo::u16_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u64_t)0, (::pilo::u16_t)0),
                                ::std::make_pair((::pilo::u64_t)1, (::pilo::u16_t)10000),
                                ::std::make_pair((::pilo::u64_t)2, (::pilo::u16_t)0),
                                ::std::make_pair((::pilo::u64_t)3, (::pilo::u16_t)20000),
                                ::std::make_pair((::pilo::u64_t)PILO_UINT64_MAX, (::pilo::u16_t)PILO_UINT16_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u64_t, ::pilo::i32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u64_t)0, (::pilo::i32_t)PILO_INT32_MIN),
                                ::std::make_pair((::pilo::u64_t)1, (::pilo::i32_t)-100),
                                ::std::make_pair((::pilo::u64_t)2, (::pilo::i32_t)0),
                                ::std::make_pair((::pilo::u64_t)3, (::pilo::i32_t)100),
                                ::std::make_pair((::pilo::u64_t)PILO_UINT64_MAX, (::pilo::i32_t)PILO_INT32_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u64_t, ::pilo::u32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u64_t)0, (::pilo::u32_t)0),
                                ::std::make_pair((::pilo::u64_t)1, (::pilo::u32_t)10000),
                                ::std::make_pair((::pilo::u64_t)2, (::pilo::u32_t)0),
                                ::std::make_pair((::pilo::u64_t)3, (::pilo::u32_t)20000),
                                ::std::make_pair((::pilo::u64_t)PILO_UINT64_MAX, (::pilo::u32_t)PILO_UINT32_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u64_t, ::pilo::i64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u64_t)0, (::pilo::i64_t)PILO_INT64_MIN),
                                ::std::make_pair((::pilo::u64_t)1, (::pilo::i64_t)-100),
                                ::std::make_pair((::pilo::u64_t)2, (::pilo::i64_t)0),
                                ::std::make_pair((::pilo::u64_t)3, (::pilo::i64_t)100),
                                ::std::make_pair((::pilo::u64_t)PILO_UINT64_MAX, (::pilo::i64_t)PILO_INT64_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u64_t, ::pilo::u64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u64_t)0, (::pilo::u64_t)0),
                                ::std::make_pair((::pilo::u64_t)1, (::pilo::u64_t)100000),
                                ::std::make_pair((::pilo::u64_t)2, (::pilo::u64_t)0),
                                ::std::make_pair((::pilo::u64_t)3, (::pilo::u64_t)200000),
                                ::std::make_pair((::pilo::u64_t)PILO_UINT64_MAX, (::pilo::u64_t)PILO_UINT64_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_boolean)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u64_t, bool>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u64_t)0, (bool)false),
                                ::std::make_pair((::pilo::u64_t)1, (bool)true),
                                ::std::make_pair((::pilo::u64_t)2, (bool)false),
                                ::std::make_pair((::pilo::u64_t)3, (bool)true),
                                ::std::make_pair((::pilo::u64_t)PILO_UINT64_MAX, (bool)false),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_f32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u64_t, ::pilo::f32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u64_t)0, (::pilo::f32_t)1.0f),
                                ::std::make_pair((::pilo::u64_t)1, (::pilo::f32_t)2.0f),
                                ::std::make_pair((::pilo::u64_t)2, (::pilo::f32_t)3.0f),
                                ::std::make_pair((::pilo::u64_t)3, (::pilo::f32_t)4.0f),
                                ::std::make_pair((::pilo::u64_t)PILO_UINT64_MAX, (::pilo::f32_t)5.0f),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_f64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u64_t, ::pilo::f64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u64_t)0, (::pilo::f64_t)11.0),
                                ::std::make_pair((::pilo::u64_t)1, (::pilo::f64_t)12.0),
                                ::std::make_pair((::pilo::u64_t)2, (::pilo::f64_t)13.0),
                                ::std::make_pair((::pilo::u64_t)3, (::pilo::f64_t)14.0),
                                ::std::make_pair((::pilo::u64_t)PILO_UINT64_MAX, (::pilo::f64_t)15.0),
                            }, false);

                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_bytes)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u64_t)0, "0123456789"),
                                ::std::make_pair((::pilo::u64_t)1, "abcdefg"),
                                ::std::make_pair((::pilo::u64_t)2, "tessakkkaz"),
                                ::std::make_pair((::pilo::u64_t)3, "supermario"),
                                ::std::make_pair((::pilo::u64_t)PILO_UINT64_MAX, "xenoblade"),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_str)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u64_t, std::string>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u64_t)0, std::string("fox1")),
                                ::std::make_pair((::pilo::u64_t)1, std::string("fox2")),
                                ::std::make_pair((::pilo::u64_t)2, std::string("fox3")),
                                ::std::make_pair((::pilo::u64_t)3, std::string("fox4")),
                                ::std::make_pair((::pilo::u64_t)PILO_UINT64_MAX, std::string("fox5")),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_tlv)
                    {
                        ::pilo::core::rtti::map_ptr_insert<::pilo::u64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair((::pilo::u64_t)0, ::pilo::tlv::allocate_single(1.2f)),
                                ::std::make_pair((::pilo::u64_t)1, ::pilo::tlv::allocate_single((::pilo::u64_t)100010)),
                                ::std::make_pair((::pilo::u64_t)2, ::pilo::tlv::allocate_single(std::string("single variable"))),
                                ::std::make_pair((::pilo::u64_t)3, ::pilo::tlv::allocate_single(true)),
                                ::std::make_pair((::pilo::u64_t)PILO_UINT64_MAX, ::pilo::tlv::allocate_single("bytes value", -1, true, false)),
                            }, false);
                    }
                    else
                    {
                        PMC_ASSERT(false);
                    }

                    t->size_ref() = 5;
                    return t;
                }
                else if (kt == ::pilo::core::rtti::wired_type::key_type_str)
                {
                    if (vt == ::pilo::core::rtti::wired_type::value_type_i8)
                    {
                        ::pilo::core::rtti::map_ptr_insert<std::string, ::pilo::i8_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair(std::string("key_0"), (::pilo::i8_t)PILO_INT8_MIN),
                                ::std::make_pair(std::string("key_1"), (::pilo::i8_t)-100),
                                ::std::make_pair(std::string("key_2"), (::pilo::i8_t)0),
                                ::std::make_pair(std::string("key_3"), (::pilo::i8_t)100),
                                ::std::make_pair(std::string("key_4"), (::pilo::i8_t)PILO_INT8_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u8)
                    {
                        ::pilo::core::rtti::map_ptr_insert<std::string, ::pilo::u8_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair(std::string("key_0"), (::pilo::u8_t)0),
                                ::std::make_pair(std::string("key_1"), (::pilo::u8_t)100),
                                ::std::make_pair(std::string("key_2"), (::pilo::u8_t)0),
                                ::std::make_pair(std::string("key_3"), (::pilo::u8_t)200),
                                ::std::make_pair(std::string("key_4"), (::pilo::u8_t)PILO_UINT8_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i16)
                    {
                        ::pilo::core::rtti::map_ptr_insert<std::string, ::pilo::i16_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair(std::string("key_0"), (::pilo::i16_t)PILO_INT16_MIN),
                                ::std::make_pair(std::string("key_1"), (::pilo::i16_t)-100),
                                ::std::make_pair(std::string("key_2"), (::pilo::i16_t)0),
                                ::std::make_pair(std::string("key_3"), (::pilo::i16_t)100),
                                ::std::make_pair(std::string("key_4"), (::pilo::i16_t)PILO_INT16_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u16)
                    {
                        ::pilo::core::rtti::map_ptr_insert<std::string, ::pilo::u16_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair(std::string("key_0"), (::pilo::u16_t)0),
                                ::std::make_pair(std::string("key_1"), (::pilo::u16_t)10000),
                                ::std::make_pair(std::string("key_2"), (::pilo::u16_t)0),
                                ::std::make_pair(std::string("key_3"), (::pilo::u16_t)20000),
                                ::std::make_pair(std::string("key_4"), (::pilo::u16_t)PILO_UINT16_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<std::string, ::pilo::i32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair(std::string("key_0"), (::pilo::i32_t)PILO_INT32_MIN),
                                ::std::make_pair(std::string("key_1"), (::pilo::i32_t)-100),
                                ::std::make_pair(std::string("key_2"), (::pilo::i32_t)0),
                                ::std::make_pair(std::string("key_3"), (::pilo::i32_t)100),
                                ::std::make_pair(std::string("key_4"), (::pilo::i32_t)PILO_INT32_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<std::string, ::pilo::u32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair(std::string("key_0"), (::pilo::u32_t)0),
                                ::std::make_pair(std::string("key_1"), (::pilo::u32_t)10000),
                                ::std::make_pair(std::string("key_2"), (::pilo::u32_t)0),
                                ::std::make_pair(std::string("key_3"), (::pilo::u32_t)20000),
                                ::std::make_pair(std::string("key_4"), (::pilo::u32_t)PILO_UINT32_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_i64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<std::string, ::pilo::i64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair(std::string("key_0"), (::pilo::i64_t)PILO_INT64_MIN),
                                ::std::make_pair(std::string("key_1"), (::pilo::i64_t)-100),
                                ::std::make_pair(std::string("key_2"), (::pilo::i64_t)0),
                                ::std::make_pair(std::string("key_3"), (::pilo::i64_t)100),
                                ::std::make_pair(std::string("key_4"), (::pilo::i64_t)PILO_INT64_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_u64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<std::string, ::pilo::u64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair(std::string("key_0"), (::pilo::u64_t)0),
                                ::std::make_pair(std::string("key_1"), (::pilo::u64_t)100000),
                                ::std::make_pair(std::string("key_2"), (::pilo::u64_t)0),
                                ::std::make_pair(std::string("key_3"), (::pilo::u64_t)200000),
                                ::std::make_pair(std::string("key_4"), (::pilo::u64_t)PILO_UINT64_MAX),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_boolean)
                    {
                        ::pilo::core::rtti::map_ptr_insert<std::string, bool>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair(std::string("key_0"), (bool)false),
                                ::std::make_pair(std::string("key_1"), (bool)true),
                                ::std::make_pair(std::string("key_2"), (bool)false),
                                ::std::make_pair(std::string("key_3"), (bool)true),
                                ::std::make_pair(std::string("key_4"), (bool)false),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_f32)
                    {
                        ::pilo::core::rtti::map_ptr_insert<std::string, ::pilo::f32_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair(std::string("key_0"), (::pilo::f32_t)1.0f),
                                ::std::make_pair(std::string("key_1"), (::pilo::f32_t)2.0f),
                                ::std::make_pair(std::string("key_2"), (::pilo::f32_t)3.0f),
                                ::std::make_pair(std::string("key_3"), (::pilo::f32_t)4.0f),
                                ::std::make_pair(std::string("key_4"), (::pilo::f32_t)5.0f),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_f64)
                    {
                        ::pilo::core::rtti::map_ptr_insert<std::string, ::pilo::f64_t>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair(std::string("key_0"), (::pilo::f64_t)11.0),
                                ::std::make_pair(std::string("key_1"), (::pilo::f64_t)12.0),
                                ::std::make_pair(std::string("key_2"), (::pilo::f64_t)13.0),
                                ::std::make_pair(std::string("key_3"), (::pilo::f64_t)14.0),
                                ::std::make_pair(std::string("key_4"), (::pilo::f64_t)15.0),
                            }, false);

                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_bytes)
                    {
                        ::pilo::core::rtti::map_ptr_insert<std::string>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair(std::string("key_0"), "0123456789"),
                                ::std::make_pair(std::string("key_1"), "abcdefg"),
                                ::std::make_pair(std::string("key_2"), "tessakkkaz"),
                                ::std::make_pair(std::string("key_3"), "supermario"),
                                ::std::make_pair(std::string("key_4"), "xenoblade"),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_str)
                    {
                        ::pilo::core::rtti::map_ptr_insert<std::string, std::string>(t->daynamic_data_ref(),
                            {
                                ::std::make_pair(std::string("key_0"), std::string("fox1")),
                                ::std::make_pair(std::string("key_1"), std::string("fox2")),
                                ::std::make_pair(std::string("key_2"), std::string("fox3")),
                                ::std::make_pair(std::string("key_3"), std::string("fox4")),
                                ::std::make_pair(std::string("key_4"), std::string("fox5")),
                            }, false);
                    }
                    else if (vt == ::pilo::core::rtti::wired_type::value_type_tlv)
                    {
                        if (level < 1)
                        {
                            ::pilo::core::rtti::map_ptr_insert<std::string>(t->daynamic_data_ref(),
                                {
                                    ::std::make_pair(std::string("key_0"), ::pilo::tlv::allocate_single(1.2f)),
                                    ::std::make_pair(std::string("key_1"), ::pilo::tlv::allocate_single((::pilo::u64_t)100010)),
                                    ::std::make_pair(std::string("key_2"), ::pilo::tlv::allocate_single(std::string("single variable"))),
                                    ::std::make_pair(std::string("key_3"), ::pilo::tlv::allocate_single(true)),
                                    ::std::make_pair(std::string("key_4"), ::pilo::tlv::allocate_single("bytes value", -1, true, false)),
                                }, false);
                        }
                        else
                        {
                            ::pilo::tlv * arr_t = create_random_array_tlv(level - 1);
                            if (arr_t == nullptr)
                            {
                                return nullptr;
                            }
                            ::pilo::core::rtti::map_ptr_insert<std::string>(t->daynamic_data_ref(),
                                {
                                    ::std::make_pair(std::string("arr_0"), arr_t),
                                }, false);

                            t->size_ref() = 1;
                            return t;

                        }
                        
                    }
                    else
                    {
                        PMC_ASSERT(false);
                    }

                    t->size_ref() = 5;
                    return t;
                }
                else
                {
                    PMC_ASSERT(false);
                }

                return t;
            }

			::pilo::tlv* create_random_single_tlv(::pilo::u16_t which_idx)
			{
				::pilo::tlv* ret = ::pilo::tlv::allocate();
				if (ret == nullptr)
				{
					PMC_ASSERT(false );
					return nullptr;
				}

                ::pilo::i64_t ts = rand();

                if (which_idx == ::pilo::core::rtti::wired_type::value_type_na)
                {
                    which_idx = (::pilo::u16_t)(rand() % (::pilo::core::rtti::wired_type::value_type_intrincs_count));
                    if (which_idx == ::pilo::core::rtti::wired_type::value_type_na)
                    {
                        which_idx = ::pilo::core::rtti::wired_type::value_type_str;
                    }
                }
   
                if (which_idx == ::pilo::core::rtti::wired_type::value_type_tlv)
                {
                    which_idx = ::pilo::core::rtti::wired_type::value_type_str;
                }
                if (which_idx == ::pilo::core::rtti::wired_type::value_type_i8)
                {
                    ret->set((::pilo::i8_t) (127 - (ts % 256)));
                }
                else if (which_idx == ::pilo::core::rtti::wired_type::value_type_u8)
                {
                    ret->set((::pilo::u8_t)((ts % 256)));
                }
                else if (which_idx == ::pilo::core::rtti::wired_type::value_type_i16)
                {
                    ret->set((::pilo::i16_t)(32767 - (ts % 65536)));
                }
                else if (which_idx == ::pilo::core::rtti::wired_type::value_type_u16)
                {
                    ret->set((::pilo::u16_t)((ts % 65536)));
                }
                else if (which_idx == ::pilo::core::rtti::wired_type::value_type_i32)
                {
                    ret->set((::pilo::u32_t)(PILO_INT32_MAX - (ts % PILO_UINT32_MAX)));
                }
                else if (which_idx == ::pilo::core::rtti::wired_type::value_type_u32)
                {
                    ret->set((::pilo::u32_t)((ts % PILO_UINT32_MAX)));
                }
                else if (which_idx == ::pilo::core::rtti::wired_type::value_type_i64)
                {
                    ret->set((::pilo::u32_t)(PILO_INT64_MAX - (ts % PILO_UINT64_MAX)));
                }
                else if (which_idx == ::pilo::core::rtti::wired_type::value_type_u64)
                {
                    ret->set((::pilo::u32_t)((ts % PILO_UINT64_MAX)));
                }
                else if (which_idx == ::pilo::core::rtti::wired_type::value_type_boolean)
                {
                    ret->set(true);
                }
                else if (which_idx == ::pilo::core::rtti::wired_type::value_type_f32)
                {
                    ret->set((::pilo::f32_t) ( ((::pilo::i32_t)ts) / 3.0f));
                }
                else if (which_idx == ::pilo::core::rtti::wired_type::value_type_f64)
                {
                    ret->set((::pilo::f64_t)(ts / 3.0f));
                }
                else if (which_idx == ::pilo::core::rtti::wired_type::value_type_str)
                {
                    int mode = ts % 3;
                    if (mode == 0)
                    {
                        ret->set_string(nullptr, -1);
                    }
                    else if (mode == 1)
                    {
                        ret->set_string(nullptr, 0);
                    }
                    else
                    {
                        std::string str;
                        create_random_memory_sample(str, 0, 1025);
                        ret->set(str);
                    }
                }
                else if (which_idx == ::pilo::core::rtti::wired_type::value_type_bytes)
                {
                    ::pilo::i32_t rlen = 0;
                    char * p = create_random_memory_sample(nullptr, 0, rlen , 0, 777);
                    ret->set_bytes(p, rlen, true);

                }
                else if (which_idx == ::pilo::core::rtti::wired_type::value_type_tlv)
                {
                    PMC_ASSERT(false);
                }
                else
                {
                    
                }
                return ret;
			}


            int validate_path(::pilo::core::io::path* p, const char* pc, ::pilo::pathlen_t len
                , const char* fn, const char* en, const char* pardir, const char* bn, ::pilo::i8_t at)
            {
                if (p == nullptr) return -1;
                if (p->length() != len)
                {
                    return -2;
                }
                if (::pilo::core::string::strict_compare(p->fullpath(), 0, pc, 0, len) != 0)
                {
                    return -3;
                }

                if (fn == nullptr)
                {
                    if (p->lastpart() != nullptr)
                        return -4;
                }
                else
                {
                    ::pilo::pathlen_t fnlen = (::pilo::pathlen_t) ::pilo::core::string::character_count(fn);
                    if (fnlen != p->lastpart_len())
                        return -5;
                    if (::pilo::core::string::strict_compare(p->lastpart(), 0, fn, 0, fnlen) != 0)
                    {
                        return -6;
                    }
                }

                if (en == nullptr)
                {
                    if (p->extname() != nullptr)
                        return -7;
                }
                else
                {
                    ::pilo::pathlen_t enlen = (::pilo::pathlen_t) ::pilo::core::string::character_count(en);
                    if (enlen != p->extname_len())
                        return -8;
                    if (::pilo::core::string::strict_compare(p->extname(), 0, en, 0, enlen) != 0)
                    {
                        return -9;
                    }
                }

                ::pilo::pathlen_t rlen = 0;
                const char* par = p->parentpath(rlen);
                if (pardir == nullptr)
                {
                    if (par != nullptr)
                        return -10;
                }
                else
                {
                    ::pilo::pathlen_t parlen = (::pilo::pathlen_t) ::pilo::core::string::character_count(pardir);
                    if (parlen != rlen)
                        return -11;
                    if (::pilo::core::string::strict_compare(pardir, 0, par, 0, rlen) != 0)
                    {
                        return -12;
                    }
                }

                const char* basename = p->basename(rlen);
                if (::pilo::core::string::strict_compare(basename, 0, bn, 0, rlen) != 0)
                {
                    return -13;
                }

                if (p->absolute_type() != at)
                {
                    return -14;
                }

                return 0;
            }
		}
	}
}