#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "tiles.h"
#include "VBO.h"
#include "EBO.h"
#include "shaderClass.h"
#include "VAO.h"
#include "window.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    VAO_Class VAO;
    tiles background(VAO);


    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window);
        
        // Render
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        background.shaderProgram.use();

        VAO.Bind();
        background.renderTiles();

        // Update
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clearing all ressources
    background.clean();
    VAO.Delete();
    glfwTerminate();
    return 0;
}


