#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include "../../pilo.hpp"
#include "../io/formatted_io.hpp"
#include "../string/string_operation.hpp"


#define PMI_FIXED_WIDTH_LINE_FMT_TRUNC    (0x01)
#define PMI_FIXED_WIDTH_LINE_FMT_LEFT_ALIGH (0x02)
#define PMI_FIXED_WIDTH_LINE_FMT_READABLIZE (0x04)


namespace pilo
{
    namespace core
    {
        namespace string
        {
            class fixed_width_line_formater
            {
            public:
                class fixed_width_meta_field
                {
                public:
                    fixed_width_meta_field(::pilo::u8_t width, ::pilo::u8_t flags, const char* name, const char* sep = " ", const char * readable_fmt = "%.2f%s") : _width(width), _flags(flags), _name(name), _sep(sep), _readable_format(readable_fmt) 
                    {

                    }
                    ::pilo::u8_t width() const { return _width; }
                    const std::string name() const { return _name; }
                    const std::string seperator() const { return _sep; }
                    const std::string readable_format() const { return _readable_format; }
                    bool trunc() const { return _flags.test_value(PMI_FIXED_WIDTH_LINE_FMT_TRUNC); }
                    bool left_align() const { return _flags.test_value(PMI_FIXED_WIDTH_LINE_FMT_LEFT_ALIGH); }
                    bool readablize() const { return _flags.test_value(PMI_FIXED_WIDTH_LINE_FMT_READABLIZE); }


                private:
                    ::pilo::u8_t _width;
                    ::pilo::bit_flag<::pilo::u8_t> _flags;
                    const std::string _name;
                    const std::string _sep;
                    const std::string _readable_format;
                };

            public:
                fixed_width_line_formater() = default;

                int add_meta_field(::pilo::u8_t width, ::pilo::u8_t flags, const char* name, const char* sep = " ", const char* rfmt = "%.4f%s")
                {                    
                    _meta_fields.push_back(fixed_width_meta_field(width, flags, name, sep, rfmt));
                    return PILO_OK;
                }

                void format_header(::std::stringstream& ss) const;
                std::string format_header() const;
                void format_seperator(::std::stringstream& ss,char ch, ::pilo::i32_t cnt) const;

                template<typename INT_T>
                void format_field(::std::stringstream& ss, ::pilo::i64_t index, INT_T iv)
                {
                    char buffer[128] = { 0 };
                    if (_meta_fields[index].readablize())
                    {
                        ::pilo::core::string::number_to_string_adv<INT_T>(buffer, PMF_COUNT_OF(buffer), iv, nullptr, _meta_fields[index].readable_format().c_str());
                        if (_meta_fields[index].left_align())
                        {
                            ss << std::left << std::setw(_meta_fields[index].width()) << buffer;
                        }
                        else
                        {
                            ss << std::right << std::setw(_meta_fields[index].width()) << buffer;
                        }
                        ss << _meta_fields[index].seperator();
                    }
                    else
                    {
                        if (_meta_fields[index].left_align())
                        {
                            ss << std::left << std::setw(_meta_fields[index].width()) << std::fixed << iv;
                        }
                        else
                        {
                            ss << std::right << std::setw(_meta_fields[index].width()) << std::fixed << iv;
                        }
                        ss << _meta_fields[index].seperator();
                    }
                }

                template<typename INT_T>
                std::string format_field(::pilo::i64_t index, INT_T iv)
                {
                    ::std::stringstream ss;
                    format_field(ss, index,  iv);
                    return ss.str();
                }

                void format_field(::std::stringstream & ss, ::pilo::i64_t index, const char* str)
                {
                    if (str == nullptr)
                        str = "null-str";
                    std::string rstr = str;
                    ::pilo::u8_t real_len = (::pilo::u8_t) ::pilo::core::string::character_count(str);
                    if (real_len > (::pilo::u8_t)_meta_fields[index].width())
                    {
                        if (_meta_fields[index].trunc())
                        {
                            rstr = rstr.substr(0, (_meta_fields[index].width()));
                        }
                    }
                    if (_meta_fields[index].left_align())
                    {
                        ss << std::left << std::setw(_meta_fields[index].width()) << rstr;
                    }
                    else
                    {
                        ss << std::right << std::setw(_meta_fields[index].width()) << rstr;
                    }
                    ss << _meta_fields[index].seperator();
                }

                std::string format_field(::pilo::i64_t index, const char * str)
                {                    
                    ::std::stringstream ss;
                    format_field(ss, index, str);
                    return ss.str();
                }

            private:
                std::vector<fixed_width_meta_field> _meta_fields;
            };
        }
    }
}