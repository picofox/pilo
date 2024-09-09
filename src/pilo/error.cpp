#include "error.hpp"
#include "core/io/formatted_io.hpp"
#include "core/string/string_operation.hpp"
#include <sstream>


#ifdef WINDOWS
#else
    extern int errno;
#endif


namespace pilo
{
    const char* g_error_messages[] = {
        "no error", "ok with info", "nosense operation","null param","invalid param"
        , "param out of range", "param too large", "param too small", "null ptr", "insufficient heap"

        , "alloc failed", "mismatch datatype", "create file failed", "delete file failed", "rename file failed"
        , "copy file failed", "io create failed", "io initialize failed", "io finalize failed", "io open failed"

        , "io close failed", "io read failed", "io  write failed", "io control failed", "io sync failed."
        , "already exist", "not exist", "null path", "invalid path", "too large (or small) path string"

        , "fs node already exist", "fs node not exist", "too small length", "too large length", "zero length"
        , "create dir failed", "delete dir failed", "rename dir failed", "copy dir failed", "link dir failed"

        , "get path failed", "travel dir failed", "cat not overwrite", "invalid param data type", "charset conversion failed."
        , "partial data has been read", "partial data has been written", "index oob", "index too large", "index to small"

        , "unsupport operation", "value too large", "value too small", "inconsistent data", "clone object failed"
        , "empty value", "could retry", "user cancelled", "test case failed", "invalid key type"

        , "invalid value type", "invalid object", "infinit loop", "offset too small", "offset too large"
        , "offset is zero", "invalid offset", "invalid length", "read file link failed", "dir not empty"

        , "has previous error", "invalid io state", "fs node not a regular file", "io seek failed", "lock file failed"
        , "unlock file failed", "not initialized.", "already initialized.", "end of file", "tell file pos failed."

        , "set file size failed.", "parse json fail", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
        , "", "", "", "", ""
    };


    const char* cstr_os_err(char* buffer, ::pilo::i64_t cc_buffer, ::pilo::os_error_number_t os_err)
    {
#if defined(WIN32) || defined(WINDOWS)
        BOOL fOk = FormatMessageA(
            FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            os_err,
            MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
            (LPSTR)buffer,
            (DWORD)cc_buffer, NULL);
        if (!fOk) {
            HMODULE hDll = LoadLibraryExA("netmsg.dll", NULL, DONT_RESOLVE_DLL_REFERENCES);
            if (hDll != NULL) {
                FormatMessageA(
                    FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_FROM_SYSTEM,
                    hDll,
                    os_err,
                    MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
                    (LPSTR)buffer,
                    (DWORD)cc_buffer, NULL);
                FreeLibrary(hDll);
            }
        }

#else  
        const char* ret = strerror_r(os_err, buffer, cc_buffer);
        if (ret != buffer) {
            ::pilo::core::string::copyz(buffer, cc_buffer, ret);
        }

#endif
        return ::pilo::core::string::trim_inplace(buffer, -1, nullptr);
    }

    const char* cstr_err_digest(err_t err, char* buffer, ::pilo::i64_t buffer_capacity, const char* prefix)
    {
        ::pilo::err_t perr = 0;
        os_error_number_t os_err = 0;
        ex_err(err, &perr, &os_err);
        if (prefix == nullptr)
            prefix = "";
        ::pilo::core::io::string_formated_output(buffer, buffer_capacity, "%s: %d / %u"
            , prefix, perr, os_err);
        return buffer;
    }

    const char* cstr_err_verbose(err_t err, char* buffer, ::pilo::i64_t buffer_capacity, const char* prefix)
    {
        ::pilo::err_t perr = 0;
        os_error_number_t os_err = 0;
        ex_err(err, &perr, &os_err);
        if (prefix == nullptr)
            prefix = "";

        if (os_err != 0)
        {
            char os_errbuf[128] = { 0 };
            cstr_os_err(os_errbuf, sizeof(os_errbuf), os_err);

            ::pilo::core::io::string_formated_output(buffer, buffer_capacity, "%s%s (%d) / %s (%u): "
                , prefix, g_error_messages[perr], perr, os_errbuf, os_err);
        }
        else {
            ::pilo::core::io::string_formated_output(buffer, buffer_capacity, "%s%s (%d)"
                , prefix, g_error_messages[perr], perr);
        }

        return buffer;
    }

    const char* cstr_err(err_t err, char* buffer, ::pilo::i64_t buffer_capacity, const char* prefix, bool verbose)
    {
        if (verbose)
            return cstr_err_verbose(err, buffer, buffer_capacity, prefix);
        else
            return cstr_err_digest(err, buffer, buffer_capacity, prefix);
    }


    std::string str_err(err_t err, const char* prefix, bool verbose)
    {
        char buffer[256] = { 0 };
        cstr_err(err, buffer, sizeof(buffer), prefix, verbose);
        return std::string(buffer);
    }

}
