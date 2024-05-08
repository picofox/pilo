#include "./byte_buffer_interface.hpp"
#include "../rtti/wired_type_deque.hpp"
#include "../rtti/wired_type_map_creation.hpp"
#include "../rtti/wired_type_map_comparation.hpp"

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template <typename TA_KEY, typename TA_VALUE>
            static ::pilo::err_t _s_map_read_then_put(::pilo::core::memory::byte_buffer_interface* bb, char* map_char_ptr, ::pilo::i32_t cnt, bool is_force)
            {
                ::pilo::err_t err = PILO_OK;
                TA_KEY k = ::pilo::_type_default_value_<TA_KEY>::get();
                TA_VALUE v = ::pilo::_type_default_value_<TA_VALUE>::get();
                for (::pilo::i32_t i = 0; i < cnt; i++)
                {
                    err = bb->read(k);
                    if (err != PILO_OK)
                    {
                        return err;
                    }
                    err = bb->read(v);
                    if (err != PILO_OK)
                    {
                        return err;
                    }
                    err = ::pilo::core::rtti::map_ptr_insert(map_char_ptr, k, v, is_force);
                    if (err != PILO_OK)
                    {
                        return err;
                    }
                }
                return PILO_OK;
            }
            template <typename TA_KEY>
            static ::pilo::err_t _s_map_read_then_put(::pilo::core::memory::byte_buffer_interface* bb, std::map<TA_KEY, ::pilo::tlv*>* tlv_map_ptr, ::pilo::i32_t cnt,bool is_force)
            {
                ::pilo::err_t err = PILO_OK;
                TA_KEY k = ::pilo::_type_default_value_<TA_KEY>::get();
                ::pilo::tlv* v = nullptr;
                for (::pilo::i32_t i = 0; i < cnt; i++)
                {
                    err = bb->read(k);
                    if (err != PILO_OK)
                    {
                        return err;
                    }
                    err = bb->read(v);
                    if (err != PILO_OK)
                    {
                        return err;
                    }
                    err = ::pilo::core::rtti::map_ptr_insert(tlv_map_ptr, k, v, is_force);
                    if (err != PILO_OK)
                    {
                        return err;
                    }
                }
                return PILO_OK;
            }


            template <typename TA_VALUE>
            static ::pilo::err_t _s_read_then_put(::pilo::core::memory::byte_buffer_interface* bb, char* rac, ::pilo::i32_t cnt)
            {
                ::pilo::err_t err = PILO_OK;
                TA_VALUE v = ::pilo::_type_default_value_<TA_VALUE>::get();
                for (::pilo::i32_t i = 0; i < cnt; i++)
                {
                    err = bb->read(v);
                    if (err != PILO_OK)
                    {
                        return err;
                    }
                    ((std::deque<TA_VALUE>*)rac)->push_back(v);
                }
                return PILO_OK;
            }

            static ::pilo::err_t _s_read_then_put(::pilo::core::memory::byte_buffer_interface* bb, std::deque<::pilo::tlv*>* rac, ::pilo::i32_t cnt)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::tlv* v = nullptr;
                for (::pilo::i32_t i = 0; i < cnt; i++)
                {
                    err = bb->read(v);
                    if (err != PILO_OK)
                    {
                        return err;
                    }
                    rac->push_back(v);
                }
                return PILO_OK;
            }


            typedef ::pilo::err_t(*handler_rac_bb_read_type)(::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t cnt);
            static handler_rac_bb_read_type s_handler_rac_bb_reader[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
            {
                [](::pilo::core::memory::byte_buffer_interface* , char*, ::pilo::i32_t) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                [](::pilo::core::memory::byte_buffer_interface* bb, char* rac, ::pilo::i32_t cnt) -> ::pilo::err_t { return _s_read_then_put<::pilo::i8_t>(bb, rac, cnt); },//1
                [](::pilo::core::memory::byte_buffer_interface* bb, char* rac, ::pilo::i32_t cnt) -> ::pilo::err_t { return _s_read_then_put<::pilo::u8_t>(bb, rac, cnt); },//2
                [](::pilo::core::memory::byte_buffer_interface* bb, char* rac, ::pilo::i32_t cnt) -> ::pilo::err_t { return _s_read_then_put<::pilo::i16_t>(bb, rac, cnt); },//3
                [](::pilo::core::memory::byte_buffer_interface* bb, char* rac, ::pilo::i32_t cnt) -> ::pilo::err_t { return _s_read_then_put<::pilo::u16_t>(bb, rac, cnt); },//4
                [](::pilo::core::memory::byte_buffer_interface* bb, char* rac, ::pilo::i32_t cnt) -> ::pilo::err_t { return _s_read_then_put<::pilo::i32_t>(bb, rac, cnt); },//5
                [](::pilo::core::memory::byte_buffer_interface* bb, char* rac, ::pilo::i32_t cnt) -> ::pilo::err_t { return _s_read_then_put<::pilo::u32_t>(bb, rac, cnt); },//6
                [](::pilo::core::memory::byte_buffer_interface* bb, char* rac, ::pilo::i32_t cnt) -> ::pilo::err_t { return _s_read_then_put<::pilo::i64_t>(bb, rac, cnt); },//7
                [](::pilo::core::memory::byte_buffer_interface* bb, char* rac, ::pilo::i32_t cnt) -> ::pilo::err_t { return _s_read_then_put<::pilo::u64_t>(bb, rac, cnt); },//8
                [](::pilo::core::memory::byte_buffer_interface* bb, char* rac, ::pilo::i32_t cnt) -> ::pilo::err_t { return _s_read_then_put<bool>(bb, rac, cnt); },//9
                [](::pilo::core::memory::byte_buffer_interface* bb, char* rac, ::pilo::i32_t cnt) -> ::pilo::err_t { return _s_read_then_put<::pilo::f32_t>(bb, rac, cnt); },//10
                [](::pilo::core::memory::byte_buffer_interface* bb, char* rac, ::pilo::i32_t cnt) -> ::pilo::err_t { return _s_read_then_put<::pilo::f64_t>(bb, rac, cnt); },//11
                [](::pilo::core::memory::byte_buffer_interface* bb, char* rac, ::pilo::i32_t cnt) -> ::pilo::err_t { return _s_read_then_put<char*>(bb, rac, cnt); },//12
                [](::pilo::core::memory::byte_buffer_interface* bb, char* rac, ::pilo::i32_t cnt) -> ::pilo::err_t { return _s_read_then_put<std::string>(bb, rac, cnt); },//13
                [](::pilo::core::memory::byte_buffer_interface* bb, char* rac, ::pilo::i32_t cnt) -> ::pilo::err_t { return _s_read_then_put(bb, (std::deque<::pilo::tlv*> *) rac, cnt); },//8
            };


            typedef ::pilo::err_t(*handler_rac_bb_write_type)(::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i64_t off, ::pilo::i64_t len);
            static handler_rac_bb_write_type s_handler_rac_bb_writer[::pilo::core::rtti::wired_type::value_type_intrincs_count] =
            {
                [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i64_t, ::pilo::i64_t) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                [](::pilo::core::memory::byte_buffer_interface* bb, char* arr, ::pilo::i64_t off, ::pilo::i64_t len) -> ::pilo::err_t { return bb->write_wired_rac<std::deque<::pilo::i8_t>>((std::deque<::pilo::i8_t>*)arr, off, len); },//1
                [](::pilo::core::memory::byte_buffer_interface* bb, char* arr, ::pilo::i64_t off, ::pilo::i64_t len) -> ::pilo::err_t { return bb->write_wired_rac<std::deque<::pilo::u8_t>>((std::deque<::pilo::u8_t>*)arr, off, len); },//2
                [](::pilo::core::memory::byte_buffer_interface* bb, char* arr, ::pilo::i64_t off, ::pilo::i64_t len) -> ::pilo::err_t { return bb->write_wired_rac<std::deque<::pilo::i16_t>>((std::deque<::pilo::i16_t>*)arr, off, len); },//3
                [](::pilo::core::memory::byte_buffer_interface* bb, char* arr, ::pilo::i64_t off, ::pilo::i64_t len) -> ::pilo::err_t { return bb->write_wired_rac<std::deque<::pilo::u16_t>>((std::deque<::pilo::u16_t>*)arr, off, len); },//4
                [](::pilo::core::memory::byte_buffer_interface* bb, char* arr, ::pilo::i64_t off, ::pilo::i64_t len) -> ::pilo::err_t { return bb->write_wired_rac<std::deque<::pilo::i32_t>>((std::deque<::pilo::i32_t>*)arr, off, len); },//5
                [](::pilo::core::memory::byte_buffer_interface* bb, char* arr, ::pilo::i64_t off, ::pilo::i64_t len) -> ::pilo::err_t { return bb->write_wired_rac<std::deque<::pilo::u32_t>>((std::deque<::pilo::u32_t>*)arr, off, len); },//6
                [](::pilo::core::memory::byte_buffer_interface* bb, char* arr, ::pilo::i64_t off, ::pilo::i64_t len) -> ::pilo::err_t { return bb->write_wired_rac<std::deque<::pilo::i64_t>>((std::deque<::pilo::i64_t>*)arr, off, len); },//7
                [](::pilo::core::memory::byte_buffer_interface* bb, char* arr, ::pilo::i64_t off, ::pilo::i64_t len) -> ::pilo::err_t { return bb->write_wired_rac<std::deque<::pilo::u64_t>>((std::deque<::pilo::u64_t>*)arr, off, len); },//8
                [](::pilo::core::memory::byte_buffer_interface* bb, char* arr, ::pilo::i64_t off, ::pilo::i64_t len) -> ::pilo::err_t { return bb->write_wired_rac<std::deque<bool>>((std::deque<bool>*)arr, off, len); },//9
                [](::pilo::core::memory::byte_buffer_interface* bb, char* arr, ::pilo::i64_t off, ::pilo::i64_t len) -> ::pilo::err_t { return bb->write_wired_rac<std::deque<::pilo::f32_t>>((std::deque<::pilo::f32_t>*)arr, off, len); },//10
                [](::pilo::core::memory::byte_buffer_interface* bb, char* arr, ::pilo::i64_t off, ::pilo::i64_t len) -> ::pilo::err_t { return bb->write_wired_rac<std::deque<::pilo::f64_t>>((std::deque<::pilo::f64_t>*)arr, off, len); },//11           
                [](::pilo::core::memory::byte_buffer_interface* bb, char* arr, ::pilo::i64_t off, ::pilo::i64_t len) -> ::pilo::err_t { return bb->write_wired_rac<std::deque<char*>>((std::deque<char*>*)arr, off, len); },//12
                [](::pilo::core::memory::byte_buffer_interface* bb, char* arr, ::pilo::i64_t off, ::pilo::i64_t len) -> ::pilo::err_t { return bb->write_wired_rac<std::deque<std::string>>((std::deque<std::string>*)arr, off, len); },//13
                [](::pilo::core::memory::byte_buffer_interface* bb, char* arr, ::pilo::i64_t off, ::pilo::i64_t len) -> ::pilo::err_t { return bb->write_wired_rac<std::deque<::pilo::tlv*>>((std::deque<::pilo::tlv*>*)arr, off, len); },//14
            };

            typedef ::pilo::err_t(*handler_map_bb_write_type)(::pilo::core::memory::byte_buffer_interface*, const char*);
            static handler_map_bb_write_type s_handler_map_bb_writer[::pilo::core::rtti::wired_type::key_type_count][::pilo::core::rtti::wired_type::value_type_intrincs_count] =
            {
                {
                    [](::pilo::core::memory::byte_buffer_interface*, const char*) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, const char*) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, const char*) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, const char*) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, const char*) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, const char*) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, const char*) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, const char*) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, const char*) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, const char*) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, const char*) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, const char*) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, const char*) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, const char*) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, const char*) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                },
                {//i8
                    [](::pilo::core::memory::byte_buffer_interface*, const char* ) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i8_t, ::pilo::i8_t>>((std::map<::pilo::i8_t, ::pilo::i8_t>*) m);},
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i8_t, ::pilo::u8_t>>((std::map<::pilo::i8_t, ::pilo::u8_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i8_t, ::pilo::i16_t>>((std::map<::pilo::i8_t, ::pilo::i16_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i8_t, ::pilo::u16_t>>((std::map<::pilo::i8_t, ::pilo::u16_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i8_t, ::pilo::i32_t>>((std::map<::pilo::i8_t, ::pilo::i32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i8_t, ::pilo::u32_t>>((std::map<::pilo::i8_t, ::pilo::u32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i8_t, ::pilo::i64_t>>((std::map<::pilo::i8_t, ::pilo::i64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i8_t, ::pilo::u64_t>>((std::map<::pilo::i8_t, ::pilo::u64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i8_t, bool>>((std::map<::pilo::i8_t, bool>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i8_t, ::pilo::f32_t>>((std::map<::pilo::i8_t, ::pilo::f32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i8_t, ::pilo::f64_t>>((std::map<::pilo::i8_t, ::pilo::f64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i8_t, char *>>((std::map<::pilo::i8_t, char*>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i8_t, std::string>>((std::map<::pilo::i8_t, std::string>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i8_t, ::pilo::tlv*>>((std::map<::pilo::i8_t, ::pilo::tlv*>*) m); },
                },
                {//u8
                    [](::pilo::core::memory::byte_buffer_interface* , const char* ) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u8_t, ::pilo::i8_t>>((std::map<::pilo::u8_t, ::pilo::i8_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u8_t, ::pilo::u8_t>>((std::map<::pilo::u8_t, ::pilo::u8_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u8_t, ::pilo::i16_t>>((std::map<::pilo::u8_t, ::pilo::i16_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u8_t, ::pilo::u16_t>>((std::map<::pilo::u8_t, ::pilo::u16_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u8_t, ::pilo::i32_t>>((std::map<::pilo::u8_t, ::pilo::i32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u8_t, ::pilo::u32_t>>((std::map<::pilo::u8_t, ::pilo::u32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u8_t, ::pilo::i64_t>>((std::map<::pilo::u8_t, ::pilo::i64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u8_t, ::pilo::u64_t>>((std::map<::pilo::u8_t, ::pilo::u64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u8_t, bool>>((std::map<::pilo::u8_t, bool>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u8_t, ::pilo::f32_t>>((std::map<::pilo::u8_t, ::pilo::f32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u8_t, ::pilo::f64_t>>((std::map<::pilo::u8_t, ::pilo::f64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u8_t, char*>>((std::map<::pilo::u8_t, char*>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u8_t, std::string>>((std::map<::pilo::u8_t, std::string>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u8_t, ::pilo::tlv*>>((std::map<::pilo::u8_t, ::pilo::tlv*>*) m); },
                },
                {//i16
                    [](::pilo::core::memory::byte_buffer_interface* , const char* ) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i16_t, ::pilo::i8_t>>((std::map<::pilo::i16_t, ::pilo::i8_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i16_t, ::pilo::u8_t>>((std::map<::pilo::i16_t, ::pilo::u8_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i16_t, ::pilo::i16_t>>((std::map<::pilo::i16_t, ::pilo::i16_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i16_t, ::pilo::u16_t>>((std::map<::pilo::i16_t, ::pilo::u16_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i16_t, ::pilo::i32_t>>((std::map<::pilo::i16_t, ::pilo::i32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i16_t, ::pilo::u32_t>>((std::map<::pilo::i16_t, ::pilo::u32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i16_t, ::pilo::i64_t>>((std::map<::pilo::i16_t, ::pilo::i64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i16_t, ::pilo::u64_t>>((std::map<::pilo::i16_t, ::pilo::u64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i16_t, bool>>((std::map<::pilo::i16_t, bool>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i16_t, ::pilo::f32_t>>((std::map<::pilo::i16_t, ::pilo::f32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i16_t, ::pilo::f64_t>>((std::map<::pilo::i16_t, ::pilo::f64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i16_t, char*>>((std::map<::pilo::i16_t, char*>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i16_t, std::string>>((std::map<::pilo::i16_t, std::string>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i16_t, ::pilo::tlv*>>((std::map<::pilo::i16_t, ::pilo::tlv*>*) m); },
                },
                {//u16
                    [](::pilo::core::memory::byte_buffer_interface* , const char* ) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u16_t, ::pilo::i8_t>>((std::map<::pilo::u16_t, ::pilo::i8_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u16_t, ::pilo::u8_t>>((std::map<::pilo::u16_t, ::pilo::u8_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u16_t, ::pilo::i16_t>>((std::map<::pilo::u16_t, ::pilo::i16_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u16_t, ::pilo::u16_t>>((std::map<::pilo::u16_t, ::pilo::u16_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u16_t, ::pilo::i32_t>>((std::map<::pilo::u16_t, ::pilo::i32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u16_t, ::pilo::u32_t>>((std::map<::pilo::u16_t, ::pilo::u32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u16_t, ::pilo::i64_t>>((std::map<::pilo::u16_t, ::pilo::i64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u16_t, ::pilo::u64_t>>((std::map<::pilo::u16_t, ::pilo::u64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u16_t, bool>>((std::map<::pilo::u16_t, bool>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u16_t, ::pilo::f32_t>>((std::map<::pilo::u16_t, ::pilo::f32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u16_t, ::pilo::f64_t>>((std::map<::pilo::u16_t, ::pilo::f64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u16_t, char*>>((std::map<::pilo::u16_t, char*>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u16_t, std::string>>((std::map<::pilo::u16_t, std::string>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u16_t, ::pilo::tlv*>>((std::map<::pilo::u16_t, ::pilo::tlv*>*) m); },
                },
                {//i32
                    [](::pilo::core::memory::byte_buffer_interface* , const char* ) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i32_t, ::pilo::i8_t>>((std::map<::pilo::i32_t, ::pilo::i8_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i32_t, ::pilo::u8_t>>((std::map<::pilo::i32_t, ::pilo::u8_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i32_t, ::pilo::i16_t>>((std::map<::pilo::i32_t, ::pilo::i16_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i32_t, ::pilo::u16_t>>((std::map<::pilo::i32_t, ::pilo::u16_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i32_t, ::pilo::i32_t>>((std::map<::pilo::i32_t, ::pilo::i32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i32_t, ::pilo::u32_t>>((std::map<::pilo::i32_t, ::pilo::u32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i32_t, ::pilo::i64_t>>((std::map<::pilo::i32_t, ::pilo::i64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i32_t, ::pilo::u64_t>>((std::map<::pilo::i32_t, ::pilo::u64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i32_t, bool>>((std::map<::pilo::i32_t, bool>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i32_t, ::pilo::f32_t>>((std::map<::pilo::i32_t, ::pilo::f32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i32_t, ::pilo::f64_t>>((std::map<::pilo::i32_t, ::pilo::f64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i32_t, char*>>((std::map<::pilo::i32_t, char*>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i32_t, std::string>>((std::map<::pilo::i32_t, std::string>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i32_t, ::pilo::tlv*>>((std::map<::pilo::i32_t, ::pilo::tlv*>*) m); },
                },
                {//u32
                    [](::pilo::core::memory::byte_buffer_interface* , const char* ) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u32_t, ::pilo::i8_t>>((std::map<::pilo::u32_t, ::pilo::i8_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u32_t, ::pilo::u8_t>>((std::map<::pilo::u32_t, ::pilo::u8_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u32_t, ::pilo::i16_t>>((std::map<::pilo::u32_t, ::pilo::i16_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u32_t, ::pilo::u16_t>>((std::map<::pilo::u32_t, ::pilo::u16_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u32_t, ::pilo::i32_t>>((std::map<::pilo::u32_t, ::pilo::i32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u32_t, ::pilo::u32_t>>((std::map<::pilo::u32_t, ::pilo::u32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u32_t, ::pilo::i64_t>>((std::map<::pilo::u32_t, ::pilo::i64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u32_t, ::pilo::u64_t>>((std::map<::pilo::u32_t, ::pilo::u64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u32_t, bool>>((std::map<::pilo::u32_t, bool>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u32_t, ::pilo::f32_t>>((std::map<::pilo::u32_t, ::pilo::f32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u32_t, ::pilo::f64_t>>((std::map<::pilo::u32_t, ::pilo::f64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u32_t, char*>>((std::map<::pilo::u32_t, char*>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u32_t, std::string>>((std::map<::pilo::u32_t, std::string>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u32_t, ::pilo::tlv*>>((std::map<::pilo::u32_t, ::pilo::tlv*>*) m); },
                },
                {//i64
                    [](::pilo::core::memory::byte_buffer_interface* , const char* ) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i64_t, ::pilo::i8_t>>((std::map<::pilo::i64_t, ::pilo::i8_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i64_t, ::pilo::u8_t>>((std::map<::pilo::i64_t, ::pilo::u8_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i64_t, ::pilo::i16_t>>((std::map<::pilo::i64_t, ::pilo::i16_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i64_t, ::pilo::u16_t>>((std::map<::pilo::i64_t, ::pilo::u16_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i64_t, ::pilo::i32_t>>((std::map<::pilo::i64_t, ::pilo::i32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i64_t, ::pilo::u32_t>>((std::map<::pilo::i64_t, ::pilo::u32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i64_t, ::pilo::i64_t>>((std::map<::pilo::i64_t, ::pilo::i64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i64_t, ::pilo::u64_t>>((std::map<::pilo::i64_t, ::pilo::u64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i64_t, bool>>((std::map<::pilo::i64_t, bool>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i64_t, ::pilo::f32_t>>((std::map<::pilo::i64_t, ::pilo::f32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i64_t, ::pilo::f64_t>>((std::map<::pilo::i64_t, ::pilo::f64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i64_t, char*>>((std::map<::pilo::i64_t, char*>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i64_t, std::string>>((std::map<::pilo::i64_t, std::string>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::i64_t, ::pilo::tlv*>>((std::map<::pilo::i64_t, ::pilo::tlv*>*) m); },
                },
                {//u64
                    [](::pilo::core::memory::byte_buffer_interface* , const char* ) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u64_t, ::pilo::i8_t>>((std::map<::pilo::u64_t, ::pilo::i8_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u64_t, ::pilo::u8_t>>((std::map<::pilo::u64_t, ::pilo::u8_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u64_t, ::pilo::i16_t>>((std::map<::pilo::u64_t, ::pilo::i16_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u64_t, ::pilo::u16_t>>((std::map<::pilo::u64_t, ::pilo::u16_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u64_t, ::pilo::i32_t>>((std::map<::pilo::u64_t, ::pilo::i32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u64_t, ::pilo::u32_t>>((std::map<::pilo::u64_t, ::pilo::u32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u64_t, ::pilo::i64_t>>((std::map<::pilo::u64_t, ::pilo::i64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u64_t, ::pilo::u64_t>>((std::map<::pilo::u64_t, ::pilo::u64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u64_t, bool>>((std::map<::pilo::u64_t, bool>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u64_t, ::pilo::f32_t>>((std::map<::pilo::u64_t, ::pilo::f32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u64_t, ::pilo::f64_t>>((std::map<::pilo::u64_t, ::pilo::f64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u64_t, char*>>((std::map<::pilo::u64_t, char*>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u64_t, std::string>>((std::map<::pilo::u64_t, std::string>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<::pilo::u64_t, ::pilo::tlv*>>((std::map<::pilo::u64_t, ::pilo::tlv*>*) m); },
                },
                {//std::string
                    [](::pilo::core::memory::byte_buffer_interface* , const char* ) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<std::string, ::pilo::i8_t>>((std::map<std::string, ::pilo::i8_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<std::string, ::pilo::u8_t>>((std::map<std::string, ::pilo::u8_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<std::string, ::pilo::i16_t>>((std::map<std::string, ::pilo::i16_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<std::string, ::pilo::u16_t>>((std::map<std::string, ::pilo::u16_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<std::string, ::pilo::i32_t>>((std::map<std::string, ::pilo::i32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<std::string, ::pilo::u32_t>>((std::map<std::string, ::pilo::u32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<std::string, ::pilo::i64_t>>((std::map<std::string, ::pilo::i64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<std::string, ::pilo::u64_t>>((std::map<std::string, ::pilo::u64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<std::string, bool>>((std::map<std::string, bool>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<std::string, ::pilo::f32_t>>((std::map<std::string, ::pilo::f32_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<std::string, ::pilo::f64_t>>((std::map<std::string, ::pilo::f64_t>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<std::string, char*>>((std::map<std::string, char*>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<std::string, std::string>>((std::map<std::string, std::string>*) m); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, const char* m) -> ::pilo::err_t { return bb->write_wired_map<std::map<std::string, ::pilo::tlv*>>((std::map<std::string, ::pilo::tlv*>*) m); },
                },
            };

            typedef ::pilo::err_t(*handler_map_bb_read_type)(::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t cnt, bool is_force);
            static handler_map_bb_read_type s_handler_map_bb_reader[::pilo::core::rtti::wired_type::key_type_count][::pilo::core::rtti::wired_type::value_type_intrincs_count] =
            {
                {
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                },
                {//i8
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i8_t, ::pilo::i8_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i8_t, ::pilo::u8_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i8_t, ::pilo::i16_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i8_t, ::pilo::u16_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i8_t, ::pilo::i32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i8_t, ::pilo::u32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i8_t, ::pilo::i64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i8_t, ::pilo::u64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i8_t, bool>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i8_t, ::pilo::f32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i8_t, ::pilo::f64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i8_t, char*>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i8_t, std::string>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i8_t>(bb, (std::map<::pilo::i8_t, ::pilo::tlv*>*) m, cnt, is_force); },
                },
                {//u8
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u8_t, ::pilo::i8_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u8_t, ::pilo::u8_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u8_t, ::pilo::i16_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u8_t, ::pilo::u16_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u8_t, ::pilo::i32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u8_t, ::pilo::u32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u8_t, ::pilo::i64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u8_t, ::pilo::u64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u8_t, bool>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u8_t, ::pilo::f32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u8_t, ::pilo::f64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u8_t, char*>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u8_t, std::string>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u8_t>(bb, (std::map<::pilo::u8_t, ::pilo::tlv*>*) m, cnt, is_force); },
                },
                {//i16
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i16_t, ::pilo::i8_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i16_t, ::pilo::u8_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i16_t, ::pilo::i16_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i16_t, ::pilo::u16_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i16_t, ::pilo::i32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i16_t, ::pilo::u32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i16_t, ::pilo::i64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i16_t, ::pilo::u64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i16_t, bool>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i16_t, ::pilo::f32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i16_t, ::pilo::f64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i16_t, char*>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i16_t, std::string>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i16_t>(bb, (std::map<::pilo::i16_t, ::pilo::tlv*>*) m, cnt, is_force); },
                },
                {//u16
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u16_t, ::pilo::i8_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u16_t, ::pilo::u8_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u16_t, ::pilo::i16_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u16_t, ::pilo::u16_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u16_t, ::pilo::i32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u16_t, ::pilo::u32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u16_t, ::pilo::i64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u16_t, ::pilo::u64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u16_t, bool>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u16_t, ::pilo::f32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u16_t, ::pilo::f64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u16_t, char*>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u16_t, std::string>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u16_t>(bb, (std::map<::pilo::u16_t, ::pilo::tlv*>*) m, cnt, is_force); },
                },
                {//i32
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i32_t, ::pilo::i8_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i32_t, ::pilo::u8_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i32_t, ::pilo::i16_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i32_t, ::pilo::u16_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i32_t, ::pilo::i32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i32_t, ::pilo::u32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i32_t, ::pilo::i64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i32_t, ::pilo::u64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i32_t, bool>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i32_t, ::pilo::f32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i32_t, ::pilo::f64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i32_t, char*>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i32_t, std::string>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i32_t>(bb, (std::map<::pilo::i32_t, ::pilo::tlv*>*) m, cnt, is_force); },
                },
                {//u32
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u32_t, ::pilo::i8_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u32_t, ::pilo::u8_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u32_t, ::pilo::i16_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u32_t, ::pilo::u16_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u32_t, ::pilo::i32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u32_t, ::pilo::u32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u32_t, ::pilo::i64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u32_t, ::pilo::u64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u32_t, bool>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u32_t, ::pilo::f32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u32_t, ::pilo::f64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u32_t, char*>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u32_t, std::string>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u32_t>(bb, (std::map<::pilo::u32_t, ::pilo::tlv*>*) m, cnt, is_force); },
                },
                {//i64
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i64_t, ::pilo::i8_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i64_t, ::pilo::u8_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i64_t, ::pilo::i16_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i64_t, ::pilo::u16_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i64_t, ::pilo::i32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i64_t, ::pilo::u32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i64_t, ::pilo::i64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i64_t, ::pilo::u64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i64_t, bool>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i64_t, ::pilo::f32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i64_t, ::pilo::f64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i64_t, char*>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i64_t, std::string>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::i64_t>(bb, (std::map<::pilo::i64_t, ::pilo::tlv*>*) m, cnt, is_force); },
                },
                {//u64
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u64_t, ::pilo::i8_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u64_t, ::pilo::u8_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u64_t, ::pilo::i16_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u64_t, ::pilo::u16_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u64_t, ::pilo::i32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u64_t, ::pilo::u32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u64_t, ::pilo::i64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u64_t, ::pilo::u64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u64_t, bool>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u64_t, ::pilo::f32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u64_t, ::pilo::f64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u64_t, char*>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u64_t, std::string>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<::pilo::u64_t>(bb, (std::map<::pilo::u64_t, ::pilo::tlv*>*) m, cnt, is_force); },
                },
                {//str
                    [](::pilo::core::memory::byte_buffer_interface*, char*, ::pilo::i32_t , bool) -> ::pilo::err_t { return ::pilo::make_core_error(PES_PARAM, PEP_TYPE_MISMATCH); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<std::string, ::pilo::i8_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<std::string, ::pilo::u8_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<std::string, ::pilo::i16_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<std::string, ::pilo::u16_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<std::string, ::pilo::i32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<std::string, ::pilo::u32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<std::string, ::pilo::i64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<std::string, ::pilo::u64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<std::string, bool>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<std::string, ::pilo::f32_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<std::string, ::pilo::f64_t>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<std::string, char*>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<std::string, std::string>(bb, m, cnt, is_force); },
                    [](::pilo::core::memory::byte_buffer_interface* bb, char* m, ::pilo::i32_t cnt, bool is_force) -> ::pilo::err_t { return _s_map_read_then_put<std::string>(bb, (std::map<std::string, ::pilo::tlv*>*) m, cnt, is_force); },
                },

            };


            
            ::pilo::err_t byte_buffer_interface::write_wired_rac(::pilo::u16_t vt, char* arr)
            {
                ::pilo::err_t err = PILO_OK;
                if (vt == ::pilo::core::rtti::wired_type::value_type_na)
                {
                    return ::pilo::make_core_error(PES_ELEM, PEP_TYPE_MISMATCH);
                }

                if (vt < ::pilo::core::rtti::wired_type::value_type_intrincs_count)
                {
                    err = s_handler_rac_bb_writer[vt](this, arr, 0, -1);
                    if (err != PILO_OK)
                        return err;
                }
                else
                {
                    PMC_ASSERT(false);
                    return ::pilo::make_core_error(PES_OP, PEP_UNSUPPORT);

                }

                return PILO_OK;
            }

            char* byte_buffer_interface::read_wired_rac(::pilo::err_t & err)
            {
                ::pilo::u16_t vt = 0;
                err = this->read_uint16(vt);
                if (err != PILO_OK)
                {
                    return nullptr;
                }

                if (vt == ::pilo::core::rtti::wired_type::value_type_na)
                {
                    err = ::pilo::make_core_error(PES_TLV, PEP_TYPE_MISMATCH);
                    return nullptr;
                }

                ::pilo::i32_t cnt = 0;
                err = this->read_int32(cnt);
                if (err != PILO_OK)
                    return nullptr;

                if (cnt < 0)
                {
                    return nullptr;
                }
                char* rac = ::pilo::core::rtti::create_deque(vt, &err);
                if (rac == nullptr)
                {
                    return nullptr;
                }     
                if (cnt == 0)
                {
                    return rac;
                }
                err = s_handler_rac_bb_reader[vt](this, rac, cnt);
                if (err != PILO_OK)
                {
                    ::pilo::core::rtti::destroy_deque(vt, &rac);
                    return nullptr;
                }

                return rac;
            }

            ::pilo::err_t byte_buffer_interface::write_wired_map(::pilo::u8_t kt, ::pilo::u16_t vt, const char* map_char_ptr)
            {
                if (kt >= ::pilo::core::rtti::wired_type::key_type_count)
                {
                    return ::pilo::make_core_error(PES_WK_TYPE, PEP_IS_INVALID);
                }

                if (vt == ::pilo::core::rtti::wired_type::value_type_na)
                {
                    return ::pilo::make_core_error(PES_WV_TYPE, PEP_IS_INVALID);
                }

                if (vt < ::pilo::core::rtti::wired_type::value_type_intrincs_count)
                {
                    return s_handler_map_bb_writer[kt][vt](this, map_char_ptr);
                }
                else
                {
                    PMC_ASSERT(false);
                    return ::pilo::make_core_error(PES_OP, PEP_UNSUPPORT);

                }                
            }

            char* byte_buffer_interface::read_wired_map(::pilo::err_t& err)
            {
                ::pilo::u8_t kt = 0;
                err = this->read_uint8(kt);
                if (err != PILO_OK)
                {
                    return nullptr;
                }
                ::pilo::u16_t vt = 0;
                err = this->read_uint16(vt);
                if (err != PILO_OK)
                {
                    return nullptr;
                }

                ::pilo::i32_t cnt = 0;
                err = this->read_int32(cnt);
                if (err != PILO_OK)
                    return nullptr;
                if (cnt < 0)
                {
                    return nullptr;
                }
                char* m = ::pilo::core::rtti::create_map(kt, vt, &err);
                if (m == nullptr)
                {
                    return nullptr;
                }
                if (cnt == 0)
                {
                    return m;
                }
                err = s_handler_map_bb_reader[kt][vt](this, m, cnt, true);
                if (err != PILO_OK)
                {
                    ::pilo::core::rtti::delete_map(kt, vt, m);
                    return nullptr;
                }
                return m;
            }
        }
    }
}