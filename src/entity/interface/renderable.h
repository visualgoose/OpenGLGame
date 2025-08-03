#pragma once

#include "type_cache.h"
#include "resource/resource_system.h"
#include "entity/entity.h"

namespace OGLGAME::Entities
{
    class IRenderable
    {
    private:
        static TypeCache::CachedType s_type;
    private:
        static void S_CacheType();
    public:
        static const TypeCache::CachedType S_GetType();
    public:
        IRenderable() = default;
        ~IRenderable();
    private:
        ResourceSystem::ResourceIndex m_model = ResourceSystem::sc_invalidResourceIndex;
    protected:
        void SetModel(ResourceSystem::ResourceIndex model);
        ResourceSystem::ResourceIndex SetModel(const std::string& pModelPath);
        friend class Client;
    };
}