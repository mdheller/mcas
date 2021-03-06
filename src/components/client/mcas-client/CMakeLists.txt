cmake_minimum_required (VERSION 3.5.1 FATAL_ERROR)

execute_process( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )

project(component-mcasclient CXX)

set(CMAKE_CXX_STANDARD 14)

include (../../../../mk/clang-dev-tools.cmake)

add_subdirectory(./unit_test)

set(GCC_COVERAGE_COMPILE_FLAGS "-g -O2 -fPIC")

file(GLOB SOURCES src/*.cpp)

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS} ${FLAG_DUMP_CLASS} -DCONFIG_DEBUG)
add_compile_options(-g -pedantic -Wall -Werror -Wextra -Wcast-align -Wcast-qual -Wconversion -Wredundant-decls -Wshadow -Wtype-limits -Wunused-parameter -Wwrite-strings)

include_directories(../../../lib/common/include/)
include_directories(../)
include_directories(../../)
include_directories(${CMAKE_INSTALL_PREFIX}/include) # city.h
link_directories(/usr/lib/x86_64-linux-gnu)

include_directories(../../../server/mcas/src/)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")

if( ${ARCHITECTURE} STREQUAL "x86_64" )
  target_link_libraries(${PROJECT_NAME} common pthread numa dl rt z) # optional 'profiler'
else()
  target_link_libraries(${PROJECT_NAME} common pthread numa dl rt z)
endif()

# set the linkage in the install/lib
set_target_properties(${PROJECT_NAME} PROPERTIES
  INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)

install (TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)


