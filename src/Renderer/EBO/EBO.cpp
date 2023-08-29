/*Copyright 2022 Kevin Poly*/
#include "EBO.h"

EBO_Class::EBO_Class(const unsigned int* data, unsigned int count, unsigned int type) {
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, type);
}

EBO_Class::~EBO_Class() {
    glDeleteBuffers(1, &m_ID);
}

void EBO_Class::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void EBO_Class::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
