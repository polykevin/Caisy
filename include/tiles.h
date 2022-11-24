#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "VBO.h"
#include "EBO.h"
#include "shaderClass.h"
#include "VAO.h"

class tiles
{
public:
	tiles(VAO_Class VAO);
    void renderTiles();
    void clean();

    Shader shaderProgram;

private:
    float firstShadingR = 0.0549019607843137f;
    float firstShadingG = 0.1176470588235294f;
    float firstShadingB = 0.1882352941176471f;

    float secondShadingR = 0.1411764705882353;
    float secondShadingG = 0.2313725490196078;
    float secondShadingB = 0.3333333333333333;

    VBO_Class VBO;
};

