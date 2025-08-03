#pragma once

int main(int argCount, char** ppArgs);

#include "type_cache.h"
#include "scene.h"
#include "resource/resource_system.h"
#include "entity/player.h"

namespace OGLGAME
{
    class Client
    {
    private:
        static Client* s_pInstance;
    public:
        static Client& S_GetInstance() { return *s_pInstance; }
    private:
        TypeCache m_typeCache;
        Scene m_scene;
        ResourceSystem m_resourceSystem;
        Entities::Player* m_pPlayer = nullptr;
    private:
        Client();
    public:
        Client(Client&) = delete;
        Client& operator=(Client&) = delete;
    public:
        TypeCache& GetTypeCache() { return m_typeCache; }
        Scene& GetScene() { return m_scene; }
        ResourceSystem& GetResourceSystem() { return m_resourceSystem; }

        Entities::Player* GetPlayer() { return m_pPlayer; }
    private:
        void CacheInterfaces();
        void CacheTypes();
    friend int ::main(int argCount, char** ppArgs); //only the main function can initialize the Class singelton
    };
}