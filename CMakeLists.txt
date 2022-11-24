cmake_minimum_required(VERSION 3.23.2)

project(Caisy VERSION 0.1)
add_subdirectory(include/)

add_executable(
  ${PROJECT_NAME}
  src/main.cpp
  src/EBO.cpp
  src/shaderClass.cpp
  src/stb_image.cpp
  src/text.cpp
  src/tiles.cpp
  src/update.cpp
  src/VAO.cpp
  src/VBO.cpp
  src/window.cpp
  src/glad.c)

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
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/glfw/CMakeLists.txt")
  message(
    FATAL_ERROR
      "The glfw submodules was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again."
  )
endif()

add_subdirectory(external/glfw)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC include/
   external/glfw/include
   include/glad/)

target_link_directories(
  ${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/ PUBLIC
  external/glfw/src)

target_link_libraries(${PROJECT_NAME} glfw glad freetype)

install(
  TARGETS ${PROJECT_NAME}
  CONFIGURATIONS Release
  DESTINATION bin)
install(
  FILES "${PROJECT_SOURCE_DIR}/include/freetype.dll"
  CONFIGURATIONS Release
  DESTINATION bin)
install(
  FILES "${PROJECT_SOURCE_DIR}/include/glfw3.dll"
  CONFIGURATIONS Release
  DESTINATION bin)
install(
  FILES "${PROJECT_SOURCE_DIR}/include/ucrtbased.dll"
  CONFIGURATIONS Release
  DESTINATION bin)
install(
  DIRECTORY "${PROJECT_SOURCE_DIR}/Resources"
  CONFIGURATIONS Release
  DESTINATION bin)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${Caisy_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${Caisy_VERSION_MINOR}")
include(CPack)
