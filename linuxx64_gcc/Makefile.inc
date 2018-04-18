#Path of Project root
PILO_CORE_PROJECT_NAME := libpilocore
PILO_CORETEST_PROJECT_NAME := test-libpilocore
PILO_PERFTEST_PROJECT_NAME := perf-libpilocore

PILO_SOLUTION_DIR := $(shell pwd)
PILO_DIR := $(shell dirname $(PILO_SOLUTION_DIR))
PILO_CORE_PROJECT_DIR := $(PILO_DIR)/$(PILO_CORE_PROJECT_NAME)
PILO_TEST_PROJECT_DIR := $(PILO_DIR)/$(PILO_CORETEST_PROJECT_NAME)
PILO_PERFTEST_PROJECT_DIR := $(PILO_DIR)/$(PILO_PERFTEST_PROJECT_NAME)
PILO_OUTPUT_DIR := $(PILO_DIR)/output
PILO_OUTPUT_LIB_DIR := $(PILO_OUTPUT_DIR)/lib
PILO_OUTPUT_INC_DIR := $(PILO_OUTPUT_DIR)/include
PILO_OUTPUT_BIN_DIR := $(PILO_OUTPUT_DIR)/bin

TmpCoreBuildDir = $(PILO_SOLUTION_DIR)/tmp/$(PILO_CORE_PROJECT_NAME)
TmpCoreOutDir = $(TmpCoreBuildDir)/out
CoreSrcDir = $(PILO_CORE_PROJECT_DIR)/pilo

TmpFuncTestBuildDir = $(PILO_SOLUTION_DIR)/tmp/$(PILO_CORETEST_PROJECT_NAME)
TmpFuncTestOutDir = $(TmpFuncTestBuildDir)/out
FuncTestSrcDir =$(PILO_TEST_PROJECT_DIR)/pilo

TmpPerfTestBuildDir = $(PILO_SOLUTION_DIR)/tmp/$(PILO_PERFTEST_PROJECT_NAME)
TmpPerfTestOutDir = $(TmpPerfTestBuildDir)/out
PerfTestSrcDir = $(PILO_PERFTEST_PROJECT_DIR)/pilo

#Arch of Current system


ifeq ($(shell getconf LONG_BIT), 64)    
    PLATFORM_NAME = x64
    PLATFORM_DEF = -D_LINUX  -D__x86_64__

else
    PLATFORM_NAME = x86 
    PLATFORM_DEF = D_LINUX -D__x86__
endif

#Tools
ifdef USE_NATIVE_CC
    CC := $(PILO_SOLUTION_DIR)/tools/gcc-4.1.2/$(PLATFORM_NAME)/bin/g++	
    LD := $(PILO_SOLUTION_DIR)/tools/gcc-4.1.2/$(PLATFORM_NAME)/bin/g++

else
    CC := g++
    LD := g++
endif

AR := ar
RM := rm
CP := cp
MKDIR := mkdir
INSTALL := install

CPPFLAGS := $(PLATFORM_DEF)
LDFLAGS :=

GCC_VER_MAJOR := $(shell $(CC) -dumpversion | cut -f1 -d.)
GCC_VER_MINOR := $(shell $(CC) -dumpversion | cut -f2 -d.)

ifeq ($(GCC_VER_MAJOR), 4)
    ifeq ($(GCC_VER_MINOR), 7)
        CPPFLAGS += -std=c++11
    endif
    ifeq ($(GCC_VER_MINOR), 8)
        CPPFLAGS += -std=c++11
    endif
endif


ifeq ($(ARCH) , x64)
    CPPFLAGS += -m64
else
endif

CPPFLAGS += -c -pthread -Wall -Werror
CPPFLAGS += $(DEFINES)
LDFLAGS +=
SYS_INC_PATH := -I/usr/include
SYS_LIBS := -lpthread -luuid

INCLUDES := $(SYS_INC_PATH) -I$(PILO_CORE_PROJECT_DIR)/pilo
TEST_INCLUDES := $(SYS_INC_PATH) -I$(PILO_OUTPUT_DIR)/include
LIBS := $(SYS_LIBS)
TEST_LIBS := -L$(OUTPUT_LIB_DIR) $(SYS_LIBS)

