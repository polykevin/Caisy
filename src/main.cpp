/*Copyright 2022 Kevin Poly*/
#include <glad/glad.h>
#include <stdlib.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "App/Application.h"

Application* Application::s_Instance = new Application();

int main(int argc, char* argv[]) {
    if (argc > 1)
        Application::Get()->file = argv[1];

    Application::Get()->Run();

    exit(EXIT_SUCCESS);
}
