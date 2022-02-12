cmake_minimum_required (VERSION 3.8)

message("")
message("====== Engine ======")
message("=")

# Project root directory
set(ENGINE_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

message("= Root Directory: ${ENGINE_ROOT_DIR}")
message("=")

# C++ Standarts
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Create project
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

# GLFW
message("== GLFW")
message("=")

set(GLFW_ROOT_DIR ${ENGINE_ROOT_DIR}/3rdparty/glfw)
message("= Root Directory: ${GLFW_ROOT_DIR}")
message("=")

set(BUILD_SHARED_LIBS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL ON)

add_subdirectory(${GLFW_ROOT_DIR})
message("=")

if(TARGET glfw)    
    set(GLFW_INCLUDE_DIR ${GLFW_ROOT_DIR}/include)

    message("= Status: Found")
else()
    message("= Status: Not Found")
endif()

message("=")

# GLEW
message("== GLEW")
message("=")

set(GLEW_ROOT_DIR ${ENGINE_ROOT_DIR}/3rdparty/glew-cmake)
message("= Root Directory: ${GLEW_ROOT_DIR}")
message("=")

add_subdirectory(${GLEW_ROOT_DIR})
message("=")

if(TARGET libglew_static)
    set(GLEW_INCLUDE_DIR ${GLEW_ROOT_DIR}/include)

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
set(ENGINE_INCLUDES_DIR ${ENGINE_ROOT_DIR}/include)
file(GLOB Engine_INCLUDES "${ENGINE_INCLUDES_DIR}/*.h" "${ENGINE_INCLUDES_DIR}/*.hpp")

message("= Headers Directory: ${ENGINE_INCLUDES_DIR}")

# Source
set(ENGINE_SOURCE_DIR ${ENGINE_ROOT_DIR}/src)
file(GLOB ENGINE_SOURCES "${ENGINE_SOURCE_DIR}/*.c" "${ENGINE_SOURCE_DIR}/*.cpp")

message("= Source Directory: ${ENGINE_SOURCE_DIR}")

# Libraries
set(ENGINE_INCLUDE_DIRS)
set(ENGINE_LIBRARIES)

# GLFW
list(APPEND ENGINE_INCLUDE_DIRS ${GLFW_INCLUDE_DIR})
list(APPEND ENGINE_LIBRARIES glfw)


# GLEW
list(APPEND ENGINE_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
list(APPEND ENGINE_LIBRARIES libglew_static)

# Create Library
include_directories(${ENGINE_INCLUDES_DIR})
add_library(Engine SHARED ${ENGINE_SOURCES})

# Link Libraries
target_include_directories(Engine PUBLIC ${ENGINE_INCLUDE_DIRS})
target_link_libraries(Engine PUBLIC ${ENGINE_LIBRARIES})


message("=")
message("====================")
message("")