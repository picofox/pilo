#include "./logger_interface.hpp"
#include <string>


namespace pilo {
    namespace core {
        namespace logging {

            const char* const g_logger_type_names[4]{
                "local_spst_text",
                "local_mpst_text",
                "local_spmt_text",
                "local_mpmt_text",
            };
        
            const char* const g_level_names[6]{
                "none",
                "fatal",
                "error",
                "warn",
                "info",
                "debug",
            };

            const char*  const g_splition_type_names[3]{
                "none",
                "by_day",
                "by_hour",
            };
                        

            const char* const g_output_dev_names[4] {
                "file",
                "stdout",
                "stderr",
            };

            const char* const g_predef_elment_names[14]{
                "Date",
                "Time",
                "TimeZone",
                "Seq",
                "MicroSec",
                "TimeStamp",
                "Level",
                "Pid",
                "StdTid",
                "LocalTid",
                "SrcFile",
                "SrcLine",
                "ProcName",
                "PPid",
            };

            const char* const g_flags[2]{
                "FlagBak",
                "FlagZip"
            };



        }
    }
}


