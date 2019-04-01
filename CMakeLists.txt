cmake_minimum_required(VERSION 3.0)

project(esearch)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2 -pipe -march=x86-64 -std=c++11")
# set(CMAKE_VERBOSE_MAKEFILE TRUE)
add_compile_options(-Wall -Wextra -O2 -pipe -march=x86-64 -std=c99)

set(
    SOURCES 
        "src/main.c"
        "libs/misc.c"
        "libs/misc.h"   
        "libs/formatter.c"
        "libs/formatter.h"      
)

# add_subdirectory (libs)
include_directories(libs)

add_executable(esearch ${SOURCES})

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
