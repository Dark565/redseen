cmake_minimum_required(VERSION 3.16)

project(planequest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(glfw3 REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(Freetype REQUIRED)
find_package(glm REQUIRED)

set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/glad/include")
file(GLOB_RECURSE GLOB_INCLUDES "${CMAKE_SOURCE_DIR}/glad/lib/include/**/*.h")

set(GLAD_SOURCE "${CMAKE_SOURCE_DIR}/lib/glad/src/glad.c")
add_library(glad STATIC ${GLAD_SOURCE} ${GLAD_INCLUDES})
target_include_directories(glad PUBLIC "${GLAD_INCLUDE_DIR}")

add_library(earcut INTERFACE)
target_include_directories(earcut INTERFACE "${CMAKE_SOURCE_DIR}/src")
target_include_directories(earcut INTERFACE ${FREETYPE_INCLUDE_DIRS})
target_include_directories(earcut INTERFACE "${CMAKE_SOURCE_DIR}/third_party/earcut.hpp/include")
target_precompile_headers(earcut INTERFACE "${CMAKE_SOURCE_DIR}/third_party/earcut.hpp/include/mapbox/earcut.hpp")

add_subdirectory(src)

option(BUILD_DEMOS "Build demo applications" OFF)

if(BUILD_DEMOS)
    add_subdirectory(demos)
endif()
