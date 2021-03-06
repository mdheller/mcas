cmake_minimum_required (VERSION 3.5.1 FATAL_ERROR)

include(../../../mk/common.cmake)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(-g -Wall -Werror -Wextra -Wcast-align -Wcast-qual -Wconversion -Wold-style-cast -Wredundant-decls -Wshadow -Wtype-limits -Wunused-parameter -Wwrite-strings) # -pedantic  -Weffc++

# use this to disable optimizations, e.g. for debugging or profiling
add_compile_options("$<$<CONFIG:Debug>:-O0>")

#set(PROFILER "profiler")
#add_compile_options("-DPROFILE")

project(mcas CXX)
include(../../../mk/clang-dev-tools.cmake)

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/src/lib/libadoproto/src)
include_directories(../../lib/common/include/)
include_directories(../../lib/libpmem/include/)
include_directories(../../lib/libnupm/include/)
include_directories(../../components/)
include_directories(${CMAKE_INSTALL_PREFIX}/include) # rapidjson
link_directories(${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}) # tbb (via nupm)
link_directories(${CMAKE_INSTALL_PREFIX}/lib) # tbb (via nupm)

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS} -DCONFIG_DEBUG)
#add_definitions(${GCC_COVERAGE_COMPILE_FLAGS} -DNDEBUG)
add_compile_options("$<$<CONFIG:Release>:-DNDEBUG>")

configure_file(example-ado.conf.in ${CMAKE_CURRENT_SOURCE_DIR}/example-ado.conf)
configure_file(graph-ado.conf.in ${CMAKE_CURRENT_SOURCE_DIR}/graph-ado.conf)

set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")

add_executable(mcas src/main.cpp src/shard.cpp src/connection_handler.cpp src/ado_manager.cpp)

target_link_libraries(mcas ${ASAN_LIB} common numa pthread dl nupm boost_program_options ${PROFILER} z)

#add_subdirectory(unit_test)

set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}:${CMAKE_INSTALL_PREFIX}/lib)

add_dependencies(${PROJECT_NAME} common nupm)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(FILES "example-ado.conf" DESTINATION bin)
install(FILES "graph-ado.conf" DESTINATION bin)
