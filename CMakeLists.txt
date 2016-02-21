cmake_minimum_required(VERSION 2.8)
project(MiniWebServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})

set(HTTP_SERVER_SRC ${PROJECT_ROOT}/network/http_server.cpp)
add_library(HTTP_SERVER_LIB STATIC ${HTTP_SERVER_SRC})

set(MAIN_APP ${PROJECT_ROOT}/main.cpp)
add_executable(final ${MAIN_APP})
target_link_libraries(final HTTP_SERVER_LIB)
