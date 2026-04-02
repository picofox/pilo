#include    "../rtti/wired_type.hpp"
#include    <vector>
#include    "../../tlv.hpp"
#include    "../dp/xls_spread_sheet.hpp"


namespace pilo
{
    namespace core
    {
        namespace config
        {
            class xls_config_set;


            class xls_config_field
            {
            public:
                friend xls_config_set;

            public:
                xls_config_field() : _index(-1), _column(0)
                {

                }

            public:
                ::pilo::i32_t index() const { return _index;  };
                ::pilo::u32_t column() const { return _column; }
                const std::string& name() const { return _name; }
                const ::pilo::i16_t value_type() const { return _wired_type.value_type(); }
                const ::pilo::i8_t key_type() const { return _wired_type.key_type(); }
                const ::pilo::i8_t wrapper_type() const { return _wired_type.wrapper_type(); }

                void set_name(const std::string& s) { _name = s; }
                void set_value_type(::pilo::i16_t vt) { _wired_type.set_value_type(vt); }
                void set_key_type(::pilo::i8_t kt) { _wired_type.set_key_type(kt); }
                void set_wrapper_type(::pilo::i8_t wt) { _wired_type.set_wrapper_type(wt); }
                void set_default_value_str(const std::string& s) { _default_value_str = s; }

                std::string to_string() const;

            private:
                ::pilo::i32_t _index;
                ::pilo::u32_t _column;
                std::string _name;
                ::pilo::core::rtti::wired_type  _wired_type;
                std::string _default_value_str;
                ::pilo::bit_flag<::pilo::u8_t>  _flags;
            };

            class xls_config_set;

            class xls_config
            {
            public:
                static ::pilo::err_t s_ui_parser(const char* src, const char* ptr, ::pilo::i64_t len, void* ctx);
                static ::pilo::err_t s_ui_sub_parser(const char* src, const char* ptr, ::pilo::i64_t len, void* ctx);
            public:
                friend xls_config_set;

            public:
                xls_config();
                ~xls_config();

            public:
                const std::string& cls_name() const { return _cls_name;  }
                const std::string& config_file_name() const { return _config_file_name; }
                const std::string& ns() const { return _ns; }
                void set_cls_name(const std::string& s) { _cls_name = s;  }
                void set_config_file_name(const std::string& s) { _config_file_name = s; }
                void set_ns(const std::string& s) { _ns = s; }
                ::pilo::err_t parse_union_index(const char* ptr, ::pilo::i64_t len);
                const std::vector<std::string>& union_index_at(int idx) { return _union_indices[idx];}
                void reset();
                ::pilo::u32_t field_count() const;
                std::string to_string() const;
                ::pilo::err_t save_config_file(const char* path_str, predefined_pilo_path prefix);

            private:
                std::string _cls_name;
                std::string _config_file_name;
                std::string _ns;
                std::vector<std::vector<std::string>>  _union_indices;
                std::vector<xls_config_field> _fields;
                ::pilo::tlv *                 _data;
            };

            class xls_config_set
            {
            public:
                const static int server = 0;
                const static int client = 1;
                const static ::pilo::u8_t   flag_primary_key = 0x1; //p
                const static ::pilo::u8_t   flag_unique = 0x2; //u
                const static ::pilo::u8_t   flag_index = 0x4; //i
                const static ::pilo::u8_t   flag_nullable = 0x8; //n


            public:
                void reset();

            public:
                const std::string& sheet_name() const { return _sheet_name; }
                const std::string& name() const { return _name; }
                const std::string& desc() const { return _desc; }
                //void set_sheet_name(const std::string& s) { _sheet_name = s; }
                void set_name(const std::string& s) { _name = s; }
                void set_desc(const std::string& s) { _desc = s; }
                const xls_config& server_config() const { return _configs[xls_config_set::server]; }
                const xls_config& client_config() const { return _configs[xls_config_set::client]; }
                ::pilo::err_t parse(const std::string& sheet_name, const char* path_str, ::pilo::predefined_pilo_path prefix, std::string& errmsg);
                ::pilo::err_t save_config_file(int which, const char* path_str, predefined_pilo_path prefix);
                std::string to_string() const;


            protected:
                void _compose_errmsg(std::string& errmsg, ::pilo::u32_t row, ::pilo::u32_t col, const char* fmt, ...);
                ::pilo::err_t _parse_field(int which, const std::string& field_str,::pilo::u32_t row, ::pilo::u32_t col);
                ::pilo::err_t _parse_record(int which, ::pilo::u32_t row, ::pilo::core::dp::xls_spread_sheet* xssp, std::string& errmsg);

            private:
                std::string  _sheet_name;
                std::string  _name;
                std::string  _desc;
                xls_config _configs[2];
            };
        }
    }
}
