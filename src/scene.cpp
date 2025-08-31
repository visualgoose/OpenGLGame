#include "scene.h"

#include <vgassert.h>

#include <ranges>

namespace OGLGAME
{
    Scene::Scene(const size_t baseGameObjectLimit) :
        m_gameObjectLimit(baseGameObjectLimit)
    {
        m_pGameObjects = static_cast<GameObjectHolder*>(malloc(baseGameObjectLimit * sizeof(GameObjectHolder)));
        for (size_t i = 0; i < baseGameObjectLimit; i++)
            m_pGameObjects[i].m_allocated = false;
    }
    
    Scene::~Scene()
    {
        size_t freed = 0;
        for (size_t i = 0; i < m_gameObjectLimit; i++)
        {
            if (freed == m_gameObjectCount)
                break;
            if (m_pGameObjects[i].m_allocated)
            {
                m_pGameObjects[i].m_gameObject.~GameObject();
                freed++;
            }
        }
        free(m_pGameObjects);
    }

    GameObject* Scene::AllocGameObject()
    {
        GameObject::GameObjectID foundID = GameObject::c_invalidGOID;
        if (m_gameObjectCount == m_gameObjectLimit)
        {
            const size_t newGameObjectLimit = 2 * m_gameObjectLimit;
            auto* pNewGameObjects = static_cast<GameObjectHolder*>(
                malloc(newGameObjectLimit * sizeof(GameObjectHolder)));
            for (size_t i = 0; i < m_gameObjectLimit; i++)
            {
                if (m_pGameObjects[i].m_allocated)
                {
                    pNewGameObjects[i].m_allocated = true;
                    pNewGameObjects[i].m_gameObject = std::move(m_pGameObjects[i].m_gameObject);
                }
            }
            for (size_t i = m_gameObjectLimit; i < newGameObjectLimit; i++)
            {
                pNewGameObjects[i].m_allocated = false;
            }

            foundID = m_gameObjectCount;

            m_gameObjectCount = newGameObjectLimit;
            free(m_pGameObjects);
            m_pGameObjects = pNewGameObjects;
        }
        else
        {
            for (GameObject::GameObjectID i = 0; i < m_gameObjectLimit; i++)
            {
                if (!m_pGameObjects[i].m_allocated)
                {
                    foundID = i;
                    break;
                }
            }
        }
        m_pGameObjects[foundID].m_allocated = true;
        auto* pGameObject = ::new(&m_pGameObjects[foundID].m_gameObject) GameObject();
        pGameObject->m_id = foundID;
        m_gameObjectCount++;
        return pGameObject;
    }

    void Scene::Tick(const double deltaTime)
    {
        size_t gameObjectsTicked = 0; //better name than "framed"
        for (size_t i = 0; i < m_gameObjectLimit; i++)
        {
            if (gameObjectsTicked == m_gameObjectCount)
                break;
            if (!m_pGameObjects[i].m_allocated)
                continue;
            GameObject* pGameObject = &m_pGameObjects[i].m_gameObject;
            pGameObject->Tick(deltaTime);
            gameObjectsTicked++;
        }
    }

    void Scene::Frame(const double deltaTime)
    {
        size_t gameObjectsFrameTicked = 0; //better name than "framed"
        for (size_t i = 0; i < m_gameObjectLimit; i++)
        {
            if (gameObjectsFrameTicked == m_gameObjectCount)
                break;
            if (!m_pGameObjects[i].m_allocated)
                continue;
            GameObject* pGameObject = &m_pGameObjects[i].m_gameObject;
            pGameObject->Frame(deltaTime);
            gameObjectsFrameTicked++;
        }
    }

    GameObject* Scene::GetGameObject(const GameObject::GameObjectID gameObjectID) const
    {
        vgassert(gameObjectID < m_gameObjectLimit);
        vgassert(m_pGameObjects[gameObjectID].m_allocated);

        return &m_pGameObjects[gameObjectID].m_gameObject;
    }
}