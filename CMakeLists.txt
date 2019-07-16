cmake_minimum_required(VERSION 3.14)
project(localutils C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

add_library(types types/comparers.c types/dict.c types/list.c)
add_executable(runtests tests/run.c tests/ListTests.c tests/DictionaryTests.c)
target_link_libraries(runtests types)

set_target_properties(types PROPERTIES PUBLIC_HEADER "types/types.h;types/comparers.h;types/list.h;types/dict.h")

INSTALL(TARGETS types
        ARCHIVE DESTINATION         /usr/local/lib/${PROJECT_NAME}
        LIBRARY DESTINATION         /usr/local/lib/${PROJECT_NAME}
        PUBLIC_HEADER DESTINATION   /usr/local/include/${PROJECT_NAME})