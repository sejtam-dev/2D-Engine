cmake_minimum_required (VERSION 3.8)

message("")
message("====== Engine ======")
message("=")

# Project root directory
set(ENGINE_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

message("= Root Directory: ${ENGINE_ROOT_DIR}")
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
list(APPEND ENGINE_LIBRARIES libglew_shared)

# GL¨M
list(APPEND ENGINE_INCLUDE_DIRS ${GLM_INCLUDE_DIR})
list(APPEND ENGINE_LIBRARIES glm_shared)

# Create Library
include_directories(${ENGINE_INCLUDES_DIR})
add_library(Engine SHARED ${ENGINE_SOURCES})

# Link Libraries
target_include_directories(Engine PUBLIC ${ENGINE_INCLUDE_DIRS})
target_link_libraries(Engine PUBLIC ${ENGINE_LIBRARIES})

set_target_properties(Engine
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/bin/Engine
        LIBRARY_OUTPUT_DIRECTORY ${ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/lib/Engine
        ARCHIVE_OUTPUT_DIRECTORY ${ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/lib/Engine
)

add_custom_command(TARGET Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ENGINE_ROOT_DIR}/Resources/
        $<TARGET_FILE_DIR:Engine>/Resources/
)

message("=")
message("====================")
message("")