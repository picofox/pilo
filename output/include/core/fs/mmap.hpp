#pragma once
#include "core/fs/file.hpp"
#include "core/container/fixed_array.hpp"

namespace pilo
{
    namespace core
    {
        namespace fs
        {
#define MB_MMAP_FLAG_ANONYMOUS  (1<<0)

            struct mmap_parameter
            {
                mmap_parameter()
                {
                    reset();
                    
                }

                void reset()
                {
                    m_address = MC_INVALID_PTR;
                    m_offset = 0;
                    m_length = 0;
                    m_priv = PiloGenericPrivillegeEnumeration::eGPE_None;
                }

                bool unused() const
                {
                    if (m_address == nullptr)
                        return true;
                    else
                        return false;
                }

                void*								m_address;
                size_t                              m_offset;
                size_t                              m_length;
                PiloGenericPrivillegeEnumeration    m_priv;
				
            };

            template<size_t _MAX_COUNT, typename _LOCK_TYPE = ::pilo::core::threading::dummy_read_write_lock>
            class mmap
            {
            public:
                typedef _LOCK_TYPE lock_type;
            public:
                mmap()
                {
					_m_global_priv = PiloGenericPrivillegeEnumeration::eGPE_None;
					_m_flags = 0;
                    _m_file_size = MC_INVALID_SIZE;
                    _m_mapped_size = MC_INVALID_SIZE;
                    _m_mapped_ptr = nullptr; 
					_m_map_parameters.clear();
#ifdef WINDOWS
                    _m_win32_map_handle = NULL;
#endif
                }

                ~mmap()
                {
                    _finalize_nolock();
                }

				size_t max_count()
				{
					return _MAX_COUNT;
				}

                size_t count();
                {
                    ::pilo::core::threading::nonrecursive_mutex<lock_type> locker(_m_lock);
                    return _m_map_parameters.size();
                }

				::pilo::error_number_t initialize(const char* path, DeviceAccessModeEnumeration eMode, PiloGenericPrivillegeEnumeration ePriv)
				{
					::pilo::core::threading::nonrecursive_mutex<lock_type> locker(_m_lock);
                    ::pilo::error_number_t ret = _finalize_nolock();
                    if (ret != ::pilo::EC_OK)
                    {
                        return ret;
                    }           

					return _initialize_nolock(path, eMode, ePriv);
				}

                ::pilo::error_number_t finalize_nolock() 
                {
                    ::pilo::core::threading::nonrecursive_mutex<lock_type> locker(_m_lock);
                    return _finalize_nolock();
                }

                ::pilo::error_number_t flush(size_t index)
                {
                    ::pilo::core::threading::nonrecursive_mutex<lock_type> locker(_m_lock);
                    if (index >= _m_map_parameters.size())
                    {
                        return ::pilo::EC_OUT_OF_RANGE;
                    }

                    if (_m_map_parameters.at(index).m_address == nullptr)
                    {
                        return ::pilo::EC_NULL_PTR;
                    }

                    return _flush_nolock(index);
                }

                ::pilo::error_number_t unmap(size_t index)
                {
                    ::pilo::core::threading::nonrecursive_mutex<lock_type> locker(_m_lock);
                    if (index >= _m_map_parameters.size())
                    {
                        return ::pilo::EC_OUT_OF_RANGE;
                    }

                    if (_m_map_parameters.at(index).m_address == nullptr)
                    {
                        return ::pilo::EC_NULL_PTR;
                    }

                    return _unmap_nolock(index);
                }

                ::pilo::error_number_t mmap(OUT size_t& ref_index, size_t index, PiloGenericPrivillegeEnumeration e_priv, size_t offset, size_t length, void* desired_start_address)
                {
                    ::pilo::core::threading::nonrecursive_mutex<lock_type> locker(_m_lock);
                    if (_m_win32_map_handle == NULL)
                    {
                        return ::pilo::EC_UNINITIALIZED;
                    }

                    return _mmap_nolock(ref_index, index, e_priv, offset, length, desired_start_address);
                }


                size_t mapped_ptr(size_t index)
                {
                    ::pilo::core::threading::nonrecursive_mutex<lock_type> locker(_m_lock);
                    if (index >= _m_map_parameters.size())
                    {
                        return ::pilo::EC_OUT_OF_RANGE;
                    }

                    return _m_map_parameters.at(index).m_address;
                }



            protected:
				::pilo::error_number_t _finalize_nolock()
				{
                    ::pilo::error_number_t ret = ::pilo::EC_OK;

					for (size_t i = 0; i < _m_map_parameters.size(); i++)
					{
						if (_m_map_parameters.at(i).m_address != nullptr)
						{
#ifdef WINDOWS
                            _unmap_nolock(i);
#else

#endif
						}
					}

#ifdef WINDOWS
                    if (_m_win32_map_handle != NULL)
                    {
                        ::CloseHandle(_m_win32_map_handle);
                        _m_win32_map_handle = NULL;
                    }
#endif
                    _m_file.finalize();

                    _m_global_priv = PiloGenericPrivillegeEnumeration::eGPE_None;
                    _m_flags = 0;
                    _m_file_size = MC_INVALID_SIZE;
                    _m_mapped_size = MC_INVALID_SIZE;
                    _m_mapped_ptr = nullptr;
                    _m_map_parameters.clear();

                    return ::pilo::EC_OK;

				}

                ::pilo::error_number_t _initialize_nolock(const char* path, DeviceAccessModeEnumeration eMode, PiloGenericPrivillegeEnumeration ePriv)
                {
                    if (path == nullptr || *path == 0)
                    {
                        _m_global_priv = ePriv;
                        pilo_set_flag_bit_by_value(_m_flags, MB_MMAP_FLAG_ANONYMOUS);
                        return ::pilo::EC_OK;
                    }

                    ::pilo::error_number_t ret = _m_file.initialize(path, 0, nullptr);
                    if (ret != ::pilo::EC_OK)
                    {
                        return ::pilo::EC_INITIALIZE_FAILED;
                    }

                    if (ePriv == PiloGenericPrivillegeEnumeration::eGPE_None)
                    {
                        return  ::pilo::EC_NONSENSE_OPERATION;
                    }

                    DeviceRWModeEnumeration e_file_acc_prev = eDRWM_ReadWrite;
					_m_global_priv.m_priv = ePriv;

                    if (pilo_test_flag_bit_by_value(ePriv, eGPE_Write))
                    {
                        e_file_acc_prev = eDRWM_ReadWrite;
                    }
                    else
                    {
                        e_file_acc_prev = eDRWM_Read;
                    } 

                    if (_m_file.open(eMode, e_file_acc_prev, 0))
                    {
                        return ::pilo::EC_OPEN_FILE_FAILED;
                    }

#ifdef WINDOWS
                    DWORD hi = 0;
                    DWORD lo = 0;
                    hi = M_HI32BIT(_m_file_size);
                    lo = M_LO32BIT(_m_file_size);
                    _m_win32_map_handle = ::CreateFileMapping(_m_file.file_descriptor(), NULL, flag, hi, lo, NULL);
                    if (NULL == _m_win32_map_handle)
                    {
                        return ::pilo::EC_CREATE_FILE_MAP_ERROR;
                    }
#else


#endif

                    return ret;
                }

                ::pilo::error_number_t _flush_nolock(size_t index)
                {

#ifdef WINDOWS
                    BOOL ret = FlushViewOfFile(_m_map_parameters.at(index).m_address, _m_map_parameters.at(index).m_length);
                    if (ret) return ::pilo::EC_OK;
                    else return ::pilo::EC_SYNC_FILE_FAILED;
#else

#endif

                }

                ::pilo::error_number_t _mmap_nolock(OUT size_t& ref_index, size_t index, PiloGenericPrivillegeEnumeration ePriv, size_t offset, size_t length, void* desired_start_address)
                {
                    size_t slot_index = _get_available_slot_index(index);
                    if (slot_index == MC_INVALID_SIZE)
                    {
                        return ::pilo::EC_OBJECT_NOT_FOUND;
                    }

#ifdef WINDOWS
                    DWORD priv = 0;
                    if (pilo_test_flag_bit_by_value(ePriv, PiloGenericPrivillegeEnumeration::eGPE_Exec))
                    {
                        priv |= FILE_MAP_EXECUTE;
                    }
                    if (pilo_test_flag_bit_by_value(ePriv, PiloGenericPrivillegeEnumeration::eGPE_Read))
                    {
                        priv |= FILE_MAP_READ;
                    }
                    if (pilo_test_flag_bit_by_value(ePriv, PiloGenericPrivillegeEnumeration::eGPE_Write))
                    {
                        priv |= FILE_MAP_WRITE;
                    }

                    DWORD hi = 0;
                    DWORD lo = 0;
                    hi = M_HI32BIT(offset);
                    lo = M_LO32BIT(offset);
                    LPVOID pret = MapViewOfFileEx(_m_win32_map_handle, priv, hi, lo, length, desired_start_address);
                    if (pret == NULL)
                    {
                        return ::pilo::EC_MAP_FAILED;
                    }


                    _m_map_parameters[slot_index].m_address = pret;
                    _m_map_parameters[slot_index].m_priv = ePriv;
                    _m_map_parameters[slot_index].m_offset = offset;
                    _m_map_parameters[slot_index].m_length = length;

                    ref_index = slot_index;

                    
#else


#endif
                    return ::pilo::EC_OK;
                }

                ::pilo::error_number_t _unmap_nolock(size_t index)
                {
                    _flush_nolock(index);
#ifdef WINDOWS

                    BOOL ret = UnmapViewOfFileEx(_m_map_parameters.at(index).m_address, _m_map_parameters.at(index).m_length);

#else

#endif

                    if ((bool)ret)
                    {
                        _m_map_parameters.at(index).reset();
                        return ::pilo::EC_OK;
                    }
                    else
                        return ::pilo::EC_UNMAP_FAILED;

                }

            private:
                size_t _find_free_slot()
                {
                    for (int i = 0; i < _m_map_parameters.size(); i++)
                    {
                        if (_m_map_parameters.at(i).m_address == nullptr)
                        {
                            return i;
                        }
                    }
                    return MC_INVALID_SIZE;
                }

                size_t _get_available_slot_index(size_t index)
                {
                    if (_m_map_parameters.full()) //only can get exist to check on full
                    {
                        if (index < _m_map_parameters.size()) 
                        {
                            if (_m_map_parameters.at(index).m_address == nullptr) //got a free one
                            {
                                return index; //it's available
                            }
                            else
                            {
                                size_t ret_index = _find_free_slot(); //find other free one
                                if (ret_index == MC_INVALID_SIZE)
                                {
                                    return MC_INVALID_SIZE; //can not go any further
                                }
                                else
                                {
                                    return ret_index; //other free slot is found
                                }
                                
                            }
                        }
                        else
                        {//if index is invalid we can't append neo item
                            return MC_INVALID_SIZE; //can not go any further
                        }
                    }

                    if (index >= _m_map_parameters.size())
                    {
                        index = MC_INVALID_SIZE;
                    }

                    if (index == MC_INVALID_SIZE)
                    {
                        mmap_parameter item_data; //use want append one neo slot
                        item_data.reset();
                        _m_map_parameters.push_back(item_data);
                        return (_m_map_parameters.size()-1);
                    }
                    else
                    {//user would like to use exist one
                        if (_m_map_parameters.at(index).m_address == nullptr)
                        {
                            return index;
                        }
                        else
                        {
                            mmap_parameter item_data;
                            item_data.reset();
                            _m_map_parameters.push_back(item_data);
                            return (_m_map_parameters.size() - 1);
                        }
                    }
                }

            protected:
				PiloGenericPrivillegeEnumeration    _m_global_priv;
				::pilo::i32_t						_m_flags;
                size_t                              _m_file_size;
                size_t                              _m_mapped_size;
                void *                              _m_mapped_ptr;
				
                
                lock_type                           _m_lock;
                ::pilo::core::fs::file<MC_PATH_MAX, ::pilo::core::threading::dummy_read_write_lock> _m_file;
				::pilo::core::container::fixed_array<mmap_parameter, _MAX_COUNT> _m_map_parameters;

#ifdef WINDOWS
                HANDLE                              _m_win32_map_handle;
#endif



            private:
                M_DISABLE_COPY
            };

        }
    }
}