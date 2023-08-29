add_subdirectory(glad)
add_subdirectory(glfw)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE glm/)
