#pragma once

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include "util.h"

class Shader {
public:
    static GLuint SOLID_COLOR_SHADER;

    static GLuint CompileShader(std::vector<char> source, GLenum shaderType) {
        GLuint shaderId = glCreateShader(shaderType);
        const GLchar* rawSource = (GLchar*)&source[0];
        int32_t size = source.size();
        glShaderSource(shaderId, 1, &rawSource, &size);
        glCompileShader(shaderId);

#ifdef DEBUG
        GLint logLength;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            GLchar* log = (GLchar*)malloc(logLength);
            glGetShaderInfoLog(shaderId, logLength, &logLength, log);
            LOGE("Shader compile failed:\n%s", log);
            free(log);
            THROW("Shader compile failed");
        }
#endif
        return shaderId;
    }

    static GLuint LinkShader(GLuint vs, GLuint fs) {
        GLuint program = glCreateProgram();
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);

#ifdef DEBUG
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            GLchar* log = (GLchar*)malloc(logLength);
            glGetProgramInfoLog(program, logLength, &logLength, log);
            LOGE("Program link failed:\n%s", log);
            free(log);
            THROW("Program link failed");
        }
#endif

        GLint status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if (status == 0) {
            glDeleteShader(vs);
            glDeleteShader(fs);
            glDeleteProgram(program);

            THROW("Program link failed");
        }

        return program;
    }
};
