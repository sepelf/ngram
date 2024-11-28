cmake_minimum_required(VERSION 3.10)

project(ngram)

set(CMAKE_CXX_STANDARD 11)

if (WITH_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

add_subdirectory(src)

if (WITH_TEST) 
    enable_testing()
    add_subdirectory(test)
endif()

