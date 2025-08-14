#include "scene.h"

#include <vgassert.h>

namespace OGLGAME
{
    Scene::Scene(size_t baseEntityLimit) :
        m_entityLimit(baseEntityLimit)
    {
        m_ppEntities = new Entity*[baseEntityLimit];
        for (size_t i = 0; i < baseEntityLimit; i++)
            m_ppEntities[i] = nullptr;
    }
    Scene::~Scene()
    {
        for (size_t i = 0; i < m_entityLimit; i++)
        {
            delete m_ppEntities[i];
        }
        delete[] m_ppEntities;
    }

    Entity* Scene::SpawnEntityInternal(Entity* pEntity)
    {
        size_t id = sc_invalidEntityID;
        if (m_entityCount == m_entityLimit)
        {
            size_t oldEntityLimit = m_entityLimit;
            m_entityLimit *= 2;
            Entity** ppNewEntities = new Entity*[m_entityLimit];
            for (size_t i = 0; i < oldEntityLimit; i++)
                ppNewEntities[i] = m_ppEntities[i];
            delete[] m_ppEntities;
            m_ppEntities = ppNewEntities;
            id = oldEntityLimit;
        }
        if (id == sc_invalidEntityID)
        {
            for (size_t i = 0; i < m_entityLimit; i++)
            {
                if (m_ppEntities[i] != nullptr)
                    continue;
                id = i;
                break;
            }
        }
        m_ppEntities[id] = pEntity;
        pEntity->m_id = id;
        m_entityCount++;
        return pEntity;
    }
    void Scene::RemoveEntity(Entity* pEntity)
    {
        RemoveEntity(pEntity->m_id);
    }
    void Scene::RemoveEntity(size_t id)
    {
        vgassert(id < m_entityLimit);
        Entity* entity = m_ppEntities[id];
        vgassert(entity != nullptr);
        delete entity;
        m_ppEntities[id] = nullptr;
        m_entityCount--;
    }

    void Scene::Tick(double deltaTime)
    {
        size_t entitiesTicked = 0;
        for (size_t i = 0; i < m_entityLimit; i++)
        {
            if (entitiesTicked == m_entityCount)
                break;
            Entity* entity = m_ppEntities[i];
            if (entity != nullptr)
            {
                entity->Tick(deltaTime);
                entitiesTicked++;
            }
        }
    }
    void Scene::Frame(double deltaTime)
    {
        size_t entitiesFrameTicked = 0;
        for (size_t i = 0; i < m_entityLimit; i++)
        {
            if (entitiesFrameTicked == m_entityCount)
                break;
            Entity* entity = m_ppEntities[i];
            if (entity != nullptr)
            {
                entity->Frame(deltaTime);
                entitiesFrameTicked++;
            }
        }
    }
}