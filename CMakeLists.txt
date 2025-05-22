cmake_minimum_required(VERSION 3.10)

project(planequest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE PROJECT_SOURCES  "${CMAKE_SOURCE_DIR}/src/**/*.cc")
file(GLOB_RECURSE PROJECT_HEADERTS "${CMAKE_SOURCE_DIR}/src/**/*.hh")

find_package(glfw3 REQUIRED)
find_package(SQLite3 REQUIRED)

add_executable(planequest ${PROJECT_SOURCES} ${PROJET_HEADERS} ${GLFW3_LIBRARIES} ${SQLITE3_LIBRARIES})
target_include_directories(planequest PRIVATE "${CMAKE_SOURCE_DIR}/src")
