#include "./services_def.hpp"
#include <unordered_map>
#include <algorithm>

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
				std::transform(key.begin(), key.end(), name,
					[](unsigned char c) { return std::tolower(c); });

				std::string key(name);
				const std::unordered_map<std::string, ::pilo::service_group_id>::const_iterator it = s_service_name_id_map.find(key);
				if (it == s_service_name_id_map.cend()) {
					return -1;
				}
				return it->second;
			}
		}
	}
}