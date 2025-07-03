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
file(GLOB_RECURSE Engine_INCLUDES
        "${ENGINE_INCLUDES_DIR}/*.h"
        "${ENGINE_INCLUDES_DIR}/*.hpp"
)

message("= Headers Directory: ${ENGINE_INCLUDES_DIR}")

# Source
set(ENGINE_SOURCE_DIR ${ENGINE_ROOT_DIR}/src)
file(GLOB_RECURSE ENGINE_SOURCES
        "${ENGINE_SOURCE_DIR}/*.c"
        "${ENGINE_SOURCE_DIR}/*.cpp"
        "${ENGINE_SOURCE_DIR}/*.cxx"
)

message("= Source Directory: ${ENGINE_SOURCE_DIR}")

# Modules
set(ENGINE_MODULES_DIR ${ENGINE_ROOT_DIR}/src)
file(GLOB_RECURSE ENGINE_MODULES
        "${ENGINE_MODULES_DIR}/*.ixx"
)

message("= Modules Directory: ${ENGINE_MODULES_DIR}")

# Libraries
set(ENGINE_INCLUDE_DIRS)
set(ENGINE_LIBRARIES)

# GLFW
list(APPEND ENGINE_INCLUDE_DIRS ${GLFW_INCLUDE_DIR})
list(APPEND ENGINE_LIBRARIES glfw)

# GLEW
list(APPEND ENGINE_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
list(APPEND ENGINE_LIBRARIES libglew_shared)

# GLM
list(APPEND ENGINE_INCLUDE_DIRS ${GLM_INCLUDE_DIR})
list(APPEND ENGINE_LIBRARIES glm)

# IMGUI
if(IMGUI_ENABLED)
    list(APPEND ENGINE_INCLUDE_DIRS ${IMGUI_INCLUDE_DIR})
    list(APPEND ENGINE_LIBRARIES imgui_shared)
endif()

# Create Library
include_directories(${ENGINE_INCLUDES_DIR})

add_library(Engine SHARED)
target_sources(Engine
    PRIVATE
        ${ENGINE_SOURCES}
    PUBLIC
        FILE_SET headers
        TYPE HEADERS
        BASE_DIRS ${ENGINE_INCLUDES_DIR}
        FILES ${ENGINE_INCLUDES}
    PUBLIC
        FILE_SET modules
        TYPE CXX_MODULES
        BASE_DIRS ${ENGINE_MODULES_DIR}
        FILES ${ENGINE_MODULES}
)


# Link Libraries
target_include_directories(Engine PUBLIC ${ENGINE_INCLUDES_DIRS})
target_link_libraries(Engine PUBLIC ${ENGINE_LIBRARIES})
target_compile_features(Engine PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(Engine PRIVATE
            /std:c++20
            /EHsc
    )
else()
    target_compile_options(Engine PRIVATE
            -fmodules-ts
            -std=c++20
    )
endif()



set_target_properties(Engine
    PROPERTIES
        PREFIX ""
        OUTPUT_NAME "engine"
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