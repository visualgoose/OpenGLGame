#include "client.h"

#include "vgassert.h"

#include "entity/interface/renderable.h"
#include "entity/render_test.h"

namespace OGLGAME
{
    Client* Client::s_pInstance = nullptr;
    Client::Client()
    {
        vgassert(s_pInstance == nullptr && "There can't be a second instance of OGLGAME::Client");
        s_pInstance = this;

        CacheInterfaces();
        CacheTypes();

        m_resourceSystem.LoadShadersAndMaterials();

        m_pPlayer = m_scene.SpawnEntity(new Entities::Player());
    }

    void Client::CacheInterfaces()
    {
        Entities::IRenderable::S_CacheType();
    }
    void Client::CacheTypes()
    {
        Entities::Player::S_CacheType();
        Entities::RenderTest::S_CacheType();
    }
}