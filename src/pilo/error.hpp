#ifndef __pilo_error_hpp_
#define __pilo_error_hpp_

#include "types.hpp"
#include <string>

#define PILO_OK  (0)

#define PEL_NONE            (0)
#define PEL_CORE            (1)
#define PEL_FRK             (2)
#define PEL_APP             (3)
#define PEL_COUNT           (4)

#define PES_NONE            (0)
#define PES_OP              (1)
#define PES_MEM             (2)
#define PES_PTR             (3)
#define PES_PARAM           (4)
#define PES_RC              (5)
#define PES_OBJ             (6)
#define PES_BUFFER          (7)
#define PES_MSG             (8)
#define PES_VAL             (9)
#define PES_TLV             (10)
#define PES_TCASE           (11)
#define PES_TLV_INT         (12)
#define PES_TLV_INTARR      (13)
#define PES_ELEM            (14)
#define PES_CONT            (15)
#define PES_WK_TYPE          (16)
#define PES_WV_TYPE          (17)
#define PES_GETCWD          (18)
#define PES_FILE_ATTR       (19)
#define PES_SYMLINK        (20)
#define PES_TEXT           (21)
#define PES_PATH_STR            (22)
#define PES_DIR              (23)
#define PES_FILE              (24)
#define PES_COUNT           (25)



#define PEP_OK              (0)
#define PEP_OK_WITH_INFO    (1)
#define PEP_NOSENSE         (2)
#define PEP_IS_NULL         (3)
#define PEP_IS_EMPTY        (4)
#define PEP_IS_INVALID      (5)
#define PEP_NOT_EXIST       (6)
#define PEP_NOT_DUP         (7)
#define PEP_RETRY           (8)
#define PEP_UNSUPPORT       (9)
#define PEP_DISABLED        (10)
#define PEP_ABORTED         (11)
#define PEP_CANCELED        (12)
#define PEP_RDFAIL          (13)
#define PEP_WRFAIL          (14)
#define PEP_OFF_INV         (15)
#define PEP_TOO_SMALL       (16)
#define PEP_TOO_LARGE       (17)
#define PEP_INF_LOOP        (18)
#define PEP_LEN_INV         (19)
#define PEP_INC_LEN         (20)
#define PEP_PART_DATA       (21)
#define PEP_INSUFF          (22)
#define PEP_INC_DATA        (23)
#define PEP_IS_VALID        (24)
#define PEP_TYPE_MISMATCH   (25)
#define PEP_VDT_FAILED      (26)
#define PEP_ARR_IDX_OOB     (27)
#define PEP_WIRED_TYPE_CONV_FAILED  (28)
#define PEP_CLONE_FAILED    (29)
#define PEP_EXIST           (30)
#define PEP_INV_CHARS       (31)
#define PEP_INCOMP          (32)
#define PEP_ICONV_FAILED    (33)
#define PEP_FMT_FATAL       (34)   
#define PEP_CREATE_FAILED   (35)
#define PEP_DEL_FAILED      (36)  

#define PEP_COUNT           (37)



namespace pilo
{
    typedef i32_t err_t;  

    os_error_number_t get_os_last_error();
 
    const char* cstr_os_error(char* buffer, ::pilo::i64_t cc_buffer, ::pilo::os_error_number_t os_err);

    err_t make_error(::pilo::i32_t layer, ::pilo::i32_t subject, ::pilo::i32_t predict, ::pilo::i32_t number = 0, os_error_number_t * os_err = nullptr);
    err_t make_core_error(::pilo::i32_t subject, ::pilo::i32_t predict, ::pilo::i32_t number = 0, os_error_number_t* os_err = nullptr);

    bool ex_error(err_t err, ::pilo::i32_t* layer, ::pilo::i32_t* subject, ::pilo::i32_t* predict, ::pilo::i32_t* number, os_error_number_t* os_err);

    const char* cstr_error(err_t err, char * buffer, ::pilo::i64_t buffer_capacity, const char* prefix = nullptr);

    std::string str_error(err_t err, const char* prefix = nullptr);

    ::pilo::i32_t ex_err_layer(err_t err);
    ::pilo::i32_t ex_err_subject(err_t err);
    ::pilo::i32_t ex_err_prediction(err_t err);
    ::pilo::i32_t ex_err_number(err_t err);

    bool is_err_sub_and_type(::pilo::err_t err, ::pilo::i32_t subject, ::pilo::i32_t predict);
    bool is_no_err(::pilo::err_t err);
    ::pilo::i32_t is_ok_or_err_type(::pilo::err_t err, ::pilo::i32_t predict);
}

#endif //__pilo_error_hpp_