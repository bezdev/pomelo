#pragma once

#ifdef BUILD_ANDROID
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#endif

#ifdef BUILD_DESKTOP
#include <glad/glad.h>
#endif

#include <vector>
#include <unordered_map>

#include "util.h"

enum class ShaderType
{
    SOLID_COLOR
};

enum ShaderVariableType
{
    ATTRIBUTE = 0,
    UNIFORM
};

struct ShaderVariable {
    ShaderVariableType Type;
    const GLchar* Name;
};

class Shader
{
public:
    static Shader* SOLID_COLOR_SHADER;

    Shader(GLuint program, std::vector<ShaderVariable>& variables);
    ~Shader();

    GLuint GetProgram() const { return m_Program; }
    const std::vector<GLuint>& GetVariables() const { return m_Variables; }

    static GLuint CompileShader(const char* name, const std::vector<char>& source, GLenum shaderType)
    {
        GLuint shaderId = glCreateShader(shaderType);
        const GLchar* rawSource = (GLchar*)&source[0];
        int32_t size = source.size();
        glShaderSource(shaderId, 1, &rawSource, &size);
        glCompileShader(shaderId);

#ifdef DEBUG
        GLint logLength;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0)
        {
            GLchar* log = (GLchar*)malloc(logLength);
            glGetShaderInfoLog(shaderId, logLength, &logLength, log);
            LOGE("Shader compile failed: %s\n%s", name, log);
            free(log);
            THROW("Shader compile failed");
        }
#endif
        return shaderId;
    }

    static GLuint LinkShader(GLuint vs, GLuint fs)
    {
        GLuint program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);

#ifdef DEBUG
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0)
        {
            GLchar* log = (GLchar*)malloc(logLength);
            glGetProgramInfoLog(program, logLength, &logLength, log);
            LOGE("Program link failed:\n%s", log);
            free(log);
            THROW("Program link failed");
        }
#endif

        GLint status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if (status == 0)
        {
            glDeleteShader(vs);
            glDeleteShader(fs);
            glDeleteProgram(program);

            THROW("Program link failed");
        }

        return program;
    }
private:
    Shader();

    GLuint m_Program;
    std::vector<GLuint> m_Variables;
};

class ShaderManager
{
public:
    static Shader* GetShader(ShaderType type)
    {

    }

    static void ClearShaders()
    {
        for (const auto& pair : s_ShaderMap)
        {
            delete pair.second;
        }
    }
private:
    static std::unordered_map<ShaderType, Shader*> s_ShaderMap;
};
