/*Copyright 2022 Kevin Poly*/
#pragma once
#include "../VBO/VBO.h"
#include "VertexArrayLayout.h"

class VAO_Class {
public:
    VAO_Class();
    ~VAO_Class();
    void AddBuffer(VBO_Class& VBO, const VertexArrayLayout& layout);
    void Bind() const;
    void Unbind() const;
private:
    unsigned int m_ID;
};

