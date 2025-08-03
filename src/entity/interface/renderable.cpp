#include "renderable.h"

#include "client.h"

namespace OGLGAME::Entities
{
    TypeCache::CachedType IRenderable::s_type = TypeCache::sc_invalidType;

    void IRenderable::S_CacheType()
    {
        s_type = Client::S_GetInstance()
            .GetTypeCache()
            .FindOrCreateType("irenderable");
    }
    const TypeCache::CachedType IRenderable::S_GetType()
    {
        return s_type;
    }

    IRenderable::~IRenderable()
    {
        if (m_model != ResourceSystem::sc_invalidResourceIndex)
        {
            Client::S_GetInstance()
                .GetResourceSystem()
                .ReleaseModel(m_model);
        }
    }

    void IRenderable::SetModel(ResourceSystem::ResourceIndex model)
    {
        if (m_model == model)
            return;
        ResourceSystem& resourceSystem = Client::S_GetInstance().GetResourceSystem();
        if (m_model != ResourceSystem::sc_invalidResourceIndex)
        {
            resourceSystem.ReleaseModel(m_model);
        }
        resourceSystem.AddRefModel(model);
    }
    ResourceSystem::ResourceIndex IRenderable::SetModel(const std::string& modelPath)
    {
        ResourceSystem& resourceSystem = Client::S_GetInstance().GetResourceSystem();
        ResourceSystem::ResourceIndex model = resourceSystem.GetModel(modelPath);
        if (m_model != ResourceSystem::sc_invalidResourceIndex && m_model == model)
            return m_model;

        if (m_model != ResourceSystem::sc_invalidResourceIndex)
            resourceSystem.ReleaseModel(m_model);

        if (model != ResourceSystem::sc_invalidResourceIndex)
            m_model = resourceSystem.AddRefModel(model);
        else
            m_model = resourceSystem.AddRefModel(modelPath);
        return m_model;
    }
}