#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include "text.h"
#include "tiles.h"
#include "VBO.h"
#include "EBO.h"
#include "shaderClass.h"
#include "VAO.h"
#include "window.h"
#include "update.h"

int main(int argc, char *argv[]) {
  // glfw: initialize and configure
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // glfw window creation
  GLFWwindow *window = glfwCreateWindow(800, 600, "Caisy", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetCharCallback(window, character_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glad: load all OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, 800, 600);

  System caisy;

  // render loop
  while (!glfwWindowShouldClose(window)) {
    // Render
    glClearColor(0.0549019607843137f, 0.1176470588235294f, 0.1882352941176471f,
                 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    caisy.update();

    // Update
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // clearing all ressources
  glfwTerminate();
  return 0;
}
