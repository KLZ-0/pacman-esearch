cmake_minimum_required(VERSION 3.10)
project(esearch C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS_DEBUG "-g -Wall -pedantic")
set(CMAKE_C_FLAGS_RELEASE "-O2")

include_directories(./include/)

add_executable(esearch src/esearch.c src/error.c src/util.c src/database.c)
