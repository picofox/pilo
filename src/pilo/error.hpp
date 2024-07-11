#ifndef __pilo_error_hpp_
#define __pilo_error_hpp_

#include "types.hpp"
#include <string>

#define PILO_OK  (0)

#define PERR_OK                 (0)
#define PERR_OK_WITH_INFO       (1)
#define PERR_NOOP               (2)
#define PERR_NULL_PARAM         (3)
#define PERR_INVALID_PARAM      (4)
#define PERR_PARAM_OOR          (5)
#define PERR_PARAM_OOR_U        (6)
#define PERR_PARAM_OOR_L        (7)
#define PERR_NULL_PTR           (8)
#define PERR_INSUF_HEAP         (9)

#define PERR_FAIL_ALLOC         (10)
#define PERR_MIS_DATA_TYPE      (11)  
#define PERR_FILE_CREAET_FAIL   (12)
#define PERR_FILE_DEL_FAIL      (13)
#define PERR_FILE_RENAME_FAIL   (14)
#define PERR_FILE_COPY_FAIL     (15)
#define PERR_IO_CREATE_FAIL     (16)
#define PERR_IO_INIT_FAIL       (17)
#define PERR_IO_FINA_FAIL       (18)
#define PERR_IO_OPEN_FAIL       (19)

#define PERR_IO_CLOSE_FAIL      (20)
#define PERR_IO_READ_FAIL       (21)
#define PERR_IO_WRITE_FAIL      (22)
#define PERR_IO_CTRL_FAIL       (23)
#define PERR_IO_SYNC_FAIL       (24)
#define PERR_EXIST              (25)
#define PERR_NON_EXIST          (26)       
#define PERR_NULL_PATH          (27)
#define PERR_INVALID_PATH       (28)
#define PERR_PATHSTR_OOL        (29)

#define PERR_FSNODE_EXIST       (30)
#define PERR_FSNODE_NON_EXIST   (31)
#define PERR_LEN_TOO_SAMLL      (32)
#define PERR_LEN_TOO_LARGE      (33)
#define PERR_LEN_IS_ZERO        (34)
#define PERR_DIR_CREAET_FAIL    (35)
#define PERR_DIR_DEL_FAIL       (36)
#define PERR_DIR_RENAME_FAIL    (37)
#define PERR_DIR_COPY_FAIL      (38)
#define PERR_FSNODE_LINK_FAIL   (39)

#define PERR_GET_PATH_FAIL      (40)
#define PERR_DIR_TRAVAL_FAIL    (41)
#define PERR_NO_OVERWRITING     (42)
#define PERR_INV_PARAM_DT       (43)
#define PERR_ICONV_FAIL         (44)
#define PERR_RD_PARTIAL_DATA    (45)
#define PERR_WR_PARTIAL_DATA    (46)
#define PERR_IDX_TOO_OOB        (47)
#define PERR_IDX_TOO_SMALL      (48)
#define PERR_IDX_TOO_LARGE      (49)

#define PERR_OP_UNSUPPORT       (50)
#define PERR_VAL_TOO_SAMLL      (51)
#define PERR_VAL_TOO_LARGE      (52)
#define PERR_INC_DATA           (53)
#define PERR_OBJECT_CLONE_FAIL  (54)
#define PERR_VAL_EMPTY          (55)
#define PERR_RETRY              (56)
#define PERR_USER_CANCEL        (57)
#define PERR_TESTCASE_FAIL      (58)
#define PERR_INV_KEY_TYPE       (59)

#define PERR_INV_VAL_TYPE       (60)
#define PERR_INV_OBJECT         (61)
#define PERR_INF_LOOP           (62)
#define PERR_OFF_TOO_SAMLL      (63)
#define PERR_OFF_TOO_LARGE      (64)
#define PERR_OFF_IS_ZERO        (65)
#define PERR_INV_OFF            (66)
#define PERR_INV_LEN            (67)
#define PERR_FSNODE_RDLINK_FAIL (68)
#define PERR_DIR_NOT_EMPTY      (69)

#define PERR_HAS_PREV_ERR       (70)
#define PERR_INV_IO_STATE       (71)
#define PERR_NOT_A_REG_FILE     (72)
#define PERR_IO_SEEK_FAIL       (73)
#define PERR_LOCK_FILE_FAIL     (74)

#define PERR_UNLOCK_FILE_FAIL   (75)

namespace pilo
{
    __inline os_error_number_t get_os_last_err()
    {
#ifdef WINDOWS
        return GetLastError();
#else
        return errno;
#endif
    }

    __inline ::pilo::err_t mk_err(::pilo::err_t perr)
    {
        ::pilo::os_error_number_t oserr = get_os_last_err();
        return (((::pilo::err_t)(oserr) << 16) | ((::pilo::u16_t)perr));
    }

    __inline ::pilo::err_t mk_err(::pilo::err_t perr, ::pilo::os_error_number_t oserr)
    {
        return (((::pilo::err_t)(oserr) << 16) | ((::pilo::u16_t)perr));
    }

    __inline ::pilo::err_t mk_perr(::pilo::err_t perr)
    {
        return (((::pilo::err_t)(0) << 16) | ((::pilo::u16_t)perr));
    }

    __inline ::pilo::os_error_number_t ex_oserr(::pilo::err_t err)
    {
        return (::pilo::os_error_number_t)((unsigned short)((err >> (16)) & 0xFFFF));
    }

    __inline ::pilo::err_t ex_perr(::pilo::err_t err)
    {
        return (::pilo::err_t)((unsigned short)((err >> (0)) & 0xFFFF));
    }

    __inline void ex_err(::pilo::err_t err, ::pilo::err_t* perr, ::pilo::os_error_number_t* oserr)
    {
        if (perr != nullptr) *perr = ex_perr(err);
        if (oserr != nullptr) *oserr = ex_oserr(err);
    }

    __inline bool is_err(::pilo::err_t err, ::pilo::err_t perr, ::pilo::os_error_number_t oserr)
    {
        return (perr == ex_perr(err) && oserr == ex_oserr(err));
    }

    __inline ::pilo::i32_t is_ok_perr(::pilo::err_t err, ::pilo::err_t perr)
    {
        if (err == PILO_OK) return 0;
        else if (perr == ex_perr(err)) return 1;
        return -1;
    }

    __inline bool is_perr(::pilo::err_t err, ::pilo::err_t perr)
    {
        return (perr == ex_perr(err));
    }

    __inline bool is_oserr(::pilo::err_t err, ::pilo::os_error_number_t oserr)
    {
        return (oserr == ex_oserr(err));
    }
    const char* cstr_err_digest(err_t err, char* buffer, ::pilo::i64_t buffer_capacity, const char* prefix);
    const char* cstr_err_verbose(err_t err, char* buffer, ::pilo::i64_t buffer_capacity, const char* prefix);
    const char* cstr_err(err_t err, char* buffer, ::pilo::i64_t buffer_capacity, const char* prefix = nullptr, bool verbose = false);
    std::string str_err(err_t err, const char* prefix = nullptr, bool verbose = false);
}

#endif //__pilo_error_hpp_