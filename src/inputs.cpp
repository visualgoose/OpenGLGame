#include "inputs.h"

#include "client.h"

namespace OGLGAME
{
    void RegisterInputs()
    {
        InputSystem::Bind bind;
        bind.m_scancodes[0] = SDL_SCANCODE_W;
        g_pForward = &InputSystem::GetBind(InputSystem::RegisterBind(bind));

        bind.m_scancodes[0] = SDL_SCANCODE_A;
        g_pLeft = &InputSystem::GetBind(InputSystem::RegisterBind(bind));

        bind.m_scancodes[0] = SDL_SCANCODE_S;
        g_pBackward = &InputSystem::GetBind(InputSystem::RegisterBind(bind));

        bind.m_scancodes[0] = SDL_SCANCODE_D;
        g_pRight = &InputSystem::GetBind(InputSystem::RegisterBind(bind));
    }
}