set(
        SOURCES
        main.c
        error.c
        util.c
        database.c
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(esearch ${SOURCES})

install(TARGETS esearch DESTINATION bin)
