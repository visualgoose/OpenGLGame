#pragma once

#include "input_system.h"

namespace OGLGAME
{
    inline const InputSystem::Bind* g_pForward  = nullptr;
    inline const InputSystem::Bind* g_pBackward = nullptr;
    inline const InputSystem::Bind* g_pLeft     = nullptr;
    inline const InputSystem::Bind* g_pRight    = nullptr;

    void RegisterInputs();
}