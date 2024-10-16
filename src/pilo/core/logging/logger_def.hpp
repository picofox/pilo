#ifndef _pilo_core_log_logger_def_hpp_
#define _pilo_core_log_logger_def_hpp_

#include "../../pilo.hpp"



namespace pilo {
    namespace core {
        namespace logging {

            const ::pilo::u32_t FlagBak = 0x00000001;
            const ::pilo::u32_t FlagZip = 0x00000002;
            const ::pilo::u32_t DefaultFlags = (FlagBak);


            const ::pilo::u32_t Date =          0x00000001;
            const ::pilo::u32_t Time =          0x00000002;
            const ::pilo::u32_t TimeZone =      0x00000004;
            const ::pilo::u32_t Seq =           0x00000008;
            const ::pilo::u32_t TotalSeq =      0x00000010;
            const ::pilo::u32_t TimeStamp =     0x00000020;
            const ::pilo::u32_t Level =         0x00000040;
            const ::pilo::u32_t Pid =           0x00000080;
            const ::pilo::u32_t StdTid =        0x00000100;
            const ::pilo::u32_t LocalTid =      0x00000200;
            const ::pilo::u32_t ProcName =      0x00000400;
            const ::pilo::u32_t PPid =          0x00000800;
            const ::pilo::u32_t SavedDate =     0x00001000;
            const ::pilo::u32_t SavedTime =     0x00002000;

            const ::pilo::u32_t DefaultHeaders = (Date | Time | Seq | TotalSeq | Level | Pid);
            const ::pilo::u32_t DefaultBakNameSuffix = (SavedDate | SavedTime);
            const ::pilo::u32_t DefaultNameSuffix = (PPid);

            const ::pilo::u8_t DevNone = 0x00;
            const ::pilo::u8_t DevLogFile = 0x01;
            const ::pilo::u8_t DevStdOut = 0x02;
            const ::pilo::u8_t DevStdErr = 0x04;

            const ::pilo::u8_t DefaultDevices = (DevLogFile | DevStdOut);


            enum class logger_type : ::pilo::u8_t
            {
                local_spst_text = 0,
                local_mpst_text = 1,
                local_spmt_text = 2,
                local_mpmt_text = 3,
            };

            enum class level : ::pilo::u8_t
            {
                none = 0,
                fatal = 1,
                error = 2,
                warn = 3,
                info = 4,
                debug = 5,
            };

            enum class splition_type : ::pilo::u8_t
            {
                none = 0,
                by_day = 1,
                by_hour = 2,
            };

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

            const ::pilo::i32_t g_level_name_lens[6]{
                4, 5, 5, 4, 4, 5,
            };

            const char* const g_splition_type_names[3]{
                "none",
                "by_day",
                "by_hour",
            };


            const char* const g_output_dev_names[4]{
                "file",
                "stdout",
                "stderr",
            };

            const char* const g_predef_elment_names[16]{
                "Date",
                "Time",
                "TimeZone",
                "Seq",
                "TotalSeq",
                "TimeStamp",
                "Level",
                "Pid",
                "StdTid",
                "LocalTid",
                "ProcName",
                "PPid",
                "SavedDate",
                "SavedTime",
            };

            const char* const g_flags[2]{
                "FlagBak",
                "FlagZip"
            };




        }
    }
}





#endif