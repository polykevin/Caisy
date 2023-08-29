/*Copyright 2022 Kevin Poly*/
#include "../App/Application.h"
#include "Input.h"
#include <GLFW/glfw3.h>

Input* Input::s_Instance = new Input();

bool Input::IsKeyPressedImpl(int keycode) {
    auto window =  static_cast<GLFWwindow*>(Application::Get()->GetWindow().GetNativeWindow());
    auto state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressedImpl(int button) {
    auto window = static_cast<GLFWwindow*>(Application::Get()->GetWindow().GetNativeWindow());
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

std::pair<float, float> Input::GetMousePositionImpl() {
    auto window = static_cast<GLFWwindow*>(Application::Get()->GetWindow().GetNativeWindow());
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    return { (float)xpos, (float)ypos };
}

