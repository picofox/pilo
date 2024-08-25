#include "./cmdline_args.hpp"

namespace pilo
{
    namespace core
    {
        namespace process
        {

            cmdline_spec::cmdline_spec(char short_name, ::pilo::u8_t wt, char key_type_char, char val_type_char, bool is_required, ::pilo::i32_t min_sa, ::pilo::i32_t max_sa, const char* long_name)
            {
                _short_name = short_name;
                _wrapper_type = wt;
                _key_type = ::pilo::core::rtti::wired_type::s_char_to_key_type(key_type_char);
                _val_type = ::pilo::core::rtti::wired_type::s_char_to_value_type(val_type_char);
                if (is_required)
                    _flags.mark_value(Required);
                else
                    _flags.clear_value(Required);
                _min_sub_arg_count = min_sa;
                _max_sub_arg_count = max_sa;
                _long_name = long_name;
            }

            cmdline_spec::cmdline_spec(const char* spec_str_fld, ::pilo::i64_t len)
            {
                _val_type = ::pilo::core::rtti::wired_type::value_type_na;

                ::pilo::cstr_ref<char> parts[6];
                ::pilo::i64_t cnt = ::pilo::core::string::split_fixed(spec_str_fld, len, ":", 1, parts, 6, false, true, true, true);

                if (cnt < 6) return;

                if (parts[0].length < 1)
                    _short_name = 0;
                else
                    _short_name = parts[0].ptr[0];

                if (parts[1].length < 2)
                    _long_name.clear();
                else
                    _long_name.assign(parts[1].ptr, parts[1].length);

                if (_short_name == 0 && _long_name.empty()) return;

                PILO_ERRRET_VOID(::pilo::core::rtti::wired_type::s_parse_cstr_type(this->_wrapper_type, this->_key_type, this->_val_type, parts[2].ptr, parts[2].length));

                PILO_ERRRET_VOID(::pilo::core::string::string_to_number(this->_min_sub_arg_count, parts[3].ptr, parts[3].length));
                PILO_ERRRET_VOID(::pilo::core::string::string_to_number(this->_max_sub_arg_count, parts[4].ptr, parts[4].length));

                bool isreq = false;
                PILO_ERRRET_VOID(::pilo::core::rtti::wired_type::s_parse_cstr_bool(isreq, parts[5].ptr, parts[5].length));

                if (isreq)
                    _flags.mark_value(Required);
                else
                    _flags.clear_value(Required);

                return;
            }

            cmdline_arg::~cmdline_arg()
            {
            }

            ::pilo::err_t cmdline_arg::parse(const char* src, ::pilo::i64_t cslen)
            {
                PMC_UNUSED(src);
                PMC_UNUSED(cslen);

                return PILO_OK;
            }

        }
    }
}