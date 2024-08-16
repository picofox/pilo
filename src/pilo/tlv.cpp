#include "tlv.hpp"
#include <vector>
#include <deque>
#include "core/pattern/function_dispatcher.hpp"
#include "core/rtti/wired_type_conversion.hpp"
#include "core/crypto/base64_codec.hpp"
#include "core/memory/byte_buffer_interface.hpp"
#include "core/pattern/function_dispatcher.hpp"
#include "core/rtti/wired_type_map_comparation.hpp"
#include "core/rtti/wired_type_map_creation.hpp"
#include "core/rtti/wired_type_deque.hpp"
#include "core/process/context.hpp"


namespace pilo
{   
    std::ostream& operator<<(std::ostream& out, ::pilo::i8_t value) {
        return out << static_cast<int>(value);
    }
    std::ostream& operator<<(std::ostream& out, ::pilo::u8_t value) {
        return out << static_cast<unsigned int>(value);
    }


    template<typename TA_KEY, typename TA_VALUE>
    static ::pilo::err_t s_to_string_on_travel(::pilo::i64_t idx, ::pilo::u8_t key_type, const TA_KEY& key, ::pilo::u16_t vt, TA_VALUE& value, void* ctx)
    {
        std::stringstream& ss = *(std::stringstream*)ctx;

        if (key_type == ::pilo::core::rtti::wired_type::key_type_na || vt == ::pilo::core::rtti::wired_type::value_type_na)
        {
            return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
        }
        else
        {
            if (idx != 0)
                ss << ",";
            ss << "\"" << key << "\"";
        }
        ss << ":";
        if (vt == ::pilo::core::rtti::wired_type::value_type_boolean)
        {
            ss << std::boolalpha << value;
        }
        else if (vt > ::pilo::core::rtti::wired_type::value_type_na && vt < ::pilo::core::rtti::wired_type::value_type_bytes)
        {
            ss << value;
        }
        else if (vt == ::pilo::core::rtti::wired_type::value_type_str)
        {
            ss << "\"" << value << "\"";
        }
        else
        {
            return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
        }

        return PILO_OK;
    }

    template<typename TA_KEY>
    static ::pilo::err_t s_to_string_on_travel(::pilo::i64_t idx, ::pilo::u8_t key_type, const TA_KEY& key, ::pilo::u16_t vt, char*& value, void* ctx)
    {
        std::stringstream& ss = *(std::stringstream*)ctx;
        if (vt != ::pilo::core::rtti::wired_type::value_type_bytes)
        {
            return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
        }
        if (key_type == ::pilo::core::rtti::wired_type::key_type_na || vt == ::pilo::core::rtti::wired_type::value_type_na)
        {
            return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
        }
        else
        {
            if (idx != 0)
                ss << ",";
            ss << "\"" << key << "\"";
        }
        ss << ":";

        if (value == nullptr)
        {
            ss << "null";
        }
        else
        {
            ss << "\"" << value << "\"";
        }



        return PILO_OK;
    }

    template<typename TA_KEY>
    static ::pilo::err_t s_to_string_on_travel(::pilo::i64_t idx, ::pilo::u8_t key_type, const TA_KEY& key, ::pilo::u16_t vt, ::pilo::tlv*& value, void* ctx)
    {
        std::stringstream& ss = *(std::stringstream*)ctx;
        if (key_type == ::pilo::core::rtti::wired_type::key_type_na || vt == ::pilo::core::rtti::wired_type::value_type_na)
        {
            return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
        }
        else
        {
            if (idx != 0)
                ss << ",";
            ss << "\"" << key << "\"";
        }
        ss << ":";

        if (vt != ::pilo::core::rtti::wired_type::value_type_tlv)
        {
            return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
        }

        if (value == nullptr)
        {
            ss << "null";
            return PILO_OK;
        }

        return value->_to_string(ss);
    }

    template<typename TA_KEY, typename TA_VALUE>
    static ::pilo::err_t internal_map_travelsal(const ::pilo::tlv* t, ::pilo::u16_t func_id, bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx)
    {
        PMC_UNUSED(func_id);
        std::map<TA_KEY, TA_VALUE>* map_ptr = (std::map<TA_KEY, TA_VALUE>*) t->daynamic_data();
        PMC_ASSERT(map_ptr != nullptr);
        if (map_ptr == nullptr) return ::pilo::mk_perr(PERR_NULL_PTR);
        if (total_cnt != nullptr)
            *total_cnt = map_ptr->size();
        ::pilo::i64_t cnt = 0;
        ::pilo::err_t err = PILO_OK;
        auto cit = map_ptr->begin();
        for (; cit != map_ptr->end(); cit++)
        {
            if (func_id == PMI_TLV_TO_STRING)
            {
                err = s_to_string_on_travel<TA_KEY, TA_VALUE>(cnt, t->key_type(), cit->first, t->value_type(), (TA_VALUE&)cit->second, ctx);
                if (err != PILO_OK)
                {
                    if (!ignore_error)
                    {
                        if (travel_cnt != nullptr)
                            *travel_cnt = cnt;
                        return err;
                    }
                }
            }
            else
            {
                return ::pilo::mk_perr(PERR_INVALID_PARAM);
            }

            cnt++;
        }
        if (travel_cnt != nullptr)
            *travel_cnt = cnt;
        return PILO_OK;
    }

    template<typename TA_KEY>
    static ::pilo::err_t internal_map_travelsal_bytes(const ::pilo::tlv* t, ::pilo::u16_t func_id, bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx)
    {        
        std::map<TA_KEY, char*>* map_ptr = (std::map<TA_KEY, char*>*) t->daynamic_data();
        PMC_ASSERT(map_ptr != nullptr);
        if (map_ptr == nullptr) return ::pilo::mk_perr(PERR_NULL_PTR);
        if (total_cnt != nullptr)
            *total_cnt = map_ptr->size();
        ::pilo::i64_t cnt = 0;
        ::pilo::err_t err = PILO_OK;
        auto cit = map_ptr->begin();
        for (; cit != map_ptr->end(); cit++)
        {
            if (func_id == PMI_TLV_TO_STRING)
            {
                err = s_to_string_on_travel<TA_KEY>(cnt, t->key_type(), cit->first, t->value_type(), (char*&)cit->second, ctx);
                if (err != PILO_OK)
                {
                    if (!ignore_error)
                    {
                        if (travel_cnt != nullptr)
                            *travel_cnt = cnt;
                        return err;
                    }
                }
            }
            else
            {
                return ::pilo::mk_perr(PERR_INVALID_PARAM);
            }
            cnt++;
        }
        if (travel_cnt != nullptr)
            *travel_cnt = cnt;
        return PILO_OK;
    }

    template<typename TA_KEY>
    static ::pilo::err_t internal_map_travelsal_tlv(const ::pilo::tlv* t, ::pilo::u16_t func_id, bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx)
    {
        std::map<TA_KEY, ::pilo::tlv*>* map_ptr = (std::map<TA_KEY, ::pilo::tlv*>*) t->daynamic_data();
        PMC_ASSERT(map_ptr != nullptr);
        if (map_ptr == nullptr) return ::pilo::mk_perr(PERR_NULL_PTR);
        if (total_cnt != nullptr)
            *total_cnt = map_ptr->size();
        ::pilo::i64_t cnt = 0;
        ::pilo::err_t err = PILO_OK;
        auto cit = map_ptr->begin();
        for (; cit != map_ptr->end(); cit++)
        {
            if (func_id == PMI_TLV_TO_STRING)
            {
                err = s_to_string_on_travel<TA_KEY>(cnt, t->key_type(), cit->first, t->value_type(), (::pilo::tlv*&)cit->second, ctx);
                if (err != PILO_OK)
                {
                    if (!ignore_error)
                    {
                        if (travel_cnt != nullptr)
                            *travel_cnt = cnt;
                        return err;
                    }
                }
            }
            else
            {
                return ::pilo::mk_perr(PERR_INVALID_PARAM);
            }
            cnt++;
        }
        if (travel_cnt != nullptr)
            *travel_cnt = cnt;
        return PILO_OK;
    }


    ::pilo::err_t tlv::_set_cstr(const char* str, ::pilo::i32_t len, bool adopt)
    {
        _type.set_value_type(::pilo::core::rtti::wired_type::value_type_bytes);
        if (str == nullptr)
        {
            _dynamic_data = nullptr;
            _size = -1;
        }
        else if (*str == 0)
        {
            _dynamic_data = ::pilo::core::string::duplicate("", 0);
            if (_dynamic_data == nullptr)
            {
                return ::pilo::mk_perr( PERR_INSUF_HEAP);
            }
            _size = 1;
        }
        else
        {
            if (adopt)
            {
                _dynamic_data = (char*)str;
                if (len == -1)
                    _size = (::pilo::i32_t) ::pilo::core::string::character_count(str) + 1;
                else
                    _size = len + 1;
            }
            else
            {
                if (len == -1)
                    len = (::pilo::i32_t) ::pilo::core::string::character_count(str);

                _dynamic_data = (char*)PMF_HEAP_MALLOC(len + 1);
                if (_dynamic_data == nullptr)
                {
                    return ::pilo::mk_perr( PERR_INSUF_HEAP);
                }
                _dynamic_data[len] = 0;
                ::pilo::core::string::n_copy(_dynamic_data, len + 1, str, len);
                _size = len + 1;
            }
        }
        this->set_flag(::pilo::tlv::FlagBytesAsCStr);
        return PILO_OK;
    }


    void stc_wrapper_clear_na(::pilo::tlv, bool ) {  }
    void stc_wrapper_clear_i8(::pilo::tlv* t, bool compact) { t->_wrapper_clear<::pilo::i8_t>(compact); }
    void stc_wrapper_clear_u8(::pilo::tlv* t, bool compact) { t->_wrapper_clear<::pilo::u8_t>(compact); }
    void stc_wrapper_clear_i16(::pilo::tlv* t, bool compact) { t->_wrapper_clear<::pilo::i16_t>(compact); }
    void stc_wrapper_clear_u16(::pilo::tlv* t, bool compact) { t->_wrapper_clear<::pilo::u16_t>(compact); }
    void stc_wrapper_clear_i32(::pilo::tlv* t, bool compact) { t->_wrapper_clear<::pilo::i32_t>(compact); }
    void stc_wrapper_clear_u32(::pilo::tlv* t, bool compact) { t->_wrapper_clear<::pilo::u32_t>(compact); }
    void stc_wrapper_clear_i64(::pilo::tlv* t, bool compact) { t->_wrapper_clear<::pilo::i64_t>(compact); }
    void stc_wrapper_clear_u64(::pilo::tlv* t, bool compact) { t->_wrapper_clear<::pilo::u64_t>(compact); }
    void stc_wrapper_clear_bool(::pilo::tlv* t, bool compact) { t->_wrapper_clear<bool>(compact); }
    void stc_wrapper_clear_f32(::pilo::tlv* t, bool compact) { t->_wrapper_clear<::pilo::f32_t>(compact); }
    void stc_wrapper_clear_f64(::pilo::tlv* t, bool compact) { t->_wrapper_clear<::pilo::f64_t>(compact); }
    void stc_wrapper_clear_bytes(::pilo::tlv* t, bool compact) { t->_wrapper_clear<char*>(compact); }    
    void stc_wrapper_clear_str(::pilo::tlv* t, bool compact) { t->_wrapper_clear<std::string>(compact);  }
    void stc_wrapper_clear_tlv(::pilo::tlv* t, bool compact) { t->_wrapper_clear<::pilo::tlv*>(compact); }

    static ::pilo::core::pattern::function_dispatcher<void(::pilo::tlv*, bool), ::pilo::core::rtti::wired_type::value_type_intrincs_count> stc_wrapper_cleaner_dispatcher(
        nullptr, stc_wrapper_clear_i8, stc_wrapper_clear_u8, stc_wrapper_clear_i16
        , stc_wrapper_clear_u16, stc_wrapper_clear_i32, stc_wrapper_clear_u32, stc_wrapper_clear_i64
        , stc_wrapper_clear_u64, stc_wrapper_clear_bool, stc_wrapper_clear_f32, stc_wrapper_clear_f64
        , stc_wrapper_clear_bytes, stc_wrapper_clear_str, stc_wrapper_clear_tlv
    );


    static ::pilo::core::pattern::function_dispatcher<::pilo::err_t(const ::pilo::tlv*, ::pilo::core::memory::byte_buffer_interface*), ::pilo::core::rtti::wired_type::value_type_intrincs_count> stc_single_serialzer_dispatcher
    (
        [](const ::pilo::tlv* , ::pilo::core::memory::byte_buffer_interface* ) -> ::pilo::err_t { return PILO_OK; },//0
        [](const ::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) -> ::pilo::err_t { if (bb->write_int8(t->int8()) != PILO_OK) { return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); } return PILO_OK; },//1
        [](const ::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) -> ::pilo::err_t { if (bb->write_uint8(t->uint8()) != PILO_OK) { return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); } return PILO_OK; },//2
        [](const ::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) -> ::pilo::err_t { if (bb->write_int16(t->int16()) != PILO_OK) { return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); } return PILO_OK; },//3
        [](const ::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) -> ::pilo::err_t { if (bb->write_uint16(t->uint16()) != PILO_OK) { return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); } return PILO_OK; },//4
        [](const ::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) -> ::pilo::err_t { if (bb->write_int32(t->int32()) != PILO_OK) { return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); } return PILO_OK; },//5
        [](const ::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) -> ::pilo::err_t { if (bb->write_uint32(t->uint32()) != PILO_OK) { return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); } return PILO_OK; },  //6     
        [](const ::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) -> ::pilo::err_t { if (bb->write_int64(t->int64()) != PILO_OK) { return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); } return PILO_OK; },//7
        [](const ::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) -> ::pilo::err_t { if (bb->write_uint64(t->uint64()) != PILO_OK) { return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); } return PILO_OK; },//8
        [](const ::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) -> ::pilo::err_t { if (bb->write_uint8(t->uint8()) != PILO_OK) { return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); } return PILO_OK; },//9
        [](const ::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) -> ::pilo::err_t { if (bb->write_float32(t->float32()) != PILO_OK) { return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); } return PILO_OK; },//10
        [](const ::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) -> ::pilo::err_t { if (bb->write_float64(t->float64()) != PILO_OK) { return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); } return PILO_OK; },//11
        [](const ::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) -> ::pilo::err_t {
            if (bb->write_bytes(t->daynamic_data(), 0,  t->size()) != PILO_OK) {
                return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); 
            }
            return PILO_OK;
        },//12        
        [](const ::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) -> ::pilo::err_t {
            const std::string* p = t->string_ptr();
            if (p != nullptr)
            {
                if (p->size() == 0)
                {
                    if (bb->write_int32(0) != PILO_OK) {
                        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
                    }
                }
                else
                {
                    if (bb->write_bytes(p->c_str(), 0, p->size()) != PILO_OK) {
                        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
                    }
                }                
            }
            else
            {
                if (bb->write_int32(-1) != PILO_OK) {
                    return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); 
                }
            }            
            return PILO_OK;
        },
        [](const ::pilo::tlv*, ::pilo::core::memory::byte_buffer_interface*) -> ::pilo::err_t { PMC_ASSERT(false); return PILO_OK; }//13
    );

    ::pilo::err_t single_tlv_deserialize_na(::pilo::tlv*, ::pilo::core::memory::byte_buffer_interface*) { return PILO_OK;  }
    ::pilo::err_t single_tlv_deserialize_i8(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) { if (bb->read_int8(t->_i8) != PILO_OK) { return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); } return PILO_OK; }
    ::pilo::err_t single_tlv_deserialize_u8(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) { if (bb->read_uint8(t->_u8) != PILO_OK) { return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); } return PILO_OK; }//2
    ::pilo::err_t single_tlv_deserialize_i16(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) { if (bb->read_int16(t->_i16) != PILO_OK) { return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); } return PILO_OK; }//3
    ::pilo::err_t single_tlv_deserialize_u16(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) { if (bb->read_uint16(t->_u16) != PILO_OK) { return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); } return PILO_OK; }//4
    ::pilo::err_t single_tlv_deserialize_i32(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) { if (bb->read_int32(t->_i32) != PILO_OK) { return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); } return PILO_OK; }//5
    ::pilo::err_t single_tlv_deserialize_u32(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) { if (bb->read_uint32(t->_u32) != PILO_OK) { return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); } return PILO_OK; }  //6     
    ::pilo::err_t single_tlv_deserialize_i64(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) { if (bb->read_int64(t->_i64) != PILO_OK) { return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); } return PILO_OK; }//7
    ::pilo::err_t single_tlv_deserialize_u64(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) { if (bb->read_uint64(t->_u64) != PILO_OK) { return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); } return PILO_OK; }//8
    ::pilo::err_t single_tlv_deserialize_bool(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) { if (bb->read_uint8(t->_u8) != PILO_OK) { return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); } return PILO_OK; }//9
    ::pilo::err_t single_tlv_deserialize_f32(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb) { if (bb->read_float32(t->_f32) != PILO_OK) { return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); } return PILO_OK; }//10
    ::pilo::err_t single_tlv_deserialize_f64(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb)  { if (bb->read_float64(t->_f64) != PILO_OK) { return ::pilo::mk_perr(PERR_IO_WRITE_FAIL); } return PILO_OK; }//11
    ::pilo::err_t single_tlv_deserialize_bytes(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb)//12
    {
        ::pilo::err_t err = PILO_OK;
        ::pilo::i64_t sz = 0;
        char* pdata = bb->read_bytes(nullptr, 0, 0, sz, err);
        if (err != PILO_OK) {
            return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
        }
        return t->set_bytes(pdata, (::pilo::i32_t)sz, true);
    }
    ::pilo::err_t single_tlv_deserialize_str(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb)
    {
        ::pilo::err_t err = PILO_OK;
        ::pilo::i64_t sz = 0;
        char* pdata = bb->read_bytes(nullptr, 0, 0, sz, err);
        if (err != PILO_OK) {
            return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
        }
        t->set_string(pdata, sz);
        if (pdata != nullptr)
            PMF_HEAP_FREE(pdata);
        return PILO_OK;

    }
    ::pilo::err_t single_tlv_deserialize_tlv(::pilo::tlv*, ::pilo::core::memory::byte_buffer_interface*) { PMC_ASSERT(false); return PILO_OK; }

    static ::pilo::core::pattern::function_dispatcher<::pilo::err_t(::pilo::tlv*, ::pilo::core::memory::byte_buffer_interface*), ::pilo::core::rtti::wired_type::value_type_intrincs_count> stc_single_deserialzer_dispatcher
    (
        single_tlv_deserialize_na, single_tlv_deserialize_i8, single_tlv_deserialize_u8, single_tlv_deserialize_i16, single_tlv_deserialize_u16, single_tlv_deserialize_i32, single_tlv_deserialize_u32, single_tlv_deserialize_i64, single_tlv_deserialize_u64
        , single_tlv_deserialize_bool, single_tlv_deserialize_f32, single_tlv_deserialize_f64, single_tlv_deserialize_bytes, single_tlv_deserialize_str, single_tlv_deserialize_tlv
    );
    

    typedef ::pilo::err_t(*handle_map_travel_func_type)(const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*, ::pilo::i64_t*, void*);
    static handle_map_travel_func_type stc_handle_map_travel_func[::pilo::core::rtti::wired_type::key_type_count][::pilo::core::rtti::wired_type::value_type_intrincs_count] = {
        {//0
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); },
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); },
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); },
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); },
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); },
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); },
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); },
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); },
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); },
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); },
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); },
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); },
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); },
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); },
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); }
        },
        {//1
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); }, //0
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i8_t, ::pilo::i8_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//1
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i8_t, ::pilo::u8_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//2
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i8_t, ::pilo::i16_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//3
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i8_t, ::pilo::u16_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//4
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i8_t, ::pilo::i32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//5
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i8_t, ::pilo::u32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//6
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i8_t, ::pilo::i64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//7
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i8_t, ::pilo::u64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//8
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i8_t, bool>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//9
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i8_t, ::pilo::f32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//10
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i8_t, ::pilo::f64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//11
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal_bytes<::pilo::i8_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//12
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i8_t, std::string>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//13
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal_tlv<::pilo::i8_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//14
        },
        {//2
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); }, //0
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u8_t, ::pilo::i8_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//1
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u8_t, ::pilo::u8_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//2
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u8_t, ::pilo::i16_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//3
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u8_t, ::pilo::u16_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//4
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u8_t, ::pilo::i32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//5
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u8_t, ::pilo::u32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//6
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u8_t, ::pilo::i64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//7
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u8_t, ::pilo::u64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//8
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u8_t, bool>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//9
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u8_t, ::pilo::f32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//10
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u8_t, ::pilo::f64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//11
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal_bytes<::pilo::u8_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//12
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u8_t, std::string>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//13
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal_tlv<::pilo::u8_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//14
        },
        {//3
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); }, //0
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i16_t, ::pilo::i8_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//1
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i16_t, ::pilo::u8_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//2
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i16_t, ::pilo::i16_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//3
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i16_t, ::pilo::u16_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//4
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i16_t, ::pilo::i32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//5
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i16_t, ::pilo::u32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//6
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i16_t, ::pilo::i64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//7
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i16_t, ::pilo::u64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//8
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i16_t, bool>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//9
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i16_t, ::pilo::f32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//10
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i16_t, ::pilo::f64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//11
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal_bytes<::pilo::i16_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//12
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i16_t, std::string>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//13
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal_tlv<::pilo::i16_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//14
        },
        {//4
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); }, //0
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u16_t, ::pilo::i8_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//1
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u16_t, ::pilo::u8_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//2
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u16_t, ::pilo::i16_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//3
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u16_t, ::pilo::u16_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//4
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u16_t, ::pilo::i32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//5
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u16_t, ::pilo::u32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//6
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u16_t, ::pilo::i64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//7
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u16_t, ::pilo::u64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//8
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u16_t, bool>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//9
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u16_t, ::pilo::f32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//10
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u16_t, ::pilo::f64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//11
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal_bytes<::pilo::u16_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//12
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u16_t, std::string>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//13
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal_tlv<::pilo::u16_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//14
        },
        {//5
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); }, //0
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i32_t, ::pilo::i8_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//1
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i32_t, ::pilo::u8_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//2
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i32_t, ::pilo::i16_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//3
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i32_t, ::pilo::u16_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//4
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i32_t, ::pilo::i32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//5
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i32_t, ::pilo::u32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//6
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i32_t, ::pilo::i64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//7
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i32_t, ::pilo::u64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//8
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i32_t, bool>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//9
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i32_t, ::pilo::f32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//10
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i32_t, ::pilo::f64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//11
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal_bytes<::pilo::i32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//12
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i32_t, std::string>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//13
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal_tlv<::pilo::i32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//14
        },
        {//6
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); }, //0
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u32_t, ::pilo::i8_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//1
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u32_t, ::pilo::u8_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//2
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u32_t, ::pilo::i16_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//3
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u32_t, ::pilo::u16_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//4
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u32_t, ::pilo::i32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//5
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u32_t, ::pilo::u32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//6
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u32_t, ::pilo::i64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//7
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u32_t, ::pilo::u64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//8
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u32_t, bool>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//9
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u32_t, ::pilo::f32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//10
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u32_t, ::pilo::f64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//11
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal_bytes<::pilo::u32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//12
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u32_t, std::string>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//13
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal_tlv<::pilo::u32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//14
        },
        {//7
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); }, //0
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i64_t, ::pilo::i8_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//1
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i64_t, ::pilo::u8_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//2
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i64_t, ::pilo::i16_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//3
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i64_t, ::pilo::u16_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//4
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i64_t, ::pilo::i32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//5
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i64_t, ::pilo::u32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//6
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i64_t, ::pilo::i64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//7
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i64_t, ::pilo::u64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//8
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i64_t, bool>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//9
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i64_t, ::pilo::f32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//10
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i64_t, ::pilo::f64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//11
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal_bytes<::pilo::i64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//12
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::i64_t, std::string>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//13
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal_tlv<::pilo::i64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//14
        },
        {//8
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); }, //0
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u64_t, ::pilo::i8_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//1
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u64_t, ::pilo::u8_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//2
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u64_t, ::pilo::i16_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//3
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u64_t, ::pilo::u16_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//4
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u64_t, ::pilo::i32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//5
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u64_t, ::pilo::u32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//6
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u64_t, ::pilo::i64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//7
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u64_t, ::pilo::u64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//8
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u64_t, bool>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//9
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u64_t, ::pilo::f32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//10
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u64_t, ::pilo::f64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//11
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal_bytes<::pilo::u64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//12
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<::pilo::u64_t, std::string>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//13
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal_tlv<::pilo::u64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//14
        },
        {//9
            [](const ::pilo::tlv*, ::pilo::u16_t,  bool, ::pilo::i64_t*,::pilo::i64_t*, void* ) -> ::pilo::err_t { return ::pilo::mk_perr(PERR_MIS_DATA_TYPE); }, //0
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<std::string, ::pilo::i8_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//1
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<std::string, ::pilo::u8_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//2
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<std::string, ::pilo::i16_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//3
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<std::string, ::pilo::u16_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//4
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<std::string, ::pilo::i32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//5
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<std::string, ::pilo::u32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//6
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<std::string, ::pilo::i64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//7
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<std::string, ::pilo::u64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//8
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<std::string, bool>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//9
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<std::string, ::pilo::f32_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//10
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<std::string, ::pilo::f64_t>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//11
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal_bytes<std::string>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//12
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal<std::string, std::string>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//13
            [](const ::pilo::tlv* t, ::pilo::u16_t func_id,  bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) -> ::pilo::err_t { return internal_map_travelsal_tlv<std::string>(t, func_id, ignore_error, total_cnt, travel_cnt, ctx); },//14
        },
    };


    

    

    ::pilo::err_t tlv::_set_bytes(const char* bs, ::pilo::i32_t len, bool adopt)
    {
        _type.set_value_type(::pilo::core::rtti::wired_type::value_type_bytes);
        if (bs == nullptr)
        {
            _dynamic_data = nullptr;
            _size = 0;
        }
        else if (len == 0)
        {
            _dynamic_data = nullptr;
            _size = 0;
        }
        else
        {
            if (len < 0)
            {
                len = (::pilo::i32_t) ::pilo::core::string::character_count(bs);
                if (len <= 0)
                {
                    _dynamic_data = nullptr;
                    _size = 0;
                }
                if (adopt)
                {
                    _dynamic_data = (char*)bs;
                    _size = len;
                }
                else
                {
                    _dynamic_data = (char*)PMF_HEAP_MALLOC(len + 1);
                    if (_dynamic_data == nullptr)
                    {
                        return ::pilo::mk_perr( PERR_INSUF_HEAP);
                    }
                    _dynamic_data[len] = 0;
                    memcpy(_dynamic_data, bs, len);
                    _size = len + 1;
                    _dynamic_data[len] = 0;
                }

            }
            else
            {
                if (adopt)
                {
                    _dynamic_data = (char*)bs;
                    _size = len;
                }
                else
                {
                    _dynamic_data = (char*)PMF_HEAP_MALLOC(len);
                    if (_dynamic_data == nullptr)
                    {
                        return ::pilo::mk_perr( PERR_INSUF_HEAP);
                    }
                    memcpy(_dynamic_data, bs, len);
                    _size = len;
                }
            }            
        }
        return PILO_OK;
    }

    void tlv::wrapper_clear(bool compact)
    {
        if (this->_type.value_type() < ::pilo::core::rtti::wired_type::value_type_intrincs_count)
        {
            stc_wrapper_cleaner_dispatcher.dispatch(this->_type.value_type())(this, compact);
        }
    }

    ::pilo::err_t tlv::serialize(::pilo::core::memory::serializable_header_interface* header, ::pilo::core::memory::byte_buffer_interface* byte_buffer) const
    {
        ::pilo::i64_t header_begin_pos = byte_buffer->write_pos();
        ::pilo::err_t ret = PILO_OK;

        if (header != nullptr)
        {
            ret = header->begin_serializing(byte_buffer);
            if (ret != PILO_OK)
            {
                return ret;
            }
        }

        ret = byte_buffer->write_int32(_size);
        if (ret != PILO_OK)
        {
            return ret;
        }
        ret = byte_buffer->write_uint8(_type.attribute());
        if (ret != PILO_OK)
        {
            return ret;
        }
        ret = byte_buffer->write_uint8(_type.flags());
        if (ret != PILO_OK)
        {
            return ret;
        }
        ret = byte_buffer->write_uint16(_type.value_type());
        if (ret != PILO_OK)
        {
            return ret;
        }

        if (_type.wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_single)
        {
            ret = stc_single_serialzer_dispatcher.dispatch(_type.value_type())(this, byte_buffer);
            if (ret != PILO_OK)
                return ret;
        }
        else if (_type.wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_array)
        {
            ret = byte_buffer->write_wired_rac(_type.value_type(), this->_dynamic_data);
            if (ret != PILO_OK)
                return ret;
        }
        else if (_type.wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_dict)
        {
            ret = byte_buffer->write_wired_map(_type.key_type(), _type.value_type(), this->_dynamic_data);
            if (ret != PILO_OK)
                return ret;
        }
        else
        {
            PMC_ASSERT(false);
        }

        if (header != nullptr)
        {
            ret = header->end_serializing(byte_buffer, header_begin_pos, byte_buffer->write_pos() - header_begin_pos - header->header_length());
            if (ret != PILO_OK)
            {
                return ret;
            }
        }
        return PILO_OK;
    }

    ::pilo::err_t tlv::deserialize(::pilo::core::memory::serializable_header_interface* header, ::pilo::core::memory::byte_buffer_interface* byte_buffer)
    {
        this->reset();

        ::pilo::i64_t initial_length = byte_buffer->read_available();
        ::pilo::err_t ret = PILO_OK;
        //::pilo::i64_t rlen = 0;
        //void* rc_ptr = nullptr;

        if (header != nullptr)
        {
            ret = header->begin_deserializing(byte_buffer);
            if (ret != PILO_OK)
            {
                return ret;
            }
        }

        if (byte_buffer->read_int32(this->_size) != PILO_OK)
        {
            return ::pilo::mk_perr(PERR_IO_READ_FAIL);
        }
        ::pilo::u8_t attr = 0;
        ::pilo::u8_t flags = 0;
        ::pilo::u16_t vtype = 0;
        if (byte_buffer->read_uint8(attr) != PILO_OK)
        {
            return ::pilo::mk_perr(PERR_IO_READ_FAIL);
        }
        if (byte_buffer->read_uint8(flags) != PILO_OK)
        {
            return ::pilo::mk_perr(PERR_IO_READ_FAIL);
        }
        if (byte_buffer->read_uint16(vtype) != PILO_OK)
        {
            return ::pilo::mk_perr(PERR_IO_READ_FAIL);
        }
        _type.set(attr, flags, vtype);

        if (_type.wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_single)
        {
            ret = stc_single_deserialzer_dispatcher.dispatch(_type.value_type())(this, byte_buffer);
            if (ret != PILO_OK)
                return ret;
        }
        else if (_type.wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_array)
        {
            if (this->_dynamic_data != nullptr)
                this->_clear_dynamic_data();
            this->_dynamic_data = byte_buffer->read_wired_rac(ret);
            if (ret != PILO_OK)
                return ret;
        }
        else if (_type.wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_dict)
        {
            if (this->_dynamic_data != nullptr)
                this->_clear_dynamic_data();
            this->_dynamic_data = byte_buffer->read_wired_map(ret);
            if (ret != PILO_OK)
                return ret;
        }
        else
        {
            PMC_ASSERT(false);
        }

        if (header != nullptr)
        {
            ret = header->end_deserializing(byte_buffer, initial_length - byte_buffer->read_available() - header->header_length());
            if (ret != PILO_OK)
            {
                return ret;
            }
        }

        return PILO_OK;
    }
        

    ::pilo::err_t tlv::_internal_map_travelsal(::pilo::u16_t func_id, bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) const
    {
        if (this->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_dict)
        {
            return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
        }

        if (this->key_type() >= ::pilo::core::rtti::wired_type::key_type_count)
        {
            return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
        }

        if (this->value_type() <= ::pilo::core::rtti::wired_type::value_type_tlv)
        {
            return stc_handle_map_travel_func[this->key_type()][this->value_type()](this, func_id, ignore_error, total_cnt, travel_cnt, ctx);
        }

        return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
    }

    ::pilo::err_t tlv::_to_string(std::stringstream& ss) const
    {
        ::pilo::err_t err = PILO_OK;
        if (this->_type.wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_single)
        {
            ss << "{";
            ss << "\"s\":" << this->_size << ",";
            this->_type.to_string(ss);
            ss << ",\"v\":";
            if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_na)
            {
                ss << "\"\"";
            }
            else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_boolean)
            {
                ss << (_i8 == 0 ? "false" : "true");
            }
            else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i8)
            {
                ss << (int) _i8;
            }
            else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u8)
            {
                ss << (int) _u8;
            }
            else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i16)
            {
                ss << _i16;
            }
            else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u16)
            {
                ss << _u16;
            }
            else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i32)
            {
                ss << _i32;
            }
            else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u32)
            {
                ss << _u32;
            }
            else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i64)
            {
                ss << _i64;
            }
            else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u64)
            {
                ss << _u64;
            }
            else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f32)
            {
                ss << _f32;
            }
            else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f64)
            {
                ss << _f64;
            }
            else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_str)
            {
                if (_dynamic_data != nullptr)
                {
                    char buffer[256] = { 0 };
                    ::pilo::i64_t rlen = 0;
                    ss << "\"";
                    char* ret = ::pilo::core::crypto::base64_codec::s_encode(buffer, sizeof(buffer), rlen, ((std::string*)_dynamic_data)->c_str(), ((std::string*)_dynamic_data)->size(), &err, nullptr, 0);
                    if (nullptr != ret)
                    {
                        ss << ret;
                        if (ret != buffer)
                            PMF_HEAP_FREE(ret);
                    }
                    ss << "\"";
                }
                else
                {
                    ss << "null";
                }

                
            }
            else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
            {
                if (this->_dynamic_data == nullptr)
                {
                    ss << "null";
                }
                else
                {
                    char buffer[256] = { 0 };
                    ::pilo::i64_t rlen = 0;
                    ss << "\"";
                    char* ret = ::pilo::core::crypto::base64_codec::s_encode(buffer, sizeof(buffer), rlen, this->_dynamic_data, this->_size, &err, nullptr, 0);
                    if (nullptr != ret)
                    {
                        ss << ret;
                        if (ret != buffer)
                            PMF_HEAP_FREE(ret);
                    }
                    ss << "\"";
                }                           
            }
            else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_tlv)
            {
                if (this->_dynamic_data == nullptr)
                {
                    ss << "null";
                }
                else
                {
                    err = _to_string(ss);
                }
                
            }
            else
            {
                
            }
            ss << "}";
        }
        else if (this->_type.wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_array)
        {
            ss << "{";
            ss << "\"s\":" << this->_size << ",";
            this->_type.to_string(ss);
            if (this->_dynamic_data == nullptr)
            {
                ss << ",\"v\":[]}";
            }
            else
            {
                ss << ",\"v\":[";
                for (::pilo::i32_t i = 0; i < _size; i++)
                {
                    if (i > 0)
                    {
                        ss << ",";
                    }
                    if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_na)
                    {
                        ss << "\"\"";
                    }
                    else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_boolean)
                    {
                        ss << (this->get<bool>(i, &err) ? "true" : "false");
                    }
                    else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i8)
                    {
                        ss << (int)this->get<::pilo::i8_t>(i, &err);
                    }
                    else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u8)
                    {
                        ss << (unsigned int)this->get<::pilo::u8_t>(i, &err);
                    }
                    else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i16)
                    {
                        ss << this->get<::pilo::i16_t>(i, &err);
                    }
                    else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u16)
                    {
                        ss << this->get<::pilo::u16_t>(i, &err);
                    }
                    else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i32)
                    {
                        ss << this->get<::pilo::i32_t>(i, &err);
                    }
                    else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u32)
                    {
                        ss << this->get<::pilo::u32_t>(i, &err);
                    }
                    else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i64)
                    {
                        ss << this->get<::pilo::i64_t>(i, &err);
                    }
                    else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u64)
                    {
                        ss << this->get<::pilo::u64_t>(i, &err);
                    }
                    else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f32)
                    {
                        ss << this->get<::pilo::f32_t>(i, &err);
                    }
                    else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f64)
                    {
                        ss << this->get<::pilo::f64_t>(i, &err);
                    }
                    else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_str)
                    {
                        std::string str0 = this->get<std::string>(i, &err);
                        char buffer[256] = { 0 };
                        ::pilo::i64_t rlen = 0;
                        ss << "\"";
                        char* ret = ::pilo::core::crypto::base64_codec::s_encode(buffer, sizeof(buffer), rlen, str0.c_str(), str0.size(), &err, nullptr, 0);
                        if (nullptr != ret)
                        {
                            ss << ret;
                            if (ret != buffer)
                                PMF_HEAP_FREE(ret);
                        }
                        ss << "\"";

                    }
                    else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
                    {
                        char* ptr = this->get<char*>(i, &err);

                        if (ptr == nullptr)
                            ss << "\"\"";
                        else
                        {
                            char buffer[256] = { 0 };
                            ::pilo::i64_t rlen = 0;
                            ss << "\"";
                            char* ret = ::pilo::core::crypto::base64_codec::s_encode(buffer, sizeof(buffer), rlen, ptr, -1, &err, nullptr, 0);
                            if (nullptr != ret)
                            {
                                ss << ret;
                                if (ret != buffer)
                                    PMF_HEAP_FREE(ret);
                            }
                            ss << "\"";
                        }
                    }
                    else if (this->_type.value_type() == ::pilo::core::rtti::wired_type::value_type_tlv)
                    {
                        this->get<tlv*>(i, &err)->_to_string(ss);
                    }
                    else
                    {
                        ss << "<invalid value type>" << ",";
                    }
                }
                ss << "]";
                ss << "}";
            }            
        }
        else if (this->_type.wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_dict)
        {
            ss << "{";
            ss << "\"s\":" << this->_size << ",";
            this->_type.to_string(ss);
            if (this->_dynamic_data == nullptr)
            {
                ss << ",\"v\":{}}";
            }
            else
            {
                ss << ",\"v\":{";
                err = this->_internal_map_travelsal(0, false, nullptr, nullptr, (void*) &ss);                
                ss << "}";
                ss << "}";
                if (err != PILO_OK)
                {
                    return err;
                }
            }            
        }
        else
        {
            ss << "[Invalid Wrapper Type]";
        }

        return PILO_OK;
      
    }

    bool tlv::as_bool(::pilo::err_t* err) const
    {
        if (_type.is_single())
        {
            if (_type.value_type_in_range(::pilo::core::rtti::wired_type::value_type_i8, ::pilo::core::rtti::wired_type::value_type_boolean))
            {
                ::pilo::i8_t bv = this->as_i8(err);
                return bv == 0 ? false : true;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_na)
            {
                
                return false;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_boolean)
            {
                
                return _i8 == 0 ? false : true;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f32)
            {
                
                return (::pilo::i8_t)_f32;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f64)
            {
                
                return (::pilo::i8_t)_f64;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_str)
            {
                return ::pilo::core::rtti::convert_to_bool((::std::string*)this->_dynamic_data, err);
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
            {
                return ::pilo::core::rtti::convert_to_bool(this->_dynamic_data, err, this->_size);
            }
        }

        if (err != nullptr) *err = ::pilo::mk_perr(PERR_INC_DATA);
        return 0;
    }

    ::pilo::i8_t tlv::as_i8(::pilo::err_t * err) const
    {
        if (_type.is_single())
        {
            if (_type.value_type_in_range(::pilo::core::rtti::wired_type::value_type_i8, ::pilo::core::rtti::wired_type::value_type_f32))
            {
                return this->_i8;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_na)
            {
                return 0;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f32)
            {
                return (::pilo::i8_t)_f32;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f64)
            {
                return (::pilo::i8_t)_f64;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_str)
            {
                ::pilo::i64_t iv = 0;
                ::pilo::err_t e = 0;
                e = ::pilo::core::string::string_to_number(iv, ((std::string*)this->_dynamic_data)->c_str(), ((std::string*)this->_dynamic_data)->size());
                if (e != PILO_OK)
                {
                    if (err != nullptr) *err = e;
                    return 0;
                }
                if (iv < PILO_INT8_MIN)
                {
                    if (err != nullptr) *err = ::pilo::mk_perr(PERR_VAL_TOO_SAMLL);
                    return PILO_INT8_MIN;
                }
                else if (iv > PILO_INT8_MAX)
                {
                    if (err != nullptr) *err = ::pilo::mk_perr(PERR_VAL_TOO_LARGE);
                    return PILO_INT8_MAX;
                }
                return (::pilo::i8_t)iv; 
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
            {
                ::pilo::i64_t iv = 0;
                ::pilo::err_t e = 0;
                if (this->_dynamic_data == nullptr || this->_size <= 1)
                {
                    return 0;
                }
                e = ::pilo::core::string::string_to_number(iv, this->_dynamic_data, this->_size - 1);
                if (e != PILO_OK)
                {
                    if (err != nullptr) *err = e;
                    return 0;
                }
                if (iv < PILO_INT8_MIN)
                {
                    if (err != nullptr) *err = ::pilo::mk_perr(PERR_VAL_TOO_SAMLL);
                    return PILO_INT8_MIN;
                }
                else if (iv > PILO_INT8_MAX)
                {
                    if (err != nullptr) *err = ::pilo::mk_perr(PERR_VAL_TOO_LARGE);
                    return PILO_INT8_MAX;
                }
                return (::pilo::i8_t)iv;
            }
        }

        if (err != nullptr) *err = ::pilo::mk_perr(PERR_INC_DATA);
        return 0;
    }

    ::pilo::u8_t tlv::as_u8(::pilo::err_t* err) const
    {
        if (_type.is_single())
        {
            if (_type.value_type_in_range(::pilo::core::rtti::wired_type::value_type_i8, ::pilo::core::rtti::wired_type::value_type_f32))
            {
                return this->_u8;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_na)
            {
                return 0;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f32)
            {
                
                return (::pilo::u8_t)_f32;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f64)
            {
                
                return (::pilo::u8_t)_f64;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_str)
            {
                ::pilo::i64_t iv = 0;
                ::pilo::err_t e = 0;
                e = ::pilo::core::string::string_to_number(iv, ((std::string*)this->_dynamic_data)->c_str(), ((std::string*)this->_dynamic_data)->size());
                if (e != PILO_OK)
                {
                    if (err != nullptr) *err = e;
                    return 0;
                }
                else if (iv > PILO_UINT8_MAX)
                {
                    if (err != nullptr) *err = ::pilo::mk_perr(PERR_VAL_TOO_LARGE);
                    return PILO_INT8_MAX;
                }
                
                return (::pilo::u8_t)iv;

            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
            {
                ::pilo::i64_t iv = 0;
                ::pilo::err_t e = 0;
                if (this->_dynamic_data == nullptr || this->_size <= 1)
                {
                    
                    return 0;
                }
                e = ::pilo::core::string::string_to_number(iv, this->_dynamic_data, this->_size - 1);
                if (e != PILO_OK)
                {
                    if (err != nullptr) *err = e;
                    return 0;
                }
                if (iv > PILO_UINT8_MAX)
                {
                    if (err != nullptr) *err = ::pilo::mk_perr(PERR_VAL_TOO_LARGE);
                    return PILO_UINT8_MAX;
                }

                return (::pilo::u8_t)iv;
            }
        }

        if (err != nullptr) *err = ::pilo::mk_perr(PERR_INC_DATA);
        return 0;
    }

    ::pilo::i16_t tlv::as_i16(::pilo::err_t* err) const
    {
        if (_type.is_single())
        {
            if (_type.value_type_in_range(::pilo::core::rtti::wired_type::value_type_i8, ::pilo::core::rtti::wired_type::value_type_f32))
            {                
                return this->_i16;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_na)
            {
                
                return 0;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f32)
            {
                
                return (::pilo::i16_t)_f32;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f64)
            {
                
                return (::pilo::i16_t)_f64;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_str)
            {
                ::pilo::i64_t iv = 0;
                ::pilo::err_t e = 0;
                e = ::pilo::core::string::string_to_number(iv, ((std::string*)this->_dynamic_data)->c_str(), ((std::string*)this->_dynamic_data)->size());
                if (e != PILO_OK)
                {
                    if (err != nullptr) *err = e;
                    return 0;
                }
                if (iv < PILO_INT16_MIN)
                {
                    if (err != nullptr) *err = ::pilo::mk_perr(PERR_VAL_TOO_SAMLL);
                    return PILO_INT16_MIN;
                }
                else if (iv > PILO_INT16_MAX)
                {
                    if (err != nullptr) *err = ::pilo::mk_perr(PERR_VAL_TOO_LARGE);
                    return PILO_INT16_MAX;
                }
                
                return (::pilo::i16_t)iv;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
            {
                ::pilo::i64_t iv = 0;
                ::pilo::err_t e = 0;
                if (this->_dynamic_data == nullptr || this->_size <= 1)
                {
                    
                    return 0;
                }
                e = ::pilo::core::string::string_to_number(iv, this->_dynamic_data, this->_size - 1);
                if (e != PILO_OK)
                {
                    if (err != nullptr) *err = e;
                    return 0;
                }
                if (iv < PILO_INT16_MIN)
                {
                    if (err != nullptr) *err = ::pilo::mk_perr(PERR_VAL_TOO_SAMLL);
                    return PILO_INT16_MIN;
                }
                else if (iv > PILO_INT16_MAX)
                {
                    if (err != nullptr) *err = ::pilo::mk_perr(PERR_VAL_TOO_LARGE);
                    return PILO_INT16_MAX;
                }

                return (::pilo::i16_t)iv;
            }
        }

        if (err != nullptr) *err = ::pilo::mk_perr(PERR_INC_DATA);
        return 0;
    }

    ::pilo::u16_t tlv::as_u16(::pilo::err_t* err) const
    {
        if (_type.is_single())
        {
            if (_type.value_type_in_range(::pilo::core::rtti::wired_type::value_type_i8, ::pilo::core::rtti::wired_type::value_type_f32))
            {                
                return this->_u16;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_na)
            {
                
                return 0;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f32)
            {
                
                return (::pilo::u16_t)_f32;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f64)
            {
                
                return (::pilo::u16_t)_f64;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_str)
            {
                ::pilo::i64_t iv = 0;
                ::pilo::err_t e = 0;
                e = ::pilo::core::string::string_to_number(iv, ((std::string*)this->_dynamic_data)->c_str(), ((std::string*)this->_dynamic_data)->size());
                if (e != PILO_OK)
                {
                    if (err != nullptr) *err = e;
                    return 0;
                }
                else if (iv > PILO_UINT16_MAX)
                {
                    if (err != nullptr) *err = ::pilo::mk_perr(PERR_VAL_TOO_LARGE);
                    return PILO_INT16_MAX;
                }
                
                return (::pilo::u16_t)iv;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
            {
                ::pilo::i64_t iv = 0;
                ::pilo::err_t e = 0;
                if (this->_dynamic_data == nullptr || this->_size <= 1)
                {
                    
                    return 0;
                }
                e = ::pilo::core::string::string_to_number(iv, this->_dynamic_data, this->_size - 1);
                if (e != PILO_OK)
                {
                    if (err != nullptr) *err = e;
                    return 0;
                }
                if (iv > PILO_UINT16_MAX)
                {
                    if (err != nullptr) *err = ::pilo::mk_perr(PERR_VAL_TOO_LARGE);
                    return PILO_UINT16_MAX;
                }

                return (::pilo::u16_t)iv;
            }
        }

        if (err != nullptr) *err = ::pilo::mk_perr(PERR_INC_DATA);
        return 0;
    }

    ::pilo::i32_t tlv::as_i32(::pilo::err_t* err) const
    {
        if (_type.is_single())
        {
            if (_type.value_type_in_range(::pilo::core::rtti::wired_type::value_type_i8, ::pilo::core::rtti::wired_type::value_type_f32))
            {
                return this->_i32;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_na)
            {
                return 0;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f32)
            {
                return (::pilo::i32_t)_f32;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f64)
            {
                return (::pilo::i32_t)_f64;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_str)
            {
                ::pilo::i64_t iv = 0;
                ::pilo::err_t e = 0;
                e = ::pilo::core::string::string_to_number(iv, ((std::string*)this->_dynamic_data)->c_str(), ((std::string*)this->_dynamic_data)->size());
                if (e != PILO_OK)
                {
                    if (err != nullptr) *err = e;
                    return 0;
                }

                
                return (::pilo::i32_t)iv;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
            {
                ::pilo::i64_t iv = 0;
                ::pilo::err_t e = 0;
                if (this->_dynamic_data == nullptr || this->_size <= 1)
                {
                    
                    return 0;
                }
                e = ::pilo::core::string::string_to_number(iv, this->_dynamic_data, this->_size - 1);
                if (e != PILO_OK)
                {
                    if (err != nullptr) *err = e;
                    return 0;
                }
                if (iv < PILO_INT32_MIN)
                {
                    if (err != nullptr) *err = ::pilo::mk_perr(PERR_VAL_TOO_SAMLL);
                    return PILO_INT32_MIN;
                }
                else if (iv > PILO_INT32_MAX)
                {
                    if (err != nullptr) *err = ::pilo::mk_perr(PERR_VAL_TOO_LARGE);
                    return PILO_INT32_MAX;
                }

                return (::pilo::i32_t)iv;
            }
        }

        if (err != nullptr) *err = ::pilo::mk_perr(PERR_INC_DATA);
        return 0;
    }

    ::pilo::u32_t tlv::as_u32(::pilo::err_t* err) const
    {
        if (_type.is_single())
        {
            if (_type.value_type_in_range(::pilo::core::rtti::wired_type::value_type_i8, ::pilo::core::rtti::wired_type::value_type_f32))
            {
                return this->_u32;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_na)
            {
                
                return 0;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f32)
            {
                
                return (::pilo::u32_t)_f32;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f64)
            {
                
                return (::pilo::u32_t)_f64;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_str)
            {
                ::pilo::i64_t iv = 0;
                ::pilo::err_t e = 0;
                e = ::pilo::core::string::string_to_number(iv, ((std::string*)this->_dynamic_data)->c_str(), ((std::string*)this->_dynamic_data)->size());
                if (e != PILO_OK)
                {
                    if (err != nullptr) *err = e;
                    return 0;
                }
                else if (iv > PILO_UINT32_MAX)
                {
                    if (err != nullptr) *err = ::pilo::mk_perr(PERR_VAL_TOO_LARGE);
                    return PILO_INT32_MAX;
                }
                
                return (::pilo::u32_t)iv;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
            {
                ::pilo::i64_t iv = 0;
                ::pilo::err_t e = 0;
                if (this->_dynamic_data == nullptr || this->_size <= 1)
                {
                    
                    return 0;
                }
                e = ::pilo::core::string::string_to_number(iv, this->_dynamic_data, this->_size - 1);
                if (e != PILO_OK)
                {
                    if (err != nullptr) *err = e;
                    return 0;
                }
                if (iv > PILO_UINT32_MAX)
                {
                    if (err != nullptr) *err = ::pilo::mk_perr(PERR_VAL_TOO_LARGE);
                    return PILO_UINT32_MAX;
                }

                return (::pilo::u32_t)iv;
            }
        }

        if (err != nullptr) *err = ::pilo::mk_perr(PERR_INC_DATA);
        return 0;
    }

    ::pilo::i64_t tlv::as_i64(::pilo::err_t* err) const
    {
        if (_type.is_single())
        {
            if (_type.value_type_in_range(::pilo::core::rtti::wired_type::value_type_i8, ::pilo::core::rtti::wired_type::value_type_f32))
            {
                
                return this->_i64;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_na)
            {
                
                return 0;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f32)
            {
                
                return (::pilo::i64_t)_f32;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f64)
            {
                
                return (::pilo::i64_t)_f64;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
            {
                if (err != nullptr) *err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                return 0;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_str)
            {
                ::pilo::i64_t iv = 0;
                ::pilo::err_t e = 0;
                e = ::pilo::core::string::string_to_number(iv, ((std::string*)this->_dynamic_data)->c_str(), ((std::string*)this->_dynamic_data)->size());
                if (e != PILO_OK)
                {
                    if (err != nullptr) *err = e;
                    return 0;
                }
                
                return (::pilo::i64_t)iv;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
            {
                ::pilo::i64_t iv = 0;
                ::pilo::err_t e = 0;
                if (this->_dynamic_data == nullptr || this->_size <= 1)
                {
                    
                    return 0;
                }
                e = ::pilo::core::string::string_to_number(iv, this->_dynamic_data, this->_size - 1);
                if (e != PILO_OK)
                {
                    if (err != nullptr) *err = e;
                    return 0;
                }
                return iv;
            }
        }

        if (err != nullptr) *err = ::pilo::mk_perr(PERR_INC_DATA);
        return 0;
    }

    ::pilo::u64_t tlv::as_u64(::pilo::err_t* err) const
    {
        if (_type.is_single())
        {
            if (_type.value_type_in_range(::pilo::core::rtti::wired_type::value_type_i8, ::pilo::core::rtti::wired_type::value_type_f32))
            {
                
                return this->_u64;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_na)
            {
                
                return 0;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f32)
            {
                
                return (::pilo::u64_t)_f32;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f64)
            {
                
                return (::pilo::u64_t)_f64;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_str)
            {
                ::pilo::u64_t iv = 0;
                ::pilo::err_t e = 0;
                e = ::pilo::core::string::string_to_number((::pilo::i64_t&)iv, ((std::string*)this->_dynamic_data)->c_str(), ((std::string*)this->_dynamic_data)->size());
                if (e != PILO_OK)
                {
                    if (err != nullptr) *err = e;
                    return 0;
                }
                
                return (::pilo::u64_t)iv;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
            {
                ::pilo::i64_t iv = 0;
                ::pilo::err_t e = 0;
                if (this->_dynamic_data == nullptr || this->_size <= 1)
                {
                    
                    return 0;
                }
                e = ::pilo::core::string::string_to_number(iv, this->_dynamic_data, this->_size - 1);
                if (e != PILO_OK)
                {
                    if (err != nullptr) *err = e;
                    return 0;
                }
                return (::pilo::u64_t)iv;
            }
        }

        if (err != nullptr) *err = ::pilo::mk_perr(PERR_INC_DATA);
        return 0;
    }

    ::pilo::f32_t tlv::as_f32(::pilo::err_t* err) const
    {
        if (_type.is_single())
        {
            if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i8)
            {
                
                return (::pilo::f32_t) this->_i8;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u8)
            {
                
                return (::pilo::f32_t)this->_u8;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i16)
            {
                
                return (::pilo::f32_t)this->_i16;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u16)
            {
                
                return (::pilo::f32_t)this->_u16;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i32)
            {
                
                return (::pilo::f32_t)this->_i32;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u32)
            {
                
                return (::pilo::f32_t)this->_u32;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i64)
            {
                
                return (::pilo::f32_t)this->_i64;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u64)
            {
                
                return (::pilo::f32_t)this->_u64;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_na)
            {
                
                return 0;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f32)
            {
                
                return _f32;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f64)
            {
                
                return (::pilo::f32_t)_f64;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_str)
            {
                ::pilo::f32_t iv = 0;
                ::pilo::err_t e = 0;
                e = ::pilo::core::string::string_to_number(iv, ((std::string*)this->_dynamic_data)->c_str(), ((std::string*)this->_dynamic_data)->size());
                if (e != PILO_OK)
                {
                    if (err != nullptr) *err = e;
                    return 0;
                }
                
                return (::pilo::f32_t)iv;

            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
            {
                ::pilo::f32_t iv = 0;
                ::pilo::err_t e = 0;
                if (this->_dynamic_data == nullptr || this->_size <= 1)
                {
                    
                    return 0;
                }
                e = ::pilo::core::string::string_to_number(iv, this->_dynamic_data, this->_size - 1);
                if (e != PILO_OK)
                {
                    if (err != nullptr) *err = e;
                    return 0;
                }
                
                return (::pilo::f32_t)iv;
            }
        }

        if (err != nullptr) *err = ::pilo::mk_perr(PERR_INC_DATA);
        return 0;
    }

    ::pilo::f64_t tlv::as_f64(::pilo::err_t* err) const
    {
        if (_type.is_single())
        {
            if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i8)
            {
                
                return (::pilo::f64_t)this->_i8;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u8)
            {                
                return (::pilo::f64_t)this->_u8;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i16)
            {                
                return (::pilo::f64_t)this->_i16;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u16)
            {                
                return (::pilo::f64_t)this->_u16;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i32)
            {                
                return (::pilo::f64_t)this->_i32;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u32)
            {                
                return (::pilo::f64_t)this->_u32;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i64)
            {                
                return (::pilo::f64_t)this->_i64;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u64)
            {                
                return (::pilo::f64_t)this->_u64;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_na)
            {                
                return 0;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f32)
            {                
                return (::pilo::f64_t)_f32;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f64)
            {                
                return (::pilo::f64_t)_f64;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_str)
            {
                ::pilo::f64_t iv = 0;
                ::pilo::err_t e = 0;
                e = ::pilo::core::string::string_to_number(iv, ((std::string*)this->_dynamic_data)->c_str(), ((std::string*)this->_dynamic_data)->size());
                if (e != PILO_OK)
                {
                    if (err != nullptr) *err = e;
                    return 0;
                }                
                return (::pilo::f64_t)iv;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
            {
                ::pilo::f64_t iv = 0;
                ::pilo::err_t e = 0;
                if (this->_dynamic_data == nullptr || this->_size <= 1)
                {
                    
                    return 0;
                }
                e = ::pilo::core::string::string_to_number(iv, this->_dynamic_data, this->_size - 1);
                if (e != PILO_OK)
                {
                    if (err != nullptr) *err = e;
                    return 0;
                }                
                return (::pilo::f64_t)iv;
            }
        }

        if (err != nullptr) *err = ::pilo::mk_perr(PERR_INC_DATA);
        return 0;
    }

    //std::deque<::pilo::i32_t>* tlv::as_i32_array()
    //{
    //    if (_dynamic_data == nullptr || wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_array)
    //        return nullptr;
    //    return (std::deque<::pilo::i32_t> *) _dynamic_data;
    //}
    //std::deque<::pilo::u32_t> * tlv::as_u32_array()
    //{
    //    if (_dynamic_data == nullptr || wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_array)
    //        return nullptr;
    //    return (std::deque<::pilo::u32_t> *) _dynamic_data;
    //}

    bool tlv::equals_to(const::pilo::tlv* other) const
    {
        if (other != nullptr)
        {
            if ((_type.category_id() != other->_type.category_id())
                || (_type.wrapper_type() != other->_type.wrapper_type())
                || (_type.value_type() != other->_type.value_type())
                || (_type.key_type() != other->_type.key_type())
                || (_size != other->_size)
                )
            {
                return false;
            }
            ::pilo::err_t err = PILO_OK;
            if (_type.wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_single)
            {
                if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i8)
                {
                    return this->_i8 == other->_i8;
                }
                else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u8)
                {
                    return this->_u8 == other->_u8;
                }
                else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i16)
                {
                    return this->_i16 == other->_i16;
                }
                else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u16)
                {
                    return this->_u16 == other->_u16;
                }
                else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i32)
                {
                    return this->_i32 == other->_i32;
                }
                else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u32)
                {
                    return this->_u32 == other->_u32;
                }
                else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i64)
                {
                    return this->_i64 == other->_i64;
                }
                else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u64)
                {
                    return this->_u64 == other->_u64;
                }
                else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_na)
                {
                    return true;
                }
                else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f32)
                {
                    if (this->_f32 == other->_f32)
                        return true;
                    else
                        return false;
                }
                else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f64)
                {
                    if (this->_f64 == other->_f64)
                        return true;
                    else
                        return false;
                }
                else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_str)
                {
                    if (this->_dynamic_data == nullptr && other->_dynamic_data == nullptr)
                        return true;
                    else if (this->_dynamic_data == nullptr || other->_dynamic_data == nullptr)
                        return false;
                    return *((std::string*)this->_dynamic_data) == *((std::string*)other->_dynamic_data);
                }
                else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
                {                    
                    if (this->_dynamic_data == nullptr && other->_dynamic_data == nullptr)
                        return true;
                    else if (this->_dynamic_data == nullptr || other->_dynamic_data == nullptr)
                        return false;
                    return memcmp(this->_dynamic_data, other->_dynamic_data, this->_size) == 0;
                }
                else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_tlv)
                {
                    PMC_ASSERT(false);
                    return false;
                }
            }
            else if (_type.wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_array)
            {                
                return ::pilo::core::rtti::deque_equals(this->value_type(), this->_dynamic_data, other->value_type(), other->_dynamic_data, &err);
            }
            else if (_type.wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_dict)
            {
                return ::pilo::core::rtti::map_equals(this->key_type(), this->value_type(), this->_dynamic_data,
                    other->key_type(), other->value_type(), other->_dynamic_data, &err);
            }

            return true;
        }
        return false;
    }

    char* tlv::as_cstr_dup(::pilo::err_t* err, ::pilo::i32_t* rlen) const
    {
        if (_type.is_single())
        {
            if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_str)
            {
                
                if (rlen != nullptr) *rlen = (::pilo::i32_t)((std::string*)this->_dynamic_data)->size();
                return ::pilo::core::string::duplicate(((std::string*)this->_dynamic_data)->c_str(), ((std::string*)this->_dynamic_data)->size());
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
            {
                
                if (rlen != nullptr) *rlen = this->_size;
                if (this->_dynamic_data == nullptr)
                    return nullptr;
                return ::pilo::core::string::duplicate(this->_dynamic_data, this->_size);
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i8)
            {
                return ::pilo::core::rtti::convert_to_bytes(_i8, err);
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u8)
            {
                return ::pilo::core::rtti::convert_to_bytes(_i8, err);
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i16)
            {
                return ::pilo::core::rtti::convert_to_bytes(_i16, err);
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u16)
            {
                return ::pilo::core::rtti::convert_to_bytes(_u16, err);
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i32)
            {
                return ::pilo::core::rtti::convert_to_bytes(_i32, err);
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u32)
            {
                return ::pilo::core::rtti::convert_to_bytes(_u32, err);
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i64)
            {
                return ::pilo::core::rtti::convert_to_bytes(_i64, err);
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u64)
            {
                return ::pilo::core::rtti::convert_to_bytes(_u64, err);
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f32)
            {
                return ::pilo::core::rtti::convert_to_bytes(_f32, err);
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f64)
            {
                return ::pilo::core::rtti::convert_to_bytes(_f64, err);
            }
            else
            {
                if (err != nullptr) *err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                return nullptr;
            }
        }

        if (err != nullptr) *err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
        return nullptr;
    }

    const char* tlv::as_cstr(char* buffer, ::pilo::i32_t sz, ::pilo::err_t* err, ::pilo::i32_t* rlen, const char* fmt) const
    {
        if (_type.is_single())
        {
            if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_str)
            {
                
                if (rlen != nullptr) *rlen = (::pilo::i32_t) ((std::string*)this->_dynamic_data)->size();
                return ((std::string*)this->_dynamic_data)->c_str();
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
            {
                
                if (rlen != nullptr) *rlen = this->_size;
                return this->_dynamic_data;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i8)
            {
                ::pilo::core::string::number_to_string(buffer, sz, _i8, fmt);
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u8)
            {
                ::pilo::core::string::number_to_string(buffer, sz, _u8, fmt);
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i16)
            {
                ::pilo::core::string::number_to_string(buffer, sz, _i16, fmt);
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u16)
            {
                ::pilo::core::string::number_to_string(buffer, sz, _u16, fmt);
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i32)
            {
                ::pilo::core::string::number_to_string(buffer, sz, _i32, fmt);
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u32)
            {
                ::pilo::core::string::number_to_string(buffer, sz, _u32, fmt);
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i64)
            {
                ::pilo::core::string::number_to_string(buffer, sz, _i64, fmt);
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u64)
            {
                ::pilo::core::string::number_to_string(buffer, sz, _u64, fmt);
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f32)
            {
                ::pilo::core::string::number_to_string(buffer, sz, _f32, nullptr);
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f64)
            {
                ::pilo::core::string::number_to_string(buffer, sz, _f64, nullptr);
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_boolean)
            {
                if (_i8 == 0)
                    ::pilo::core::string::copyz(buffer, sz, "false");
                else
                    ::pilo::core::string::copyz(buffer, sz, "true");
            }
            else
            {
                if (err != nullptr) *err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                return nullptr;
            }

            
            return buffer;
        }

        if (err != nullptr) *err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
        return nullptr;
    }

    const char* tlv::as_bytes(char* buffer, ::pilo::i32_t sz, ::pilo::err_t* err, ::pilo::i32_t* rlen) const
    {
        if (_type.is_single())
        {
            if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_str)
            {
                if (this->_dynamic_data != nullptr)
                {
                    
                    if (rlen != nullptr) *rlen = (::pilo::i32_t)((std::string*)this->_dynamic_data)->size();
                    return ((std::string*)this->_dynamic_data)->c_str();
                }
                
                if (rlen != nullptr) *rlen = 0;
                return nullptr;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
            {
                
                if (rlen != nullptr) *rlen = this->_size;
                return this->_dynamic_data;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i8)
            {
                
                if (rlen != nullptr) *rlen = 1;
                buffer[0] = _i8;
                return buffer;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u8)
            {
                
                if (rlen != nullptr) *rlen = 1;
                buffer[0] = _u8;
                return buffer;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i16)
            {
                
                if (rlen != nullptr) *rlen = 2;
                memcpy(buffer, &_i16, 2);
                return buffer;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u16)
            {
                
                if (rlen != nullptr) *rlen = 2;
                memcpy(buffer, &_u16, 2);
                return buffer;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i32)
            {
                
                if (rlen != nullptr) *rlen = 4;
                memcpy(buffer, &_i32, 4);
                return buffer;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u32)
            {
                
                if (rlen != nullptr) *rlen = 4;
                memcpy(buffer, &_u32, 4);
                return buffer;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i64)
            {
                
                if (rlen != nullptr) *rlen = 8;
                memcpy(buffer, &_i64, 8);
                return buffer;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u64)
            {
                
                if (rlen != nullptr) *rlen = 8;
                memcpy(buffer, &_u64, 8);
                return buffer;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f32)
            {
                
                if (rlen != nullptr) *rlen = 4;
                memcpy(buffer, &_f32, 4);
                return buffer;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u64)
            {
                
                if (rlen != nullptr) *rlen = 8;
                memcpy(buffer, &_f64, 8);
                return buffer;
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_boolean)
            {
                if (_i8 == 0)
                    ::pilo::core::string::copyz(buffer, sz, "false");
                else
                    ::pilo::core::string::copyz(buffer, sz, "true");
            }
            else
            {
                if (err != nullptr) *err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                return nullptr;
            }
        }

        if (err != nullptr) *err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
        return nullptr;
    }

    const std::string* tlv::as_str_ptr(::pilo::err_t* err) const
    {
        if (_type.is_single())
        {
            if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_str)
            {
                
                return ((std::string*)this->_dynamic_data);
            }
        }

        if (err != nullptr) *err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
            return nullptr;        
    }

    std::string tlv::as_str(::pilo::err_t* err, const char* fmt) const
    {
        if (this->value_type() == ::pilo::core::rtti::wired_type::value_type_str)
        {
            return ((std::string) * (std::string*)this->_dynamic_data);
        }
        else
        {
            char buffer[32] = { 0 };
            ::pilo::i32_t rlen = 0;
            const char* ret_ptr = as_cstr(buffer, 32, err, &rlen, fmt);
            return std::string(ret_ptr);
        }
    }

    ::pilo::i64_t tlv::internal_array_size(::pilo::err_t * err) const
    {
        if (this->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_single)
        {
            if (err != nullptr) *err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
            return -1;
        }

        if (_dynamic_data == nullptr)
        {
            if (err != nullptr) *err = ::pilo::mk_perr(PERR_NULL_PTR);
            return -1;
        }
        else
        {
            if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_tlv)
            {
                return ((std::deque<::pilo::tlv*>*) this->_dynamic_data)->size();
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_str)
            {
                return ((std::deque<std::string>*) this->_dynamic_data)->size();
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i64)
            {
                return ((std::deque<::pilo::i64_t>*) this->_dynamic_data)->size();
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u64)
            {
                return ((std::deque<::pilo::u64_t>*) this->_dynamic_data)->size();
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i32)
            {
                return ((std::deque<::pilo::i32_t>*) this->_dynamic_data)->size();
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u32)
            {
                return ((std::deque<::pilo::u32_t>*) this->_dynamic_data)->size();
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i16)
            {
                return ((std::deque<::pilo::i16_t>*) this->_dynamic_data)->size();
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u16)
            {
                return ((std::deque<::pilo::u16_t>*) this->_dynamic_data)->size();
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_i8)
            {
                return ((std::deque<::pilo::i8_t>*) this->_dynamic_data)->size();
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_u8)
            {
                return ((std::deque<::pilo::u8_t>*) this->_dynamic_data)->size();
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f32)
            {
                return ((std::deque<::pilo::f32_t>*) this->_dynamic_data)->size();
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_f64)
            {
                return ((std::deque<::pilo::f64_t>*) this->_dynamic_data)->size();
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_boolean)
            {
                return ((std::deque<bool>*) this->_dynamic_data)->size();
            }
            else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
            {
                return ((std::deque<char*>*) this->_dynamic_data)->size();
            }
            else
            {
                if (err != nullptr) *err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                return -1;
            }
        }
    }

    ::pilo::tlv* tlv::allocate()
    {
        return PILO_CONTEXT->allocate_tlv();
    }

    void tlv::deallocate(::pilo::tlv* tlvp) 
    {
        PILO_CONTEXT->deallocate_tlv(tlvp);
    }

    std::shared_ptr<::pilo::tlv> tlv::allocate_shared()
    {
        std::shared_ptr<::pilo::tlv> t(PILO_CONTEXT->allocate_tlv(),
            [](::pilo::tlv* x) {
                PILO_CONTEXT->deallocate_tlv(x);
            });
        return t;
    }

    std::unique_ptr<::pilo::tlv, void(*)(::pilo::tlv*)> tlv::allocate_unique()
    {
        std::unique_ptr<::pilo::tlv, void(*)(::pilo::tlv*)> t(PILO_CONTEXT->allocate_tlv(),
            [](::pilo::tlv* x) {
                PILO_CONTEXT->deallocate_tlv(x);
            });
        return t;
    }

    ::pilo::err_t tlv::update_pool_object_stat(::pilo::core::stat::pool_object_stat_manager::stat_item* si)
    {
        ::pilo::i64_t avail = 0;
        ::pilo::i64_t total = 0;
        PILO_CONTEXT->tlv_pool()->m_memory_pool.calc_available_units_nolock(avail, total, 0);
        si->set(total, avail);
        return PILO_OK;
    }

    //::pilo::tlv* tlv::get_sub_tlv(const char* path, bool create_on_nonexist, ::pilo::err_t * err)
    //{
    //    if (!valid()) return nullptr;

    //    if (wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_single)
    //    {
    //        if (path == nullptr || *path == 0)
    //        {
    //            return this;
    //        }
    //        else
    //            return nullptr;
    //    }

    //    ::pilo::cstr_ref<char> parts[64];
    //    ::pilo::i64_t pcnt = ::pilo::core::string::split_fixed(path, -1,  ".", 1, parts, PMF_COUNT_OF(parts), false, true, true, true);
    //    if (pcnt < 1)
    //    {
    //        ::pilo::set_if_ptr_is_not_null<::pilo::err_t>(err, ::pilo::mk_perr(PES_PARAM, PEP_IS_INVALID));
    //        return nullptr;
    //    }

    //}

    tlv* tlv::clone() const
    {
        tlv* ret_tlv = tlv::allocate();
        ret_tlv->_size = this->_size;
        ret_tlv->_type = this->_type;
        ::pilo::err_t err = PILO_OK;
        if (this->wrapper_type() == wired_type::wrapper_single)
        {
            if (this->value_type() == ::pilo::core::rtti::wired_type::value_type_tlv)
            {
                PMC_ASSERT(false);
                return nullptr;
            }
            else if (this->value_type() == ::pilo::core::rtti::wired_type::value_type_str)
            {
                if (this->_dynamic_data == nullptr)
                    ret_tlv->_dynamic_data = nullptr;
                else
                    ret_tlv->_dynamic_data = (char*) new std::string((*(std::string*)this->_dynamic_data));
            }
            else if (this->value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
            {
                if (this->_dynamic_data == nullptr)
                    ret_tlv->_dynamic_data = nullptr;
                else
                {
                    ret_tlv->_dynamic_data = (char*)PMF_HEAP_MALLOC(ret_tlv->_size);
                    if (ret_tlv->_dynamic_data == nullptr)
                    {
                        tlv::deallocate(ret_tlv);
                        return nullptr;
                    }
                    memcpy(ret_tlv->_dynamic_data, this->_dynamic_data, ret_tlv->_size);
                }
            }
            else
            {
                ret_tlv->_u64 = this->_u64;
            }
        }
        else if (this->wrapper_type() == wired_type::wrapper_array)
        {
            err = ret_tlv->_clone_data_array(this);
            if (err != PILO_OK)
                return nullptr;
        }
        else if (this->wrapper_type() == wired_type::wrapper_dict)
        {
            err = ret_tlv->_clone_data_dict(this);
            if (err != PILO_OK)
                return nullptr;
        }
        else
        {
            PMC_ASSERT(false);
            tlv::deallocate(ret_tlv);
            return nullptr;
        }

        return ret_tlv;
    }

    std::shared_ptr<::pilo::tlv> tlv::clone_shared() const
    {
        std::shared_ptr<::pilo::tlv> t(this->clone(),
            [](::pilo::tlv* x) {
                PILO_CONTEXT->deallocate_tlv(x);
            });
        return t;
    }

    std::unique_ptr<::pilo::tlv, void(*)(::pilo::tlv*)> tlv::clone_unique() const
    {
        std::unique_ptr<::pilo::tlv, void(*)(::pilo::tlv*)> t(this->clone(),
            [](::pilo::tlv* x) {
                PILO_CONTEXT->deallocate_tlv(x);
            });
        return t;
    }

    ::pilo::err_t tlv::_clone_data_array(const ::pilo::tlv* src_tlv)
    {
        if (src_tlv == nullptr)
        {
            return ::pilo::mk_perr(PERR_NULL_PARAM);
        }

        if (this->_dynamic_data == nullptr && src_tlv->_dynamic_data == nullptr)
        {
            return PILO_OK;
        }
        else if (this->_dynamic_data != nullptr && src_tlv->_dynamic_data == nullptr)
        {
            _clear_dynamic_data();
            return PILO_OK;
        }

        if (_dynamic_data != nullptr)
        {
            _clear_dynamic_data();
        }

        ::pilo::err_t err = PILO_OK;
        _dynamic_data = ::pilo::core::rtti::clone_deque(src_tlv->_type.value_type(), src_tlv->_dynamic_data, &err);
        return err;
    }

    ::pilo::err_t tlv::_clone_data_dict(const::pilo::tlv* src_tlv)
    {
        if (src_tlv == nullptr)
        {
            return ::pilo::mk_perr(PERR_NULL_PARAM);
        }

        if (this->_dynamic_data == nullptr && src_tlv->_dynamic_data == nullptr)
        {
            return PILO_OK;
        }
        else if (this->_dynamic_data != nullptr && src_tlv->_dynamic_data == nullptr)
        {
            _clear_dynamic_data();
            return PILO_OK;
        }

        if (_dynamic_data != nullptr)
        {
            _clear_dynamic_data();
        }

        ::pilo::err_t err = PILO_OK;
        _dynamic_data = ::pilo::core::rtti::clone_map(src_tlv->key_type(), src_tlv->value_type(), src_tlv->_dynamic_data, &err);

        return err;
    }


    ::pilo::err_t tlv::_make_dynamic_data_array()
    {
        ::pilo::err_t err = PILO_OK;
        if (_dynamic_data == nullptr)
        {
            _dynamic_data = ::pilo::core::rtti::create_deque(_type.value_type(), &err);
            this->_size = 0;
        }

        return PILO_OK;
    }

    void tlv::_clear_dynamic_data()
    {
        if (this->_dynamic_data != nullptr)
        {
            if (this->_type.wrapper_type() == wired_type::wrapper_single)
            {
                if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
                {
                    PMF_HEAP_FREE(_dynamic_data);
                }
                else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_tlv)
                {
                    tlv::deallocate((::pilo::tlv*) this->_dynamic_data);                    
                }
                else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_str)
                {
                    delete ((std::string*)this->_dynamic_data);

                }

            }
            else if (this->_type.wrapper_type() == wired_type::wrapper_array)
            {
                ::pilo::core::rtti::destroy_deque(this->value_type(), & this->_dynamic_data);
            }           
            else if (this->_type.wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_dict)
            {
                ::pilo::core::rtti::delete_map(this->key_type(), this->value_type(), this->_dynamic_data);
            }


            _dynamic_data = nullptr;
        }
    }



}