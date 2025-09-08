#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_time.h>
#include <SDL3/SDL_timer.h>

#include <SDL3_image/SDL_image.h>

#include <glad/glad.h>

#include <filesystem>

#include "components/transform.h"

namespace fs = std::filesystem;

#include "logging.h"
#include "file_system.h"
#include "client.h"
#include "scene.h"

#include "inputs.h"

#include "components/model_filter.h"

#include "gl_debug.h"

using namespace OGLGAME;

int main(int argCount, char** ppArgs)
{
    const fs::path engineRootDir = FS::GetExePath().parent_path().parent_path().parent_path();
    if (!fs::is_directory(engineRootDir))
    {
        g_log.Fatal("Engine executable should be in <game path>/Bin/"
#ifdef OGLGAME_PLATFORM_WINDOWS
            "Win"
#elif OGLGAME_PLATFORM_LINUX
            "Linux"
#endif
            "-" OGLGAME_BUILD_TYPE);
        return -1;
    }

    //set engine's root directory as current directory
    fs::current_path(engineRootDir);
    g_log.Info("Engine directory: {}", engineRootDir.string());

    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        g_log.Fatal("SDL3 failed to initialize:")
            .NextLine("{}", SDL_GetError());
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, GL_TRUE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#ifdef OGLGAME_SHOWDBGINFO
    int flags = 0;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &flags);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, flags | SDL_GL_CONTEXT_DEBUG_FLAG);
#endif
    SDL_Window* pWindow = SDL_CreateWindow("OpenGLGame window", 1600, 900, SDL_WINDOW_OPENGL);
    if (!pWindow)
    {
        g_log.Fatal("Failed to create a window:")
            .NextLine("{}", SDL_GetError());
        return -1;
    }
    SDL_GLContext openGLContext = SDL_GL_CreateContext(pWindow);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
    {
        g_log.Fatal("Glad failed to load");
        return -1;
    }

#ifdef OGLGAME_SHOWDBGINFO
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif
    {
        Client client(pWindow); //creates Client singleton

        GameObject* pGameObject = Scene::AllocGameObject();
        auto* pModelFilter = pGameObject->AddComponent<Components::ModelFilter>();
        pModelFilter->SetModel("models/test.obj");
        auto* pTransform = pGameObject->AddComponent<Components::Transform>();

        pGameObject = Scene::AllocGameObject();
        pModelFilter = pGameObject->AddComponent<Components::ModelFilter>();
        pModelFilter->SetModel("models/test.obj");
        pTransform = pGameObject->AddComponent<Components::Transform>();
        pTransform->m_position.z += 10.0f;

        RegisterInputs();

        SDL_Event event;
        bool shouldNotClose = true;
        uint64_t start;
        uint64_t end;
        double deltaTime = 1.0;

        SDL_GL_SetSwapInterval(0);

        SDL_Cursor* pCursor = SDL_GetCursor();

        while (shouldNotClose)
        {
            start = SDL_GetTicksNS();
            InputSystem::Frame();
            while (shouldNotClose && SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                        shouldNotClose = false;
                        break;
                    case SDL_EVENT_MOUSE_BUTTON_DOWN:
                        if (event.button.button == SDL_BUTTON_LEFT)
                            SDL_SetWindowRelativeMouseMode(pWindow, true);
                        break;
                    case SDL_EVENT_KEY_DOWN:
                        if (event.key.scancode == SDL_SCANCODE_ESCAPE)
                        {
                            SDL_SetWindowKeyboardGrab(pWindow, false);
                            SDL_SetWindowMouseGrab(pWindow, false);
                            SDL_SetWindowRelativeMouseMode(pWindow, false);
                            break;
                        }
                        InputSystem::UpdateBinds(event.key.scancode, true);
                        break;
                    case SDL_EVENT_KEY_UP:
                        InputSystem::UpdateBinds(event.key.scancode, false);
                        break;
                    case SDL_EVENT_MOUSE_MOTION:
                        InputSystem::UpdateMouseDelta(glm::vec2(event.motion.xrel, event.motion.yrel));
                        break;
                    default:
                        break;
                }
            }

            Scene::Frame(deltaTime);
            Renderer::Render();
            SDL_GL_SwapWindow(pWindow);
            end = SDL_GetTicksNS();
            deltaTime = (double)(end - start) / 1000000000.0;
        }
    }

    SDL_GL_DestroyContext(openGLContext);
    SDL_DestroyWindow(pWindow);
    SDL_GL_UnloadLibrary();
    SDL_Quit();
}