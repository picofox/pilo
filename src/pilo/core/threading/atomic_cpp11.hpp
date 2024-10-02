#pragma once
#include "../../pilo.hpp"
#include <atomic>

namespace pilo {
    namespace core {
        namespace threading
        {
			// clang-format off
			inline void signalFenceConsume() { std::atomic_signal_fence(std::memory_order_acquire); }
			inline void signalFenceAcquire() { std::atomic_signal_fence(std::memory_order_acquire); }
			inline void signalFenceRelease() { std::atomic_signal_fence(std::memory_order_release); }
			inline void signalFenceSeqCst() { std::atomic_signal_fence(std::memory_order_seq_cst); }

			inline void threadFenceConsume() { std::atomic_thread_fence(std::memory_order_acquire); }
			inline void threadFenceAcquire() { std::atomic_thread_fence(std::memory_order_acquire); }
			inline void threadFenceRelease() { std::atomic_thread_fence(std::memory_order_release); }
			inline void threadFenceSeqCst() { std::atomic_thread_fence(std::memory_order_seq_cst); }
			// clang-format on

			enum memory_order {
				relaxed = std::memory_order_relaxed,
				consume = std::memory_order_consume,
				acquire = std::memory_order_acquire,
				release = std::memory_order_release,
				consume_release = std::memory_order_acq_rel,
				acquire_release = std::memory_order_acq_rel,
			};

			template <typename T>
			class atomic_cpp11 : protected std::atomic<T> {
			private:
				// Hide operator=
				T operator=(T value);

			public:
				atomic_cpp11() {
				}
				atomic_cpp11(T value) : std::atomic<T>(value) {
				}
				atomic_cpp11(const atomic_cpp11& other) : std::atomic<T>(other.loadNonatomic()) {
				}
				T load_nonatomic() const {
					return std::atomic<T>::load(std::memory_order_relaxed);
				}
				T load(memory_order memoryOrder) const {
					return std::atomic<T>::load((std::memory_order)memoryOrder);
				}
				void store_nonatomic(T value) {
					return std::atomic<T>::store(value, std::memory_order_relaxed);
				}
				void store(T value, memory_order memoryOrder) {
					return std::atomic<T>::store(value, (std::memory_order)memoryOrder);
				}
				T compare_exchange(T expected, T desired, memory_order memoryOrder) {
					std::atomic<T>::compare_exchange_strong(expected, desired, (std::memory_order)memoryOrder);
					return expected; // modified by reference by compare_exchange_strong
				}
				bool compare_exchange_strong(T& expected, T desired, memory_order memoryOrder) {
					return std::atomic<T>::compare_exchange_strong(expected, desired, (std::memory_order)memoryOrder);
				}
				bool compare_exchange_weak(T& expected, T desired, memory_order success, memory_order failure) {
					return std::atomic<T>::compare_exchange_weak(expected, desired, (std::memory_order)success, (std::memory_order)failure);
				}
				T exchange(T desired, memory_order memoryOrder) {
					return std::atomic<T>::exchange(desired, (std::memory_order)memoryOrder);
				}
				T fetch_add(T operand, memory_order memoryOrder) {
					return std::atomic<T>::fetch_add(operand, (std::memory_order)memoryOrder);
				}
				T fetch_sub(T operand, memory_order memoryOrder) {
					return std::atomic<T>::fetch_sub(operand, (std::memory_order)memoryOrder);
				}
				T fetch_and(T operand, memory_order memoryOrder) {
					return std::atomic<T>::fetch_and(operand, (std::memory_order)memoryOrder);
				}
				T fetch_or(T operand, memory_order memoryOrder) {
					return std::atomic<T>::fetch_or(operand, (std::memory_order)memoryOrder);
				}
			};
        }
    }
}
