#include "core/string/string_util.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/io/format_output.hpp"
#include "functional_test_module_class_fixed_memory_stream.hpp"
#include "core/io/fixed_memory_istream.hpp"
#include "core/io/fixed_memory_ostream.hpp"
#include "core/io/memory_ostream_adapter.hpp"
#include "core/io/memory_istream_adapter.hpp"
#include <set>
#include <algorithm>

using namespace pilo;

struct test_stream_pod
{
    void reset()
    {
        memset(this, 0x00, sizeof(*this));
    }
    void fill()
    {
        reset();
        m_i32   = -12345678;
        m_i8    = -126;
        m_float = -456.1234f;
        m_i64   = -12345678901234;
        m_i16   = -30001;
        m_u32   = 3000000001;
        m_double= 0.f;
        m_u8    = 253;
        m_u64   = 0xFFFFFFFFFFFFFFFE; //m_u64 = 18446744073709551614
        pilo::core::string::string_util::copy(m_str8, sizeof(m_str8), "a12345678901234567890z", MC_INVALID_SIZE);
        m_u16   = 65531;
        pilo::core::string::string_util::copy(m_str16, sizeof(m_str16), "x12345678901234567890y", MC_INVALID_SIZE);
        pilo::core::string::string_util::copy(m_str32, sizeof(m_str32), "m12345678901234567890n", MC_INVALID_SIZE);
        
    } 




    pilo::i32_t         m_i32;
    pilo::i8_t          m_i8;
    float               m_float;
    pilo::i64_t         m_i64;
    pilo::i16_t         m_i16;
    pilo::u32_t         m_u32;
    double              m_double;
    pilo::u8_t          m_u8;
    pilo::u64_t         m_u64;
    char                m_str8[64];
    pilo::u16_t         m_u16;
    char                m_str16[64];
    char                m_str32[64];

    size_t serialize(pilo::core::io::ostream_interface* stream)
    {
        stream->write_i32(m_i32);
        stream->write_i8(m_i8);
        stream->write_float(m_float);
        stream->write_i64(m_i64);
        stream->write_i16(m_i16);
        stream->write_u32(m_u32);
        stream->write_double(m_double);
        stream->write_u8(m_u8);
        stream->write_u64(m_u64);
        stream->write_string8(m_str8, MC_INVALID_SIZE, false);
        stream->write_u16(m_u16);
        stream->write_string16(m_str16, MC_INVALID_SIZE, false);
        stream->write_string32(m_str32, MC_INVALID_SIZE, false);

        return stream->tell();
    }


    size_t unserialize(pilo::core::io::istream_interface* stream)
    {
        stream->read_i32(m_i32);
        stream->read_i8(m_i8);
        stream->read_float(m_float);
        stream->read_i64(m_i64);
        stream->read_i16(m_i16);
        stream->read_u32(m_u32);
        stream->read_double(m_double);
        stream->read_u8(m_u8);
        stream->read_u64(m_u64);
        stream->read_string8(m_str8,64);
        stream->read_u16(m_u16);
        stream->read_string16(m_str16,64);
        stream->read_string32(m_str32,64);

        return stream->tell();
    }

};


namespace pilo
{
    namespace test
    {
        static pilo::i32_t functional_fixed_memory_istream_basic(void* param);
        static pilo::i32_t functional_fixed_memory_istream_redir_operator(void* param);        

        pilo::i32_t functional_fixed_memory_istream_redir_operator(void* param)
        {
            M_UNUSED(param);

            pilo::core::string::fixed_astring<333>  astr = "[nihao, fox ]";

            pilo::core::io::fixed_memory_ostream<sizeof(test_stream_pod)> os;
            os << 1 << "a01234567890b"<<1.345f ;

            os << astr;

            pilo::core::io::memory_istream_adapter is_adp(os.data(),os.size());
            pilo::i32_t i;
            float dbl;
            std::string str;
            pilo::core::string::fixed_astring<333>  astr2 = "afdafafasdfasfas";
            is_adp >> i >> str >> dbl ;
            is_adp >> astr2;

            if (i != 1)
            {
                return -1;
            }

            if (dbl != 1.345f)
            {
                return -2;
            }

            if (str != "a01234567890b")
            {
                return -3;
            }

            if (astr2 != astr)
            {
                return -4;
            }


            return 0;
        }

        pilo::test::testing_case g_functional_cases_fixed_memory_stream[] =
        {
            /*---"----------------------------------------------"*/
            { 1, "Basic Functions of istream                    ", nullptr, functional_fixed_memory_istream_basic, 0, -1, (pilo::u32_t) - 1 },
            { 2, "redir operator                                ", nullptr, functional_fixed_memory_istream_redir_operator, 0, -1, (pilo::u32_t) - 1 },


            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };

        pilo::i32_t functional_fixed_memory_istream_basic(void* param)
        {
            M_UNUSED(param);

            test_stream_pod t;
            t.fill();

            pilo::core::io::fixed_memory_ostream<sizeof(test_stream_pod)> os;
            
            size_t ret = t.serialize(&os);
            if (ret == MC_INVALID_SIZE)
            {
                return -1;
            }

            pilo::core::io::memory_istream_adapter isd;
            isd.attach(os.data(), os.size());

            test_stream_pod t2;
            t2.reset();
            t2.unserialize(&isd);

            if (os.size() != isd.size())
            {
                return -3;
            }

            if (memcmp(os.data(), isd.data(), os.size()))
            {
                return -2;
            }

            if (memcmp(&t, &t2, os.size()))
            {
                return -2;
            }

            return 0;
        }

        

    }
}