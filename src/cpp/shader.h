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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ECS.h"
#include "buffers.h"
#include "util.h"

#define DEBUG

enum ShaderVariableType
{
    ATTRIBUTE = 0,
    UNIFORM
};

struct ShaderVariable
{
    ShaderVariableType Type;
    const GLchar* Name;
};

class Shader
{
public:
    static GLuint CompileShader(const std::vector<char>& source, GLenum shaderType)
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
            LOGE("Shader compile failed: %s\n%s", source, log);
            free(log);
            THROW("Shader compile failed");
        }
#endif
        return shaderId;
    }

    static GLuint LinkShader(const std::vector<GLuint>& shaders)
    {
        GLuint program = glCreateProgram();
        for (auto s : shaders)
        {
            glAttachShader(program, s);
        }
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
            for (auto s : shaders) {
                glDeleteShader(s);
            }
            glDeleteProgram(program);

            THROW("Program link failed");
        }

        return program;
    }

    Shader();
    Shader(const std::vector<GLuint>& shaders, const std::vector<ShaderVariable>& variables);
    ~Shader();

    void Use() const { glUseProgram(m_Program); }

    virtual void SetVPMatrix(glm::f32* viewMatrix, glm::f32* projectionMatrix) = 0;
    virtual void SetPerEntity(const Entity* entity) = 0;
    virtual void SetPerMaterial(const Components::Material* material) = 0;
    virtual void Draw(const RenderBuffer* renderBuffer) = 0;

    GLuint GetProgram() const { return m_Program; }
    const std::vector<GLuint>& GetVariables() const { return m_Variables; }

protected:
    GLuint m_Program;
    std::vector<GLuint> m_Variables;
    std::vector<GLuint> m_Shaders;
};

class SolidColorShader : public Shader
{
public:
    SolidColorShader();
    void SetVPMatrix(glm::f32* viewMatrix, glm::f32* projectionMatrix) override;
    void SetPerEntity(const Entity* entity) override;
    void SetPerMaterial(const Components::Material* material) override;
    void Draw(const RenderBuffer* renderBuffer) override;
};

class PixelColorShader : public Shader
{
public:
    PixelColorShader();
    void SetVPMatrix(glm::f32* viewMatrix, glm::f32* projectionMatrix) override;
    void SetPerEntity(const Entity* entity) override;
    void SetPerMaterial(const Components::Material* material) override;
    void Draw(const RenderBuffer* renderBuffer) override;
};

#define DEFINE_SHADER_ENUM_CLASS_LIST(MACRO) \
    MACRO(SOLID_COLOR, SolidColorShader) \
    MACRO(PIXEL_COLOR, PixelColorShader)

#define GENERATE_ENUM_VALUE(name, func) name,
enum class ShaderType
{
    DEFINE_SHADER_ENUM_CLASS_LIST(GENERATE_ENUM_VALUE)
    COUNT
};

class ShaderManager
{
public:
    ShaderManager();
    ~ShaderManager();

#define GENERATE_CASE_VALUE(name, func) case ShaderType::name: return new func();

    Shader* CreateShader(ShaderType type)
    {
        switch(type)
        {
            DEFINE_SHADER_ENUM_CLASS_LIST(GENERATE_CASE_VALUE)
        }
        return nullptr;
    }
    Shader* GetShader(ShaderType type);

    void Cleanup();
private:
    std::vector<Shader*> m_Shaders;
};

#undef GENERATE_ENUM_VALUE
#undef GENERATE_CASE_VALUE
#undef DEFINE_SHADER_ENUM_CLASS_LIST
