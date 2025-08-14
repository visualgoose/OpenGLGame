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
    private: //static variables
        static Client* s_pInstance;

    public: //static functions
        static Client& S_GetInstance() { return *s_pInstance; }

    private: //member variables
        TypeCache m_typeCache;
        Scene m_scene;
        ResourceSystem m_resourceSystem;
        Entities::Player* m_pPlayer = nullptr;

    private: //constructors
        Client();
    public:
        Client(Client&) = delete;

    public: //operators
        Client& operator=(Client&) = delete;

    public: //member functions
        TypeCache& GetTypeCache() { return m_typeCache; }
        Scene& GetScene() { return m_scene; }
        ResourceSystem& GetResourceSystem() { return m_resourceSystem; }

        Entities::Player* GetPlayer() { return m_pPlayer; }
    private:
        void CacheInterfaces();
        void CacheTypes();
    friend int ::main(int argCount, char** ppArgs); //only the main function can initialize the Class singleton
    };
}