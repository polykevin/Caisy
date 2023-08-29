/*Copyright 2022 Kevin Poly*/
#pragma once
#include <glad/glad.h>

class EBO_Class {
    public:
        EBO_Class(const unsigned int* data = nullptr, unsigned int count = 0, unsigned int type = GL_STATIC_DRAW);
        ~EBO_Class();
        void Bind() const;
        void Unbind() const;
    private:
        unsigned int m_ID;
};
