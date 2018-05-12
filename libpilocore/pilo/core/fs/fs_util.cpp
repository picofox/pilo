#include "./fs_util.hpp"
#include "core/string/string_util.hpp"
#include "core/io/format_output.hpp"
#include <cctype>
#include <time.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <stdio.h>  
#include <errno.h>  


#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib ")
#include <boost/filesystem.hpp>      //boost          


namespace pilo
{
    namespace core
    {
        namespace fs
        {

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
                size_t szMesured = 0;
                size_t ret_size = 0;

                if (bUseHeap != 0)
                {
                    *bUseHeap = false;
                }                

#           ifdef  WINDOWS
                szMesured = (DWORD)::GetCurrentDirectory(0, 0);
                if ((buffer == 0) || (bufferSize == MC_INVALID_SIZE) || (bufferSize == 0) || (szMesured >= bufferSize))
                {                    
                    strRet = (char*) malloc (szMesured + 1);
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
                char tmpPathBuffer[MC_PATH_MAX];
                char* retTmpFinal;

                retTmpFinal = ::getcwd(buffer, bufferSize-1);
                if (retTmpFinal == nullptr && errno != ERANGE)
                {
                    return ::pilo::EC_UNDEFINED;
                }

                if (retTmpFinal != nullptr)
                {
                    if (bUseHeap != 0) 
                    {
                        *bUseHeap = false;
                    }
                }
                else
                {
                    retTmpFinal = ::getcwd(nullptr, 0);
                    if (bUseHeap != 0) 
                    {
                        *bUseHeap = true;
                    }
                }

                ret_size = ::pilo::core::string::string_util::length(retTmpFinal);                
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
                if (path == nullptr || *path == 0)
                {
                    return false;
                }

                size_t sz = ::pilo::core::string::string_util::length(path);
#           ifdef  WINDOWS
                if (sz < 2)
                {
                    return false;
                }
                if (isalpha(path[0]) && path[1] == ':')
                {
                    return true;                    
                }               
 
#           else
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
#ifdef WINDOWS
            ::pilo::error_number_t fs_util::travel_path_preorder(const char* root, fs_node_visitor_interface* fsnvi, bool stop_on_error, bool visit_last_dir)
            {
                if (root == nullptr || *root == '\0')
                {
                    return ::pilo::EC_NULL_PARAM;
                }      

                bool has_error = false;
                HANDLE  handle;
                WIN32_FIND_DATA  findData;
                char szBuff[MC_PATH_MAX] = { 0 };

                try
                {
                    size_t len = ::pilo::core::string::string_util::length(root);
                    ::pilo::core::string::string_util::copy(szBuff, sizeof(szBuff), root, len);
					if (szBuff[len - 1] != M_PATH_SEP_C)
                    {
                        szBuff[len] = M_PATH_SEP_C;
                        szBuff[len + 1] = 0;
                    }                    
                    ::pilo::core::string::string_util::concatenate_string(szBuff, sizeof(szBuff), "*.*", MC_INVALID_SIZE);
                    
                    handle = FindFirstFile(szBuff, &findData);    // 查找目录中的第一个文件
                    if (handle == INVALID_HANDLE_VALUE)
                    {
                        return MAKE_SYSERR(::pilo::EC_OPEN_DIR_ERROR);
                    }

                    do
                    {
                        if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)
                            continue;

						int pre_ret = fsnvi->pre_dir_visit(root);
						if (pre_ret != ::pilo::EC_OK)
						{
							if (stop_on_error)
							{
                                FindClose(handle);
								return pre_ret;
							}
						}

                        ::pilo::core::string::string_util::copy(szBuff, root);
                        size_t len = ::pilo::core::string::string_util::length(szBuff);
                        if (szBuff[len - 1] != M_PATH_SEP_C)
                        {
                            szBuff[len] = M_PATH_SEP_C;
                            szBuff[len + 1] = 0;
                        }
                        ::pilo::core::string::string_util::concatenate_string(szBuff, sizeof(szBuff), findData.cFileName, MC_INVALID_SIZE);

                        if (findData.dwFileAttributes  & FILE_ATTRIBUTE_DIRECTORY)
                        {
                           ::pilo::i32_t ret = travel_path_preorder(szBuff, fsnvi, stop_on_error, true);
                            if (ret != ::pilo::EC_OK)
                            {
                                if (stop_on_error)
                                {
                                    FindClose(handle);
                                    return ret;
                                }
                                else
                                {
                                    has_error = true;
                                }
                            }                            
                        }
                        else
                        {
                            if (fsnvi != nullptr)
                            {
                                fs_find_data fd;
                                fd.set_type(eFSNT_RegularFile);
                                fd.set_full_pathname(root, findData.cFileName);
                                ::pilo::i32_t ret = fsnvi->visit(szBuff, &fd);
                                if (ret != ::pilo::EC_OK)
                                {
                                    if (stop_on_error)
                                    {
                                        FindClose(handle);
                                        return ret;
                                    }
                                    else
                                    {
                                        has_error = true;
                                    }
                                }
                            }
                        }

                    } while (FindNextFile(handle, &findData) != FALSE);    // 查找目录中的下一个文件
                } //end of try
                catch (...)
                {
                }    

                if (visit_last_dir)
                {
                    ::pilo::i32_t post_ret = fsnvi->post_dir_visit(root);
                    if (post_ret != ::pilo::EC_OK)
                    {
                        if (stop_on_error)
                        {
                            FindClose(handle);
                            return post_ret;
                        }
                    }
                }				

                if (FindClose(handle) == -1)
                {
					return  MAKE_SYSERR(::pilo::EC_CLOSE_DIR_ERROR);
                }

                if (has_error)
                {
                    return ::pilo::EC_ITER_DIR_ERROR;   
                }

                return ::pilo::EC_OK;
            }

#else
::pilo::i32_t fs_util::travel_path_preorder(const char* root, fs_node_visitor_interface* fsnvi, bool stop_on_error, bool visit_last_dir)
            {
                if (root == nullptr || *root == '\0')
                {
                    return ::pilo::EC_NULL_PARAM;
                }

                struct stat statBuf;
                DIR *pDir = nullptr;
                size_t path_len = 0;
                size_t len = 0;
                char szBuff[MC_PATH_MAX] = { 0 };
                bool has_error = false;
                struct dirent *dir_result_ptr = NULL;
                struct dirent dir_entry = {0};

                path_len = ::pilo::core::string::string_util::length(root);

                if (path_len >= MC_PATH_MAX - 2)
                {
                    return ::pilo::EC_REACH_UPPER_LIMIT;
                }

                if ((pDir = opendir(root)) == nullptr)
                {                
                    return MAKE_SYSERR(::pilo::EC_OPEN_DIR_ERROR);
                }

                if (!S_ISDIR(statBuf.st_mode))  //当前路径为文件
                {
                    closedir(pDir);
                    return MAKE_SYSERR(::pilo::EC_DIR_NOT_EXIST); 
                }

                while ((! readdir_r(pDir, &dir_entry, &dir_result_ptr)) && (dir_result_ptr != NULL))
                {
                    if ( strcmp(dir_result_ptr->d_name, ".") == 0 ||
                        strcmp(dir_result_ptr->d_name, "..") == 0)
                    {
                        continue;
                    }
                    len = ::pilo::core::string::string_util::length(dir_result_ptr->d_name);
                    if (path_len +  len > MC_PATH_MAX)  //路径超过了最大长度
                    {
                        closedir(pDir);
                        return ::pilo::EC_REACH_UPPER_LIMIT;;
                    }

                    ::pilo::core::string::string_util::copy(szBuff, root);
                    ::pilo::core::string::string_util::concatenate_string(szBuff, sizeof(szBuff) - path_len, M_PATH_SEP_S, 1);
                    ::pilo::core::string::string_util::concatenate_string(szBuff, sizeof(szBuff) - path_len - 1, dir_result_ptr->d_name, MC_INVALID_SIZE);
                
                    if (lstat(szBuff, &statBuf) != 0)
                    {
                        if (stop_on_error)
                        {
                            closedir(pDir);
                            return MAKE_SYSERR(::pilo::EC_OPEN_DIR_ERROR);
                        }
                        else
                        {
                            has_error = true;
                            continue;
                        }                       
                    }
                    if (S_ISDIR(statBuf.st_mode))  //当前路径为目录
                    {
                        ::pilo::i32_t ret = travel_path_preorder(szBuff, fsnvi, stop_on_error, true);
                        if (ret != ::pilo::EC_OK)
                        {
                            if (stop_on_error)
                            {
                                closedir(pDir);
                                return ret;
                            }
                            else
                            {
                                has_error = true;
                            }
                        }
                        else
                        {
                            if (fsnvi != nullptr)
                            {
                                fs_find_data fd;
                                
                                if (S_ISREG(statBuf.st_mode))  
                                {
                                    fd.set_type(eFSNT_RegularFile);
                                }
                                else   
                                {
                                    fd.set_type(eFSNT_TypeUndefined);
                                }

                                fd.set_full_pathname(root, dir_result_ptr->d_name);

                                ::pilo::i32_t ret = fsnvi->visit(szBuff, &fd);
                                if (ret != ::pilo::EC_OK)
                                {
                                    if (stop_on_error)
                                    {
                                        closedir(pDir);
                                        return ret;
                                    }
                                    else
                                    {
                                        has_error = true;
                                    }
                                }
                            }
            
                        }
                        
                    } // end of if is dir
                } //end of while

                closedir(pDir);

                if (visit_last_dir)
                {
                    ::pilo::i32_t post_ret = fsnvi->post_dir_visit(root);    
                    if (post_ret != ::pilo::EC_OK)
                    {
                        if (stop_on_error)
                        {
                            return post_ret;
                        }
                    }  
                }                             

                if (has_error)
                {
                    return ::pilo::EC_ITER_DIR_ERROR;
                }

                return ::pilo::EC_OK;
            }   

#endif

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
            
            ::pilo::error_number_t fs_util::get_absolute_path(char* abs_path, const char* path, size_t d_len /*= MC_INVALID_SIZE*/)
            {
                if (abs_path == nullptr)
                {
                    return ::pilo::EC_NULL_PARAM;
                }
                
                if (path == nullptr)
                {
                    return ::pilo::EC_NULL_PARAM;
                }

                if (*path == 0)
                {
                    return ::pilo::EC_OK;
                }

                if (::pilo::core::fs::fs_util::is_absolute_path(path))
                {
                    size_t ret_len = ::pilo::core::string::string_util::copy(abs_path, d_len, path, MC_INVALID_SIZE);
                    if (MC_INVALID_SIZE == ret_len)
                    {
                        return ::pilo::EC_COPY_STRING_FAILED;
                    }

                    trim_path_last_seperator(abs_path, ret_len);
                    

#               ifdef  WINDOWS
                    if (::pilo::core::string::string_util::rescanable_replace<char>(abs_path, MC_INVALID_SIZE, "/", "\\", nullptr) != ::pilo::EC_OK)
                    {
                        return ::pilo::EC_COPY_STRING_FAILED;
                    }
#               endif


                    return ::pilo::EC_OK;
                }

                ::pilo::core::fs::fs_util::EnumPathSeparator psep = ::pilo::core::fs::fs_util::calculate_path_separator(path);
                if (psep == ::pilo::core::fs::fs_util::ePS_Both)
                {
                    return ::pilo::EC_INVALID_PATH;
                }
#               ifdef  WINDOWS

#               else

                if (psep == ::pilo::core::fs::fs_util::ePS_BackwardSlash)
                {
                    return ::pilo::EC_INVALID_PATH;
                }                

#               endif

                char buffer_max[MC_PATH_MAX] = { 0 };
                bool bUseHeap = false;
                char* p = ::pilo::core::fs::fs_util::get_current_working_directory(buffer_max, sizeof(buffer_max), &bUseHeap, nullptr);
                if (p == nullptr)
                {
                    return ::pilo::EC_INSUFFICIENT_MEMORY;
                }
                size_t cwd_len = ::pilo::core::string::string_util::length(p);

                if (d_len != MC_INVALID_SIZE)
                {
                    if (cwd_len >= (d_len+3))
                    {
                        if (bUseHeap)
                        {
                            MP_SAFE_FREE(p);
                        }
                        return ::pilo::EC_BUFFER_TOO_SMALL;
                    }
                }  

                bool is_root_only = ::pilo::core::fs::fs_util::is_root(p);
                size_t sz_ret = ::pilo::core::string::string_util::copy(abs_path, d_len, p, cwd_len);
                if (bUseHeap)
                {
                    MP_SAFE_FREE(p);
                }
                if (sz_ret != cwd_len)
                {
                    return ::pilo::EC_COPY_STRING_FAILED;
                }

                if (abs_path[cwd_len - 1] != M_PATH_SEP_C)
                {
                    abs_path[cwd_len] = M_PATH_SEP_C;
                    abs_path[cwd_len+1] = 0;
                    cwd_len ++;
                }

                size_t flen = ::pilo::core::string::string_util::length(path);
                if (::pilo::core::string::string_util::concatenate_string(abs_path, d_len, path, flen) == nullptr)
                {
                    return ::pilo::EC_COPY_STRING_FAILED;
                }

#               ifdef  WINDOWS
                if (::pilo::core::string::string_util::rescanable_replace<char>(abs_path, MC_INVALID_SIZE, "/", "\\", nullptr) != ::pilo::EC_OK)
                {
                    return ::pilo::EC_COPY_STRING_FAILED;
                }
#               endif

                size_t start_scan_pos = cwd_len;;

                int fp_level = 0;
                while (flen > 2)
                {
                    size_t cur_pos = start_scan_pos + 3*fp_level;
                    if (abs_path[cur_pos] == '.' && abs_path[cur_pos + 1] == '.' && abs_path[cur_pos + 2] == M_PATH_SEP_C)
                    {
                        fp_level ++;
                    }
                    else
                    {
                        break;
                    }
                }

                if (fp_level == 0)
                {
                    trim_path_last_seperator(abs_path, MC_INVALID_SIZE);
                    return ::pilo::EC_OK;
                }

                if (is_root_only)
                {
                    return ::pilo::EC_INVALID_PATH;
                }
                
                char* ptr_move = &(abs_path[start_scan_pos + 3 * fp_level]);
                flen -= fp_level*3;


                for (int i = (int) start_scan_pos - 2; i >= 0; i--)
                {

                    if (abs_path[i] == M_PATH_SEP_C)
                    {
                        fp_level --;
                        if (fp_level == 0)
                        {
                            ::pilo::core::string::string_util::s_move(abs_path + i + 1, d_len-i - 1, ptr_move, flen);
                            trim_path_last_seperator(abs_path, MC_INVALID_SIZE);
                            return ::pilo::EC_OK;
                        }
                    }
                }   
                                

                return ::pilo::EC_INVALID_PATH;
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
                                return ::pilo::EC_FILE_ALREAY_EXIST;
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
                                return ::pilo::EC_FILE_ALREAY_EXIST;
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

				DWORD dwSavedAttr = ::GetFileAttributes(path);
				::SetFileAttributes(path, FILE_ATTRIBUTE_NORMAL);
                if (::DeleteFile(path) == TRUE)
                {					
					return ::pilo::EC_OK;
                }
				else
				{
					::SetFileAttributes(path, dwSavedAttr);
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

				::pilo::core::fs::fs_node_delete_visitor fnd;
				
                return ::pilo::core::fs::fs_util::travel_path_preorder(path, &fnd, false, inc_dir);
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

                if (::pilo::core::fs::fs_util::eFSNT_Directory != ::pilo::core::fs::fs_util::calculate_type(dir_path_buffer))
                {
                    printf("XXXXX  dir (%s) not exist\n", dir_path_buffer);
                }
                WIN32_FIND_DATA wfd;
                HANDLE hFind = FindFirstFile(dir_path_buffer, &wfd);
                if (INVALID_HANDLE_VALUE != hFind && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                {
                    ;
                }
                else
                {
                    printf("XXXXX   dir (%s) not exist\n", dir_path_buffer);
                }

                ::pilo::os_file_descriptor_t fd = MC_INVALID_FILE_DESCRIPTOR;
                ::pilo::core::fs::fs_util::open_file(fd, path, eMode, ::pilo::core::fs::eDRWM_ReadWrite, 0);
                if (fd == MC_INVALID_FILE_DESCRIPTOR)
                {
                    if (::pilo::core::fs::fs_util::eFSNT_Directory != ::pilo::core::fs::fs_util::calculate_type(dir_path_buffer))
                    {
                        printf("XXXXX creating  dir (%s) Failed\n", dir_path_buffer);
                    }
                    else
                    {
                        printf("XXXXX creating  dir (%s) OK\n", dir_path_buffer);
                    }

                    WIN32_FIND_DATA wfd;
                    HANDLE hFind = FindFirstFile(dir_path_buffer, &wfd);
                    if (INVALID_HANDLE_VALUE != hFind && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                    {
                        printf("XXXXX creating  dir (%s) OK\n", dir_path_buffer);
                    }
                    else
                    {
                        printf("XXXXX creating  dir (%s) Failed\n", dir_path_buffer);
                    }

    
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
                M_UNUSED(flags);
                fildes = MC_INVALID_FILE_DESCRIPTOR;
                
                if (path == nullptr)
                {
                    return ::pilo::EC_NULL_PARAM;
                }

#               ifdef  WINDOWS
                DWORD dwAccesss = (DWORD) drwm;
                if (flags & MC_IO_DEV_OP_FLAG_APPEND)
                {
                    dwAccesss = FILE_APPEND_DATA;
                }
                fildes = ::CreateFile(path, dwAccesss, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, dam, 0, 0);

#               else
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

            ::pilo::error_number_t fs_util::_compact_path_once(char * pBuffer, size_t len)
            {
                if (pBuffer == nullptr || len <=0)
                {
                    return ::pilo::EC_NULL_PARAM;
                }

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

            ::pilo::i32_t fs_node_delete_visitor::visit(const char* path, const fs_find_data* data)
            {
                if (data == nullptr) return ::pilo::EC_NULL_PARAM;

                M_UNUSED(data);
                if (data->type() == ::pilo::core::fs::fs_util::eFSNT_Directory)
                {           
                    ;
                }
                else
                {
                    ::pilo::core::fs::fs_util::delete_regular_file(path);
                }

                return  ::pilo::EC_OK;
            }

            ::pilo::i32_t fs_node_delete_visitor::post_dir_visit(const char* path)
            {
                return ::pilo::core::fs::fs_util::delete_empty_directory(path);        

            }

        }
    }
}