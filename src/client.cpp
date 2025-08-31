#include "client.h"

#include "vgassert.h"

#include "components/model_filter.h"
#include "components/player.h"
#include "components/transform.h"

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

        m_pPlayerObject = m_scene.AllocGameObject();
        m_pPlayerObject->AddComponent<Components::Player>();
    }

    void Client::CacheInterfaces()
    {

    }

    void Client::CacheTypes()
    {
        Components::Transform::RegisterType();
        Components::ModelFilter::RegisterType();
        Components::Player::RegisterType();
    }
}