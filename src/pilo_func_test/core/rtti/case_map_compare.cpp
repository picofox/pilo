#include "../../pilo_func_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/core/rtti/wired_type_map_creation.hpp"
#include "pilo/core/rtti/wired_type.hpp"
#include "pilo/core/rtti/wired_type_map_comparation.hpp"
#include "pilo/core/memory/linked_byte_buffer.hpp"
#include "pilo/core/testing/case_utils.hpp"
#include <limits>
using namespace ::pilo::func_test;


namespace pilo
{
	namespace func_test
	{
		namespace core
		{
			namespace rtti
			{
				template <typename TA_KEY, typename TA_VALUE> static std::map<TA_KEY, TA_VALUE>* s_create_testing_map(TA_KEY cnt, TA_VALUE vbeg)
				{
					std::map<TA_KEY, TA_VALUE>* ret = new std::map<TA_KEY, TA_VALUE>;
					TA_KEY i = (std::numeric_limits<TA_KEY>::min)();
					TA_KEY iend = i + cnt - 1;
					for ( ; i < iend; i++)
					{
						(*ret)[i] = vbeg++;
					}

					return ret;
				}
				template <typename TA_KEY, typename TA_VALUE> static ::pilo::err_t s_do_map_set_compare_test_once(::pilo::core::testing::func_test_case* p_case
					, const char* name
					, ::pilo::core::memory::linked_byte_buffer<4096, 1024, false> & lbb
					, ::pilo::i32_t tcnt
					, ::pilo::u8_t kt, ::pilo::i16_t vt, TA_KEY cnt, TA_VALUE vbeg)
				{
					std::vector<char*> aa;
					::pilo::err_t err = PILO_OK;

					for (::pilo::i32_t i = 0; i < (::pilo::i32_t) tcnt; i++)
					{
						char* a = (char*)s_create_testing_map<TA_KEY, TA_VALUE>(cnt, vbeg);
						if (a == nullptr)
						{
							return p_case->error(::pilo::make_core_error(PES_MEM, PEP_INSUFF), "create map<%s> failed.", name);
						}
						aa.push_back(a);
						err = lbb.write_wired_map(kt, vt, a);
						if (p_case->check_error(err, PILO_OK, "write map<%s> to buf failed.",name))
						{
							return 0;
						}
					}

					for (::pilo::i32_t i = 0; i < (::pilo::i32_t)tcnt; i++)
					{
						char* b = lbb.read_wired_map(err);
						if (b == nullptr || err != PILO_OK)
						{
							return p_case->error(::pilo::make_core_error(PES_MEM, PEP_INSUFF), "read map<%s> failed ", name);
						}

						bool eq = ::pilo::core::rtti::map_equals(kt, vt, aa[i], b, &err);
						if (!eq)
						{
							return p_case->error(::pilo::make_core_error(PES_MEM, PEP_INSUFF), "compare map<%s> failed ", name);
						}

						::pilo::core::rtti::delete_map(kt, vt, b);
						::pilo::core::rtti::delete_map(kt, vt, aa[i]);
						aa[i] = nullptr;
					}

					if (lbb.read_available() != 0)
					{
						return p_case->error(::pilo::make_core_error(PES_MEM, PEP_INSUFF), "map<%s>, lbb not clean ", name);
					}

					return PILO_OK;
				}

				template <typename TA_VALUE>
				static std::map<std::string, TA_VALUE>* s_create_testing_map(int cnt, TA_VALUE vbeg)
				{
					std::map<std::string, TA_VALUE>* ret = new std::map<std::string, TA_VALUE>;
					for (int i = 0; i < cnt; i++)
					{
						char buffer[32] = { 0 };
						::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "key_%d", i);
						std::string key(buffer);
						(*ret)[key] = vbeg++;
					}

					return ret;
				}
				template <typename TA_VALUE>
				static ::pilo::err_t s_do_map_set_compare_test_once(::pilo::core::testing::func_test_case* p_case
					, const char* name
					, ::pilo::core::memory::linked_byte_buffer<4096, 1024, false>& lbb
					, ::pilo::i32_t tcnt
				    , ::pilo::i16_t vt, int cnt, TA_VALUE vbeg)
				{
					std::vector<char*> aa;
					::pilo::err_t err = PILO_OK;

					for (::pilo::i32_t i = 0; i < (::pilo::i32_t)tcnt; i++)
					{
						char* a = (char*)s_create_testing_map<TA_VALUE>(cnt, vbeg);
						if (a == nullptr)
						{
							return p_case->error(::pilo::make_core_error(PES_MEM, PEP_INSUFF), "create map<%s> failed.", name);
						}
						aa.push_back(a);
						err = lbb.write_wired_map(::pilo::core::rtti::wired_type::key_type_str, vt, a);
						if (p_case->check_error(err, PILO_OK, "write map<%s> to buf failed.", name))
						{
							return 0;
						}
					}

					for (::pilo::i32_t i = 0; i < (::pilo::i32_t)tcnt; i++)
					{
						char* b = lbb.read_wired_map(err);
						if (b == nullptr || err != PILO_OK)
						{
							return p_case->error(::pilo::make_core_error(PES_MEM, PEP_INSUFF), "read map<%s> failed ", name);
						}

						bool eq = ::pilo::core::rtti::map_equals(::pilo::core::rtti::wired_type::key_type_str, vt, aa[i], b, &err);
						if (!eq)
						{
							return p_case->error(::pilo::make_core_error(PES_MEM, PEP_INSUFF), "compare map<%s> failed ", name);
						}

						::pilo::core::rtti::delete_map(::pilo::core::rtti::wired_type::key_type_str, vt, b);
						::pilo::core::rtti::delete_map(::pilo::core::rtti::wired_type::key_type_str, vt, aa[i]);
						aa[i] = nullptr;
					}

					if (lbb.read_available() != 0)
					{
						return p_case->error(::pilo::make_core_error(PES_MEM, PEP_INSUFF), "map<%s>, lbb not clean ", name);
					}

					return PILO_OK;
				}


				int case_map_serialize(::pilo::core::testing::func_test_case* p_case)
				{
					::pilo::err_t err = PILO_OK;
					::pilo::core::memory::linked_byte_buffer<4096, 1024, false> lbb;	

					err = s_do_map_set_compare_test_once<::pilo::i64_t, ::pilo::i64_t>(p_case, "i64-i64", lbb, 1000
						, ::pilo::core::rtti::wired_type::wired_type::key_type_i64
						, ::pilo::core::rtti::wired_type::wired_type::value_type_i64
						, (::pilo::i64_t)100, (::pilo::i64_t)1000);
					if (err != PILO_OK)
					{
						return 0;
					}

					err = s_do_map_set_compare_test_once<::pilo::i8_t, ::pilo::i8_t>(p_case, "i8-i8", lbb, 1000
						, ::pilo::core::rtti::wired_type::wired_type::key_type_i8
						, ::pilo::core::rtti::wired_type::wired_type::value_type_i8
						, (::pilo::i8_t)100, (::pilo::i8_t) 0);
					if (err != PILO_OK)
					{
						return 0;
					}

					err = s_do_map_set_compare_test_once<::pilo::i8_t, ::pilo::u8_t>(p_case, "i8-u8", lbb, 1000
						, ::pilo::core::rtti::wired_type::wired_type::key_type_i8
						, ::pilo::core::rtti::wired_type::wired_type::value_type_u8
						, (::pilo::i8_t)100, (::pilo::u8_t)1);
					if (err != PILO_OK)
					{
						return 0;
					}

					err = s_do_map_set_compare_test_once<::pilo::i32_t>(p_case, "str-i32", lbb, 1000
						, ::pilo::core::rtti::wired_type::wired_type::value_type_i32
						, 100, (::pilo::i32_t)1);
					if (err != PILO_OK)
					{
						return 0;
					}

					p_case->set_result(PILO_OK);
					return 0;
				}

				int case_map_keyi8(::pilo::core::testing::func_test_case* p_case)
				{
					::pilo::err_t err = PILO_OK;
					char* a = nullptr;
					char* b = nullptr;

					a = ::pilo::core::rtti::create_map(::pilo::core::rtti::wired_type::key_type_str, ::pilo::core::rtti::wired_type::value_type_i64, &err);
					b = ::pilo::core::rtti::create_map(::pilo::core::rtti::wired_type::key_type_str, ::pilo::core::rtti::wired_type::value_type_i64, &err);
					std::string key_a;
					std::string key_b;
					char buf[32] = { 0 };
					for (::pilo::i64_t i = 0; i < 1024; i++)
					{
						::pilo::core::io::string_formated_output(buf, 32, "key_%lld", i);
						key_a = buf;
						key_b = buf;
						err = ::pilo::core::rtti::map_ptr_insert<std::string, ::pilo::i64_t>(a, key_a, i, false);
						err = ::pilo::core::rtti::map_ptr_insert<std::string, ::pilo::i64_t>(b, key_b, i, false);
					}

					bool eq = ::pilo::core::rtti::map_equals(::pilo::core::rtti::wired_type::key_type_str, ::pilo::core::rtti::wired_type::value_type_i64,a
						, ::pilo::core::rtti::wired_type::key_type_str, ::pilo::core::rtti::wired_type::value_type_i64, b, &err);

					if (!eq)
					{
						p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "map not eq");
						return 0;
					}
					err = ::pilo::core::rtti::delete_map(::pilo::core::rtti::wired_type::key_type_str, ::pilo::core::rtti::wired_type::value_type_i64, a);
					err = ::pilo::core::rtti::delete_map(::pilo::core::rtti::wired_type::key_type_str, ::pilo::core::rtti::wired_type::value_type_i64, b);


					for (::pilo::i64_t j = 0; j < 100; j++)
					{
						a = ::pilo::core::rtti::create_map(::pilo::core::rtti::wired_type::key_type_str, ::pilo::core::rtti::wired_type::value_type_bytes, &err);
						for (::pilo::i64_t i = 0; i < 128; i++)
						{
							::pilo::core::io::string_formated_output(buf, 32, "key_%lld", i);
							key_a = buf;
							err = ::pilo::core::rtti::map_ptr_insert<std::string>(a, key_a, buf, false, false);
						}

						char* c = nullptr;
						c = ::pilo::core::rtti::clone_map(::pilo::core::rtti::wired_type::key_type_str, ::pilo::core::rtti::wired_type::value_type_bytes, a, &err);
						if (!::pilo::core::rtti::map_equals(::pilo::core::rtti::wired_type::key_type_str, ::pilo::core::rtti::wired_type::value_type_bytes, a,c, &err))
						{
							p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "cloned map not eq");
							return 0;
						}

						err = ::pilo::core::rtti::delete_map(::pilo::core::rtti::wired_type::key_type_str, ::pilo::core::rtti::wired_type::value_type_bytes, a);
						err = ::pilo::core::rtti::delete_map(::pilo::core::rtti::wired_type::key_type_str, ::pilo::core::rtti::wired_type::value_type_bytes, c);

					}

					



					

					p_case->set_result(PILO_OK);
					return 0;
				}
			}
		}
	}
}