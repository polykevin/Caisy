#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include "tiles.h"
#include "VBO.h"
#include "EBO.h"
#include "text.h"
#include "window.h"
#include "shaderClass.h"
#include "VAO.h"


class System
{
public:
	System();

	void update();

private:
	TextRenderer text;

};

