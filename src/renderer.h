#pragma once
#include <SDL3/SDL_video.h>

namespace OGLGAME
{
    class Renderer
    {
    private:
        SDL_Window* m_pWindow = nullptr;

    public: //static functions
        static void Render();

    public: //constructors
        explicit Renderer(SDL_Window* pWindow);

    public: //member functions
        void M_Render() const;
    };
}