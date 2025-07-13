#pragma once

int main(int argCount, char** ppArgs);

#include "type_cache.h"
#include "scene.h"
#include "entity/player.h"

namespace OGLGAME
{
    class Client
    {
    private:
        static Client* s_pInstance;
    private:
        TypeCache m_typeCache;
        Scene m_scene;
        Entities::Player* m_pPlayer = nullptr;
    private:
        Client();
    public:
        Client(Client&) = delete;
        Client& operator=(Client&) = delete;
    public:
        static Client& GetInstance();
    public:
        TypeCache& GetTypeCache();
        Scene& GetScene();
    private:
        void CacheInterfaces();
        void CacheTypes();
    friend int ::main(int argCount, char** ppArgs); //only the main function can initialize the Class singelton
    };
}