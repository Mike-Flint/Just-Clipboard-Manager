cmake_minimum_required(VERSION 3.10)

project(ClipboardManager LANGUAGES CXX)

# List of sources
set(SOURCES 
    src/loader.cpp
    src/save.cpp
    src/init.cpp
    src/main.cpp
)   

# Basic settings for all targets
function(setup_target TARGET_NAME)
    target_link_libraries(${TARGET_NAME} ${OPENSSL_LIBRARIES})
    target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include )
endfunction()

# Main target
add_executable(ClipboardManager ${SOURCES})
setup_target(ClipboardManager)
set_target_properties(ClipboardManager PROPERTIES WIN32_EXECUTABLE YES)


# Target with console
add_executable(CM_WithConsole ${SOURCES})
setup_target(CM_WithConsole)
set_target_properties(CM_WithConsole PROPERTIES WIN32_EXECUTABLE NO)



