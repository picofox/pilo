#include "../../pilo_stable_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/core/io/path.hpp"
#include "pilo/core/testing/case_utils.hpp"

using namespace ::pilo::stable_test;
using namespace ::pilo::core::testing;


namespace pilo
{
	namespace stable_test
	{
		namespace core
		{
			namespace io
			{
				static ::pilo::err_t _s_path_creation(::pilo::core::testing::stable_test_case* p_case)
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
					if ((vret = ::pilo::core::testing::validate_path(&p0, "\\\\d", 3, "d", nullptr, "\\", "d", ::pilo::core::io::path::other_absolute)) != 0)
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
					if (err == PILO_OK)
					{
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed !f:");
					}

					err = p0.set("a/");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed a/");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "a", 1, nullptr, nullptr, "a", "a", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed f: %d", vret);

					err = p0.set("/a/b/c/d/e.txt");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed /a/b/c/d/e.txt");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "/a/b/c/d/e.txt", 14, "e.txt", "txt", "/a/b/c/d", "e", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed /a/b/c/d/e.txt %d", vret);

					err = p0.set("/a/////b/////c////d/e.txt");
					if (err != PILO_OK)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed /a/b/c/d/e.txt");
					if ((vret = ::pilo::core::testing::validate_path(&p0, "/a/b/c/d/e.txt", 14, "e.txt", "txt", "/a/b/c/d", "e", ::pilo::core::io::path::relative)) != 0)
						return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "p0 val failed /a/b/c/d/e.txt %d", vret);

#endif

					p_case->set_result(PILO_OK);
					return 0;
				}

				int case_path_creation(::pilo::core::testing::stable_test_case* p_case)
				{
					p_case->set_begin();
					::pilo::err_t err = PILO_OK;
					for (::pilo::i64_t i = 0; i < p_case->target_trans_count(); i++)
					{
						err = _s_path_creation(p_case);
						if (err != PILO_OK)
						{
							return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_ABORTED), "abort on loop %lld", i);
						}
						p_case->inc_print_progress();
					}

					p_case->set_result(PILO_OK);
					return PILO_OK;
				}
			}
		}
	}
}