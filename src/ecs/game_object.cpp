#include "game_object.h"

#include "client.h"
#include "logging.h"

namespace OGLGAME
{
    GameObject::GameObject()
    {
        m_components.reserve(4);
        m_children.reserve(4);
    }

    GameObject::~GameObject()
    {
        for (const auto component : m_components)
        {
            delete component;
        }
    }

    GameObject& GameObject::operator=(GameObject&& other)
    {
        m_components = std::move(other.m_components);
        m_children = std::move(other.m_children);
        m_id = other.m_id;
        m_parentID = other.m_parentID;
        for (const auto& component : m_components)
        {
            component->m_pGameObject = this;
        }
        return *this;
    }

    void GameObject::Tick(const double deltaTime)
    {
        for (const auto component : m_components)
        {
            component->Tick(deltaTime);
        }
    }

    void GameObject::Frame(const double deltaTime)
    {
        for (const auto component : m_components)
        {
            component->Frame(deltaTime);
        }
    }

    void GameObject::SetParent(GameObject* pParent)
    {
        if (m_parentID == pParent->m_id)
            return;
        if (m_parentID != c_invalidGOID)
        {
            const Scene& scene = Client::S_GetInstance().GetScene();
            GameObject* pOldParent = scene.GetGameObject(m_parentID);
            for (auto it = pOldParent->m_children.begin(); it != pOldParent->m_children.end(); ++it)
            {
                if (*it == m_id)
                {
                    pOldParent->m_children.erase(it);
                    break;
                }
            }
        }
        m_parentID = pParent->m_id;
        pParent->m_children.push_back(m_id);
    }

    void GameObject::SetParent(const GameObjectID parentID)
    {
        const Scene& scene = Client::S_GetInstance().GetScene();
        SetParent(scene.GetGameObject(parentID));
    }
}