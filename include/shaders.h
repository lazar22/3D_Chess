//
// Created by Lazar on 11/15/2025.
//

#ifndef SHADERS_H
#define SHADERS_H

#include <cstdlib>
#include <SDL_log.h>
#include <gl/glew.h>

namespace shaders
{
    namespace status
    {
        enum
        {
            OK = 0,
            ERROR
        };
    }

    static GLuint compile(const GLenum type, const char* source)
    {
        const GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        GLint status = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        if (status != GL_TRUE)
        {
            char log[1024];
            GLsizei len = 0;
            glGetShaderInfoLog(shader, sizeof(log), &len, log);
            SDL_Log("Failed to compile %s shader: %.*s",
                    (type == GL_VERTEX_SHADER ? "vertex" : type == GL_FRAGMENT_SHADER ? "fragment" : "unknown"),
                    static_cast<int>(len), log);
            std::exit(status::ERROR);
        }

        return shader;
    }

    static GLuint link_program(const GLuint vs, const GLuint fs)
    {
        const GLuint program = glCreateProgram();

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);

        GLint status = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &status);

        if (status != GL_TRUE)
        {
            char log[1024];
            GLsizei len = 0;
            glGetProgramInfoLog(program, sizeof(log), &len, log);
            SDL_Log("Failed to link program: %.*s", static_cast<int>(len), log);
            std::exit(status::ERROR);
        }

        glDetachShader(program, vs);
        glDetachShader(program, fs);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }
}

#endif //SHADERS_H
