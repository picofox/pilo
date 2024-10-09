#include "./cmdline_args.hpp"
#include "./context.hpp"

namespace pilo
{
    namespace core
    {
        namespace process
        {
            static std::string arg_tlv_to_string(const ::pilo::core::config::cmdline_spec* spec, ::pilo::tlv* tlvp)
            {
                std::string str;
                char buffer[64] = { 0 };
                const char* ptr = buffer;
                ::pilo::core::rtti::wired_type::s_to_wired_type_cstr(buffer, sizeof(buffer), tlvp->wrapper_type(), tlvp->key_type(), tlvp->value_type());

                str += spec->name();
                str += " (";
                str += buffer;
                str += ")";
                if (spec->is_single()) {
                    str += ":";
                    ptr = tlvp->as_cstr(buffer, sizeof(buffer), nullptr, nullptr, nullptr);
                    str += ptr;
                }
                else if (spec->is_list()) {
                    str += ":";
                    str += tlvp->value_to_string();
                }
                else if (spec->is_dict()) {
                    str += ":";
                    str += tlvp->value_to_string();
                }
                return str;
            }

            cmdline_arg::~cmdline_arg()
            {
                for (std::size_t i = 0; i < _targets.size(); i++) {
                    ::pilo::tlv::deallocate(_targets[i]);
                }
                _targets.clear();
                for (int c = 0; c < 128; c++) {
                    if (_short_args[c] != nullptr) {
                        ::pilo::tlv::deallocate(_short_args[c]);
                        const ::pilo::core::config::cmdline_spec* spec = PILO_CONTEXT->core_config()->cmdline_arg_spec().spec((char)c);
                        if (spec == nullptr) {
                            PMC_ASSERT(false);
                            return;
                        }
                        if (! spec->long_name().empty()) {
                            _long_args.erase(spec->long_name());
                        }
                    }
                }
                std::map<std::string, ::pilo::tlv*>::const_iterator cit = _long_args.cbegin();
                for (; cit != _long_args.cend(); cit++) {
                    ::pilo::tlv::deallocate(cit->second);
                }
                _long_args.clear();
            }

            ::pilo::err_t cmdline_arg::parse(int argc, char* argv[], std::string& errmsg)
            {
                ::pilo::i32_t arg_len = 0;
                ::pilo::err_t err = PILO_OK;                               
                const ::pilo::core::config::cmdline_spec* spec = nullptr;
                bool is_all_set = false;
                bool do_targets = false;

                for (int i = 1; i < argc; i++) {
                    arg_len = (::pilo::i32_t) ::pilo::core::string::character_count(argv[i]);
                    if (arg_len < 0)
                        return ::pilo::mk_perr(PERR_INVALID_PARAM);

                    if (spec == nullptr) {
                        if (arg_len > 1 && argv[i][0] == '-' && argv[i][1] == '-') {
                            if (do_targets) {
                                _compose_errmsg(errmsg, "Need a Target, but got a arg ", argv[i], "", nullptr);
                                return ::pilo::mk_perr(PERR_INVALID_PARAM);
                            }
                            if (arg_len < 3) {
                                _compose_errmsg(errmsg, "Long-arg ", argv[i], "is not followed by valid content.", nullptr);
                                return ::pilo::mk_perr(PERR_INVALID_PARAM);
                            }
                            spec = PILO_CONTEXT->core_config()->cmdline_arg_spec().spec(std::string(argv[i]+2, arg_len-2));
                            if (spec == nullptr) {
                                _compose_errmsg(errmsg, "Spec for Long-arg ", argv[i], " not found.", nullptr);
                                return ::pilo::mk_perr(PERR_INVALID_PARAM);
                            }

                            err = _set_arg(spec, errmsg, nullptr, 0, &is_all_set);
                            if (err != PILO_OK) {
                                return err;
                            }
                            if (spec->is_flag()) {
                                spec = nullptr;
                            }
                        }
                        else if (arg_len > 0 && argv[i][0] == '-') {
                            if (do_targets) {
                                _compose_errmsg(errmsg, "Need a Target, but got a arg ", argv[i], "", nullptr);
                                return ::pilo::mk_perr(PERR_INVALID_PARAM);
                            }
                            if (arg_len == 1) {
                                _compose_errmsg(errmsg, "Found Short-arg has no real content", nullptr , nullptr, nullptr);
                                return ::pilo::mk_perr(PERR_INVALID_PARAM);
                            }
                            else if (arg_len == 2) {
                                spec = PILO_CONTEXT->core_config()->cmdline_arg_spec().spec(argv[i][1]);
                                if (spec == nullptr) {
                                    _compose_errmsg(errmsg, "Spec for Short-arg ", argv[i], "not found.", nullptr);
                                    return ::pilo::mk_perr(PERR_INVALID_PARAM);
                                }
                                err = _set_arg(spec, errmsg, nullptr, 0, &is_all_set);
                                if (err != PILO_OK) {
                                    return err;
                                }
                                if (spec->is_flag()) {
                                    spec = nullptr;
                                }
                            }
                            else if (arg_len > 2) {
                                spec = PILO_CONTEXT->core_config()->cmdline_arg_spec().spec(argv[i][1]);
                                if (spec == nullptr) {
                                    _compose_errmsg(errmsg, "Spec for Short-arg ", argv[i], "not found.", nullptr);
                                    return ::pilo::mk_perr(PERR_INVALID_PARAM);
                                }
                                err = _set_arg(spec, errmsg, nullptr, 0, &is_all_set);
                                if (err != PILO_OK) {
                                    return err;
                                }
                                if (spec->is_flag()) { //-f??
                                    bool is_all_flag = _is_all_flag(argv[i] + 2, arg_len - 2);
                                    if (is_all_flag) { //-fgh
                                        for (int k = 0; k < arg_len - 2; k++) {
                                            char sc = argv[i][2 + k];
                                            spec = PILO_CONTEXT->core_config()->cmdline_arg_spec().spec(sc);
                                            if (spec == nullptr) {
                                                _compose_errmsg(errmsg, "Spec for Short-arg ", "", "not found.", nullptr);
                                                return ::pilo::mk_perr(PERR_INVALID_PARAM);
                                            }
                                            err = _set_arg(spec, errmsg, nullptr, 0, &is_all_set);
                                            if (err != PILO_OK) {
                                                return err;
                                            }
                                        }
                                        spec = nullptr;
                                    }
                                    else { // -fsubarg err
                                        _compose_errmsg(errmsg, "Spec for Short-arg ", spec->name().c_str(), "is flag can't have sub arg:", argv[i]+2);
                                        return ::pilo::mk_perr(PERR_INVALID_PARAM);
                                    }
                                } else { //-a
                                    err = _set_arg(spec, errmsg, argv[i] + 2, arg_len - 2, &is_all_set);
                                    if (err != PILO_OK) {
                                        return err;
                                    }
                                }


                            }
                        }
                        else {
                            do_targets = true;
                            spec = nullptr;
                            err = PILO_CONTEXT->core_config()->cmdline_arg_spec().target_spec().can_append_sub_arg((::pilo::i32_t)this->_targets.size());
                            if (err != PILO_OK) {
                                _compose_errmsg(errmsg, "Target full, can not add ", argv[i], nullptr, nullptr);
                                return err;
                            }
                            err = _make_target(argv[i], arg_len);
                            if (err != PILO_OK) {
                                _compose_errmsg(errmsg, "make target ", argv[i], "failed", nullptr);
                                return err;
                            }
                        }
                    }
                    else { //spec is valid
                        if (spec->is_flag()) {
                            _compose_errmsg(errmsg, "Internal Error when parsing arg : ", argv[i], "", nullptr);
                            return ::pilo::mk_perr(PERR_UNDEF);
                        }
                        else if (spec->is_single()) {                            
                            err = _set_arg(spec, errmsg, argv[i], arg_len, &is_all_set);
                            if (err != PILO_OK) {
                                return err;
                            }
                            spec = nullptr;
                        }
                        else if (spec->is_list()) {
                            err = _set_arg(spec, errmsg, argv[i], arg_len, &is_all_set);
                            if (err != PILO_OK) {
                                return err;
                            }
                            if (is_all_set)
                                 spec = nullptr;
                            else {
                                if (i < argc - 1) {
                                    ::pilo::i32_t next_arg_len = (::pilo::i32_t) ::pilo::core::string::character_count(argv[i + 1]);
                                    if (next_arg_len < 0)
                                        return ::pilo::mk_perr(PERR_INVALID_PARAM);
                                    if (next_arg_len > 0 && argv[i+1][0] == '-') {
                                        spec = nullptr;
                                    }
                                }
                            }
                        }
                        else if (spec->is_dict()) {
                            err = _set_arg(spec, errmsg, argv[i], arg_len, &is_all_set);
                            if (err != PILO_OK) {
                                return err;
                            }
                            if (is_all_set)
                                spec = nullptr;
                            else {
                                if (i < argc - 1) {
                                    ::pilo::i32_t next_arg_len = (::pilo::i32_t) ::pilo::core::string::character_count(argv[i + 1]);
                                    if (next_arg_len < 0)
                                        return ::pilo::mk_perr(PERR_INVALID_PARAM);
                                    if (next_arg_len > 0 && argv[i + 1][0] == '-') {
                                        spec = nullptr;
                                    }
                                }
                            }
                        }
                        else {
                            _compose_errmsg(errmsg, "Invalid Spec while parsing cmdline arg", spec->name().c_str(), nullptr, nullptr);
                            return ::pilo::mk_err(PERR_MIS_DATA_TYPE);
                        }
                    }
                }

                err = _validate(errmsg);

                return err;
            }

            std::string cmdline_arg::args_to_string() const
            {
                std::stringstream ss;
                bool not_first = false;
                for (int i = 0; i < 128; i++) {
                    if (this->_short_args[i] != nullptr) {
                        if (not_first) {
                            ss << ", ";                            
                        }
                            
                        const ::pilo::core::config::cmdline_spec* sp = PILO_CONTEXT->core_config()->cmdline_arg_spec().spec((char)i);                   
                        ss << arg_tlv_to_string(sp, _short_args[i]);  
                        if (!not_first)
                            not_first = true;
                    }
                }

                std::map<std::string, ::pilo::tlv*>::const_iterator cit = _long_args.cbegin();
                for (; cit != _long_args.cend(); cit++) {

                    const ::pilo::core::config::cmdline_spec* sp = PILO_CONTEXT->core_config()->cmdline_arg_spec().spec(cit->first);
                    if (sp == nullptr) {
                        PMC_ASSERT(false)
                    }
                    else {                        
                        if (sp->short_name() < 0) {
                            if (not_first) {
                                ss << ", ";
                            }
                            ss << arg_tlv_to_string(sp, cit->second);
                            if (!not_first)
                                not_first = true;
                        }                           
                        
                    }
                    
                }

                return ss.str();
            }

            std::string cmdline_arg::targets_to_string() const
            {
                std::stringstream ss;
                char buffer[64] = { 0 };
                const char* ptr = buffer;

                for (int i = 0; i < _targets.size(); i++) {
                    if (i != 0)
                        ss << ", ";
                    ptr = _targets[i]->as_cstr(buffer, sizeof(buffer), nullptr, nullptr, nullptr);
                    ss << ptr;
                }
                ss << std::endl;
                return ss.str();
            }

            ::pilo::tlv* cmdline_arg::find_arg(char s_name, const std::string& str) const
            {
                if (s_name >= 0) {
                    if (_short_args[s_name] != nullptr)
                        return _short_args[s_name];
                }
                if (!str.empty()) {
                    std::map<std::string, ::pilo::tlv*>::const_iterator it = _long_args.find(str);
                    if (it != _long_args.cend())
                        return it->second;
                }
                return nullptr;
            }

            ::pilo::err_t cmdline_arg::_validate(std::string& errmsg)
            {         
                const ::pilo::core::config::cmdline_spec* spec = nullptr;
                if (_targets.size() > 0) {
                    spec = &(PILO_CONTEXT->core_config()->cmdline_arg_spec().target_spec());
                    if (spec == nullptr) {
                        _compose_errmsg(errmsg, "No target spec is specified", nullptr, nullptr, nullptr);
                        return ::pilo::mk_perr(PERR_INVALID_PARAM);
                    }
                    if (spec->max_count() >= 0 && _targets.size() > spec->max_count()) {
                        _compose_errmsg(errmsg, "Too many targets specified. ", nullptr, nullptr, nullptr);
                        return ::pilo::mk_perr(PERR_INVALID_PARAM);
                    }
                    if (spec->min_count() >= 0 && _targets.size() < spec->min_count()) {
                        _compose_errmsg(errmsg, "Too few targets specified. ", nullptr, nullptr, nullptr);
                        return ::pilo::mk_perr(PERR_INVALID_PARAM);
                    }
                }

                for (int c = 0; c < 128; c++) {
                    if (_short_args[c] != nullptr) {
                        spec = PILO_CONTEXT->core_config()->cmdline_arg_spec().spec((char)c);
                        if (spec == nullptr) {
                            _compose_errmsg(errmsg, "spec for Arg by short arg is not found", nullptr, nullptr, nullptr);
                            return ::pilo::mk_perr(PERR_INVALID_PARAM);;
                        }
                        if (spec->is_list() || spec->is_dict()) {
                            if (spec->min_count() >= 0 && _short_args[c]->size() < spec->min_count()) {
                                _compose_errmsg(errmsg, "Too few sub-arg specified for Arg ", spec->name().c_str(), nullptr, nullptr);
                                return ::pilo::mk_perr(PERR_INVALID_PARAM);
                            }
                            if (spec->max_count() >= 0 && _short_args[c]->size() > spec->max_count()) {
                                _compose_errmsg(errmsg, "Too many sub-arg specified for Arg ", spec->name().c_str(), nullptr, nullptr);
                                return ::pilo::mk_perr(PERR_INVALID_PARAM);
                            }
                        }
                    }
                }
                std::map<std::string, ::pilo::tlv*>::const_iterator cit = _long_args.cbegin();
                for (; cit != _long_args.cend(); cit++) {
                    spec = PILO_CONTEXT->core_config()->cmdline_arg_spec().spec(cit->first);
                    if (spec == nullptr) {
                        _compose_errmsg(errmsg, "spec for Arg by long arg ", spec->name().c_str(), " cmd is not found", nullptr);
                        return ::pilo::mk_perr(PERR_INVALID_PARAM);;
                    }
                    if (spec->short_name() < 0) {                    
                        if (spec->is_list() || spec->is_dict()) {
                            if (spec->min_count() >= 0 && cit->second->size() < spec->min_count()) {
                                _compose_errmsg(errmsg, "Too few sub-arg specified for Arg ", spec->name().c_str(), nullptr, nullptr);
                                return ::pilo::mk_perr(PERR_INVALID_PARAM);
                            }
                            if (spec->max_count() >= 0 && cit->second->size() > spec->max_count()) {
                                _compose_errmsg(errmsg, "Too many sub-arg specified for Arg ", spec->name().c_str(), nullptr, nullptr);
                                return ::pilo::mk_perr(PERR_INVALID_PARAM);
                            }
                        }
                    }
                }

                return PILO_OK;
            }

            ::pilo::err_t  cmdline_arg::_make_target(const char* arg, ::pilo::i32_t len)
            {
                const ::pilo::core::config::cmdline_spec& sp = PILO_CONTEXT->core_config()->cmdline_arg_spec().target_spec();
                ::pilo::tlv* tlvp = ::pilo::tlv::allocate(sp.wrapper_type(), sp.key_type(), sp.value_type());
                if (tlvp == nullptr) {
                    return ::pilo::mk_perr(PERR_NULL_PTR);
                }
                ::pilo::err_t err = PILO_OK;
                PILO_CHKERR_RET(err, tlvp->set_value(arg, len));
                _targets.push_back(tlvp);
                return PILO_OK;
            }

            ::pilo::err_t cmdline_arg::_set_arg(const ::pilo::core::config::cmdline_spec* spec, std::string& errmsg, const char* arg, ::pilo::i32_t len, bool* is_all_set)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::set_if_ptr_is_not_null(is_all_set, false);
                ::pilo::tlv* tlvp = find_arg(spec->short_name(), spec->long_name());
                if (spec->is_flag()) {
                    ::pilo::set_if_ptr_is_not_null(is_all_set, true);
                    if (tlvp != nullptr)
                        return PILO_OK;
                    tlvp = ::pilo::tlv::allocate();                    
                    if (tlvp == nullptr) {
                        ::pilo::tlv::deallocate(tlvp);
                        _compose_errmsg(errmsg, "Create TLV (f) for", spec->name().c_str(), "Failed", nullptr);
                        return ::pilo::mk_err(PERR_CREATE_TLV_FAIL);
                    } 
                }
                else if (spec->is_single()) {
                    if (tlvp != nullptr) {                        
                        err = tlvp->set_value(arg, len);
                        if (err != PILO_OK) {
                            return err;
                        }
                        ::pilo::set_if_ptr_is_not_null(is_all_set, true);
                        return PILO_OK;
                    }
                    else {
                        tlvp = ::pilo::tlv::allocate(spec->wrapper_type(), spec->key_type(), spec->value_type());
                        if (tlvp == nullptr) {
                            _compose_errmsg(errmsg, "Create TLV (s) for ", spec->name().c_str(), " Failed", nullptr);
                            return ::pilo::mk_err(PERR_CREATE_TLV_FAIL);
                        }
                    }
                }
                else if (spec->is_list()) {
                    if (tlvp != nullptr) {
                        if (spec->can_append_sub_arg(tlvp->size()) != PILO_OK) {                            
                            _compose_errmsg(errmsg, "Arg ", spec->name().c_str(), " has too many sub args", nullptr);
                            return ::pilo::mk_perr(PERR_INVALID_PARAM);
                        }
                        err = tlvp->push_back_value(arg, len);
                        if (err != PILO_OK) {
                            return err;
                        }
                        if (tlvp->size() >= spec->max_count()) {
                            ::pilo::set_if_ptr_is_not_null(is_all_set, true);
                        }
                        return PILO_OK;
                    }
                    tlvp = ::pilo::tlv::allocate(spec->wrapper_type(), spec->key_type(), spec->value_type());
                    if (tlvp == nullptr) {
                        _compose_errmsg(errmsg, "Create TLV (l) for ", spec->name().c_str(), " Failed", nullptr);
                        return ::pilo::mk_err(PERR_CREATE_TLV_FAIL);
                    }
                }
                else if (spec->is_dict()) {
                    if (tlvp != nullptr) {
                        if (spec->can_append_sub_arg(tlvp->size()) != PILO_OK) {
                            _compose_errmsg(errmsg, "Arg ", spec->name().c_str(), " has too many sub args", nullptr);
                            return ::pilo::mk_perr(PERR_INVALID_PARAM);
                        }
                        ::pilo::cstr_ref<char> ret_parts[2];
                        ::pilo::i64_t cnt = ::pilo::core::string::split_fixed(arg, len, ":", 1, ret_parts, 2, false, true, true, true);
                        if (cnt != 2)
                            return ::pilo::mk_perr(PERR_INVALID_PARAM);
                        return tlvp->insert_value(ret_parts[0].ptr, (::pilo::i32_t)ret_parts[0].length, ret_parts[1].ptr, (::pilo::i32_t)ret_parts[1].length, true);
                    }
                    tlvp = ::pilo::tlv::allocate(spec->wrapper_type(), spec->key_type(), spec->value_type());
                    if (tlvp == nullptr) {
                        _compose_errmsg(errmsg, "Create TLV (d) for ", spec->name().c_str(), " Failed", nullptr);
                        return ::pilo::mk_err(PERR_CREATE_TLV_FAIL);
                    }
                }
                else {
                    _compose_errmsg(errmsg, "Invalid Spec while creating cmdline arg", spec->name().c_str(), nullptr, nullptr);
                    return ::pilo::mk_err(PERR_MIS_DATA_TYPE);
                }

                if (spec->short_name() >= 0) {
                    this->_short_args[spec->short_name()] = tlvp;
                }
                if (!spec->long_name().empty()) {
                    this->_long_args.insert(std::pair<std::string, ::pilo::tlv*>(spec->long_name(), tlvp));
                }
                return PILO_OK;
            }

            void cmdline_arg::_compose_errmsg(std::string& errmsg, const char* part1, const char* which_arg, const char* part2, const char* val)
            {
                if (part1 != nullptr)
                    errmsg += part1;

                if (which_arg != nullptr)
                    errmsg += which_arg;
                
                if (part2 != nullptr)
                    errmsg += part2;

                if (val != nullptr)
                    errmsg += val;


            }

            bool cmdline_arg::_is_all_flag(const char* str, int len)
            {
                const ::pilo::core::config::cmdline_spec* spec = nullptr;
                for (int i = 0; i < len; i++) {
                    spec = PILO_CONTEXT->core_config()->cmdline_arg_spec().spec(str[i]);
                    if (spec == nullptr) {
                        return false;
                    }
                    else {
                        if (!spec->is_flag()) {
                            return false;
                        }
                    }
                }               
                return true;
            }


        }
    }
}