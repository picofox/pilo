#include "../../pilo_func_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/core/io/path.hpp"
#include "pilo/core/i18n/encoding_conversion.hpp"
#include <iostream>
#include "pilo/core/process/context.hpp"
#include "pilo/core/testing/case_utils.hpp"

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

				static ::pilo::err_t create_complex_dir_hirechy()
				{
					pilo::err_t err = PILO_OK;
					::pilo::core::io::path rt("test_fs\\cdh", ::pilo::core::io::path::unknow_length, 0, ::pilo::predefined_pilo_dir_enum::tmp);
					err = rt.remove(false, false);
					if (err != PILO_OK)
					{
						return err;
					}
					::pilo::core::io::path p1 = rt;
					err = p1.append("\\目录1\\目录2-1\\层1-1-main.txt", ::pilo::core::io::path::unknow_length);
					if (err != PILO_OK)
					{
						return err;
					}
					err = p1.create(::pilo::core::io::path::fs_node_type_file, false);
					if (err != PILO_OK)
					{
						return err;
					}

					//::pilo::core::io::path p1_link_0("H:\\main-l.lnk", ::pilo::core::io::path::unknow_length);
					//err = p1.create_link_by(&p1_link_0, true);

					//::pilo::core::io::path p1_link_1("I:\\2222.lnk", ::pilo::core::io::path::unknow_length);
					//p1_link_0.create_link_by(&p1_link_1, true);

					const char* test_str_0 = "test_fs\\cdh\\目录1\\目录2\\目录3\\abc1\\abc3\\.\\..\\..\\目录_del\\目录_del\\..\\..\\目录4\\";
					::pilo::core::io::path p0;
					err = p0.set(test_str_0, ::pilo::predefined_pilo_dir_enum::tmp);
					if (err != PILO_OK)
						return err;
					err = p0.create(::pilo::core::io::path::fs_node_type_dir, false);
					if (err != PILO_OK)
					{
						return err;
					}
					char buffer[128] = { 0 };					
					for (int i = 0; i < 20; i++)
					{
						::pilo::core::io::string_formated_output(buffer, 128, "层4-f-%d.txt", i);
						err = p0.create_sub_file(buffer, ::pilo::core::io::path::unknow_length, false, nullptr);
						if (err != PILO_OK)
							return err;
					}

					return PILO_OK;
				}

				static ::pilo::err_t create_complex_dir_hirechy_with_link()
				{	
					::pilo::err_t err = create_complex_dir_hirechy();
					if (err != PILO_OK)
						return err;

					::pilo::core::io::path rt2("test_fs\\cdh2\\", ::pilo::predefined_pilo_dir_enum::tmp);
					if (rt2.invalid())
					{
						return ::pilo::make_core_error(PES_PATH_STR, PEP_IS_INVALID);
					}
					rt2.remove(false, false);
					err = rt2.create(::pilo::core::io::path::fs_node_type_dir, true);
					if (err != PILO_OK)
					{
						return err;
					}

					::pilo::core::io::path rt2_0("test_fs\\cdh2\\测文件符号\\", ::pilo::predefined_pilo_dir_enum::tmp);
					if (rt2_0.invalid())
					{
						return ::pilo::make_core_error(PES_PATH_STR, PEP_IS_INVALID);
					}
					err = rt2_0.create(::pilo::core::io::path::fs_node_type_dir, true);
					if (err != PILO_OK)
					{
						return err;
					}
					::pilo::core::io::path p1_link("test_fs\\cdh\\目录1\\目录2\\目录3\\cdh2.lnk", ::pilo::predefined_pilo_dir_enum::tmp);
					err = rt2.create_link_by(&p1_link, true);
					if (err != PILO_OK)
					{
						return err;
					}


					::pilo::core::io::path path_file1;
					err = rt2_0.create_sub_file("一号文件abc.txt", ::pilo::core::io::path::unknow_length,false, &path_file1);
					if (err != PILO_OK)
					{
						return err;
					}					
					::pilo::core::io::path p2_link("test_fs\\cdh\\目录1\\目录2\\一号文件abc.txt.lnk", ::pilo::predefined_pilo_dir_enum::tmp);
					err = path_file1.create_link_by(&p2_link, true);
					if (err != PILO_OK)
					{
						return err;
					}		

					return PILO_OK;
				}

				int case_path_fs_op(::pilo::core::testing::func_test_case* p_case)
				{
					::pilo::err_t err = PILO_OK;

					err = create_complex_dir_hirechy();
					if (err != PILO_OK)
					{
						return  p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "set long chn path");
					}

					::pilo::core::io::path rt("test_fs\\cdh", ::pilo::core::io::path::unknow_length, 0, ::pilo::predefined_pilo_dir_enum::tmp);
					err = rt.remove(false, false);
					if (err != PILO_OK)
					{
						return  p_case->error(err, "set long chn path");;
					}

					::pilo::i8_t nt = rt.get_fs_info(nullptr, nullptr);
					if (nt != ::pilo::core::io::path::node_type_na)
					{
						return  p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "set long chn path");
					}

					err = create_complex_dir_hirechy_with_link();
					if (err != PILO_OK)
					{
						return  p_case->error(err, "set long chn path");;
					}

					err = rt.remove(false, true);
					if (err != PILO_OK)
					{
						return  p_case->error(err, "set long chn path");;
					}


					p_case->set_result(PILO_OK);
					return PILO_OK;
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
					if ((vret = ::pilo::core::testing::validate_path(&p0, "a", 1, "a", nullptr, nullptr, "a", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed %d", vret);
					

					::pilo::err_t err = p0.set("|");
					if (err == PILO_OK)					
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed |");
					

					err = p0.set("ab");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed ab");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "ab", 2, "ab", nullptr, nullptr, "ab", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed ab %d", vret);

					err = p0.set("ddd");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed ddd");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "ddd", 3, "ddd", nullptr, nullptr, "ddd", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed f: %d", vret);

					err = p0.set("ddd.txt");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed ddd");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "ddd.txt", 7, "ddd.txt", "txt", nullptr, "ddd", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed f: %d", vret);

					err = p0.set(".ddd.txt");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed ddd");
					if ((vret = ::pilo::core::testing::validate_path(&p0, ".ddd.txt", 8, ".ddd.txt", "txt", nullptr, ".ddd", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed f: %d", vret);

					err = p0.set("dir0/dir1/ddd.txt", ::pilo::predefined_pilo_dir_enum::home);
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed ddd");					

					err = p0.set("../../../dir0/dir1/ddd.txt", ::pilo::predefined_pilo_dir_enum::cwd);
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed ddd");

                    err = p0.set("目录1/../../目录2/目录3/目录4");
                    if (err != PILO_OK)
                        return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "set long (目录1/../../目录2/目录3/目录4) path");
                    if ((vret = ::pilo::core::testing::validate_path(&p0, "目录1/../../目录2/目录3/目录4", (::pilo::pathlen_t)::pilo::core::string::character_count("目录1/../../目录2/目录3/目录4"), "目录4", nullptr, "目录1/../../目录2/目录3", "目录4", ::pilo::core::io::path::relative)) != 0)
                        return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed chn dir path %d", vret);

					//p0.forward_iterate([](const ::pilo::core::io::path* p, ::pilo::pathlen_t len, ::pilo::i32_t, bool is_last, void*) {
					//	std::string part(p->fullpath(), len);
					//	printf("\n%s\n", part.c_str());
					//	return PILO_OK;
					//	}, nullptr, false);

#ifdef WINDOWS
					err = p0.set("\\\\?\\Z:");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed \\\\?\\Z:");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "\\\\?\\Z:", 6, "\\\\?\\Z:", nullptr, nullptr, "\\\\?\\Z:", ::pilo::core::io::path::absolute)) != 0)
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
					if ((vret = ::pilo::core::testing::validate_path(&p0, "\\\\?\\f:", 6, "\\\\?\\f:", nullptr, nullptr, "\\\\?\\f:", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed f: %d", vret);					

					err = p0.set("a\\");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed a\\");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "a", 1, "a", nullptr, nullptr, "a", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed %d", vret);

					err = p0.set("d\\\\");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed d\\\\");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "d", 1, "d", nullptr, nullptr, "d", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed %d", vret);

					err = p0.set("\\\\d");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed \\\\d");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "\\\\d", 3, "d", nullptr, "\\", "d",  ::pilo::core::io::path::other_absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed \\\\d %d", vret);

					err = p0.set("f:\\");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed f:");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "\\\\?\\f:", 6, "\\\\?\\f:", nullptr, nullptr, "\\\\?\\f:", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed f: %d", vret);

					err = p0.set("E:d");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed E:d");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "\\\\?\\E:\\d", 8, "d", nullptr, "\\\\?\\E:", "d", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed E:d %d", vret);

					err = p0.set("f:/");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed f:/");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "\\\\?\\f:", 6, "\\\\?\\f:", nullptr, nullptr, "\\\\?\\f:", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed f: %d", vret);

					err = p0.set("a/b");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed a/b");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "a\\b", 3, "b", nullptr, "a", "b", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed a/b %d", vret);

					err = p0.set("E:\\a\\b\\c\\d\\e.txt");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed a/b");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "\\\\?\\E:\\a\\b\\c\\d\\e.txt", 20, "e.txt", "txt", "\\\\?\\E:\\a\\b\\c\\d", "e", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed a/b %d", vret);

					err = p0.set("E:\\a\\\\\\b\\c\\d\\\\\\e.txt");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed a/b");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "\\\\?\\E:\\a\\b\\c\\d\\e.txt", 20, "e.txt", "txt", "\\\\?\\E:\\a\\b\\c\\d", "e", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed a/b %d", vret);

					err = p0.set("E:\\\\a\\\\\\b\\.\\c\\d\\.\\\\e.txt");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed a/b");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "\\\\?\\E:\\a\\b\\c\\d\\e.txt", 20, "e.txt", "txt", "\\\\?\\E:\\a\\b\\c\\d", "e", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed a/b %d", vret);

					err = p0.set("E:\\a\\b\\c\\..\\..\\d\\e.txt");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed E:\\a\\b\\c\\..\\..\\d\\e.txt");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "\\\\?\\E:\\a\\d\\e.txt", 16, "e.txt", "txt", "\\\\?\\E:\\a\\d", "e", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed E:\\a\\b\\c\\..\\..\\d\\e.txt %d", vret);

					err = p0.set("E:\\a\\b\\c\\..\\..\\d\\e\\\\");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "E:\\a\\b\\c\\..\\..\\d\\e\\\\");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "\\\\?\\E:\\a\\d\\e", (::pilo::pathlen_t)::pilo::core::string::character_count("\\\\?\\E:\\a\\d\\e"), "e", nullptr, "\\\\?\\E:\\a\\d", "e", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed E:\\a\\b\\c\\..\\..\\d\\e.txt %d", vret);
					err = p0.append("f\\g\\h");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "append f\\g\\h");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "\\\\?\\E:\\a\\d\\e\\f\\g\\h", (::pilo::pathlen_t)::pilo::core::string::character_count("\\\\?\\E:\\a\\d\\e\\f\\g\\h"), "h", nullptr, "\\\\?\\E:\\a\\d\\e\\f\\g", "h", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed E:\\a\\b\\c\\..\\..\\d\\e.txt %d", vret);

					err = p0.set("dir0/dir1/dir2/dir3/dir4");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed ab");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "dir0\\dir1\\dir2\\dir3\\dir4", 24, "dir4", nullptr, "dir0\\dir1\\dir2\\dir3", "dir4", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed dir0/dir1/dir2/dir3/dir4 %d", vret);

					err = p0.set("f:\\目录1\\目录2\\目录3\\目录_del\\目录_del\\..\\..\\目录4\\目录5\\目录_del3\\..\\目录6");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "set long chn path");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "\\\\?\\f:\\目录1\\目录2\\目录3\\目录4\\目录5\\目录6", (::pilo::pathlen_t)::pilo::core::string::character_count("\\\\?\\f:\\目录1\\目录2\\目录3\\目录4\\目录5\\目录6"), "目录6", nullptr, "\\\\?\\f:\\目录1\\目录2\\目录3\\目录4\\目录5", "目录6", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed chn dir path %d", vret);


#else

					err = p0.set("f:");
					if (err != PILO_OK)
					{
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed !f:");
					}
                    if (err != PILO_OK)
                        return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed f:");
                    if ((vret = ::pilo::core::testing::validate_path(&p0, "f:", 2, "f:", nullptr, nullptr, "f:", ::pilo::core::io::path::relative)) != 0)
                        return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed f: %d", vret);

					err = p0.set("a/");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed a/");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "a", 1, "a", nullptr, nullptr, "a", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed f: %d", vret);

					err = p0.set("/a/b/c/d/e.txt");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed /a/b/c/d/e.txt");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "/a/b/c/d/e.txt", 14, "e.txt", "txt", "/a/b/c/d", "e", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed /a/b/c/d/e.txt %d", vret);

                    err = p0.set("a/b/c/d/e.txt");
                    if (err != PILO_OK)
                        return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed a/b/c/d/e.txt");
                    if ((vret = ::pilo::core::testing::validate_path(&p0, "a/b/c/d/e.txt", 13, "e.txt", "txt", "a/b/c/d", "e", ::pilo::core::io::path::relative)) != 0)
                        return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed /a/b/c/d/e.txt %d", vret);

                    err = p0.set("/a/////b/////c////d/e.txt");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed /a/b/c/d/e.txt");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "/a/b/c/d/e.txt", 14, "e.txt", "txt", "/a/b/c/d", "e", ::pilo::core::io::path::absolute)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed /a/b/c/d/e.txt %d", vret);

                    err = p0.set("/目录1/目录2/目录3/目录_del/目录_del/../../目录4/目录5/目录_del3/../目录6");
                    if (err != PILO_OK)
                        return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "set long chn path");
                    if ((vret = ::pilo::core::testing::validate_path(&p0, "/目录1/目录2/目录3/目录4/目录5/目录6", (::pilo::pathlen_t)::pilo::core::string::character_count("/目录1/目录2/目录3/目录4/目录5/目录6"), "目录6", nullptr, "/目录1/目录2/目录3/目录4/目录5", "目录6", ::pilo::core::io::path::absolute)) != 0)
                        return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed chn dir path %d", vret);

                    err = p0.set("目录1/目录2/目录3/目录_del/目录_del/../../目录4/目录5/目录_del3/../目录6");
                    if (err != PILO_OK)
                        return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "set long chn path");
                    if ((vret = ::pilo::core::testing::validate_path(&p0, "目录1/目录2/目录3/目录4/目录5/目录6", (::pilo::pathlen_t)::pilo::core::string::character_count("目录1/目录2/目录3/目录4/目录5/目录6"), "目录6", nullptr, "目录1/目录2/目录3/目录4/目录5", "目录6", ::pilo::core::io::path::relative)) != 0)
                        return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed chn dir path %d", vret);
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