#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <string>
#include <unordered_map>

class Shader
{
public:
    static Shader* Create(const std::string& filePath)
    {
        return new Shader(filePath);
    }
    static Shader* Create(const std::string& name, const std::string& vertexShader, const std::string& fragmentShader)
    {
        return new Shader(name, vertexShader, fragmentShader);
    }
public:
    Shader(const std::string& filePath);
    Shader(const std::string& name, const std::string& vertexShader, const std::string& fragmentShader);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetInt(const std::string& name, int value);
    void SetFloat3(const std::string& name, const glm::vec3& value);
    void SetFloat4(const std::string& name, const glm::vec4& value);
    void SetMat4(const std::string& name, const glm::mat4& value);

    void UploadUniformInt(const std::string& name, int value);
    void UploadUniformFloat(const std::string& name, float value);
    void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
    void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
    void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

    void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
    void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

    const std::string& GetName() const { return m_Name; }

private:
    GLint GetUniformLocation(const std::string& name);
    std::string ReadFile(const std::string& filepath);
	std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
    void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
private:
    uint32_t m_ProgramID;
    std::string m_Name;
    std::unordered_map<std::string, GLint> m_UniformLocation;
};
