cmake_minimum_required(VERSION 3.23.2)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source.")
endif()

project(
    "Caisy" 
    VERSION 0.1 
    LANGUAGES C CXX)

add_subdirectory(libs/)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# get glfw from github
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
  # Update submodules as needed
  option(GIT_SUBMODULE "Check submodules during build" ON)
  if(GIT_SUBMODULE)
    message(STATUS "Submodule update")
    execute_process(
      COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
      message(
        FATAL_ERROR
          "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules"
      )
    endif()
  endif()
endif()

# CHECK ALL THE SUBMODULES
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/libs/glfw/CMakeLists.txt")
  message(
    FATAL_ERROR
      "The glfw submodules was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again."
  )
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/App/Application.cpp
    src/App/Application.h
    src/TextEditor/TextEditor.cpp
    src/TextEditor/TextEditor.h
    src/Renderer/Shader/Shader.cpp
    src/Renderer/Shader/Shader.h
    src/Renderer/VBO/VBO.cpp
    src/Renderer/VBO/VBO.h
    src/Renderer/VAO/VAO.cpp
    src/Renderer/VAO/VAO.h
    src/Renderer/EBO/EBO.cpp
    src/Renderer/EBO/EBO.h
    src/Events/Event.h
    src/Events/ApplicationEvent.h
    src/Events/KeyEvent.h
    src/Events/MouseEvent.h
    src/Events/Input.cpp
    src/Events/Input.h
    src/App/WindowClass.h
    src/App/WindowClass.cpp
    src/App/Object.h
    src/App/ObjectStack.h
    src/App/ObjectStack.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC libs/freetype/include/ 
    libs/pango/include/pango-1.0/
    libs/glib/include/glib-2.0/
    libs/harfbuzz/include/harfbuzz/
    libs/icu-windows/include/)
target_link_directories(${PROJECT_NAME} PUBLIC libs/freetype/include/
    libs/icu-windows/lib64/
    libs/pango/lib/
    libs/glib/lib/
    libs/harfbuzz/lib/)

target_link_libraries(${PROJECT_NAME} PUBLIC glad glfw glm freetype icuuc glib-2.0 gobject-2.0  harfbuzz pango-1.0 pangoft2-1.0)
