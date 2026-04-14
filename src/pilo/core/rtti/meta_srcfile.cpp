#include    "./meta_srcfile.hpp"
#include    <sstream>
#include    "../string/string_operation.hpp"


namespace pilo
{
    namespace core
    {
        namespace rtti
        {
            meta_srcfile::~meta_srcfile()
            {
            }

            ::pilo::err_t meta_srcfile::append_to_stringstream_cpp(std::stringstream& ss, ::pilo::u32_t flags, const std::string& strparam, const char* indent_cstr) const
            {
                PMC_UNUSED(ss);
                PMC_UNUSED(indent_cstr);
                PMC_UNUSED(flags);
                PMC_UNUSED(strparam);

                return PERR_OK;
            }

            /*meta_srcfile::~meta_srcfile()
            {
                for (std::size_t i = 0; i < _nodes.size(); i++) {
                    if (_nodes.at(i) != nullptr) {
                        delete _nodes.at(i);
                    }
                }
                _nodes.clear();
                _nodes.shrink_to_fit();
            }

            meta_ns* meta_srcfile::add_ns(const std::string& nsstr, std::string& errmsg)
            {
                errmsg.clear();
                ::pilo::cstr_ref<char>    parts[SP_PMI_CODE_GEN_MAX_NS+1];
                std::string one_part_name;
                std::string tmp_ns;
                ::pilo::i64_t pcnt = ::pilo::core::string::split_fixed(nsstr.c_str(), nsstr.size(), ".", 1, parts, SP_PMI_CODE_GEN_MAX_NS+1, false, true, true, true);
                if (pcnt < 1) {
                    ::pilo::compose_errmsg(errmsg, "Invalid namespace string expressing: (%s)", nsstr.c_str());
                    return nullptr;
                }

                if (pcnt > SP_PMI_CODE_GEN_MAX_NS) {
                    ::pilo::compose_errmsg(errmsg, "Too many namespace section (%s), max %d", nsstr.c_str(), SP_PMI_CODE_GEN_MAX_NS);
                    return nullptr;
                }

                meta_src_node* cur = this;
                for (auto i = 0; i < pcnt; i++) {
                    one_part_name.assign(parts[i].ptr, parts[i].length);
                    meta_src_node* nodes = cur->get_child_node(one_part_name, errmsg);
                    if (nodes != nullptr) {
                        if (nodes->meta_type() != meta_node_type_enum::ns) {
                            ::pilo::compose_errmsg(errmsg, "Node (%s) type Not NS: (%d)", one_part_name.c_str(), nodes->meta_type());
                            return nullptr;
                        }
                        cur = nodes;
                    }
                    else {
                        tmp_ns = ::pilo::core::string::merge_cstr_ref(parts, i + 1, ".", 1);
                        meta_ns* ns = new meta_ns((short)(i+1), tmp_ns);
                        _nodes.push_back(ns);
                        cur = ns;
                    }                    
                }

                
                return nullptr;
            }


            std::string meta_srcfile::name() const
            {
                return _filename;
            }

            meta_src_node* meta_srcfile::get_child_node(std::string& name, std::string& )
            {
                for (auto i = 0; i < _nodes.size(); i++) {
                    if (_nodes.at(i)->name() == name) {
                        return _nodes.at(i);
                    }
                }
                return nullptr;
            }

            void meta_srcfile::append_to_stringstream(std::stringstream& ss, short depth) const
            {
                ss << "file:" << _filename << std::endl;
                ::pilo::core::string::append_char_to_stringstream(ss, ' ', depth * 4);

                for (auto i = 0; i < _nodes.size(); i++) {
                    _nodes.at(i)->append_to_stringstream(ss, depth + 1);
                }

                ss << std::endl;
            }*/
        }
    }
}



