#include "logger_config.hpp"
#include "../ml/tlv_driver_interface.hpp"
#include <string>
#include "../../pilo.hpp"
#include "../memory/util.hpp"


namespace pilo {
    namespace core {
        namespace config {
            ::pilo::err_t logger::save_to_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* driver)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::core::memory::object_array<char, 128>   cb;
                char tmp_buffer[256] = { 0 };
                char* tmp_buffer_ptr = tmp_buffer;

                ::pilo::i32_t fqdn_len = (::pilo::i32_t) ::pilo::core::string::character_count(fqdn_path);
                cb.check_space(fqdn_len + 32);
                ::pilo::core::string::n_copyz(cb.begin(), cb.capacity(), fqdn_path, fqdn_len);
                cb.set_size(fqdn_len);

                ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), ".type", 5);
                PILO_CHKERR_RET(err, driver->set_value(cb.begin(), type_name(), -1, false, true, true));

                ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), ".level", 6);
                PILO_CHKERR_RET(err, driver->set_value(cb.begin(), level_name(), -1, false, true, true));

                ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), ".splition_type", 14);
                PILO_CHKERR_RET(err, driver->set_value(cb.begin(), splition_type_name(), -1, false, true, true));

                ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), ".outputs", 8);
                get_outputs_devs(tmp_buffer, sizeof(tmp_buffer));
                PILO_CHKERR_RET(err, driver->set_value(cb.begin(), tmp_buffer, -1, false, true, true));

                ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), ".name_suffix", 12);
                tmp_buffer_ptr = get_predef_elements(name_suffix().data(), tmp_buffer, sizeof(tmp_buffer));
                PILO_CHKERR_RET(err, driver->set_value(cb.begin(), tmp_buffer_ptr, -1, false, true, true));
                PMF_COMPARE_HEAP_FREE(tmp_buffer_ptr, tmp_buffer);

                ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), ".headers", 8);
                tmp_buffer_ptr = get_predef_elements(headers().data(), tmp_buffer, sizeof(tmp_buffer));
                PILO_CHKERR_RET(err, driver->set_value(cb.begin(), tmp_buffer_ptr, -1, false, true, true));
                PMF_COMPARE_HEAP_FREE(tmp_buffer_ptr, tmp_buffer);

                ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), ".split_every", 12);
                PILO_CHKERR_RET(err, driver->set_value(cb.begin(), split_every(), true));

                ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), ".flags", 6);
                tmp_buffer_ptr = get_flags(tmp_buffer, sizeof(tmp_buffer));
                PILO_CHKERR_RET(err, driver->set_value(cb.begin(), tmp_buffer_ptr, -1, false, true, true));
                PMF_COMPARE_HEAP_FREE(tmp_buffer_ptr, tmp_buffer);

                ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), ".bak_name_suffix", 16);
                tmp_buffer_ptr = get_predef_elements(bak_name_suffix().data(), tmp_buffer, sizeof(tmp_buffer));
                PILO_CHKERR_RET(err, driver->set_value(cb.begin(), tmp_buffer_ptr, -1, false, true, true));
                PMF_COMPARE_HEAP_FREE(tmp_buffer_ptr, tmp_buffer);

                ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), ".size_quota", 11);
                PILO_CHKERR_RET(err, driver->set_value(cb.begin(), size_quota(), true));

                ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), ".piece_quota", 12);
                PILO_CHKERR_RET(err, driver->set_value(cb.begin(), piece_quota(), true));

                ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), ".name", 5);
                PILO_CHKERR_RET(err, driver->set_value(cb.begin(), name().c_str(), (::pilo::i32_t) name().size(), false, true, true));

                ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), ".bak_dir", 8);
                PILO_CHKERR_RET(err, driver->set_value(cb.begin(), bak_dir().c_str(), (::pilo::i32_t)bak_dir().size(), false, true, true));

                ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), ".line_sep", 9);
                PILO_CHKERR_RET(err, driver->set_value(cb.begin(), line_sep().c_str(), (::pilo::i32_t)line_sep().size(), false, true, true));

                ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), ".field_sep", 10);
                PILO_CHKERR_RET(err, driver->set_value(cb.begin(), field_sep().c_str(), (::pilo::i32_t)field_sep().size(), false, true, true));

                return PILO_OK;
            }

            ::pilo::err_t logger::load_from_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* driver)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::tlv* svp = nullptr;
                ::pilo::tlv* tlv_val_ptr = nullptr;
                svp = driver->get_value_node(fqdn_path, err);
                PILO_ERRRET(err);

                err = svp->get<std::string, ::pilo::tlv*>(std::string("type"), tlv_val_ptr);
                PILO_ERRRET(err);
                err = tlv_val_ptr->to_enum_id(_type, ::pilo::core::logging::g_logger_type_names, (::pilo::u8_t)(PMF_COUNT_OF(::pilo::core::logging::g_logger_type_names)), true);
                PILO_ERRRET(err);

                err = svp->get<std::string, ::pilo::tlv*>(std::string("level"), tlv_val_ptr);
                PILO_ERRRET(err);
                err = tlv_val_ptr->to_enum_id(_level, ::pilo::core::logging::g_level_names, (::pilo::u8_t)(PMF_COUNT_OF(::pilo::core::logging::g_level_names)), true);
                PILO_ERRRET(err);

                err = svp->get<std::string, ::pilo::tlv*>(std::string("splition_type"), tlv_val_ptr);
                PILO_ERRRET(err);
                err = tlv_val_ptr->to_enum_id(_splition_type, ::pilo::core::logging::g_splition_type_names, (::pilo::u8_t)(PMF_COUNT_OF(::pilo::core::logging::g_splition_type_names)), true);
                PILO_ERRRET(err);

                err = svp->get<std::string, ::pilo::tlv*>(std::string("outputs"), tlv_val_ptr);
                PILO_ERRRET(err);
                tlv_val_ptr->to_flags<::pilo::u8_t, PMF_COUNT_OF(::pilo::core::logging::g_output_dev_names)>(_outputs, ",", 1, ::pilo::core::logging::g_output_dev_names);

                err = svp->get<std::string, ::pilo::tlv*>(std::string("name_suffix"), tlv_val_ptr);
                PILO_ERRRET(err);
                tlv_val_ptr->to_flags<::pilo::u32_t, PMF_COUNT_OF(::pilo::core::logging::g_predef_elment_names)>(_name_suffix, ",", 1, ::pilo::core::logging::g_predef_elment_names);


                err = svp->get<std::string, ::pilo::tlv*>(std::string("headers"), tlv_val_ptr);
                PILO_ERRRET(err);
                tlv_val_ptr->to_flags<::pilo::u32_t, PMF_COUNT_OF(::pilo::core::logging::g_predef_elment_names)>(_headers, ",", 1, ::pilo::core::logging::g_predef_elment_names);

                err = svp->get<std::string, ::pilo::tlv*>(std::string("split_every"), tlv_val_ptr);
                _split_every = tlv_val_ptr->as_i32(&err);
                PILO_ERRRET(err);

                err = svp->get<std::string, ::pilo::tlv*>(std::string("flags"), tlv_val_ptr);
                PILO_ERRRET(err);
                tlv_val_ptr->to_flags<::pilo::u32_t, PMF_COUNT_OF(::pilo::core::logging::g_flags)>(_flags, ",", 1, ::pilo::core::logging::g_flags);

                err = svp->get<std::string, ::pilo::tlv*>(std::string("bak_name_suffix"), tlv_val_ptr);
                PILO_ERRRET(err);
                tlv_val_ptr->to_flags<::pilo::u32_t, PMF_COUNT_OF(::pilo::core::logging::g_predef_elment_names)>(_bak_name_suffix, ",", 1, ::pilo::core::logging::g_predef_elment_names);

                err = svp->get<std::string, ::pilo::tlv*>(std::string("size_quota"), tlv_val_ptr);
                _size_quota = tlv_val_ptr->as_i64(&err);
                PILO_ERRRET(err);

                err = svp->get<std::string, ::pilo::tlv*>(std::string("piece_quota"), tlv_val_ptr);
                _piece_quota = tlv_val_ptr->as_i64(&err);
                PILO_ERRRET(err);

                err = svp->get<std::string, ::pilo::tlv*>(std::string("name"), tlv_val_ptr);
                PILO_ERRRET(err);
                err = tlv_val_ptr->assign_as_str(_name);
                PILO_ERRRET(err);

                err = svp->get<std::string, ::pilo::tlv*>(std::string("bak_dir"), tlv_val_ptr);
                PILO_ERRRET(err);
                err = tlv_val_ptr->assign_as_str(_bak_dir);
                PILO_ERRRET(err);

                err = svp->get<std::string, ::pilo::tlv*>(std::string("line_sep"), tlv_val_ptr);
                PILO_ERRRET(err);
                err = tlv_val_ptr->assign_as_str(_line_sep);
                PILO_ERRRET(err);

                err = svp->get<std::string, ::pilo::tlv*>(std::string("field_sep"), tlv_val_ptr);
                PILO_ERRRET(err);
                err = tlv_val_ptr->assign_as_str(_field_sep);
                PILO_ERRRET(err);


                return PILO_OK;
            }

            ::pilo::err_t logger::load()
            {
                return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
            }

            ::pilo::err_t logger::save()
            {
                return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
            }

            ::pilo::err_t logger::set_default()
            {
                _type = ::pilo::core::logging::logger_type::local_spmt_text;
                _level = ::pilo::core::logging::level::debug;
                _splition_type = ::pilo::core::logging::splition_type::by_day;
                _name_suffix = 0;
                _headers = ::pilo::core::logging::DefaultHeaders;
                _split_every = 0;
                _outputs = ::pilo::core::logging::DevStdOut;
                _flags = ::pilo::core::logging::DefaultFlags;
                _bak_name_suffix = 0;
                _size_quota = 0;
                _piece_quota = 0;
                _name.clear();
                _bak_dir.clear();
                _line_sep = "\n";
                _field_sep = "\t";
                return PILO_OK;            
            }

            void logger::clear(bool purge)
            {
                PMC_UNUSED(purge);
                set_default();
                _name.clear();
            }

            const::pilo::core::io::path* logger::file_path() const
            {
                return nullptr;
            }

            bool logger::invalid() const
            {
                return _name.empty();
            }


        }
    }
}