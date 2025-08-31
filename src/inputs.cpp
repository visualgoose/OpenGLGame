#include "inputs.h"

#include "client.h"

namespace OGLGAME
{
    void RegisterInputs()
    {
        InputSystem& inputSystem = Client::S_GetInstance().GetInputSystem();

        InputSystem::Bind bind;
        bind.m_scancodes[0] = SDL_SCANCODE_W;
        g_pForward = &inputSystem.GetBind(inputSystem.RegisterBind(bind));

        bind.m_scancodes[0] = SDL_SCANCODE_A;
        g_pLeft = &inputSystem.GetBind(inputSystem.RegisterBind(bind));

        bind.m_scancodes[0] = SDL_SCANCODE_S;
        g_pBackward = &inputSystem.GetBind(inputSystem.RegisterBind(bind));

        bind.m_scancodes[0] = SDL_SCANCODE_D;
        g_pRight = &inputSystem.GetBind(inputSystem.RegisterBind(bind));
    }
}