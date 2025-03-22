cmake_minimum_required(VERSION 3.10)

project(ClipboardManager LANGUAGES CXX)

add_executable(ClipboardManager
                    ClipboardManager.cpp
                    Init.cpp
                    ClipMonitor.cpp
                    )

set_target_properties(ClipboardManager PROPERTIES WIN32_EXECUTABLE YES)

# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

