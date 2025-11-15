//
// Created by Lazar on 11/14/2025.
//


#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <platform.h>
#include <shaders.h>

static SDL_DisplayMode window_size;

static const char* window_title{platform::windows::TITLE};

static constexpr int window_width{platform::windows::WIDTH};
static constexpr int window_height{platform::windows::HEIGHT};

int SDL_main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        std::exit(shaders::status::ERROR);
    }

    // OpenGL
    SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "0");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    //

    SDL_GetCurrentDisplayMode(0, &window_size);

    const int position_x = (window_size.w - window_width) / 2;
    const int position_y = (window_size.h - window_height) / 2;

    SDL_Window* window = SDL_CreateWindow(window_title,
                                          position_x, position_y,
                                          window_width, window_height,
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return 1;
    }

    SDL_GLContext glctx = SDL_GL_CreateContext(window);
    if (!glctx)
    {
        SDL_Log("SDL_GL_CreateContext failed: %s", SDL_GetError());
        std::exit(1);
    }
    if (SDL_GL_MakeCurrent(window, glctx) != 0)
    {
        SDL_Log("SDL_GL_MakeCurrent failed: %s", SDL_GetError());
        std::exit(1);
    }

    SDL_GL_SetSwapInterval(1);

    glewExperimental = GL_TRUE;
    const GLenum glew_status = glewInit();

    glGetError();
    if (glew_status != GLEW_OK)
    {
        SDL_Log("Failed to initialize GLEW: %s", glewGetErrorString(glew_status));
        std::exit(shaders::status::ERROR);
    }

    // Test
    const char* vs_src = R"(
        #version 330 core
        layout(location=0) in vec2 aPos;
        layout(location=1) in vec3 aColor;
        out vec3 vColor;
        void main() {
            vColor = aColor;
            gl_Position = vec4(aPos, 0.0, 1.0);
        }
    )";

    const char* fs_src = R"(
        #version 330 core
        in vec3 vColor;
        out vec4 FragColor;
        void main() {
            FragColor = vec4(vColor, 1.0);
        }
    )";

    GLuint prog = shaders::link_program(shaders::compile(GL_VERTEX_SHADER, vs_src),
                                        shaders::compile(GL_FRAGMENT_SHADER, fs_src));

    float verts[] = {
        //  x,     y,    r,    g,    b
        0.0f, 0.7f, 1.0f, 0.2f, 0.2f,
        -0.7f, -0.7f, 0.2f, 1.0f, 0.2f,
        0.7f, -0.7f, 0.2f, 0.2f, 1.0f
    };

    GLuint vao = 0;
    GLuint vbo = 0;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(nullptr));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));

    glBindVertexArray(0);

    glViewport(0, 0, window_width, window_height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //

    SDL_Event event;

    bool running = true;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(prog);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        SDL_GL_SwapWindow(window);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(prog);

    SDL_GL_DeleteContext(SDL_GL_GetCurrentContext());
    SDL_DestroyWindow(window);

    SDL_Quit();

    return shaders::status::OK;
}
