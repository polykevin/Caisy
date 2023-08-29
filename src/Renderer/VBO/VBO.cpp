/*Copyright 2022 Kevin Poly*/
#include "VBO.h"

VBO_Class::VBO_Class(const void* data, unsigned int size, unsigned int type) {
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, size, data, type);
}

VBO_Class::~VBO_Class() {
    glDeleteBuffers(1, &m_ID);
}

void VBO_Class::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void VBO_Class::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
