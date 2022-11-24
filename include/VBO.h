#pragma once

#include <glad/glad.h>

class VBO_Class
{
public:
	// Reference ID of the Vertex Buffer Object
	unsigned int ID;
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VBO_Class(float* vertices, GLsizeiptr size);

	// Binds the VBO
	void Bind();
	// Uninds the VBO
	void Unbind();
	// Deletes the VBO
	void Delete();

};