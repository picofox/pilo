#pragma once
#include "core/coredefs.hpp"

#ifndef WINDOWS


extern volatile pilo::u64_t __glb_tick_count;
extern volatile int __glb_tick_count_simulative_thread_started;
pilo::u64_t __GetMillisecondTickCount();
#define PiloGetTickCount64() (__glb_tick_count_simulative_thread_started ? __glb_tick_count : __GetMillisecondTickCount()) 
#define PiloGetTickCount32() ((unsigned int) PiloGetTickCount64())

#endif

