#pragma once

#include <glad/glad.h>

class EBO_Class {
public:
	// ID reference of Elements Buffer Object
	unsigned int ID;
	// Constructor that generates a Elements Buffer Object and links it ot indices 
	EBO_Class(unsigned int* indices, GLsizeiptr size);

	// Binds the EBO
	void Bind();
	// Unbinds the EBO
	void Unbind();
	// Deletes the EBO
	void Delete();
};