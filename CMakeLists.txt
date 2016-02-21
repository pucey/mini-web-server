cmake_minimum_required(VERSION 2.8)
project(MiniWebServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})

set(HTTP_SERVER_EXCEPTION_SRC ${PROJECT_ROOT}/network/http_exception.cpp)
add_library(HTTP_SERVER_EXCEPTION_LIB STATIC ${HTTP_SERVER_EXCEPTION_SRC})

set(HTTP_SERVER_SRC ${PROJECT_ROOT}/network/http_server.cpp)
add_library(HTTP_SERVER_LIB STATIC ${HTTP_SERVER_SRC})
target_include_directories(HTTP_SERVER_LIB PUBLIC ${PROJECT_ROOT}/include)
target_link_libraries(HTTP_SERVER_LIB HTTP_SERVER_EXCEPTION_LIB ${PROJECT_ROOT}/lib)

set(MAIN_APP ${PROJECT_ROOT}/main.cpp)
add_executable(final ${MAIN_APP})
target_link_libraries(final HTTP_SERVER_EXCEPTION_LIB HTTP_SERVER_LIB)
