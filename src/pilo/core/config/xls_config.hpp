#include    "../rtti/wired_type.hpp"
#include    <vector>
#include    <array>
#include    "../../tlv.hpp"
#include    "../dp/xls_spread_sheet.hpp"
#include    "../logging/logger_def.hpp"
 

#define     PMI_XLS_GEN_ERR_BUFF_SIZE   (1024)

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
                friend class xls_config_generator;
                friend xls_config_set;

            public:
                xls_config_field() :_pri(-1), _index(-1), _column(0), _flags(0)
                {

                }

                xls_config_field(xls_config_field& rhs) = delete;
                xls_config_field& operator=(const xls_config_field& rhs) = delete;

                xls_config_field(xls_config_field&& rhs) noexcept
                    :_pri(rhs._pri),_index(rhs._index),_column(rhs._column),_flags(std::move(rhs._flags))
                    ,_name(std::move(rhs._name)),_wired_type(std::move(rhs._wired_type)), _default_value_str(std::move(rhs._default_value_str))
                {
                    
                }

                xls_config_field& operator=(xls_config_field&& rhs) noexcept
                {
                    if (this != &rhs) {
                        _pri = rhs._pri;
                        _index = rhs._index;
                        _column = rhs._column;
                        _flags = std::move(rhs._flags);
                        _name = std::move(rhs._name);
                        _wired_type = std::move(rhs._wired_type);
                        _default_value_str = std::move(rhs._default_value_str);
                    }
                    return *this;
                }

            public:
                ::pilo::i32_t pri() const { return _pri; }
                ::pilo::i32_t index() const { return _index;  };
                ::pilo::u32_t column() const { return _column; }
                const std::string& name() const { return _name; }
                const ::pilo::i16_t value_type() const { return _wired_type.value_type(); }
                const ::pilo::i8_t key_type() const { return _wired_type.key_type(); }
                const ::pilo::i8_t wrapper_type() const { return _wired_type.wrapper_type(); }
                const std::string& default_value() const { return _default_value_str; }
                

                void set_name(const std::string& s) { _name = s; }
                void set_value_type(::pilo::i16_t vt) { _wired_type.set_value_type(vt); }
                void set_key_type(::pilo::i8_t kt) { _wired_type.set_key_type(kt); }
                void set_wrapper_type(::pilo::i8_t wt) { _wired_type.set_wrapper_type(wt); }
                void set_default_value_str(const std::string& s) { _default_value_str = s; }
                bool test_flag(::pilo::u32_t flag) { return _flags.test_value(flag);  }

                std::string to_string() const;

            private:
                ::pilo::i32_t _pri;
                ::pilo::i32_t _index;
                ::pilo::u32_t _column;
                ::pilo::bit_flag<::pilo::u32_t>  _flags;
                std::string _name;
                ::pilo::core::rtti::wired_type  _wired_type;
                std::string _default_value_str;
                
            };

            class xls_config_set;

            class xls_config
            {
            public:
                friend class xls_config_generator;

            public:
                static ::pilo::err_t s_ui_parser(const char* src, const char* ptr, ::pilo::i64_t len, void* ctx);
                static ::pilo::err_t s_ui_sub_parser(const char* src, const char* ptr, ::pilo::i64_t len, void* ctx);
            public:
                friend xls_config_set;

            public:
                xls_config();
                ~xls_config();

            public:
                xls_config(xls_config& rhs) = delete;
                xls_config& operator=(const xls_config& rhs) = delete;

                xls_config(xls_config&& rhs) noexcept
                    : _cls_name(std::move(rhs._cls_name))
                    , _config_file_name(std::move(rhs._config_file_name))
                    , _ns(std::move(rhs._ns))
                    , _union_indices(std::move(rhs._union_indices))
                    , _fields(std::move(rhs._fields))   
                    , _cls_fields_index_map(rhs._cls_fields_index_map)

                {
                    _data = rhs._data;
                    rhs._data = nullptr;
                }

                xls_config& operator=(xls_config&& rhs) noexcept;
                

            public:
                const std::string& cls_name() const { return _cls_name;  }
                const std::string& config_file_name() const { return _config_file_name; }
                const std::string& ns() const { return _ns; }
                ::pilo::err_t parse_union_index(const char* ptr, ::pilo::i64_t len);
                const std::vector<std::string>& union_index_at(int idx) { return _union_indices[idx];}
                void reset();
                ::pilo::u32_t field_count() const;
                std::string to_string() const;
                ::pilo::i32_t find_lowest_pri_field() const;
                ::pilo::i32_t find_existing_field_idx_by_name(const std::string& name) const;

            private:
                std::string _cls_name;
                std::string _config_file_name;
                std::string _ns;
                std::vector<std::vector<std::string>>  _union_indices;
                std::vector<xls_config_field> _fields;
                std::vector<std::pair<::pilo::i32_t, ::pilo::i32_t>> _cls_fields_index_map;
                ::pilo::tlv *                 _data;
            };


            class xls_config_set
            {
            public:
                friend class xls_config_generator;

            public:
                const static int server = 0;
                const static int client = 1;
                const static ::pilo::u32_t   flag_primary_key = 0x1; //p
                const static ::pilo::u32_t   flag_unique = 0x2; //u
                const static ::pilo::u32_t   flag_index = 0x4; //i
                const static ::pilo::u32_t   flag_nullable = 0x8; //n
                const static ::pilo::u32_t   flag_primary_key_array = 0x10; //P

            public:
                xls_config_set() {}

                xls_config_set(const xls_config_set&) = delete;
                xls_config_set& operator=(const xls_config_set&) = delete;

                xls_config_set(xls_config_set&& rhs) noexcept
                    : _xls_name(std::move(rhs._xls_name))
                    , _sheet_name(std::move(rhs._sheet_name))
                    , _desc(std::move(rhs._desc))
                    , _configs(std::move(rhs._configs))
                {

                }

                xls_config_set& operator=(xls_config_set&& rhs) noexcept
                {
                    if (this != &rhs) {
                        _xls_name = std::move(rhs._xls_name);
                        _sheet_name = std::move(rhs._sheet_name);
                        _desc = std::move(rhs._desc);
                        _configs = std::move(rhs._configs);

                    }
                    return *this;
                }


            public:
                void reset();

            public:
                const std::string& xls_name() const { return _xls_name; }
                const std::string& sheet_name() const { return _sheet_name; }
                const std::string& desc() const { return _desc; }
                const xls_config& server_config() const { return _configs[xls_config_set::server]; }
                const xls_config& client_config() const { return _configs[xls_config_set::client]; }
                std::string to_string() const;

            private:
                std::string                 _xls_name;
                std::string                 _sheet_name;
                std::string                 _desc;
                std::array<xls_config,2>    _configs;
            };

            ::pilo::err_t s_xls_file_iter_func(::pilo::i8_t event_type, const ::pilo::core::io::path* src_path, ::pilo::i8_t fsnt, ::pilo::i32_t layer_idx, ::pilo::i32_t file_idx, void* ctx);

            class xls_config_generator
            {
            public:
                enum class parse_phase_enum
                {
                    header_vars = 0,
                    col_spec = 1,
                    row_data = 2,
                };

                static ::pilo::err_t s_xls_file_iter_func(::pilo::i8_t event_type, const ::pilo::core::io::path* src_path, ::pilo::i8_t fsnt, ::pilo::i32_t layer_idx, ::pilo::i32_t file_idx, void* ctx);
            public:
                xls_config_generator() 
                {  
                    
                }

                xls_config_generator(const xls_config_generator&) = delete;           
                xls_config_generator& operator=(const xls_config_generator&) = delete; 

                xls_config_generator(xls_config_generator&&) = delete;
                xls_config_generator& operator=(xls_config_generator&&) = delete;   

            public:
                ::pilo::err_t set(const char* xls_dir_path, ::pilo::predefined_pilo_path xls_dir_path_base
                    , const char* dest_server_config_dir_path, ::pilo::predefined_pilo_path dest_server_config_path_base
                    , const char* dest_client_config_dir_path, ::pilo::predefined_pilo_path dest_client_config_dir_path_base
                    , const char* dest_server_source_dir_path, ::pilo::predefined_pilo_path dest_server_source_dir_path_base
                    , const char* dest_client_source_dir_path, ::pilo::predefined_pilo_path dest_client_source_dir_path_base);

                void clear();

                ::pilo::err_t parse();
                ::pilo::err_t generate_server_config();
                ::pilo::err_t generate_client_config();
                ::pilo::err_t generate_server_source();
                ::pilo::err_t generate_client_source();                

                void add_log(::pilo::core::logging::level level, ::pilo::u32_t row, ::pilo::u32_t col, const char* fmt, ...);
                void add_log(::pilo::core::logging::level level, const char* fmt, ...);
                void add_log(::pilo::core::logging::level level, const std::string & msg);

                const std::vector<::pilo::core::logging::info_item>& logs() const { return _logs;  }
                std::map<std::string, xls_config_set>& config_set() { return _config_set_map;  }


            private:
                ::pilo::err_t _parse_xls(const char* filename);
                ::pilo::err_t _parse_worksheet(::pilo::core::dp::xls_spread_document & doc, const char* filename, ::pilo::u32_t ws_idx);
                ::pilo::err_t _parse_field_spec(xls_config_set& cfg_set, int which, const std::string& field_str, ::pilo::u32_t row, ::pilo::u32_t col,const char* xlsfullfilepath, const char* wsnamecstr);
                ::pilo::err_t _parse_record(xls_config_set& cfg_set, int which, ::pilo::u32_t row, ::pilo::core::dp::xls_spread_sheet* wsp, const char* xlsfullfilepath, const char* wsnamecstr);
                bool _check_and_make_default_for_two_vars(const char* t1, const char* t2, std::string& a, std::string& b, const char* file, const char* wsname);
                bool _check_duplicate_vars_in_header(const std::string& ccname, const std::string& scname, const std::string& ccfg, const std::string& scfg, const char* file, const char* wsname);

                ::pilo::err_t _generate_config(int which, const char* name_of_which);

            private:
                ::pilo::core::io::path          _xls_dir_path;
                ::pilo::core::io::path          _dest_config_dir_path[2];
                ::pilo::core::io::path          _dest_source_dir_path[2];
                std::vector<::pilo::core::logging::info_item>   _logs;
                std::map<std::string, std::string> _ccnames;
                std::map<std::string, std::string> _scnames;
                std::map<std::string, std::string> _ccfgs;
                std::map<std::string, std::string> _scfgs;
                std::map<std::string, xls_config_set>  _config_set_map;                
            };

            
        }
    }
}
