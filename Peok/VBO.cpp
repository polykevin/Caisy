#include "VBO.h"

// Constructor that genertaes a Vertex BUffer Object and links it to vertives
VBO_Class::VBO_Class(float* vertices, GLsizeiptr size) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

// Binds the VBO
void VBO_Class::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Unbinds the VBO
void VBO_Class::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VBO
void VBO_Class::Delete() {
	glDeleteBuffers(1, &ID);
}