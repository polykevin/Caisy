#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void processInput(GLFWwindow* window);
// Change the render size of window when the window changes its size
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
