
file(GLOB_RECURSE ENGINE_SOURCES "**/*.cc")
file(GLOB_RECURSE ENGINE_HEADERS "**/*.hh")

add_library(Planequest_Engine STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})

target_include_directories(Planequest_Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(Planequest_Engine PUBLIC cxx_std_17)

target_link_libraries(Planequest_Engine PRIVATE glad earcut SQLite::SQLite3 Freetype::Freetype)
target_link_libraries(Planequest_Engine PUBLIC glm::glm glfw)

target_compile_definitions(Planequest_Engine PRIVATE -DGLFW_INCLUDE_NONE)
target_compile_definitions(Planequest_Engine PRIVATE $<IF:$<CONFIG:Debug>,DEBUG,>)