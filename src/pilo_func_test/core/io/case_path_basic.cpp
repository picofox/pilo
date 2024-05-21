#include "../../pilo_func_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/core/io/path.hpp"
#include "pilo/core/i18n/encoding_conversion.hpp"
#include <iostream>
#include "pilo/core/process/context.hpp"

using namespace ::pilo::func_test;

#define PILO_FBB_TEST_CNT_0 (100000)

namespace pilo
{
	namespace func_test
	{
		namespace core
		{
			namespace io
			{
				char_buffer_t test()
				{
					char tb[1] = { 0 };
					auto tt = ::pilo::char_buffer_t(tb, 1, 0, false);
					char* pt = (char*)PMF_HEAP_MALLOC(1111);
					tt.reset(pt, 1111, 1, true);
					return tt;
				}

				static int s_validate_path(::pilo::core::io::path* p, const char* pc, ::pilo::pathlen_t len
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

				int case_path_basic(::pilo::core::testing::func_test_case* p_case)
				{
					::pilo::core::io::path p0;
					if (!p0.invalid())					
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "empty path should be invalid");
					
					p0.set("a");
					if (p0.invalid())					
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 shoud be valid");					
					int vret = 0;
					if ((vret = s_validate_path(&p0, "a", 1, "a", nullptr, nullptr, "a", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed %d", vret);
					

					::pilo::err_t err = p0.set("|");
					if (err == PILO_OK)					
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed |");
					

					err = p0.set("ab");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed ab");
					if ((vret = s_validate_path(&p0, "ab", 2, "ab", nullptr, nullptr, "ab", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed ab %d", vret);

					err = p0.set("ddd");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed ddd");
					if ((vret = s_validate_path(&p0, "ddd", 3, "ddd", nullptr, nullptr, "ddd", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed f: %d", vret);

					err = p0.set("ddd.txt");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed ddd");
					if ((vret = s_validate_path(&p0, "ddd.txt", 7, "ddd.txt", "txt", nullptr, "ddd", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed f: %d", vret);

					err = p0.set(".ddd.txt");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed ddd");
					if ((vret = s_validate_path(&p0, ".ddd.txt", 8, ".ddd.txt", "txt", nullptr, ".ddd", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed f: %d", vret);

					err = p0.set("dir0/dir1/ddd.txt", ::pilo::predefined_pilo_dir_enum::home);
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed ddd");					

					err = p0.set("../../../dir0/dir1/ddd.txt", ::pilo::predefined_pilo_dir_enum::cwd);
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed ddd");					

					//p0.forward_iterate([](const ::pilo::core::io::path* p, ::pilo::pathlen_t len, ::pilo::i32_t, bool is_last, void*) {
					//	std::string part(p->fullpath(), len);
					//	printf("\n%s\n", part.c_str());
					//	return PILO_OK;
					//	}, nullptr, false);

#ifdef WINDOWS
					err = p0.set("\\\\?\\Z:");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed \\\\?\\Z:");
					if ((vret = s_validate_path(&p0, "\\\\?\\Z:", 6, "\\\\?\\Z:", nullptr, nullptr, "\\\\?\\Z:", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed \\\\?\\Z: %d", vret);

					err = p0.set("\\\\?\\");
					if (err == PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed ! \\\\?\\ %d", vret);

					err = p0.set("\\\\?\\C");
					if (err == PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed ! \\\\?\\C %d", vret);

					err = p0.set("/");
					if (err == PILO_OK)					
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed ! %d", vret);					

					err = p0.set("f:");
					if (err != PILO_OK)					
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed f:");					
					if ((vret = s_validate_path(&p0, "\\\\?\\f:", 6, "\\\\?\\f:", nullptr, nullptr, "\\\\?\\f:", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed f: %d", vret);					

					err = p0.set("a\\");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed a\\");
					if ((vret = s_validate_path(&p0, "a", 1, "a", nullptr, nullptr, "a", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed %d", vret);

					err = p0.set("d\\\\");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed d\\\\");
					if ((vret = s_validate_path(&p0, "d", 1, "d", nullptr, nullptr, "d", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed %d", vret);

					err = p0.set("\\\\d");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed \\\\d");
					if ((vret = s_validate_path(&p0, "\\\\d", 3, "d", nullptr, "\\", "d",  ::pilo::core::io::path::other_absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed \\\\d %d", vret);

					err = p0.set("f:\\");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed f:");
					if ((vret = s_validate_path(&p0, "\\\\?\\f:", 6, "\\\\?\\f:", nullptr, nullptr, "\\\\?\\f:", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed f: %d", vret);

					err = p0.set("E:d");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed E:d");
					if ((vret = s_validate_path(&p0, "\\\\?\\E:\\d", 8, "d", nullptr, "\\\\?\\E:", "d", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed E:d %d", vret);

					err = p0.set("f:/");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed f:/");
					if ((vret = s_validate_path(&p0, "\\\\?\\f:", 6, "\\\\?\\f:", nullptr, nullptr, "\\\\?\\f:", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed f: %d", vret);

					err = p0.set("a/b");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed a/b");
					if ((vret = s_validate_path(&p0, "a\\b", 3, "b", nullptr, "a", "b", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed a/b %d", vret);

					err = p0.set("E:\\a\\b\\c\\d\\e.txt");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed a/b");
					if ((vret = s_validate_path(&p0, "\\\\?\\E:\\a\\b\\c\\d\\e.txt", 20, "e.txt", "txt", "\\\\?\\E:\\a\\b\\c\\d", "e", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed a/b %d", vret);

					err = p0.set("E:\\a\\\\\\b\\c\\d\\\\\\e.txt");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed a/b");
					if ((vret = s_validate_path(&p0, "\\\\?\\E:\\a\\b\\c\\d\\e.txt", 20, "e.txt", "txt", "\\\\?\\E:\\a\\b\\c\\d", "e", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed a/b %d", vret);

					err = p0.set("E:\\\\a\\\\\\b\\.\\c\\d\\.\\\\e.txt");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed a/b");
					if ((vret = s_validate_path(&p0, "\\\\?\\E:\\a\\b\\c\\d\\e.txt", 20, "e.txt", "txt", "\\\\?\\E:\\a\\b\\c\\d", "e", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed a/b %d", vret);

					err = p0.set("E:\\a\\b\\c\\..\\..\\d\\e.txt");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed E:\\a\\b\\c\\..\\..\\d\\e.txt");
					if ((vret = s_validate_path(&p0, "\\\\?\\E:\\a\\d\\e.txt", 16, "e.txt", "txt", "\\\\?\\E:\\a\\d", "e", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed E:\\a\\b\\c\\..\\..\\d\\e.txt %d", vret);

					err = p0.set("E:\\a\\b\\c\\..\\..\\d\\e\\\\");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "E:\\a\\b\\c\\..\\..\\d\\e\\\\");
					if ((vret = s_validate_path(&p0, "\\\\?\\E:\\a\\d\\e", (::pilo::pathlen_t)::pilo::core::string::character_count("\\\\?\\E:\\a\\d\\e"), "e", nullptr, "\\\\?\\E:\\a\\d", "e", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed E:\\a\\b\\c\\..\\..\\d\\e.txt %d", vret);
					err = p0.append("f\\g\\h");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "append f\\g\\h");
					if ((vret = s_validate_path(&p0, "\\\\?\\E:\\a\\d\\e\\f\\g\\h", (::pilo::pathlen_t)::pilo::core::string::character_count("\\\\?\\E:\\a\\d\\e\\f\\g\\h"), "h", nullptr, "\\\\?\\E:\\a\\d\\e\\f\\g", "h", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed E:\\a\\b\\c\\..\\..\\d\\e.txt %d", vret);

					err = p0.set("dir0/dir1/dir2/dir3/dir4");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed ab");
					if ((vret = s_validate_path(&p0, "dir0\\dir1\\dir2\\dir3\\dir4", 24, "dir4", nullptr, "dir0\\dir1\\dir2\\dir3", "dir4", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed dir0/dir1/dir2/dir3/dir4 %d", vret);

					err = p0.set("f:\\目录1\\目录2\\目录3\\目录_del\\目录_del\\..\\..\\目录4\\目录5\\目录_del3\\..\\目录6");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "set long chn path");
					if ((vret = s_validate_path(&p0, "\\\\?\\f:\\目录1\\目录2\\目录3\\目录4\\目录5\\目录6", (::pilo::pathlen_t)::pilo::core::string::character_count("\\\\?\\f:\\目录1\\目录2\\目录3\\目录4\\目录5\\目录6"), "目录6", nullptr, "\\\\?\\f:\\目录1\\目录2\\目录3\\目录4\\目录5", "目录6", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed chn dir path %d", vret);


#else


					err = p0.set("f:");
					if (err == PILO_OK)
					{
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed !f:");
					}

					err = p0.set("a/");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed a/");
					if ((vret = s_validate_path(&p0, "a", 1, nullptr, nullptr, "a", "a", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed f: %d", vret);

					err = p0.set("/a/b/c/d/e.txt");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed /a/b/c/d/e.txt");
					if ((vret = s_validate_path(&p0, "/a/b/c/d/e.txt", 14, "e.txt", "txt", "/a/b/c/d", "e", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed /a/b/c/d/e.txt %d", vret);

					err = p0.set("/a/////b/////c////d/e.txt");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed /a/b/c/d/e.txt");
					if ((vret = s_validate_path(&p0, "/a/b/c/d/e.txt", 14, "e.txt", "txt", "/a/b/c/d", "e", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed /a/b/c/d/e.txt %d", vret);

#endif

					p_case->set_result(PILO_OK);
					return 0;
				}


				int case_get_paths(::pilo::core::testing::func_test_case* p_case)
				{
					char big_arr[1024] = { 0 };
					::pilo::char_buffer_t enbuf(big_arr, sizeof(big_arr));
					::pilo::err_t err = PILO_OK;
					err = ::pilo::core::io::path::get_cwd(enbuf, ::pilo::core::io::path::sepmode_needsep, 0);
					if (err != PILO_OK)
					{
						return p_case->error(::pilo::make_core_error(PES_GETCWD, PEP_IS_NULL), "get cwd failed");
					}
					if (enbuf.invalid())
					{
						return p_case->error(::pilo::make_core_error(PES_GETCWD, PEP_IS_NULL), "get cwd failed");
					}
					if (enbuf.is_dynamic())
					{
						return p_case->error(::pilo::make_core_error(PES_GETCWD, PEP_INC_DATA), "get cwd should not alloc memory");
					}


					char buffer_small[2] = { 0 };
					::pilo::char_buffer_t smbuf(buffer_small, 2);
					err = ::pilo::core::io::path::get_cwd(smbuf,::pilo::core::io::path::sepmode_needsep, 0);	
					if (err != PILO_OK)
					{
						return p_case->error(::pilo::make_core_error(PES_GETCWD, PEP_IS_NULL), "get cwd failed");
					}
					if (smbuf.invalid())
					{
						return p_case->error(::pilo::make_core_error(PES_GETCWD, PEP_IS_NULL), "get cwd invalid");
					}
					if (!smbuf.is_dynamic())
					{
						return p_case->error(::pilo::make_core_error(PES_GETCWD, PEP_INC_DATA), "get cwd should not alloc memory");
					}




					//::pilo::wchar_buffer_t wb;

					//
					//::pilo::core::i18n::utf8_to_os_unicode(wb, smbuf.begin(), smbuf.size(), 0);
					//std::wstring wstr(wb.begin(), wb.size());
					//std::wcout << wstr << std::endl;
					//

					//::pilo::i8_t src_type = ::pilo::core::io::path::node_type_na;
					//::pilo::core::io::path::get_path_node_type(ret.ptr(), ::pilo::core::io::path::local_fs_path, src_type, nullptr, true, nullptr, 0, err);
					//if (src_type != ::pilo::core::io::path::fs_node_type_dir)
					//{
					//	return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "cwd is not dir type");
					//}

					p_case->set_result(PILO_OK);
					return 0;
				}

				int case_paths_attr(::pilo::core::testing::func_test_case* p_case)
				{
					//::pilo::err_t err = PILO_OK;
					//::pilo::i8_t src_type = ::pilo::core::io::path::node_type_na;
					//::pilo::i8_t target_type = ::pilo::core::io::path::node_type_na;
					//char buffer[1];
					//auto ret = ::pilo::core::io::path::get_path_node_type("垃圾.txt.lnk", ::pilo::core::io::path::local_fs_path, src_type, &target_type, true, buffer, sizeof(buffer), err);
					//if (ret.invalid())
					//{
					//	return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "get node type failed of symlink");
					//}



					p_case->set_result(PILO_OK);
					return 0;
				}
			}
		}
	}
}