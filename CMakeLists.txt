cmake_minimum_required(VERSION 2.8)
project(MiniWebServer)

set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})

#execute_process(
#  COMMAND tar zxf libevent.tar.gz
#  WORKING_DIRECTORY ${PROJECT_ROOT}
#)

#execute_process(
#  COMMAND ./configure --prefix=${PROJECT_ROOT}
#  WORKING_DIRECTORY ${PROJECT_ROOT}/libevent
#)

#execute_process(
#  COMMAND make
#  WORKING_DIRECTORY ${PROJECT_ROOT}/libevent
#)

#execute_process(
#  COMMAND make install
#  WORKING_DIRECTORY ${PROJECT_ROOT}/libevent
#)


#include(ExternalProject)
#ExternalProject_Add(
#  libevent
#  SOURCE_DIR ${PROJECT_ROOT}/libevent
#  CONFIGURE_COMMAND ${PROJECT_ROOT}/libevent/configure --prefix=${PROJECT_ROOT}
#  PREFIX ${PROJECT_ROOT}
#  BUILD_COMMAND make
#  INSTALL_COMMAND make install
#  BUILD_IN_SOURCE 1
#)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(HTTP_SERVER_EXCEPTION_SRC ${PROJECT_ROOT}/network/http_exception.cpp)
add_library(HTTP_SERVER_EXCEPTION_LIB STATIC ${HTTP_SERVER_EXCEPTION_SRC})

set(HTTP_SERVER_SRC ${PROJECT_ROOT}/network/http_server.cpp)
add_library(HTTP_SERVER_LIB STATIC ${HTTP_SERVER_SRC})
#target_include_directories(HTTP_SERVER_LIB PUBLIC ${PROJECT_ROOT}/include)
#target_link_libraries(HTTP_SERVER_LIB HTTP_SERVER_EXCEPTION_LIB ${PROJECT_ROOT}/lib/libevent.a)

set(MAIN_APP ${PROJECT_ROOT}/main.cpp)
add_executable(final ${MAIN_APP})
target_link_libraries(final HTTP_SERVER_EXCEPTION_LIB HTTP_SERVER_LIB)
