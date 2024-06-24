#include "path.hpp"
#include "../i18n/encoding_conversion.hpp"
#include "../memory/util.hpp"
#include "../string/string_operation.hpp"
#include "../pattern/resource_cleaner.hpp"
#include <vector>

#ifdef WINDOWS
#include <Shlobj.h>
#include <atlstr.h>
#include <ShellAPI.h>

#define PMI_PATH_PREF_ABS_LEN (4)

#else
#define PMI_PATH_PREF_ABS_LEN (0)

#include <sys/stat.h>
#include <dirent.h>

#endif

namespace pilo {
    namespace core {
        namespace io {
            ::pilo::err_t
            path::s_node_deletion_handler(::pilo::i8_t event_type, const path *src_path, ::pilo::i8_t, ::pilo::i32_t,
                                          ::pilo::i32_t, void *) {
                if (event_type == path::evt_node_visiting) {
                    return path::remove_file(src_path->fullpath(), src_path->length());
                } else if (event_type == path::evt_dir_leaving) {
                    return path::remove_dir(src_path->fullpath(), src_path->length());
                }

                return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH);
            }


            bool path::equals_to(const ::pilo::core::io::path &p) const {
                if (this == &p) {
                    return true;
                }

                if (this->invalid() && p.invalid()) {
                    return true;
                }

                if (this->_m_type != p._m_type)
                    return false;
                if (this->_m_ext_name_len != p._m_ext_name_len)
                    return false;
                if (this->_m_lastpart_start_pos != p._m_lastpart_start_pos)
                    return false;
                if (this->_m_length != p._m_length)
                    return false;
                if (0 != ::pilo::core::string::strict_compare(this->_m_pathstr_ptr, 0, p._m_pathstr_ptr, 0, -1))
                    return false;
                return true;
            }

            ::pilo::i8_t path::absolute_type() const {
                if (this->invalid())
                    return path::path_type_na;

                return path::absolute_type(this->_m_pathstr_ptr, nullptr, this->_m_length);
            }


            ::pilo::err_t path::set(const char *p, ::pilo::i64_t len, ::pilo::pathlen_t extra,
                                    predefined_pilo_dir_enum rel_to_abs_basis) {
                if (p == nullptr) return ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL);
                bool isabs = false;
                char sb[1] = {0};
                ::pilo::char_buffer_t buffer(sb, sizeof(sb), 0, false);
                ::pilo::i8_t fs_type = ::pilo::core::io::path::path_type_na;
                ::pilo::err_t err = validate_path(&buffer, p, len, extra, fs_type, isabs, rel_to_abs_basis);

                if (err != PILO_OK) {
                    return err;
                }

                ::pilo::i32_t tmp_capa = buffer.size() + 1 + extra;
                if (this->_m_pathstr_ptr != nullptr && this->_m_capacity < tmp_capa) {
                    this->reset();
                }

                if (this->_m_pathstr_ptr == nullptr) {
                    if (buffer.is_dynamic()) {
                        this->_m_pathstr_ptr = buffer.begin();
                        this->_m_capacity = (::pilo::pathlen_t) buffer.capacity();
                        buffer.set_adopt(false);
                    } else {
                        if (tmp_capa < 0 || tmp_capa > path::length_max) {
                            return ::pilo::make_core_error(PES_PATH_STR, PEP_IS_INVALID);
                        }
                        this->_m_pathstr_ptr = (char *) PMF_HEAP_MALLOC(tmp_capa);
                        if (this->_m_pathstr_ptr == nullptr)
                            return ::pilo::make_core_error(PES_MEM, PEP_INSUFF);
                        ::pilo::core::string::n_copyz(this->_m_pathstr_ptr, this->_m_capacity, buffer.begin(),
                                                      buffer.size());
                        this->_m_capacity = (::pilo::pathlen_t) tmp_capa;
                    }
                } else //pathstr_ptr is notnull
                {
                    if (tmp_capa < 0 || tmp_capa > path::length_max) {
                        return ::pilo::make_core_error(PES_PATH_STR, PEP_IS_INVALID);
                    }
                    ::pilo::core::string::n_copyz(this->_m_pathstr_ptr, this->_m_capacity, buffer.begin(),
                                                  buffer.size());
                    this->_m_capacity = (::pilo::pathlen_t) tmp_capa;
                }

                this->_m_ext_name_len = path::invalid_ext_length;
                this->_m_lastpart_start_pos = 0;
                this->_m_length = (::pilo::pathlen_t) buffer.size();
                this->_m_type = fs_type;

                const char *filename_sep = ::pilo::core::string::rfind_char(this->_m_pathstr_ptr, this->_m_length,
                                                                            PMI_PATH_SEP);
                if (filename_sep == nullptr) {
                    this->_m_lastpart_start_pos = 0;
                } else {
                    this->_m_lastpart_start_pos = (::pilo::pathlen_t) (filename_sep - _m_pathstr_ptr + 1);

                    if (isabs) {
#ifdef  WINDOWS
                        if (this->_m_length == 6)
                        {
                            this->_m_lastpart_start_pos = 0;
                        }
#else
                        if (this->_m_length == 0) {
                            this->_m_lastpart_start_pos = -1;
                        }
#endif //  WINDOWS
                    }

                }

                for (::pilo::pathlen_t i = this->_m_length - 1; i > 0; i--) {
                    if (this->_m_pathstr_ptr[i] == PMI_PATH_SEP) {
                        break;
                    } else if (this->_m_pathstr_ptr[i] == '.') {
                        if (i > 1 && this->_m_pathstr_ptr[i - 1] != PMI_PATH_SEP) {
                            this->_m_ext_name_len = (::pilo::u8_t) (this->_m_length - i - 1);
                        }
                    }
                }

                return PILO_OK;
            }

            ::pilo::i8_t path::get_fs_info(::pilo::i8_t *target_node_type, path *target) const {
                ::pilo::i8_t fs_node_type = path::path_type_na;
                if (this->invalid()) {
                    return path::path_type_na;
                }

                if (target == nullptr) {
                    path::get_path_node_type(this->_m_pathstr_ptr, this->_m_length, path::local_fs_path, fs_node_type,
                                             target_node_type, nullptr);
                    return fs_node_type;
                } else {
                    char target_path_buffer[PMI_STCPARAM_PATH_DEFAULT_LENGTH] = {0};
                    ::pilo::char_buffer_t target_path(target_path_buffer, sizeof(target_path_buffer), 0, false);
                    ::pilo::err_t err = path::get_path_node_type(this->_m_pathstr_ptr, this->_m_length,
                                                                 path::local_fs_path, fs_node_type, target_node_type,
                                                                 &target_path);
                    if (err != PILO_OK)
                        return path::path_type_na;
                    target->set(target_path.begin(), target_path.size(), 0);
                    return fs_node_type;
                }
            }

            ::pilo::err_t path::forward_iterate(iter_path_part_func_t func, void *ctx, bool ignore_err) {
                if (this->invalid()) {
                    return ::pilo::make_core_error(PES_PATH_STR, PEP_IS_INVALID);
                }
                if (func == nullptr) {
                    return ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL);
                }

                if (this->is_root())
                    return PILO_OK;

                ::pilo::err_t err = PILO_OK;
                const char *p = nullptr;
                const char *s = _m_pathstr_ptr;
                ::pilo::i64_t l = (::pilo::i64_t) _m_length;
                ::pilo::i32_t idx = 0;
                ::pilo::pathlen_t beg = 0;
                if (this->absolute_type() == path::relative) {

                } else if (this->absolute_type() == path::other_absolute) {
#ifdef WINDOWS
                    beg = 2;
#else
                    return ::pilo::make_core_error(PES_OP, PEP_UNSUPPORT);
#endif
                } else if (this->absolute_type() == path::absolute) {
#ifdef WINDOWS
                    beg = 7;
#else
                    beg = 1;
#endif
                } else {
                    return ::pilo::make_core_error(PES_PATH_STR, PEP_TYPE_MISMATCH);
                }
                s += beg;
                l -= beg;
                while (true) {
                    p = ::pilo::core::string::find_char(s, l, PMI_PATH_SEP);
                    if (p != nullptr) {
                        ::pilo::pathlen_t plen = (::pilo::pathlen_t) (p - _m_pathstr_ptr);
                        err = func(this, plen, idx++, false, ctx);
                        if (err != PILO_OK) {
                            if (!ignore_err)
                                return err;
                        }

                        s = p + 1;
                        l = _m_length - plen - 1;
                        if (*s == 0 || l <= 0) {
                            break;
                        }
                    } else {
                        err = func(this, this->_m_length, idx++, true, ctx);
                        if (err != PILO_OK) {
                            if (!ignore_err)
                                return err;
                        }
                        break;
                    }
                }

                return err;
            }

            ::pilo::err_t path::create(::pilo::i8_t fs_node_type, bool is_force) {
                if (fs_node_type == path::fs_node_type_dir)
                    return this->forward_iterate(path::s_dir_create_handler, is_force ? (void *) "" : nullptr, false);
                else if (fs_node_type == path::fs_node_type_file)
                    return this->forward_iterate(path::s_file_create_handler, is_force ? (void *) "" : nullptr, false);

                else
                    return ::pilo::make_core_error(PES_PARAM, PEP_IS_INVALID);
            }

            ::pilo::err_t path::remove(bool content_only, bool follow_link) const {
                if (this->invalid())
                    return ::pilo::make_core_error(PES_PATH_STR, PEP_IS_VALID);

                return path::remove_dir_recursively(fullpath(), length(), content_only, follow_link);
            }

            ::pilo::err_t path::create_sub_dir(const char *file_path_str, ::pilo::pathlen_t path_len, bool is_force,
                                               path *result) const {
                path *ppath = result;
                path tmp;
                if (ppath == nullptr) {
                    ppath = &tmp;
                }
                *ppath = *this;

                ::pilo::err_t err = ppath->append(file_path_str, path_len);
                if (err != PILO_OK)
                    return err;
                return path::make_dir(ppath->fullpath(), ppath->length(), is_force);
            }

            ::pilo::err_t path::create_link_by(const path *target, bool is_force) const {
                return path::create_local_fs_link(this, target, is_force);
            }

            ::pilo::err_t path::create_sub_file(const char *file_path_str, ::pilo::pathlen_t path_len, bool is_force,
                                                path *result) const {
                path *ppath = result;
                path tmp;
                if (ppath == nullptr) {
                    ppath = &tmp;
                }
                *ppath = *this;
                ::pilo::err_t err = ppath->append(file_path_str, path_len);
                if (err != PILO_OK)
                    return err;
                return path::touch_file(ppath->fullpath(), ppath->length(), is_force);
            }

            ::pilo::err_t path::fill_with_cwd(::pilo::pathlen_t extra) {
                char cbf[PMI_PATH_MAX] = {0};
                ::pilo::char_buffer_t buffer(cbf, sizeof(cbf), 0, false);
                ::pilo::err_t err = ::pilo::core::io::path::get_cwd(buffer, path::sepmode_nosep, extra);
                if (err != PILO_OK)
                    return err;

                return this->set(buffer.begin(), buffer.size(), extra, ::pilo::predefined_pilo_dir_enum::cwd);
            }

            ::pilo::err_t path::fill_with_exe(::pilo::pathlen_t extra) {
                return path::get_executable_path(this, extra);
            }

            ::pilo::err_t path::fill_with_home(::pilo::pathlen_t extra) {
                path p;
                ::pilo::err_t err = p.fill_with_bin(extra);
                if (err != PILO_OK)
                    return err;
                ::pilo::pathlen_t rlen = 0;
                const char *par = p.parentpath(rlen);
                return this->set(par, rlen, extra);
            }

            ::pilo::err_t path::fill_with_bin(::pilo::pathlen_t extra) {
                path p;
                ::pilo::err_t err = p.fill_with_exe(extra);
                if (err != PILO_OK)
                    return err;
                ::pilo::pathlen_t rlen = 0;
                const char *par = p.parentpath(rlen);
                return this->set(par, rlen, extra);
            }

            ::pilo::err_t path::fill_with_cnf(::pilo::pathlen_t extra) {
                ::pilo::err_t err = fill_with_home(extra);
                if (err != PILO_OK)
                    return err;
                return append(PMS_STCPARAM_PILO_PREDEF_DIR_CNF, path::unknow_length, extra);
            }

            ::pilo::err_t path::fill_with_log(::pilo::pathlen_t extra) {
                ::pilo::err_t err = fill_with_home(extra);
                if (err != PILO_OK)
                    return err;
                return append(PMS_STCPARAM_PILO_PREDEF_DIR_LOG, path::unknow_length, extra);
            }

            ::pilo::err_t path::fill_with_tmp(::pilo::pathlen_t extra) {
                ::pilo::err_t err = fill_with_home(extra);
                if (err != PILO_OK)
                    return err;
                return append(PMS_STCPARAM_PILO_PREDEF_DIR_TMP, path::unknow_length, extra);
            }

            ::pilo::err_t path::append(const char *p, ::pilo::i64_t len, ::pilo::pathlen_t extra,
                                       predefined_pilo_dir_enum rel_to_abs_basis) {
                if (p == nullptr) return ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL);

                if (len == path::unknow_length)
                    len = ::pilo::core::string::character_count(p);
                if (len < 1) {
                    return ::pilo::make_core_error(PES_PARAM, PEP_IS_EMPTY);
                }
                bool isabs = false;
                ::pilo::i8_t fs_type = ::pilo::core::io::path::path_type_na;
                ::pilo::core::memory::object_array<char, PMI_PATH_MAX> tbuf;
                ::pilo::char_buffer_t buffer(this->_m_pathstr_ptr, this->_m_capacity, this->_m_length, true);

                if (p[0] != PMI_PATH_SEP) {
                    buffer.check_space(
                            (::pilo::i32_t) this->length() + (::pilo::i32_t) len + 2 + (::pilo::i32_t) extra);
                    buffer.append(PMI_PATH_SEP);
                    buffer.add_size(1);
                    buffer.append(0);
                } else {
                    buffer.check_space(
                            (::pilo::i32_t) this->length() + (::pilo::i32_t) len + 1 + (::pilo::i32_t) extra);
                }
                tbuf.check_space(buffer.capacity());
                ::pilo::core::string::n_copyz(tbuf.begin(), tbuf.capacity(), buffer.begin(), buffer.size());
                ::pilo::core::string::n_concatenate_inplace(tbuf.ptr(), tbuf.space_available(), p, len);
                tbuf.set_size(buffer.size() + (::pilo::i32_t) len);
                ::pilo::err_t err = validate_path(&buffer, tbuf.begin(), tbuf.size(), extra, fs_type, isabs,
                                                  rel_to_abs_basis);
                buffer.set_adopt(false);
                this->_m_pathstr_ptr = buffer.begin();
                if (err != PILO_OK) {
                    return err;
                }

                const char *filename_sep = ::pilo::core::string::rfind_char(buffer.begin(), buffer.size(),
                                                                            PMI_PATH_SEP);
                if (filename_sep == nullptr) {
                    this->_m_lastpart_start_pos = 0;
                } else {
                    this->_m_lastpart_start_pos = (::pilo::pathlen_t) (filename_sep - buffer.begin() + 1);
                    if (isabs) {
#ifdef  WINDOWS
                        if (buffer.size() == 6)
                        {
                            this->_m_lastpart_start_pos = 0;
                        }
#else
                        if (buffer.size() == 0) {
                            this->_m_lastpart_start_pos = -1;
                        }
#endif //  WINDOWS
                    }
                }

                this->_m_pathstr_ptr = buffer.begin();
                this->_m_capacity = (::pilo::pathlen_t) buffer.capacity();
                this->_m_length = (::pilo::pathlen_t) buffer.size();
                this->_m_type = fs_type;

                for (::pilo::pathlen_t i = this->_m_length - 1; i > 0; i--) {
                    if (this->_m_pathstr_ptr[i] == PMI_PATH_SEP) {
                        break;
                    } else if (this->_m_pathstr_ptr[i] == '.') {
                        if (i > 1 && this->_m_pathstr_ptr[i - 1] != PMI_PATH_SEP) {
                            this->_m_ext_name_len = (::pilo::u8_t) (this->_m_length - i - 1);
                        }
                    }
                }

                return PILO_OK;
            }


#ifdef WINDOWS
            ::pilo::err_t path::validate_path(::pilo::char_buffer_t* buffer, const char* path_str, ::pilo::i64_t path_str_len, ::pilo::pathlen_t extra, ::pilo::i8_t& fs_type, bool& isabs, predefined_pilo_dir_enum rel_to_abs_basis)
            {
                if (buffer == nullptr)
                    return ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL);
                fs_type = ::pilo::core::io::path::path_type_na;
                if (path_str == nullptr) return ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL);

                ::pilo::i64_t test_len = ::pilo::core::string::character_count(path_str);

                if (test_len < 1 || path_str_len < 1)
                {
                    return ::pilo::make_core_error(PES_PATH_STR, PEP_IS_EMPTY);
                }


                if (path_str_len == path::unknow_length)
                {
                    path_str_len = test_len;
                }
                else if (path_str_len > (path::length_max - (::pilo::i64_t)extra) - PMI_PATH_PREF_ABS_LEN)
                {
                    return ::pilo::make_core_error(PES_PATH_STR, PEP_TOO_LARGE);
                }

                ::pilo::core::memory::object_array<char, 64> tmp_path;
                ::pilo::i8_t abs_type = path::absolute_type(path_str, nullptr, (::pilo::pathlen_t)path_str_len);
                if (abs_type == path::relative)
                {
                    if (rel_to_abs_basis != predefined_pilo_dir_enum::count)
                    {
                        path prefix;
                        if (prefix.fill_with_predef_path(rel_to_abs_basis, 0) != PILO_OK)
                        {
                            return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL);
                        }
                        tmp_path.check_space((::pilo::i32_t)prefix.length() + (::pilo::i32_t)path_str_len + 2);
                        ::pilo::core::string::n_copyz(tmp_path.begin(), tmp_path.capacity(), prefix.fullpath(), prefix.length());
                        tmp_path.set_size(prefix.length());
                        if (path_str[0] != PMI_PATH_SEP)
                        {
                            *tmp_path.ptr() = PMI_PATH_SEP;
                            tmp_path.add_size(1);
                        }
                        ::pilo::core::string::n_copyz(tmp_path.ptr(), tmp_path.space_available(), path_str, path_str_len);
                        tmp_path.add_size((::pilo::i32_t)path_str_len);
                    }
                    else
                    {
                        tmp_path.check_space((::pilo::i32_t)path_str_len + 1);
                        ::pilo::core::string::n_copyz(tmp_path.begin(), tmp_path.capacity(), path_str, path_str_len);
                        tmp_path.set_size((::pilo::i32_t)path_str_len);
                    }
                }
                else
                {
   
                    if (::std::isalpha((unsigned char)(unsigned char)path_str[0]) && path_str[1] == ':')
                    {
                        tmp_path.check_space((::pilo::i32_t)path_str_len + 7);
                        ::pilo::core::string::n_copyz(tmp_path.begin(), tmp_path.capacity(), ::pilo::core::string::constants<char>::root_dir_sep(), ::pilo::core::string::constants<char>::root_dir_sep_length());
                        tmp_path.set_size((::pilo::i32_t) ::pilo::core::string::constants<char>::root_dir_sep_length());
                    }
                    else
                    {
                        tmp_path.check_space((::pilo::i32_t)path_str_len + 1);
                    }
                    ::pilo::core::string::n_copyz(tmp_path.ptr(), tmp_path.space_available(), path_str, path_str_len);
                    tmp_path.add_size((::pilo::i32_t)path_str_len);
                }

                ::pilo::pathlen_t endidx = 0;
                if (tmp_path.size() > 3 && ::pilo::core::string::strict_compare(tmp_path.begin(), 0
                    , ::pilo::core::string::constants<char>::root_dir_sep(), 0
                    , ::pilo::core::string::constants<char>::root_dir_sep_length()) == 0)
                {
                    if (tmp_path.size() >= 4 && tmp_path.size() < 6)
                    {
                        return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL);
                    }
                    isabs = true;
                    endidx = 4;
                }
                else if (tmp_path.size() > 1)
                {
                    if (tmp_path.at(0) == '\\' && tmp_path.at(1) == '\\')
                    {
                        endidx = 2;
                    }
                }

                ::pilo::core::string::replace_char(tmp_path.begin() + endidx, tmp_path.size() - endidx, '/', '\\');

                ::pilo::i64_t rscnt = 0;
                ::pilo::i64_t rscnt2 = 0;
                ::pilo::core::string::rescanable_replace_inplace(tmp_path.begin() + endidx, tmp_path.size() - endidx, tmp_path.capacity() - endidx, "\\\\", "\\", &rscnt);
                ::pilo::core::string::rescanable_replace_inplace(tmp_path.begin() + endidx, tmp_path.size() - endidx, tmp_path.capacity() - endidx, "\\.\\", "\\", &rscnt2);
                if (rscnt > 0 || rscnt2 > 0)
                    tmp_path.set_size((::pilo::i32_t) ::pilo::core::string::character_count(tmp_path.begin()));


                ::pilo::i32_t len_to_dec = 0;
                for (::pilo::i32_t i = (::pilo::pathlen_t)tmp_path.size() - 1; i >= endidx; i--)
                {
                    if (tmp_path.begin()[i] == PMI_PATH_SEP)
                    {
                        tmp_path.begin()[i] = 0;
                        len_to_dec--;

                    }
                    else
                        break;
                }
                tmp_path.add_size(len_to_dec);

                if (tmp_path.size() < 1)
                {
                    return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL);
                }

                //all cases

                if (tmp_path.size() == 1)
                {
                    if (tmp_path.begin()[0] == '\\' || tmp_path.begin()[0] == '/')
                        return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL, 1);
                    else if (::pilo::core::string::find_char(::pilo::core::string::constants<char>::illegal_path_chars(), (::pilo::i64_t) ::pilo::core::string::constants<char>::illegal_path_chars_length(), tmp_path.begin()[0]))
                        return ::pilo::make_core_error(PES_PATH_STR, PEP_INV_CHARS);
                    fs_type = ::pilo::core::io::path::local_fs_path;
                    buffer->check_space(2);
                    ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), tmp_path.begin(), 1);
                    buffer->set_size(1);
                    return PILO_OK;
                }
                else if (tmp_path.size() == 2)
                {
                    if ((tmp_path.begin()[0]) == '\\')
                        return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL, 0);
                    else if (::std::isalpha((unsigned char)tmp_path.begin()[0]) && tmp_path.begin()[1] == ':')
                    {
                        buffer->check_space(7);
                        ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), ::pilo::core::string::constants<char>::root_dir_sep(), ::pilo::core::string::constants<char>::root_dir_sep_length());
                        ::pilo::core::string::n_copyz(buffer->ptr(::pilo::core::string::constants<char>::root_dir_sep_length()), buffer->space_available(), tmp_path.begin(), 2);
                        buffer->set_size(6);
                        fs_type = ::pilo::core::io::path::local_fs_path;
                        return PILO_OK;
                    }
                    buffer->check_space(3);
                    for (::pilo::pathlen_t i = 0; i < tmp_path.size(); i++)
                    {
                        if (::pilo::core::string::find_char(::pilo::core::string::constants<char>::illegal_path_chars()
                            , (::pilo::i64_t) ::pilo::core::string::constants<char>::illegal_path_chars_length(), tmp_path.begin()[i]))
                            return ::pilo::make_core_error(PES_PATH_STR, PEP_INV_CHARS);

                    }
                    ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), tmp_path.begin(), 2);
                    buffer->set_size(2);
                    return PILO_OK;
                }
                else if (tmp_path.size() == 3)
                {
                    buffer->check_space(4);
                    if ((tmp_path.begin()[0]) == '\\' && (tmp_path.begin()[1]) == '\\')
                    {
                        if (::pilo::core::string::find_char(::pilo::core::string::constants<char>::illegal_path_chars()
                            , (::pilo::i64_t) ::pilo::core::string::constants<char>::illegal_path_chars_length(), tmp_path.begin()[2]) != nullptr
                            || tmp_path.begin()[2] == '\\')
                        {
                            return ::pilo::make_core_error(PES_PATH_STR, PEP_INV_CHARS);
                        }
                        ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), tmp_path.begin(), 3);
                        buffer->set_size(3);
                        fs_type = path::smb_fs_path;
                        return PILO_OK;
                    }
                    else if ((tmp_path.begin()[0]) == '\\')
                    {
                        return ::pilo::make_core_error(PES_PATH_STR, PEP_INV_CHARS);
                    }
                    else if (::std::isalpha((unsigned char)tmp_path.begin()[0]) && tmp_path.begin()[1] == ':')
                    {
                        buffer->check_space(10);
                        if (::pilo::core::string::find_char(::pilo::core::string::constants<char>::illegal_path_chars()
                            , (::pilo::i64_t) ::pilo::core::string::constants<char>::illegal_path_chars_length(), tmp_path.begin()[2]) != nullptr)
                        {
                            return ::pilo::make_core_error(PES_PATH_STR, PEP_INV_CHARS);
                        }
                        ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), ::pilo::core::string::constants<char>::root_dir_sep(), ::pilo::core::string::constants<char>::root_dir_sep_length());
                        ::pilo::core::string::n_copyz(buffer->ptr(::pilo::core::string::constants<char>::root_dir_sep_length()), buffer->space_available(), tmp_path.begin(), 2);
                        buffer->set_value(6, PMI_PATH_SEP);
                        buffer->set_value(7, tmp_path.begin()[2]);
                        buffer->set_size(8);
                        buffer->append(0);

                        fs_type = ::pilo::core::io::path::local_fs_path;
                        return PILO_OK;
                    }
                }

                fs_type = ::pilo::core::io::path::local_fs_path;
                if (isabs)
                {
                    if (tmp_path.size() == 6)
                    {
                        buffer->check_space(7);
                        ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), tmp_path.begin(), tmp_path.size());
                        buffer->set_size((::pilo::i32_t)tmp_path.size());
                        fs_type = ::pilo::core::io::path::local_fs_path;
                        return PILO_OK;
                    }
                    else if (tmp_path.size() < 6)
                    {
                        return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL);
                    }
                    else
                    {
                        if (tmp_path.at(6) != PMI_PATH_SEP)
                        {
                            ::pilo::core::string::n_movez(tmp_path.begin() + 7, tmp_path.capacity() - 7, tmp_path.begin() + 6, tmp_path.size() - 6);
                            tmp_path.begin()[6] = PMI_PATH_SEP;
                            tmp_path.add_size(1);
                        }

                        for (::pilo::pathlen_t i = 6; i < tmp_path.size(); i++)
                        {
                            if (::pilo::core::string::find_char(::pilo::core::string::constants<char>::illegal_path_chars()
                                , (::pilo::i64_t) ::pilo::core::string::constants<char>::illegal_path_chars_length(), tmp_path.at(i)) != nullptr)
                            {
                                return ::pilo::make_core_error(PES_PATH_STR, PEP_INV_CHARS);
                            }
                        }

                        ::pilo::i32_t pcnt = (::pilo::i32_t) ::pilo::core::string::cstring_ch_count(tmp_path.begin(), 5, tmp_path.size() - 5, PMI_PATH_SEP);
                        if (pcnt > 0 && ::pilo::core::string::find_substring(tmp_path.begin(), "..", tmp_path.size()))
                        {
                            ::pilo::core::memory::object_array<::pilo::cstr_ref<char>, 1> parts;
                            parts.check_space(pcnt + 1);
                            ::pilo::i64_t nparts = ::pilo::core::string::split_fixed(tmp_path.begin() + 5, tmp_path.size() - 5
                                , "\\", 1, parts.begin(), parts.capacity(), false, true, false, false);
                            if (nparts < 2)
                            {
                                PMC_ASSERT(false);
                                return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL);
                            }

                            parts.at(0).ptr = tmp_path.begin();
                            parts.at(0).length += 5;
                            std::vector<::pilo::cstr_ref<char>*> vec;
                            vec.push_back(&parts.at((int)0));
                            for (::pilo::i64_t j = 1; j < nparts; j++)
                            {
                                //printf(" -> [%s]\n", parts.at((int)j).to_string().c_str());
                                if (parts.at((int)j).length == 2 && parts.at((int)j).ptr[0] == '.' && parts.at((int)j).ptr[1] == '.')
                                {
                                    if (vec.size() > 1)
                                        vec.pop_back();
                                    else
                                    {
                                        return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL);
                                    }
                                }
                                else
                                {
                                    vec.push_back(&parts.at((int)j));
                                }

                            }
                            ::pilo::core::string::concatenate_rac(buffer, vec, "\\", 1);
                        }
                        else
                        {
                            buffer->check_space(tmp_path.size() + 1);
                            ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), tmp_path.begin(), tmp_path.size());
                            buffer->set_size(tmp_path.size());
                        }

                        return PILO_OK;
                    }
                }
                else //relative p
                {
                    for (::pilo::pathlen_t i = 0; i < tmp_path.size(); i++)
                    {
                        if (::pilo::core::string::find_char(::pilo::core::string::constants<char>::illegal_path_chars()
                            , (::pilo::i64_t) ::pilo::core::string::constants<char>::illegal_path_chars_length(), tmp_path.at(i)) != nullptr)
                        {
                            return ::pilo::make_core_error(PES_PATH_STR, PEP_INV_CHARS);
                        }
                    }

                    buffer->check_space(tmp_path.size() + 1);
                    ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), tmp_path.begin(), tmp_path.size());
                    buffer->set_size(tmp_path.size());
                }          

                return PILO_OK;
            } //end of func

#else

            ::pilo::err_t path::validate_path(::pilo::char_buffer_t *buffer, const char *path_str, ::pilo::i64_t path_str_len,
                                ::pilo::pathlen_t extra, ::pilo::i8_t &fs_type, bool &isabs,
                                predefined_pilo_dir_enum rel_to_abs_basis) {
                if (buffer == nullptr)
                    return ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL);
                fs_type = ::pilo::core::io::path::path_type_na;
                if (path_str == nullptr) return ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL);

                ::pilo::i64_t test_len = ::pilo::core::string::character_count(path_str);

                if ((test_len < 1 || path_str_len < 1 || *path_str == 0) ||
                    ((path_str_len == 1) && (path_str[0] == '/'))) {
                    buffer->check_space(extra + 1);
                    buffer->set_value(0, 0);
                    buffer->set_size(0);
                    fs_type = ::pilo::core::io::path::local_fs_path;
                    return PILO_OK;
                }

                if (path_str_len == path::unknow_length) {
                    path_str_len = test_len;
                } else if (path_str_len > (path::length_max - (::pilo::i64_t) extra) - PMI_PATH_PREF_ABS_LEN) {
                    return ::pilo::make_core_error(PES_PATH_STR, PEP_TOO_LARGE);
                }

                ::pilo::core::memory::object_array<char, 64> tmp_path;
                ::pilo::i8_t abs_type = path::absolute_type(path_str, nullptr, (::pilo::pathlen_t) path_str_len);
                if (abs_type == path::relative) {
                    if (rel_to_abs_basis != predefined_pilo_dir_enum::count) {
                        path prefix;
                        if (prefix.fill_with_predef_path(rel_to_abs_basis, 0) != PILO_OK) {
                            return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL);
                        }
                        tmp_path.check_space((::pilo::i32_t) prefix.length() + (::pilo::i32_t) path_str_len + 2);
                        ::pilo::core::string::n_copyz(tmp_path.begin(), tmp_path.capacity(), prefix.fullpath(),
                                                      prefix.length());
                        tmp_path.set_size(prefix.length());
                        if (path_str[0] != PMI_PATH_SEP) {
                            *tmp_path.ptr() = PMI_PATH_SEP;
                            tmp_path.add_size(1);
                        }
                        ::pilo::core::string::n_copyz(tmp_path.ptr(), tmp_path.space_available(), path_str,
                                                      path_str_len);
                        tmp_path.add_size((::pilo::i32_t) path_str_len);
                        isabs = true;
                    } else {
                        tmp_path.check_space((::pilo::i32_t) path_str_len + 1);
                        ::pilo::core::string::n_copyz(tmp_path.begin(), tmp_path.capacity(), path_str, path_str_len);
                        tmp_path.set_size((::pilo::i32_t) path_str_len);
                        isabs = false;
                    }
                } else {
                    tmp_path.check_space((::pilo::i32_t) path_str_len + 1);
                    ::pilo::core::string::n_copyz(tmp_path.ptr(), tmp_path.space_available(), path_str, path_str_len);
                    tmp_path.add_size((::pilo::i32_t) path_str_len);
                    isabs = true;
                }

                ::pilo::pathlen_t endidx = 0;
                ::pilo::i64_t rscnt = 0;
                ::pilo::i64_t rscnt2 = 0;
                ::pilo::core::string::rescanable_replace_inplace(tmp_path.begin() + endidx, tmp_path.size() - endidx,
                                                                 tmp_path.capacity() - endidx, "//", "/", &rscnt);
                ::pilo::core::string::rescanable_replace_inplace(tmp_path.begin() + endidx, tmp_path.size() - endidx,
                                                                 tmp_path.capacity() - endidx, "/./", "/", &rscnt2);
                if (rscnt > 0 || rscnt2 > 0)
                    tmp_path.set_size((::pilo::i32_t) ::pilo::core::string::character_count(tmp_path.begin()));
                ::pilo::i32_t len_to_dec = 0;
                for (::pilo::i32_t i = (::pilo::pathlen_t) tmp_path.size() - 1; i >= endidx; i--) {
                    if (tmp_path.begin()[i] == PMI_PATH_SEP) {
                        tmp_path.begin()[i] = 0;
                        len_to_dec--;

                    } else
                        break;
                }
                tmp_path.add_size(len_to_dec);

                if (tmp_path.size() < 1) {
                    return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL);
                }

                for (::pilo::pathlen_t i = 0; i < (::pilo::pathlen_t) tmp_path.size(); i++) {
                    if (::pilo::core::string::find_char(::pilo::core::string::constants<char>::illegal_path_chars(),
                                                        (::pilo::i64_t) ::pilo::core::string::constants<char>::illegal_path_chars_length(),
                                                        tmp_path.at(i)) != nullptr) {
                        return ::pilo::make_core_error(PES_PATH_STR, PEP_INV_CHARS);
                    }
                }

                if (isabs) {
                    ::pilo::i32_t pcnt = (::pilo::i32_t) ::pilo::core::string::cstring_ch_count(tmp_path.begin(), 0,
                                                                                                tmp_path.size(),
                                                                                                PMI_PATH_SEP);
                    if (pcnt > 0 && ::pilo::core::string::find_substring(tmp_path.begin(), "..", tmp_path.size())) {
                        ::pilo::core::memory::object_array<::pilo::cstr_ref<char>, 1> parts;
                        parts.check_space(pcnt + 1);
                        ::pilo::i64_t nparts = ::pilo::core::string::split_fixed(tmp_path.begin() + 1,
                                                                                 tmp_path.size() - 1, "/", 1,
                                                                                 parts.begin(), parts.capacity(), false,
                                                                                 true, false, false);
                        if (nparts < 2) {
                            PMC_ASSERT(false);
                            return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL);
                        }

                        parts.at(0).ptr = tmp_path.begin();
                        parts.at(0).length = 0;
                        std::vector<::pilo::cstr_ref<char> *> vec;
                        vec.push_back(&parts.at((int) 0));
                        for (::pilo::i64_t j = 1; j < nparts; j++) {
                            //printf(" -> [%s]\n", parts.at((int)j).to_string().c_str());
                            if (parts.at((int) j).length == 2 && parts.at((int) j).ptr[0] == '.' &&
                                parts.at((int) j).ptr[1] == '.') {
                                if (vec.size() > 1)
                                    vec.pop_back();
                                else {
                                    return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL);
                                }
                            } else {
                                vec.push_back(&parts.at((int) j));
                            }

                        }
                        ::pilo::core::string::concatenate_rac(buffer, vec, "/", 1);
                        buffer->check_more_space(extra + 1);
                    } else {
                        buffer->check_space(tmp_path.size() + 1 + extra);
                        ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), tmp_path.begin(),
                                                      tmp_path.size());
                        buffer->set_size(tmp_path.size());
                    }
                } else //relative path
                {
                    for (::pilo::pathlen_t i = 0; i < (::pilo::pathlen_t) tmp_path.size(); i++) {
                        if (::pilo::core::string::find_char(::pilo::core::string::constants<char>::illegal_path_chars(),
                                                            (::pilo::i64_t) ::pilo::core::string::constants<char>::illegal_path_chars_length(),
                                                            tmp_path.at(i)) != nullptr) {
                            return ::pilo::make_core_error(PES_PATH_STR, PEP_INV_CHARS);
                        }
                    }

                    buffer->check_space(tmp_path.size() + 1 + extra);
                    ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), tmp_path.begin(),
                                                  tmp_path.size());
                    buffer->set_size(tmp_path.size());
                }

                fs_type = ::pilo::core::io::path::local_fs_path;
                return PILO_OK;
            }

#endif


            pilo::err_t
            path::get_cwd(::pilo::char_buffer_t &buffer, ::pilo::i32_t endsep_mode, ::pilo::pathlen_t extra_space) {
                ::pilo::err_t err = PILO_OK;
#           ifdef  WINDOWS
                ::pilo::pathlen_t szMesured = 0;
                ::pilo::pathlen_t szFinal = 0;
                ::pilo::core::memory::object_array<wchar_t, MAX_PATH> tmp_wbuff;                
                szMesured = (::pilo::pathlen_t)GetCurrentDirectoryW(0, 0);
                ::pilo::u32_t test_size = szMesured + 5 + extra_space;
                if (test_size > path::length_max)
                {
                    return ::pilo::make_core_error(PES_BUFFER, PEP_INSUFF);
                }
                tmp_wbuff.check_space(test_size);                    

                ::pilo::core::string::n_copy(tmp_wbuff.begin(), (::pilo::i64_t)tmp_wbuff.capacity(),
                    ::pilo::core::string::constants<wchar_t>::root_dir_sep(),
                    (::pilo::i64_t)::pilo::core::string::constants<wchar_t>::root_dir_sep_length());
                tmp_wbuff.set_size((::pilo::i32_t)::pilo::core::string::constants<wchar_t>::root_dir_sep_length());
                szFinal = (::pilo::pathlen_t)GetCurrentDirectoryW((DWORD) tmp_wbuff.space_available(), tmp_wbuff.ptr());
                if (szFinal <= 0)
                {
                    return ::pilo::make_core_error(PES_GETCWD, PEP_RDFAIL);
                }
                tmp_wbuff.ref_size() += szFinal;

                if ((tmp_wbuff.at(::pilo::core::string::constants<wchar_t>::root_dir_sep_length()) == (wchar_t)'\\')
                    || (tmp_wbuff.at(::pilo::core::string::constants<wchar_t>::root_dir_sep_length() + 1) == (wchar_t)'\\'))
                {
                    ::pilo::core::string::movez(tmp_wbuff.begin(), tmp_wbuff.capacity(), tmp_wbuff.ptr((::pilo::i32_t)::pilo::core::string::constants<char>::root_dir_sep_length()));
                    tmp_wbuff.ref_size() -= (::pilo::pathlen_t)(DWORD)::pilo::core::string::constants<char>::root_dir_sep_length();
                }

                err = ::pilo::core::i18n::os_unicode_to_utf8(buffer, tmp_wbuff.begin(), tmp_wbuff.size(), (::pilo::i32_t)extra_space);
                if (err != PILO_OK)
                {
                    return err;
                }


#           else
                char *strRet = nullptr;
                if (buffer.more_space_available(extra_space + 1) > 0) {
                    strRet = getcwd(buffer.begin(), buffer.more_space_available(extra_space + 1));
                }
                if (strRet == nullptr && errno != ERANGE) {
                    return ::pilo::make_core_error(PES_GETCWD, PEP_RDFAIL);
                }
                ::pilo::i32_t ret_size = (::pilo::i32_t) ::pilo::core::string::character_count(strRet);
                if (strRet != nullptr) {
                    buffer.add_size(ret_size);
                } else {
                    strRet = getcwd(nullptr, 0);
                    if (strRet == nullptr)
                        return ::pilo::make_core_error(PES_GETCWD, PEP_RDFAIL);
                    buffer.check_more_space(extra_space + 1 + ret_size);
                    ::pilo::core::string::n_copyz(buffer.ptr(), buffer.space_available(), strRet, ret_size);
                }

#           endif
                if (endsep_mode == path::sepmode_needsep) {
                    err = path::string_append_endsep_inplace(buffer);
                } else if (endsep_mode == path::sepmode_nosep) {
                    err = path::string_remove_endsep_inplace(buffer);
                }

                return err;
            }

#           ifdef  WINDOWS
            static ::pilo::err_t _s_make_local_fs_link_win32(const char * src, const wchar_t * dst)
            {
                PMC_UNUSED(src);
                HRESULT hr = CoInitialize(NULL);
                if (SUCCEEDED(hr))
                {
                    IShellLink* pisl;
                    hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&pisl);
                    if (SUCCEEDED(hr))
                    {
                        IPersistFile* pIPF;
                        pisl->SetPath(src);
                        pisl->SetDescription("");
                        hr = pisl->QueryInterface(IID_IPersistFile, (void**)&pIPF);
                        if (SUCCEEDED(hr))
                        {
                            hr = pIPF->Save(dst, TRUE);
                            pIPF->Release();
                            if (hr == S_OK)
                            {
                                return PILO_OK;
                            }
                            
                        }
                        pisl->Release();
                    }
                    CoUninitialize();
                }
                return ::pilo::make_core_error(PES_FILE, PEP_FS_LINK_FAILED);
            }


            static ::pilo::err_t GetLnkFileName(OUT wchar_buffer_t & dst, IN wchar_buffer_t & src)
            {
                CoInitialize(0);
                IShellLinkW* shlink = 0;
                IPersistFile* persist = 0;
                WIN32_FIND_DATAW wfd = { 0 };

                dst.check_space(MAX_PATH);

                HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkW, (void**)&shlink);
                if (SUCCEEDED(hr)) {
                    hr = shlink->QueryInterface(IID_IPersistFile, (void**)&persist);
                    if (SUCCEEDED(hr)) {
                        hr = persist->Load(src.begin(), STGM_READ);
                        if (SUCCEEDED(hr)) {
                            // shlink->Resolve(0, SLR_ANY_MATCH | SLR_NO_UI);           
                            hr = shlink->GetPath(dst.begin(), MAX_PATH, &wfd, SLGP_RAWPATH);
                            dst.set_size((::pilo::i32_t) ::pilo::core::string::character_count(dst.begin()));
                        }
                        persist->Release();
                    }
                    shlink->Release();
                }
                CoUninitialize();
                return PILO_OK;
            }

            ::pilo::err_t path::get_path_node_type(const char* path_cstr, ::pilo::pathlen_t path_len, ::pilo::i8_t path_type_hint
                , ::pilo::i8_t& node_type, ::pilo::i8_t* target_node_type, ::pilo::char_buffer_t* buffer)
            {                
                if (path_cstr == nullptr || *path_cstr == 0)
                {
                    node_type = path::node_type_na;
                    return ::pilo::make_core_error(PES_PARAM, PEP_IS_INVALID);
                }
                if (path_len < 0)
                {
                    ::pilo::i64_t tmplen = ::pilo::core::string::character_count(path_cstr);
                    if (tmplen >= path::length_max)
                    {
                        return ::pilo::make_core_error(PES_PARAM, PEP_ARR_IDX_OOB);
                    }
                    path_len = (::pilo::pathlen_t)tmplen;
                }

                ::pilo::err_t err = PILO_OK;
                if (path_type_hint == path::local_fs_path)
                {
                    wchar_t warr[PMI_STCPARAM_PATH_DEFAULT_LENGTH] = { 0 };
                    ::pilo::wchar_buffer_t wbuf(warr, PMI_STCPARAM_PATH_DEFAULT_LENGTH);
                    err = ::pilo::core::i18n::utf8_to_os_unicode(wbuf, path_cstr, path_len, 0);
                    if (err != PILO_OK)
                    {
                        node_type = path::node_type_na;
                        return err;
                    }

                    DWORD attributes = GetFileAttributesW(wbuf.begin());
                    if (attributes == INVALID_FILE_ATTRIBUTES) 
                    {
                        DWORD e = ::GetLastError();
                        if (ERROR_PATH_NOT_FOUND == e)
                        {
                            node_type = path::node_type_na;
                            return ::pilo::make_core_error(PES_FILE, PEP_NOT_EXIST);
                        }

                        node_type = path::node_type_na;
                        return ::pilo::make_core_error(PES_FILE_ATTR, PEP_RDFAIL);
                    }

                    if (attributes & FILE_ATTRIBUTE_DIRECTORY) 
                    {
                        node_type = path::fs_node_type_dir;
                    }
                    else
                    {
                        SHFILEINFOW shFileInfo = { 0 };
                        SHGetFileInfoW(wbuf.begin()+4, 0, &shFileInfo, sizeof(SHFILEINFOW),
                            SHGFI_DISPLAYNAME | SHGFI_ICON | SHGFI_SMALLICON | SHGFI_TYPENAME | SHGFI_ATTRIBUTES);
                        if (shFileInfo.dwAttributes & SFGAO_LINK)
                        {
                            node_type = path::fs_node_type_lnk;
                            if (buffer != nullptr)
                            {
                                wchar_t wdst_arr[MAX_PATH + 1] = { 0 };
                                ::pilo::wchar_buffer_t wdst(wdst_arr, MAX_PATH+1);

                                err = GetLnkFileName(wdst, wbuf); //XXXXXXX
                                if (err != PILO_OK)
                                {
                                    node_type = path::node_type_na;
                                    return err;
                                }

                                if (::pilo::core::i18n::os_unicode_to_utf8(*buffer, wdst.begin(), wdst.size(), 0) != PILO_OK)
                                    return err;

                                if (target_node_type != nullptr)
                                {
                                    attributes = GetFileAttributesW(wdst.begin());
                                    if (attributes == INVALID_FILE_ATTRIBUTES) {
                                        ::pilo::set_if_ptr_is_not_null<::pilo::i8_t>(target_node_type, (::pilo::i8_t)path::node_type_na);
                                        return ::pilo::make_core_error(PES_FILE_ATTR, PEP_RDFAIL);
                                    }

                                    if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
                                        ::pilo::set_if_ptr_is_not_null<::pilo::i8_t>(target_node_type, (::pilo::i8_t)path::fs_node_type_dir);
                                    }
                                    else
                                    {
                                        ::pilo::set_if_ptr_is_not_null<::pilo::i8_t>(target_node_type, (::pilo::i8_t)path::fs_node_type_file);
                                    }
                                }                                
                            }
                        }
                        else
                        {
                            node_type = path::fs_node_type_file;
                        }
                    }
                }
                else
                {
                    err = ::pilo::make_core_error(PES_OP, PEP_UNSUPPORT);
                }

                return err;
            }



#else


            static ::pilo::err_t _s_read_link_posix(::pilo::char_buffer_t *dst, const char *src) {
                ::pilo::i32_t avail = dst->space_available();
                ssize_t tmp_ret = readlink(src, dst->ptr(), avail);
                if (tmp_ret < 0) {
                    return ::pilo::make_core_error(PES_SYMLINK, PEP_RDFAIL);
                }
                if ((::pilo::i32_t) tmp_ret < avail) {
                    dst->add_size(tmp_ret);
                    return PILO_OK;
                }

                do {
                    dst->check_more_space(64);
                    avail = dst->space_available();
                    tmp_ret = readlink(src, dst->ptr(), avail);
                    if (tmp_ret < 0) {
                        return ::pilo::make_core_error(PES_SYMLINK, PEP_RDFAIL);
                    }
                    if ((::pilo::i32_t) tmp_ret < avail) {
                        dst->add_size(tmp_ret);
                        return PILO_OK;
                    }

                } while (dst->space_available() <= PMI_PATH_MAX);

                return ::pilo::make_core_error(PES_SYMLINK, PEP_TOO_LARGE);;
            }

            static ::pilo::err_t
            _s_read_link_recursively_posix(::pilo::i8_t &node_type, ::pilo::char_buffer_t *dst, const char *src) {
                char sbb[PMI_STCPARAM_PATH_DEFAULT_LENGTH] = {0};
                ::pilo::char_buffer_t sbuf(sbb, sizeof(sbb), 0, false);
                char dbb[PMI_STCPARAM_PATH_DEFAULT_LENGTH] = {0};
                ::pilo::char_buffer_t dbuf(dbb, sizeof(dbb), 0, false);
                ::pilo::err_t err = PILO_OK;

                ::pilo::pathlen_t src_len = (::pilo::pathlen_t) ::pilo::core::string::character_count(src);
                sbuf.check_space(src_len + 1);
                ::pilo::core::string::n_copyz(sbuf.begin(), sbuf.capacity(), src, src_len);
                sbuf.set_size(src_len);

                while (true) {
                    err = _s_read_link_posix(&dbuf, sbuf.begin());
                    if (err != PILO_OK)
                        return err;

                    struct stat stBuff = {0};
                    if (stat(dbuf.begin(), &stBuff) != 0) {
                        node_type = path::node_type_na;
                        if (errno == ENOENT) {
                            return ::pilo::make_core_error(PES_FILE, PEP_NOT_EXIST);
                        }
                        return ::pilo::make_core_error(PES_FILE_ATTR, PEP_RDFAIL);
                    }
                    if ((stBuff.st_mode & S_IFDIR) == S_IFDIR) {
                        node_type = path::fs_node_type_dir;
                        break;
                    } else if (S_ISLNK(stBuff.st_mode)) {
                        sbuf.check_space(dbuf.size() + 1);
                        ::pilo::core::string::n_copyz(sbuf.begin(), sbuf.capacity(), dbuf.begin(), dbuf.size());
                        sbuf.set_size(dbuf.size());
                    } else if ((stBuff.st_mode & S_IFREG) == S_IFREG) {
                        node_type = path::fs_node_type_file;
                        break;
                    } else {
                        node_type = path::fs_node_type_other;
                        break;
                    }
                }

                dst->check_more_space(dbuf.size() + 1);
                ::pilo::core::string::n_copyz(dst->ptr(), dst->space_available(), dbuf.begin(), dbuf.size());
                dst->add_size(dbuf.size());

                return PILO_OK;
            }

            ::pilo::err_t
            path::get_path_node_type(const char *p, ::pilo::pathlen_t path_len, ::pilo::i8_t path_type_hint,
                                     ::pilo::i8_t &node_type, ::pilo::i8_t *target_node_type,
                                     ::pilo::char_buffer_t *buffer) {
                if (p == nullptr || *p == 0) {
                    node_type = path::node_type_na;
                    return ::pilo::make_core_error(PES_PARAM, PEP_IS_INVALID);
                }
                if (path_len < 0) {
                    ::pilo::i64_t tmplen = ::pilo::core::string::character_count(p);
                    if (tmplen >= path::length_max) {
                        return ::pilo::make_core_error(PES_PARAM, PEP_ARR_IDX_OOB);
                    }
                    path_len = (::pilo::pathlen_t) tmplen;
                }
                ::pilo::err_t err = PILO_OK;
                struct stat stBuff = {0};
                if (path_type_hint == path::local_fs_path) {
                    if (::stat(p, &stBuff) != 0) {
                        node_type = path::node_type_na;
                        if (errno == ENOENT) {
                            return ::pilo::make_core_error(PES_FILE, PEP_NOT_EXIST);
                        }
                        return ::pilo::make_core_error(PES_FILE_ATTR, PEP_RDFAIL);
                    }

                    if ((stBuff.st_mode & S_IFDIR) == S_IFDIR)
                        node_type = path::fs_node_type_dir;
                    else if (S_ISLNK(stBuff.st_mode)) {
                        node_type = path::fs_node_type_lnk;
                        if (buffer != nullptr) {
                            ::pilo::i8_t tgt_node_type = path::node_type_na;
                            err = _s_read_link_recursively_posix(tgt_node_type, buffer, p);
                            if (err != PILO_OK)
                                return err;
                            ::pilo::set_if_ptr_is_not_null<::pilo::i8_t>(target_node_type, tgt_node_type);
                        }

                    } else if ((stBuff.st_mode & S_IFREG) == S_IFREG)
                        node_type = path::fs_node_type_file;
                    else
                        node_type = path::fs_node_type_other;
                } else {
                    err = ::pilo::make_core_error(PES_OP, PEP_UNSUPPORT);
                }

                return err;
            }

#endif


            ::pilo::err_t path::make_dir(const char *dirpath, ::pilo::pathlen_t path_len, bool) {
#ifdef WINDOWS
                wchar_t wb[PMI_STCPARAM_PATH_DEFAULT_LENGTH] = { 0 };
                ::pilo::wchar_buffer_t buffer(wb, PMI_STCPARAM_PATH_DEFAULT_LENGTH, 0, false);
                ::pilo::err_t err = ::pilo::core::i18n::utf8_to_os_unicode(buffer, dirpath, path_len);
                if (err != PILO_OK)
                    return err;
                BOOL result = CreateDirectoryW(buffer.begin(), NULL);
                if (! result) 
                {
                    ::pilo::i8_t node_type = path::node_type_na;

                    std::string dstr(dirpath, path_len);
                    err = path::get_path_node_type(dirpath, path_len, path::local_fs_path, node_type, nullptr, nullptr);
                    if (err != PILO_OK)
                        return err;
                    if (node_type == path::fs_node_type_dir)
                    {
                        return PILO_OK;
                    }
                    else
                    {
                        return pilo::make_core_error(PES_PATH_STR, PEP_IS_INVALID);
                    }
                }
#else
                int result = 0;
                if (path_len == ::pilo::core::io::path::unknow_length)
                {
                    result = mkdir(dirpath, 0755);
                }
                else
                {
                    std::string d(dirpath, (size_t) path_len);
                    result = mkdir(d.c_str(), 0755);
                }
                if (result != 0) {
                    ::pilo::i8_t node_type = path::node_type_na;
                    ::pilo::err_t err = path::get_path_node_type(dirpath, path_len, path::local_fs_path, node_type,
                                                                 nullptr, nullptr);
                    if (err != PILO_OK)
                        return err;
                    if (node_type == path::fs_node_type_dir) {
                        return PILO_OK;
                    } else {
                        return pilo::make_core_error(PES_PATH_STR, PEP_IS_INVALID);
                    }
                }
#endif
                return PILO_OK;
            }

            ::pilo::err_t path::touch_file(const char *filepath, ::pilo::pathlen_t path_len, bool delete_exist) {
                const char *p = filepath;
                ::pilo::core::memory::object_array<char, PMI_STCPARAM_PATH_DEFAULT_LENGTH> src;
                if (path_len != path::unknow_length) {
                    src.check_space(path_len + 1);
                    ::pilo::core::string::n_copyz(src.begin(), src.capacity(), filepath, path_len);
                    p = src.begin();
                }

#ifdef WINDOWS
                wchar_t wb[PMI_STCPARAM_PATH_DEFAULT_LENGTH] = { 0 };
                ::pilo::wchar_buffer_t buffer(wb, PMI_STCPARAM_PATH_DEFAULT_LENGTH, 0, false);
                ::pilo::err_t err = ::pilo::core::i18n::utf8_to_os_unicode(buffer, filepath, path_len);
                if (err != PILO_OK)
                    return err;
                HANDLE fh = CreateFileW(buffer.begin(),
                    GENERIC_WRITE,
                    FILE_SHARE_WRITE, 
                    NULL, 
                    CREATE_NEW, 
                    FILE_ATTRIBUTE_NORMAL,
                    NULL);
                if (fh != INVALID_HANDLE_VALUE) {
                    CloseHandle(fh);

                } else {
                    if (!delete_exist) {
                        if (errno == EEXIST) {
                            return ::pilo::make_core_error(PES_FILE, PEP_EXIST);
                        } else {
                            return ::pilo::make_core_error(PES_FILE, PEP_CREATE_FAILED);
                        }
                    } else {
                        err = remove_fs_node(path::node_type_na, filepath, path_len, true);
                        if (err != PILO_OK)
                            return err;
                        fh = CreateFileW(buffer.begin(),
                            GENERIC_WRITE,
                            FILE_SHARE_WRITE,
                            NULL,
                            CREATE_NEW,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);
                        if (fh != INVALID_HANDLE_VALUE) {
                            CloseHandle(fh);
                        } else {
                            return ::pilo::make_core_error(PES_FILE, PEP_CREATE_FAILED);
                        }
                    }
                }
#else
                int fd = 0;
                if (path_len == ::pilo::core::io::path::unknow_length)
                {
                    fd = open(filepath, O_CREAT | O_EXCL |O_RDWR, 0644);
                }
                else
                {
                    std::string d(filepath, (size_t) path_len);
                    fd = open(d.c_str(), O_CREAT | O_EXCL| O_RDWR, 0644);
                }
                if (fd != -1) {
                    close(fd);
                } else {
                    if (! delete_exist) {
                        if (errno == EEXIST) {
                            return ::pilo::make_core_error(PES_FILE, PEP_EXIST);
                        } else {
                            return ::pilo::make_core_error(PES_FILE, PEP_CREATE_FAILED);
                        }
                    } else {
                        ::pilo::err_t  err = remove_fs_node(path::node_type_na, filepath, path_len, true);
                        if (err != PILO_OK)
                            return err;
                        if (path_len == ::pilo::core::io::path::unknow_length)
                        {
                            fd = open(filepath, O_CREAT | O_EXCL |O_RDWR, 0644);
                        }
                        else
                        {
                            std::string d(filepath, (size_t) path_len);
                            fd = open(d.c_str(), O_CREAT | O_EXCL| O_RDWR, 0644);
                        }
                        if (fd != -1) {
                            close(fd);
                        } else {
                            return ::pilo::make_core_error(PES_FILE, PEP_CREATE_FAILED);
                        }
                    }
                }

#endif
                return PILO_OK;
            }

            ::pilo::err_t path::remove_fs_node(::pilo::i8_t fs_node_type, const char *pth, ::pilo::pathlen_t path_len) 
            {
                ::pilo::err_t err = PILO_OK;
                if (fs_node_type == path::path_type_na) {
                    err = path::get_path_node_type(pth, path_len, path::local_fs_path, fs_node_type, nullptr, nullptr);
                    if (err != PILO_OK) {
                        if (::pilo::is_err_sub_and_type(err, PES_FILE, PEP_NOT_EXIST))
                            return PILO_OK;
                        return err;
                    }

                    if (fs_node_type == path::path_type_na) {
                        return PILO_OK;
                    }
                }
                if (fs_node_type == path::fs_node_type_dir) {
                    return path::remove_dir(pth, path_len);
                } else if (fs_node_type == path::fs_node_type_file || fs_node_type == path::fs_node_type_lnk) {
                    return path::remove_file(pth, path_len);
                } else {
                    return ::pilo::make_core_error(PES_FILE, PEP_TYPE_MISMATCH);
                }
            }

            ::pilo::err_t path::remove_fs_node(::pilo::i8_t fs_node_type, const path *p) {
                return remove_fs_node(fs_node_type, p->fullpath(), p->length());
            }

            ::pilo::err_t
            path::remove_fs_node(::pilo::i8_t fs_node_type, const char *path_str, ::pilo::pathlen_t path_len,bool follow_link) 
            {
                if (follow_link)
                    return remove_fs_node_follow_link(fs_node_type, path_str, path_len);
                else
                    return remove_fs_node(fs_node_type, path_str, path_len);
            }

            ::pilo::err_t
            path::remove_fs_node(::pilo::i8_t fs_node_type, const path *p,  bool follow_link) {
                return remove_fs_node(fs_node_type, p->fullpath(), p->length(), follow_link);
            }

            ::pilo::err_t
            path::remove_fs_node_follow_link(::pilo::i8_t fs_node_type, const char *pth, ::pilo::pathlen_t path_len) 
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::i8_t target_node_type = path::node_type_na;
                char target_path_buffer[PMI_STCPARAM_PATH_DEFAULT_LENGTH] = {0};
                ::pilo::char_buffer_t target_path(target_path_buffer, sizeof(target_path_buffer), 0, false);
                bool can_follow = false;
                if (fs_node_type == path::path_type_na || fs_node_type == path::fs_node_type_lnk) {
                    err = path::get_path_node_type(pth, path_len, path::local_fs_path, fs_node_type, &target_node_type,
                                                   &target_path);
                    if (err != PILO_OK)
                        return err;
                    if (target_node_type == path::fs_node_type_dir || target_node_type == path::fs_node_type_file) {
                        can_follow = true;
                    }
                }

                if (fs_node_type == path::fs_node_type_dir) {
                    return path::remove_dir(pth, path_len);
                } else if (fs_node_type == path::fs_node_type_file) {
                    return path::remove_file(pth, path_len);
                } else if (fs_node_type == path::fs_node_type_lnk) {//could be very danger, inf loop
                    return remove_fs_node_follow_link(path::fs_node_type_lnk, target_path.begin(),
                                                      (::pilo::pathlen_t) target_path.size());
                } else {
                    return ::pilo::make_core_error(PES_FILE, PEP_TYPE_MISMATCH);
                }

            }

            const path *path::_parse_path(const path *src, path *tmp, ::pilo::i8_t &target_type) {
                ::pilo::i8_t node_type = path::node_type_na;
                path target;
                node_type = src->get_fs_info(&target_type, &target);
                if (node_type == path::node_type_na) {
                    target_type = path::node_type_na;
                    return nullptr;
                }
                if (node_type != path::fs_node_type_lnk) {
                    target_type = node_type;
                    return src;
                }

                if (target_type == path::fs_node_type_lnk) {
                    PMC_ASSERT(false);
                    return nullptr;
                } else if (target_type == path::node_type_na) {
                    PMC_ASSERT(false);
                    return nullptr;
                }
                return tmp;
            }

            const path *path::parse_path(const path *src, path *tmp, ::pilo::i8_t &target_type) {
                if (tmp == nullptr || tmp->invalid() || src == nullptr || src->invalid()) {
                    return nullptr;
                }
                return _parse_path(src, tmp, target_type);
            }

            ::pilo::err_t path::remove_dir(const char *dirpath, ::pilo::pathlen_t path_len) {
#ifdef WINDOWS
                wchar_t wb[PMI_STCPARAM_PATH_DEFAULT_LENGTH] = { 0 };
                ::pilo::wchar_buffer_t buffer(wb, PMI_STCPARAM_PATH_DEFAULT_LENGTH, 0, false);
                ::pilo::err_t err = ::pilo::core::i18n::utf8_to_os_unicode(buffer, dirpath, path_len);
                if (err != PILO_OK)
                    return err;
                BOOL bRet = ::RemoveDirectoryW(buffer.begin());
                if (bRet != TRUE) //2=ne 145=nem
                {
                    DWORD ret = ::GetLastError();
                    if (ret == ERROR_FILE_NOT_FOUND)
                    {
                        return PILO_OK;
                    }
                    else if (ret == ERROR_DIR_NOT_EMPTY)
                    {
                        return ::pilo::make_core_error(PES_DIR, PEP_DEL_FAILED);
                    }
                    return ::pilo::make_core_error(PES_DIR, PEP_DEL_FAILED);
                }
#else
                const char *p = dirpath;
                ::pilo::core::memory::object_array<char, PMI_STCPARAM_PATH_DEFAULT_LENGTH> src;
                if (path_len != path::unknow_length) {
                    src.check_space(path_len + 1);
                    ::pilo::core::string::n_copyz(src.begin(), src.capacity(), dirpath, path_len);
                    p = src.begin();
                }
                if (::rmdir(p) != 0) {
                    if (ENOENT == errno) {
                        return PILO_OK;
                    }
                    if (EEXIST == errno && ENOTEMPTY == errno) {
                        return ::pilo::make_core_error(PES_DIR, PEP_EXIST);
                    }
                    return ::pilo::make_core_error(PES_DIR, PEP_DEL_FAILED);
                }
#endif
                return PILO_OK;
            }

            ::pilo::err_t
            path::remove_dir_recursively(const char *dirpath, ::pilo::pathlen_t path_len, bool content_only,
                                         bool follow_link) {
                path p(dirpath, path_len);
                if (p.invalid() || p.is_root()) {
                    return ::pilo::make_core_error(PES_PARAM, PEP_IS_INVALID);
                }

                ::pilo::u32_t flags = path::evt_node_visiting | path::evt_dir_leaving;
                if (content_only)
                    flags |= path::flags_supress_final_dir_leaving;
                if (follow_link)
                    flags |= path::flags_follow_link;

                pilo::err_t err = path::dfs_travel_path(&p, path::s_node_deletion_handler, nullptr, false, flags);
                if (::pilo::is_err_sub_and_type(err, PES_FILE, PEP_NOT_EXIST)) {
                    return PILO_OK;
                }

                return err;
            }

            ::pilo::err_t path::remove_file(const char *pth, ::pilo::pathlen_t path_len) {
#ifdef WINDOWS
                wchar_t wb[PMI_STCPARAM_PATH_DEFAULT_LENGTH] = { 0 };
                ::pilo::wchar_buffer_t buffer(wb, PMI_STCPARAM_PATH_DEFAULT_LENGTH, 0, false);
                ::pilo::err_t err = ::pilo::core::i18n::utf8_to_os_unicode(buffer, pth, path_len);
                if (err != PILO_OK)
                    return err;
                BOOL r = ::DeleteFileW(buffer.begin());
                if (r == TRUE)
                {
                    return PILO_OK;
                }
#else
                const char *p = pth;
                ::pilo::core::memory::object_array<char, PMI_STCPARAM_PATH_DEFAULT_LENGTH> src;
                if (path_len != path::unknow_length) {
                    src.check_space(path_len + 1);
                    ::pilo::core::string::n_copyz(src.begin(), src.capacity(), pth, path_len);
                    p = src.begin();
                }
                int ri = ::unlink(p);
                if (ri == 0) {
                    return PILO_OK;
                }
#endif
                return ::pilo::make_core_error(PES_FILE, PEP_DEL_FAILED);
            }

            ::pilo::err_t path::get_executable_path(::pilo::core::io::path *p, ::pilo::pathlen_t extra) {

#ifdef WINDOWS
                WCHAR tp[PMI_PATH_MAX + 1] = { 0 };
                DWORD need_capa = GetModuleFileNameW(NULL, tp, sizeof(tp));
                if (need_capa == 0)
                {
                    return ::pilo::make_core_error(PES_PATH_STR, PEP_IS_NULL);
                }

                char buf[PMI_PATH_MAX] = { 0 };
                ::pilo::char_buffer_t buffer(buf,sizeof(buf), 0, false);                
                ::pilo::core::i18n::os_unicode_to_utf8(buffer, tp, need_capa, 0);
                return p->set(buf, path::unknow_length, extra);

#else
                char tp[PMI_PATH_MAX] = {0};
                ssize_t len = readlink("/proc/self/exe", tp, sizeof(tp) - 1);
                if (len < 0 || (size_t) len >= sizeof(tp)) {
                    return ::pilo::make_core_error(PES_PATH_STR, PEP_IS_NULL);
                }
                tp[len] = '\0';
                return p->set(tp, path::unknow_length, extra);
#endif

            }

            ::pilo::err_t
            path::_dfs_travel_path(::pilo::i32_t l_idx, const path *p, traval_fs_node_func_t handler, void *ctx,
                                   bool ignore_err, ::pilo::u32_t flags) {
                ::pilo::err_t err = PILO_OK;
                ::pilo::i8_t target_node_type = path::node_type_na;
                path tmp_target;
                ::pilo::i8_t node_type = p->get_fs_info(&target_node_type, &tmp_target);
                if (node_type == path::node_type_na) {
                    return ::pilo::make_core_error(PES_FILE, PEP_NOT_EXIST);
                }
                ::pilo::i32_t f_idx = 0;
                if (node_type == path::fs_node_type_file || node_type == path::fs_node_type_other) {
                    if (::pilo::bit_flag<::pilo::u32_t>::test_value(flags, path::evt_node_visiting))
                        return handler(path::evt_node_visiting, p, node_type, l_idx, f_idx, ctx);
                    return PILO_OK;
                } else if (node_type == path::fs_node_type_lnk) {

                    if (::pilo::bit_flag<::pilo::u32_t>::test_value(flags, path::evt_node_visiting))
                        return handler(path::evt_node_visiting, p, node_type, l_idx, f_idx, ctx);
                    if (target_node_type == path::node_type_na) {
                        return PILO_OK;
                    }
                    if (::pilo::bit_flag<::pilo::u32_t>::test_value(flags, path::flags_follow_link)) {
                        p = &tmp_target;
                        if (target_node_type == path::fs_node_type_file ||
                            target_node_type == path::fs_node_type_other) {
                            if (::pilo::bit_flag<::pilo::u32_t>::test_value(flags, path::evt_node_visiting))
                                return handler(path::evt_node_visiting, p, node_type, l_idx, f_idx, ctx);
                            return PILO_OK;
                        } else if (target_node_type == path::fs_node_type_lnk) {
                            PMC_ASSERT(false);
                            return pilo::make_core_error(PES_PATH_STR, PEP_IS_INVALID);
                        }
                        node_type = path::fs_node_type_dir;
                    } else
                        return PILO_OK;
                }

                if (::pilo::bit_flag<::pilo::u32_t>::test_value(flags, path::evt_dir_entered)) {
                    err = handler(path::evt_dir_entered, p, path::fs_node_type_dir, l_idx, -1, ctx);
                    if (err != PILO_OK) {
                        if (!ignore_err)
                            return err;
                    }
                }

#ifdef WINDOWS
                wchar_t wb[PMI_STCPARAM_PATH_DEFAULT_LENGTH] = { 0 };
                ::pilo::wchar_buffer_t wbuffer(wb, PMI_STCPARAM_PATH_DEFAULT_LENGTH, 0, false);
                err = ::pilo::core::i18n::utf8_to_os_unicode(wbuffer, p->fullpath(), (::pilo::i32_t)p->length(), 5);
                if (err != PILO_OK)
                    return err;
                ::pilo::i32_t base_len = (::pilo::i32_t)wbuffer.size() + 1;
                ::pilo::core::string::n_concatenate_inplace(wbuffer.begin(), wbuffer.size(), L"\\*.*", 4);
                wbuffer.set_size(base_len);
                ::pilo::core::pattern::resource_cleaner<1> rcc;    

                WIN32_FIND_DATAW  findData;
                HANDLE  handle = FindFirstFileW(wbuffer.begin(), &findData);
                if (handle == INVALID_HANDLE_VALUE)
                {
                    return ::pilo::make_core_error(PES_DIR, PEP_RDFAIL);
                }
                rcc.push(
                    [](void* ptr, void* ) {
                        HANDLE h = (HANDLE)ptr;
                        ::FindClose(h);
                        return PILO_OK;
                    }
                    , (void*) handle
                    , nullptr
                );
                do
                {
                    if (::pilo::core::string::strict_compare(findData.cFileName, 0, L".", 0, -1) == 0
                        || ::pilo::core::string::strict_compare(findData.cFileName, 0, L"..", 0, -1) == 0)
                        continue;

                    ::pilo::pathlen_t fn_len = (::pilo::pathlen_t) ::pilo::core::string::character_count(findData.cFileName);
                    wbuffer.check_more_space(fn_len + 1);
                    ::pilo::core::string::copyz(wbuffer.ptr(base_len), wbuffer.space_available(), findData.cFileName);
                    ::pilo::i64_t sublen = wbuffer.size() + fn_len;
                    char dir_bb[PMI_STCPARAM_PATH_DEFAULT_LENGTH] = { 0 };
                    ::pilo::char_buffer_t dir_buf(dir_bb, sizeof(dir_bb), 0, false);
                    err = ::pilo::core::i18n::os_unicode_to_utf8(dir_buf, wbuffer.begin(), (::pilo::i32_t)sublen);
                    if (err != PILO_OK)
                    {
                        return err;
                    }
                    path sub_p(dir_buf.begin(), dir_buf.size());
                    if (sub_p.invalid())
                    {
                        return ::pilo::make_core_error(PES_PATH_STR, PEP_IS_INVALID);
                    }
                    if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    {
                        err = _dfs_travel_path(l_idx + 1, &sub_p, handler, ctx, ignore_err, flags);
                        if (err != PILO_OK)
                        {
                            return err;
                        }
                    }
                    else
                    {
                        ::pilo::i8_t sub_target_type = path::node_type_na;
                        path sub_tmp_target;
                        ::pilo::i8_t nt = path::node_type_na;
                        if (::pilo::bit_flag<::pilo::u32_t>::test_value(flags, path::flags_follow_link))
                        {
                            nt = sub_p.get_fs_info(&sub_target_type, &sub_tmp_target);
                        }

                        if (::pilo::bit_flag<::pilo::u32_t>::test_value(flags, path::evt_node_visiting))
                        {
                            if (findData.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE
                                && ::pilo::bit_flag<::pilo::u32_t>::test_value(flags, path::evt_node_visiting))
                            {
                                err = handler(path::evt_node_visiting, &sub_p, path::fs_node_type_other, l_idx, f_idx++, ctx);
                            }
                            else
                            {
                                err = handler(path::evt_node_visiting, &sub_p, path::fs_node_type_file, l_idx, f_idx++, ctx);
                            }
                            if (err != PILO_OK)
                            {
                                return err;
                            }
                        }

                        if (nt == path::fs_node_type_lnk && ::pilo::bit_flag<::pilo::u32_t>::test_value(flags, path::flags_follow_link) && ! sub_tmp_target.invalid() && ! sub_tmp_target.is_root())
                        {                           
                            
                            if (sub_target_type == path::fs_node_type_dir)
                            {
                                err = _dfs_travel_path(l_idx + 1, &sub_tmp_target, handler, ctx, ignore_err, flags);
                                if (err != PILO_OK)
                                {
                                    return err;
                                }
                            }
                            else
                            {
                                if (findData.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE)
                                {
                                    err = handler(path::evt_node_visiting, &sub_tmp_target, path::fs_node_type_other, l_idx, f_idx++, ctx);
                                }
                                else
                                {
                                    err = handler(path::evt_node_visiting, &sub_tmp_target, path::fs_node_type_file, l_idx, f_idx++, ctx);
                                }
                                if (err != PILO_OK)
                                {
                                    return err;
                                }
                            }                            
                        }                      
                    }
                } while (FindNextFileW(handle, &findData) != FALSE); //find next


#else
                DIR *pDir = nullptr;
                struct dirent *dir_result_ptr = nullptr;
                if ((pDir = ::opendir(p->fullpath())) == nullptr) {
                    return ::pilo::make_core_error(PES_DIR, PEP_OPEN_FAILED);
                }
                while ((dir_result_ptr = ::readdir(pDir)) != NULL) {
                    if (::strcmp(dir_result_ptr->d_name, ".") == 0 ||
                        ::strcmp(dir_result_ptr->d_name, "..") == 0) {
                        continue;
                    }


                } // endof while


#endif
                if (::pilo::bit_flag<::pilo::u32_t>::test_value(flags, path::evt_dir_leaving)) {
                    if (l_idx > 0
                        || !::pilo::bit_flag<::pilo::u32_t>::test_value(flags, path::flags_supress_final_dir_leaving)) {
                        err = handler(path::evt_dir_leaving, p, path::fs_node_type_dir, l_idx, -1, ctx);
                        if (err != PILO_OK) {
                            if (!ignore_err) {
                                return err;
                            }

                        }
                    }
                }
                return PILO_OK;
            }

            ::pilo::err_t
            path::dfs_travel_path(const path *p, traval_fs_node_func_t handler, void *ctx, bool ignore_err,
                                  ::pilo::u32_t flags) {
                if (p == nullptr || handler == nullptr)
                    return ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL);
                if (p->invalid())
                    return ::pilo::make_core_error(PES_PARAM, PEP_IS_INVALID);

                return _dfs_travel_path(0, p, handler, ctx, ignore_err, flags);
            }

            ::pilo::err_t path::create_local_fs_link(const path *src, const path *target, bool is_force) {
                if (src == nullptr || target == nullptr) {
                    return ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL);
                }
                if (src->invalid() || target->invalid()) {
                    return ::pilo::make_core_error(PES_PARAM, PEP_IS_INVALID);
                }

                ::pilo::i8_t nt = target->get_fs_info(nullptr, nullptr);
                if (nt != path::node_type_na) {
                    if (!is_force)
                        return ::pilo::make_core_error(PES_FILE, PEP_EXIST);
                    ::pilo::err_t err = target->remove(false, false);
                    if (err != PILO_OK)
                        return err;
                }
#ifdef  WINDOWS
                char src_bb[PMI_STCPARAM_PATH_DEFAULT_LENGTH] = { 0 };
                wchar_t dst_bb[PMI_STCPARAM_PATH_DEFAULT_LENGTH] = { 0 };
                ::pilo::char_buffer_t src_buf(src_bb, PMI_STCPARAM_PATH_DEFAULT_LENGTH, 0, false);
                ::pilo::wchar_buffer_t dst_buf(dst_bb, PMI_STCPARAM_PATH_DEFAULT_LENGTH, 0, false);
                ::pilo::core::i18n::utf8_to_ansi(src_buf, src->fullpath(), src->length());
                ::pilo::core::i18n::utf8_to_os_unicode(dst_buf, target->fullpath(), target->length());
                if (src->absolute_type() == path::absolute && src->length() > 4)
                    return _s_make_local_fs_link_win32(src_buf.begin()+4, dst_buf.begin());
                else
                    return _s_make_local_fs_link_win32(src_buf.begin(), dst_buf.begin());

#else


#endif //  WINDOWS


                return PILO_OK;
            }

        }
    }
}
 