#include "gl_debug.h"

#include "logging.h"

#include <SDL3/SDL_video.h>

namespace OGLGAME
{
    void APIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
        GLsizei length, const GLchar* pMessage,
        const void* pUserParam)
    {
        //ignore non-significant error/warning codes
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
            return;

        const char* pTypeName;
        Logger::LogType logType = Logger::LogType_info;
        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR:               pTypeName = "Type: Error";                logType = Logger::LogType_error; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: pTypeName = "Type: Deprecated Behaviour"; logType = Logger::LogType_warning; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  pTypeName = "Type: Undefined Behaviour";  logType = Logger::LogType_error; break;
            case GL_DEBUG_TYPE_PORTABILITY:         pTypeName = "Type: Portability";          logType = Logger::LogType_warning; break;
            case GL_DEBUG_TYPE_PERFORMANCE:         pTypeName = "Type: Performance";          logType = Logger::LogType_warning; break;
            case GL_DEBUG_TYPE_MARKER:              pTypeName = "Type: Marker";               logType = Logger::LogType_info; break;
            case GL_DEBUG_TYPE_PUSH_GROUP:          pTypeName = "Type: Push Group";           logType = Logger::LogType_info; break;
            case GL_DEBUG_TYPE_POP_GROUP:           pTypeName = "Type: Pop Group";            logType = Logger::LogType_info; break;
            case GL_DEBUG_TYPE_OTHER:               pTypeName = "Type: Other";                logType = Logger::LogType_info; break;
            default:                                pTypeName = "Type: Unknown";              logType = Logger::LogType_info; break;
        }
        LogContinue& logContinue = g_log.PrintType(logType, "---OpenGL Debug Message---")
            .NextLine("{}", pTypeName);

        const char* pSourceName;
        switch (source)
        {
            case GL_DEBUG_SOURCE_API:               pSourceName = "Source: API"; break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     pSourceName = "Source: Window System"; break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER:   pSourceName = "Source: Shader Compiler"; break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:       pSourceName = "Source: Third Party"; break;
            case GL_DEBUG_SOURCE_APPLICATION:       pSourceName = "Source: Application"; break;
            case GL_DEBUG_SOURCE_OTHER:             pSourceName = "Source: Other"; break;
            default:                                pSourceName = "Source: Unknown"; break;
        }
        logContinue.NextLine("{}", pSourceName);

        const char* pSeverityName;
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:            pSeverityName = "Severity: HIGH"; break;
            case GL_DEBUG_SEVERITY_MEDIUM:          pSeverityName = "Severity: Medium"; break;
            case GL_DEBUG_SEVERITY_LOW:             pSeverityName = "Severity: Low"; break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:    pSeverityName = "Severity: Notification"; break;
            default:                                pSeverityName = "Severity: Unknown"; break;
        }
        logContinue.NextLine("{}", pSeverityName);

        logContinue.NextLine("ID: {}", id)
            .NextLine("Message: {}", g_log.LF2Indent(pMessage));
    }
}

