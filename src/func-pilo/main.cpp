#include "pilo_test.hpp"
#include "pilo.hpp"
#include <cstring>
#include "core/io/format_output.hpp"
#include "core/fs/functional_test_module_path.hpp"
#include "core/string/functional_test_module_class_string_util.hpp"
#include "core/string/functional_test_module_class_fixed_astring.hpp"
#include "core/string/functional_test_module_class_fixed_wstring.hpp"
#include "core/threading/functional_test_module_class_basic_thread.hpp"
#include "core/threading/functional_test_module_class_nonrecursive_timed_mutex.hpp"
#include "core/threading/functional_test_module_class_nonrecursive_mutex.hpp"
#include "core/threading/functional_test_module_class_recursive_timed_mutex.hpp"
#include "core/threading/functional_test_module_class_recursive_mutex.hpp"
#include "core/container/functional_test_module_class_fixed_array.hpp"
#include "core/container/functional_test_module_class_fixed_bin_heap.hpp"
#include "core/fs/functional_test_module_mmap.hpp"
#include "core/io/functional_test_module_class_fixed_memory_stream.hpp"
#include "core/container/functional_test_module_class_singly_linked_selflist.hpp"
#include "core/memory/functional_test_module_class_overlapped_memory_pool.hpp"
#include "core/memory/functional_test_module_class_memory_pool.hpp"
#include "core/memory/functional_test_module_class_compactable_memory_pool.hpp"
#include "core/memory/functional_test_module_class_compactable_overlapped_memory_pool.hpp"
#include "core/memory/functional_test_module_class_portable_object_pool.hpp"
#include "core/memory/functional_test_module_class_io_buffer.hpp"
#include "core/datetime/functional_test_module_class_datetime.hpp"
#include "core/fs/functional_test_module_fs_util.hpp"
#include "core/memory/functional_test_module_class_dynamic_buffer.hpp"
#include "core/fs/functional_test_module_file.hpp"
#include "core/fs/fs_util.hpp"
#include "core/process/process_util.hpp"
#include "core/random/simple_random.hpp"
#include "core/datetime/datetime.hpp"
#include "core/pattern/lazy_singletion_mts.hpp"
#include <iostream>
#include <limits>



#include "core/memory/presure_test_portable_object_pool.hpp"

#ifdef _PILO_DEBUG_HEAP
#include "core/debug/heap/debug_new.hpp"
#endif //

const unsigned int __glb_cst_power_of_2[32] = {
    0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010, 0x00000020, 0x00000040, 0x00000080,
    0x00000100, 0x00000200, 0x00000400, 0x00000800, 0x00001000, 0x00002000, 0x00004000, 0x00008000,
    0x00010000, 0x00020000, 0x00040000, 0x00080000, 0x00100000, 0x00200000, 0x00400000, 0x00800000,
    0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000
};

template<size_t CAPA_CNT, size_t UNIT_BITS>
class fixed_bits_array
{
public:
    typedef fixed_bits_array<CAPA_CNT, UNIT_BITS>  My_type;
    const static size_t BYTE_LENGTH = (CAPA_CNT*UNIT_BITS + 7) >> 3;
    const static size_t ARRAY_LENGTH = (BYTE_LENGTH + 3) >> 2;

public:
    fixed_bits_array()
    {
        M_ASSERT(UNIT_BITS < 32);
        clear();
    }

    fixed_bits_array(const My_type &src)
    {
        M_ASSERT(UNIT_BITS < 32);
        memcpy(_m_bitmap, src._m_bitmap, BYTE_LENGTH);
    }

    ::pilo::error_number_t assign(const char* data, size_t sz, size_t count)
    {
        M_ASSERT(UNIT_BITS < 32);
        M_ASSERT(UNIT_BITS > 0);

        if (count > CAPA_CNT)
        {
            return ::pilo::EC_OUT_OF_RANGE;
        }

        if (data == nullptr)
        {
            return ::pilo::EC_NULL_PARAM;
        }

        if (UNIT_BITS <= 0 || UNIT_BITS > 32)
        {
            return ::pilo::EC_OUT_OF_RANGE;
        }

        clear();

        size_t min_sz = min(sz, BYTE_LENGTH);
        ::memcpy((char*)_m_bitmap, data, min_sz);

        _m_count = count;

        return ::pilo::EC_OK;
    }

    

    /**
    * clear all bits
    */
    inline My_type& clear()
    {
        _m_count = 0;
        memset(_m_bitmap, 0x00, ARRAY_LENGTH*4);
        return *this;
    }




protected:
    pilo::u32_t _m_bitmap[ARRAY_LENGTH];
    size_t _m_count;

};




template<typename T> class Combinations {
    // Combinations(std::vector<T> s, int m) iterate all Combinations without repetition
    // from set s of size m s = {0,1,2,3,4,5} all permuations are: {0, 1, 2}, {0, 1,3}, 
    // {0, 1, 4}, {0, 1, 5}, {0, 2, 3}, {0, 2, 4}, {0, 2, 5}, {0, 3, 4}, {0, 3, 5},
    // {0, 4, 5}, {1, 2, 3}, {1, 2, 4}, {1, 2, 5}, {1, 3, 4}, {1, 3, 5}, {1, 4, 5}, 
    // {2, 3, 4}, {2, 3, 5}, {2, 4, 5}, {3, 4, 5}

    public:
    Combinations(std::vector<T> s, int m) : M(m), set(s), partial(std::vector<T>(M))
    {
        N = (int) s.size(); // unsigned long can't be casted to int in initialization

        out = std::vector<std::vector<T>>(comb(N, M), std::vector<T>(M)); // allocate space
        count = 0;

        generate(0, N - 1, M - 1);
    };

    typedef typename std::vector<std::vector<T>>::const_iterator const_iterator;
    typedef typename std::vector<std::vector<T>>::iterator iterator;
    iterator begin() { return out.begin(); }
    iterator end() { return out.end(); }
    std::vector<std::vector<T>> get() { return out; }

    private:
    void generate(int i, int j, int m);
    unsigned long long comb(unsigned long long n, unsigned long long k); // C(n, k) = n! / (n-k)!

    int N;
    int M;
    std::vector<T> set;
    std::vector<T> partial;
    std::vector<std::vector<T>> out;
    int count;

};

template<typename T>
void Combinations<T>::generate(int i, int j, int m) {
    // combination of size m (number of slots) out of set[i..j]
    if (m > 0) {
        for (int z = i; z < j - m + 1; z++) {
            partial[M - m - 1] = set[z]; // add element to permutation
            generate(z + 1, j, m - 1);
        }
    }
    else {
        // last position
        for (int z = i; z < j - m + 1; z++) {
            partial[M - m - 1] = set[z];
            out[count++] = std::vector<T>(partial); // add to output vector
        }
    }
}

template<typename T>
unsigned long long
Combinations<T>::comb(unsigned long long n, unsigned long long k) {
    // this is from Knuth vol 3

    if (k > n) {
        return 0;
    }
    unsigned long long r = 1;
    for (unsigned long long d = 1; d <= k; ++d) {
        r *= n--;
        r /= d;
    }
    return r;
}

//arr为原始数组
//start为遍历起始位置
//result保存结果，为一维数组
//count为result数组的索引值，起辅助作用
//NUM为要选取的元素个数

template <typename T, int _Capacity>
class array_combinaton_record
{
public:
    array_combinaton_record() {}
    T   data[_Capacity];
};

template <typename T, int _Capacity, int _Choose>
class array_combinaton
{
public:
    typedef array_combinaton_record<T, _Choose> result_record_type;

public:
    array_combinaton(T* orig_arr)
    {
        memmove(_array, orig_arr, sizeof(_array));
    }

    bool generate_combination_asecnd()
    {
        if (_Choose > _Capacity)
        {
            return false;
        }

        _result.clear();
        int  result[_Choose] = { 0 };
        _combine_ascend(_array, 0, result, _Choose, _Choose, sizeof(_array) / sizeof(T));

        return true;
    }

    bool generate_combination_descend()
    {
        if (_Choose > _Capacity)
        {
            return false;
        }

        _result.clear();
        int  result[_Choose] = { 0 };
        _combine_descend(_array, _Capacity, result, _Choose, _Choose);

        return true;
    }

    std::vector<result_record_type>& get() { return _result; }

private:
    T       _array[_Capacity];
    std::vector<result_record_type> _result;

private:
    void _combine_ascend(T* arr, int start, int* result, int count, const int NUM, const int arr_len)
    {
        printf("start = %d  loop=%d r0=%d r1=%d r2=%d cnt=%d N=%d\n", start, (arr_len + 1 - count), result[0], result[1],result[2], count, NUM);
        int i = 0;
        for (i = start; i < arr_len + 1 - count; i++)
        {            
            result[count - 1] = i;
            if (count - 1 == 0)
            {                
                int j;
                result_record_type v;
                int v_idx = 0;
                printf("j from=%d \n",NUM-1);
                for (j = NUM - 1; j >= 0; j--)
                {
                    v.data[v_idx++] = arr[result[j]];                    
                }
                _result.push_back(v);

            }
            else
                _combine_ascend(arr, i + 1, result, count - 1, NUM, arr_len);
        }
    }

    void _combine_descend(T* arr, int start, int* result, int count, const int NUM)
    {
        int i;
        for (i = start; i >= count; i--)
        {
            result[count - 1] = i - 1;
            if (count > 1)
            {
                _combine_descend(arr, i - 1, result, count - 1, NUM);
            }
            else
            {
                int j;
                result_record_type v;
                int v_idx = 0;
                for (j = NUM - 1; j >= 0; j--)
                {
                    v.data[v_idx++] = arr[result[j]];
                }
                _result.push_back(v);
            }
        }
    }

};


void combine_decrease(int* arr, int start, int* result, int count, const int NUM)
{
    int i;
    for (i = start; i >= count; i--)
    {
        result[count - 1] = i - 1;
        if (count > 1)
        {
            combine_decrease(arr, i - 1, result, count - 1, NUM);
        }
        else
        {
            int j;
            for (j = NUM - 1; j >= 0; j--)
                printf("%d\t", arr[result[j]]);
            printf("\n");
        }
    }
}


void combine_increase(int* arr, int start, int* result, int count, const int NUM, const int arr_len)
{
    int i = 0;
    for (i = start; i < arr_len + 1 - count; i++)
    {
        result[count - 1] = i;
        if (count - 1 == 0)
        {
            int j;
            for (j = NUM - 1; j >= 0; j--)
                printf("%d\t", arr[result[j]]);
            printf("\n");
        }
        else
            combine_increase(arr, i + 1, result, count - 1, NUM, arr_len);
    }
}


// #include "core/fs/directory_change_monitor.hpp"
// 
// class test_directory_change_monitor : public ::pilo::core::fs::directory_change_monitor
// {
// public:
// 
//     virtual i32_t on_error(const char* dir_pathname, ::pilo::error_number_t err_code, ::pilo::os_error_number_t os_err_code)
//     {
//         ::pilo::core::io::console_format_output("ERROR!!! :%s code %d:%d\n", dir_pathname, err_code,os_err_code);
//         return ::pilo::EC_OK;
//     }
// 
//     virtual i32_t on_monitor_started(const char* path, ::pilo::error_number_t err_code)
//     {        
//         ::pilo::core::io::console_format_output("on_monitor_start:%s code=%d\n",path, err_code);
//         return ::pilo::EC_OK;
//     }
// 
//     virtual i32_t on_monitor_stopped(const char* path, ::pilo::error_number_t err_code)
//     {
//         ::pilo::core::io::console_format_output("on_monitor_stop:%s code=%d\n", path, err_code);
//         return ::pilo::EC_OK;
//     }
// 
//     virtual i32_t on_file_added(const char* dir_pathname, const char* filename_ptr, size_t filename_length)
//     {
//         ::pilo::core::io::console_format_output("[%s]: file <%s> added len = %d\n", dir_pathname, filename_ptr, filename_length);
//         return 0;
//     }
// 
//     virtual i32_t on_file_removed(const char* dir_pathname, const char* filename_ptr, size_t filename_length)
//     {
//         ::pilo::core::io::console_format_output("[%s]: file <%s> removed len = %d\n", dir_pathname,filename_ptr, filename_length);
//         return 0;
//     }
// 
//     virtual i32_t on_file_modified(const char* dir_pathname, const char* filename_ptr, size_t filename_length)
//     {
//         ::pilo::core::io::console_format_output("[%s]: file <%s> modified %d\n", dir_pathname, filename_ptr, filename_length);
//         return 0;
//     }
// 
//     virtual i32_t on_file_renamed(const char* dir_pathname, const char* old_filename_ptr, size_t old_filename_length, const char* neo_filename_ptr, size_t neo_filename_length)
//     { 
//         ::pilo::core::io::console_format_output("[%s]: old_file <%s> %d renamed <%s> %d\n",dir_pathname, old_filename_ptr, old_filename_length, neo_filename_ptr, neo_filename_length);
//     
//         return ::pilo::EC_OK; 
//     }
// 
// 
// };

#include <mutex>
#include "core/threading/nonrecursive_timed_mutex.hpp"
#include "core/threading/recursive_timed_mutex.hpp"

u64_t __aa = 0;
//std::mutex __mtx;
//::pilo::core::threading::nonrecursive_timed_mutex __mtx;
::pilo::core::threading::recursive_timed_mutex __mtx;

class test_mtx : public ::pilo::core::threading::basic_thread
{
    public:
    virtual i32_t on_run()
    {
        bool bret = false;
        int i = 0; 
        for (;;)
        {          
            
            ::pilo::i64_t start = ::pilo::core::datetime::datetime::steady_now_microsecs();

            if (i % 4 == 0) 
            {
                std::chrono::milliseconds ms(1800);
                bret = __mtx.try_lock_for(ms);
                printf("try_lock_for Chrono: ");
            }
            else if (i % 4 == 1)
            { 
                bret = __mtx.try_lock_for(1800);
                printf("try_lock_for milli: ");
            }
            else if (i % 4 == 2)
            {
                std::chrono::milliseconds ms(1800);
                bret = __mtx.try_lock_until(std::chrono::system_clock::now() + ms);
                printf("try_lock_until chrono: ");
            }
            else 
            {
                bret = __mtx.try_lock_until(time(0) + 2);    
                printf("try_lock_until sec: ");
            }



            ::pilo::i64_t end = ::pilo::core::datetime::datetime::steady_now_microsecs();

            if (bret)
            {
                printf("lock ok ! time costs %llu\n", end-start);
                i ++;
            }
            else
            {
                printf("lock timeout ! time cost %llu\n", end - start);
            }

            i ++;

            if (i == (int) 0xFFFFFFFF)
            {
                break;
            }
            
            
        }
        return 0;
    }
   
};


int main(int argc, char *argv[])
{
    M_UNUSED(argc);
    M_UNUSED(argv);	 

    int init_pilo_rc = -1;
    if ((init_pilo_rc = ::pilo_initialize()) != 0)
    {
        ::pilo::core::io::console_format_output("pilo init failed. return code is %d", init_pilo_rc);
        return -1;
    }

#ifdef _PILO_DEBUG_HEAP
    pilo_debug_heap_leak_set_verbose(false);
    pilo_debug_heap_leak_set_report_onexit(true);
    pilo_debug_heap_leak_set_report_output_mode(M_DEBUG_NEW_REPORT_OUTPUT_FILE, "./memleak.log");   
#endif

    bool break_on_error = true;
    pilo::i32_t id = 1;

    pilo::test::functional_test_module_class_fixed_astring  fixed_astring_cases(id++, "functional_test_module_class_fixed_astring", pilo::test::g_functional_cases_fixed_astring);
    fixed_astring_cases.run_cases(break_on_error);
    fixed_astring_cases.console_output();

    pilo::test::functional_test_module_path path_cases(id++, "functional_test_module_path", pilo::test::g_functional_cases_path);
    path_cases.run_cases(break_on_error);
    path_cases.console_output();

    ::pilo::test::functional_test_module_fs_utils fs_util_cases(id++, "functional_test_module_fs_utils", pilo::test::g_functional_cases_fs_util);
    fs_util_cases.run_cases(break_on_error);
    fs_util_cases.console_output();

    pilo::test::functional_test_module_file  file_cases(id++, "functional_test_module_file", pilo::test::g_functional_cases_file);
    file_cases.run_cases(break_on_error);
    file_cases.console_output();

    ::pilo::test::functional_test_module_class_datetime datetime_cases(id++, "functional_test_module_class_datetime", pilo::test::g_functional_cases_datetime);
    datetime_cases.run_cases(break_on_error);
    datetime_cases.console_output();

    pilo::test::functional_test_module_mmap  mmap_cases(id++, "functional_test_module_mmap", pilo::test::g_functional_cases_mmap);
    mmap_cases.run_cases(break_on_error);
    mmap_cases.console_output();

//     ::pilo::test::presure_test_portable_object_pool presure_obj_pool;
//     presure_obj_pool.test(nullptr, 0, nullptr);
//     presure_obj_pool.reprot(nullptr, 0, nullptr);


// 

// 
//     pilo::test::functional_test_module_class_portable_object_pool  portable_object_pool_cases(id++, "functional_test_module_class_portable_object_pool", pilo::test::g_functional_cases_portable_object_pool);
//     portable_object_pool_cases.run_cases(break_on_error);
//     portable_object_pool_cases.console_output();
	
//     pilo::test::functional_test_module_class_string_util  string_util_cases(id++, "functional_test_module_class_string_util", pilo::test::g_functional_cases_string_util);
//     string_util_cases.run_cases(break_on_error);
//     string_util_cases.console_output();


// 
//     pilo::test::functional_test_module_class_fixed_wstring  fixed_wstring_cases(id++, "functional_test_module_class_fixed_wstring", pilo::test::g_functional_cases_fixed_wstring);
//     fixed_wstring_cases.run_cases(break_on_error);
//     fixed_wstring_cases.console_output();
// 


//     pilo::test::functional_test_module_class_basic_thread  basic_thread_cases(id++, "functional_test_module_class_basic_thread", pilo::test::g_functional_cases_basic_thread);
//     basic_thread_cases.run_cases(break_on_error);
//     basic_thread_cases.console_output();
// 
//     pilo::test::functional_test_module_class_nonrecursive_timed_mutex nonrecursive_timed_mutex_cases(id++, "functional_test_module_class_nr_timed_mutex", pilo::test::g_functional_cases_nonrecursive_timed_mutex);
//     nonrecursive_timed_mutex_cases.run_cases(break_on_error);
//     nonrecursive_timed_mutex_cases.console_output();
// 
//     pilo::test::functional_test_module_class_nonrecursive_mutex nonrecursive_mutex_cases(id++, "functional_test_module_class_nr_mutex", pilo::test::g_functional_cases_nonrecursive_mutex);
//     nonrecursive_mutex_cases.run_cases(break_on_error);
//     nonrecursive_mutex_cases.console_output();
// 
//     pilo::test::functional_test_module_class_recursive_timed_mutex recursive_timed_mutex_cases(id++, "functional_test_module_class_r_t_mutex", pilo::test::g_functional_cases_recursive_timed_mutex);
//     recursive_timed_mutex_cases.run_cases(break_on_error);
//     recursive_timed_mutex_cases.console_output();

//     pilo::test::functional_test_module_class_recursive_mutex recursive_mutex_cases(id++, "functional_test_module_class_r_mutex", pilo::test::g_functional_cases_recursive_mutex);
//     recursive_mutex_cases.run_cases(break_on_error);
//     recursive_mutex_cases.console_output();


//     pilo::test::functional_test_module_class_fixed_array fixed_array_cases(id++, "functional_test_module_class_fixed_array", pilo::test::g_functional_cases_fixed_array);
//     fixed_array_cases.run_cases(break_on_error);
//     fixed_array_cases.console_output();
// 
//     pilo::test::functional_test_module_class_fixed_bin_heap fixed_bin_heap_cases(id++, "functional_test_module_class_fixed_bin_heap", pilo::test::g_functional_cases_fixed_bin_heap);
//     fixed_bin_heap_cases.run_cases(break_on_error);
//     fixed_bin_heap_cases.console_output();
// 
//     pilo::test::functional_test_module_class_fixed_memory_stream fixed_memory_stream_cases(id++, "functional_test_module_class_fixed_memory_streams", pilo::test::g_functional_cases_fixed_memory_stream);
//     fixed_memory_stream_cases.run_cases(break_on_error);
//     fixed_memory_stream_cases.console_output();
// 
//     pilo::test::functional_test_module_class_singly_linked_selflist singly_linked_selflist_cases(id++, "functional_test_module_class_singly_linked_selflist", pilo::test::g_functional_cases_singly_linked_selflist);
//     singly_linked_selflist_cases.run_cases(break_on_error);
//     singly_linked_selflist_cases.console_output();
// 
//     pilo::test::functional_test_module_class_overlapped_memory_pool overlapped_memory_pool_cases(id++, "functional_test_module_class_overlapped_memory_pool", pilo::test::g_functional_cases_overlapped_memory_pool);
//     overlapped_memory_pool_cases.run_cases(break_on_error);
//     overlapped_memory_pool_cases.console_output();
// 
//     pilo::test::functional_test_module_class_memory_pool memory_pool_cases(id++, "functional_test_module_class_memory_pool", pilo::test::g_functional_cases_memory_pool);
//     memory_pool_cases.run_cases(break_on_error);
//     memory_pool_cases.console_output();
// 
//     pilo::test::functional_test_module_class_compactable_memory_pool compactable_memory_pool_cases(id++, "functional_test_module_class_compactable_memory_pool", pilo::test::g_functional_cases_compactable_memory_pool);
//     compactable_memory_pool_cases.run_cases(break_on_error);
//     compactable_memory_pool_cases.console_output();
// 
//     pilo::test::functional_test_module_class_compactable_overlapped_memory_pool compactable_overlapped_memory_pool_cases(id++, "functional_test_module_class_compactable_overlapped_memory_pool", pilo::test::g_functional_cases_compactable_overlapped_memory_pool);
//     compactable_overlapped_memory_pool_cases.run_cases(break_on_error);
//     compactable_overlapped_memory_pool_cases.console_output();
// 


//     pilo::test::functional_test_module_class_io_buffer io_buffer_cases(id++, "functional_test_module_class_io_buffer", pilo::test::g_functional_cases_io_buffer);
//     io_buffer_cases.run_cases(break_on_error);
//     io_buffer_cases.console_output();
// 
//     ::pilo::test::functional_test_module_class_datetime datetime_cases(id++, "functional_test_module_class_datetime", pilo::test::g_functional_cases_datetime);
//     datetime_cases.run_cases(break_on_error);
//     datetime_cases.console_output();
// 

// 

// 

//     pilo::test::functional_test_module_class_dynamic_buffer  dynamic_buffer_cases(id++, "functional_test_module_class_dynamic_buffer", pilo::test::g_functional_cases_dynamic_buffer);
//     dynamic_buffer_cases.run_cases(break_on_error);
//     dynamic_buffer_cases.console_output();



    getchar();
    return 0;
}
