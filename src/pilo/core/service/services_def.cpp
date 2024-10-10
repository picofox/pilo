#include "./services_def.hpp"
#include <unordered_map>
#include "../string/string_operation.hpp"


namespace pilo
{
	namespace core
	{
		namespace service
		{
			static const char* s_service_id_name_map[] = {
				PMS_BUILTIN_SERVICE_TIMER,
			};

			static const std::unordered_map<std::string, ::pilo::service_group_id> s_service_name_id_map{
				std::pair<std::string, ::pilo::service_group_id>(std::string(PMS_BUILTIN_SERVICE_TIMER), PMI_BUILTIN_SERVICE_TIMER),
			};

			const char* service_id_to_name(::pilo::service_group_id id)
			{
				return s_service_id_name_map[id];
			}
			::pilo::service_group_id service_name_to_id(const char* name)
			{
				std::string key;
				::pilo::i64_t sz = ::pilo::core::string::character_count(name);
				key.reserve(sz+1);
				for (::pilo::i64_t i = 0; i < sz; i++) {
					key.push_back((char)tolower(name[i]));
				}

				const std::unordered_map<std::string, ::pilo::service_group_id>::const_iterator it = s_service_name_id_map.find(key);
				if (it == s_service_name_id_map.cend()) {
					return -1;
				}
				return it->second;
			}
		}
	}
}