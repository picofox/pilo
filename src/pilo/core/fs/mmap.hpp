#pragma once
#include "core/fs/file.hpp"
#include "core/container/fixed_array.hpp"
#include "core/info/system_info.hpp"
#include "core/threading/rw_mutex_r_locker.hpp"
#include "core/threading/rw_mutex_w_locker.hpp"

namespace pilo
{
	namespace core
	{
		namespace fs
		{
			struct mmap_parameter
			{
				mmap_parameter()
				{
					reset();
				}

				void reset()
				{
					m_address = nullptr;
					m_address_required = nullptr;
					m_offset = 0;
					m_offset_required = 0;
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
				void*								m_address_required;
				size_t                              m_offset;
				size_t                              m_offset_required;
				size_t								m_length;
				PiloGenericPrivillegeEnumeration    m_priv;

			};
		}
	}
}

DECLARE_SIMPLE_TYPE(::pilo::core::fs::mmap_parameter);


namespace pilo
{
    namespace core
    {
        namespace fs
        {

            template<size_t _MAX_COUNT, typename _LOCK_TYPE = ::pilo::core::threading::dummy_read_write_lock>
            class mmap
            {
            public:
                typedef _LOCK_TYPE lock_type;
            public:
                mmap()
                {
					_m_global_priv = PiloGenericPrivillegeEnumeration::eGPE_None;
                    _m_file_size_init = MC_INVALID_SIZE;
					_m_map_parameters.clear();
#ifdef WINDOWS
                    _m_win32_map_handle = NULL;
#endif
                }

                ~mmap()
                {
                    _finalize_nolock();
                }

				size_t capacity()
				{
					return _MAX_COUNT;
				}

                size_t count()
                {
					::pilo::core::threading::rw_mutex_r_locker<lock_type> locker(_m_lock);
                    return _m_map_parameters.size();
                }

				::pilo::error_number_t initialize(
					const char* path, 
					size_t length,
					DeviceAccessModeEnumeration eMode, 
					PiloGenericPrivillegeEnumeration ePriv)
				{
                    ::pilo::core::threading::rw_mutex_w_locker<lock_type> locker(_m_lock);
                    ::pilo::error_number_t ret = _finalize_nolock();
                    if (ret != ::pilo::EC_OK)
                    {
                        return ret;
                    }           

					return _initialize_nolock(path, length, eMode, ePriv);
				}

                ::pilo::error_number_t finalize() 
                {
                    ::pilo::core::threading::rw_mutex_w_locker<lock_type> locker(_m_lock);
                    return _finalize_nolock();
                }

                ::pilo::error_number_t flush(size_t index)
                {
                    ::pilo::core::threading::rw_mutex_w_locker<lock_type> locker(_m_lock);
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
                    ::pilo::core::threading::rw_mutex_w_locker<lock_type> locker(_m_lock);
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

                ::pilo::error_number_t map(OUT size_t& ref_index, size_t index, PiloGenericPrivillegeEnumeration e_priv, size_t offset, size_t length, void* desired_start_address)
                {
                    ::pilo::core::threading::rw_mutex_w_locker<lock_type> locker(_m_lock);
                    if (_m_win32_map_handle == NULL)
                    {
                        return ::pilo::EC_UNINITIALIZED;
                    }

                    return _map_nolock(ref_index, index, e_priv, offset, length, desired_start_address);
                }


                void* mapped_ptr(size_t index)
                {
                    ::pilo::core::threading::rw_mutex_w_locker<lock_type> locker(_m_lock);
                    if (index >= _m_map_parameters.size())
                    {
                        return nullptr;
                    }

                    return _m_map_parameters.at(index).m_address;
                }

                ::pilo::error_number_t get_mapped_info(mmap_parameter* pinfo, size_t index)
                {
                    ::pilo::core::threading::rw_mutex_r_locker<lock_type> locker(_m_lock);
                    if (index >= _m_map_parameters.size())
                    {
                        return ::pilo::EC_OUT_OF_RANGE;
                    }

                    if (pinfo)
                    {
                        *pinfo = _m_map_parameters.at(index);
                    }

                    return ::pilo::EC_OK;
                }



            protected:
				::pilo::error_number_t _finalize_nolock()
				{
					for (size_t i = 0; i < _m_map_parameters.size(); i++)
					{
						if (_m_map_parameters.at(i).m_address != nullptr)
						{
                            _unmap_nolock(i);
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
                    _m_file_size_init = MC_INVALID_SIZE;
                    _m_map_parameters.clear();

                    return ::pilo::EC_OK;

				}				

				::pilo::error_number_t _initialize_nolock(
					const char* path,
					size_t length,
					DeviceAccessModeEnumeration eMode, 
					PiloGenericPrivillegeEnumeration ePriv)
                {
                    if (path == nullptr || *path == 0)
                    {
                        return ::pilo::EC_NULL_PARAM;
                    }

                    os_file_descriptor_t temp_fd = MC_INVALID_FILE_DESCRIPTOR;
                    if (ePriv == PiloGenericPrivillegeEnumeration::eGPE_None)
                    {
                        return  ::pilo::EC_NONSENSE_OPERATION;
                    }

                    DeviceRWModeEnumeration e_file_acc_prev = eDRWM_ReadWrite;
                    _m_global_priv = ePriv;

#ifdef WINDOWS
                    DWORD win32_protect = PAGE_READONLY;
#endif
                    if (pilo_test_flag_bit_by_value(ePriv, eGPE_Write))
                    {

                        e_file_acc_prev = eDRWM_ReadWrite;
                        win32_protect = PAGE_READWRITE;
                    }

                    ::pilo::error_number_t ret = _m_file.initialize(path, 0, nullptr);
                    if (ret != ::pilo::EC_OK)
                    {
                        return ::pilo::EC_INITIALIZE_FAILED;
                    }

                    if (_m_file.open(eMode, e_file_acc_prev, 0))
                    {
                        return ::pilo::EC_OPEN_FILE_FAILED;
                    }

                    temp_fd = _m_file.file_descriptor();

                    if (temp_fd == MC_INVALID_FILE_DESCRIPTOR)
                    {
                        return ::pilo::EC_OPEN_FILE_FAILED;
                    }

                    if (length <= 0)
                    {
                        _m_file_size_init = _m_file.get_file_size();
                    }
                    else
                    {
                        _m_file_size_init = length;
                    }

                    if (_m_file_size_init <= 0)
                    {
                        return ::pilo::EC_INVALID_PARAM;
                    }
				

#ifdef WINDOWS
					DWORD hi = 0;
					DWORD lo = 0;
					if (length >= 0)
					{
#ifdef __x86_64__
                        hi = M_HI32BIT(length);
#else
                        hi = 0;
#endif // __x86_64__

						lo = M_LO32BIT(length);
					}                    

                    _m_win32_map_handle = ::CreateFileMapping(temp_fd, NULL, win32_protect, hi, lo, NULL);
					if (NULL == _m_win32_map_handle)
					{
						return ::pilo::EC_CREATE_FILE_MAP_ERROR;
					}
#else
                    size_t file_size = _m_file_size_init;
					if (length == 0)
					{
						length = file_size;
					}
					else
					{
						if (length < file_size)
						{
							if (_m_file.seek(length, eDSW_Begin, nullptr) != ::pilo::EC_OK)
							{
								return ::pilo::EC_SEEK_FILE_ERROR;
							}
						}
					}
#endif	

                    return ::pilo::EC_OK;
                }

                ::pilo::error_number_t _flush_nolock(size_t index)
                {

#ifdef WINDOWS
                    BOOL ret = FlushViewOfFile(_m_map_parameters.at(index).m_address, _m_map_parameters.at(index).m_length);
                    if (ret) return ::pilo::EC_OK;
                    else return ::pilo::EC_SYNC_FILE_FAILED;
#else
					int ret = msync(_m_map_parameters.at(index).m_address, _m_map_parameters.at(index).m_length);
					if (ret == 0) return ::pilo::EC_OK;
					else return ::pilo::EC_SYNC_FILE_FAILED;
#endif

                }

                ::pilo::error_number_t _map_nolock(OUT size_t& ref_index, size_t index, PiloGenericPrivillegeEnumeration ePriv, size_t offset, size_t length, void* desired_start_address)
                {
                    size_t slot_index = _get_available_slot_index(index);
                    if (slot_index == MC_INVALID_SIZE)
                    {
                        return ::pilo::EC_OBJECT_NOT_FOUND;
                    }

					size_t mmap_offset = M_ALIGN_SIZE(offset, ::pilo::core::info::system_info::mmap_granuity());
					void* mmap_addr = nullptr;
                    if (length == 0)
                    {
                        length = _m_file_size_init;
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
#ifdef __x86_64__
                    hi = M_HI32BIT(length);
#else
                    hi = 0;
#endif // __x86_64__

					lo = M_LO32BIT(mmap_offset);                    

					mmap_addr = MapViewOfFileEx(_m_win32_map_handle, priv, hi, lo, length, desired_start_address);
								                        
#else
					int priv = PROT_NONE;
					if (pilo_test_flag_bit_by_value(ePriv, PiloGenericPrivillegeEnumeration::eGPE_Exec))
					{
						priv |= PROT_EXEC;
					}
					if (pilo_test_flag_bit_by_value(ePriv, PiloGenericPrivillegeEnumeration::eGPE_Read))
					{
						priv |= PROT_READ;
					}
					if (pilo_test_flag_bit_by_value(ePriv, PiloGenericPrivillegeEnumeration::eGPE_Write))
					{
						priv |= PROT_WRITE;
					}

                    mmap_addr = mmap(desired_start_address, length, priv, MAP_SHARED, _m_file.file_descriptor(), mmap_offset);
				

#endif
					if (mmap_addr == MC_MAP_FAILED_INDICATOR)
					{
						return ::pilo::EC_MAP_FAILED;
					}

					ref_index = slot_index;
					_m_map_parameters[slot_index].m_address = mmap_addr;
					_m_map_parameters[slot_index].m_address_required = desired_start_address;
					_m_map_parameters[slot_index].m_offset = mmap_offset;
					_m_map_parameters[slot_index].m_offset_required = offset;
					_m_map_parameters[slot_index].m_length = length;
					_m_map_parameters[slot_index].m_priv = ePriv;
					
					return ::pilo::EC_OK;
                }

                ::pilo::error_number_t _unmap_nolock(size_t index)
                {
                    _flush_nolock(index);
#ifdef WINDOWS
					BOOL unmap_ret = UnmapViewOfFile(_m_map_parameters.at(index).m_address);
					if (!unmap_ret) return ::pilo::EC_UNMAP_FAILED;
#else
					int unmap_ret = munmap(_m_map_parameters.at(index).m_address_m_map_parameters.at(index).m_length);
					if (unmap_ret != 0) return ::pilo::EC_UNMAP_FAILED;
#endif
					_m_map_parameters.at(index).reset();

					return ::pilo::EC_OK;
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
                size_t                              _m_file_size_init;               
                lock_type                           _m_lock;
                ::pilo::core::fs::file<MC_PATH_MAX, ::pilo::core::threading::dummy_read_write_lock> _m_file;
				::pilo::core::container::fixed_array<mmap_parameter, _MAX_COUNT> _m_map_parameters;

#ifdef WINDOWS
                HANDLE                              _m_win32_map_handle;
#endif



            private:
                M_DISABLE_COPY(mmap);
            };

        }
    }
}