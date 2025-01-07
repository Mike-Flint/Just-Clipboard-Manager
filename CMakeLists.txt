cmake_minimum_required(VERSION 3.10)

project(ClipboardManager LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ClipboardManager ClipboardManager.cpp)

set_target_properties(ClipboardManager PROPERTIES WIN32_EXECUTABLE NO)

