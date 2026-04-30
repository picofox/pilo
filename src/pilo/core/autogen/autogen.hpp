#ifndef _pilo_core_autogen_autogen_hpp_
#define _pilo_core_autogen_autogen_hpp_

#include    <string>


namespace pilo
{
    namespace core
    {
        namespace autogen
        {

            class autogen_config
            {
            public:
                autogen_config()
                    : _newline_sep("\n"), _indent_str("    ")
                {}

                const std::string& newline_sep() const
                {
                    return _newline_sep;
                }

                void set_unix_style_newline_sep()
                {
                    _newline_sep = "\n";
                }

                void set_win_style_newline_sep()
                {
                    _newline_sep = "\r\n";
                }



                const std::string& indent_str() const
                {
                    return _indent_str;
                }

                void set_indent_str(const std::string& indent_str)
                {
                    _indent_str = indent_str;
                }


            private:
                std::string  _newline_sep;
                std::string  _indent_str;
            };

            extern autogen_config g_autogen_config;
        }                
    }
}



#endif