#pragma once
#include "../../pilo.hpp"


namespace pilo
{
    namespace core
    {
        namespace string
        {
            template<typename CHAR_T>
            class constants
            { 
            };

            template<>
            class constants<char>
            {
            public:
                inline static const char* alpha_num() { return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"; }
                inline static ::pilo::i32_t alpha_num_length() { return 62; }
                inline static const char* parent_dir() { return ".."; }
                inline static ::pilo::i32_t parent_dir_length() { return 2; }
                inline static const char* dot3() { return "..."; }
                inline static ::pilo::i32_t dot3_length() { return 3; }
#ifdef WINDOWS
                inline static const char* root_dir_sep() { return "\\\\?\\"; }
                inline static ::pilo::i32_t root_dir_sep_length() { return 4; }
                inline static const char* root_dir() { return "\\\\?"; }
                inline static ::pilo::i32_t root_dir_length() { return 3; }
                inline static const char* illegal_path_chars() { return "|<>\"\'`:"; }
                inline static ::pilo::i32_t illegal_path_chars_length() { return 7; }
                inline static const char* smb_root() { return "\\\\"; }
                inline static ::pilo::i32_t smb_root_length() { return 2; }
                inline static const char* cruurent_dir() { return "\\.\\"; }
                inline static ::pilo::i32_t cruurent_dir_length() { return 3; }
                inline static const char* path_sep() { return "\\"; }
                inline static ::pilo::i32_t path_sep_length() { return 1; }
                inline static const char* path_sep2() { return "\\\\"; }
                inline static ::pilo::i32_t path_sep2_length() { return 2; }
#else
                inline static const char* root_dir_sep() { return "/"; }
                inline static ::pilo::i32_t root_dir_sep_length() { return 1; }
                inline static const char* root_dir() { return ""; }
                inline static ::pilo::i32_t root_dir_length() { return 0; }
                inline static const char* illegal_path_chars() { return "|<>\"\'`"; }
                inline static ::pilo::i32_t illegal_path_chars_length() { return 6; }
                inline static const char* smb_root() { return "//"; }
                inline static ::pilo::i32_t smb_root_length() { return 2; }
                inline static const char* cruurent_dir() { return "/./"; }
                inline static ::pilo::i32_t cruurent_dir_length() { return 3; }
                inline static const char* path_sep() { return "/"; }
                inline static ::pilo::i32_t path_sep_length() { return 1; }
                inline static const char* path_sep2() { return "//"; }
                inline static ::pilo::i32_t path_sep2_length() { return 2; }
#endif



                inline static const char* empty() { return ""; }
                inline static const char* bincstr(::pilo::u8_t b)
                {
                    static const char* s_bincstr_arr[256] = { "00000000","00000001", "00000010", "00000011", "00000100", "00000101", "00000110", "00000111", "00001000", "00001001", "00001010", "00001011", "00001100", "00001101", "00001110", "00001111", "00010000", "00010001", "00010010", "00010011", "00010100", "00010101", "00010110", "00010111", "00011000", "00011001", "00011010", "00011011", "00011100", "00011101", "00011110", "00011111", "00100000", "00100001", "00100010", "00100011", "00100100", "00100101", "00100110", "00100111", "00101000", "00101001", "00101010", "00101011", "00101100", "00101101", "00101110", "00101111", "00110000", "00110001", "00110010", "00110011", "00110100", "00110101", "00110110", "00110111", "00111000", "00111001", "00111010", "00111011", "00111100", "00111101", "00111110", "00111111", "01000000", "01000001", "01000010", "01000011", "01000100", "01000101", "01000110", "01000111", "01001000", "01001001", "01001010", "01001011", "01001100", "01001101", "01001110", "01001111", "01010000", "01010001", "01010010", "01010011", "01010100", "01010101", "01010110", "01010111", "01011000", "01011001", "01011010", "01011011", "01011100", "01011101", "01011110", "01011111", "01100000", "01100001", "01100010", "01100011", "01100100", "01100101", "01100110", "01100111", "01101000", "01101001", "01101010", "01101011", "01101100", "01101101", "01101110", "01101111", "01110000", "01110001", "01110010", "01110011", "01110100", "01110101", "01110110", "01110111", "01111000", "01111001", "01111010", "01111011", "01111100", "01111101", "01111110", "01111111", "10000000", "10000001", "10000010", "10000011", "10000100", "10000101", "10000110", "10000111", "10001000", "10001001", "10001010", "10001011", "10001100", "10001101", "10001110", "10001111", "10010000", "10010001", "10010010", "10010011", "10010100", "10010101", "10010110", "10010111", "10011000", "10011001", "10011010", "10011011", "10011100", "10011101", "10011110", "10011111", "10100000", "10100001", "10100010", "10100011", "10100100", "10100101", "10100110", "10100111", "10101000", "10101001", "10101010", "10101011", "10101100", "10101101", "10101110", "10101111", "10110000", "10110001", "10110010", "10110011", "10110100", "10110101", "10110110", "10110111", "10111000", "10111001", "10111010", "10111011", "10111100", "10111101", "10111110", "10111111", "11000000", "11000001", "11000010", "11000011", "11000100", "11000101", "11000110", "11000111", "11001000", "11001001", "11001010", "11001011", "11001100", "11001101", "11001110", "11001111", "11010000", "11010001", "11010010", "11010011", "11010100", "11010101", "11010110", "11010111", "11011000", "11011001", "11011010", "11011011", "11011100", "11011101", "11011110", "11011111", "11100000", "11100001", "11100010", "11100011", "11100100", "11100101", "11100110", "11100111", "11101000", "11101001", "11101010", "11101011", "11101100", "11101101", "11101110", "11101111", "11110000", "11110001", "11110010", "11110011", "11110100", "11110101", "11110110", "11110111", "11111000", "11111001", "11111010", "11111011", "11111100", "11111101", "11111110", "11111111" };
                    return s_bincstr_arr[b];
                }
            };


            template<>
            class constants<wchar_t>
            {
            public:
                inline static const wchar_t* alpha_num() { return L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"; }
                inline static ::pilo::i32_t alpha_num_length() { return 62; }
                inline static const wchar_t* parent_dir() { return L".."; }
                inline static ::pilo::i32_t parent_dir_length() { return 2; }
                inline static const wchar_t* dot3() { return L"..."; }
                inline static ::pilo::i32_t dot3_length() { return 3; }

#ifdef WINDOWS
                inline static const wchar_t* root_dir_sep() { return L"\\\\?\\"; }
                inline static ::pilo::i32_t root_dir_sep_length() { return 4; }
                inline static const wchar_t* root_dir() { return L"\\\\?"; }
                inline static ::pilo::i32_t root_dir_length() { return 3; }
                inline static const wchar_t* illegal_path_chars() { return L"|<>\"\'`:"; }
                inline static ::pilo::i32_t illegal_path_chars_length() { return 7; }
                inline static const wchar_t* smb_root() { return L"\\\\"; }
                inline static ::pilo::i32_t smb_root_length() { return 2; }
                inline static const wchar_t* cruurent_dir() { return L"\\.\\"; }
                inline static ::pilo::i32_t cruurent_dir_length() { return 3; }
                inline static const wchar_t* path_sep() { return L"\\"; }
                inline static ::pilo::i32_t path_sep_length() { return 1; }
                inline static const wchar_t* path_sep2() { return L"\\\\"; }
                inline static ::pilo::i32_t path_sep2_length() { return 2; }
#else
                inline static const wchar_t* root_dir_sep() { return L"/"; }
                inline static ::pilo::i32_t root_dir_sep_length() { return 1; }
                inline static const wchar_t* root_dir() { return L"";}
                inline static ::pilo::i32_t root_dir_length() { return 0; }
                inline static const wchar_t* illegal_path_chars() { return L"|<>\"\'`"; }
                inline static ::pilo::i32_t illegal_path_chars_length() { return 6; }
                inline static const wchar_t* smb_root() { return L"//"; }
                inline static ::pilo::i32_t smb_root_length() { return 2; }
                inline static const wchar_t* cruurent_dir() { return L"/./"; }
                inline static ::pilo::i32_t cruurent_dir_length() { return 3; }
                inline static const wchar_t* path_sep() { return L"/"; }
                inline static ::pilo::i32_t path_sep_length() { return 1; }
                inline static const wchar_t* path_sep2() { return L"//"; }
                inline static ::pilo::i32_t path_sep2_length() { return 2; }
#endif

                inline static const wchar_t* empty() { return L""; }
                inline static const wchar_t* bincstr(::pilo::u8_t b)
                {
                    static const wchar_t *sw_bincstr_arr[256] = {L"00000000",L"00000001", L"00000010", L"00000011", L"00000100", L"00000101", L"00000110", L"00000111", L"00001000", L"00001001", L"00001010", L"00001011", L"00001100", L"00001101", L"00001110", L"00001111", L"00010000", L"00010001", L"00010010", L"00010011", L"00010100", L"00010101", L"00010110", L"00010111", L"00011000", L"00011001", L"00011010", L"00011011", L"00011100", L"00011101", L"00011110", L"00011111", L"00100000", L"00100001", L"00100010", L"00100011", L"00100100", L"00100101", L"00100110", L"00100111", L"00101000", L"00101001", L"00101010", L"00101011", L"00101100", L"00101101", L"00101110", L"00101111", L"00110000", L"00110001", L"00110010", L"00110011", L"00110100", L"00110101", L"00110110", L"00110111", L"00111000", L"00111001", L"00111010", L"00111011", L"00111100", L"00111101", L"00111110", L"00111111", L"01000000", L"01000001", L"01000010", L"01000011", L"01000100", L"01000101", L"01000110", L"01000111", L"01001000", L"01001001", L"01001010", L"01001011", L"01001100", L"01001101", L"01001110", L"01001111", L"01010000", L"01010001", L"01010010", L"01010011", L"01010100", L"01010101", L"01010110", L"01010111", L"01011000", L"01011001", L"01011010", L"01011011", L"01011100", L"01011101", L"01011110", L"01011111", L"01100000", L"01100001", L"01100010", L"01100011", L"01100100", L"01100101", L"01100110", L"01100111", L"01101000", L"01101001", L"01101010", L"01101011", L"01101100", L"01101101", L"01101110", L"01101111", L"01110000", L"01110001", L"01110010", L"01110011", L"01110100", L"01110101", L"01110110", L"01110111", L"01111000", L"01111001", L"01111010", L"01111011", L"01111100", L"01111101", L"01111110", L"01111111", L"10000000", L"10000001", L"10000010", L"10000011", L"10000100", L"10000101", L"10000110", L"10000111", L"10001000", L"10001001", L"10001010", L"10001011", L"10001100", L"10001101", L"10001110", L"10001111", L"10010000", L"10010001", L"10010010", L"10010011", L"10010100", L"10010101", L"10010110", L"10010111", L"10011000", L"10011001", L"10011010", L"10011011", L"10011100", L"10011101", L"10011110", L"10011111", L"10100000", L"10100001", L"10100010", L"10100011", L"10100100", L"10100101", L"10100110", L"10100111", L"10101000", L"10101001", L"10101010", L"10101011", L"10101100", L"10101101", L"10101110", L"10101111", L"10110000", L"10110001", L"10110010", L"10110011", L"10110100", L"10110101", L"10110110", L"10110111", L"10111000", L"10111001", L"10111010", L"10111011", L"10111100", L"10111101", L"10111110", L"10111111", L"11000000", L"11000001", L"11000010", L"11000011", L"11000100", L"11000101", L"11000110", L"11000111", L"11001000", L"11001001", L"11001010", L"11001011", L"11001100", L"11001101", L"11001110", L"11001111", L"11010000", L"11010001", L"11010010", L"11010011", L"11010100", L"11010101", L"11010110", L"11010111", L"11011000", L"11011001", L"11011010", L"11011011", L"11011100", L"11011101", L"11011110", L"11011111", L"11100000", L"11100001", L"11100010", L"11100011", L"11100100", L"11100101", L"11100110", L"11100111", L"11101000", L"11101001", L"11101010", L"11101011", L"11101100", L"11101101", L"11101110", L"11101111", L"11110000", L"11110001", L"11110010", L"11110011", L"11110100", L"11110101", L"11110110", L"11110111", L"11111000", L"11111001", L"11111010", L"11111011", L"11111100", L"11111101", L"11111110", L"11111111"};
                    return sw_bincstr_arr[b];
                }
            };





        } //end of string
    }
}