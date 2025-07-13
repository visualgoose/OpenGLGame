#pragma once

#include "type_cache.h"

namespace OGLGAME::Entities
{
    class IRenderable
    {
    private:
        static TypeCache::CachedType s_cachedInterface;
    public:
        static void CacheInterface();
    public:
        IRenderable() = default;
        ~IRenderable();
    private:
        size_t model;
    protected:
        size_t SetModel();
    };
}