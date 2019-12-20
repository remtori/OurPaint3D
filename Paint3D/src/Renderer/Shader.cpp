#include "pch.h"
#include "Shader.h"

#include <fstream>

#include <glm/gtc/type_ptr.hpp>

static GLenum ShaderTypeFromString(const std::string& type)
{
    if (type == "vertex")
        return GL_VERTEX_SHADER;
    if (type == "fragment" || type == "pixel")
        return GL_FRAGMENT_SHADER;

    ASSERT(false, "Unknown shader type '{0}'", type);
    return 0;
}

Shader::Shader(const std::string& filePath)
{
    std::string source = ReadFile(filePath);
    auto shaderSources = PreProcess(source);
    Compile(shaderSources);

    // Extract name from filePath
    auto lastSlash = filePath.find_last_of("/\\");
    lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    auto lastDot = filePath.rfind('.');
    auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
    m_Name = filePath.substr(lastSlash, count);
}

Shader::Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    : m_Name(name)
{
    std::unordered_map<GLenum, std::string> sources;
    sources[GL_VERTEX_SHADER] = vertexSrc;
    sources[GL_FRAGMENT_SHADER] = fragmentSrc;
    Compile(sources);
}

Shader::~Shader()
{
    glDeleteProgram(m_ProgramID);
}

std::string Shader::ReadFile(const std::string& filePath)
{
    std::string result;
    std::ifstream inp(filePath, std::ios::binary | std::ios::in);

    if (inp)
    {
        inp.seekg(0, std::ios::end);
        size_t size = inp.tellg();
        if (size != -1)
        {
            result.resize(size);
            inp.seekg(0, std::ios::beg);
            inp.read(&result[0], size);
            inp.close();
        }
        else
        {
            LOG_ERROR("Could not read file '{0}'", filePath);
        }
    }
    else
    {
        LOG_ERROR("Could not open file '{0}'", filePath);
    }

	return result;
}

std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string& source)
{
    std::unordered_map<GLenum, std::string> shaderSources;

    const char* typeToken = "#type";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos = source.find(typeToken, 0);

    while (pos != std::string::npos)
    {
        //  End of shader type declaration line, must be CRLF
        size_t eol = source.find_first_of("\r\n", pos);
        ASSERT(eol != std::string::npos, "Syntax error");

        // Start of shader type name (after "#type " keyword)
        size_t begin = pos + typeTokenLength + 1;
        std::string type = source.substr(begin, eol - begin);
        GLenum shaderType = ShaderTypeFromString(type);
        ASSERT(shaderType, "Invalid shader type specified");

        // Start of shader code after shader type declaration line
        size_t nextLinePos = source.find_first_not_of("\r\n", eol);
        ASSERT(nextLinePos != std::string::npos, "Syntax error");

        // Start of next shader type declaration line
        pos = source.find(typeToken, nextLinePos);

        shaderSources[shaderType] = (pos == std::string::npos)
            ? source.substr(nextLinePos)
            : source.substr(nextLinePos, pos - nextLinePos);
    }

    return shaderSources;
}

void Shader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
{
    m_ProgramID = glCreateProgram();
    std::array<GLenum, 2> glShaderIDs;

    int glShaderIDIndex = 0;
    for (auto& kv : shaderSources)
    {
        GLenum type = kv.first;
        const std::string& source = kv.second;

        GLuint shader = glCreateShader(type);

        const GLchar* sourceCStr = source.c_str();
        glShaderSource(shader, 1, &sourceCStr, 0);

        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(shader);

            LOG_ERROR("{0}", infoLog.data());
            ASSERT(false, "Shader compilation failure!");
            break;
        }

        glAttachShader(m_ProgramID, shader);
        glShaderIDs[glShaderIDIndex++] = shader;
    }

    glLinkProgram(m_ProgramID);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(m_ProgramID, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(m_ProgramID, maxLength, &maxLength, &infoLog[0]);

        glDeleteProgram(m_ProgramID);

        for (auto id : glShaderIDs)
            glDeleteShader(id);

        LOG_ERROR("{0}", infoLog.data());
        ASSERT(false, "Shader link failure!");
        return;
    }

    for (auto id : glShaderIDs)
    {
        glDetachShader(m_ProgramID, id);
        glDeleteShader(id);
    }
}

void Shader::Bind() const
{
    glUseProgram(m_ProgramID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetInt(const std::string& name, int value)
{
    UploadUniformInt(name, value);
}

void Shader::SetFloat3(const std::string& name, const glm::vec3& value)
{
    UploadUniformFloat3(name, value);
}

void Shader::SetFloat4(const std::string& name, const glm::vec4& value)
{
    UploadUniformFloat4(name, value);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value)
{
    UploadUniformMat4(name, value);
}

GLint Shader::GetUniformLocation(const std::string& name)
{
    GLint location;
    auto it = m_UniformLocation.find(name);

    if (it == m_UniformLocation.cend())
    {
        location = glGetUniformLocation(m_ProgramID, name.c_str());
        m_UniformLocation.insert({ name, location });
    }
    else
        location = it->second;

    return location;
}

void Shader::UploadUniformInt(const std::string& name, int value)
{
    glUniform1i(Shader::GetUniformLocation(name), value);
}

void Shader::UploadUniformFloat(const std::string& name, float value)
{
    glUniform1f(Shader::GetUniformLocation(name), value);
}

void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
{
    glUniform2f(Shader::GetUniformLocation(name), value.x, value.y);
}

void Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
{
    glUniform3f(
        Shader::GetUniformLocation(name),
        value.x, value.y, value.z
    );
}

void Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
{
    glUniform4f(
        Shader::GetUniformLocation(name),
        value.x, value.y, value.z, value.w
    );
}

void Shader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
{
    glUniformMatrix3fv(
        Shader::GetUniformLocation(name),
        1, GL_FALSE, glm::value_ptr(matrix)
    );
}

void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(
        Shader::GetUniformLocation(name),
        1, GL_FALSE, glm::value_ptr(matrix)
    );
}
