#include "./fs_util.hpp"
#include "core/string/string_util.hpp"
#include "core/io/format_output.hpp"
#include "core/fs/path_string.hpp"
#include <cctype>
#include <time.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <stdio.h>  
#include <errno.h> 

namespace pilo
{
    namespace core
    {
        namespace fs
        {

            ::pilo::error_number_t fs_util::travel_path_preorder(const char* str, fs_node_visitor_interface* fsnvi, ::pilo::u32_t flags, bool stop_on_error)
            {
                if (str == nullptr)
                {
                    return ::pilo::EC_NULL_PARAM;
                }

                ::pilo::core::fs::path_string<MC_PATH_MAX>pathstr(str);

                return travel_path_preorder(pathstr, fsnvi, flags, stop_on_error);
            }

            void fs_util::trim_path_last_seperator(char* path, size_t len /*= MC_INVALID_SIZE*/)
            {
                MP_CHECK_EMPTY_CSTR_RET_VOID(path);
                
                if (len == MC_INVALID_SIZE)
                {
                    len = ::pilo::core::string::string_util::length(path);
                }

                if (len > 0)
                {
#ifdef          WINDOWS
                    if (path[len - 1] == '/' || path[len - 1] == '\\')
                    {
                        path[len - 1] = 0;
                    }
#else
                    if (path[len - 1] == '/')
                    {
                        path[len - 1] = 0;
                    }
#endif
                }
            }

            ::pilo::error_number_t fs_util::concatenate_path(char* path1, size_t path1_buffer_size, const char* path2, size_t path2_len)
            {
                if (path1 == nullptr || path2 == nullptr)
                {
                    return ::pilo::EC_NULL_PARAM;
                }

                if (path2_len == MC_INVALID_SIZE)
                {
                    path2_len = ::pilo::core::string::string_util::length(path2);
                }

                if (*path1 == 0)
                {
                    ::pilo::core::string::string_util::copy(path1, path1_buffer_size, path2, path2_len);
                    return ::pilo::EC_OK;
                }

                ::pilo::core::fs::fs_util::EnumPathSeparator e1 = ::pilo::core::fs::fs_util::calculate_path_separator(path1);
                ::pilo::core::fs::fs_util::EnumPathSeparator e2 = ::pilo::core::fs::fs_util::calculate_path_separator(path2);
                size_t len = ::pilo::core::string::string_util::length(path1);

#ifdef WINDOWS
                M_ASSERT(e1 == ::pilo::core::fs::fs_util::ePS_BackwardSlash || e1 == ::pilo::core::fs::fs_util::ePS_ForwardSlash || e1 == ::pilo::core::fs::fs_util::ePS_None);
                M_ASSERT(e2 == ::pilo::core::fs::fs_util::ePS_BackwardSlash || e2 == ::pilo::core::fs::fs_util::ePS_ForwardSlash || e2 == ::pilo::core::fs::fs_util::ePS_None);
                
                if (path1[len-1] != '\\' && path1[len-1] != '/')
                {
                    ::pilo::core::string::string_util::concatenate_string(path1, path1_buffer_size, "\\", 1);
                }
              
                ::pilo::core::string::string_util::concatenate_string(path1, path1_buffer_size, path2, path2_len);
                ::pilo::core::string::string_util::rescanable_replace<char>(path1, MC_INVALID_SIZE, "/", "\\", 0);

#else
                M_ASSERT(e1 == ::pilo::core::fs::fs_util::ePS_ForwardSlash || e1 == ::pilo::core::fs::fs_util::ePS_None);
                M_ASSERT(e2 == ::pilo::core::fs::fs_util::ePS_ForwardSlash || e2 == ::pilo::core::fs::fs_util::ePS_None);
                
                if (path1[len - 1] != '/')
                {
                    ::pilo::core::string::string_util::concatenate_string(path1, path1_buffer_size, "/", 1);
                }
                ::pilo::core::string::string_util::concatenate_string(path1, path1_buffer_size, path2, path2_len);
#endif

                return ::pilo::EC_OK;
                
            }

            ::pilo::core::fs::fs_util::EnumPathSeparator fs_util::calculate_path_separator(const char* path)
            {
                bool bFSFound = (::pilo::core::string::string_util::find<char>(path, "/", MC_INVALID_SIZE) != nullptr);
                bool bBSFound = (::pilo::core::string::string_util::find<char>(path, "\\", MC_INVALID_SIZE) != nullptr);

                if ((!bFSFound) && (!bBSFound)) {
                    return ::pilo::core::fs::fs_util::ePS_None;
                }
                else if ((bFSFound) && (bBSFound)) {
                    return ::pilo::core::fs::fs_util::ePS_Both;
                }
                else if ((!bFSFound) && (bBSFound)) {
                    return ::pilo::core::fs::fs_util::ePS_BackwardSlash;
                }
                else { // ((bFSFound) && (! bBSFound)){
                    return ::pilo::core::fs::fs_util::ePS_ForwardSlash;
                }
            }

            char* fs_util::get_current_working_directory(char* buffer, size_t bufferSize, bool* bUseHeap, size_t* outSize, bool bAppendSep)
            {
                char* strRet = 0;                
                size_t ret_size = 0;

                if (bUseHeap != 0)
                {
                    *bUseHeap = false;
                }                

#           ifdef  WINDOWS
                size_t szMesured = 0;
                szMesured = (DWORD)::GetCurrentDirectory(0, 0);
                if ((buffer == 0) || (bufferSize == MC_INVALID_SIZE) || (bufferSize == 0) || (szMesured >= bufferSize))
                {                    
                    strRet = (char*) malloc (szMesured + 2);
                    if (strRet == nullptr) 
                    {
                        return 0;
                    }

                    if (bUseHeap != 0) 
                    {
                        *bUseHeap = true;
                    }
                    else
                    {
                        if (buffer != nullptr)
                        {
                            return nullptr;
                        }
                    }

                    size_t szFinal = (size_t) ::GetCurrentDirectory((DWORD)szMesured, strRet);
                    ret_size = szFinal;
                }
                else 
                {  //use user buffer case
                    size_t szFinal = (size_t) ::GetCurrentDirectory((DWORD)bufferSize, buffer);
                    if (szFinal == 0) 
                    {
                        return 0; //This native Function will return Zero on failed
                    }

                    if (bUseHeap != 0) 
                    {
                        *bUseHeap = false;
                    }
                    
                    strRet = buffer;
                    ret_size = szFinal;
                }

#           else

                strRet = ::getcwd(buffer, bufferSize-1);
                if (strRet == nullptr && errno != ERANGE)
                {
                    return nullptr;
                }

                if (strRet != nullptr)
                {
                    if (bUseHeap != 0) 
                    {
                        *bUseHeap = false;
                    }
                }
                else
                {
                    strRet = ::getcwd(nullptr, 0);
                    if (bUseHeap != 0) 
                    {
                        *bUseHeap = true;
                    }
                }

                ret_size = ::pilo::core::string::string_util::length(strRet);                
#           endif

                if (bAppendSep)
                {
                    if (strRet[ret_size-1] != M_PATH_SEP_C)
                    {
                        strRet[ret_size] = M_PATH_SEP_C;
                        ret_size ++;
                        strRet[ret_size] = 0;
                    }
                }

                if (outSize != nullptr)
                {  
                    *outSize = ret_size;
                }
            
                return strRet;
            }

            ::pilo::i32_t fs_util::check_path(const char* path)
            {
                if (path == nullptr) 
                {
                    return ::pilo::EC_NULL_PTR;
                }

                if (*path == 0)
                {
                    return ::pilo::EC_NULL_PTR;
                }

                size_t tmpLen = ::pilo::core::string::string_util::length(path);
                if (tmpLen == 0) 
                {
                    return ::pilo::EC_INVALID_PATH;
                }

#           ifdef WINDOWS
                if ((tmpLen) == 2 && isalpha(path[0]) && path[1] == ':')
                {
                    return ::pilo::EC_OK;
                }
                if ((tmpLen) >= 3 && isalpha(path[0]) && path[1] == ':' && path[2] == '\\')
                {
                    return ::pilo::EC_OK;
                }
                if ((tmpLen) >= 3 && isalpha(path[0]) && path[1] == ':' && path[2] == '/')
                {
                    return ::pilo::EC_INVALID_PATH;
                }

                if (strstr(path, "\\\\"))
                {
                    return ::pilo::EC_INVALID_PATH;
                }           
#else
                if ((tmpLen) == 1 && path[0] == '/')
                {
                    return ::pilo::EC_OK;
                }

                if (strstr(path, "//"))
                {
                    return ::pilo::EC_INVALID_PATH;
                }

#endif

                //tocheck ... or .... sort of
                const char* pFind = ::pilo::core::string::string_util::find(path, "..");
                if (pFind != nullptr)
                {
                    if ((*(pFind + 2)) == '.')
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }
                }

                const char* pch = path;
                while (*pch != 0) 
                {
                    if (::pilo::core::string::string_util::is_invalid_path_char(*pch, pch - path)) 
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }
                    pch++;
                }

                if (::pilo::core::fs::fs_util::calculate_path_separator(path) == ::pilo::core::fs::fs_util::ePS_Both) 
                {
                    return ::pilo::EC_INVALID_PATH;
                }

                return EC_OK;
            }

            bool fs_util::is_absolute_path(const char* path)
            {
                if (path == nullptr)
                {
                    return false;
                }

                size_t sz = ::pilo::core::string::string_util::length(path);
#           ifdef  WINDOWS
                if (*path == 0)
                {
                    return false;
                }
                if (sz < 2)
                {
                    return false;
                }
                if (isalpha(path[0]) && path[1] == ':')
                {
                    return true;                    
                }               
 
#           else
                if (sz == 0 && path[0] == 0)
                {
                    return true;
                }
                if ((sz > 0) && (path[0] == '/')) 
                {
                    return true;
                }
#           endif

                return false;
            }

            ::pilo::core::fs::fs_util::EnumFSNodeType fs_util::calculate_type(const char* path)
            {                
                size_t len = ::pilo::core::string::string_util::length(path);
                if (len == MC_INVALID_SIZE || len == 0) return eFSNT_InvalidPath;
   

#           ifdef  WINDOWS
                wchar_t wszPathBuffer[MC_PATH_MAX] = { 0 };
                const wchar_t* pRetPth = convert_str_to_wstr(wszPathBuffer, MC_PATH_MAX, path, MC_INVALID_SIZE, CP_ACP);
                DWORD attr(::GetFileAttributesW(pRetPth));
                if (pRetPth != nullptr && pRetPth != wszPathBuffer)
                {
                    delete pRetPth;
                }
                if (attr == INVALID_FILE_ATTRIBUTES)
                {
                    return eFSNT_NonExist;
                }

                return ((attr & FILE_ATTRIBUTE_DIRECTORY)
                        ? eFSNT_Directory
                        : eFSNT_RegularFile);


#           else
                struct stat stBuff;
                if (stat(path, &stBuff) != 0) {
                    if (errno == ENOENT) {
                        return eFSNT_NonExist;
                    }
                    return eFSNT_Error;
                }

                if ((stBuff.st_mode & S_IFDIR) == S_IFDIR) {
                    return eFSNT_Directory;
                }
                else if ((stBuff.st_mode & S_IFREG) == S_IFREG) {
                    return eFSNT_RegularFile;
                }
                return eFSNT_TypeUndefined;
#           endif
            }

            bool fs_util::file_exist(const char* path)
            {
                return (eFSNT_RegularFile == ::pilo::core::fs::fs_util::calculate_type(path));
            }

            bool fs_util::dir_exist(const char* path)
            {
                return (eFSNT_Directory == ::pilo::core::fs::fs_util::calculate_type(path));
            }

            bool fs_util::is_root(const char* path)
            {
                if (path == nullptr)
                {
                    return false;
                }

                if (*path == 0)
                {
                    return false;
                }

                if (::pilo::core::fs::fs_util::check_path(path) != EC_OK)
                {
                    return false;
                }

                size_t len = ::pilo::core::string::string_util::length(path);        
                
#ifdef  WINDOWS
                ::pilo::core::fs::fs_util::EnumPathSeparator psep = ::pilo::core::fs::fs_util::calculate_path_separator(path);
                if (psep == ::pilo::core::fs::fs_util::ePS_BackwardSlash)
                {
                    if (len == 3)
                    {
                        if (isalpha(path[0]) && path[1] == ':' && path[2] == '\\')
                        {
                            return true;
                        }
                    }
                }
                else if (psep == ::pilo::core::fs::fs_util::ePS_ForwardSlash)
                {
                    if (len == 3)
                    {
                        if (isalpha(path[0]) && path[1] == ':' && path[2] == '/')
                        {
                            return true;
                        }
                    }
                }

#           else
                if (len == 1 && path[0] == '/')
                {
                    return true;
                }

#           endif

                return false;
            }

            size_t lv = 0;
            ::pilo::error_number_t fs_util::get_path_depth(size_t& dep, const char* path)
            {
                if (path == nullptr)
                {
                    return ::pilo::EC_NULL_PARAM;
                }

                if (*path == 0)
                {
                    return ::pilo::EC_INVALID_PATH;
                }

                if (::pilo::core::fs::fs_util::check_path(path) != EC_OK)
                {
                    return ::pilo::EC_INVALID_PATH;
                }

                size_t len = ::pilo::core::string::string_util::length(path);
                
                if (! ::pilo::core::fs::fs_util::is_absolute_path(path))
                {
                    for (size_t i=0; i<len; i++)
                    {
#           ifdef  WINDOWS
                        if (path[i] == '/' || path[i] == '\\')
                        {
                            lv ++;
                        }

#           else
                        if (path[i] == '/')
                        {
                            lv ++;
                        }
#           endif
                    }

                    dep = lv;
                    return ::pilo::EC_OK;
                }                

#           ifdef  WINDOWS
                ::pilo::core::fs::fs_util::EnumPathSeparator psep = ::pilo::core::fs::fs_util::calculate_path_separator(path);
                if (psep == ::pilo::core::fs::fs_util::ePS_BackwardSlash)
                {
                    if (len == 3)
                    {
                        dep = 0;
                        return ::pilo::EC_OK;
                    }
                    for (size_t i = 0; i < len; i++)
                    {
                        if (path[i] == '\\')
                        {
                            lv++;
                        }
                    }
                }
                else if (psep == ::pilo::core::fs::fs_util::ePS_ForwardSlash)
                {
                    if (len == 3)
                    {
                        dep = 0;
                        return ::pilo::EC_OK;
                    }
                    for (size_t i = 0; i < len; i++)
                    {
                        if (path[i] == '/')
                        {
                            lv++;
                        }
                    }
                }

#           else
                if (len == 1)
                {
                    dep = 0;
                    return ::pilo::EC_OK;
                }
                for (size_t i = 0; i < len; i++)
                {
                    if (path[i] == '/')
                    {
                        lv++;
                    }
                }
#           endif


                dep = lv;
                return ::pilo::EC_OK;

            }

            pilo::error_number_t fs_util::lock_file(os_file_descriptor_t fildes, bool is_exclusive, size_t start_pos, size_t size_to_lock)
            {
#           ifdef  WINDOWS
                DWORD flag = 0;
                if (is_exclusive)
                {
                    flag = LOCKFILE_EXCLUSIVE_LOCK;
                }

                OVERLAPPED overlapvar = { 0 };
                ULARGE_INTEGER li_size;
                li_size.QuadPart = (size_to_lock == MC_INVALID_SIZE) ? UINT64_MAX : size_to_lock;
                LARGE_INTEGER li_start_pos;
                li_start_pos.QuadPart = start_pos;
                overlapvar.Offset = li_start_pos.LowPart;
                overlapvar.OffsetHigh = li_start_pos.HighPart;
                
                BOOL success = ::LockFileEx(fildes, flag, 0, li_size.LowPart, li_size.HighPart, &overlapvar);
                if (! success)
                {
                    return MAKE_SYSERR(::pilo::EC_LOCK_FILE_ERROR);
                }
                
#           else
                struct  flock   param ;
                int             type_param = (is_exclusive) ? F_WRLCK : F_RDLCK;
                size_t          size_param = (size_to_lock == MC_INVALID_SIZE) ? 0 : size_to_lock;

                param.l_type = type_param;
                param.l_whence = SEEK_SET;
                param.l_start = start_pos; 
                param.l_len = size_param;                

                if (fcntl(fildes, F_SETLKW, &param) == -1)
                {
                    return MAKE_SYSERR(::pilo::EC_LOCK_FILE_ERROR);
                }
#           endif

                return ::pilo::EC_OK;
            }

            pilo::error_number_t fs_util::try_lock_file(os_file_descriptor_t fildes, bool is_exclusive, size_t start_pos, size_t size_to_lock)
            {
#           ifdef  WINDOWS
                DWORD flag = 0;
                if (is_exclusive)
                {
                    flag = LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY;
                }

                OVERLAPPED overlapvar = { 0 };
                ULARGE_INTEGER li_size;
                li_size.QuadPart = (size_to_lock == MC_INVALID_SIZE) ? UINT64_MAX : size_to_lock;
                LARGE_INTEGER li_start_pos;
                li_start_pos.QuadPart = start_pos;
                overlapvar.Offset = li_start_pos.LowPart;
                overlapvar.OffsetHigh = li_start_pos.HighPart;

                BOOL success = ::LockFileEx(fildes, flag, 0, li_size.LowPart, li_size.HighPart, &overlapvar);
                if (!success)
                {
                    return MAKE_SYSERR(::pilo::EC_LOCK_FILE_ERROR);
                }

#           else
                struct  flock   param;
                int             type_param = (is_exclusive) ? F_WRLCK : F_RDLCK;
                size_t          size_param = (size_to_lock == MC_INVALID_SIZE) ? 0 : size_to_lock;

                param.l_type = type_param;
                param.l_whence = SEEK_SET;
                param.l_start = start_pos;
                param.l_len = size_param;

                if (fcntl(fildes, F_SETLK, &param) == -1)
                {
                    return MAKE_SYSERR(::pilo::EC_LOCK_FILE_ERROR);
                }
#           endif

                return ::pilo::EC_OK;
            }

            pilo::error_number_t fs_util::unlock_file(os_file_descriptor_t fildes, size_t start_pos, size_t size_to_lock)
            {
#           ifdef  WINDOWS
                OVERLAPPED overlapvar = { 0 };
                ULARGE_INTEGER li_size;
                li_size.QuadPart = (size_to_lock == MC_INVALID_SIZE) ? UINT64_MAX : size_to_lock;
                LARGE_INTEGER li_start_pos;
                li_start_pos.QuadPart = start_pos;
                overlapvar.Offset = li_start_pos.LowPart;
                overlapvar.OffsetHigh = li_start_pos.HighPart;

                BOOL success = ::UnlockFileEx(fildes, 0, li_size.LowPart, li_size.HighPart, &overlapvar);
                if (!success)
                {
                    return MAKE_SYSERR(::pilo::EC_LOCK_FILE_ERROR);
                }


#           else
                struct  flock   param;
                size_t          size_param = (size_to_lock == MC_INVALID_SIZE) ? 0 : size_to_lock;

                param.l_type = F_UNLCK;;
                param.l_whence = SEEK_SET;
                param.l_start = start_pos;
                param.l_len = size_param;

                if (fcntl(fildes, F_SETLK, &param) == -1)
                {
                    return MAKE_SYSERR(::pilo::EC_LOCK_FILE_ERROR);
                }
#           endif

                return ::pilo::EC_OK;
            }            

            ::pilo::error_number_t fs_util::split_path_to_dir_and_filename(char* dirpath, size_t dirpath_size, char* filename, size_t filename_size, const char* path)
            {
                if (path == nullptr || *path == 0) return ::pilo::EC_NULL_PARAM;

                size_t len = ::pilo::core::string::string_util::length(path);

                if (path[len - 1] == M_PATH_SEP_C)
                {
                    return ::pilo::EC_INVALID_PATH;
                }

                const char* sep = ::pilo::core::string::string_util::find_reversely(path, M_PATH_SEP_S);
                if (sep == nullptr)
                {
                    if (filename != 0)
                    {
                        size_t len = ::pilo::core::string::string_util::length(path);
                        if (filename_size <= len)
                        {
                            return ::pilo::EC_BUFFER_TOO_SMALL;
                        }
                        ::pilo::core::string::string_util::copy(filename, filename_size, path, len);
                    }
                    if (dirpath_size != 0)
                    {
                        *dirpath = 0;
                    }
                }
                else
                {
                    size_t dlen = sep - path;
                    if (dirpath != nullptr)
                    {                        
                        if (::pilo::core::string::string_util::copy(dirpath, dirpath_size, path, dlen) != dlen)
                        {
                            return ::pilo::EC_BUFFER_TOO_SMALL;
                        }
                    }
                    if (filename != nullptr)
                    {
                        ::pilo::i64_t flen = ((path+len-1) - sep);
                        if (flen < 0)
                        {
                            return ::pilo::EC_UNDEFINED;
                        }
                        if (::pilo::core::string::string_util::copy(filename, filename_size, sep+1, (size_t)flen) != (size_t) flen)
                        {
                            return ::pilo::EC_BUFFER_TOO_SMALL;
                        }
                    }
                    

                }

                return ::pilo::EC_OK;
            }


            ::pilo::error_number_t fs_util::create_directory_recursively(const char* dir_path, bool force)
			{
				if (dir_path == nullptr) return ::pilo::EC_NULL_PARAM;

				char path[MC_PATH_MAX];
                memset(path, 0x00, MC_PATH_MAX);
				const char* saved_pos = dir_path;
                const char* pos = nullptr;
                ::pilo::i32_t ret = ::pilo::EC_OK;

				while ((pos = ::pilo::core::string::string_util::find(saved_pos, M_PATH_SEP_S)) != nullptr)
				{
					::memcpy(path, dir_path, pos - dir_path + 1);
					pos++;
                   // printf("---> Checking (%s)\n",path);
                    EnumFSNodeType fs_node_type = ::pilo::core::fs::fs_util::calculate_type(path);
                    if (fs_node_type == eFSNT_Directory)
					{
                       // printf("\t---> Checing (%s), is DIR Cont\n", path);
                        saved_pos = pos;
						continue;
					}
					else
					{
                        if (fs_node_type == eFSNT_RegularFile)
                        {
                            if (force)
                            {
                               // printf("\t---> Checing (%s), is REGF del\n", path);

                                ret = ::pilo::core::fs::fs_util::delete_regular_file(path);
                                if (ret != ::pilo::EC_OK)
                                {
                                    return MAKE_SYSERR(::pilo::EC_DELETE_FILE_ERROR);
                                }
                                ret = ::pilo::core::fs::fs_util::create_directory(path, 0);
                                if (ret != ::pilo::EC_OK)
                                {
                                    return MAKE_SYSERR(::pilo::EC_OPEN_DIR_ERROR);
                                }
                            }
                            else
                            {
                               // printf("\t---> Checing (%s), is REGF errret\n", path);
                                return ::pilo::EC_FILE_ALREADY_EXIST;
                            }
                            
                        }
                        else if (fs_node_type == eFSNT_NonExist)
                        {
                            ret = ::pilo::core::fs::fs_util::create_directory(path, 0);
                            if (ret != ::pilo::EC_OK)
                            {
                                ret = ::pilo::core::fs::fs_util::create_directory(path, 0);
                                if (ret != ::pilo::EC_OK)
                                {
                                    return MAKE_SYSERR(::pilo::EC_OPEN_DIR_ERROR);
                                }
                            }
                        }
                        else
                        {
                            return ::pilo::EC_UNDEFINED_FILE_TYPE;
                        }		
					}

                    saved_pos = pos;
				}

                if (*saved_pos != 0)
                {
                    size_t len = ::pilo::core::string::string_util::length(dir_path);
                    ::memcpy(path, dir_path, len);
                    EnumFSNodeType fs_node_type = ::pilo::core::fs::fs_util::calculate_type(path);
                    if (fs_node_type == eFSNT_Directory)
                    {
                        return ::pilo::EC_OK;
                    }
                    else
                    {
                        if (fs_node_type == eFSNT_RegularFile)
                        {
                            if (force)
                            {
                                ::pilo::i32_t ret = ::pilo::core::fs::fs_util::delete_regular_file(path);
                                if (ret != ::pilo::EC_OK)
                                {
                                    return MAKE_SYSERR(::pilo::EC_DELETE_FILE_ERROR);
                                }
                                ret = ::pilo::core::fs::fs_util::create_directory(path, 0);
                                if (ret != ::pilo::EC_OK)
                                {
                                    return MAKE_SYSERR(::pilo::EC_OPEN_DIR_ERROR);
                                }
                            }
                            else
                            {
                                return ::pilo::EC_FILE_ALREADY_EXIST;
                            }

                        }
                        else if (fs_node_type == eFSNT_NonExist)
                        {
                       ////     printf("\twill creating last dir (%s)\n", path);
                            ret = ::pilo::core::fs::fs_util::create_directory(path, 0);
                            if (ret != ::pilo::EC_OK)
                            {
                                ret = ::pilo::core::fs::fs_util::create_directory(path, 0);
                                if (ret != ::pilo::EC_OK)
                                {
                                    return MAKE_SYSERR(::pilo::EC_OPEN_DIR_ERROR);
                                }                                
                            }
                        }
                        else
                        {
                            return ::pilo::EC_UNDEFINED_FILE_TYPE;
                        }


                    }
                }

				return ::pilo::EC_OK;
			}

            ::pilo::error_number_t fs_util::delete_fs_node(const char* path, bool force)
			{
				if (path == nullptr) return ::pilo::EC_NULL_PARAM;

				EnumFSNodeType e_type = ::pilo::core::fs::fs_util::calculate_type(path);
				if (e_type == eFSNT_RegularFile)
				{
					return ::pilo::core::fs::fs_util::delete_regular_file(path);
				}
				else if (e_type == eFSNT_Directory)
				{
					::pilo::i32_t ret = ::pilo::core::fs::fs_util::delete_empty_directory(path);
					if ( (ret == ::pilo::EC_OK) || !force )
					{
						return ret;
					}

					return ::pilo::core::fs::fs_util::delete_directory(path, true);
				}

				return ::pilo::EC_UNDEFINED_FILE_TYPE;
			}

            ::pilo::error_number_t fs_util::delete_regular_file(const char* path)
            {
				if (path == nullptr)
				{
					return ::pilo::EC_NULL_PARAM;
				}

#               ifdef  WINDOWS


                if (::DeleteFile(path) == TRUE)
                {					
					return ::pilo::EC_OK;
                }

#               else

                if (::unlink(path) == 0)
                {
                    return ::pilo::EC_OK;
                }

#               endif

                return MAKE_SYSERR(::pilo::EC_DELETE_FILE_ERROR);
            }

            

            ::pilo::error_number_t fs_util::delete_directory(const char* path, bool inc_dir)
			{
				if (path == nullptr) return ::pilo::EC_NULL_PARAM;

                ::pilo::core::fs::path_string<MC_PATH_MAX> strpath(path);
                return delete_directory<MC_PATH_MAX>(strpath, inc_dir);
			}

            ::pilo::error_number_t fs_util::delete_empty_directory(const char* path)
            {
#               ifdef WINDOWS
                if (path == nullptr) return ::pilo::EC_NULL_PARAM;        

                BOOL bRet = ::RemoveDirectory(path);
                if ( bRet != TRUE) //2=ne 145=nem
                {                    
                    DWORD ret = ::GetLastError();                    
                    if (ret == ERROR_FILE_NOT_FOUND)
                    {
                        return ::pilo::EC_DIR_NOT_EXIST;
                    }
                    else if (ret == ERROR_DIR_NOT_EMPTY)
                    {
                        bRet = ::RemoveDirectory(path);
                        if (bRet) return ::pilo::EC_OK;
                        return ::pilo::EC_DIR_NOT_EMPTY;
                    }
                    return MAKE_SYSERR(::pilo::EC_DELETE_DIR_ERROR);
                }

#               else
                if (::rmdir(path) != 0)
                {
                    if (ENOENT == errno)
                    {
                        return ::pilo::EC_DIR_NOT_EXIST;
                    }
                    if (EEXIST == errno && ENOTEMPTY == errno)
                    {
                        return ::pilo::EC_DIR_NOT_EMPTY;
                    }                   

                    return MAKE_SYSERR(::pilo::EC_DELETE_DIR_ERROR);
                }
#               endif

                
                return ::pilo::EC_OK;
            }

            ::pilo::error_number_t fs_util::create_regular_file(const char* path, bool always)
            {
                DeviceAccessModeEnumeration eMode;

                if (always)
                {
                    eMode = ::pilo::core::fs::eDAM_CreateAlways;
                }
                else
                {
                    eMode = ::pilo::core::fs::eDAM_CreateNew;
                }

                char dir_path_buffer[MC_PATH_MAX];
                if (::pilo::EC_OK != ::pilo::core::fs::fs_util::split_path_to_dir_and_filename(dir_path_buffer, sizeof(dir_path_buffer), nullptr, 0, path))
                {
                    return ::pilo::EC_INVALID_PATH;
                }

                if (::pilo::EC_OK != ::pilo::core::fs::fs_util::create_directory_recursively(dir_path_buffer, always))
                {
                    return ::pilo::EC_CREATE_DIR_ERROR;
                }

                ::pilo::os_file_descriptor_t fd = MC_INVALID_FILE_DESCRIPTOR;
                ::pilo::core::fs::fs_util::open_file(fd, path, eMode, ::pilo::core::fs::eDRWM_ReadWrite, 0);
                if (fd == MC_INVALID_FILE_DESCRIPTOR)
                {
                    return ::pilo::EC_OPEN_FILE_FAILED;
                }

                if (::pilo::EC_OK != ::pilo::core::fs::fs_util::close_file(fd, 0))
                {
                    return ::pilo::EC_CLOSE_FILE_ERROR;
                }

                return ::pilo::EC_OK;
            }

            ::pilo::error_number_t fs_util::open_file(  ::pilo::os_file_descriptor_t& fildes, 
                                                        const char* path, 
                                                        ::pilo::core::fs::DeviceAccessModeEnumeration dam, 
                                                        ::pilo::core::fs::DeviceRWModeEnumeration drwm, 
                                                        ::pilo::u32_t flags)
            {
                
                fildes = MC_INVALID_FILE_DESCRIPTOR;
                
                if (path == nullptr)
                {
                    return ::pilo::EC_NULL_PARAM;
                }

#               ifdef  WINDOWS
                DWORD dwAccesss = (DWORD) drwm;
                if (flags & MB_IO_DEV_OPEN_FLAG_APPEND)
                {
                    dwAccesss = FILE_APPEND_DATA;
                }

                DWORD internal_flags = FILE_ATTRIBUTE_NORMAL;
                if (flags & MB_IO_DEV_OPEN_SYNC)
                {
                    internal_flags |= FILE_FLAG_WRITE_THROUGH;
                }
                if (flags & MB_IO_DEV_OPEN_NO_OS_CACHE)
                {
                    internal_flags |= FILE_FLAG_NO_BUFFERING;
                    internal_flags |= FILE_FLAG_WRITE_THROUGH;
                }

                fildes = ::CreateFile(path, dwAccesss, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, dam, internal_flags, 0);

#               else
                if (flags & MB_IO_DEV_OPEN_SYNC)
                {
                    flags |= O_DSYNC;
                }
                if (flags & MB_IO_DEV_OPEN_NO_OS_CACHE)
                {
                    flags |= O_SYNC;
                    flags |= O_DIRECT;
                }

                fildes = open(path, (int) drwm | dam | flags);

#               endif

                if (fildes == MC_INVALID_FILE_DESCRIPTOR)
                {
                    return ::pilo::EC_OPEN_FILE_FAILED;
                }

                return ::pilo::EC_OK;

            }

            ::pilo::error_number_t fs_util::close_file(::pilo::os_file_descriptor_t filedes, ::pilo::u32_t flags)
            {
                M_UNUSED(flags);
                if (filedes == MC_INVALID_FILE_DESCRIPTOR)
                {
                    return ::pilo::EC_INVALID_PARAM;
                }

                ::pilo::i32_t ret = ::pilo::EC_OK;

#               ifdef  WINDOWS

                if (::CloseHandle(filedes) != TRUE)
                {
                    ret = ::pilo::EC_CLOSE_FILE_ERROR;
                }

#               else

                if ((close(filedes)) != 0)
                {
                    ret = ::pilo::EC_CLOSE_FILE_ERROR;
                }

#               endif
                

                return ret;
                
            }

            ::pilo::error_number_t fs_util::truncate_file(::pilo::os_file_descriptor_t fildes, size_t sz, bool reset_ptr)
            {                
#ifdef WINDOWS
                if (seek_file(fildes, (ssize_t) sz, eDSW_Begin) != ::pilo::EC_OK)
                {
                    return MAKE_SYSERR(::pilo::EC_DEV_SEEK_ERROR);
                }

                if (!SetEndOfFile(fildes))
                {
                    return MAKE_SYSERR(::pilo::EC_DEV_TRUNK_ERROR);
                }
#else
                if (ftruncate(fildes, sz))
                {
                    return MAKE_SYSERR(::pilo::EC_DEV_TRUNK_ERROR);
                }
#endif

                if (reset_ptr)
                {
                    if (seek_file(fildes, 0, eDSW_Begin) != ::pilo::EC_OK)
                    {
                        return MAKE_SYSERR(::pilo::EC_DEV_SEEK_ERROR);
                    }
                }

                return ::pilo::EC_OK;
            }

            ::pilo::error_number_t fs_util::truncate_file(const char* path, size_t sz)
            {
#ifdef WINDOWS
                os_file_descriptor_t fd;
                ::pilo::error_number_t ret = ::pilo::core::fs::fs_util::open_file(fd, path, ::pilo::core::fs::eDAM_OpenExisting, ::pilo::core::fs::eDRWM_Write, 0);
                if (ret != ::pilo::EC_OK)
                {
                    return MAKE_SYSERR(::pilo::EC_OPEN_FILE_FAILED);
                }
                
                if (::pilo::EC_OK != truncate_file(fd, sz, false))
                {
                    return ::pilo::EC_DEV_TRUNK_ERROR;
                }

                if (::pilo::EC_OK !=  ::pilo::core::fs::fs_util::close_file(fd ,0))
                {
                    return ::pilo::EC_CLOSE_FILE_ERROR;
                }

#else
                if (truncate(path, sz))
                {
                    return MAKE_SYSERR(::pilo::EC_DEV_TRUNK_ERROR);
                }
#endif

                return ::pilo::EC_OK;
            }

            ::pilo::error_number_t fs_util::seek_file(::pilo::os_file_descriptor_t fildes, ssize_t sz, DeviceSeekWhenceEnumeration eWhence)
            {
#ifdef WINDOWS
                LARGE_INTEGER li;
                li.QuadPart = sz;
                if (!::SetFilePointerEx(fildes, li, NULL, eWhence))
                {
                    return MAKE_SYSERR(::pilo::EC_DEV_SEEK_ERROR);
                }

#else
                off64_t nRet = lseek64(fildes, sz, eWhence);
                if (nRet == -1)
                {
                    return MAKE_SYSERR(::pilo::EC_DEV_SEEK_ERROR);
                }
#endif
                return ::pilo::EC_OK;
            }

            ::pilo::error_number_t fs_util::get_file_modified_time(::pilo::core::datetime::datetime &dt, const char* filepath)
            {
                MP_CHECK_EMPTY_CSTR_RET(filepath, ::pilo::EC_INVALID_PARAM);

#ifdef WINDOWS
                FILETIME modified_os_time = {0};
                FILETIME modified_os_time_local = { 0 };
                FILETIME created_os_time = { 0 };
                FILETIME accessed_os_time = { 0 };
                HANDLE hFile = ::CreateFile(filepath, GENERIC_READ,          // open for reading
                    FILE_SHARE_READ,       // share for reading
                    NULL,                            // default security
                    OPEN_EXISTING,          // existing file only
                    FILE_FLAG_BACKUP_SEMANTICS, // normal file
                    NULL);
                if (!GetFileTime(hFile, &created_os_time, &accessed_os_time, &modified_os_time))
                {
                    ::CloseHandle(hFile);
                    return MAKE_SYSERR(::pilo::EC_GET_FILE_TIME_ERROR);
                }              
                ::CloseHandle(hFile);
                SYSTEMTIME local_os_time;

                FileTimeToLocalFileTime(&modified_os_time, &modified_os_time_local);

                ZeroMemory(&local_os_time, sizeof(SYSTEMTIME));
                FileTimeToSystemTime(&modified_os_time_local, &local_os_time);  
                

                ::pilo::core::datetime::local_datetime pilo_local_dt;
                pilo_local_dt.year = local_os_time.wYear;
                pilo_local_dt.month = local_os_time.wMonth;
                pilo_local_dt.day = local_os_time.wDay;
                pilo_local_dt.hour = local_os_time.wHour;
                pilo_local_dt.minute = local_os_time.wMinute;
                pilo_local_dt.second = local_os_time.wSecond;
                pilo_local_dt.millisecond = local_os_time.wMilliseconds;

                dt.from_local_datetime(pilo_local_dt);

#else
                struct stat stbuf;
                if (::stat(filepath, &stbuf) == -1)
                {
                    return MAKE_SYSERR(::pilo::EC_GET_FILE_TIME_ERROR);
                }

                dt = ((::pilo::i64_t) stbuf.st_mtime);

#endif // WINDOWS
                


                return ::pilo::EC_OK;
            }            

            ::pilo::i64_t fs_util::get_file_modified_time(const char* filepath)
            {
                ::pilo::i64_t t = -1;
#ifdef WINDOWS
                HANDLE hFile = ::CreateFile(
                    filepath,
                    0,
                    FILE_SHARE_READ,
                    NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL);

                if (hFile == INVALID_HANDLE_VALUE)
                {
                    return -1;
                }

                FILETIME lwt;                
                if (::GetFileTime(hFile, 0, 0, &lwt))
                {
                    t = (static_cast<__int64>(lwt.dwHighDateTime) << 32) + lwt.dwLowDateTime;
                }
                ::CloseHandle(hFile);
#else
                struct stat stbuf;
                if (::stat(filepath, &stbuf) == -1)
                {
                    return MAKE_SYSERR(::pilo::EC_GET_FILE_TIME_ERROR);
                }

                t = (::pilo::i64_t) stbuf.st_mtime;
#endif
                return t;
            }

            ::pilo::error_number_t fs_util::_compact_path_once(char * pBuffer, size_t len)
            {
#ifdef WINDOWS
                if (pBuffer == nullptr || len <= 0)
                {
                    return ::pilo::EC_NULL_PARAM;
                }
#else
                if (pBuffer == nullptr)         
                {
                    return ::pilo::EC_NULL_PARAM;
                }
                else if (*pBuffer==0 && len == 0)
                {                    
                    return ::pilo::EC_NONSENSE_OPERATION;
                }              
                
#endif
                

                char* pEnd = pBuffer + len;

                char* dot2ptr = ::strstr(pBuffer, "..");
                if (dot2ptr == nullptr)
                {
                    return ::pilo::EC_NONSENSE_OPERATION;
                }

                bool bLast = false;
                if (pEnd - dot2ptr <= 2)
                {
                    bLast = true; //.. is last part of path
                }

#ifdef WINDOWS
                if (dot2ptr - pBuffer < 5)
#else
                if (dot2ptr - pBuffer < 3)
#endif
                {
                    return ::pilo::EC_INVALID_PATH;
                }                

                char* p = ::pilo::core::string::string_util::find_reversely(pBuffer, M_PATH_SEP_S, dot2ptr - pBuffer - 1);
                if (p == nullptr)
                {
                    return ::pilo::EC_INVALID_PATH;
                }

                if (bLast)
                {
                    *p = 0;
                }
                else
                {
                    size_t tmplen = strlen(dot2ptr + 2);
                    ::memmove(p, dot2ptr + 2, tmplen);
                    *(p+tmplen) = 0;
                }

                
                return ::pilo::EC_OK;
                
            }

            ::pilo::error_number_t fs_util::compact_path(char * pBuffer, size_t len)
            {
				::pilo::error_number_t ret = ::pilo::EC_OK;
                do
                {
                    ret = _compact_path_once(pBuffer, len);
                    if (ret == ::pilo::EC_NONSENSE_OPERATION)
                    {
                        return ::pilo::EC_OK;
                    }
          
                    
                } while (ret == ::pilo::EC_OK);

                return ::pilo::EC_INVALID_PATH;
            }

            ::pilo::i32_t fs_node_delete_visitor::visit(const fs_find_data* data)
            {
                if (data == nullptr) return ::pilo::EC_NULL_PARAM;

                M_UNUSED(data);
                if (data->type() == ::pilo::core::fs::fs_util::eFSNT_Directory)
                {           
                    ;
                }
                else
                {
                    ::pilo::core::fs::fs_util::delete_regular_file(data->full_pathname());
                }

                return  ::pilo::EC_OK;
            }

            ::pilo::i32_t fs_node_delete_visitor::post_dir_visit(const char* path)
            {
                return ::pilo::core::fs::fs_util::delete_empty_directory(path);
			}


			::pilo::i32_t fs_dir_copy_visitor::visit(const ::pilo::core::fs::fs_find_data* data)
			{
				M_UNUSED(data);
				printf("creating file %s\n", data->filename());
				return 0;
			}

			::pilo::i32_t fs_dir_copy_visitor::pre_dir_visit(const char* path)
			{
				M_UNUSED(path);
				printf("creating dir %s\n", path);
				return 0;
			}

        }
    }
}