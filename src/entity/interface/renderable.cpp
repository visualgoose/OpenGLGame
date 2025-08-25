#include "renderable.h"

#include "client.h"
#include "logging.h"

namespace OGLGAME::Entities
{
    TypeCache::CachedType Renderable::s_type = TypeCache::c_invalidType;

    void Renderable::S_CacheType()
    {
        if (s_type != TypeCache::c_invalidType)
            return;
        s_type = Client::S_GetInstance()
            .GetTypeCache()
            .FindOrCreateType("irenderable");
    }

    TypeCache::CachedType Renderable::S_GetType()
    {
        return s_type;
    }

    Renderable::~Renderable()
    {
        if (m_model != ResourceSystem::c_invalidResourceIndex)
        {
            Client::S_GetInstance()
                .GetResourceSystem()
                .ModelRelease(m_model);
        }
    }

    void Renderable::SetModel(const ResourceSystem::ResourceIndex model)
    {
        if (m_model == model)
            return;
        ResourceSystem& resourceSystem = Client::S_GetInstance().GetResourceSystem();
        if (m_model != ResourceSystem::c_invalidResourceIndex)
        {
            resourceSystem.ModelRelease(m_model);
        }
        resourceSystem.ModelAddRef(model);
        m_model = model;
    }

    ResourceSystem::ResourceIndex Renderable::SetModel(const std::string& modelPath)
    {
        ResourceSystem& resourceSystem = Client::S_GetInstance().GetResourceSystem();
        const ResourceSystem::ResourceIndex modelIndex = resourceSystem.GetResourceID(modelPath).m_resourceIndex;
        if (m_model != ResourceSystem::c_invalidResourceIndex && m_model == modelIndex)
            return m_model;

        if (m_model != ResourceSystem::c_invalidResourceIndex)
            resourceSystem.ModelRelease(m_model);

        if (modelIndex != ResourceSystem::c_invalidResourceIndex)
        {
            m_model = modelIndex;
            resourceSystem.ModelAddRef(m_model);
        }
        else
            m_model = resourceSystem.ModelAddRef(modelPath);
        return m_model;
    }
}