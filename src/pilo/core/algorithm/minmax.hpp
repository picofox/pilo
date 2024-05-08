#pragma once

namespace pilo
{
    namespace core
    {
        namespace algorithm
        {
            template <class T> const T& pilo_min(const T& t1, const T& t2) { return t1 < t2 ? t1 : t2; }
            template <class T> const T& pilo_max(const T& t1, const T& t2) { return t1 < t2 ? t2 : t1; }
        }
    }
}

