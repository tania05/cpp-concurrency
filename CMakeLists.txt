set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(cpp_containers LANGUAGES CXX)


add_executable(test_queue app/test_queue.cpp include/ra/queue.hpp)
add_executable(test_thread_pool app/test_thread_pool.cpp include/ra/thread_pool.hpp lib/thread_pool.cpp)
#add_executable(test_julia_set app/test_julia_set.cpp include/ra/julia_set.hpp)

target_include_directories(test_queue PUBLIC include "${CMAKE_CURRENT_BINARY_DIR}/include")
target_include_directories(test_thread_pool PUBLIC include "${CMAKE_CURRENT_BINARY_DIR}/include")
#target_include_directories(test_julia_set PUBLIC include "${CMAKE_CURRENT_BINARY_DIR}/include")

option(ENABLE_ASAN "Initial Address Sanitizer" false)
if (ENABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()

option(ENABLE_TSAN "Initial Thread Sanitizer" false)
if (ENABLE_TSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
endif()

option(ENABLE_UBSAN "Initial Undefined Behaviour Sanitizer" false)
if (ENABLE_UBSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
endif()

option(ENABLE_LSAN "Initial Leak Sanitizer" false)
if (ENABLE_LSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=leak")
endif()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -pthread")