#pragma once

#include "../../pilo.hpp"
#include "../io/formatted_io.hpp"
#include "../string/string_operation.hpp"
#include "../datetime/timestamp.hpp"
#include "../memory/serializable_interface.hpp"
#include "../memory/serializable_header_interface.hpp"
#include "./inner_sample_data.hpp"
#include "../memory/default_portable_object_pool.hpp"

#define _PILO_TEST_DC_STR	"this is dc str."
#define _PILO_TEST_DC_STR_SIZE	sizeof(_PILO_TEST_DC_STR)
#define _PILO_TEST_DC_STR_LEN	(_PILO_TEST_DC_STR_SIZE - 1)


namespace pilo
{
	namespace core
	{
		namespace testing
		{
			class large_sample_object : public ::pilo::core::memory::serializable_interface
				,public ::pilo::core::memory::default_portable_object_pool<large_sample_object>
			{
			public:			

				static large_sample_object* deserialise(::pilo::core::memory::serializable_header_interface* header, ::pilo::core::memory::byte_buffer_interface* byte_buffer);

			public:
				large_sample_object(bool is_server, ::pilo::u32_t thread_no)
					: _is_server(is_server), _thread_no(thread_no), _id(0), _i8v(0), _u8v(0),_i16v(0),_u16v(0), _i32v(0), _i64v(0), _f32v(0), _f64v(0)
					, _dcstr(nullptr), _fcstr{ 0 }, _bin_data(nullptr), _bin_data_fiexed{ 0 }, _db_arr(nullptr), _fb_arr{ 0 }, _i8_d_arr(nullptr), _i8_f_arr{0}
					, _u8_d_arr(nullptr), _u8_f_arr{ 0 }, _f32_d_arr(nullptr), _f32_f_arr{ 0 }, _f64_d_arr(nullptr), _f64_f_arr{0}
					, _i64_d_arr(nullptr), _i64_f_arr{0}, _u64_d_arr(nullptr), _u64_f_arr{ 0 }, _i32_d_arr(nullptr), _i32_f_arr{ 0 }, _u32_d_arr(nullptr), _u32_f_arr{ 0 }
					, _i16_d_arr(nullptr), _i16_f_arr{ 0 }, _u16_d_arr(nullptr), _u16_f_arr{ 0 }, _inner_d(nullptr), _inner_null(nullptr), _inner_d_arr{nullptr}, _inner_d2_arr(nullptr)
				{

				}

				large_sample_object()
					: _is_server(false), _thread_no(0), _id(0), _i8v(0), _u8v(0), _i16v(0), _u16v(0), _i32v(0), _i64v(0), _f32v(0), _f64v(0)
					, _dcstr(nullptr), _fcstr{ 0 }, _bin_data(nullptr), _bin_data_fiexed{ 0 }, _db_arr(nullptr), _fb_arr{ 0 }, _i8_d_arr(nullptr), _i8_f_arr{ 0 }
					, _u8_d_arr(nullptr), _u8_f_arr{ 0 }, _f32_d_arr(nullptr), _f32_f_arr{ 0 }, _f64_d_arr(nullptr), _f64_f_arr{ 0 }
					, _i64_d_arr(nullptr), _i64_f_arr{ 0 }, _u64_d_arr(nullptr), _u64_f_arr{ 0 }, _i32_d_arr(nullptr), _i32_f_arr{ 0 }, _u32_d_arr(nullptr), _u32_f_arr{ 0 }
					, _i16_d_arr(nullptr), _i16_f_arr{ 0 }, _u16_d_arr(nullptr), _u16_f_arr{ 0 }, _inner_d(nullptr), _inner_null(nullptr), _inner_d_arr{ nullptr }, _inner_d2_arr(nullptr)
				{
	
				}

				~large_sample_object()
				{
					_reset_ptrs();
				}
			public:
				void set(bool is_server, ::pilo::u32_t thread_no)
				{
					_is_server = is_server;
					_thread_no = thread_no;
					set();
				}

				void set()
				{
					static ::pilo::u64_t s_id;
					_id = s_id++;
					_i8v = (::pilo::i8_t)_id % 256;
					_u8v = (::pilo::u8_t)_id % 256;
					_i16v = (::pilo::i16_t)_id % 65536;
					_u16v = (::pilo::u16_t)_id % 65536;
					_i32v = (::pilo::i32_t)_id % PILO_INT32_MAX;
					_i64v = (::pilo::i64_t)_id;
					_f32v = ((::pilo::f32_t)_i32v / 3.0f);
					_f64v = ((::pilo::f64_t)_i64v / 3.0f);

					if (_dcstr == nullptr)
						_dcstr = ::pilo::core::string::duplicate(_PILO_TEST_DC_STR, -1);
					::pilo::core::string::copyz(_fcstr, sizeof(_fcstr), _PILO_TEST_DC_STR);

					if (_bin_data == nullptr)
						_bin_data = (char*) PMF_HEAP_MALLOC(37);
					if (_bin_data == nullptr)
						abort();
					memset(_bin_data, 'x', 37);
					::pilo::core::string::n_copy(_bin_data_fiexed, sizeof(_bin_data_fiexed), _bin_data, 37);

					if (_db_arr == nullptr)
						_db_arr = (bool*)PMF_HEAP_CALLOC(11 , sizeof(bool));
					if (_db_arr == nullptr)
						abort();
					_db_arr[0] = true;
					_db_arr[10] = true;
					memset(_fb_arr, 0x00, sizeof(_fb_arr));
					_fb_arr[0] = true;

					_fb_arr[10] = true;
					if (_i8_d_arr == nullptr)
						_i8_d_arr = (::pilo::i8_t*) PMF_HEAP_CALLOC(1, 15);
					
					if (_i8_d_arr == nullptr)
						abort();
					_i8_d_arr[0] = '@';
					_i8_d_arr[14] = '$';
					memcpy(_i8_f_arr,  _i8_d_arr, 15);

					if (_u8_d_arr == nullptr)
						_u8_d_arr = (::pilo::u8_t*)PMF_HEAP_CALLOC(1, 11);
					if (_u8_d_arr == nullptr)
						abort();
					_u8_d_arr[0] = '@';
					_u8_d_arr[10] = '$';
					memcpy(_u8_f_arr, _u8_d_arr, 11);

					if (_f32_d_arr == nullptr)
						_f32_d_arr = (::pilo::f32_t *) (::pilo::u8_t*)PMF_HEAP_CALLOC(1, 11*sizeof(::pilo::f32_t));
					if (_f32_d_arr == nullptr)
						abort();
					_f32_d_arr[0] = 0.2f;
					_f32_d_arr[10] = 0.02f;
					memcpy(_f32_f_arr, _f32_d_arr, 11*sizeof(::pilo::f32_t));

					if (_f64_d_arr == nullptr)
						_f64_d_arr = (::pilo::f64_t*)PMF_HEAP_CALLOC(1, 10 * sizeof(::pilo::f64_t));
					if (_f64_d_arr == nullptr)
						abort();
					_f64_d_arr[0] = 1.0;
					_f64_d_arr[9] = 2.0;
					memcpy(_f64_f_arr, _f64_d_arr, 10 * sizeof(::pilo::f64_t));

					if (_i64_d_arr == nullptr)
						_i64_d_arr = (::pilo::i64_t*)PMF_HEAP_CALLOC(1, 10 * sizeof(::pilo::i64_t));
					if (_i64_d_arr == nullptr)
						abort();
					_i64_d_arr[0] = -1;
					_i64_d_arr[9] = PILO_INT64_MAX;
					memcpy(_i64_f_arr, _i64_d_arr, 10 * sizeof(::pilo::i64_t));

					if (_u64_d_arr == nullptr)
						_u64_d_arr = (::pilo::u64_t*)PMF_HEAP_CALLOC(1, 10 * sizeof(::pilo::u64_t));
					if (_u64_d_arr == nullptr)
						abort();
					_u64_d_arr[0] = 99;
					_u64_d_arr[9] = PILO_UINT64_MAX;
					memcpy(_u64_f_arr, _u64_d_arr, 10 * sizeof(::pilo::u64_t));


					if (_i32_d_arr == nullptr)
						_i32_d_arr = (::pilo::i32_t*)PMF_HEAP_CALLOC(1, 10 * sizeof(::pilo::i32_t));
					if (_i32_d_arr == nullptr)
						abort();
					_i32_d_arr[0] = -1;
					_i32_d_arr[9] = PILO_INT32_MAX;
					memcpy(_i32_f_arr, _i32_d_arr, 10 * sizeof(::pilo::i32_t));

					if (_u32_d_arr == nullptr)
						_u32_d_arr = (::pilo::u32_t*)PMF_HEAP_CALLOC(1, 10 * sizeof(::pilo::u32_t));
					if (_u32_d_arr == nullptr)
						abort();
					_u32_d_arr[0] = 99;
					_u32_d_arr[9] = PILO_UINT32_MAX;
					memcpy(_u32_f_arr, _u32_d_arr, 10 * sizeof(::pilo::u32_t));

					if (_i16_d_arr == nullptr)
						_i16_d_arr = (::pilo::i16_t*)PMF_HEAP_CALLOC(1, 10 * sizeof(::pilo::i16_t));
					if (_i16_d_arr == nullptr)
						abort();
					_i16_d_arr[0] = -1;
					_i16_d_arr[9] = PILO_INT16_MAX;
					memcpy(_i16_f_arr, _i16_d_arr, 10 * sizeof(::pilo::i16_t));

					if (_u16_d_arr == nullptr)
						_u16_d_arr = (::pilo::u16_t*)PMF_HEAP_CALLOC(1, 10 * sizeof(::pilo::u16_t));
					if (_u16_d_arr == nullptr)
						abort();
					_u16_d_arr[0] = 11;
					_u16_d_arr[9] = PILO_UINT16_MAX;
					memcpy(_u16_f_arr, _u16_d_arr, 10 * sizeof(::pilo::u16_t));
					_inner_f.set(this->_i32v);

					if (_inner_d == nullptr)
						_inner_d = ::pilo::core::testing::inner_sample_data::allocate();

					_inner_null = nullptr;

					_inner_d->set(this->_i32v);

					for (int k = 0; k < 7; k++)
					{
						_inner_f_arr[k].set(this->_i32v + k);
					}

					for (int k = 0; k < 7; k++)
					{
						if (k != 3)
						{
							if (_inner_d_arr[k] == nullptr)
							{
								_inner_d_arr[k] = ::pilo::core::testing::inner_sample_data::allocate();
							}							
							_inner_d_arr[k]->set(this->_i32v + k);
						}							
						else
							_inner_d_arr[k] = nullptr;

					}


					if (_inner_d2_arr == nullptr)
					{
						_inner_d2_arr = (inner_sample_data**)PMF_HEAP_CALLOC(7, sizeof(inner_sample_data*));	
						if (_inner_d2_arr == nullptr)
						{
							abort();
						}
					}					
					for (int k = 0; k < 7; k++)
					{
						if (k != 3)
						{
							if (_inner_d2_arr[k] == nullptr)
							{
								_inner_d2_arr[k] = ::pilo::core::testing::inner_sample_data::allocate();
							}
							_inner_d2_arr[k]->set(this->_i32v + k);
						}
						else
							_inner_d2_arr[k] = nullptr;

					}

				}

				::pilo::i64_t byte_size() const
				{
					return 1639;
				}

				bool fast_validate() const
				{
					if (_i32v != (::pilo::i32_t)_id % PILO_INT32_MAX)
						return false;
					if (_db_arr == nullptr || _db_arr[0] != true || _db_arr[10] != true)
						return false;
					return true;
				}

				bool validate() const
				{
					return validate(_id);
				}

				bool validate(::pilo::u64_t id) const
				{	
					if (id != _id)
						return false;
					if (_i8v != (::pilo::i8_t)_id % 256)
						return false;
					if (_u8v != (::pilo::u8_t)_id % 256)
						return false;
					if (_i16v != (::pilo::i16_t)_id % 65536)
						return false;
					if (_u16v != (::pilo::u16_t)_id % 65536)
						return false;
					if (_i32v != (::pilo::i32_t)_id % PILO_INT32_MAX)
						return false;
					if (_i64v != (::pilo::i64_t)_id)
						return false;
					if (_f32v != ((::pilo::f32_t)_i32v / 3.0f))
						return false;
					if (_f64v != ((::pilo::f64_t)_i64v / 3.0f))
						return false;
					if (::pilo::core::string::strict_compare(_PILO_TEST_DC_STR, 0, _dcstr, 0, -1) != 0)
						return false;
					if (::pilo::core::string::strict_compare(_PILO_TEST_DC_STR, 0, _fcstr, 0, -1) != 0)
						return false;
					if (_bin_data != nullptr && (_bin_data[0] != 'x' || _bin_data[36] != 'x'))
						return false;
					if (_bin_data_fiexed[0] != 'x' || _bin_data_fiexed[36] != 'x')
						return false;
					if (_i8_d_arr != nullptr && (_i8_d_arr[0] != '@' || _i8_d_arr[14] != '$'))
						return false;
					if (_i8_f_arr[0] != '@' || _i8_f_arr[14] != '$')
						return false;
					if (_u8_f_arr[0] != '@' || _u8_f_arr[10] != '$')
						return false;
					if (_u8_f_arr[0] != '@' || _u8_f_arr[10] != '$')
						return false;
					if (_f32_d_arr[0] != 0.2f || _f32_d_arr[10] != 0.02f)
						return false;
					if (_f32_f_arr[0] != 0.2f || _f32_f_arr[10] != 0.02f)
						return false;
					if (_f64_d_arr[0] != 1.0 || _f64_d_arr[9] != 2.0)
						return false;
					if (_f64_f_arr[0] != 1.0 || _f64_f_arr[9] != 2.0)
						return false;
					if (_i64_d_arr[0] != -1 || _i64_d_arr[9] != PILO_INT64_MAX)
						return false;
					if (_i64_f_arr[0] != -1 || _i64_f_arr[9] != PILO_INT64_MAX)
						return false;
					if (_u64_d_arr[0] != 99 || _u64_d_arr[9] != PILO_UINT64_MAX)
						return false;
					if (_u64_f_arr[0] != 99 || _u64_f_arr[9] != PILO_UINT64_MAX)
						return false;
					if ((_i32_d_arr != nullptr) && (_i32_d_arr[0] != -1 || _i32_d_arr[9] != PILO_INT32_MAX))
						return false;
					if (_i32_f_arr[0] != -1 || _i32_f_arr[9] != PILO_INT32_MAX)
						return false;
					if ((_u32_d_arr != nullptr) && (_u32_d_arr[0] != 99 || _u32_d_arr[9] != PILO_UINT32_MAX))
						return false;
					if (_u32_f_arr[0] != 99 || _u32_f_arr[9] != PILO_UINT32_MAX)
						return false;
					if ((_i16_d_arr != nullptr) && (_i16_d_arr[0] != -1 || _i16_d_arr[9] != PILO_INT16_MAX))
						return false;
					if (_i16_f_arr[0] != -1 || _i16_f_arr[9] != PILO_INT16_MAX)
						return false;
					if ((_u16_d_arr != nullptr) && (_u16_d_arr[0] != 11 || _u16_d_arr[9] != PILO_UINT16_MAX))
						return false;
					if (_u16_f_arr[0] != 11 || _u16_f_arr[9] != PILO_UINT16_MAX)
						return false;

					if (!_inner_f.validate(_i32v))
						return false;						

					if (_inner_d != nullptr && !_inner_f.validate(_i32v))
						return false;

					if (_inner_null != nullptr)
						return false;

					for (int k=0; k<7; k++)
						if (!_inner_f_arr[k].validate(_i32v+k))
						{
							return false;
						}

					for (int k = 0; k < 7; k++)
					{
						if (k != 3)
						{
							if (!_inner_d_arr[k]->validate(_i32v + k))
							{
								return false;
							}
						}
						else
						{
							if (nullptr != _inner_d_arr[k])
								return false;
						}
					}

					for (int k = 0; k < 7; k++)
					{
						if (k != 3)
						{
							if (!_inner_d2_arr[k]->validate(_i32v + k))
							{
								return false;
							}
						}
						else
						{
							if (nullptr != _inner_d2_arr[k])
								return false;
						}
					}
						

					return true;
				}
				::pilo::i64_t id() const { return _id; }

				// Inherited via serializable_interface
				::pilo::err_t serialize(::pilo::core::memory::serializable_header_interface* header, ::pilo::core::memory::byte_buffer_interface* byte_buffer) const override;

				::pilo::err_t deserialize(::pilo::core::memory::serializable_header_interface* header, ::pilo::core::memory::byte_buffer_interface* byte_buffer) override;

			private:
				void _reset_ptrs()
				{
					if (_dcstr != nullptr)
					{
						PMF_HEAP_FREE(_dcstr);
						_dcstr = nullptr;
					}

					if (_bin_data != nullptr)
					{
						PMF_HEAP_FREE(_bin_data);
						_bin_data = nullptr;
					}

					if (_db_arr != nullptr)
					{
						PMF_HEAP_FREE(_db_arr);
						_db_arr = nullptr;
					}

					if (_i8_d_arr != nullptr)
					{
						PMF_HEAP_FREE(_i8_d_arr);
						_i8_d_arr = nullptr;
					}

					if (_u8_d_arr != nullptr)
					{
						PMF_HEAP_FREE(_u8_d_arr);
						_u8_d_arr = nullptr;
					}

					if (_f32_d_arr != nullptr)
					{
						PMF_HEAP_FREE(_f32_d_arr);
						_f32_d_arr = nullptr;
					}

					if (_f64_d_arr != nullptr)
					{
						PMF_HEAP_FREE(_f64_d_arr);
						_f64_d_arr = nullptr;
					}

					if (_i64_d_arr != nullptr)
					{
						PMF_HEAP_FREE(_i64_d_arr);
						_i64_d_arr = nullptr;
					}

					if (_u64_d_arr != nullptr)
					{
						PMF_HEAP_FREE(_u64_d_arr);
						_u64_d_arr = nullptr;
					}

					if (_i32_d_arr != nullptr)
					{
						PMF_HEAP_FREE(_i32_d_arr);
						_i32_d_arr = nullptr;
					}

					if (_u32_d_arr != nullptr)
					{
						PMF_HEAP_FREE(_u32_d_arr);
						_u32_d_arr = nullptr;
					}

					if (_i16_d_arr != nullptr)
					{
						PMF_HEAP_FREE(_i16_d_arr);
						_i16_d_arr = nullptr;
					}

					if (_u16_d_arr != nullptr)
					{
						PMF_HEAP_FREE(_u16_d_arr);
						_u16_d_arr = nullptr;
					}

					if (_inner_d != nullptr)
					{
						::pilo::core::testing::inner_sample_data::deallocate(this->_inner_d);
						this->_inner_d = nullptr;
					}

					for (int k = 0; k < 7; k++)
					{
						if (_inner_d_arr[k] != nullptr)
						{
							::pilo::core::testing::inner_sample_data::deallocate(this->_inner_d_arr[k]);
						}
					}

					if (_inner_d2_arr != nullptr) 
					{
						for (int k = 0; k < 7; k++)
						{
							if (_inner_d2_arr[k] != nullptr)
							{
								::pilo::core::testing::inner_sample_data::deallocate(this->_inner_d2_arr[k]);
							}
						}
						PMF_HEAP_FREE(_inner_d2_arr);					
					}
				}

			private:
				bool			_is_server;
				::pilo::u32_t	_thread_no; //5
				::pilo::u64_t	_id; //13
				::pilo::i8_t	_i8v; //14
				::pilo::u8_t    _u8v; //15
				::pilo::i16_t	_i16v; //17
				::pilo::u16_t	_u16v;  //19
				::pilo::i32_t   _i32v; //23
				::pilo::i64_t	_i64v; //31
				::pilo::f32_t   _f32v; //35
				::pilo::f64_t   _f64v; //43
				char* _dcstr;  //63
				char  _fcstr[35]; //83
				char* _bin_data;  //41 -- 124
				char _bin_data_fiexed[37]; //41 -- 165
				bool* _db_arr; // 180
				bool  _fb_arr[11]; //195
				::pilo::i8_t* _i8_d_arr; //214
				::pilo::i8_t _i8_f_arr[17]; //233
				::pilo::u8_t* _u8_d_arr; //248
				::pilo::u8_t _u8_f_arr[11]; //263
				::pilo::f32_t* _f32_d_arr; //311
				::pilo::f32_t _f32_f_arr[11]; //359 
				::pilo::f64_t* _f64_d_arr; // 443 
				::pilo::f64_t _f64_f_arr[10]; // 527 + 84
				::pilo::i64_t* _i64_d_arr; //611 + 84
				::pilo::i64_t _i64_f_arr[10]; // 695 
				::pilo::u64_t* _u64_d_arr; //779
				::pilo::u64_t _u64_f_arr[10]; // 863
				::pilo::i32_t* _i32_d_arr; 
				::pilo::i32_t _i32_f_arr[10]; 
				::pilo::u32_t* _u32_d_arr; 
				::pilo::u32_t _u32_f_arr[10]; // 1039
				::pilo::i16_t* _i16_d_arr;
				::pilo::i16_t _i16_f_arr[10];
				::pilo::u16_t* _u16_d_arr;
				::pilo::u16_t _u16_f_arr[10]; //1135
				::pilo::core::testing::inner_sample_data _inner_f; //1158 + 23
				::pilo::core::testing::inner_sample_data * _inner_d; //1181
				::pilo::core::testing::inner_sample_data* _inner_null; //1182 + 165
				::pilo::core::testing::inner_sample_data _inner_f_arr[7]; // 1347

				::pilo::core::testing::inner_sample_data *_inner_d_arr[7]; // 1490
				::pilo::core::testing::inner_sample_data ** _inner_d2_arr; // 1633
				
			};

		}
	}
}