#include "./fixed_width_line_formater.hpp"

namespace pilo
{
    namespace core
    {
        namespace string
        {
            std::string fixed_width_line_formater::format_header() const
            {
                ::std::stringstream ss;
                for (::pilo::i64_t i = 0; i < (::pilo::i64_t) _meta_fields.size(); i++)
                {
                    ::pilo::u8_t real_len = (::pilo::u8_t)_meta_fields[i].name().size();
                    ::pilo::u8_t diff = 0;
                    if (real_len < (::pilo::u8_t)_meta_fields[i].width())
                    {
                        diff = (::pilo::u8_t)_meta_fields[i].width() - real_len;
                    }
                    if (_meta_fields[i].left_align())
                    {
                        ss << _meta_fields[i].name();
                        for (int j = 0; j < diff; j++)
                        {
                            ss << ' ';
                        }
                    }
                    else
                    {
                        for (int j = 0; j < diff; j++)
                        {
                            ss << ' ';
                        }
                        ss << _meta_fields[i].name();
                    }
                    ss << _meta_fields[i].seperator();
                }
                ss << std::endl;
                return ss.str();;
            }
        }
    }
}