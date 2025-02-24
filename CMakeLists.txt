cmake_minimum_required (VERSION 3.11)

option(IMGUI_ENABLED "Build project with IMGUI" OFF)
option(TEST_GAME_ENABLED "Build TestGame with Project" OFF)

message("==== Project ====")
message("=")

project(Engine VERSION 1.0.0)

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
message("= Root Directory: ${ROOT_DIR}")
message("=")

# C++ Standarts
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

## Create project
message("= C++ Version: ${CMAKE_CXX_STANDARD}")
message("= C++ Compiler: ${CMAKE_CXX_COMPILER_ID}")
message("= C++ Compiler Path: ${CMAKE_CXX_COMPILER}")
message("=")
message("=== Architecture ===")
message("=")

# Project architecture
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

## DEFINITIONS
if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_definitions(-DDEBUG)
else()
    add_definitions(-DRELEASE)
endif()

if(IMGUI_ENABLED)
    add_definitions(-DIMGUI_ENABLED)
endif()

## Libraries
message("=== Libraries ===")
message("=")

# GLFW
message("== GLFW")
message("=")

set(GLFW_ROOT_DIR ${ROOT_DIR}/3rdparty/glfw)
message("= Root Directory: ${GLFW_ROOT_DIR}")
message("=")

set(BUILD_SHARED_LIBS ON)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL ON)

add_subdirectory(${GLFW_ROOT_DIR})

if(TARGET glfw)    
    set(GLFW_INCLUDE_DIR ${GLFW_ROOT_DIR}/include)

    message("= Status: Found")
else()
    message("= Status: Not Found")
endif()

set_target_properties(glfw
    PROPERTIES
        PREFIX ""
        OUTPUT_NAME "glfw"
        DEBUG_POSTFIX ""
        RUNTIME_OUTPUT_DIRECTORY ${ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/bin/glfw
        LIBRARY_OUTPUT_DIRECTORY ${ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/lib/glfw
        ARCHIVE_OUTPUT_DIRECTORY ${ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/lib/glfw
)

message("=")

# GLEW
message("== GLEW")
message("=")

set(GLEW_ROOT_DIR ${ROOT_DIR}/3rdparty/glew-cmake)
message("= Root Directory: ${GLEW_ROOT_DIR}")
message("=")

add_subdirectory(${GLEW_ROOT_DIR})

if(TARGET libglew_shared)
    set(GLEW_INCLUDE_DIR ${GLEW_ROOT_DIR}/include)

    message("= Status: Found")
else()
    message("= Status: Not Found")
endif()

set_target_properties(libglew_shared
    PROPERTIES
        PREFIX ""
        OUTPUT_NAME "glew"
        DEBUG_POSTFIX ""
        RUNTIME_OUTPUT_DIRECTORY ${ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/bin/glew
        LIBRARY_OUTPUT_DIRECTORY ${ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/lib/glew
        ARCHIVE_OUTPUT_DIRECTORY ${ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/lib/glew
)

# GLM
message("== GLM")
message("=")

set(GLM_BUILD_LIBRARY on)

set(GLM_ROOT_DIR ${ROOT_DIR}/3rdparty/glm)
message("= Root Directory: ${GLM_ROOT_DIR}")
message("=")

add_subdirectory(${GLM_ROOT_DIR})

if(TARGET glm)
    set(GLM_INCLUDE_DIR ${GLM_ROOT_DIR}/glm)

    message("= Status: Found")

    set_target_properties(glm
            PROPERTIES
            PREFIX ""
            OUTPUT_NAME "glm"
            DEBUG_POSTFIX ""
            RUNTIME_OUTPUT_DIRECTORY ${ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/bin/glm
            LIBRARY_OUTPUT_DIRECTORY ${ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/lib/glm
            ARCHIVE_OUTPUT_DIRECTORY ${ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/lib/glm
    )
else()
    message("= Status: Not Found")
endif()

# ImGUI
if(IMGUI_ENABLED)
    message("== ImGUI")
    message("=")

    set(IMGUI_ROOT_DIR ${ROOT_DIR}/3rdparty/imgui)
    message("= Root Directory: ${IMGUI_ROOT_DIR}")
    message("=")

    # Create library
    add_library(imgui_shared SHARED)

    # Set tart sources
    target_sources(imgui_shared
        PRIVATE
            ${IMGUI_ROOT_DIR}/imgui.cpp
            ${IMGUI_ROOT_DIR}/imgui_draw.cpp
            ${IMGUI_ROOT_DIR}/imgui_tables.cpp
            ${IMGUI_ROOT_DIR}/imgui_widgets.cpp
            ${IMGUI_ROOT_DIR}/backends/imgui_impl_glfw.cpp
            ${IMGUI_ROOT_DIR}/backends/imgui_impl_opengl3.cpp
    )

    # Include directories
    target_include_directories(imgui_shared
        PUBLIC
            ${IMGUI_ROOT_DIR}
            ${IMGUI_ROOT_DIR}/backends
        PRIVATE
            ${GLFW_ROOT_DIR}/include
    )

    # Link libraries
    target_link_libraries(imgui_shared
        PRIVATE glfw
    )

    if(TARGET imgui_shared)
        set(IMGUI_INCLUDE_DIR)
        list(APPEND IMGUI_INCLUDE_DIR ${IMGUI_ROOT_DIR})
        list(APPEND IMGUI_INCLUDE_DIR ${IMGUI_ROOT_DIR}/backends)

        message("= Status: Found")
    else()
        message("= Status: Not Found")
    endif()

    set_target_properties(imgui_shared
        PROPERTIES
            PREFIX ""
            OUTPUT_NAME "imgui"
            DEBUG_POSTFIX ""
            RUNTIME_OUTPUT_DIRECTORY ${ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/bin/imgui
            LIBRARY_OUTPUT_DIRECTORY ${ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/lib/imgui
            ARCHIVE_OUTPUT_DIRECTORY ${ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/lib/imgui
    )
endif()

message("=")
message("====================")
message("")

add_subdirectory(Engine)

if(TEST_GAME_ENABLED)
    add_subdirectory(TestGame)
endif()