/*Copyright 2022 Kevin Poly*/
#pragma once
#include <glad/glad.h>

class VBO_Class {
    public:
        VBO_Class(const void* data = nullptr, unsigned int size = 0, unsigned int type = GL_STATIC_DRAW);
        ~VBO_Class();
        void Bind() const;
        void Unbind() const;
    private:
        unsigned int m_ID;
};
