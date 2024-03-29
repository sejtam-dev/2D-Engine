﻿cmake_minimum_required (VERSION 3.8)

message("")
message("===== TestGame =====")
message("=")

# Project root directory
set(TEST_GAME_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

message("= Root Directory: ${TEST_GAME_ROOT_DIR}")
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
set(ENGINE_INCLUDE_DIR ${ROOT_DIR}/Engine/include)
list(APPEND TEST_GAME_INCLUDE_DIRS ${ENGINE_INCLUDE_DIR})
list(APPEND TEST_GAME_LIBRARIES Engine)

# Create Executable File
include_directories(${TEST_GAME_INCLUDES_DIR})
add_executable(TestGame ${TEST_GAME_SOURCES})

# Link Libraries
target_include_directories(TestGame PUBLIC ${TEST_GAME_INCLUDE_DIRS})
target_link_libraries(TestGame PRIVATE ${TEST_GAME_LIBRARIES})

set_target_properties(TestGame
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/bin/TestGame
        LIBRARY_OUTPUT_DIRECTORY ${ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/lib/TestGame
        ARCHIVE_OUTPUT_DIRECTORY ${ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/lib/TestGame
)

add_custom_command(TARGET TestGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE_DIR:Engine>/engine.dll
        $<TARGET_FILE_DIR:TestGame>
)

add_custom_command(TARGET TestGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        $<TARGET_FILE_DIR:Engine>/Resources/
        $<TARGET_FILE_DIR:TestGame>/Resources
)

add_custom_command(TARGET TestGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE_DIR:glfw>/glfw.dll
        $<TARGET_FILE_DIR:TestGame>
)

add_custom_command(TARGET TestGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE_DIR:libglew_shared>/glew.dll
        $<TARGET_FILE_DIR:TestGame>
)

if(IMGUI_ENABLED)
    add_custom_command(TARGET TestGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE_DIR:imgui_shared>/imgui.dll
            $<TARGET_FILE_DIR:TestGame>
    )
endif()

add_custom_command(TARGET TestGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE_DIR:glm_shared>/glm.dll
        $<TARGET_FILE_DIR:TestGame>
)

message("=")
message("====================")
message("")