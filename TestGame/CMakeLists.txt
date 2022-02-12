cmake_minimum_required (VERSION 3.8)

message("")
message("===== TestGame =====")
message("=")

# Project root directory
set(TEST_GAME_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
get_filename_component(TEST_GAME_PARENT_DIR ${TEST_GAME_ROOT_DIR} DIRECTORY)

message("= Root Directory: ${TEST_GAME_ROOT_DIR}")
message("=")

# C++ Standarts
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project Settings
message("= C++ Version: ${CMAKE_CXX_STANDARD}")
message("= C++ Compiler: ${CMAKE_CXX_COMPILER_ID}")
message("= C++ Compiler Path: ${CMAKE_CXX_COMPILER}")
message("=")
message("=== Architecture ===")
message("=")

# project architecture
if(${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    set(PROJECT_ARCHITECTURE x64)
else()
    set(PROJECT_ARCHITECTURE x86)
endif()

message("= Architecture: ${PROJECT_ARCHITECTURE}")

# Project OS
if(WIN32 OR MSVC OR MSYS OR MINGW)
    set(WINDOWS true)    

    if(PROJECT_ARCHITECTURE STREQUAL x64)
        set(WINDOWS64 true)
        set(PROJECT_OS "Win64")
    else()
        set(WINDOWS32 true)
        set(PROJECT_OS "Win32")
    endif()
elseif(UNIX AND NOT APPLE)
    set(LINUX true)

    if(PROJECT_ARCHITECTURE STREQUAL x64)
        set(LINUX64 true)
        set(PROJECT_OS "Linux64")
    else()
        set(LINUX32 true)
        set(PROJECT_OS "Linux32")
    endif()
endif()

message("= OS: ${PROJECT_OS}")
message("=")

## Dependencies
message("=== Dependencies ===")
message("=")

# Engine
message("== Engine")
message("=")

set(ENGINE_ROOT_DIR ${TEST_GAME_PARENT_DIR}/Engine)
message("= Root Directory: ${ENGINE_ROOT_DIR}")
message("=")

if(TARGET Engine)    
    set(ENGINE_INCLUDE_DIR ${ENGINE_ROOT_DIR}/include)

    message("= Status: Found")
else()
    message("= Status: Not Found")
endif()
message("=")

## Project
message("=== Project ========")
message("=")

message("= Version: ${Engine_VERSION}")
message("= ")

# Headers
set(TEST_GAME_INCLUDES_DIR ${TEST_GAME_ROOT_DIR}/include)
file(GLOB TEST_GAME_INCLUDES "${TEST_GAME_INCLUDES_DIR}/*.h" "${TEST_GAME_INCLUDES_DIR}/*.hpp")

message("= Headers Directory: ${TEST_GAME_INCLUDES_DIR}")

# Source
set(TEST_GAME_SOURCE_DIR ${TEST_GAME_ROOT_DIR}/src)
file(GLOB TEST_GAME_SOURCES "${TEST_GAME_SOURCE_DIR}/*.c" "${TEST_GAME_SOURCE_DIR}/*.cpp")

message("= Source Directory: ${TEST_GAME_SOURCE_DIR}")

# Libraries
set(TEST_GAME_INCLUDE_DIRS)
set(TEST_GAME_LIBRARIES)

# Engine
list(APPEND TEST_GAME_INCLUDE_DIRS ${ENGINE_INCLUDE_DIR})
list(APPEND TEST_GAME_LIBRARIES Engine)

# Create Executable File
include_directories(${TEST_GAME_INCLUDES_DIR})
add_executable(TestGame ${TEST_GAME_SOURCES})

# Link Libraries
target_include_directories(TestGame PUBLIC ${TEST_GAME_INCLUDE_DIRS})
target_link_libraries(TestGame PRIVATE ${TEST_GAME_LIBRARIES})


message("=")
message("====================")
message("")