project(generowanie_sygnalu)
cmake_minimum_required(VERSION 2.8)
file(GLOB Demo_HEADERS RELATIVE ${CMAKE_SOURCE_DIR}/include "include/*.h")
SET(sygnal_SOURCES main.c)
SET(sygnal_HEADERS include/header.h)
aux_source_directory(. SRC_LIST)
set(CMAKE_BUILD_TYPE Debug)
add_executable(${PROJECT_NAME} ${sygnal_SOURCES} ${sygnal_HEADERS})
#math library
target_link_libraries(${PROJECT_NAME} m)

