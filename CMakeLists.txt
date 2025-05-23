cmake_minimum_required(VERSION 3.10)

project(planequest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE PROJECT_SOURCES  "${CMAKE_SOURCE_DIR}/src/**/*.cc")
file(GLOB_RECURSE PROJECT_HEADERS "${CMAKE_SOURCE_DIR}/src/**/*.hh")

set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/include")
file(GLOB_RECURSE GLOB_INCLUDES "${CMAKE_SOURCE_DIR}/lib/include/**/*.h")

find_package(glfw3 REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(Freetype REQUIRED)

file(GLOB_RECURSE GLAD_SOURCE "${CMAKE_SOURCE_DIR}/lib/src/glad.c")
add_library(glad STATIC ${GLAD_SOURCE} ${GLAD_INCLUDES})
target_include_directories(glad PUBLIC "${GLAD_INCLUDE_DIR}")

add_executable(planequest ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_include_directories(planequest PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(planequest PRIVATE glad glfw ${SQLITE3_LIBRARIES} ${FREETYPE_LIBRARIES})
