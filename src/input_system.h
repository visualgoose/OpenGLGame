#pragma once

#include <functional>
#include <vector>
#include <SDL3/SDL_keycode.h>

class InputSystem
{
public: //data types and constants
    using BindIndex = size_t;
    static constexpr BindIndex c_invalidBindIndex = -1;
    static constexpr size_t c_maxScancodesPerBind = 4;

    using BindCallback = std::function<void(SDL_Scancode key[c_maxScancodesPerBind], bool state, bool prevState)>;
    struct Bind
    {
        BindIndex m_bindIndex = c_invalidBindIndex;
        SDL_Scancode m_scancode[c_maxScancodesPerBind] = { SDL_SCANCODE_UNKNOWN };
        BindCallback m_callback;
    };

private: //member variables
    std::vector<Bind> m_binds;

public: //constructors
    InputSystem();

public: //member functions
    [[nodiscard]] BindIndex RegisterBind(const Bind& bind);
    [[nodiscard]] const Bind& GetBind(BindIndex bindIndex);

};