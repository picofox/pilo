#ifndef _pilo_core_rtti_meta_variable_hpp_
#define _pilo_core_rtti_meta_variable_hpp_

#include    "../../pilo.hpp"
#include    "../string/string_operation.hpp"
#include    "./meta_src_node.hpp"
#include    <sstream>


namespace pilo
{
    namespace core
    {
        namespace rtti
        {
            class meta_variable : public meta_src_node
            {
            public:
                meta_variable(::pilo::i16_t indent, ::pilo::u64_t modifiers, ::pilo::u32_t accessor_flag, const std::string& namestr, const std::string& typestr, const std::string& valuestr)
                    : meta_src_node(meta_node_type_enum::var, indent, modifiers)
                    , _m_accessor_flags(accessor_flag)
                    , _m_name(namestr), _m_type(typestr), _m_value(valuestr)

                {
                    if (this->_m_modifiers.test_value(mod_prefix)) {
                        this->_m_name = s_prefix_to_variable_name(this->_m_name, this->access_priv_type());
                    }
                }
                ~meta_variable();

                // Inherited via meta_src_node
                ::pilo::err_t append_to_stringstream_cpp(std::stringstream& ss, ::pilo::u32_t flags, const std::string& strparam = "", const char* indent_cstr = nullptr, ::pilo::i16_t effect_indent = -1) const override;




                void append_as_param_string(std::stringstream& ss,::pilo::u32_t flags) const;
                const ::pilo::bit_flag<::pilo::u64_t>& modifiers() { return _m_modifiers; }
                const std::string& name() const { return _m_name; }
                const std::string& type() const { return _m_type; }
                const std::string& value() const { return _m_value; }

                void set_name(const std::string& s) { _m_name = s; }
                void set_type(const std::string& s) { _m_type = s; }
                void set_value(const std::string& s) { _m_value = s; }
                void add_setter_flag(::pilo::u8_t f) { _m_accessor_flags.set_value(f, true); }
                void clear_setter_flag(::pilo::u8_t f) { _m_accessor_flags.set_value(f, false); }
                void set_setter_flag(::pilo::u8_t all_flags) { _m_accessor_flags.set(all_flags); }


                void reset()
                {
                    _m_modifiers.reset();
                    _m_accessor_flags.reset();
                    _m_name.clear();
                    _m_type.clear();
                    _m_value.clear();
                }

            protected:                
                ::pilo::bit_flag<::pilo::u32_t> _m_accessor_flags;
                std::string _m_name;
                std::string _m_type;
                std::string _m_value;
                

            };
        }
    }
}


#endif // !_pilo_core_rtti_meta_variable_hpp_




