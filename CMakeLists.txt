cmake_minimum_required(VERSION 3.12)
project(google_books)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/headers)

add_executable(book
        main.cpp
        class.cpp
        class.h)
