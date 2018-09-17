

#ifdef _PILO_DEBUG_HEAP
#define _CRT_SECURE_NO_WARNINGS


#include "core/parameter.hpp"


#include <new>
#include <stdio.h>
#include <stdlib.h>
#include "core/threading/basic_thread.hpp"
#include "core/threading/recursive_mutex.hpp"
#include "core/threading/mutex_locker.hpp"
#include "core/datetime/datetime.hpp"

#ifdef WINDOWS
#include <io.h>
#include <fcntl.h>
#include <share.h>
#include <sys/stat.h>
#include <windows.h>
#include <DbgHelp.h>
#pragma comment(lib,"DbgHelp.lib")
#endif

#ifdef M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_REPORT
#define  M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_NEW
#endif

#ifdef M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_REPORT
#define  M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_NEW
#define  M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_DELETE
#endif

#ifdef WINDOWS
void pilo_debug_get_backtrace_info(char* buffer, int sz)
{
    unsigned int   i;
    void         * stack[100];
    unsigned short frames;
    SYMBOL_INFO  * symbol;
    HANDLE         process;

    process = GetCurrentProcess();

    SymInitialize(process, NULL, TRUE);

    frames = CaptureStackBackTrace(0, 100, stack, NULL);
    symbol = (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO)+256 * sizeof(char), 1);
    symbol->MaxNameLen = 255;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

    size_t index = 0;
    char szbuffer[2048] = {0};
    *buffer = 0;
    for (i = 0; i < frames; i++)
    {

        SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);

        sprintf_s(szbuffer, sizeof(szbuffer), "\t%i: %s - 0x%0X\n", frames - i - 1, symbol->Name, symbol->Address);
        size_t len = strlen (szbuffer);

        if (len >= sz-index) break;

        sprintf_s(buffer + index, sz-index, "%s", szbuffer);

        index += (len);
    }

    free(symbol);
}
#else
void pilo_debug_get_backtrace_info(char* buffer, int sz)
{
    
}

#endif // WINDOWS


#define M_DEBUG_NEW_REPORT_OUTPUT_NONE	0
#define M_DEBUG_NEW_REPORT_OUTPUT_STDOUT 1
#define M_DEBUG_NEW_REPORT_OUTPUT_STDERR 2
#define M_DEBUG_NEW_REPORT_OUTPUT_FILE	3
#define M_DEBUG_NEW_REPORT_OUTPUT_VCDBG_CONSOLE 4

#ifdef _MSC_VER
#pragma warning(disable: 4073)
#pragma init_seg(lib)
#endif

#ifndef M_DEBUG_NEW_INTERNAL_BUCKET_SIZE
#define M_DEBUG_NEW_INTERNAL_BUCKET_SIZE 256
#endif

#ifndef DEBUG_NEW_HASH
#define DEBUG_NEW_HASH(p) (((::pilo::uint_r)(p) >> 8) % M_DEBUG_NEW_INTERNAL_BUCKET_SIZE)
#endif

// The default behaviour now is to copy the file name, because we found
// that the exit leakage check cannot access the address of the file
// name sometimes (in our case, a core dump will occur when trying to
// access the file name in a shared library after a SIGINT).
#ifndef DEBUG_NEW_FILENAME_LEN
#define DEBUG_NEW_FILENAME_LEN	20
#endif
#if DEBUG_NEW_FILENAME_LEN == 0 && !defined(M_DEBUG_NEW_USE_REF_FILENAME)
#define M_DEBUG_NEW_USE_REF_FILENAME
#endif
#ifndef M_DEBUG_NEW_USE_REF_FILENAME
#include <string.h>
#endif

struct new_ptr_list_t
{
	new_ptr_list_t*		next;
#ifdef M_DEBUG_NEW_USE_REF_FILENAME
	const char*			file;
#else
	char				file[DEBUG_NEW_FILENAME_LEN];
#endif
	int					line;
	size_t				size;

#if defined (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_NEW) || defined  (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_REPORT)
    ::pilo::u64_t          ats;
#endif

#if defined (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_DELETE) || defined  (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_NEW)
    ::pilo::u64_t           dts;
#endif

#if defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_NEW) || defined  (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_REPORT)
    os_thread_id_t        atid;
#endif

#if defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_DELETE) || defined  (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_NEW)
    os_thread_id_t        dtid;
#endif

#if defined (M_DEBUG_NEW_INFO_REQUIRE_BT_ON_NEW) || defined  (M_DEBUG_NEW_INFO_REQUIRE_BT_ON_REPORT)
    char                bt_strinfo[M_DEBUG_NEW_BT_INFO_SIZE];
#endif
};

static new_ptr_list_t* __pilo_var_heap_leak_hash_container[M_DEBUG_NEW_INTERNAL_BUCKET_SIZE];
static bool __pilo_var_heap_leak_verbose = false;
static bool __pilo_var_heap_leak_report_onexit = true;
static char __pilo_var_heap_leak_output_mode = M_DEBUG_NEW_REPORT_OUTPUT_NONE;
static char __pilo_var_heap_leak_output_filename[513] = {0};
static FILE* __pilo_var_heap_leak_output_filep = NULL;

#ifdef WINDOWS
static void __pilo_debug_vcdbg_console_format_output(const char * format, ...)
{
    char buf[256];
    va_list ap;
    va_start(ap, format);
    _vsnprintf(buf, sizeof(buf), format, ap);
    va_end(ap);
    OutputDebugString(buf);
}
#endif

#ifdef M_DEBUG_NEW_REQUIRE_THREAD_SAFE
    static ::pilo::core::threading::recursive_mutex __pilo_var_stc_heap_leak_checker_mutex;
#endif


void pilo_debug_heap_leak_set_verbose(bool b)
{
    __pilo_var_heap_leak_verbose = b;
}

void pilo_debug_heap_leak_set_report_onexit(bool b)
{
    __pilo_var_heap_leak_report_onexit = b;
}

static bool _pilo_debug_heap_leak_init_report_file()
{    

    if (NULL != __pilo_var_heap_leak_output_filep)
    {
        fclose(__pilo_var_heap_leak_output_filep);
    }

#ifdef WINDOWS
    int fd = -1;
    int err = _sopen_s(&fd, __pilo_var_heap_leak_output_filename, _O_RDWR | _O_APPEND | _O_CREAT, _SH_DENYNO, _S_IREAD | _S_IWRITE);
    if (err != 0)
    {
        return false;
    }
        
    if( (__pilo_var_heap_leak_output_filep = _fdopen( fd, "a" )) == NULL )
    {
        _close(fd);
        return false;
    }

#else
    __pilo_var_heap_leak_output_filep = fopen(__pilo_var_heap_leak_output_filename, "a");
    if (NULL == __pilo_var_heap_leak_output_filep)
    {
        return false;
    }
#endif

    

    if (__pilo_var_heap_leak_output_filep == NULL)
    {
        return false;
    }

    ::pilo::core::datetime::local_datetime ldt = ::pilo::core::datetime::datetime::now();
    fprintf(__pilo_var_heap_leak_output_filep, "[Init] Open file <%s> to output analyze reprot. (%04d-%02d-%02d %02d:%02d:%02d)\n",
            __pilo_var_heap_leak_output_filename, ldt.year, ldt.month, ldt.day, ldt.hour, ldt.minute, ldt.second);

    return true;
}

static void _pilo_debug_heap_leak_fini_report_file()
{

    if (NULL != __pilo_var_heap_leak_output_filep)
    {
        ::pilo::core::datetime::local_datetime ldt = ::pilo::core::datetime::datetime::now();        
        fprintf(__pilo_var_heap_leak_output_filep, "[Fini] Close file <%s> of analyze reprot. (%04d-%02d-%02d %02d:%02d:%02d)\n", 
            __pilo_var_heap_leak_output_filename, ldt.year, ldt.month,ldt.day, ldt.hour, ldt.minute, ldt.second );
        fclose(__pilo_var_heap_leak_output_filep);
        __pilo_var_heap_leak_output_filep = NULL;
    }
}

void pilo_debug_heap_leak_set_report_output_mode(char mode, const char* arg)
{
#ifdef M_DEBUG_NEW_REQUIRE_THREAD_SAFE
    ::pilo::core::threading::mutex_locker<::pilo::core::threading::recursive_mutex> guard(__pilo_var_stc_heap_leak_checker_mutex);
#endif

#ifdef WINDOWS
    int max_mode = M_DEBUG_NEW_REPORT_OUTPUT_VCDBG_CONSOLE;
#else
    int max_mode = M_DEBUG_NEW_REPORT_OUTPUT_FILE;
#endif // WINDOWS

	if (mode < M_DEBUG_NEW_REPORT_OUTPUT_NONE || mode > max_mode)
	{
		mode = M_DEBUG_NEW_REPORT_OUTPUT_STDOUT;
		return;
	}
    
    if (__pilo_var_heap_leak_output_mode != M_DEBUG_NEW_REPORT_OUTPUT_NONE && mode == M_DEBUG_NEW_REPORT_OUTPUT_FILE)
    {
        mode = M_DEBUG_NEW_REPORT_OUTPUT_STDOUT;
        return;
    }

	__pilo_var_heap_leak_output_mode = mode;
    if (__pilo_var_heap_leak_output_mode == M_DEBUG_NEW_REPORT_OUTPUT_FILE)
    {
        if (arg == 0 || arg[0] == 0)
        {
            arg = M_DEBUG_NEW_DEFAULT_REPORT_FILE_PATH;
        }
#ifdef WINDOWS
        strncpy_s(__pilo_var_heap_leak_output_filename, arg, sizeof(__pilo_var_heap_leak_output_filename));
#else
        strncpy(__pilo_var_heap_leak_output_filename, arg, sizeof(__pilo_var_heap_leak_output_filename));
#endif // WINDOWS

        if (!_pilo_debug_heap_leak_init_report_file())
        {
            __pilo_var_heap_leak_output_mode = M_DEBUG_NEW_REPORT_OUTPUT_STDOUT;
        }
    }

}

bool pilo_debug_heap_check_leaks()
{
#ifdef M_DEBUG_NEW_REQUIRE_THREAD_SAFE
    ::pilo::core::threading::mutex_locker<::pilo::core::threading::recursive_mutex> guard(__pilo_var_stc_heap_leak_checker_mutex);
#endif
    if (__pilo_var_heap_leak_output_mode == M_DEBUG_NEW_REPORT_OUTPUT_FILE)
    {
        if (NULL == __pilo_var_heap_leak_output_filep)
        {
            _pilo_debug_heap_leak_init_report_file();
        }
    }

#if defined (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_NEW)
    ::pilo::i64_t cts = ::pilo::core::datetime::datetime::steady_now_microsecs();
#endif

    

	bool fLeaked = false;
	for (int i = 0; i < M_DEBUG_NEW_INTERNAL_BUCKET_SIZE; ++i)
	{
		new_ptr_list_t* ptr = __pilo_var_heap_leak_hash_container[i];
		if (ptr == NULL)
			continue;
		fLeaked = true;
		while (ptr)
		{
            if (M_DEBUG_NEW_REPORT_OUTPUT_STDOUT == __pilo_var_heap_leak_output_mode)
            {
#if defined(M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_REPORT) && defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_REPORT)
                fprintf(stdout, "Leak! Obj addr @ 0x%p (s=%u) <%s:%d> t=%lld(%lld) atid=%u\n",
                        (char*)ptr + sizeof(new_ptr_list_t),
                        (unsigned int) ptr->size,
                        ptr->file,
                        ptr->line,
                        cts - ptr->ats,
                        ptr->ats,
                        ptr->atid);
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_REPORT)
                fprintf(stdout, "Leak! Obj addr @ 0x%p (s=%u) <%s:%d> t=%lld(%lld)\n",
                        (char*)ptr + sizeof(new_ptr_list_t),
                        (unsigned int) ptr->size,
                        ptr->file,
                        ptr->line,
                        cts - ptr->ats,
                        ptr->ats);
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_REPORT)
                fprintf(stdout, "Leak! Obj addr @ 0x%p (s=%u) <%s:%d> atid=%u\n",
                        (char*)ptr + sizeof(new_ptr_list_t),
                        (unsigned int) ptr->size,
                        ptr->file,
                        ptr->line,
                        ptr->atid);                
#else
                fprintf(stdout, "Leak! Obj addr @ 0x%p (s=%u) <%s:%d>\n",
                        (char*)ptr + sizeof(new_ptr_list_t),
                        (unsigned int) ptr->size,
                        ptr->file,
                        ptr->line);

#endif
               
            }
            else if (M_DEBUG_NEW_REPORT_OUTPUT_STDERR == __pilo_var_heap_leak_output_mode)
            {
#if defined(M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_REPORT) && defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_REPORT)
                fprintf(stderr, "Leak! Obj addr @ 0x%p (s=%u) <%s:%d> t=%lld(%lld) atid=%u\n",
                        (char*)ptr + sizeof(new_ptr_list_t),
                        (unsigned int) ptr->size,
                        ptr->file,
                        ptr->line,
                        cts - ptr->ats,
                        ptr->ats,
                        ptr->atid);
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_REPORT)
                fprintf(stderr, "Leak! Obj addr @ 0x%p (s=%u) <%s:%d> t=%lld(%lld)\n",
                        (char*)ptr + sizeof(new_ptr_list_t),
                        (unsigned int)ptr->size,
                        ptr->file,
                        ptr->line,
                        cts - ptr->ats,
                        ptr->ats);
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_REPORT)
                fprintf(stderr, "Leak! Obj addr @ 0x%p (s=%u) <%s:%d> atid=%u\n",
                        (char*)ptr + sizeof(new_ptr_list_t),
                        (unsigned int) ptr->size,
                        ptr->file,
                        ptr->line,
                        ptr->atid);
#else
                fprintf(stderr, "Leak! Obj addr @ 0x%p (s=%u) <%s:%d>\n",
                        (char*)ptr + sizeof(new_ptr_list_t),
                        (unsigned int) ptr->size,
                        ptr->file,
                        ptr->line);

#endif
            }
            else if (M_DEBUG_NEW_REPORT_OUTPUT_FILE == __pilo_var_heap_leak_output_mode)
            {
                if (NULL != __pilo_var_heap_leak_output_filep)
                {
#if defined(M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_REPORT) && defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_REPORT)
                    fprintf(__pilo_var_heap_leak_output_filep, "Leak! Obj addr @ 0x%p(s=%u) <%s:%d> t=%lld(%lld) atid=%u\n",
                            (char*)ptr + sizeof(new_ptr_list_t),
                            (unsigned int) ptr->size,
                            ptr->file,
                            ptr->line,
                            cts - ptr->ats,
                            ptr->ats,
                            ptr->atid);    
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_REPORT)
                    fprintf(__pilo_var_heap_leak_output_filep, "Leak! Obj addr @ 0x%p(s=%u) <%s:%d> t=%lld(%lld)\n",
                            (char*)ptr + sizeof(new_ptr_list_t),
                            (unsigned int)ptr->size,
                            ptr->file,
                            ptr->line,
                            cts - ptr->ats,
                            ptr->ats);
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_REPORT)
                    fprintf(__pilo_var_heap_leak_output_filep, "Leak! Obj addr @ 0x%p(s=%u) <%s:%d> atid=%u\n",
                            (char*)ptr + sizeof(new_ptr_list_t),
                            (unsigned int) ptr->size,
                            ptr->file,
                            ptr->line,
                            ptr->atid;
#else
                    fprintf(__pilo_var_heap_leak_output_filep, "Leak! Obj addr @ 0x%p(s=%u) <%s:%d>\n",
                            (char*)ptr + sizeof(new_ptr_list_t),
                            (unsigned int) ptr->size,
                            ptr->file,
                            ptr->line);
#endif

                    

#ifdef M_DEBUG_NEW_INFO_REQUIRE_BT_ON_REPORT
                    fprintf(__pilo_var_heap_leak_output_filep, "%s", ptr->bt_strinfo);
#endif // M_DEBUG_NEW_INFO_REQUIRE_BT_ON_REPORT

                }
                
            }
#ifdef WINDOWS
            else if (M_DEBUG_NEW_REPORT_OUTPUT_VCDBG_CONSOLE == __pilo_var_heap_leak_output_mode)
            {
#if defined(M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_REPORT) && defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_REPORT)
                __pilo_debug_vcdbg_console_format_output("Leak! Obj addr @ 0x%p (s=%u) <%s:%d> t=%lld(%lld) atid=%u\n",
                        (char*)ptr + sizeof(new_ptr_list_t),
                        (unsigned int) ptr->size,
                        ptr->file,
                        ptr->line,
                        cts - ptr->ats,
                        ptr->ats,
                        ptr->atid);
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_REPORT)
                __pilo_debug_vcdbg_console_format_output("Leak! Obj addr @ 0x%p (s=%u) <%s:%d> t=%lld(%lld)\n",
                        (char*)ptr + sizeof(new_ptr_list_t),
                        (unsigned int)ptr->size,
                        ptr->file,
                        ptr->line,
                        cts - ptr->ats,
                        ptr->ats);
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_REPORT)
                __pilo_debug_vcdbg_console_format_output("Leak! Obj addr @ 0x%p (s=%u) <%s:%d> atid=%u\n",
                        (char*)ptr + sizeof(new_ptr_list_t),
                        (unsigned int) ptr->size,
                        ptr->file,
                        ptr->line,
                        ptr->atid);
#else
                __pilo_debug_vcdbg_console_format_output("Leak! Obj addr @ 0x%p (s=%u) <%s:%d>\n",
                        (char*)ptr + sizeof(new_ptr_list_t),
                        (unsigned int) ptr->size,
                        ptr->file,
                        ptr->line);
#endif
              
            }
#endif

			
			ptr = ptr->next;
		}
	}

    fflush(__pilo_var_heap_leak_output_filep);

	if (fLeaked)
		return true;
	else
		return false;
}

void* operator new(size_t size, const char* file, int line)
{
#ifdef M_DEBUG_NEW_REQUIRE_THREAD_SAFE
    ::pilo::core::threading::mutex_locker<::pilo::core::threading::recursive_mutex> guard(__pilo_var_stc_heap_leak_checker_mutex);
#endif
	size_t s = size + sizeof(new_ptr_list_t);
	new_ptr_list_t* ptr = (new_ptr_list_t*)malloc(s);
	if (ptr == NULL)
	{
        if (M_DEBUG_NEW_REPORT_OUTPUT_STDOUT == __pilo_var_heap_leak_output_mode)
        {
            fprintf(stdout, "[HeapAllocDbg] [Fatal] new:  out of memory when allocating %u bytes\n", (unsigned int)size);
        }
        else if (M_DEBUG_NEW_REPORT_OUTPUT_STDERR == __pilo_var_heap_leak_output_mode)
        {
            fprintf(stderr, "[HeapAllocDbg] [Fatal] new:  out of memory when allocating %u bytes\n", (unsigned int)size);
        }
        else if (M_DEBUG_NEW_REPORT_OUTPUT_FILE == __pilo_var_heap_leak_output_mode)
        {
            if (NULL != __pilo_var_heap_leak_output_filep)
            {
                fprintf(__pilo_var_heap_leak_output_filep, "[HeapAllocDbg] [Fatal] new:  out of memory when allocating %u bytes\n", (unsigned int)size);
            }


        }
#ifdef WINDOWS
        else if (M_DEBUG_NEW_REPORT_OUTPUT_VCDBG_CONSOLE == __pilo_var_heap_leak_output_mode)
        {
            __pilo_debug_vcdbg_console_format_output("[HeapAllocDbg] [Fatal] new:  out of memory when allocating %u bytes\n", (unsigned int)size);
        }
#endif
		abort();
	}
	void* pointer = (char*)ptr + sizeof(new_ptr_list_t);
	size_t hash_index = DEBUG_NEW_HASH(pointer);
	ptr->next = __pilo_var_heap_leak_hash_container[hash_index];
#ifdef M_DEBUG_NEW_USE_REF_FILENAME
	ptr->file = file;
#else
	strncpy(ptr->file, file, DEBUG_NEW_FILENAME_LEN - 1);
	ptr->file[DEBUG_NEW_FILENAME_LEN - 1] = '\0';
#endif
	ptr->line = line;
	ptr->size = size;

#if defined(M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_NEW) || defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_REPORT)
    ptr->atid = UINT32_MAX;
#endif

#if defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_DELETE)
    ptr->dtid = UINT32_MAX;
#endif

#if defined (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_NEW) || defined (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_REPORT)
    ptr->ats = 0;
#endif

#if defined (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_DELETE) || defined (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_NEW)
    ptr->dts = 0;
#endif

#if defined (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_NEW)
    ptr->ats = ::pilo::core::datetime::datetime::steady_now_microsecs();
#endif

#if defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_NEW) 
    ptr->atid = ::pilo::core::threading::basic_thread::id();
#endif

#if defined (M_DEBUG_NEW_INFO_REQUIRE_BT_ON_NEW) || defined (M_DEBUG_NEW_INFO_REQUIRE_BT_ON_REPORT)
    pilo_debug_get_backtrace_info(ptr->bt_strinfo, M_DEBUG_NEW_BT_INFO_SIZE - 1);
#endif

	__pilo_var_heap_leak_hash_container[hash_index] = ptr;
    if (__pilo_var_heap_leak_verbose)
    {
        if (M_DEBUG_NEW_REPORT_OUTPUT_STDOUT == __pilo_var_heap_leak_output_mode)
        {
#if defined(M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_NEW) && defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_NEW)
            fprintf(stdout, "new: + addr @ 0x%p (s=%u) <%s:%d> ats=%lld atid=%u\n", pointer, (unsigned int) size, file, line, ptr->ats, ptr->atid);
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_NEW)
            fprintf(stdout, "new: + addr @ 0x%p (s=%u) <%s:%d> ats=%lld\n", pointer, (unsigned int)size, file, line, ptr->ats);
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_NEW)
            fprintf(stdout, "new: + addr @ 0x%p (s=%u) <%s:%d> atid=%u\n", pointer,(unsigned int)  size, file, line, ptr->atid);
#else
            fprintf(stdout, "new: + addr @ 0x%p (s=%u) <%s:%d>\n", pointer, (unsigned int) size, file, line);
#endif
            
        }
        else if (M_DEBUG_NEW_REPORT_OUTPUT_STDERR == __pilo_var_heap_leak_output_mode)
        {
#if defined(M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_NEW) && defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_NEW)
            fprintf(stderr, "new: + addr @ 0x%p (s=%u) <%s:%d> ats=%lld atid=%u\n", pointer, (unsigned int) size, file, line, ptr->ats, ptr->atid);
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_NEW)
            fprintf(stderr, "new: + addr @ 0x%p (s=%u) <%s:%d> ats=%lld\n", pointer, (unsigned int)size, file, line, ptr->ats);
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_NEW)
            fprintf(stderr, "new: + addr @ 0x%p (s=%u) <%s:%d> atid=%u\n", pointer, (unsigned int) size, file, line, ptr->atid);
#else
            fprintf(stderr, "new: + addr @ 0x%p (s=%u) <%s:%d>\n", pointer,(unsigned int)  size, file, line);
#endif
        }
        else if (M_DEBUG_NEW_REPORT_OUTPUT_FILE == __pilo_var_heap_leak_output_mode)
        {
            if (NULL != __pilo_var_heap_leak_output_filep)
            {
#if defined(M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_NEW) && defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_NEW)
                fprintf(__pilo_var_heap_leak_output_filep,
                    "new: + addr @ 0x%p (s=%u) <%s:%d> ats=%lld atid=%u\n",
                    pointer,(unsigned int)  size, file, line, ptr->ats, ptr->atid);
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_NEW)
                fprintf(__pilo_var_heap_leak_output_filep,
                        "new: + addr @ 0x%p (s=%u) <%s:%d> ats=%lld\n",
                        pointer, (unsigned int)size, file, line, ptr->ats);

#elif defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_NEW)
               fprintf(__pilo_var_heap_leak_output_filep,
                        "new: + addr @ 0x%p (s=%u) <%s:%d> atid=%u\n",
                        pointer, (unsigned int) size, file, line, ptr->atid);
#else
                fprintf(__pilo_var_heap_leak_output_filep,
                        "new: + addr @ 0x%p (s=%u) <%s:%d>\n",
                        pointer, (unsigned int) size, file, line);
#endif

#ifdef M_DEBUG_NEW_INFO_REQUIRE_BT_ON_NEW
                fprintf(__pilo_var_heap_leak_output_filep, "%s", ptr->bt_strinfo );
#endif

            }
        }
#ifdef WINDOWS
        else if (M_DEBUG_NEW_REPORT_OUTPUT_VCDBG_CONSOLE == __pilo_var_heap_leak_output_mode)
        {
#if defined(M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_NEW) && defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_NEW)
            __pilo_debug_vcdbg_console_format_output("new: + addr @ 0x%p (s=%u) <%s:%d> ats=%lld atid=%u\n", pointer, (unsigned int) size, file, line, ptr->ats, ptr->atid);
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_NEW)
            __pilo_debug_vcdbg_console_format_output("new: + addr @ 0x%p (s=%u) <%s:%d> ats=%lld\n", pointer, (unsigned int)size, file, line, ptr->ats);
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_NEW)
            __pilo_debug_vcdbg_console_format_output("new: + addr @ 0x%p (s=%u) <%s:%d> atid=%u\n", pointer, (unsigned int) size, file, line, ptr->atid);
#else
            __pilo_debug_vcdbg_console_format_output("new: + addr @ 0x%p (s=%u) <%s:%d>\n", pointer, (unsigned int) size, file, line);
#endif
        }
#endif
    }
		
	return pointer;
}

void* operator new[](size_t size, const char* file, int line)
{
	return operator new(size, file, line);
}

void* operator new(size_t size)
{
	return operator new(size, "<Unknown>", 0);
}

void* operator new[](size_t size)
{
	return operator new(size);
}

void* operator new(size_t size, const std::nothrow_t&) throw()
{
	return operator new(size);
}

void* operator new[](size_t size, const std::nothrow_t&) throw()
{
	return operator new[](size);
}

void operator delete(void* pointer)
{
#ifdef M_DEBUG_NEW_REQUIRE_THREAD_SAFE
    ::pilo::core::threading::mutex_locker<::pilo::core::threading::recursive_mutex> guard(__pilo_var_stc_heap_leak_checker_mutex);
#endif
	if (pointer == NULL)
		return;
	size_t hash_index = DEBUG_NEW_HASH(pointer);
	new_ptr_list_t* ptr = __pilo_var_heap_leak_hash_container[hash_index];
	new_ptr_list_t* ptr_last = NULL;

#if defined (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_DELETE)
    LARGE_INTEGER freq;
    LARGE_INTEGER pcounter;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&pcounter);
    double sec = (double)pcounter.QuadPart / (double)freq.QuadPart;
    long long cts = (long long)sec * 1000 * 1000;
#endif

#if defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_DELETE) 
    unsigned int ctid = ::pilo::core::threading::basic_thread::id();
#endif

	while (ptr)
	{
		if ((char*)ptr + sizeof(new_ptr_list_t) == pointer)
		{
            if (__pilo_var_heap_leak_verbose)
            {
                if (M_DEBUG_NEW_REPORT_OUTPUT_STDOUT == __pilo_var_heap_leak_output_mode)
                {
#if defined(M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_DELETE) && defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_DELETE)
                    fprintf(stdout, "del: - addr @ 0x%p (s=%u) t=%lld(%lld) dtid=%u\n", pointer, (unsigned int) ptr->size,cts-ptr->ats,cts,ctid);
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_DELETE)
                    fprintf(stdout, "del: - addr @ 0x%p (s=%u) t=%lld(%lld)\n", pointer,(unsigned int)  ptr->size, cts - ptr->ats, cts);
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_DELETE)
                    fprintf(stdout, "del: - addr @ 0x%p (s=%u) dtid=%u\n", pointer, (unsigned int)ptr->size, ctid);
#else
                    fprintf(stdout, "del: - addr @ 0x%p (s=%u)\n", pointer, (unsigned int) ptr->size);
#endif
                    
                }
                else if (M_DEBUG_NEW_REPORT_OUTPUT_STDERR == __pilo_var_heap_leak_output_mode)
                {
#if defined(M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_DELETE) && defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_DELETE)
                    fprintf(stderr, "del: - addr @ 0x%p (s=%u) t=%lld(%lld) dtid=%u\n", pointer, (unsigned int) ptr->size, cts - ptr->ats, cts, ctid);
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_DELETE)
                    fprintf(stderr, "del: - addr @ 0x%p (s=%u) t=%lld(%lld)\n", pointer, (unsigned int) ptr->size, cts - ptr->ats, cts);
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_DELETE)
                    fprintf(stderr, "del: - addr @ 0x%p (s=%u) dtid=%u\n", pointer, (unsigned int)ptr->size, ctid);
#else
                    fprintf(stderr, "del: - addr @ 0x%p (s=%u)\n", pointer, (unsigned int) ptr->size);
#endif
                }
                else if (M_DEBUG_NEW_REPORT_OUTPUT_FILE == __pilo_var_heap_leak_output_mode)
                {
                    if (NULL != __pilo_var_heap_leak_output_filep)
                    {
#if defined(M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_DELETE) && defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_DELETE)
                        fprintf(__pilo_var_heap_leak_output_filep, "del: - addr @ 0x%p (s=%u) t=%lld(%lld) dtid=%u\n", pointer,(unsigned int)  ptr->size, cts - ptr->ats, cts, ctid);
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_DELETE)
                        fprintf(__pilo_var_heap_leak_output_filep, "del: - addr @ 0x%p (s=%u) t=%lld(%lld)\n", pointer, (unsigned int) ptr->size, cts - ptr->ats, cts);
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_DELETE)
                        fprintf(__pilo_var_heap_leak_output_filep, "del: - addr @ 0x%p (s=%u) dtid=%u\n", pointer, (unsigned int)ptr->size, ctid);
#else
                        fprintf(__pilo_var_heap_leak_output_filep, "del: - addr @ 0x%p (s=%u)\n", pointer,(unsigned int)  ptr->size);
#endif
                    }                
                }
#ifdef WINDOWS
                else if (M_DEBUG_NEW_REPORT_OUTPUT_VCDBG_CONSOLE == __pilo_var_heap_leak_output_mode)
                {
#if defined(M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_DELETE) && defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_DELETE)
                    __pilo_debug_vcdbg_console_format_output("del: - addr @ 0x%p (s=%u) t=%lld(%lld) dtid=%u\n", pointer,(unsigned int)  ptr->size, cts - ptr->ats, cts, ctid);
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_DELETE)
                    __pilo_debug_vcdbg_console_format_output("del: - addr @ 0x%p (s=%u) t=%lld(%lld)\n", pointer,(unsigned int)  ptr->size, cts - ptr->ats, cts);
#elif defined (M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_DELETE)
                    __pilo_debug_vcdbg_console_format_output("del: - addr @ 0x%p (s=%u) dtid=%u\n", pointer, (unsigned int)ptr->size, ctid);
#else
                    __pilo_debug_vcdbg_console_format_output("del: - addr @ 0x%p (s=%u)\n", pointer,(unsigned int)  ptr->size);
#endif
                }
#endif
            }
				
			if (ptr_last == NULL)
				__pilo_var_heap_leak_hash_container[hash_index] = ptr->next;
			else
				ptr_last->next = ptr->next;
			free(ptr);
			return;
		}
		ptr_last = ptr;
		ptr = ptr->next;
	}

    if (M_DEBUG_NEW_REPORT_OUTPUT_STDOUT == __pilo_var_heap_leak_output_mode)
    {
        fprintf(stdout, "[HeapAllocDbg] [Fatal] delete: invalid pointer addr @ 0x%p\n", pointer);
    }
    else if (M_DEBUG_NEW_REPORT_OUTPUT_STDERR == __pilo_var_heap_leak_output_mode)
    {
        fprintf(stderr, "[HeapAllocDbg] [Fatal] delete: invalid pointer addr @ 0x%p\n", pointer);
    }
    else if (M_DEBUG_NEW_REPORT_OUTPUT_FILE == __pilo_var_heap_leak_output_mode)
    {
        if (NULL != __pilo_var_heap_leak_output_filep)
        {
            fprintf(__pilo_var_heap_leak_output_filep, "[HeapAllocDbg] [Fatal] delete: invalid pointer addr @ 0x%p\n", pointer);
        }
    }
#ifdef WINDOWS
    else if (M_DEBUG_NEW_REPORT_OUTPUT_VCDBG_CONSOLE == __pilo_var_heap_leak_output_mode)
    {
        __pilo_debug_vcdbg_console_format_output("[HeapAllocDbg] [Fatal] delete: invalid pointer addr @ 0x%p\n", pointer);
    }
#endif

	abort();
}

void operator delete[](void* pointer)
{
	operator delete(pointer);
}

// Some older compilers like Borland C++ Compiler 5.5.1 and Digital Mars
// Compiler 8.29 do not support placement delete operators.
// NO_PLACEMENT_DELETE needs to be defined when using such compilers.
// Also note that in that case memory leakage will occur if an exception
// is thrown in the initialization (constructor) of a dynamically
// created object.
#ifndef NO_PLACEMENT_DELETE
void operator delete(void* pointer, const char* file, int line)
{
    if (__pilo_var_heap_leak_verbose)
    {
        if (M_DEBUG_NEW_REPORT_OUTPUT_STDOUT == __pilo_var_heap_leak_output_mode)
        {
            fprintf(stdout, "info: exception thrown on initializing object addr @ 0x%p (%s:%d)\n", pointer, file, line);
        }
        else if (M_DEBUG_NEW_REPORT_OUTPUT_STDERR == __pilo_var_heap_leak_output_mode)
        {
            fprintf(stderr, "info: exception thrown on initializing object addr @ 0x%p (%s:%d)\n", pointer, file, line);
        }
        else if (M_DEBUG_NEW_REPORT_OUTPUT_FILE == __pilo_var_heap_leak_output_mode)
        {
            if (NULL != __pilo_var_heap_leak_output_filep)
            {
                fprintf(__pilo_var_heap_leak_output_filep, "info: exception thrown on initializing object addr @ 0x%p (%s:%d)\n", pointer, file, line);
            }
        }
#ifdef WINDOWS
        else if (M_DEBUG_NEW_REPORT_OUTPUT_VCDBG_CONSOLE == __pilo_var_heap_leak_output_mode)
        {
            __pilo_debug_vcdbg_console_format_output("info: exception thrown on initializing object addr @ 0x%p (%s:%d)\n", pointer, file, line);
        }
#endif
    }
	operator delete(pointer);
}

void operator delete[](void* pointer, const char* file, int line)
{
	operator delete(pointer, file, line);
}

void operator delete(void* pointer, const std::nothrow_t&)
{
	operator delete(pointer, "<Unknown>", 0);
}

void operator delete[](void* pointer, const std::nothrow_t&)
{
	operator delete(pointer, std::nothrow);
}
#endif // NO_PLACEMENT_DELETE

// Proxy class to automatically call check_leaks if new_autocheck_flag is set
class pilo_debug_heap_leak_delator
{
public:
	pilo_debug_heap_leak_delator() {}
	~pilo_debug_heap_leak_delator()
	{
		if (__pilo_var_heap_leak_report_onexit)
		{
			// Check for leakage.
			// If any leaks are found, set new_verbose_flag so that any
			// delete operations in the destruction of global/static
			// objects will display information to compensate for
			// possible false leakage reports.
			if (pilo_debug_heap_check_leaks())
				__pilo_var_heap_leak_verbose = true;
		}

        _pilo_debug_heap_leak_fini_report_file();
	}
};
static pilo_debug_heap_leak_delator __pilo_var_stc_heap_leak_delator;

#undef _CRT_SECURE_NO_WARNINGS

#endif

