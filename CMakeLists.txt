
cmake_minimum_required(VERSION 3.22)

project(o1shm
        VERSION 1.0
        DESCRIPTION "Simple Shared Memory Library"
        LANGUAGES C
)

set(FILES "src/o1shm.h" "src/o1shm.c" "src/o1shm-static.h")

add_library(o1shm ${FILES})

install(TARGETS o1shm ARCHIVE DESTINATION lib COMPONENT libraries)
install(FILES ${FILES} DESTINATION include COMPONENT headers)

include(CPack)
