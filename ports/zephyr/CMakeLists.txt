cmake_minimum_required(VERSION 3.8)

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(NONE)

target_sources(app PRIVATE src/zephyr_start.c src/zephyr_getchar.c)

add_library(libmicropython STATIC IMPORTED)
set_target_properties(libmicropython PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libmicropython.a)
target_link_libraries(app PUBLIC libmicropython)
