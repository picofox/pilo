#pragma once
#include "core/coredefs.hpp"
#include "./io_device.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/datetime/datetime.hpp"

#if defined(WIN32) || defined(WINDOWS)
#   include <io.h>
#else
#   include <sys/io.h>
#   include <dirent.h>
#   include <sys/stat.h>
#endif

#if defined(WIN32) || defined(WINDOWS)
#   define PILO_CREATE_DIRECTORY(P)(::CreateDirectoryW(P, 0)!= 0)
#else
#   define PILO_CREATE_DIRECTORY(P)(::mkdir(P, S_IRWXU | S_IRWXG | S_IRWXO) == 0)
#endif


namespace pilo
{
    namespace core
    {
        namespace fs
        {
            class fs_find_data;

            class fs_node_visitor_interface
            {
            public:
                virtual ~fs_node_visitor_interface() { }
                virtual ::pilo::i32_t visit(const char* dir, const fs_find_data* data) = 0;
                virtual ::pilo::i32_t pre_dir_visit(const char* path) { M_UNUSED(path); return ::pilo::EC_OK; }
                virtual ::pilo::i32_t post_dir_visit(const char* path) { M_UNUSED(path); return ::pilo::EC_OK; }
            };

            class fs_node_delete_visitor : public fs_node_visitor_interface
            {
            public:
                virtual ::pilo::i32_t visit(const char* path, const fs_find_data* data);
                virtual ::pilo::i32_t post_dir_visit(const char* path);
            };

            class fs_util
            {
            public:
                enum EnumPathSeparator
                {
                    ePS_None = 0,      /**< No Separator Found */
                    ePS_ForwardSlash = 0x2F,   /**< Linux Style Separator Found */
                    ePS_BackwardSlash = 0x5C,   /**< Dos Separator Found */
                    ePS_Both = 0xFF,   /**< Both Separator Found */
                };

                /** Types of file system node */
                enum EnumFSNodeType
                {
                    eFSNT_Error = -2,       /**< Error occurred when getting type */
                    eFSNT_InvalidPath = -1, /**< Incorrect path string format */
                    eFSNT_NonExist = 0,     /**< Node does Not exist */
                    eFSNT_RegularFile,      /**< Normal file */
                    eFSNT_Directory,        /**< Directory */
                    eFSNT_TypeUndefined,    /**< Type that don't know */

                };  
                
                
            public:

                static void trim_path_last_seperator(char* path, size_t len = MC_INVALID_SIZE);
                static ::pilo::error_number_t concatenate_path(char* buf, size_t dstBufferCount, const char* src, size_t srclen);

                static ::pilo::core::fs::fs_util::EnumPathSeparator calculate_path_separator(const char* path);
               

               //! To retrieve current working directory
               /*!
               This method can retrieve current working directory by 2 ways, user buffer or heap\n
               if the user buffer which caller given is zero length or insufficient, heap allocation would be use as 2nd solution.
               unless you are entirely sure the the path string is legal.
               @param buffer [Out] a pointer to a char buffer given by the caller. if null is passed, heap allocation is engaged.
               @param bufferSize [In] size of the user buffer. if 0 or MC_INVALID_SIZE is  passed, heap allocation is engaged.
               @param bUseHeap [Out] if the heap allocation is used. when this is set by the method, \n
               it's is the caller's responsibility to free the memory.
               @param outSize [Out] the length of characters of current working directory path.
               @return return the path of current on success.\n
               return 0 on failed.\n
               @note The caller should better always use and check the bUseHeap unless  you are entirely sure the buffer is sufficient. \n
               This method assume current working directory is never longer than MC_PATH_MAX, if this function failed abnormally, enlarge that macro.\n
               @see PILO::Core::Memory::MM::CHeapAllocator::Free()
               @see MC_PATH_MAX
               */
               static char* get_current_working_directory(char* buffer, size_t bufferSize, bool* bUseHeap, size_t* outSize, bool bAppendSep = false);
                

                //! To judge if a path has valid format.
                /*!
                @param path [In] a constant pointer to a path string to decide it's separator type.
                @return return EC_NULL_PARAMETER on null path string is passed.\n
                return EC_PATH_FORMAT_ERROR on invalid format of path.\n
                return EC_PATH_CONTAIN_BOTH_SEPARATOR on both path separator is occurred.\n
                return EC_OK on a healthy path string.
                */
                static ::pilo::i32_t check_path(const char* path);
                
                //! To judge if a path is absolute or relative.
                /*!
                @param path [In] a constant pointer to a path string to decide it's separator type.
                @return return TRUE on absolute path.\n
                return FASLE on absolute path.\n
                @note This method should be used after CheckPath() returned EC_OK, unless you are entirely sure the the path string is legal. \n
                */
                static bool is_absolute_path(const char* path);
        

                //! To judge the type of a given path.
                /*!
                @param path [In] the path to analyze.
                @return return FSNodeTypeEnumeration type value to represent the type of path.
                @note This method should be used after CheckPath() returned EC_OK, unless you are entirely sure the the path string is legal. \n
                @see FSNodeTypeEnumeration
                */
                static EnumFSNodeType calculate_type(const char* path);

                //! To judge if a path is an existing file.
                /*!
                @param path [In] the path to analyze.
                @return return TRUE on the path's type is EnumTypeRegularFile\n
                return FALSE otherwise.
                @note This method should be used after CheckPath() returned EC_OK, unless you are entirely sure the the path string is legal. \n
                @see FSNodeTypeEnumeration
                */
                static bool file_exist(const char* path);

                //! To judge if a path is a directory
                /*!
                @param path [In] the path of the file to remove.
                @return return TRUE on the path's type is EnumTypeDirectory\n
                return FALSE otherwise.
                @note This method should be used after CheckPath() returned EC_OK, unless you are entirely sure the the path string is legal. \n
                @see FSNodeTypeEnumeration
                */
                static bool dir_exist(const char* path);

                static ::pilo::error_number_t travel_path_preorder(const char* root, fs_node_visitor_interface* fsnvi, bool stop_on_error, bool visit_last_dir);
                static bool is_root(const char* path);
                static ::pilo::error_number_t get_path_depth(size_t& dep, const char* path);
                static ::pilo::error_number_t lock_file(os_file_descriptor_t fildes, bool is_exclusive, size_t start_pos, size_t size_to_lock);
                static ::pilo::error_number_t try_lock_file(os_file_descriptor_t fildes, bool is_exclusive, size_t start_pos, size_t size_to_lock);
                static ::pilo::error_number_t unlock_file(os_file_descriptor_t fildes, size_t start_pos, size_t size_to_lock);


                static ::pilo::error_number_t split_path_to_dir_and_filename(char* dirpath, size_t dirpath_size, char* filename, size_t filename_size, const char* path);
                static ::pilo::error_number_t create_directory_recursively(const char* path, bool force);
                static ::pilo::error_number_t delete_fs_node(const char* path, bool force);
                static ::pilo::error_number_t delete_regular_file(const char* path);
                static ::pilo::error_number_t delete_directory(const char* path, bool content_only);
                static ::pilo::error_number_t delete_empty_directory(const char* path);              

                
                static ::pilo::error_number_t create_regular_file(const char* path, bool always);
                static ::pilo::error_number_t open_file(::pilo::os_file_descriptor_t& fildes, const char* path, ::pilo::core::fs::DeviceAccessModeEnumeration, ::pilo::core::fs::DeviceRWModeEnumeration, ::pilo::u32_t flags);
                static ::pilo::error_number_t close_file(::pilo::os_file_descriptor_t fd, ::pilo::u32_t flags);

                static ::pilo::error_number_t  get_file_modified_time(::pilo::core::datetime::datetime &dt, const char* filepath);

                static ::pilo::error_number_t dir_str_append_last_sep_nocheck(char* buffer, size_t* rlen)
                {
                    if (buffer == nullptr) return ::pilo::EC_NULL_PARAM;
                    size_t len = strlen(buffer);

                    if (buffer[len-1] != M_PATH_SEP_C)
                    {
                        buffer[len] = M_PATH_SEP_C;
                        len++;
                        buffer[len] = 0;
                    }

                    if (rlen != nullptr)
                    {
                        *rlen = len;
                    }                    
                    
                    return ::pilo::EC_OK;
                }

                static ::pilo::error_number_t _compact_path_once(char * pBuffer, size_t len);

                static ::pilo::error_number_t compact_path(char * pBuffer, size_t len);

                static ::pilo::error_number_t validate_and_parse_path_string(char * buffer, size_t buffer_sz, const char* path_str, size_t len)
                {
                    if (buffer == nullptr) return ::pilo::EC_NULL_PARAM;
                    if (path_str == nullptr || *path_str == 0)
                    {
                        return ::pilo::EC_INVALID_PARAM;
                    }

                    if (len == MC_INVALID_SIZE)
                    {
                        len = ::pilo::core::string::string_util::length(path_str);
                    }

                    if (buffer_sz < len + 1)
                    {
                        return ::pilo::EC_BUFFER_TOO_SMALL;
                    }
                    ::memset(buffer, 0x00, buffer_sz);
                    ::pilo::core::string::string_util::copy(buffer, buffer_sz, path_str, len);
                    ::pilo::core::string::string_util::trim(buffer);
                    
                    
                    
#if             defined(WIN32)
                    if (::pilo::EC_OK != ::pilo::core::string::string_util::rescanable_replace(buffer, MC_INVALID_SIZE, "/", "\\", nullptr))
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }

                    if (::pilo::EC_OK != ::pilo::core::string::string_util::rescanable_replace(buffer, MC_INVALID_SIZE, "\\\\", "\\", nullptr))
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }

                    if (buffer[1] == ':' && buffer[2] != '\\')
                    {
                        size_t tmplen = ::strlen(buffer + 2);
                        if (tmplen + 4 >= buffer_sz)
                        {
                            return ::pilo::EC_BUFFER_TOO_SMALL;
                        }
                        
                        ::memmove(buffer + 3, buffer + 2, tmplen);
                        buffer[2] = M_PATH_SEP_C;
                        buffer[3 + tmplen] = 0;
                    }

#               else
                    if (::pilo::EC_OK != ::pilo::core::string::string_util::rescanable_replace(buffer, MC_INVALID_SIZE, "//", "/", nullptr))
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }
#               endif

                    if (::pilo::EC_OK != fs_util::check_path(buffer))
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }

                    trim_path_last_seperator(buffer, MC_INVALID_SIZE);                    

                    return ::pilo::EC_OK;
                }


                template <size_t BUFFSZ>
                static ::pilo::error_number_t validate_and_parse_path_string(char(&buffer)[BUFFSZ], const char* path_str)
                {
                    if (buffer == nullptr) return ::pilo::EC_NULL_PARAM;

                    if (path_str == nullptr || *path_str == 0)
                    {
                        return ::pilo::EC_INVALID_PARAM;
                    }

                    size_t len = ::pilo::core::string::string_util::length(path_str);
                    if (BUFFSZ < len + 1)
                    {
                        return ::pilo::EC_BUFFER_TOO_SMALL;
                    }

                    ::memset(buffer, 0x00, BUFFSZ);

                    ::pilo::core::string::string_util::copy(buffer, BUFFSZ, path_str, len);

                    ::pilo::core::string::string_util::trim(buffer);                    

#if             defined(WIN32)
                    if (::pilo::EC_OK != ::pilo::core::string::string_util::rescanable_replace(buffer, MC_INVALID_SIZE, "/", "\\", nullptr))
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }

                    if (::pilo::EC_OK != ::pilo::core::string::string_util::rescanable_replace(buffer, MC_INVALID_SIZE, "\\\\", "\\", nullptr))
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }          

                    if (buffer[1] == ':' && buffer[2] != '\\')
                    {
                        size_t tmplen = ::strlen(buffer + 2);
                        if (tmplen + 4 >= BUFFSZ)
                        {
                            return ::pilo::EC_BUFFER_TOO_SMALL;
                        }

                        ::memmove(buffer + 3, buffer + 2, tmplen);
                        buffer[2] = M_PATH_SEP_C;
                        buffer[3 + tmplen] = 0;
                    }

#               else 
                    if (::pilo::EC_OK != ::pilo::core::string::string_util::rescanable_replace(buffer, MC_INVALID_SIZE, "//", "/", nullptr))
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }

#               endif



                    if (::pilo::EC_OK != fs_util::check_path(buffer))
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }
                    trim_path_last_seperator(buffer, MC_INVALID_SIZE);

                    return ::pilo::EC_OK;
                }
                
#if defined(WIN32)               

                static const wchar_t* convert_str_to_wstr(wchar_t* wBuffer, size_t wBufferLen, const char* src_str, size_t src_len = MC_INVALID_SIZE, ::pilo::u32_t m_encode = CP_ACP)
                {
                    wchar_t* wDynBuffer = nullptr;
                    wchar_t* pBuffer = nullptr;

                    if (src_len == MC_INVALID_SIZE)
                    {
                        src_len = strlen(src_str);
                    }

                    int len = MultiByteToWideChar(m_encode, 0, src_str, (int) src_len, NULL, 0);
                    if (wBuffer == nullptr || wBufferLen <= 0 || len < wBufferLen)
                    {
                        pBuffer = wBuffer;
                    }
                    else
                    {
                        wDynBuffer = new wchar_t[len + 1];
                        pBuffer = wDynBuffer;
                    }
                    
                    MultiByteToWideChar(m_encode, 0, src_str, (int) strlen(src_str), pBuffer, len);
                    pBuffer[len] = '\0';

                    return pBuffer;
                }
// 
//                 std::string WcharToChar(const wchar_t* wp, size_t m_encode = CP_ACP)
//                 {
//                     std::string str;
//                     int len = WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);
//                     char	*m_char = new char[len + 1];
//                     WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), m_char, len, NULL, NULL);
//                     m_char[len] = '\0';
//                     str = m_char;
//                     delete m_char;
//                     return str;
//                 }
#else

#endif

                static ::pilo::error_number_t create_directory(const char* path, ::pilo::u32_t mode)
                {
                    M_UNUSED(mode);
#if defined(WIN32)    
                    wchar_t wszPathBuffer[MC_PATH_MAX] = { 0 };
                    const wchar_t* pRetPth = convert_str_to_wstr(wszPathBuffer, MC_PATH_MAX, path, MC_INVALID_SIZE, CP_ACP);
                    bool bOk = PILO_CREATE_DIRECTORY(wszPathBuffer);
                    if (pRetPth != nullptr && pRetPth != wszPathBuffer)
                    {
                        delete pRetPth;
                    }                    
#else
                    bool bOk = PILO_CREATE_DIRECTORY(path);
#endif
                    if (!bOk)
                    {
                        return ::pilo::EC_CREATE_DIR_ERROR;
                    }
                    return ::pilo::EC_OK;
                }
               
            };

            class fs_find_data
            {
            public:
                fs_find_data()
                {
                    reset();
                }

            protected:
                ::pilo::core::fs::fs_util::EnumFSNodeType _m_type;
                ::pilo::core::string::fixed_astring<MC_PATH_MAX> _m_full_pathname;
                const char*                                      _m_filename_ptr;

            public:
                void reset() 
                {
                    _m_type = ::pilo::core::fs::fs_util::eFSNT_NonExist;
                    _m_full_pathname.clear();
                    _m_filename_ptr = nullptr;
                }
                inline ::pilo::core::fs::fs_util::EnumFSNodeType type() const { return _m_type; }
                inline const char* filename() const { return _m_filename_ptr; }
                inline const char* full_pathname() const { return _m_full_pathname.c_str(); }
                inline size_t dir_length() const
                {
                    size_t dir_len = _m_filename_ptr - _m_full_pathname.c_str();
                    if (dir_len <= 0)
                        return MC_INVALID_SIZE;

                    return --dir_len;
                }
                inline size_t get_dir_path(char* buffer, size_t sz) const
                {
                    if (buffer == nullptr) return MC_INVALID_SIZE;

                    size_t dir_len = _m_filename_ptr - _m_full_pathname.c_str();
                    if (dir_len > 0)
                        dir_len--;

                    if (dir_len > 0)
                    {
                        ::pilo::core::string::string_util::copy(buffer, 0, sz,
                            _m_full_pathname.c_str(), 0, dir_len);
                    }
                    else
                    {
                        *buffer = 0;
                    }

                    return dir_len;
                }

                inline void set_type(::pilo::core::fs::fs_util::EnumFSNodeType t) { _m_type = t; }
                inline void set_full_pathname(const char* dir, const char* filename)
                {
#ifdef WINDOWS
                    MP_CHECK_EMPTY_CSTR_RET_VOID(dir);
#else
                    MP_CHECK_NULL_PTR_RET_VOID(dir);
#endif // WINDOWS

                    
                    MP_CHECK_EMPTY_CSTR_RET_VOID(filename);


                    size_t len = ::pilo::core::string::string_util::length(dir);
                    if (len == MC_INVALID_SIZE) return;

                    if (len > 0)
                    {
                        _m_full_pathname = dir;
                    }
                    
                    _m_full_pathname.concatenate(M_PATH_SEP_S);
                    _m_full_pathname.concatenate(filename);

                    size_t index = len + 1;
                    _m_filename_ptr = _m_full_pathname.c_str() + index;                    
                }

            };
        }
    }
}