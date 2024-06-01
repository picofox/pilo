#include "error.hpp"
#include "core/io/formatted_io.hpp"
#include "core/string/string_operation.hpp"
#include <sstream>


#ifdef WINDOWS
#else
    extern int errno;
#endif

#define PMM_NUMBER_BITS     (7)
#define PMM_PRED_BITS       (10)
#define PMM_SUBJECT_BITS    (11)
#define PMM_LAYER_BITS      (3)

#define PMM_NUMBER_MASK     ((1<<PMM_NUMBER_BITS) - 1)
#define PMM_PRED_MASK       ((1<<PMM_PRED_BITS) - 1)
#define PMM_SUBJECT_MASK    ((1<<PMM_SUBJECT_BITS) - 1)
#define PMM_LAYER_MASK      ((1<<PMM_LAYER_BITS) - 1)

#define PMM_PRED_OFF        (7)
#define PMM_SUBJECT_OFF     (17)
#define PMM_LAYER_OFF       (28)


namespace pilo
{
    static const char* g_err_layer_name[PEL_COUNT] =
    {
        "", "Core", "Frk", "App"
    };

    static const char* g_err_subjuect_name[PES_COUNT] =
    {
        "Non-Spec", "Operation", "Memroy","Pointer","Param"
        ,"Resource", "Object", "Buffer", "Message", "Value"
        ,"TLV","TestCase", "TLV internal data", "TLV internal array", "Element"
        , "Container", "Wired Key Type", "Wired Value Type", "Get CWD", "File Attribute"
        , "Symbol Link", "Text", "Path String", "Dir", "File"
    };

    static const char* g_err_predict_desc[PEP_COUNT]
    {
        "OK", "OK but with info", "no sense", "is null", "is empty"
        , "is invalid", "not exist", "is duplicated", "is temporary NA", "is not support"
        , "is disabled", "is aborted", "is canceled", "Read Failed", "Write Failed"
        , "offset invalid", "too small", "too large", "loops infinitely", "length invalid"
        , "lenth is inconsistent", "buffer data is partial", "is insufficient", "data inconsistent", "is valid."
        , "type mismatch.", "validation Failed.", "array index out of bound.", "wired type convert Failed.", "clone Failed."
        , "Already exist","Charset Conv Failed", "Contain Invalid chars", "Is Incomplete", "Format Fatal Error"
        , "Creation Failed", "Deletion Failed", "Link Failed", "Open Failed"
    };   

    os_error_number_t get_os_last_error()
    {
#ifdef WINDOWS
        return GetLastError();
#else
        return errno;
#endif
    }

    const char* cstr_os_error(char* buffer, ::pilo::i64_t cc_buffer, ::pilo::os_error_number_t os_err)
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
        if (strerror_r(os_err, buffer, cc_buffer) != 0)
        {
            ::pilo::core::string::copyz(buffer, cc_buffer, "Get os error string failed!");
        }
#endif
        return ::pilo::core::string::trim_inplace(buffer, -1, nullptr);
    }

    err_t make_error(::pilo::i32_t layer, ::pilo::i32_t subject, ::pilo::i32_t predict, ::pilo::i32_t number, os_error_number_t* os_err)
    {
        if (os_err == nullptr)
        {
            return (((layer & PMM_LAYER_MASK) << PMM_LAYER_OFF)
                | ((subject & PMM_SUBJECT_MASK) << PMM_SUBJECT_OFF)
                | ((predict & PMM_PRED_MASK) << PMM_PRED_OFF)
                | ((number & PMM_NUMBER_MASK)));
        }
        else
        {
            *os_err = get_os_last_error();
            if (*os_err != 0)
            {
                return (((layer & PMM_LAYER_MASK) << PMM_LAYER_OFF)
                    | ((subject & PMM_SUBJECT_MASK) << PMM_SUBJECT_OFF)
                    | ((predict & PMM_PRED_MASK) << PMM_PRED_OFF)
                    | ((number & PMM_NUMBER_MASK)))
                    | 1 << 31;
            }
            else
            {
                return (((layer & PMM_LAYER_MASK) << PMM_LAYER_OFF)
                    | ((subject & PMM_SUBJECT_MASK) << PMM_SUBJECT_OFF)
                    | ((predict & PMM_PRED_MASK) << PMM_PRED_OFF)
                    | ((number & PMM_NUMBER_MASK)));
            }
            
        }
    }


    bool ex_error(err_t err, ::pilo::i32_t* layer, ::pilo::i32_t* subject, ::pilo::i32_t* predict, ::pilo::i32_t* number, os_error_number_t* os_err)
    {
        if (layer != nullptr)
            *layer = (err >> PMM_LAYER_OFF) & PMM_LAYER_MASK;
        if (subject != nullptr)
            *subject = (err >> PMM_SUBJECT_OFF) & PMM_SUBJECT_MASK;
        if (predict != nullptr)
            *predict = (err >> PMM_PRED_OFF) & PMM_PRED_MASK;
        if (number != nullptr)
            *number = (err & PMM_NUMBER_MASK);

        if (((err >> 31) & 1) == 0)
        {
            if (os_err != nullptr)
            {
                *os_err = 0;
            }
            return false;
        }
        else
        {
            if (os_err != nullptr)
            {
                *os_err = get_os_last_error();
            }
            return true;
        }
    }

    const char* cstr_error(err_t err, char * buffer, ::pilo::i64_t buffer_capacity, const char* prefix)
    {
        ::pilo::i32_t layer;
        ::pilo::i32_t subject;
        ::pilo::i32_t predict;
        ::pilo::i32_t number;
        os_error_number_t os_err;
        bool has_os_err = ex_error(err, &layer, &subject, &predict, &number, &os_err);
        if (prefix == nullptr)
            prefix = "";

        ::pilo::core::io::string_formated_output(buffer, buffer_capacity, "%s[%s] %s %s #%d"
            , prefix
            , g_err_layer_name[layer]
            , g_err_subjuect_name[subject]
            , g_err_predict_desc[predict]
            , number);

        if (has_os_err)
        {
            ::pilo::i64_t poff = ::pilo::core::string::character_count(buffer);
            if (poff + 8 < buffer_capacity)
            {
                cstr_os_error(buffer + poff, buffer_capacity - poff, os_err);
            }
        }

        return buffer;
    }

    std::string str_error(err_t err, const char* prefix)
    {
        ::pilo::i32_t layer;
        ::pilo::i32_t subject;
        ::pilo::i32_t predict;
        ::pilo::i32_t number;
        os_error_number_t os_err;
        bool has_os_err = ex_error(err, &layer, &subject, &predict, &number, &os_err);
        if (prefix == nullptr)
            prefix = "";

        std::stringstream ss;

        ss << prefix << '[' << g_err_layer_name[layer] << "] " << g_err_subjuect_name[subject] << ' ' << g_err_predict_desc[predict] << " #" << number;
        
        if (has_os_err)
        {
            char buffer[128] = { 0 };
            cstr_os_error(buffer, 128, os_err);
            ss << " (" << buffer << ')';
        }

        return ss.str();
    }

    ::pilo::i32_t ex_err_layer(err_t err)
    {
        return (err >> PMM_LAYER_OFF) & PMM_LAYER_MASK;        
    }

    ::pilo::i32_t ex_err_subject(err_t err)
    {
        return (err >> PMM_SUBJECT_OFF) & PMM_SUBJECT_MASK;        
    }

    ::pilo::i32_t ex_err_prediction(err_t err)
    {
       return (err >> PMM_PRED_OFF) & PMM_PRED_MASK;
    }

    ::pilo::i32_t ex_err_number(err_t err)
    {
       return (err & PMM_NUMBER_MASK);
    }

    bool is_err_sub_and_type(::pilo::err_t err, ::pilo::i32_t subject, ::pilo::i32_t predict)
    {
        if (subject == ((err >> PMM_SUBJECT_OFF) & PMM_SUBJECT_MASK))
        {
            if (predict == ((err >> PMM_PRED_OFF) & PMM_PRED_MASK))
            {
                return true;
            }
        }
        return false;
    }

    bool is_no_err(::pilo::err_t err)
    {
        if (((err >> PMM_PRED_OFF) & PMM_PRED_MASK) == PEP_OK)
            return true;
        return false;
    }

    ::pilo::i32_t is_ok_or_err_type(::pilo::err_t err, ::pilo::i32_t predict)
    {
        ::pilo::i32_t pred = ((err >> PMM_PRED_OFF) & PMM_PRED_MASK);
        if (pred == PEP_OK)
        {
            return 0;
        }
        else
        {
            if (predict == pred)
            {
                return 1;
            }
        }
        return -1;
    }


    err_t make_core_error(::pilo::i32_t subject, ::pilo::i32_t predict, ::pilo::i32_t number, os_error_number_t* os_err)
    {
        return make_error(PEL_CORE, subject, predict, number, os_err);
    }

}
