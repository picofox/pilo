#include "./cmdline_args.hpp"
#include "./context.hpp"

namespace pilo
{
    namespace core
    {
        namespace process
        {
            cmdline_arg::~cmdline_arg()
            {
            }

            ::pilo::err_t cmdline_arg::parse(int argc, char* argv[])
            {
                ::pilo::i32_t arg_len = 0;
                int expection = -1; /*-1=flag or target   0=flag, 1-arg 2-target*/
                if (PILO_CONTEXT->core_config()->cmdline_arg_spec().has_arg()
                    && PILO_CONTEXT->core_config()->cmdline_arg_spec().has_target()) {                    
                    expection = -1;
                }
                else if (PILO_CONTEXT->core_config()->cmdline_arg_spec().has_arg()) {
                    expection = 0;
                }                
                else if (PILO_CONTEXT->core_config()->cmdline_arg_spec().has_target()) {
                    expection = 2;
                }
                else {
                    return PILO_OK;
                }
                
                

                for (int i = 1; i < argc; i++) {
                    arg_len = (::pilo::i32_t) ::pilo::core::string::character_count(argv[i]);
                    if (arg_len < 0)
                        return ::pilo::mk_perr(PERR_INVALID_PARAM);

                    if (expection == -1) {
                        if (arg_len > 0 && argv[i][0] == '-') {
                            
                        }
                        else { //target
                            PILO_ERRRET(_make_target(argv[i], arg_len));
                        }

                    } else if (expection == 0) {
                        if (arg_len == 0 || argv[i][0] != '-') {
                            return ::pilo::mk_perr(PERR_INVALID_PARAM);
                        }

                    } else if (expection == 1) {

                          
                    } else if (expection == 2) {
                        if (PILO_CONTEXT->core_config()->cmdline_arg_spec().target_spec().max_count() >= 0 
                            && this->_targets.size() >= PILO_CONTEXT->core_config()->cmdline_arg_spec().target_spec().max_count()) {
                            return ::pilo::mk_perr(PERR_LEN_TOO_LARGE);
                        }

                        PILO_ERRRET(_make_target(argv[i], arg_len));
                         

                        
                    }

         


                }

                return PILO_OK;
            }

            ::pilo::err_t  cmdline_arg::_make_target(const char* argv, ::pilo::i32_t len)
            {
                PMC_UNUSED(argv);
                PMC_UNUSED(len);

                const ::pilo::core::config::cmdline_spec& sp = PILO_CONTEXT->core_config()->cmdline_arg_spec().target_spec();
                ::pilo::tlv* tlvp = ::pilo::tlv::allocate();

                tlvp->set_single_type(sp.value_type());


                _targets.push_back(tlvp);
                return PILO_OK;
            }


        }
    }
}