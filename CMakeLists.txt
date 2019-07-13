cmake_minimum_required(VERSION 3.14)
project(localutils C)

set(CMAKE_C_STANDARD 11)

set(TYPES_SOURCE_FILES types/comparers.c types/dict.c types/list.c)
set(TYPES_HEADER_FILES types/types.h;types/comparers.h;types/list.h;types/dict.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

add_library(types ${TYPES_SOURCE_FILES})
add_executable(runtests tests/runtests.c)
target_link_libraries(runtests types)

set_target_properties(types PROPERTIES PUBLIC_HEADER "${TYPES_HEADER_FILES}")

INSTALL(TARGETS types
        ARCHIVE DESTINATION         /usr/local/lib/${PROJECT_NAME}
        LIBRARY DESTINATION         /usr/local/lib/${PROJECT_NAME}
        PUBLIC_HEADER DESTINATION   /usr/local/include/${PROJECT_NAME})