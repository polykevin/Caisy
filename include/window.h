#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <Windows.h>
#include "tiles.h"
#include "VBO.h"
#include "EBO.h"
#include "shaderClass.h"
#include "VAO.h"

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);// Call when click or whell
void character_callback(GLFWwindow* window, unsigned int codepoint);// Call when text
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);// Call when input 
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);// Call when scroll
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

extern bool textMode;
extern double xposition, yposition;
extern std::string textString;
extern bool press;
extern float lastFrame;
extern float deltaTime;
extern float cameraSpeed;
extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;
extern std::vector<std::string> history_Z;
extern int nbReturn_Z;
extern std::vector<std::string> history_Y;
extern int nbReturn_Y;
extern int cursor;
extern int cursorMinus; // The amount of time we need to go back for the cursor
extern char* pointerCursor;


