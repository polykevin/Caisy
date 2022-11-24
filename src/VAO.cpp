#include "VAO.h"

// Constructor that generates a VAO ID
VAO_Class::VAO_Class() {
	glGenVertexArrays(1, &ID);
}

// Links a VBO to the VAO using a certain layout
void VAO_Class::LinkVBO(VBO_Class& VBO, unsigned int layout) {
	VBO.Bind();
	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

// Binds the VAO
void VAO_Class::Bind() {
	glBindVertexArray(ID);
}

// Unbinds the VAO
void VAO_Class::Unbind() {
	glBindVertexArray(0);
}

// Deletes the VAO
void VAO_Class::Delete() {
	glDeleteVertexArrays(1, &ID);
}