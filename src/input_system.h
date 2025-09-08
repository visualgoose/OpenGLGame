#pragma once

#include <functional>
#include <vector>
#include <SDL3/SDL_keycode.h>

#include <glm/vec2.hpp>

namespace OGLGAME
{
    class InputSystem
    {
    public: //data types and constants
        using BindIndex = size_t;
        static constexpr BindIndex c_invalidBindIndex = -1;
        static constexpr size_t c_maxScancodesPerBind = 4;

        struct Bind;
        using BindCallback = std::function<void(const Bind& bind)>;
        struct Bind
        {
            BindIndex m_bindIndex = c_invalidBindIndex;
            SDL_Scancode m_scancodes[c_maxScancodesPerBind] = { SDL_SCANCODE_UNKNOWN };
            bool m_scancodesPressed[c_maxScancodesPerBind] = { false };
            size_t m_pressedNeeded = 0;
            BindCallback m_callback;
            bool m_state = false;
            bool m_prevState = false;
        };

    public: //static functions
        [[nodiscard]] static BindIndex RegisterBind(const Bind& bind);
        [[nodiscard]] static const Bind& GetBind(BindIndex bindIndex);

        static void Frame();
        static void UpdateMouseDelta(glm::vec2 mouseDelta);
        static void UpdateBinds(SDL_Scancode scancode, bool state);

        [[nodiscard]] static glm::vec2 GetMouseDelta();

    private: //member variables
        size_t m_bindCount = 0;
        std::vector<Bind> m_binds;

        glm::vec2 m_mouseDelta = { 0.0f, 0.0f };

    public: //constructors
        InputSystem();

    public: //member functions
        [[nodiscard]] BindIndex M_RegisterBind(const Bind& bind);
        [[nodiscard]] const Bind& M_GetBind(BindIndex bindIndex) const;

        void M_Frame();
        void M_UpdateMouseDelta(glm::vec2 mouseDelta);
        void M_UpdateBinds(SDL_Scancode scancode, bool state);

        [[nodiscard]] glm::vec2 M_GetMouseDelta() const { return m_mouseDelta; }
    };
}

