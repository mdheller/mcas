cmake_minimum_required (VERSION 3.5.1 FATAL_ERROR)

project(common CXX)

add_subdirectory(unit_test)

include(../../../mk/clang-dev-tools.cmake)

set(CMAKE_CXX_STANDARD 14)

configure_file(include/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/config.h)

execute_process( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )

set(CMAKE_INCLUDE_CURRENT_DIR true)
include_directories(./include)

if( ${ARCHITECTURE} STREQUAL "x86_64" )
  add_definitions(${GCC_COVERAGE_COMPILE_FLAGS} -msse4.2)
endif()

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS} -DCONFIG_DEBUG)

file(GLOB SOURCES src/*.c*)

add_library(common SHARED ${SOURCES})

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
install(DIRECTORY "include/common" DESTINATION include FILES_MATCHING PATTERN "*.h*")
install(DIRECTORY "include/component" DESTINATION include FILES_MATCHING PATTERN "*.h*")
