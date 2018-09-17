#pragma once

#include "core/coredefs.hpp"

namespace pilo
{
    namespace core
    {
        namespace container
        {
            template <size_t LENGTH>
            class fixed_bitset
            {
            public:
                typedef fixed_bitset<LENGTH>  My_type;
                const static size_t BYTE_LENGTH = (LENGTH + 7) >> 3;
                const static size_t ARRAY_LENGTH = (BYTE_LENGTH + 3) >> 2;

            public:
                fixed_bitset()
                {
                    reset();
                }

                fixed_bitset(const My_type &src)
                {
                    memcpy(m_bitmap, src.m_bitmap, BYTE_LENGTH);
                }

                /**
                * 所有bit清零
                */
                inline My_type& reset()
                {
                    memset(m_bitmap, 0x00, BYTE_LENGTH);
                    return *this;
                }

                /**
                * 设某一位为0
                */
                inline My_type& reset(size_t pos)
                {
                    M_ASSERT(pos < LENGTH);
                    m_bitmap[pos >> 5] &= ~(1 << (pos & 0x1f));
                    return *this;
                }

                /**
                * 所有bit置1
                */
                inline My_type& set()
                {
                    memset(m_bitmap, 0xff, BYTE_LENGTH);
                    return *this;
                }

                /**
                * 设某一位为1
                */
                inline My_type& set(size_t pos)
                {
                    M_ASSERT(pos < LENGTH);
                    m_bitmap[pos >> 5] |= (1 << (pos & 0x1f));
                    return *this;
                }

                inline My_type& flip()
                {
                    for (size_t i = 0; i < BYTE_LENGTH; i++)
                    {
                        m_bitmap[i] = ~m_bitmap[i];
                    }

                    return *this;
                }

                inline My_type& flip(size_t pos)
                {
                    if (test(pos))
                    {
                        reset(pos);
                    }
                    else
                    {
                        set(pos);
                    }

                    return *this;
                }

                /**
                * 检查某位，为1返回true，否则false
                */
                inline bool test(size_t pos) const
                {
                    M_ASSERT(pos < LENGTH);
                    return (m_bitmap[pos >> 5] & (1 << (pos & 0x1f))) != 0;
                }

                inline bool none() const
                {
                    bool bRet = true;
                    for (int i = 0; i < ARRAY_LENGTH; i++)
                    {
                        if (m_bitmap[i] != 0)
                        {
                            bRet = false;
                            break;
                        }
                    }
                    return bRet;
                }

                inline bool all() const
                {
                    bool bRet = true;
                    for (int i = 0; i < ARRAY_LENGTH; i++)
                    {
                        if (m_bitmap[i] == 0)
                        {
                            bRet = false;
                            break;
                        }
                    }
                    return bRet;
                }

                inline bool any() const
                {
                    bool bRet = false;
                    for (int i = 0; i < ARRAY_LENGTH; i++)
                    {
                        if (m_bitmap[i] != 0)
                        {
                            bRet = true;
                            break;
                        }
                    }
                    return bRet;
                }

                inline size_t count() const
                {
                    size_t count = 0;

                    for (size_t i = 0; i < BYTE_LENGTH; i++)
                    {
                        count += _count_1_bit(m_bitmap[i]);
                    }

                    return count;
                }


                /**
                * 同check
                */
                inline bool operator[](const size_t pos) const
                {
                    return test(pos);
                }
               

            protected:
                pilo::u32_t m_bitmap[ARRAY_LENGTH];

            private:
                size_t _count_1_bit(pilo::u8_t n)
                {
                    size_t c = 0;
                    for (c = 0; n; ++c)
                    {
                        n &= (n - 1); // 清除最低位的1
                    }
                    return c;
                }
            };
        }
    }
}