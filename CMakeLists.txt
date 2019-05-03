cmake_minimum_required(VERSION 3.0)

project(esearch)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2 -pipe -march=x86-64 -std=c++11")
# set(CMAKE_VERBOSE_MAKEFILE TRUE)

set(
    SOURCES 
        "src/main.cpp"
        "src/Query.cpp"
        "src/Query.h"        
        "src/Database.cpp"
        "src/Database.h"
)

add_executable(esearch ${SOURCES})

if (UNIX)
    target_compile_options(esearch PUBLIC "-Wall -Wextra -O2 -pipe -std=c++11")
endif (UNIX)

if (MSVC)
    target_compile_options(esearch PUBLIC "/MD /O2 /Ob2 /std:c++11")
endif (MSVC)

enable_testing()

# Basic functionality tests
add_test(noargs esearch)
add_test(version esearch -v)
add_test(help esearch -h)

# Option tests
add_test(search esearch plasma-desktop)
add_test(instonly esearch -I plasma)
add_test(notinst esearch -N plasma)
add_test(nocolor esearch -n plasma-desktop)
add_test(searchdesc esearch -S plasma-desktop)

# merged option tests
add_test(mergedsearchonly esearch -enI gcc)
add_test(mergedsearchmixed esearch -n --notinst plasma)

install(TARGETS esearch RUNTIME DESTINATION bin)
install(PROGRAMS "eupdatedb.sh" DESTINATION bin RENAME "eupdatedb")
