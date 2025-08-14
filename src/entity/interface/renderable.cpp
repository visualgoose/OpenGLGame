#include "renderable.h"

#include "client.h"

namespace OGLGAME::Entities
{
    TypeCache::CachedType Renderable::s_type = TypeCache::sc_invalidType;

    void Renderable::S_CacheType()
    {
        s_type = Client::S_GetInstance()
            .GetTypeCache()
            .FindOrCreateType("irenderable");
    }
    const TypeCache::CachedType Renderable::S_GetType()
    {
        return s_type;
    }

    Renderable::~Renderable()
    {
        if (m_model != ResourceSystem::sc_invalidResourceIndex)
        {
            //Client::S_GetInstance()
            //    .GetResourceSystem()
            //    .ReleaseModel(m_model);
        }
    }

    void Renderable::SetModel(ResourceSystem::ResourceIndex model)
    {
        if (m_model == model)
            return;
        ResourceSystem& resourceSystem = Client::S_GetInstance().GetResourceSystem();
        if (m_model != ResourceSystem::sc_invalidResourceIndex)
        {
            //resourceSystem.ReleaseModel(m_model);
        }
        //resourceSystem.AddRefModel(model);
    }
    ResourceSystem::ResourceIndex Renderable::SetModel(const std::string& modelPath)
    {
        ResourceSystem& resourceSystem = Client::S_GetInstance().GetResourceSystem();
        //ResourceSystem::ResourceIndex model = resourceSystem.GetModel(modelPath);
        //if (m_model != ResourceSystem::sc_invalidResourceIndex && m_model == model)
        //    return m_model;

        if (m_model != ResourceSystem::sc_invalidResourceIndex)
            //resourceSystem.ReleaseModel(m_model);

            //if (model != ResourceSystem::sc_invalidResourceIndex)
                //m_model = resourceSystem.AddRefModel(model);
                ;
        else
            //m_model = resourceSystem.AddRefModel(modelPath);
        return m_model;
    }
}