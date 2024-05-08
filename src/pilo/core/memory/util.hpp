#pragma once

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template<typename T>
            void clear_raw_ptr_linear_container(T& cont, bool purge)
            {
                for (typename T::iterator it = cont.begin(); it != cont.end(); ++it)
                {
                    delete* it;
                }
                cont.clear();
                if (purge)
                {
                    cont.shrink_to_fit();
                }
            }

        }
    }
}