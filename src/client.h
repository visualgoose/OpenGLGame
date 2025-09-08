#pragma once

int main(int argCount, char** ppArgs);

#include "type_cache.h"
#include "scene.h"
#include "resource/resource_system.h"
#include "input_system.h"
#include "renderer.h"

namespace OGLGAME
{
    class Client
    {
    private: //static variables
        static Client* s_pInstance;

    public: //static functions
        static Client& GetInstance() { return *s_pInstance; }

        static GameObject* GetPlayerObject() { return GetInstance().m_pPlayerObject; }

    private: //member variables
        TypeCache m_typeCache;
        ResourceSystem m_resourceSystem;
        Scene m_scene;
        InputSystem m_inputSystem;
        Renderer m_renderer;

        GameObject* m_pPlayerObject = nullptr;

    private: //constructors
        explicit Client(SDL_Window* pWindow);
    public:
        Client(Client&) = delete;

    public: //operators
        Client& operator=(Client&) = delete;

    public: //member functions
        TypeCache& GetTypeCache() { return m_typeCache; }
        Scene& GetScene() { return m_scene; }
        ResourceSystem& GetResourceSystem() { return m_resourceSystem; }
        InputSystem& GetInputSystem() { return m_inputSystem; }
        Renderer& GetRenderer() { return m_renderer; }

    private:
        void CacheInterfaces();
        void CacheTypes();
    friend int ::main(int argCount, char** ppArgs); //only the main function can initialize the Class singleton
    };
}