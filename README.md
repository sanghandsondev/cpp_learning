# C++ Learning

## C++14
1. [Generic Lambdas](cpp_14/01_generic_lambdas.md) - auto parameters trong lambda
2. [std::make_unique](cpp_14/02_make_unique.md) - tạo unique_ptr an toàn
3. [Move Capture Lambdas](cpp_14/03_move_capture_lambdas.md) - move ownership vào lambda/thread
4. [std::shared_timed_mutex](cpp_14/04_shared_timed_mutex.md) - reader-writer lock
5. [std::exchange](cpp_14/05_std_exchange.md) - swap-and-return, move semantics

## C++17
1. [Structured Bindings](cpp_17/01_structured_bindings.md) - auto [a, b] = pair
2. [std::optional](cpp_17/02_optional.md) - nullable value type
3. [shared_mutex & scoped_lock](cpp_17/03_shared_mutex_scoped_lock.md) - reader-writer lock + deadlock-free multi-lock
4. [Parallel Algorithms](cpp_17/04_parallel_algorithms.md) - multi-core STL (execution::par)
5. [std::variant](cpp_17/05_variant.md) - type-safe union, state machine

## C++20
1. [Concepts](cpp_20/01_concepts.md) - template constraints
2. [Ranges](cpp_20/02_ranges.md) - lazy pipelines (filter/transform)
3. [Coroutines](cpp_20/03_coroutines.md) - co_await, co_yield, generators
4. [std::jthread & stop_token](cpp_20/04_jthread_stop_token.md) - auto-join thread + cancellation
5. [latch, barrier, semaphore](cpp_20/05_synchronization_primitives.md) - synchronization primitives
