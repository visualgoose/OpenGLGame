#pragma once

#include <glad/glad.h>

namespace OGLGAME
{
    void APIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
        GLsizei length, const GLchar* pMessage,
        const void* pUserParam);
}