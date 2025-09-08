#include "input_system.h"

#include "client.h"
#include "vgassert.h"

namespace OGLGAME
{
    InputSystem::BindIndex InputSystem::RegisterBind(const Bind& bind)
    {
        return Client::GetInstance().GetInputSystem().M_RegisterBind(bind);
    }

    const InputSystem::Bind& InputSystem::GetBind(const BindIndex bindIndex)
    {
        return Client::GetInstance().GetInputSystem().M_GetBind(bindIndex);
    }

    void InputSystem::Frame()
    {
        Client::GetInstance().GetInputSystem().M_Frame();
    }

    void InputSystem::UpdateMouseDelta(const glm::vec2 mouseDelta)
    {
        Client::GetInstance().GetInputSystem().M_UpdateMouseDelta(mouseDelta);
    }

    void InputSystem::UpdateBinds(const SDL_Scancode scancode, const bool state)
    {
        Client::GetInstance().GetInputSystem().M_UpdateBinds(scancode, state);
    }

    glm::vec2 InputSystem::GetMouseDelta()
    {
        return Client::GetInstance().GetInputSystem().M_GetMouseDelta();
    }

    InputSystem::InputSystem()
    {
        m_binds.reserve(16);
    }

    InputSystem::BindIndex InputSystem::M_RegisterBind(const Bind& bind)
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

    const InputSystem::Bind& InputSystem::M_GetBind(const BindIndex bindIndex) const
    {
        vgassert(bindIndex < m_binds.size());

        return m_binds[bindIndex];
    }

    void InputSystem::M_Frame()
    {
        m_mouseDelta.x = 0.0f;
        m_mouseDelta.y = 0.0f;
        for (auto& bind : m_binds)
        {
            bind.m_prevState = bind.m_state;
        }
    }

    void InputSystem::M_UpdateMouseDelta(const glm::vec2 mouseDelta)
    {
        m_mouseDelta = mouseDelta;
    }

    void InputSystem::M_UpdateBinds(const SDL_Scancode scancode, const bool state)
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