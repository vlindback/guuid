cmake_minimum_required (VERSION 2.6)
project (guuid)
add_library(guuid guuid.c)
target_include_directories(guuid, PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)