#include "../../pilo_func_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/core/io/path.hpp"
#include "pilo/core/i18n/encoding_conversion.hpp"
#include <iostream>
#include "pilo/core/process/context.hpp"
#include "pilo/core/testing/case_utils.hpp"
#include "pilo/core/io/file.hpp"
#include "pilo/core/io/text_file.hpp"
#include "pilo/core/memory/util.hpp"
#include "pilo/core/logging/text_logger.hpp"

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


				static ::pilo::err_t _s_test_nl_no_cache(::pilo::i32_t num, ::pilo::core::testing::func_test_case* p_case, ::pilo::core::io::text_file<>* f, const char* rsep, const char* wsep)
				{
					f->set_read_seperator(rsep);
					f->set_write_seperator(wsep);

					::pilo::err_t err = f->set_size(0);
					if (err != PILO_OK) {
						return p_case->error(PERR_IO_READ_FAIL, "#%d: trunc file failed.", num);
					}
						
					char cmp_buf[777] = { 0 };
					::pilo::i64_t rlen = 0;					
					char read_buf[777] = { 0 };

					for (int i = 0; i < 9; i++) {
						memset(cmp_buf, 0x30 + i, 771);
						::pilo::core::string::n_copyz(cmp_buf+771, 6, f->write_seperator(), f->write_seperator_length());
						err = f->write_line(cmp_buf, 771, true, &rlen);
						if (err != PILO_OK) {
							return p_case->error(PERR_IO_READ_FAIL, "#%d: write line failed.", num);
						}
						if (rlen != 771 + f->write_seperator_length()) {
							return p_case->error(PERR_IO_READ_FAIL, "#%d: write line %lld != %lld.", num, rlen, 771 + f->write_seperator_length());
						}
					}

					f->seek(::pilo::core::io::seek_whence::begin, 0);

					for (int i = 0; i < 9; i++) {
						memset(cmp_buf, 0x30 + i, 771);
						::pilo::core::string::n_copyz(cmp_buf + 771, 6, f->read_seperator(), f->read_seperator_length());
						char * readed_ret = f->read_line(read_buf, sizeof(read_buf), false, &rlen, err);
						if (readed_ret == nullptr || err != PILO_OK) {
							return p_case->error(PERR_IO_READ_FAIL, "#%d: read line failed. %s", num, ::pilo::str_err(err, "", false).c_str());
						}
						if (rlen != 771 + f->read_seperator_length()) {
							return p_case->error(PERR_IO_READ_FAIL, "#%d: read line cnt %lld != %lld.", num, rlen, 771 + f->read_seperator_length());
						}

						if (::pilo::core::string::strict_compare(cmp_buf, 0, readed_ret, 0, rlen) != 0) {
							return p_case->error(PERR_IO_READ_FAIL, "#%d: read line data error.", num);
						}

						if (readed_ret != read_buf) {
							return p_case->error(PERR_IO_READ_FAIL, "#%d: large buffer should not allocate.", num);
						}
					}



					f->seek(::pilo::core::io::seek_whence::begin, 0);
					for (int i = 0; i < 9; i++) {
						memset(cmp_buf, 0x30 + i, 771);
						::pilo::core::string::n_copyz(cmp_buf + 771, 6, f->read_seperator(), f->read_seperator_length());
						char* readed_ret = f->read_line(nullptr, 0, false, &rlen, err);
						if (readed_ret == nullptr || err != PILO_OK) {
							return p_case->error(PERR_IO_READ_FAIL, "#%d: read line failed. %s", num, ::pilo::str_err(err, "", false).c_str());
						}
						if (rlen != 771 + f->read_seperator_length()) {
							return p_case->error(PERR_IO_READ_FAIL, "#%d: read line cnt %lld != %lld.", num, rlen, 771 + f->read_seperator_length());
						}

						if (::pilo::core::string::strict_compare(cmp_buf, 0, readed_ret, 0, rlen) != 0) {
							return p_case->error(PERR_IO_READ_FAIL, "#%d: read line data error.", num);
						}

						if (readed_ret == read_buf) {
							return p_case->error(PERR_IO_READ_FAIL, "#%d: large buffer should not allocate.", num);
						}

						::pilo::core::io::text_file<>::s_free_line_buffer(&readed_ret);
					}


					return PILO_OK;
				}

				static ::pilo::i64_t s_generate_line(char * buffer, ::pilo::i64_t capa, ::pilo::i64_t idx)
				{
					char v = idx % 10;
					if (idx >= 0 && idx < 10000) {
						if ((idx & 0x1) == 0) {
							memset(buffer, 0x30 + v, capa - 1);
							buffer[capa -  1] = 0;
							return capa - 1;
						} else {
							::pilo::i64_t cnt = (capa / 10) >= 32 ? capa / 10 : 32;
							memset(buffer, 0x30 + v, cnt);
							buffer[cnt] = 0;
							return cnt;
						}
					}
					return -1;
				}


				int case_text_file_rdwrbuf(::pilo::core::testing::func_test_case* p_case)
				{
					::pilo::core::logging::text_logger<> logger;

					::pilo::core::io::path p;
					::pilo::err_t err = PILO_OK;
					const char* test_file_path_str = nullptr;

#ifdef WINDOWS
					test_file_path_str = "f:\\test_file.txt";
#else
					test_file_path_str = "/tmp/test_file.txt";
#endif


					p.set(test_file_path_str, predefined_pilo_dir::count);
					if (p.invalid())
					{
						return p_case->error(PERR_INVALID_PATH, "init path failed");
					}

					p.remove(true, true);

					::pilo::core::io::text_file<> tf0(4096, 4096, "\n", "\r\n");

					err = tf0.open(&p, ::pilo::core::io::creation_mode::create_always, ::pilo::core::io::access_permission::read_write, ::pilo::core::io::dev_open_flags::none);
					if (tf0.error()) {
						return p_case->error(PERR_IO_READ_FAIL, "open (create_always,read_write) text failed");
					}

					::pilo::i64_t idx = 0;
					::pilo::i64_t rlen = 0;
					char tmp_buffer[4100];
					for (; idx < 10000; idx++) {
						::pilo::i64_t cnt = s_generate_line(tmp_buffer, 4096, idx);
						PMC_ASSERT(cnt > 0);
						err = tf0.write_line(tmp_buffer, cnt, true, &rlen);
						if (err != PILO_OK) {
							return p_case->error(PERR_IO_WRITE_FAIL, "write text failed");
						}
						if (cnt != rlen - tf0.write_seperator_length()) {
							return p_case->error(PERR_IO_WRITE_FAIL, "write text cnt failed");
						}
					}
					memset(tmp_buffer, 'x', 4091);
					tmp_buffer[4091] = 0;
					err = tf0.write_line(tmp_buffer, 4091, false, &rlen);
					if (err != PILO_OK) {
						return p_case->error(PERR_IO_WRITE_FAIL, "write text failed");
					}
					if (4091 != rlen ) {
						return p_case->error(PERR_IO_WRITE_FAIL, "write text cnt failed");
					}


					char tmp_r_buffer[4100];
					tf0.seek(::pilo::core::io::seek_whence::begin, 0);
					char* rdbufp = nullptr;

					for (idx = 0; idx < 10000; idx++) {
						::pilo::i64_t cnt = s_generate_line(tmp_buffer, 4096, idx);
						PMC_ASSERT(cnt > 0);
						rdbufp = tf0.read_line(tmp_r_buffer, 4100, true, &rlen, err);
						if (rdbufp == nullptr || err != PILO_OK) {
							return p_case->error(PERR_IO_READ_FAIL, "read text failed");
						}
						if (rlen != cnt) {
							return p_case->error(PERR_IO_READ_FAIL, "read text cnt failed");
						}
						if (::pilo::core::string::strict_compare(rdbufp, 0, tmp_buffer, 0, -1) != 0) {
							return p_case->error(PERR_IO_READ_FAIL, "read text content val failed");
						}

						if (rdbufp != tmp_r_buffer) {
							return p_case->error(PERR_INC_DATA, "read text should not alloc");
						}

					}
					rdbufp = tf0.read_line(tmp_r_buffer, 4100, true, &rlen, err);
					if (rdbufp == nullptr || err != PERR_EOF) {
						return p_case->error(PERR_IO_READ_FAIL, "read text failed");
					}
					if (rlen != 4091) {
						return p_case->error(PERR_IO_READ_FAIL, "read text cnt failed");
					}
					memset(tmp_buffer, 'x', 4091);
					tmp_buffer[4091] = 0;
					if (::pilo::core::string::strict_compare(tmp_r_buffer, 0, tmp_buffer, 0, -1) != 0) {
						return p_case->error(PERR_IO_READ_FAIL, "read text content val failed");
					}
					if (rdbufp != tmp_r_buffer) {
						return p_case->error(PERR_INC_DATA, "read last text should not alloc");
					}

					//////////////2//////////////////////
					char tmp_rsm_buffer[1000] = {0};
					tf0.seek(::pilo::core::io::seek_whence::begin, 0);

					for (idx = 0; idx < 10000; idx++) {
						::pilo::i64_t cnt = s_generate_line(tmp_buffer, 4096, idx);
						PMC_ASSERT(cnt > 0);
						rdbufp = tf0.read_line(tmp_rsm_buffer, sizeof(tmp_rsm_buffer), true, &rlen, err);
						if (rdbufp == nullptr || err != PILO_OK) {
							return p_case->error(PERR_IO_READ_FAIL, "read text failed");
						}
						if (rlen != cnt) {
							return p_case->error(PERR_IO_READ_FAIL, "read text cnt failed");
						}
						if (::pilo::core::string::strict_compare(rdbufp, 0, tmp_buffer, 0, -1) != 0) {
							return p_case->error(PERR_IO_READ_FAIL, "read text content val failed");
						}

						if ((idx & 0x1) == 0) {
							if (rdbufp == tmp_rsm_buffer) {
								return p_case->error(PERR_INC_DATA, "read text should alloc");
							}
							::pilo::core::io::text_file<>::s_free_line_buffer(&rdbufp);
						} else {
							if (rdbufp != tmp_rsm_buffer) {
								return p_case->error(PERR_INC_DATA, "read text should not alloc");
							}

							
						}		

					}
					rdbufp = tf0.read_line(tmp_rsm_buffer, sizeof(tmp_rsm_buffer), true, &rlen, err);
					if (rdbufp == nullptr || err != PERR_EOF) {
						return p_case->error(PERR_IO_READ_FAIL, "read text failed");
					}
					if (rlen != 4091) {
						return p_case->error(PERR_IO_READ_FAIL, "read text cnt failed");
					}
					memset(tmp_buffer, 'x', 4091);
					tmp_buffer[4091] = 0;
					if (::pilo::core::string::strict_compare(rdbufp, 0, tmp_buffer, 0, -1) != 0) {
						return p_case->error(PERR_IO_READ_FAIL, "read text content val failed");
					}
					if (rdbufp == tmp_rsm_buffer) {
						return p_case->error(PERR_INC_DATA, "read last text should alloc");
					}
					::pilo::core::io::text_file<>::s_free_line_buffer(&rdbufp);

					/////////////////////////3/////////////////////////////////////////
					tf0.seek(::pilo::core::io::seek_whence::begin, 0);

					for (idx = 0; idx < 10000; idx++) {
						::pilo::i64_t cnt = s_generate_line(tmp_buffer, 4096, idx);
						tmp_buffer[cnt] = '\n';
						tmp_buffer[cnt+1] = '\0';
						cnt++;
						PMC_ASSERT(cnt > 0);
						rdbufp = tf0.read_line(tmp_rsm_buffer, sizeof(tmp_rsm_buffer), false, &rlen, err);
						if (rdbufp == nullptr || err != PILO_OK) {
							return p_case->error(PERR_IO_READ_FAIL, "read text failed");
						}
						if (rlen != cnt) {
							return p_case->error(PERR_IO_READ_FAIL, "read text cnt failed");
						}
						if (::pilo::core::string::strict_compare(rdbufp, 0, tmp_buffer, 0, -1) != 0) {
							return p_case->error(PERR_IO_READ_FAIL, "read text content val failed");
						}

						if ((idx & 0x1) == 0) {
							if (rdbufp == tmp_rsm_buffer) {
								return p_case->error(PERR_INC_DATA, "read text should alloc");
							}
							::pilo::core::io::text_file<>::s_free_line_buffer(&rdbufp);
						}
						else {
							if (rdbufp != tmp_rsm_buffer) {
								return p_case->error(PERR_INC_DATA, "read text should not alloc");
							}
						}
					}
					rdbufp = tf0.read_line(tmp_rsm_buffer, sizeof(tmp_rsm_buffer), false, &rlen, err);
					if (rdbufp == nullptr || err != PERR_EOF) {
						return p_case->error(PERR_IO_READ_FAIL, "read text failed");
					}
					if (rlen != 4092) {
						return p_case->error(err, "read text cnt failed");
					}
					memset(tmp_buffer, 'x', 4091);
					tmp_buffer[4091] = '\n';
					tmp_buffer[4092] = 0;
					if (::pilo::core::string::strict_compare(rdbufp, 0, tmp_buffer, 0, -1) != 0) {
						return p_case->error(PERR_IO_READ_FAIL, "read text content val failed");
					}
					if (rdbufp == tmp_rsm_buffer) {
						return p_case->error(PERR_INC_DATA, "read last text should alloc");
					}
					::pilo::core::io::text_file<>::s_free_line_buffer(&rdbufp);

					tf0.seek(::pilo::core::io::seek_whence::begin, 0);
					std::vector<std::string > vec;
					err = tf0.read_lines(vec, true, true);
					for (idx = 0; idx < 10000; idx++) {
						::pilo::i64_t cnt = s_generate_line(tmp_buffer, 4096, idx);
						PMC_ASSERT(cnt > 0);

						if ((::pilo::i64_t) vec[idx].size() != cnt) {
							return p_case->error(PERR_IO_READ_FAIL, "read text cnt failed");
						}
						if (::pilo::core::string::strict_compare(vec[idx].c_str(), 0, tmp_buffer, 0, -1) != 0) {
							return p_case->error(PERR_IO_READ_FAIL, "read text content val failed");
						}
					}

					
                    tf0.flush(::pilo::core::io::flush_level::all);


					tf0.formatted_output(true, "today 's dinner is %s at price %8.8f\n", "yuxiangrousi", 3.15f);




					tf0.close();
					p_case->set_result(PILO_OK);
					return PILO_OK;

				}


				int case_text_file_nb(::pilo::core::testing::func_test_case* p_case)
				{
					::pilo::core::io::path p;
					::pilo::err_t err = PILO_OK;
					

					p.set("test_fs/oc/f1.txt", predefined_pilo_dir::tmp);
					if (p.invalid())
					{
						return p_case->error(PERR_INVALID_PATH, "init path failed");
					}

					p.remove(true, true);

					::pilo::core::io::text_file<> tf0(0, 0, "\r\n", "\n");

					err = tf0.open(&p, ::pilo::core::io::creation_mode::create_always, ::pilo::core::io::access_permission::read_write, ::pilo::core::io::dev_open_flags::none);
					if (tf0.error()) {
						return p_case->error(err, "open (create_always,read_write) text failed");
					}

					err = _s_test_nl_no_cache(1, p_case, &tf0, "\r\n", "\n");
					if (err != PILO_OK) {
						return -1;
					}					
					err = _s_test_nl_no_cache(1, p_case, &tf0, "\n", "\n");
					if (err != PILO_OK) {
						return -1;
					}
					err = _s_test_nl_no_cache(1, p_case, &tf0, "\n", "\r\n");
					if (err != PILO_OK) {
						return -1;
					}
					err = _s_test_nl_no_cache(1, p_case, &tf0, "\r\n", "\r\n");
					if (err != PILO_OK) {
						return -1;
					}




					tf0.close();
					p_case->set_result(PILO_OK);
					return PILO_OK;
					
				}

				int case_file_openmode(::pilo::core::testing::func_test_case* p_case)
				{
					::pilo::core::io::path p;
					::pilo::err_t err = PILO_OK;

					p.set("test_fs/oc/f0.txt", predefined_pilo_dir::tmp);
					if (p.invalid())
					{
						return p_case->error(PERR_INVALID_PATH, "init path failed");
					}

					p.remove(true, true);

					::pilo::core::io::file<> f0;

					f0.open(&p, ::pilo::core::io::creation_mode::open_existing, ::pilo::core::io::access_permission::read_write, ::pilo::core::io::dev_open_flags::none);
					if (!f0.error()) {
						return p_case->error(err, "open (open_existing,read_write) should failed");
					}

					f0.clear_error();
									
					f0.open(&p, ::pilo::core::io::creation_mode::create_always, ::pilo::core::io::access_permission::read_write, ::pilo::core::io::dev_open_flags::none);
					if (f0.error()) {
						return p_case->error(err, "open (create_always,read_write) failed");
					}

					if (f0.exist() != PERR_FSNODE_EXIST) {
						return p_case->error(err, "open (create_always,read_write) failed");
					}
					f0.close();

					f0.open(&p, ::pilo::core::io::creation_mode::open_existing, ::pilo::core::io::access_permission::read_write, ::pilo::core::io::dev_open_flags::none);
					if (f0.error()) {
						return p_case->error(err, "open (open_existing,read_write) failed");
					}

					f0.close();

					f0.open(&p, ::pilo::core::io::creation_mode::create_neo, ::pilo::core::io::access_permission::read_write, ::pilo::core::io::dev_open_flags::none);
					if (! f0.error()) {
						return p_case->error(err, "open (create_neo,read_write) should failed");
					}
					f0.clear_error();
					f0.remove();

					f0.open(&p, ::pilo::core::io::creation_mode::create_neo, ::pilo::core::io::access_permission::read_write, ::pilo::core::io::dev_open_flags::none);
					if (f0.error()) {
						return p_case->error(err, "open (create_neo,read_write) failed");
					}
					f0.close();

					f0.open(&p, ::pilo::core::io::creation_mode::open_always, ::pilo::core::io::access_permission::read_write, ::pilo::core::io::dev_open_flags::none);
					if (f0.error()) {
						return p_case->error(err, "open (create_neo,read_write) failed");
					}
					f0.close();


					

					
					p_case->set_result(PILO_OK);
					return 0;
				}
			}
		}
	}
}