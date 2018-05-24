
#ifndef _DEBUG_NEW_H
#define _DEBUG_NEW_H


#define M_DEBUG_NEW_REPORT_OUTPUT_NONE	0
#define M_DEBUG_NEW_REPORT_OUTPUT_STDOUT 1
#define M_DEBUG_NEW_REPORT_OUTPUT_STDERR 2
#define M_DEBUG_NEW_REPORT_OUTPUT_FILE	3
#define M_DEBUG_NEW_REPORT_OUTPUT_VCDBG_CONSOLE 4

#include <new>

/* Prototypes */
void pilo_debug_heap_leak_set_verbose(bool b);
void pilo_debug_heap_leak_set_report_onexit(bool b);
void pilo_debug_heap_leak_set_report_output_mode(char mode, const char* arg);

bool pilo_debug_heap_check_leaks();

void* operator new(size_t size, const char* file, int line);
void* operator new[](size_t size, const char* file, int line);
#ifndef NO_PLACEMENT_DELETE
    void operator delete(void* pointer, const char* file, int line);
    void operator delete[](void* pointer, const char* file, int line);
#endif // NO_PLACEMENT_DELETE
void operator delete[](void*);	// MSVC 6 requires this declaration

/* Macros */
#ifndef DEBUG_NEW_NO_NEW_REDEFINITION
#   define new DEBUG_NEW
#   define DEBUG_NEW new(__FILE__, __LINE__)
#   define debug_new new
#else
#   define debug_new new(__FILE__, __LINE__)
#endif // DEBUG_NEW_NO_NEW_REDEFINITION

#ifdef DEBUG_NEW_EMULATE_MALLOC
#   include <stdlib.h>
#   define malloc(s) ((void*)(debug_new char[s]))
#   define free(p) delete[] (char*)(p)
#endif // DEBUG_NEW_EMULATE_MALLOC


#endif // _DEBUG_NEW_H
