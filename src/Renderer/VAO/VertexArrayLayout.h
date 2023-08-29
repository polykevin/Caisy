
/*Copyright 2022 Kevin Poly*/
#pragma once
#include <glad/glad.h>
#include <vector>
#include <cassert>

struct VertexArrayElements {
    unsigned int count;
    unsigned int type;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type) {
        switch(type) {
            case GL_FLOAT:          return 4;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_BYTE:  return 1;
        }
        return 0;
    }
};

class VertexArrayLayout {
public:
    VertexArrayLayout()
        :m_Stride(0) {}

    void Push(unsigned count) {
        assert(false);
    }

    void PushFloat (unsigned int count) {
        m_Elements.push_back({count, GL_FLOAT, GL_FALSE});
        m_Stride += count * VertexArrayElements::GetSizeOfType(GL_FLOAT);
    }

    void PushUnsignedInt (unsigned int count) {
        m_Elements.push_back({count, GL_UNSIGNED_INT, GL_FALSE});
        m_Stride += count * VertexArrayElements::GetSizeOfType(GL_UNSIGNED_INT);
    }

    void PushUnsignedChar (unsigned int count) {
        m_Elements.push_back({count, GL_UNSIGNED_BYTE, GL_TRUE});
        m_Stride += count * VertexArrayElements::GetSizeOfType(GL_UNSIGNED_BYTE);
    }

    inline const std::vector<VertexArrayElements> GetElements() const& { return m_Elements; }
    inline unsigned int GetStride() const { return m_Stride; }
private:
    std::vector<VertexArrayElements> m_Elements;
    unsigned int m_Stride;
};
