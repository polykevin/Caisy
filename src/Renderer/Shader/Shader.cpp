/*Copyright 2022 Kevin Poly*/
#include "Shader.h"
#include <ostream>
#include <sstream>

Shader::Shader(const std::string& shaderFile) 
    : m_FilePath(shaderFile)
{
    ShaderProgramSource source = ParseShader(m_FilePath);
    m_ID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
    glDeleteProgram(m_ID);
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Shader::Bind() const {
    glUseProgram(m_ID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

void Shader::SetMat4(const std::string &name, int count, GLboolean transpose, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), count, transpose, &mat[0][0]);
}

void Shader::Set3F(const std::string& name, const glm::vec3& data) const {
    glUniform3f(glGetUniformLocation(m_ID, name.c_str()), data.x, data.y, data.z);
}

void Shader::Set1F(const std::string& name, const float data) const {
    glUniform1f(glGetUniformLocation(m_ID, name.c_str()), data);
}