cmake_minimum_required(VERSION 3.4.1)

add_library(JNILoader SHARED JNILoader.cpp)

find_library(log-lib log)

target_link_libraries(JNILoader ${log-lib})
