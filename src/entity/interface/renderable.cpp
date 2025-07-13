#include "renderable.h"

#include "client.h"

namespace OGLGAME::Entities
{
    TypeCache::CachedType IRenderable::s_cachedInterface = TypeCache::sc_invalidType;
    void IRenderable::CacheInterface()
    {
        s_cachedInterface = Client::GetInstance()
            .GetTypeCache()
            .FindOrCreateType("irenderable");
    }
}