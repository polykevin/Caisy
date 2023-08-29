/*Copyright 2022 Kevin Poly*/
#pragma once
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <glm/glm.hpp>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
    public:
        Shader(const std::string& filepath);
        ~Shader();
        void Bind() const;
        void Unbind() const;
        void SetMat4(const std::string& name, int count, GLboolean transpose, const glm::mat4& mat) const;
        void Set3F(const std::string& name, const glm::vec3& data) const;
        void Set1F(const std::string& name, const float data) const;
    private:
        unsigned int m_ID;
        std::string m_FilePath;
        unsigned int CompileShader(unsigned int type, const std::string& source);
        unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
        ShaderProgramSource ParseShader(const std::string& filepath);
};

