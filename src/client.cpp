#include "client.h"

#include "vgassert.h"

#include "entity/interface/renderable.h"

namespace OGLGAME
{
    Client* Client::s_pInstance = nullptr;
    Client::Client()
    {
        vgassert(s_pInstance == nullptr && "There can't be a second instance of OGLGAME::Client");
        s_pInstance = this;

        CacheTypes();

        m_pPlayer = m_scene.SpawnEntity(new Entities::Player());
    }

    Client& Client::GetInstance()
    {
        return *s_pInstance;
    }

    TypeCache& Client::GetTypeCache()
    {
        return m_typeCache;
    }
    Scene& Client::GetScene()
    {
        return m_scene;
    }

    void Client::CacheInterfaces()
    {
        Entities::IRenderable::CacheInterface();
    }
    void Client::CacheTypes()
    {
        Entities::Player::CacheType();
    }
}