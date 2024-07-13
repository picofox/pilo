#ifndef __pilo_core_memory_linked_byte_buffer_hpp_
#define __pilo_core_memory_linked_byte_buffer_hpp_

#include <mutex>
#include <algorithm>
#include "../../pilo.hpp"
#include "../string/string_operation.hpp"
#include "./abstract_byte_buffer.hpp"
#include "../container/singly_linked_selflist.hpp"
#include "../memory/object_pool.hpp"
#include "../memory/compactable_object_pool.hpp"
#include "../algorithm/minmax.hpp"
#include "../threading/spin_mutex.hpp"
namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template<::pilo::i64_t TA_UNIT_SIZE, ::pilo::i64_t TA_STEP, bool TA_BIGENDIAN>
            class linked_byte_buffer : public abstract_byte_buffer<TA_BIGENDIAN>
            {
            public:
                struct buffer_node : public ::pilo::core::memory::portable_compactable_object_pool<buffer_node, TA_STEP, ::pilo::core::threading::spin_mutex>
                {                    
                    buffer_node* m_next;
                    char         data[TA_UNIT_SIZE - sizeof(buffer_node*) - sizeof(void*)];                    
                };

   
                inline static ::pilo::i64_t piece_size() { return TA_UNIT_SIZE - sizeof(buffer_node*) - sizeof(void*); }

            public:
                linked_byte_buffer() : _m_capacity(0), _m_begin_pos(0), _m_length(0), _m_saved_node_for_writing(nullptr), _m_saved_pos_for_writing(0)
                {

                }

                virtual ~linked_byte_buffer()
                {
                    clear();
                }

                virtual std::string to_string() const
                {
                    std::stringstream ss;
                    ss << _m_capacity << "," << _m_begin_pos << "," << _m_length << "," << this->_m_node_list.size();
                    return ss.str();
                }

                virtual ::pilo::err_t compact(::pilo::ioop_type_enum ioop)
                {
                    ::pilo::err_t err = PILO_OK;
                    if (((::pilo::u8_t)ioop & (::pilo::u8_t) ::pilo::ioop_type_enum::read) != 0)
                    {
                        while (this->_m_begin_pos >= piece_size())
                        {
                            err = this->_clear_front();
                            if (err != PILO_OK)
                                break;
                        }
                    }
                    if (((::pilo::u8_t)ioop & (::pilo::u8_t) ::pilo::ioop_type_enum::write) != 0)
                    {
                        ::pilo::i64_t wp = this->_m_begin_pos + this->_m_length;
                        ::pilo::i64_t node_cnt = wp / piece_size();
                        ::pilo::i64_t remain = wp % piece_size();
                        if (remain > 0)
                            node_cnt++;
                        ::pilo::i64_t to_del_cnt = this->_m_node_list.size() - node_cnt;
                        if (to_del_cnt < 1)
                            return PILO_OK;
                        buffer_node* pnode = nullptr;
                        if (node_cnt > 0)
                        {
                            pnode = this->_m_node_list.front();
                            for (::pilo::i64_t i = 0; i < node_cnt - 1; i++)
                            {
                                pnode = this->_m_node_list.next(pnode);
                            }
                        }

                        buffer_node* del_pnode = nullptr;
                        while (to_del_cnt > 0)
                        {
                            if (!this->_m_node_list.erase_after(pnode, &del_pnode))
                            {
                                return ::pilo::mk_perr( PERR_INC_DATA);
                            }
                            buffer_node::deallocate(del_pnode);
                            to_del_cnt--;
                            this->_m_capacity -= piece_size();
                        }
                    }

                    return err;
                }

                virtual ::pilo::err_t readable_seek(::pilo::seek_whence_enum whence, ::pilo::i64_t off, bool compact, ::pilo::i64_t* old_pos)
                {
                    ::pilo::set_if_ptr_is_not_null<::pilo::i64_t>(old_pos, this->read_pos());
                    if (whence == ::pilo::seek_whence_enum::begin)
                    {
                        if (off < 0 || off > this->_m_begin_pos + this->_m_length)
                        {
                            return ::pilo::mk_perr( PERR_INV_OFF);
                        }
                        off = off - this->_m_length; //convert to relative
                    }
                    else if (whence == ::pilo::seek_whence_enum::end)
                    {
                        if (off < (0 - this->_m_begin_pos) || off > this->_m_length)
                        {
                            return ::pilo::mk_perr( PERR_INV_OFF);
                        }
                    }
                    else if (whence == ::pilo::seek_whence_enum::end)
                    {
                        return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
                    }
                    if (off == 0)
                        return PILO_OK;

                    if (this->_m_node_list.front() == nullptr)
                    {
                        return ::pilo::mk_perr(PERR_NULL_PTR);
                    }

                    this->_m_begin_pos = this->_m_begin_pos + off;
                    this->_m_length = this->_m_length - off;

                    if (compact && (this->_m_begin_pos >= piece_size()))
                    {
                        return this->compact(::pilo::ioop_type_enum::read);
                    }

                    return PILO_OK;
                }
                virtual ::pilo::err_t writable_seek(::pilo::seek_whence_enum whence, ::pilo::i64_t off, bool compact, ::pilo::i64_t* old_pos)
                {
                    ::pilo::set_if_ptr_is_not_null<::pilo::i64_t>(old_pos, this->_m_begin_pos + this->_m_length);
                    if (whence == ::pilo::seek_whence_enum::begin)
                    {
                        if (off < this->_m_begin_pos)
                        {
                            return ::pilo::mk_perr( PERR_INV_OFF);
                        }
                        off = off - this->_m_length - this->_m_begin_pos; //convert to relative
                    }
                    else if (whence == ::pilo::seek_whence_enum::end)
                    {
                        return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
                    }
                    if (off == 0)
                        return PILO_OK;
                    else if (off < 0)
                    {
                        this->_m_length += off;
                    }
                    else
                    {
                        ::pilo::i64_t wp = this->_m_begin_pos + off + this->_m_length;
                        ::pilo::i64_t cnt = wp / piece_size();
                        ::pilo::i64_t rem = wp % piece_size();
                        if (rem > 0)
                            cnt++;
                        buffer_node* next_node = this->_m_node_list.back();
                        while (cnt > 0)
                        {
                            next_node = this->_get_next_node(next_node);
                            if (next_node == nullptr)
                            {
                                return ::pilo::mk_perr( PERR_INSUF_HEAP);
                            }
                            cnt--;
                        }
                        this->_m_length += off;
                    }

                    if (compact && (this->_m_capacity - this->_m_begin_pos - this->_m_length >= piece_size()))
                    {
                        return this->compact(::pilo::ioop_type_enum::write);
                    }

                    return PILO_OK;
                }


                virtual ::pilo::i64_t read_available() const
                {
                    return this->_m_length;
                }

                virtual ::pilo::i64_t write_available() const
                {
                    return this->_m_capacity - this->_m_begin_pos - this->_m_length;
                }

                virtual ::pilo::i64_t read_pos() const
                {
                    return this->_m_begin_pos;
                }

                virtual ::pilo::i64_t write_pos() const
                {
                    return this->_m_begin_pos + this->_m_length;
                }

                virtual ::pilo::i64_t capacity() const
                {
                    return this->_m_capacity;
                }

                virtual void clear()
                {
                    this->_m_begin_pos = 0;
                    this->_m_length = 0;
                    this->_m_capacity = 0;
                    buffer_node* node_ptr = this->_m_node_list.begin();
                    while ((node_ptr = this->_m_node_list.pop_front()) != nullptr)
                    {
                        buffer_node::deallocate(node_ptr);
                    }
                    this->_m_node_list.clear();
                }

                virtual void reset()
                {
                    clear();
                }

                virtual char* read_raw_bytes(char* dst_buffer, ::pilo::i64_t dst_buffer_size, ::pilo::i64_t dst_pos, ::pilo::i64_t read_length, ::pilo::err_t& err)
                {
                    char* ret_buffer_ptr = dst_buffer;
                    if (read_length > 0)
                    {
                        if (_m_length < read_length)
                        {
                            err = ::pilo::mk_perr(PERR_RD_PARTIAL_DATA);
                            return dst_buffer;
                        }
                        if (dst_buffer == nullptr || dst_buffer_size - dst_pos < read_length)
                        {
                            ::pilo::i64_t neo_sz = read_length + dst_pos;
                            ret_buffer_ptr = (char*)PMF_HEAP_MALLOC(neo_sz);
                            if (ret_buffer_ptr == nullptr)
                            {
                                err = ::pilo::mk_perr( PERR_INSUF_HEAP);
                                return nullptr;
                            }

                            if (dst_buffer != nullptr && dst_buffer_size > 0)
                            {
                                memcpy(ret_buffer_ptr, dst_buffer, dst_buffer_size);
                            }
                            
                            dst_buffer_size = neo_sz;
                        }
                        ::pilo::i64_t cur_turn_read = piece_size() - _m_begin_pos;
                        if (cur_turn_read < 0)
                        {
                            PMF_FREE_IF_DIFF_ADDR(dst_buffer, ret_buffer_ptr);
                            return nullptr;
                        }
                        if (cur_turn_read > read_length)
                        {
                            cur_turn_read = read_length;
                        }
                        ::pilo::i64_t idx = 0;

                        while (read_length > 0)
                        {
                            if (_m_node_list.front() == nullptr)
                            {
                                PMF_FREE_IF_DIFF_ADDR(dst_buffer, ret_buffer_ptr);
                                err = ::pilo::mk_perr( PERR_INC_DATA);
                                return nullptr;
                            }
                            memcpy(ret_buffer_ptr + dst_pos + idx
                                , _m_node_list.front()->data + _m_begin_pos
                                , cur_turn_read);
                            idx += cur_turn_read;
                            _m_begin_pos += cur_turn_read;
                            _m_length -= cur_turn_read;
                            read_length -= cur_turn_read;

                            if (_m_begin_pos >= piece_size())
                            {
                                if ((err = this->compact(::pilo::ioop_type_enum::read)) != PILO_OK)
                                {
                                    PMF_FREE_IF_DIFF_ADDR(dst_buffer, ret_buffer_ptr);
                                    return nullptr;
                                }
                            }
                            if (read_length == 0)
                            {
                                break;
                            }
                            else if (read_length < 0)
                            {
                                PMF_FREE_IF_DIFF_ADDR(dst_buffer, ret_buffer_ptr);
                                err = ::pilo::mk_perr(PERR_LEN_TOO_SAMLL);
                                return nullptr;
                            }
                            else if (read_length > piece_size())
                            {
                                cur_turn_read = piece_size();
                            }
                            else
                            {
                                cur_turn_read = read_length;
                            }
                        } // end of while
                        err = PILO_OK;
                        return ret_buffer_ptr;
                    }
                    else if (read_length == 0)
                    {
                        err = PILO_OK;
                        return dst_buffer;
                    }
                    else
                    {
                        err = ::pilo::mk_perr(PERR_INVALID_PARAM);
                        return nullptr;
                    }
                }

                virtual char* peek_raw_bytes(::pilo::i64_t peek_off, char* dst_buffer, ::pilo::i64_t dst_buffer_size, ::pilo::i64_t dst_pos, ::pilo::i64_t peek_length, ::pilo::err_t& err)
                {
                    if (peek_length > 0)
                    {
                        char* ret_buffer_ptr = dst_buffer;
                        ::pilo::i64_t simu_begin_pos = this->_m_begin_pos + peek_off;
                        ::pilo::i64_t simu_length = this->_m_length - peek_off;
                        if (simu_begin_pos > this->_m_begin_pos + this->_m_length)
                        {
                            err = ::pilo::mk_perr(PERR_RD_PARTIAL_DATA);
                            return nullptr;
                        }
                        ::pilo::i64_t cur_trun_read_bytes = 0;
                        if (simu_length < peek_length)
                        {
                            err = ::pilo::mk_perr(PERR_RD_PARTIAL_DATA);
                            return nullptr;
                        }

                        if (dst_buffer_size - dst_pos < peek_length)
                        {
                            ::pilo::i64_t neo_sz = peek_length + dst_pos;
                            ret_buffer_ptr = (char*)PMF_HEAP_MALLOC(neo_sz);
                            if (ret_buffer_ptr == nullptr)
                            {
                                err = ::pilo::mk_perr( PERR_INSUF_HEAP);
                                return nullptr;
                            }
                            memcpy(ret_buffer_ptr, dst_buffer, dst_buffer_size);
                            dst_buffer_size = neo_sz;
                        }

                        ::pilo::i64_t neo_begin_pos = 0;
                        buffer_node* node = this->_find_node(simu_begin_pos, neo_begin_pos);
                        if (node == nullptr)
                        {
                            PMF_FREE_IF_DIFF_ADDR(dst_buffer, ret_buffer_ptr);
                            err = ::pilo::mk_perr( PERR_INC_DATA);
                            return nullptr;
                        }
                        ::pilo::i64_t read_avail_cur_node = piece_size() - neo_begin_pos;
                        if (read_avail_cur_node >= peek_length)
                            cur_trun_read_bytes = peek_length;
                        else
                            cur_trun_read_bytes = read_avail_cur_node;
                        ::pilo::i64_t idx = 0;
                        while (idx < peek_length)
                        {
                            memcpy(ret_buffer_ptr + idx, node->data + neo_begin_pos, cur_trun_read_bytes);
                            idx += cur_trun_read_bytes;
                            simu_begin_pos += cur_trun_read_bytes;
                            simu_length -= cur_trun_read_bytes;
                            if (idx == peek_length)
                                break;
                            else if (idx > peek_length)
                            {
                                PMF_FREE_IF_DIFF_ADDR(dst_buffer, ret_buffer_ptr);
                                err = ::pilo::mk_perr( PERR_INV_OFF);
                                return nullptr;
                            }

                            if (simu_begin_pos >= piece_size())
                            {
                                node = this->_m_node_list.next(node);
                                if (node == nullptr)
                                {
                                    PMF_FREE_IF_DIFF_ADDR(dst_buffer, ret_buffer_ptr);
                                    err = ::pilo::mk_perr( PERR_INC_DATA);
                                    return nullptr;
                                }
                                neo_begin_pos = 0;
                            }

                            if (peek_length - idx > piece_size())
                                cur_trun_read_bytes = piece_size();
                            else
                                cur_trun_read_bytes = peek_length - idx;
                        }

                        err = PILO_OK;
                        return ret_buffer_ptr;
                    }
                    else if (peek_length == 0)
                    {
                        err = PILO_OK;
                        return dst_buffer;
                    }
                    else
                    {
                        err = ::pilo::mk_perr(PERR_INVALID_PARAM);
                        return nullptr;
                    }
                }

                virtual ::pilo::err_t write_raw_bytes_pc(const char* write_buffer, ::pilo::i64_t write_off, ::pilo::i64_t write_len)
                {
                    if (write_len <= 0)
                    {
                        return PILO_OK;
                    }
                    ::pilo::i64_t off = 0;
                    PMC_ASSERT(_m_begin_pos < piece_size());
                    if (_m_begin_pos >= piece_size())
                    {
                        return ::pilo::mk_perr( PERR_INV_OFF);
                    }
                    ::pilo::i64_t begpos = 0;
                    buffer_node* next_node = this->_node_for_writing(begpos);
                    if (next_node == nullptr)
                    {
                        return ::pilo::mk_perr(PERR_NULL_PTR);
                    }
                    ::pilo::i64_t cur_turn_write = piece_size() - begpos;
                    this->_m_saved_node_for_writing = next_node;
                    this->_m_saved_pos_for_writing = begpos;
                    if (cur_turn_write >= write_len)
                    {
                        memcpy(next_node->data + begpos, write_buffer + write_off, write_len);
                        _m_length += write_len;
                    }
                    else
                    {
                        while (write_len > 0)
                        {
                            memcpy(next_node->data + begpos, write_buffer + write_off + off, cur_turn_write);
                            _m_length += cur_turn_write;
                            off += cur_turn_write;
                            write_len -= cur_turn_write;
                            if (write_len < 0)
                            {
                                return ::pilo::mk_perr(PERR_LEN_TOO_SAMLL);
                            }
                            else if (write_len == 0)
                            {
                                break;
                            }

                            next_node = this->_get_next_node(next_node);
                            if (next_node == nullptr)
                            {
                                return ::pilo::mk_perr(PERR_NULL_PTR);
                            }
                            begpos = 0;
                            if (piece_size() < write_len)
                                cur_turn_write = piece_size();
                            else
                                cur_turn_write = write_len;
                        }
                    }

                    return PILO_OK;
                }

                virtual ::pilo::err_t write_raw_bytes(const char* write_buffer, ::pilo::i64_t write_off, ::pilo::i64_t write_len)
                {
                    if (write_len <= 0)
                    {
                        return PILO_OK;
                    }
                    ::pilo::i64_t off = 0;
                    PMC_ASSERT(_m_begin_pos < piece_size());
                    if (_m_begin_pos >= piece_size())
                    {
                        return ::pilo::mk_perr( PERR_INV_OFF);
                    }
                    ::pilo::i64_t begpos = 0;
                    buffer_node* next_node = this->_node_for_writing(begpos);
                    if (next_node == nullptr)
                    {
                        return ::pilo::mk_perr(PERR_NULL_PTR);
                    }
                    ::pilo::i64_t cur_turn_write = piece_size() - begpos;
                    if (cur_turn_write >= write_len)
                    {
                        memcpy(next_node->data + begpos, write_buffer + write_off, write_len);
                        _m_length += write_len;
                    }
                    else
                    {
                        while (write_len > 0)
                        {
                            memcpy(next_node->data + begpos, write_buffer + write_off + off, cur_turn_write);
                            _m_length += cur_turn_write;
                            off += cur_turn_write;
                            write_len -= cur_turn_write;
                            if (write_len < 0)
                            {
                                return ::pilo::mk_perr(PERR_LEN_TOO_SAMLL);
                            }
                            else if (write_len == 0)
                            {
                                break;
                            }

                            next_node = this->_get_next_node(next_node);
                            if (next_node == nullptr)
                            {
                                return ::pilo::mk_perr(PERR_NULL_PTR);
                            }
                            begpos = 0;
                            if (piece_size() < write_len)
                                cur_turn_write = piece_size();
                            else
                                cur_turn_write = write_len;
                        }
                    }

                    return PILO_OK;
                }

                virtual ::pilo::err_t set_raw_bytes(::pilo::i64_t buffer_off, const char* write_buffer, ::pilo::i64_t write_off, ::pilo::i64_t write_len)
                {
                    ::pilo::i64_t neo_off = 0;
                    buffer_node* node = this->_find_node(buffer_off, neo_off);
                    if (node == nullptr)
                    {
                        return ::pilo::mk_perr( PERR_INV_OFF);
                    }

                    return _set_raw_bytes_by_node(node, neo_off, write_buffer, write_off, write_len);
                }

                virtual ::pilo::err_t set_pc_raw_bytes(const char* write_buffer, ::pilo::i64_t write_off, ::pilo::i64_t write_len)
                {
                    return _set_raw_bytes_by_node(this->_m_saved_node_for_writing, this->_m_saved_pos_for_writing, write_buffer, write_off, write_len);
                }

                virtual buffer_node* _node_for_writing(::pilo::i64_t& blen)
                {
                    if (this->_m_capacity - this->_m_begin_pos - this->_m_length >= piece_size())
                    {
                        ::pilo::err_t err = this->compact(::pilo::ioop_type_enum::write);
                        if (err != PILO_OK)
                            return nullptr;
                    }

                    ::pilo::i64_t wp = _m_begin_pos + _m_length;
                    //::pilo::i64_t cur_node_count = wp / piece_size();
                    blen = wp % piece_size();
                    buffer_node* node = this->_m_node_list.back();
                    if (blen > 0)
                    {
                        return node;
                    }
                    else if (blen == 0 && node != nullptr && node->m_next != nullptr)
                    {
                        PMC_ASSERT(false);
                    }
                    return this->_get_next_node(node);
                }

                virtual char* read_bytes(char* dst_buffer, ::pilo::i64_t dst_buffer_size, ::pilo::i64_t dst_pos, ::pilo::i64_t& bytes_read, ::pilo::err_t& err)
                {
                    ::pilo::i32_t blen = 0;
                    err = this->peek_int32(0, blen);
                    if (err != PILO_OK)
                        return nullptr;
                    if (blen < 0)
                    {
                        bytes_read = -1;
                        this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        return nullptr;
                    }
                    else if (blen == 0)
                    {
                        bytes_read = 0;
                        this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        return dst_buffer;
                    }
                    if (this->read_available() < ((::pilo::i64_t)blen + 4))
                    {
                        err = ::pilo::mk_perr(PERR_RD_PARTIAL_DATA);
                        bytes_read = 0;
                        return nullptr;
                    }
                    this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                    char* dst_buffer_ptr = this->read_raw_bytes(dst_buffer, dst_buffer_size, dst_pos, blen, err);
                    if (dst_buffer_ptr == nullptr || err != PILO_OK)
                    {
                        bytes_read = 0;
                        return nullptr;
                    }
                    bytes_read = blen;
                    return dst_buffer_ptr;
                }

                virtual char* peek_bytes(::pilo::i64_t peek_off, char* dst_buffer, ::pilo::i64_t dst_buffer_size, ::pilo::i64_t dst_pos, ::pilo::i64_t& bytes_peeked, ::pilo::err_t& err)
                {
                    ::pilo::i32_t blen = 0;
                    err = this->peek_int32(peek_off, blen);
                    if (err != PILO_OK)
                        return nullptr;
                    if (blen < 0)
                    {
                        bytes_peeked = 0;
                        return nullptr;
                    }
                    else if (blen == 0)
                    {
                        bytes_peeked = 0;
                        return dst_buffer;
                    }
                    if (this->read_available() - peek_off < ((::pilo::i64_t)blen + 4))
                    {
                        err = ::pilo::mk_perr(PERR_RD_PARTIAL_DATA);
                        bytes_peeked = 0;
                        return nullptr;
                    }

                    char* dst_buffer_ptr = this->peek_raw_bytes(sizeof(::pilo::i32_t) + peek_off, dst_buffer, dst_buffer_size, dst_pos, blen, err);
                    if (dst_buffer_ptr != nullptr || err != PILO_OK)
                    {
                        bytes_peeked = 0;
                        return nullptr;
                    }
                    bytes_peeked = blen;
                    return dst_buffer_ptr;
                }

                virtual ::pilo::err_t write_bytes(const char* write_buffer, ::pilo::i64_t write_off, ::pilo::i64_t write_len)
                {
                    PMC_ASSERT(write_len <= PILO_INT32_MAX);
                    if (write_buffer == nullptr)
                    {
                        return this->write_int32((::pilo::i32_t)-1);
                    }                    
                    if (write_len <= 0)
                    {
                        return this->write_int32((::pilo::i32_t) 0);
                    }

                    if (this->write_int32((::pilo::i32_t)write_len) == PILO_OK)
                    {
                        return this->write_raw_bytes(write_buffer, write_off, write_len);
                    }
                    return ::pilo::mk_perr( PERR_IO_READ_FAIL);
                }

                virtual ::pilo::err_t set_bytes(::pilo::i64_t buffer_off, const char* write_buffer, ::pilo::i64_t write_off, ::pilo::i64_t write_len)
                {
                    PMC_ASSERT(write_len <= PILO_INT32_MAX);
                    if (this->set_int32(buffer_off, (::pilo::i32_t)write_len) == PILO_OK)
                    {
                        return this->set_raw_bytes(4 + buffer_off, write_buffer, write_off, write_len);
                    }
                    return ::pilo::mk_perr( PERR_IO_READ_FAIL);
                }

                virtual char* read_cstring(char* dst_buffer, ::pilo::i64_t dst_buffer_size, ::pilo::i64_t dst_pos, ::pilo::i64_t& str_len, ::pilo::err_t& err)
                {
                    ::pilo::i32_t blen = 0;
                    err = this->peek_int32(0, blen);
                    if (err != PILO_OK)
                        return nullptr;
                    if (blen < 1)
                    {
                        str_len = -1;
                        this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        return nullptr;
                    }
                    else if (blen == 1)
                    {
                        str_len = 0;
                        this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        return dst_buffer;
                    }
                    if (this->read_available() < ((::pilo::i64_t)blen + 4))
                    {
                        err = ::pilo::mk_perr(PERR_RD_PARTIAL_DATA);
                        str_len = 0;
                        return nullptr;
                    }
                    this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                    char* dst_buffer_ptr = this->read_raw_bytes(dst_buffer, dst_buffer_size, dst_pos, blen, err);
                    if (dst_buffer_ptr == nullptr || err != PILO_OK)
                    {
                        str_len = 0;
                        return nullptr;
                    }
                    str_len = (::pilo::i64_t)(blen - 1);
                    return dst_buffer_ptr;
                }

                virtual char* peek_cstring(::pilo::i64_t peek_off, char* dst_buffer, ::pilo::i64_t dst_buffer_size, ::pilo::i64_t dst_pos, ::pilo::i64_t& str_peeked_len, ::pilo::err_t& err)
                {
                    ::pilo::i32_t blen = 0;
                    err = this->peek_int32(peek_off, blen);
                    if (err != PILO_OK)
                        return nullptr;
                    if (blen < 1)
                    {
                        str_peeked_len = 0;
                        return nullptr;
                    }
                    else if (blen == 1)
                    {
                        str_peeked_len = 0;
                        return dst_buffer;
                    }
                    if (this->read_available() - peek_off < (blen + (::pilo::i64_t)sizeof(::pilo::i32_t)))
                    {
                        err = ::pilo::mk_perr(PERR_RD_PARTIAL_DATA);
                        str_peeked_len = 0;
                        return nullptr;
                    }

                    char* dst_buffer_ptr = this->peek_raw_bytes(sizeof(::pilo::i32_t) + peek_off, dst_buffer, dst_buffer_size, dst_pos, blen, err);
                    if (dst_buffer_ptr != nullptr || err != PILO_OK)
                    {
                        str_peeked_len = 0;
                        return nullptr;
                    }
                    str_peeked_len = (::pilo::i64_t)(blen - 1);
                    return dst_buffer_ptr;
                }

                virtual ::pilo::err_t write_cstring(const char* cstr, ::pilo::i64_t str_off, ::pilo::i64_t str_len)
                {
                    PMC_ASSERT(str_len < PILO_INT32_MAX);
                    if (str_len == -1)
                    {
                        if (cstr == nullptr)
                            return ::pilo::mk_perr(PERR_NULL_PARAM);
                        str_len = ::pilo::core::string::character_count(cstr) - str_off;
                    }

                    if (cstr != nullptr)
                    {

                        
                            if (str_len > 0)
                            {
                                if (this->write_int32((::pilo::i32_t)(str_len + 1)) == PILO_OK)
                                {
                                    if (this->write_raw_bytes(cstr, str_off, str_len) == PILO_OK)
                                    {
                                        if (this->write_int8(0) == PILO_OK)
                                        {
                                            return PILO_OK;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                if (this->write_int32((::pilo::i32_t)(1)) == PILO_OK)
                                {
                                    if (this->write_int8(0) == PILO_OK)
                                    {
                                        return PILO_OK;
                                    }
                                }
                            }
                            return ::pilo::mk_perr( PERR_IO_WRITE_FAIL);
                        
                    }

                    if (this->write_int32((::pilo::i32_t)(-1)) == PILO_OK)
                    {
                        return PILO_OK;
                    }

                    return ::pilo::mk_perr( PERR_IO_WRITE_FAIL);
                }
                virtual ::pilo::err_t set_cstring(::pilo::i64_t buffer_off, const char* cstr, ::pilo::i64_t str_off, ::pilo::i64_t str_len)
                {
                    PMC_ASSERT(str_len < PILO_INT32_MAX);
                    if (str_len == -1)
                    {
                        if (cstr == nullptr)
                            return ::pilo::mk_perr(PERR_NULL_PARAM);
                        str_len = ::pilo::core::string::character_count(cstr) - str_off;
                    }

                    if (cstr != nullptr)
                    {

                        
                            if (str_len > 0)
                            {
                                if (this->set_int32(buffer_off, (::pilo::i32_t)(str_len + 1)) == PILO_OK)
                                {
                                    if (this->set_raw_bytes(buffer_off, cstr, str_off, str_len) == PILO_OK)
                                    {
                                        if (this->set_int8(buffer_off, 0) == PILO_OK)
                                        {
                                            return PILO_OK;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                if (this->set_int32(buffer_off, (::pilo::i32_t)(1)) == PILO_OK)
                                {
                                    if (this->set_int8(buffer_off, 0) == PILO_OK)
                                    {
                                        return PILO_OK;
                                    }
                                }
                            }
                            return ::pilo::mk_perr( PERR_IO_WRITE_FAIL);
                        
                    }

                    if (this->set_int32(buffer_off, (::pilo::i32_t)(-1)) == PILO_OK)
                    {
                        return PILO_OK;
                    }

                    return ::pilo::mk_perr( PERR_IO_WRITE_FAIL);
                }

                virtual ::pilo::err_t read_string(std::string& str)
                {
                    char dst_buffer[64] = { 0 };
                    ::pilo::i32_t blen = 0;
                    ::pilo::err_t err = PILO_OK;
                    str.clear();
                    if (this->read_int32(blen) == PILO_OK)
                    {
                        if (blen > 0)
                        {
                            while ((::pilo::i64_t)blen > (::pilo::i64_t) sizeof(dst_buffer))
                            {
                                char* dst_buffer_ptr = this->read_raw_bytes(dst_buffer, sizeof(dst_buffer), 0, sizeof(dst_buffer), err);
                                if (dst_buffer_ptr == nullptr)
                                {
                                    return err;
                                }
                                str.append(dst_buffer, sizeof(dst_buffer));
                                blen -= sizeof(dst_buffer);
                            }
                            if (blen > 0)
                            {
                                char* dst_buffer_ptr = this->read_raw_bytes(dst_buffer, sizeof(dst_buffer), 0, blen, err);
                                if (dst_buffer_ptr == nullptr)
                                {
                                    return err;
                                }
                                str.append(dst_buffer, blen);
                            }
                            return PILO_OK;
                        }
                        else
                        {
                            str.append("");
                            return PILO_OK;
                        }
                    }

                    return ::pilo::mk_perr( PERR_IO_READ_FAIL);
                }
                virtual ::pilo::err_t peek_string(::pilo::i64_t peek_off, std::string& str)
                {
                    char dst_buffer[64] = { 0 };
                    ::pilo::i32_t blen = 0;
                    ::pilo::i64_t idx = 0;
                    ::pilo::err_t err = PILO_OK;
                    if (this->peek_int32(peek_off, blen) == PILO_OK)
                    {
                        if (blen > 0)
                        {
                            while ((::pilo::i64_t) blen > (::pilo::i64_t) sizeof(dst_buffer))
                            {
                                char* dst_buffer_ptr = this->peek_raw_bytes(peek_off + 4 + idx, dst_buffer, sizeof(dst_buffer), 0, sizeof(dst_buffer), err);
                                if (dst_buffer_ptr == nullptr)
                                {
                                    return err;
                                }
                                idx += sizeof(dst_buffer);
                                str.append(dst_buffer, sizeof(dst_buffer));
                                blen -= sizeof(dst_buffer);
                            }
                            if (blen > 0)
                            {
                                char* dst_buffer_ptr = this->peek_raw_bytes(peek_off + 4 + idx, dst_buffer, sizeof(dst_buffer), 0, blen, err);
                                if (dst_buffer_ptr == nullptr)
                                {
                                    return err;
                                }
                                str.append(dst_buffer, blen);
                            }
                            return PILO_OK;
                        }
                        else
                        {
                            str.append("");
                            return PILO_OK;
                        }

                    }

                    return ::pilo::mk_err( PERR_IO_READ_FAIL);
                }
                virtual ::pilo::err_t write_string(const std::string& str)
                {
                    return this->write_bytes(str.c_str(), 0, str.size());
                }
                virtual ::pilo::err_t set_string(::pilo::i64_t buffer_off, std::string& str)
                {
                    return this->set_cstring(buffer_off, str.c_str(), 0, str.size());
                }

            protected:
                buffer_node* _find_node(::pilo::i64_t begin_pos, ::pilo::i64_t& neo_pos)
                {
                    if (begin_pos >= this->_m_begin_pos + this->_m_length)
                    {
                        return nullptr;
                    }
                    buffer_node* node = this->_m_node_list.front();
                    if (node == nullptr)
                        return nullptr;
                    ::pilo::i64_t skip_nodes = begin_pos / piece_size();
                    neo_pos = begin_pos % piece_size();
                    while (skip_nodes > 0)
                    {                        
                        node = this->_m_node_list.next(node);
                        if (node == nullptr)
                        {
                            return nullptr;
                        }
                        skip_nodes--;
                    }
                    return node;
                }

                buffer_node* _get_next_node(buffer_node* cur_node)
                {
                    buffer_node* next_node = nullptr;
                    if (cur_node != nullptr)
                    {
                        next_node = this->_m_node_list.next(cur_node);
                        if (next_node == nullptr)
                        {
                            next_node = buffer_node::allocate();
                            if (next_node != nullptr)
                            {
                                this->_m_node_list.push_back(next_node);
                                this->_m_capacity += piece_size();
                            }
                            else
                                return nullptr;
                        }                        
                    }
                    else
                    {
                        next_node = buffer_node::allocate();
                        if (next_node != nullptr)
                        {
                            this->_m_node_list.push_back(next_node);
                            this->_m_capacity += piece_size();
                        }
                        else
                            return nullptr;
                    }   
                    return next_node;
                }
                
                ::pilo::err_t _set_raw_bytes_by_node(buffer_node* node, ::pilo::i64_t pos, const char* pbuf, ::pilo::i64_t offset, ::pilo::i64_t length)
                {
                    ::pilo::i64_t remain_bs_in_cur_block = piece_size() - pos;
                    if (remain_bs_in_cur_block >= length)
                    {
                        memcpy(node->data + pos, pbuf + offset , length);
                        return PILO_OK;
                    }
                    else
                    {
                        ::pilo::i64_t cur_turn_write_bytes = remain_bs_in_cur_block;
                        ::pilo::i64_t idx = 0;
                        while (length > 0)
                        {
                            memcpy(node->data + pos, pbuf + offset + idx, cur_turn_write_bytes);
                            idx += cur_turn_write_bytes;
                            length -= cur_turn_write_bytes;
                            if (length < 0)
                            {
                                return ::pilo::mk_perr(PERR_LEN_TOO_SAMLL);
                            }
                            else if (length == 0)
                            {
                                break;
                            }

                            if (node->m_next == nullptr)
                            {
                                return ::pilo::mk_perr(PERR_NULL_PTR);
                            }

                            node = this->_m_node_list.next(node);
                            pos = 0;
                            cur_turn_write_bytes = ::pilo::core::algorithm::pilo_min(piece_size(), length);
                        }
                    }
                    return PILO_OK;
                }

                ::pilo::err_t _clear_front()
                {
                    if (_m_node_list.front() != nullptr)
                    {                        
                        buffer_node* head = _m_node_list.pop_front();
                        PMC_ASSERT(head != nullptr);
                        buffer_node::deallocate(head);
                        head = nullptr;
                        _m_begin_pos -= piece_size();
                        _m_capacity -= piece_size();
                        if (_m_begin_pos < 0)
                        {
                            return ::pilo::mk_perr( PERR_INV_OFF);
                        }
                        return PILO_OK;
                    }

                    return ::pilo::mk_perr(PERR_NULL_PTR);
                }

                

                

            public:
                bool validate(std::string& info, ::pilo::i64_t cap, ::pilo::i64_t beg, ::pilo::i64_t len, ::pilo::i64_t list_size)
                {
                    std::stringstream ss;
                    if (this->_m_capacity != cap)
                    {
                        ss << "capa invliad expect " << cap << " got " << this->_m_capacity;
                        info = ss.str();
                        return false;
                    }
                    if (this->_m_begin_pos != beg)
                    {
                        ss << "beg invliad expect " << beg << " got " << this->_m_begin_pos;
                        info = ss.str();
                        return false;
                    }
                    if (this->_m_length != len)
                    {
                        ss << "len invliad expect " << len << " got " << this->_m_length;
                        info = ss.str();
                        return false;
                    }

                    if (this->_m_node_list.size() != list_size)
                    {
                        ss << "list_size invliad expect " << list_size << " got " << this->_m_node_list.size();
                        info = ss.str();
                        return false;
                    }
                    return true;
                }

            public:
                virtual ::pilo::err_t iterate(iterate_func_type iter, void* ctx, ::pilo::i64_t max_bytes, ::pilo::i64_t* out_bytes, bool ign_err)
                {
                    ::pilo::i64_t total_bytes = 0;
                    ::pilo::i64_t rpos = 0;
                    ::pilo::i64_t remain_len = _m_length < max_bytes ? _m_length : max_bytes;
                    buffer_node* p = _find_node(_m_begin_pos, rpos);
                    ::pilo::set_if_ptr_is_not_null(out_bytes, (::pilo::i64_t) 0);
                    if (p == nullptr)
                        return PILO_OK;

                    ::pilo::err_t err = PILO_OK;
                    ::pilo::i64_t cur_node_remain = 0;

                    for (; p != _m_node_list.end(); p = _m_node_list.next(p)) {
                        cur_node_remain = piece_size() - rpos;
                        if (cur_node_remain > remain_len)
                            cur_node_remain = remain_len;

                        err = iter(this, p->data + rpos , cur_node_remain, ctx);
                        if (err != PILO_OK)
                        {
                            if (!ign_err) {
                                return err;
                            }
                        }

                        total_bytes += cur_node_remain;
                        remain_len -= cur_node_remain;
                        rpos = 0; 
                    }

                    ::pilo::set_if_ptr_is_not_null(out_bytes, total_bytes);
                    return PILO_OK;
                }
                

            protected:
                ::pilo::i64_t _m_capacity;
                ::pilo::i64_t _m_begin_pos;
                ::pilo::i64_t _m_length;            
                buffer_node  * _m_saved_node_for_writing;
                ::pilo::i64_t _m_saved_pos_for_writing;
                mutable ::pilo::core::container::singly_linked_selflist<buffer_node, pilo::core::threading::dummy_mutex> _m_node_list;
            };

        }
    }
}

#endif
