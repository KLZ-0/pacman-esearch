cmake_minimum_required(VERSION 3.15)
project(esearch C)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_compile_options(-Wall -pedantic)

set(CMAKE_C_STANDARD 11)

include_directories(./include/)

add_executable(esearch src/main.c src/misc.c)

#enable_testing()
#
## Basic functionality tests
#add_test(noargs esearch)
#add_test(version esearch -v)
#add_test(help esearch -h)
#
## Option tests
#add_test(search esearch plasma-desktop)
#add_test(instonly esearch -I plasma)
#add_test(notinst esearch -N plasma)
#add_test(nocolor esearch -n plasma-desktop)
#add_test(nowarndb esearch -w plasma-desktop)
#
## merged option tests
#add_test(mergedsearchonly esearch -enI gcc)
#add_test(mergedsearchmixed esearch -n --notinst plasma)

#install(TARGETS esearch RUNTIME DESTINATION bin)
#install(PROGRAMS "eupdatedb.sh" DESTINATION bin RENAME "eupdatedb")
