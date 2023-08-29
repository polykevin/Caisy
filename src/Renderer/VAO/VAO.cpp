/*Copyright 2022 Kevin Poly*/
#include "VAO.h"
#include <glad/glad.h>

VAO_Class::VAO_Class() {
    glGenVertexArrays(1, &m_ID);
}

VAO_Class::~VAO_Class() {
    glDeleteVertexArrays(1,&m_ID);
}

void VAO_Class::AddBuffer(VBO_Class& VBO, const VertexArrayLayout& layout) {
    VBO.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (int i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
        offset += element.count * VertexArrayElements::GetSizeOfType(element.type);
    }
}

void VAO_Class::Bind() const {
    glBindVertexArray(m_ID);
}

void VAO_Class::Unbind() const {
    glBindVertexArray(0);
}
