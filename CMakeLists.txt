cmake_minimum_required(VERSION 3.14)
project(localutils C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

include_directories(
        /usr/local/opt/curl/include/curl
        /usr/local/include)

link_directories(
        /usr/local/opt/curl/lib
        /usr/local/lib)

find_library(CURL_LIB curl PATH /usr/local/opt/curl/lib NO_DEFAULT_PATH)
find_library(ZIP_LIB zip PATH /usr/local/lib/libzip NO_DEFAULT_PATH)

add_library(types STATIC src/types/comparers.c src/types/dict.c src/types/list.c)
add_library(files STATIC src/files/files.c)
add_library(json STATIC src/json/jsonparser.c src/json/tokenizer.c)
target_link_libraries(files PUBLIC ${CURL_LIB} ${ZIP_LIB})

add_executable(runtests tests/run.c tests/ListTests.c tests/DictionaryTests.c tests/FileTests.c tests/JsonTests.c src/json/tokenizer.c)
target_link_libraries(runtests types files json)

set_target_properties(types PROPERTIES PUBLIC_HEADER "src/types/types.h;src/types/comparers.h;src/types/list.h;src/types/dict.h")
set_target_properties(files PROPERTIES PUBLIC_HEADER "src/files/files.h")
set_target_properties(json PROPERTIES PUBLIC_HEADER "src/json/jsonparser.h")

INSTALL(TARGETS types files
        ARCHIVE DESTINATION         /usr/local/lib/${PROJECT_NAME}
        LIBRARY DESTINATION         /usr/local/lib/${PROJECT_NAME}
        PUBLIC_HEADER DESTINATION   /usr/local/include/${PROJECT_NAME})