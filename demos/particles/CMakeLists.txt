file(GLOB_RECURSE PARTICLES_SOURCES "*.cc")
file(GLOB_RECURSE PARTICLES_HEADERS *)

add_executable(particles ${PARTICLES_SOURCES} ${PARTICLES_HEADERS})

target_link_libraries(particles PRIVATE Planequest_Engine)
# As a temporary solution the target must link to glfw3 for certain definitions
target_link_libraries(particles PRIVATE glfw)