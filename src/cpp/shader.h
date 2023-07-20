#pragma once

#ifdef BUILD_ANDROID
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#endif

#ifdef BUILD_DESKTOP
#include <glad/glad.h>
#endif

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ECS.h"
#include "buffers.h"
#include "util.h"

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

#ifdef BUILD_DESKTOP
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

#ifdef BUILD_DESKTOP
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

    ~Shader();

    void LoadShader(const std::vector<GLuint>& shaders, const std::vector<ShaderVariable>& variables);
    void Use() const { glUseProgram(m_Program); }

    virtual void SetVPMatrix(glm::f32* viewMatrix, glm::f32* projectionMatrix) = 0;
    virtual void SetPerRenderObject(const std::vector<const Entity*>& entities) = 0;
    virtual void Draw(const RenderBuffer* renderBuffer) = 0;

    GLuint GetProgram() const { return m_Program; }
    const std::vector<GLuint>& GetVariables() const { return m_Variables; }

protected:
    Shader();
    GLuint m_Program;
    std::vector<GLuint> m_Variables;
    std::vector<GLuint> m_Shaders;
};

#define DEFINE_SHADER_ENUM_CLASS_LIST(MACRO)                \
    MACRO(SOLID_COLOR, SolidColorShader)                    \
    MACRO(SOLID_COLOR_INSTANCED, SolidColorShaderInstanced) \
    MACRO(PIXEL_COLOR, PixelColorShader)

#define GENERATE_ENUM_VALUE(name, className) name,
enum class ShaderType
{
    DEFINE_SHADER_ENUM_CLASS_LIST(GENERATE_ENUM_VALUE)
    COUNT
};

#define GENERATE_SUB_CLASSES(name, className)                                    \
class className : public Shader {                                                \
public:                                                                          \
    className();                                                                 \
    void SetVPMatrix(glm::f32* viewMatrix, glm::f32* projectionMatrix) override; \
    void SetPerRenderObject(const std::vector<const Entity*>& entities) override;      \
    void Draw(const RenderBuffer* renderBuffer) override;                        \
};
DEFINE_SHADER_ENUM_CLASS_LIST(GENERATE_SUB_CLASSES)

#define GENERATE_CASE_VALUE(name, className) case ShaderType::name: return new className();

class ShaderManager
{
public:
    ShaderManager();
    ~ShaderManager();

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
#undef GENERATE_SUB_CLASSES
#undef GENERATE_CASE_VALUE
#undef DEFINE_SHADER_ENUM_CLASS_LIST
