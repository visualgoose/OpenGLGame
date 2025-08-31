#include "input_system.h"

#include "vgassert.h"

namespace OGLGAME
{
    InputSystem::InputSystem()
    {
        m_binds.resize(16);
    }

    InputSystem::BindIndex InputSystem::RegisterBind(const Bind& bind)
    {
        const BindIndex bindIndex = m_binds.size();
        Bind& registeredBind = m_binds.emplace_back(bind);
        registeredBind.m_bindIndex = bindIndex;
        for (const auto& scancode : registeredBind.m_scancodes)
        {
            if (scancode != SDL_SCANCODE_UNKNOWN)
                registeredBind.m_pressedNeeded++;
        }
        return bindIndex;
    }

    const InputSystem::Bind& InputSystem::GetBind(const BindIndex bindIndex) const
    {
        vgassert(bindIndex < m_binds.size());

        return m_binds[bindIndex];
    }

    void InputSystem::Frame()
    {
        m_mouseDelta.x = 0.0f;
        m_mouseDelta.y = 0.0f;
        for (auto& bind : m_binds)
        {
            bind.m_prevState = bind.m_state;
        }
    }

    void InputSystem::UpdateMouseDelta(const glm::vec2 mouseDelta)
    {
        m_mouseDelta = mouseDelta;
    }

    void InputSystem::UpdateBinds(const SDL_Scancode scancode, const bool state)
    {
        for (auto& bind : m_binds)
        {
            for (size_t i = 0; i < c_maxScancodesPerBind; i++)
            {
                if (bind.m_scancodes[i] == scancode)
                    bind.m_scancodesPressed[i] = state;
            }
            size_t pressed = 0;
            for (const bool scancodePressed : bind.m_scancodesPressed)
            {
                if (scancodePressed)
                    pressed++;
            }
            bind.m_state = pressed == bind.m_pressedNeeded;
        }
    }
}