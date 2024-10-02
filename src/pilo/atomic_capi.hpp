#pragma once
#include "platform.hpp"

#include PMS_PFD_ATOMIC_API_H

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum {
		ePMOR_Relaxed,
		ePMOR_Acquire,
		ePMOR_Release,
		ePMOR_AcqRel
	} pilo_memory_order_t;

	//--------------------------------------------------------------
	//  Wrappers for 8-bit operations with built-in ordering constraints
	//--------------------------------------------------------------
	PMC_INLINE uint8_t pilo_atomic_load_8(const pilo_atomic8_t* object, pilo_memory_order_t memoryOrder) {
		uint8_t result = pilo_atomic_load_8_relaxed(object);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel) // a little forgiving
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE int8_t pilo_atomic_load_8_(const int8_t* object, pilo_memory_order_t memoryOrder) {
		int8_t result = pilo_atomic_load_8_relaxed_(object);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel) // a little forgiving
			pilo_thread_fence_acquire();
		return result;
	}

	PMC_INLINE void pilo_atomic_store_8(pilo_atomic8_t* object, uint8_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel) // a little forgiving
			pilo_thread_fence_release();
		pilo_atomic_store_8_relaxed(object, desired);
	}
	PMC_INLINE void pilo_atomic_store_8_(int8_t* object, uint8_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel) // a little forgiving
			pilo_thread_fence_release();
		pilo_atomic_store_8_relaxed_(object, desired);
	}

	PMC_INLINE uint8_t pilo_atomic_compare_exchange_8(pilo_atomic8_t* object, uint8_t expected, uint8_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		uint8_t result = pilo_atomic_compare_exchange_8_relaxed(object, expected, desired);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE uint8_t pilo_atomic_compare_exchange_8_(int8_t* object, uint8_t expected, uint8_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		int8_t result = pilo_atomic_compare_exchange_8_relaxed_(object, expected, desired);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}

	PMC_INLINE intptr_t pilo_atomic_compare_exchange_weak_8(pilo_atomic8_t* object, uint8_t* expected, uint8_t desired, int success, int failure) {
		if ((success == ePMOR_Release || success == ePMOR_AcqRel) ||
			(failure == ePMOR_Release || failure == ePMOR_AcqRel))
			pilo_thread_fence_release();
		intptr_t result = pilo_atomic_compare_exchange_weak_8_relaxed(object, expected, desired);
		if (result) {
			if (success == ePMOR_Acquire || success == ePMOR_AcqRel)
				pilo_thread_fence_acquire();
		}
		else {
			if (failure == ePMOR_Acquire || failure == ePMOR_AcqRel)
				pilo_thread_fence_acquire();
		}
		return result;
	}
	PMC_INLINE intptr_t pilo_atomic_compare_exchange_weak_8_(int8_t* object, int8_t* expected, int8_t desired, int success, int failure) {
		if ((success == ePMOR_Release || success == ePMOR_AcqRel) ||
			(failure == ePMOR_Release || failure == ePMOR_AcqRel))
			pilo_thread_fence_release();
		intptr_t result = pilo_atomic_compare_exchange_weak_8_relaxed_(object, expected, desired);
		if (result) {
			if (success == ePMOR_Acquire || success == ePMOR_AcqRel)
				pilo_thread_fence_acquire();
		}
		else {
			if (failure == ePMOR_Acquire || failure == ePMOR_AcqRel)
				pilo_thread_fence_acquire();
		}
		return result;
	}

	PMC_INLINE uint8_t pilo_atomic_exchange_8(pilo_atomic8_t* object, uint8_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		uint8_t result = pilo_atomic_exchange_8_relaxed(object, desired);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE int8_t pilo_atomic_exchange_8_(int8_t* object, uint8_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		int8_t result = pilo_atomic_exchange_8_relaxed_(object, desired);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}


	PMC_INLINE uint8_t pilo_atomic_fetch_add_8(pilo_atomic8_t* object, int8_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		uint8_t result = pilo_atomic_fetch_add_8_relaxed(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE int8_t pilo_atomic_fetch_add_8_(int8_t* object, int8_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		int8_t result = pilo_atomic_fetch_add_8_relaxed_(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}

	PMC_INLINE uint8_t pilo_atomic_fetch_and_8(pilo_atomic8_t* object, uint8_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		uint8_t result = pilo_atomic_fetch_and_8_relaxed(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE uint8_t pilo_atomic_fetch_and_8_(int8_t* object, int8_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		int8_t result = pilo_atomic_fetch_and_8_relaxed_(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}

	PMC_INLINE uint8_t pilo_atomic_fetch_or_8(pilo_atomic8_t* object, uint8_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		uint8_t result = pilo_atomic_fetch_or_8_relaxed(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE int8_t pilo_atomic_fetch_or_8_(int8_t* object, int8_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		int8_t result = pilo_atomic_fetch_or_8_relaxed_(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}

	//--------------------------------------------------------------
	//  Wrappers for 16-bit operations with built-in ordering constraints
	//--------------------------------------------------------------
	PMC_INLINE uint16_t pilo_atomic_load_16(const pilo_atomic16_t* object, pilo_memory_order_t memoryOrder) {
		uint16_t result = pilo_atomic_load_16_relaxed(object);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel) // a little forgiving
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE int16_t pilo_atomic_load_16_(const int16_t* object, pilo_memory_order_t memoryOrder) {
		int16_t result = pilo_atomic_load_16_relaxed_(object);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel) // a little forgiving
			pilo_thread_fence_acquire();
		return result;
	}

	PMC_INLINE void pilo_atomic_store_16(pilo_atomic16_t* object, uint16_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel) // a little forgiving
			pilo_thread_fence_release();
		pilo_atomic_store_16_relaxed(object, desired);
	}
	PMC_INLINE void pilo_atomic_store_16_(int16_t* object, int16_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel) // a little forgiving
			pilo_thread_fence_release();
		pilo_atomic_store_16_relaxed_(object, desired);
	}

	PMC_INLINE uint16_t pilo_atomic_compare_exchange_16(pilo_atomic16_t* object, uint16_t expected, uint16_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		uint16_t result = pilo_atomic_compare_exchange_16_relaxed(object, expected, desired);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE int16_t pilo_atomic_compare_exchange_16_(int16_t* object, int16_t expected, int16_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		int16_t result = pilo_atomic_compare_exchange_16_relaxed_(object, expected, desired);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}

	PMC_INLINE intptr_t pilo_atomic_compare_exchange_weak_16(pilo_atomic16_t* object, uint16_t* expected, uint16_t desired, int success, int failure) {
		if ((success == ePMOR_Release || success == ePMOR_AcqRel) ||
			(failure == ePMOR_Release || failure == ePMOR_AcqRel))
			pilo_thread_fence_release();
		intptr_t result = pilo_atomic_compare_exchange_weak_16_relaxed(object, expected, desired);
		if (result) {
			if (success == ePMOR_Acquire || success == ePMOR_AcqRel)
				pilo_thread_fence_acquire();
		}
		else {
			if (failure == ePMOR_Acquire || failure == ePMOR_AcqRel)
				pilo_thread_fence_acquire();
		}
		return result;
	}
	PMC_INLINE intptr_t pilo_atomic_compare_exchange_weak_16_(int16_t* object, int16_t* expected, int16_t desired, int success, int failure) {
		if ((success == ePMOR_Release || success == ePMOR_AcqRel) ||
			(failure == ePMOR_Release || failure == ePMOR_AcqRel))
			pilo_thread_fence_release();
		intptr_t result = pilo_atomic_compare_exchange_weak_16_relaxed_(object, expected, desired);
		if (result) {
			if (success == ePMOR_Acquire || success == ePMOR_AcqRel)
				pilo_thread_fence_acquire();
		}
		else {
			if (failure == ePMOR_Acquire || failure == ePMOR_AcqRel)
				pilo_thread_fence_acquire();
		}
		return result;
	}

	PMC_INLINE uint16_t pilo_atomic_exchange_16(pilo_atomic16_t* object, uint16_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		uint16_t result = pilo_atomic_exchange_16_relaxed(object, desired);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE int16_t pilo_atomic_exchange_16_(int16_t* object, uint16_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		int16_t result = pilo_atomic_exchange_16_relaxed_(object, desired);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}

	PMC_INLINE uint16_t pilo_atomic_fetch_add_16(pilo_atomic16_t* object, int16_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		uint16_t result = pilo_atomic_fetch_add_16_relaxed(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE int16_t pilo_atomic_fetch_add_16_(int16_t* object, int16_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		int16_t result = pilo_atomic_fetch_add_16_relaxed_(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}

	PMC_INLINE uint16_t pilo_atomic_fetch_and_16(pilo_atomic16_t* object, uint16_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		uint16_t result = pilo_atomic_fetch_and_16_relaxed(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE int16_t pilo_atomic_fetch_and_16_(int16_t* object, uint16_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		int16_t result = pilo_atomic_fetch_and_16_relaxed_(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}

	PMC_INLINE uint16_t pilo_atomic_fetch_or_16(pilo_atomic16_t* object, uint16_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		uint16_t result = pilo_atomic_fetch_or_16_relaxed(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE uint16_t pilo_atomic_fetch_or_16_(int16_t* object, uint16_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		int16_t result = pilo_atomic_fetch_or_16_relaxed_(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}

	//--------------------------------------------------------------
	//  Wrappers for 32-bit operations with built-in ordering constraints
	//--------------------------------------------------------------
	PMC_INLINE uint32_t pilo_atomic_load_32(const pilo_atomic32_t* object, pilo_memory_order_t memoryOrder) {
		uint32_t result = pilo_atomic_load_32_relaxed(object);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel) // a little forgiving
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE uint32_t pilo_atomic_load_32_(const int32_t* object, pilo_memory_order_t memoryOrder) {
		int32_t result = pilo_atomic_load_32_relaxed_(object);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel) // a little forgiving
			pilo_thread_fence_acquire();
		return result;
	}

	PMC_INLINE void pilo_atomic_store_32(pilo_atomic32_t* object, uint32_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel) // a little forgiving
			pilo_thread_fence_release();
		pilo_atomic_store_32_relaxed(object, desired);
	}
	PMC_INLINE void pilo_atomic_store_32_(int32_t* object, uint32_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel) // a little forgiving
			pilo_thread_fence_release();
		pilo_atomic_store_32_relaxed_(object, desired);
	}


	PMC_INLINE uint32_t pilo_atomic_compare_exchange_32(pilo_atomic32_t* object, uint32_t expected, uint32_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		uint32_t result = pilo_atomic_compare_exchange_32_relaxed(object, expected, desired);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE int32_t pilo_atomic_compare_exchange_32_(int32_t* object, int32_t expected, int32_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		int32_t result = pilo_atomic_compare_exchange_32_relaxed_(object, expected, desired);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}

	PMC_INLINE intptr_t pilo_atomic_compare_exchange_weak_32(pilo_atomic32_t* object, uint32_t* expected, uint32_t desired, int success, int failure) {
		if ((success == ePMOR_Release || success == ePMOR_AcqRel) ||
			(failure == ePMOR_Release || failure == ePMOR_AcqRel))
			pilo_thread_fence_release();
		intptr_t result = pilo_atomic_compare_exchange_weak_32_relaxed(object, expected, desired);
		if (result) {
			if (success == ePMOR_Acquire || success == ePMOR_AcqRel)
				pilo_thread_fence_acquire();
		}
		else {
			if (failure == ePMOR_Acquire || failure == ePMOR_AcqRel)
				pilo_thread_fence_acquire();
		}
		return result;
	}
	PMC_INLINE intptr_t pilo_atomic_compare_exchange_weak_32_(int32_t* object, int32_t* expected, int32_t desired, int success, int failure) {
		if ((success == ePMOR_Release || success == ePMOR_AcqRel) ||
			(failure == ePMOR_Release || failure == ePMOR_AcqRel))
			pilo_thread_fence_release();
		intptr_t result = pilo_atomic_compare_exchange_weak_32_relaxed_(object, expected, desired);
		if (result) {
			if (success == ePMOR_Acquire || success == ePMOR_AcqRel)
				pilo_thread_fence_acquire();
		}
		else {
			if (failure == ePMOR_Acquire || failure == ePMOR_AcqRel)
				pilo_thread_fence_acquire();
		}
		return result;
	}

	PMC_INLINE uint32_t pilo_atomic_exchange_32(pilo_atomic32_t* object, uint32_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		uint32_t result = pilo_atomic_exchange_32_relaxed(object, desired);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE int32_t pilo_atomic_exchange_32_(int32_t* object, int32_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		int32_t result = pilo_atomic_exchange_32_relaxed_(object, desired);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}

	PMC_INLINE uint32_t pilo_atomic_fetch_add_32(pilo_atomic32_t* object, int32_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		uint32_t result = pilo_atomic_fetch_add_32_relaxed(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE int32_t pilo_atomic_fetch_add_32_(int32_t* object, int32_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		int32_t result = pilo_atomic_fetch_add_32_relaxed_(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}

	PMC_INLINE uint32_t pilo_atomic_fetch_and_32(pilo_atomic32_t* object, uint32_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		uint32_t result = pilo_atomic_fetch_and_32_relaxed(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE int32_t pilo_atomic_fetch_and_32_(int32_t* object, int32_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		int32_t result = pilo_atomic_fetch_and_32_relaxed_(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}

	PMC_INLINE uint32_t pilo_atomic_fetch_or_32(pilo_atomic32_t* object, uint32_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		uint32_t result = pilo_atomic_fetch_or_32_relaxed(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE int32_t pilo_atomic_fetch_or_32_(int32_t* object, int32_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		int32_t result = pilo_atomic_fetch_or_32_relaxed_(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}

	//--------------------------------------------------------------
	//  Wrappers for 64-bit operations with built-in ordering constraints
	//--------------------------------------------------------------
	PMC_INLINE uint64_t pilo_atomic_load_64(const pilo_atomic64_t* object, pilo_memory_order_t memoryOrder) {
		uint64_t result = pilo_atomic_load_64_relaxed(object);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel) // a little forgiving
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE int64_t pilo_atomic_load_64_(const int64_t* object, pilo_memory_order_t memoryOrder) {
		int64_t result = pilo_atomic_load_64_relaxed_(object);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel) // a little forgiving
			pilo_thread_fence_acquire();
		return result;
	}

	PMC_INLINE void pilo_atomic_store_64(pilo_atomic64_t* object, uint64_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel) // a little forgiving
			pilo_thread_fence_release();
		pilo_atomic_store_64_relaxed(object, desired);
	}
	PMC_INLINE void pilo_atomic_store_64_(int64_t* object, int64_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel) // a little forgiving
			pilo_thread_fence_release();
		pilo_atomic_store_64_relaxed_(object, desired);
	}

	PMC_INLINE uint64_t pilo_atomic_compare_exchange_64(pilo_atomic64_t* object, uint64_t expected, uint64_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		uint64_t result = pilo_atomic_compare_exchange_64_relaxed(object, expected, desired);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE int64_t pilo_atomic_compare_exchange_64_(int64_t* object, int64_t expected, int64_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		int64_t result = pilo_atomic_compare_exchange_64_relaxed_(object, expected, desired);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}

	PMC_INLINE intptr_t pilo_atomic_compare_exchange_weak_64(pilo_atomic64_t* object, uint64_t* expected, uint64_t desired, int success, int failure) {
		if ((success == ePMOR_Release || success == ePMOR_AcqRel) ||
			(failure == ePMOR_Release || failure == ePMOR_AcqRel))
			pilo_thread_fence_release();
		intptr_t result = pilo_compare_exchange_weak_64_relaxed(object, expected, desired);
		if (result) {
			if (success == ePMOR_Acquire || success == ePMOR_AcqRel)
				pilo_thread_fence_acquire();
		}
		else {
			if (failure == ePMOR_Acquire || failure == ePMOR_AcqRel)
				pilo_thread_fence_acquire();
		}
		return result;
	}
	PMC_INLINE intptr_t pilo_atomic_compare_exchange_weak_64_(int64_t* object, int64_t* expected, int64_t desired, int success, int failure) {
		if ((success == ePMOR_Release || success == ePMOR_AcqRel) ||
			(failure == ePMOR_Release || failure == ePMOR_AcqRel))
			pilo_thread_fence_release();
		intptr_t result = pilo_compare_exchange_weak_64_relaxed_(object, expected, desired);
		if (result) {
			if (success == ePMOR_Acquire || success == ePMOR_AcqRel)
				pilo_thread_fence_acquire();
		}
		else {
			if (failure == ePMOR_Acquire || failure == ePMOR_AcqRel)
				pilo_thread_fence_acquire();
		}
		return result;
	}

	PMC_INLINE uint64_t pilo_atomic_exchange_64(pilo_atomic64_t* object, uint64_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		uint64_t result = pilo_atomic_exchange_64_relaxed(object, desired);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE int64_t pilo_atomic_exchange_64_(int64_t* object, int64_t desired, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		int64_t result = pilo_atomic_exchange_64_relaxed_(object, desired);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}

	PMC_INLINE uint64_t pilo_atomic_fetch_add_64(pilo_atomic64_t* object, int64_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		uint64_t result = pilo_atomic_fetch_add_64_relaxed(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE int64_t pilo_atomic_fetch_add_64_(int64_t* object, int64_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		int64_t result = pilo_atomic_fetch_add_64_relaxed_(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}

	PMC_INLINE uint64_t pilo_atomic_fetch_and_64(pilo_atomic64_t* object, uint64_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		uint64_t result = pilo_atomic_fetch_and_64_relaxed(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE int64_t pilo_atomic_fetch_and_64_(int64_t* object, int64_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		int64_t result = pilo_atomic_fetch_and_64_relaxed_(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}

	PMC_INLINE uint64_t pilo_atomic_fetch_or_64(pilo_atomic64_t* object, uint64_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		uint64_t result = pilo_atomic_fetch_or_64_relaxed(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}
	PMC_INLINE int64_t pilo_atomic_fetch_or_64_(int64_t* object, int64_t operand, pilo_memory_order_t memoryOrder) {
		if (memoryOrder == ePMOR_Release || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_release();
		int64_t result = pilo_atomic_fetch_or_64_relaxed_(object, operand);
		if (memoryOrder == ePMOR_Acquire || memoryOrder == ePMOR_AcqRel)
			pilo_thread_fence_acquire();
		return result;
	}


	//--------------------------------------------------------------
	//  Pointer-sized atomic operations
	//--------------------------------------------------------------
#if PMI_PFD_PTR_SIZE == 4
	PMC_INLINE void* pilo_atomic_load_ptr_relaxed(const pilo_atomic_ptr_t* object) {
		return (void*)pilo_atomic_load_32_relaxed((const pilo_atomic32_t*)object);
	}
	PMC_INLINE void pilo_atomic_store_ptr_relaxed(pilo_atomic_ptr_t* object, void* desired) {
		pilo_atomic_store_32_relaxed((pilo_atomic32_t*)object, (size_t)desired);
	}
	PMC_INLINE void* pilo_atomic_compare_exchange_ptr_relaxed(pilo_atomic_ptr_t* object, void* expected, void* desired) {
		return (void*)pilo_atomic_compare_exchange_32_relaxed((pilo_atomic32_t*)object, (size_t)expected, (size_t)desired);
	}
	PMC_INLINE intptr_t pilo_atomic_compare_exchange_weak_ptr_relaxed(pilo_atomic_ptr_t* object, void** expected, void* desired) {
		return pilo_atomic_compare_exchange_weak_32_relaxed((pilo_atomic32_t*)object, (uint32_t*)expected, (size_t)desired);
	}
	PMC_INLINE void* pilo_atomic_exchange_ptr_relaxed(pilo_atomic_ptr_t* object, void* desired) {
		return (void*)pilo_atomic_exchange_32_relaxed((pilo_atomic32_t*)object, (size_t)desired);
	}
	PMC_INLINE void* pilo_atomic_fetch_add_ptr_relaxed(pilo_atomic_ptr_t* object, ptrdiff_t operand) {
		return (void*)pilo_atomic_fetch_add_32_relaxed((pilo_atomic32_t*)object, operand);
	}
	PMC_INLINE void* pilo_atomic_fetch_and_ptr_relaxed(pilo_atomic_ptr_t* object, size_t operand) {
		return (void*)pilo_atomic_fetch_and_32_relaxed((pilo_atomic32_t*)object, operand);
	}
	PMC_INLINE void* pilo_atomic_fetch_or_ptr_relaxed(pilo_atomic_ptr_t* object, size_t operand) {
		return (void*)pilo_atomic_fetch_or_32_relaxed((pilo_atomic32_t*)object, operand);
	}
	PMC_INLINE void* pilo_atomic_load_ptr(const pilo_atomic_ptr_t* object, pilo_memory_order_t memoryOrder) {
		return (void*)pilo_atomic_load_32((const pilo_atomic32_t*)object, memoryOrder);
	}
	PMC_INLINE void pilo_atomic_store_ptr(pilo_atomic_ptr_t* object, void* desired, pilo_memory_order_t memoryOrder) {
		pilo_atomic_store_32((pilo_atomic32_t*)object, (size_t)desired, memoryOrder);
	}
	PMC_INLINE void* pilo_atomic_compare_exchange_ptr(pilo_atomic_ptr_t* object, void* expected, void* desired, pilo_memory_order_t memoryOrder) {
		return (void*)pilo_atomic_compare_exchange_32((pilo_atomic32_t*)object, (size_t)expected, (size_t)desired, memoryOrder);
	}
	PMC_INLINE intptr_t pilo_atomic_compare_exchange_weak_ptr(pilo_atomic_ptr_t* object, void** expected, void* desired, int success, int failure) {
		return pilo_atomic_compare_exchange_weak_32((pilo_atomic32_t*)object, (uint32_t*)expected, (size_t)desired, success, failure);
	}
	PMC_INLINE void* pilo_atomic_exchange_ptr(pilo_atomic_ptr_t* object, void* desired, pilo_memory_order_t memoryOrder) {
		return (void*)pilo_atomic_exchange_32((pilo_atomic32_t*)object, (size_t)desired, memoryOrder);
	}
	PMC_INLINE void* pilo_atomic_fetch_add_ptr(pilo_atomic_ptr_t* object, ptrdiff_t operand, pilo_memory_order_t memoryOrder) {
		return (void*)pilo_atomic_fetch_add_32((pilo_atomic32_t*)object, operand, memoryOrder);
	}
	PMC_INLINE void* pilo_atomic_fetch_and_ptr(pilo_atomic_ptr_t* object, size_t operand, pilo_memory_order_t memoryOrder) {
		return (void*)pilo_atomic_fetch_and_32((pilo_atomic32_t*)object, operand, memoryOrder);
	}
	PMC_INLINE void* pilo_atomic_fetch_or_ptr(pilo_atomic_ptr_t* object, size_t operand, pilo_memory_order_t memoryOrder) {
		return (void*)pilo_atomic_fetch_or_32((pilo_atomic32_t*)object, operand, memoryOrder);
	}
#elif PMI_PFD_PTR_SIZE == 8
	PMC_INLINE void* pilo_atomic_load_ptr_relaxed(const pilo_atomic_ptr_t* object) {
		return (void*)pilo_atomic_load_64_relaxed((const pilo_atomic64_t*)object);
	}
	PMC_INLINE void pilo_atomic_store_ptr_relaxed(pilo_atomic_ptr_t* object, void* desired) {
		pilo_atomic_store_64_relaxed((pilo_atomic64_t*)object, (size_t)desired);
	}
	PMC_INLINE void* pilo_atomic_compare_exchange_ptr_relaxed(pilo_atomic_ptr_t* object, void* expected, void* desired) {
		return (void*)pilo_atomic_compare_exchange_64_relaxed((pilo_atomic64_t*)object, (size_t)expected, (size_t)desired);
	}
	PMC_INLINE intptr_t pilo_atomic_compare_exchange_weak_ptr_relaxed(pilo_atomic_ptr_t* object, void** expected, void* desired) {
		return pilo_compare_exchange_weak_64_relaxed((pilo_atomic64_t*)object, (uint64_t*)expected, (size_t)desired);
	}
	PMC_INLINE void* pilo_atomic_exchange_ptr_relaxed(pilo_atomic_ptr_t* object, void* desired) {
		return (void*)pilo_atomic_exchange_64_relaxed((pilo_atomic64_t*)object, (size_t)desired);
	}
	PMC_INLINE void* pilo_atomic_fetch_add_ptr_relaxed(pilo_atomic_ptr_t* object, ptrdiff_t operand) {
		return (void*)pilo_atomic_fetch_add_64_relaxed((pilo_atomic64_t*)object, operand);
	}
	PMC_INLINE void* pilo_atomic_fetch_and_ptr_relaxed(pilo_atomic_ptr_t* object, size_t operand) {
		return (void*)pilo_atomic_fetch_and_64_relaxed((pilo_atomic64_t*)object, operand);
	}
	PMC_INLINE void* pilo_atomic_fetch_or_ptr_relaxed(pilo_atomic_ptr_t* object, size_t operand) {
		return (void*)pilo_atomic_fetch_or_64_relaxed((pilo_atomic64_t*)object, operand);
	}
	PMC_INLINE void* pilo_atomic_load_ptr(const pilo_atomic_ptr_t* object, pilo_memory_order_t memoryOrder) {
		return (void*)pilo_atomic_load_64((const pilo_atomic64_t*)object, memoryOrder);
	}
	PMC_INLINE void pilo_atomic_store_ptr(pilo_atomic_ptr_t* object, void* desired, pilo_memory_order_t memoryOrder) {
		pilo_atomic_store_64((pilo_atomic64_t*)object, (size_t)desired, memoryOrder);
	}
	PMC_INLINE void* pilo_atomic_compare_exchange_ptr(pilo_atomic_ptr_t* object, void* expected, void* desired, pilo_memory_order_t memoryOrder) {
		return (void*)pilo_atomic_compare_exchange_64((pilo_atomic64_t*)object, (size_t)expected, (size_t)desired, memoryOrder);
	}
	PMC_INLINE intptr_t pilo_atomic_compare_exchange_weak_ptr(pilo_atomic_ptr_t* object, void** expected, void* desired, int success, int failure) {
		return pilo_atomic_compare_exchange_weak_64((pilo_atomic64_t*)object, (uint64_t*)expected, (size_t)desired, success, failure);
	}
	PMC_INLINE void* pilo_atomic_exchange_ptr(pilo_atomic_ptr_t* object, void* desired, pilo_memory_order_t memoryOrder) {
		return (void*)pilo_atomic_exchange_64((pilo_atomic64_t*)object, (size_t)desired, memoryOrder);
	}
	PMC_INLINE void* pilo_atomic_fetch_add_ptr(pilo_atomic_ptr_t* object, ptrdiff_t operand, pilo_memory_order_t memoryOrder) {
		return (void*)pilo_atomic_fetch_add_64((pilo_atomic64_t*)object, operand, memoryOrder);
	}
	PMC_INLINE void* pilo_atomic_fetch_and_ptr(pilo_atomic_ptr_t* object, size_t operand, pilo_memory_order_t memoryOrder) {
		return (void*)pilo_atomic_fetch_and_64((pilo_atomic64_t*)object, operand, memoryOrder);
	}
	PMC_INLINE void* pilo_atomic_fetch_or_ptr(pilo_atomic_ptr_t* object, size_t operand, pilo_memory_order_t memoryOrder) {
		return (void*)pilo_atomic_fetch_or_64((pilo_atomic64_t*)object, operand, memoryOrder);
	}
#else
#error "PMI_PFD_PTR_SIZE not set!"
#endif

// clang-format on

#ifdef __cplusplus
} // extern "C"
#endif