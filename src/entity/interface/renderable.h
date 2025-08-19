#pragma once

#include "type_cache.h"
#include "resource/resource_system.h"

namespace OGLGAME::Entities
{
    class Renderable
    {
    private: //static variables
        static TypeCache::CachedType s_type;

    public: //static functions
        static void S_CacheType();
        static TypeCache::CachedType S_GetType();

    public: //constructors
        Renderable() = default;
        ~Renderable();

    private: //member variables
        ResourceSystem::ResourceIndex m_model = ResourceSystem::c_invalidResourceIndex;

    protected: //member functions
        void SetModel(ResourceSystem::ResourceIndex model);
        ResourceSystem::ResourceIndex SetModel(const std::string& pModelPath);
    };
}