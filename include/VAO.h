#pragma once

#include <glad/glad.h>
#include "VBO.h"

class VAO_Class
{
public:
	// ID reference for the Vertex Array Object
	unsigned int ID;
	// Constructor that generates a VAO ID
	VAO_Class();

	// Links a VBO to the VAO using a certain layout
	void LinkVBO(VBO_Class& VBO, unsigned int layout);
	// Binds the VAO
	void Bind();
	// Unbinds the VAO
	void Unbind();
	// Deletes the VAO
	void Delete();

};