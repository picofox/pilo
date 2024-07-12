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
				int case_file_openmode(::pilo::core::testing::func_test_case* p_case)
				{
					::pilo::core::io::path p;
					::pilo::err_t err = PILO_OK;

					p.set("test_fs/oc/f0.txt", predefined_pilo_dir_enum::tmp);
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