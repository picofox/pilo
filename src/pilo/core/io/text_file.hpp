#ifndef _pilo_core_io_file_text_hpp_
#define _pilo_core_io_file_text_hpp_


#include    "./file.hpp"

namespace pilo
{
    namespace core
    {
        namespace io
        {
            template <typename PLOCK = ::pilo::core::process::dummy_file_lock
                , typename TLOCK = ::pilo::core::threading::dummy_read_write_lock>
            class text_file : public file<PLOCK, TLOCK>
            {
            public:
                typedef file<PLOCK, TLOCK> base_type;
                typedef PLOCK process_lock_type;
                typedef TLOCK thread_lock_type;

            public:
                static void s_free_line_buffer(char** p)
                {
                    if (p != nullptr && *p != nullptr) {
                        PMF_HEAP_FREE(*p);
                        *p = nullptr;
                    }
                }

            public:
                text_file(::pilo::i64_t read_buf_capa = SP_PMI_TEXT_FILE_RDBUF_SIZE
                    , ::pilo::i64_t write_buf_capa = SP_PMI_TEXT_FILE_WRBUF_SIZE
                    , const char* read_sep = nullptr
                    , const char* write_sep = nullptr)
                {
                    if (read_buf_capa < 0)
                        this->_m_rd_buf_capa = SP_PMI_TEXT_FILE_RDBUF_SIZE;
                    else
                        this->_m_rd_buf_capa = read_buf_capa;
                    if (this->_m_rd_buf_capa > 0) {
                        this->_m_rd_buf = (char*)PMF_HEAP_MALLOC(this->_m_rd_buf_capa);
                    } else {
                        this->_m_rd_buf = nullptr;                        
                    }
                    _m_rd_buf_idx = 0;
                    _m_rd_buf_size = 0;

                    if (write_buf_capa < 0)
                        this->_m_wr_buf_capa = SP_PMI_TEXT_FILE_WRBUF_SIZE;
                    else
                        this->_m_wr_buf_capa = write_buf_capa;
                    if (this->_m_wr_buf_capa > 0) {
                        this->_m_wr_buf = (char*)PMF_HEAP_MALLOC(this->_m_wr_buf_capa);
                    } else {
                        this->_m_wr_buf = nullptr;
                    }         
                    _m_wr_buf_size = 0;

                    if (read_sep != nullptr)
                        this->_m_read_sep = read_sep;
                    else
                        this->_m_read_sep = PMS_NEW_LINE_SEQ;
                    _m_read_sep_len = (::pilo::i32_t) ::pilo::core::string::character_count(_m_read_sep);

                    if (write_sep != nullptr)
                        this->_m_write_sep = write_sep;
                    else
                        this->_m_write_sep = PMS_NEW_LINE_SEQ;
                    _m_write_sep_len = (::pilo::i32_t)::pilo::core::string::character_count(_m_write_sep);

                }
                virtual ~text_file()
                {
                    this->finalize();
                }

                virtual ::pilo::err_t initialize(::pilo::tlv* param)
                {
                    ::pilo::err_t err = file<PLOCK, TLOCK>::initialize(param);
                    return err;
                }

                virtual ::pilo::err_t open(const char* path_str, creation_mode cm, access_permission perm, predefined_pilo_dir_enum prefix, dev_open_flags f)
                {
                    ::pilo::err_t err = file<PLOCK, TLOCK>::open(path_str, cm, perm, prefix, f);
                    return err;
                }

                virtual ::pilo::err_t open(const ::pilo::core::io::path* p, creation_mode cm, access_permission perm, dev_open_flags f)
                {
                    ::pilo::err_t err = file<PLOCK, TLOCK>::open(p, cm, perm, f);
                    return err;
                }

                virtual ::pilo::err_t open(const char* path_str, creation_mode cm, access_permission perm, dev_open_flags f)
                {
                    ::pilo::err_t err = file<PLOCK, TLOCK>::open(path_str, cm, perm, f);
                    return err;
                }

                virtual ::pilo::err_t close()
                {
                    ::pilo::core::io::xpf_close_file(&this->_m_fd);
                    this->set_state(state_code::closed);
                    return PILO_OK;
                }

                virtual ::pilo::err_t finalize()
                {
                    ::pilo::err_t err = file<PLOCK, TLOCK>::finalize();
                    _reset();
                    return err;
                }

                char* read_line(char* buffer, ::pilo::i64_t len, bool trim_nl, ::pilo::i64_t * rlen, ::pilo::err_t& r_err)
                {
                    PILO_ERRRET_NULL(this->_check_wirte_buffer(true));

                    if (this->_m_rd_buf == nullptr) {
                        return this->_read_line_no_buffer(buffer, len, trim_nl, rlen, r_err);
                    } else {
                        return this->_read_line(buffer, len, trim_nl, rlen, r_err);
                    }
                }

                ::pilo::err_t write_line(const char* buffer, ::pilo::i64_t wlen, bool nl = true, ::pilo::i64_t* rlen = nullptr)
                {

                    if (this->_m_wr_buf == nullptr) {
                        return this->_write_line_no_buffer(buffer, wlen, nl, rlen);
                    }
                    else {
                        return this->_write_line(buffer, wlen, nl, rlen);
                    }
                }

                virtual ::pilo::err_t seek(seek_whence whence, ::pilo::i64_t off)
                {
                    this->_check_wirte_buffer(true);
                    return base_type::seek(whence, off);
                }

                virtual ::pilo::err_t tell(::pilo::i64_t& off) 
                {
                    this->_check_wirte_buffer(true);
                    return base_type::tell(off);
                }

                virtual ::pilo::err_t flush(flush_level lv)
                {
                    this->_check_wirte_buffer(true);
                    return base_type ::flush(lv);
                }


                const char* read_seperator() const
                {
                    return this->_m_read_sep;
                }

                ::pilo::i64_t read_seperator_length() const
                {
                    return this->_m_read_sep_len;
                }

                const char* write_seperator() const
                {
                    return this->_m_write_sep;
                }

                ::pilo::i64_t write_seperator_length() const
                {
                    return this->_m_write_sep_len;
                }

                void set_read_seperator(const char* read_sep)
                {
                    if (read_sep != nullptr)
                        this->_m_read_sep = read_sep;
                    else
                        this->_m_read_sep = PMS_NEW_LINE_SEQ;
                    _m_read_sep_len = (::pilo::i32_t) ::pilo::core::string::character_count(_m_read_sep);
                }

                ::pilo::err_t set_write_seperator(const char* write_sep)
                {
                    PILO_ERRRET(this->_check_wirte_buffer(true));
                    if (write_sep != nullptr)
                        this->_m_write_sep = write_sep;
                    else
                        this->_m_write_sep = PMS_NEW_LINE_SEQ;
                    _m_write_sep_len = (::pilo::i32_t)::pilo::core::string::character_count(_m_write_sep);
                    return PILO_OK;
                }

                ::pilo::err_t formatted_output(bool nl, const char* fmt, ...)
                {
                    this->_check_wirte_buffer(true);
                    ::pilo::err_t err = PILO_OK;
                    va_list args;
                    va_start(args, fmt);

#               if defined(WINDOWS)
                    ::pilo::core::memory::object_array<char, 4096> buffer;
                    ::pilo::i32_t i = 0;
                    ::pilo::i64_t ret = 0;
                    bool done = false;
                    
                    while (i < 16) {
                        ret = _vsnprintf_s(buffer.begin(), buffer.capacity(), _TRUNCATE, fmt, args);
                        if (ret >= 0) {
                            buffer.set_size((::pilo::i32_t)ret);
                            done = true;
                            break;
                        }
                        buffer.check_space(4096 * (i + 1));
                        i++;
                    }

                    va_end(args);

                    if (!done) {
                        ::pilo::i64_t tr_size = ::pilo::core::string::character_count(buffer.begin());
                        buffer.set_size((::pilo::i32_t)tr_size);
                    }


                    ::pilo::i64_t rlen = 0;
                    err = this->write(buffer.begin(), buffer.size(), &rlen);   
                    if (err != PILO_OK) {
                        return err;
                    }     
                    if (rlen != buffer.size())
                        return err;

                    if (nl) {
                        err = this->write(this->_m_write_sep, this->_m_write_sep_len, &rlen);
                        if (err != PILO_OK) {
                            return err;
                        }
                    }
                                   

                    return PILO_OK;

#               else
                    int n = vdprintf(this->_m_fd, fmt, args);
                    if (n < 0)
                        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
                    va_end(args);

                    if (nl) {
                        dprintf(this->_m_fd, "%s", this->_m_write_sep);
                    }
                    if (n < 0)
                        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);

                    return PILO_OK;
#               endif

                    

                }

                template<typename CONTAINER_T>
                ::pilo::err_t read_lines(CONTAINER_T& cont, bool trim_nl, bool trim_empty_line)
                {
                    char buffer[4096] = { 0 };
                    char* ret = buffer;
                    ::pilo::i64_t rlen = 0;
                    ::pilo::err_t err = PILO_OK;
                    std::string str;
                    while (true) {
                        ret = this->read_line(buffer, sizeof(buffer), trim_nl, &rlen, err);
                        if (err != PILO_OK) {
                            if (err != PERR_EOF) {
                                if (ret != nullptr && ret != buffer) {
                                    ::pilo::core::io::text_file<>::s_free_line_buffer(&ret);                                    
                                }            
                                return err;
                            } else {                                
                                if (ret == nullptr)
                                {
                                    break;
                                }                                
                                if (*ret == 0 || rlen < 1) {
                                    if (!trim_empty_line) {
                                        cont.push_back(str);
                                    }                                    
                                } else {
                                    str.assign(ret, rlen);
                                    cont.push_back(str);
                                }       

                                break;
                            }
                        } else { //OK

                            if (*ret == 0 || rlen < 1) {
                                if (!trim_empty_line) {
                                    cont.push_back(str);
                                }
                            } else {
                                str.assign(ret, rlen);
                                cont.push_back(str);
                            }
                            if (ret != nullptr && ret != buffer) {
                                ::pilo::core::io::text_file<>::s_free_line_buffer(&ret);
                            }                            
                        }
                    }
                    if (ret != nullptr && ret != buffer) {
                        ::pilo::core::io::text_file<>::s_free_line_buffer(&ret);
                    }
                    return PILO_OK;
                }

            protected:

                ::pilo::err_t _check_wirte_buffer(bool b_force = false)
                {
                    if ((this->_m_wr_buf_size > 0) && (b_force || this->_m_wr_buf_capa <= this->_m_wr_buf_size)) {
                        ::pilo::err_t err = this->write(this->_m_wr_buf, this->_m_wr_buf_size, nullptr);
                        if (err != PILO_OK) {
                            return err;
                        }
                        this->_m_wr_buf_size = 0;
                    }
                    return PILO_OK;
                }



                ::pilo::err_t _write_line_no_buffer(const char* buffer, ::pilo::i64_t wlen, bool nl = true, ::pilo::i64_t* rlen = nullptr)
                {
                    ::pilo::i64_t rl = 0;
                    ::pilo::i64_t rl2 = 0;
                    ::pilo::err_t err = PILO_OK;
                    err = this->write(buffer, wlen, &rl);
                    if (err != PILO_OK) {
                        ::pilo::set_if_ptr_is_not_null(rlen, rl);
                        return err;
                    }
                    if (nl) {                        
                        err = this->write(this->_m_write_sep, this->_m_write_sep_len, &rl2);
                        rl += rl2;
                        ::pilo::set_if_ptr_is_not_null(rlen, rl);
                        if (err != PILO_OK) {    
                            return err;
                        }

                    } else {
                        ::pilo::set_if_ptr_is_not_null(rlen, rl);
                    }                    

                    return PILO_OK;
                }

                ::pilo::err_t _write_line(const char* buffer, ::pilo::i64_t wlen, bool nl = true, ::pilo::i64_t* rlen = nullptr)
                {
                    ::pilo::i64_t wlen2 = wlen;
                    if (nl)
                        wlen2 += this->_m_write_sep_len;

                    ::pilo::i64_t buf_avail = this->_m_wr_buf_capa - this->_m_wr_buf_size;

                    if (buf_avail >= wlen2) {
                        if (nl) {
                            ::memcpy(this->_m_wr_buf + this->_m_wr_buf_size, buffer, wlen);
                            ::memcpy(this->_m_wr_buf + this->_m_wr_buf_size + wlen, this->_m_write_sep, this->_m_write_sep_len);
                            this->_m_wr_buf_size += wlen2;
                        } else {
                            ::memcpy(this->_m_wr_buf + this->_m_wr_buf_size, buffer, wlen);
                            this->_m_wr_buf_size += wlen;
                        }

                        PILO_ERRRET(_check_wirte_buffer(false));
                        ::pilo::set_if_ptr_is_not_null(rlen, wlen2);
                        return PILO_OK;
                    
                    } else {
                        PILO_ERRRET(_check_wirte_buffer(true));
                        PILO_ERRRET(_write_line_no_buffer(buffer, wlen, nl, rlen));
                    }
                    
                    return PILO_OK;
                }

                char* _expand_buffer(char* old_buffer, char* orig_buffer, ::pilo::i64_t & len, ::pilo::i64_t to_length = -1) 
                {
                    if (to_length < len)
                        to_length = len + SP_PMI_TEXT_FILE_LINE_STEP_SIZE;

                    char* ret = nullptr;
                    if (old_buffer == orig_buffer) {
                        ret = (char*)PMF_HEAP_MALLOC(to_length);
                        if (ret != nullptr) {
                            memcpy(ret, old_buffer, len);
                        } else {
                            return nullptr;
                        }
                        
                    } else {
                        ret = (char*) PMF_HEAP_REALLOC(old_buffer, to_length);
                        if (ret == nullptr) {
                            if (old_buffer != nullptr)
                                PMF_HEAP_FREE(old_buffer);
                            return nullptr;
                        }
                    }
                    
                    len = to_length;
                    return ret;
                }

                char* _append_term_null_char(char* old_buffer, char* orig_buffer, ::pilo::i64_t& buf_capa, ::pilo::i64_t idx, ::pilo::err_t & err)
                {
                    if (idx >= buf_capa - 1) {
                        old_buffer = _expand_buffer(old_buffer, orig_buffer, buf_capa);
                        if (old_buffer == nullptr) {
                            err = ::pilo::mk_err(PERR_INSUF_HEAP);
                            return nullptr;
                        }
                    }
                    old_buffer[idx] = 0;
                    return old_buffer;
                }

                char* _append_read_sep(char* old_buffer, char* orig_buffer, ::pilo::i64_t buf_capa, ::pilo::i64_t& idx, ::pilo::err_t& err)
                {
                    if (idx >= buf_capa - 1 - this->_m_read_sep_len) {
                        old_buffer = _expand_buffer(old_buffer, orig_buffer, buf_capa);
                        if (old_buffer == nullptr) {
                            err = ::pilo::mk_err(PERR_INSUF_HEAP);
                            return nullptr;
                        }
                    }
                    ::pilo::core::string::n_copyz(old_buffer+idx, buf_capa, this->_m_read_sep, this->_m_read_sep_len);
                    idx += this->_m_read_sep_len;
                    return old_buffer;
                }

                char* _read_line_no_buffer(char* buffer, ::pilo::i64_t len, bool trim_nl, ::pilo::i64_t* rlen, ::pilo::err_t& r_err)
                {
                    ::pilo::i64_t idx = 0;
                    char* ret = buffer;
                    ::pilo::i64_t matching_idx = 0;

                    if (ret == nullptr || len < 1) {
                        ret = (char*)PMF_HEAP_MALLOC(SP_PMI_TEXT_FILE_DFL_LINE_SIZE);
                        if (ret == nullptr) {
                            ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i64_t)0);
                            r_err = ::pilo::mk_err(PERR_INSUF_HEAP);
                            return nullptr;
                        }
                        len = SP_PMI_TEXT_FILE_DFL_LINE_SIZE;
                    }
      
                    while (true) {
                        r_err = this->read(ret + idx, (::pilo::i64_t) 1, nullptr);
                         if (r_err != PILO_OK) {
                            if (r_err == PERR_EOF) {
                                ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i64_t)idx);
                                ret = this->_append_term_null_char(ret, buffer, len, idx, r_err);
                                return ret;
                            } else {
                                ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i64_t)0);
                                return nullptr;
                            }

                        }
                        if (ret[idx] == this->_m_write_sep[matching_idx]) {
                            if (_m_write_sep[matching_idx + 1] == 0) {
                                if (!trim_nl) {
                                    idx++;
                                    idx -= ::pilo::core::string::character_count(this->_m_write_sep);
                                    ret = this->_append_read_sep(ret, buffer, len, idx, r_err);
                                    ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i64_t)idx);
                                    return ret;

                                } else {
                                    ret[idx] = 0;
                                    idx++;
                                    idx -= ::pilo::core::string::character_count(this->_m_write_sep);                                    
                                    ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i64_t)idx);
                                    return ret;
                                }
                                
                            }
                            if (trim_nl) {
                                ret[idx] = 0;
                            }                      
                            matching_idx ++;

                        } else {
                            matching_idx = 0;
                        }

                        idx++;
                        
                        if (idx >= len) {
                            ret = _expand_buffer(ret, buffer, len);
                            if (ret == nullptr) {
                                r_err = ::pilo::mk_err(PERR_INSUF_HEAP);
                                return nullptr;
                            }
                        }                        
                    } 

                }

                char* _read_line(char* buffer, ::pilo::i64_t len, bool trim_nl, ::pilo::i64_t* rlen,::pilo::err_t& r_err)
                {                
                    char* ret = buffer;
                    bool is_eof = false;
                    ::pilo::i64_t tmp_n_read = 0;
                    ::pilo::i64_t total_rlen = 0;
                    ::pilo::i64_t rdbuf_rd_availe = 0;
                    ::pilo::i64_t rdbuf_wr_availe = 0;
                    ::pilo::i64_t line_content_len = 0;
                    ::pilo::i64_t totoal_space_taken = 0;
                    ::pilo::i64_t user_buffer_idx = 0;
                    while (true) {
                        rdbuf_wr_availe = this->_m_rd_buf_capa - this->_m_rd_buf_size;
                        if (rdbuf_wr_availe > 0) { //fill rd buf first of all
                            r_err = this->read(this->_m_rd_buf + this->_m_rd_buf_size, rdbuf_wr_availe, &tmp_n_read);
                            if (r_err != PILO_OK) {
                                if (r_err != PERR_EOF) {
                                    if (ret != buffer && ret != nullptr) {
                                        s_free_line_buffer(&ret);
                                    }
                                    ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i64_t) 0);
                                    return nullptr;
                                }
                                is_eof = true; 
                            }
                            this->_m_rd_buf_size += tmp_n_read;
                        }
                        //to check if current buffer meet our requirement, copy to user buffer if yes,then ret
                        rdbuf_rd_availe = _m_rd_buf_size - _m_rd_buf_idx;
                        if (rdbuf_rd_availe < 1) {
                            this->_m_rd_buf_size = 0;
                            this->_m_rd_buf_idx = 0;
                            ::pilo::set_if_ptr_is_not_null(rlen, total_rlen);
                            if (total_rlen <= 0) {
                                if (ret != buffer && ret != nullptr) {
                                    s_free_line_buffer(&ret);
                                }
                                ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i64_t) 0);
                                return nullptr;
                            }
                            return ret;
                        }

                        ::pilo::i64_t needle_matched_len = 0;
                        const char* src = ::pilo::core::string::find_substring(_m_rd_buf + _m_rd_buf_idx,  rdbuf_rd_availe, _m_write_sep, _m_write_sep_len,needle_matched_len);
                        if (nullptr != src) {
                            line_content_len = src - (_m_rd_buf + _m_rd_buf_idx); //line find in cache, no line-sep TODO check negative value
                            if (trim_nl) {
                                totoal_space_taken = totoal_space_taken + line_content_len + 1;
                            } else {
                                totoal_space_taken = totoal_space_taken + line_content_len + 1 + this->_m_read_sep_len;
                            }
                            if (totoal_space_taken > len) {
                                ret = this->_expand_buffer(ret, buffer, len , totoal_space_taken);
                            }
                            ::pilo::core::string::n_copyz(ret + user_buffer_idx, len - user_buffer_idx, _m_rd_buf + _m_rd_buf_idx, line_content_len);
                            _m_rd_buf_idx = _m_rd_buf_idx + line_content_len + this->_m_write_sep_len;
                            user_buffer_idx += line_content_len;
                            total_rlen += line_content_len;
                            if (!trim_nl) {
                                ::pilo::core::string::n_copyz(ret + user_buffer_idx, len - user_buffer_idx, this->_m_read_sep, this->_m_read_sep_len);
                                total_rlen += this->_m_read_sep_len;
                                user_buffer_idx += this->_m_read_sep_len;
                            } 
                            ::pilo::set_if_ptr_is_not_null(rlen, total_rlen);
                            return ret;

                        }  else if (needle_matched_len > 0) {
                            totoal_space_taken = totoal_space_taken + rdbuf_rd_availe + 1;
                            if (len < totoal_space_taken) {
                                ret = this->_expand_buffer(ret, buffer, len, totoal_space_taken);
                            }
                            ::pilo::i64_t clen = rdbuf_rd_availe - needle_matched_len;
                            ::pilo::core::string::n_copyz(ret + user_buffer_idx, len - user_buffer_idx, _m_rd_buf + _m_rd_buf_idx, clen);
                            user_buffer_idx += clen;
                            total_rlen += clen;

                            ::pilo::i64_t remain_in_buffer = this->_m_write_sep_len - needle_matched_len;
                            ::pilo::i64_t remain_in_buffer2 = this->_m_rd_buf_capa - (_m_rd_buf_idx + clen);
                            if (remain_in_buffer != remain_in_buffer2) {
                                r_err = ::pilo::mk_perr(PERR_INC_DATA);
                                if (ret != buffer && ret != nullptr) {
                                    s_free_line_buffer(&ret);
                                }
                                ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i64_t)0);
                                return nullptr;
                            }

                            ::pilo::core::string::n_move(_m_rd_buf, this->_m_rd_buf_capa , _m_rd_buf + _m_rd_buf_idx + clen, this->_m_write_sep_len - needle_matched_len);
                            _m_rd_buf_idx = 0;
                            _m_rd_buf_size = this->_m_write_sep_len - needle_matched_len;
                            if (is_eof) {
                                if (!trim_nl) {
                                    ret = this->_append_read_sep(ret, buffer, len, total_rlen, r_err);
                                }
                                ::pilo::set_if_ptr_is_not_null(rlen, total_rlen);
                                return ret;
                            }


                        }  else {
                            totoal_space_taken = totoal_space_taken + rdbuf_rd_availe + 1;
                            if (len < totoal_space_taken) {
                                ret = this->_expand_buffer(ret, buffer, len, totoal_space_taken);
                            }

                            ::pilo::core::string::n_copyz(ret + user_buffer_idx, len - user_buffer_idx, _m_rd_buf + _m_rd_buf_idx, rdbuf_rd_availe);
                            user_buffer_idx += rdbuf_rd_availe;
                            total_rlen += rdbuf_rd_availe;
                            _m_rd_buf_idx = 0;
                            _m_rd_buf_size = 0;
                            if (is_eof) {
                                if (!trim_nl) {
                                    ret = this->_append_read_sep(ret, buffer, len, total_rlen, r_err);
                                }
                                
                                ::pilo::set_if_ptr_is_not_null(rlen, total_rlen);
                                return ret;
                            }                        
                        }
                    
                    } //end of while

                    
                    return nullptr;
                }

                void _reset()
                {
                    if (_m_rd_buf != nullptr) {
                        PMF_HEAP_FREE(_m_rd_buf);
                        _m_rd_buf = nullptr;
                    }
                    _m_rd_buf_size = -1;
                    _m_rd_buf_idx = 0;

                    if (_m_wr_buf != nullptr) {
                        PMF_HEAP_FREE(_m_wr_buf);
                        _m_wr_buf = nullptr;
                    }
                    _m_wr_buf_size = -1;
                }

            protected:
                ::pilo::i64_t   _m_rd_buf_capa;
                ::pilo::i64_t   _m_wr_buf_capa;                
                const char* _m_read_sep;
                const char* _m_write_sep;
                ::pilo::i32_t _m_read_sep_len;
                ::pilo::i32_t _m_write_sep_len;

                char* _m_rd_buf;
                char* _m_wr_buf;
                ::pilo::i64_t   _m_rd_buf_idx;
                ::pilo::i64_t   _m_rd_buf_size;
                ::pilo::i64_t   _m_wr_buf_size;

            };
        } //end of name space io
    }//end of name space core
}//end of name space pilo


#endif