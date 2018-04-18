#pragma once

#include "core/coredefs.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/memory/object_pool.hpp"
#include "core/threading/recursive_mutex.hpp"
#include "core/datetime/datetime.hpp"
#include <vector>

namespace pilo
{
    namespace test
    {
        class GameSceneObject
        {
        public:
            GameSceneObject()
            {
                m_id = (::pilo::u64_t) ~0;
                m_creation_ts = PiloGetTickCount64();
            }

            ~GameSceneObject()
            {

            }

            ::pilo::u64_t ID() const { return m_id; }
            void SetID(::pilo::i64_t id) { m_id = id; }

            virtual void OnCreated() {}
        protected:
            ::pilo::i64_t                               m_id;
            ::pilo::tick64_t                            m_creation_ts;
        };

        class TestNpc0 :
            public GameSceneObject,
            public ::pilo::core::memory::portable_object_pool<TestNpc0, 1024, ::pilo::core::threading::recursive_mutex>
        {
        public:
            TestNpc0()
            {
                m_name = "hongge666";
                for (int i = 0; i < 77; i++)
                {
                    m_attr_key_list.push_back(i);
                    m_attr_value_list.push_back(100 - i);
                }
            }

            bool Valid() const
            {
                if (m_name.size() != 9) return false;
                if (m_attr_value_list.size() != 77) return false;
                if (m_attr_key_list.size() != 77) return false;

                if (m_name != "hongge666")
                {
                    return false;
                }

                for (int i = 0; i < 77; i++)
                {
                    if (m_attr_key_list[i] + m_attr_value_list[i] != 100)
                    {
                        return false;
                    }
                }
                return true;
            }

            void Clear()
            {
                m_name.clear();
                m_attr_value_list.clear();
                m_attr_key_list.clear();
            }

            const ::pilo::core::string::fixed_astring<64>& Name() const { return m_name; }
            void SetName(const char* name) { m_name = name; }

            std::vector<int>                            m_attr_key_list;
            std::vector<int>                            m_attr_value_list;

        private:
            
            ::pilo::core::string::fixed_astring<64>     m_name;
            
        };
    }
}