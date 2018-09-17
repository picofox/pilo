#pragma once

namespace pilo
{
    namespace topics
    {
        namespace path_finding
        {
            class abstract_path_2d_map
            {
            public:
                virtual bool reachable(pilo::int_r x, pilo::int_r y, void* param) const = 0;
                virtual bool invalid_pos(pilo::int_r x, pilo::int_r y) const = 0;
            };
        }
    }
}