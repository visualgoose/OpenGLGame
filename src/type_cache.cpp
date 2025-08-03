#include "type_cache.h"

namespace OGLGAME
{
    TypeCache::TypeCache()
    {
        m_types.reserve(16);
    }

    TypeCache::CachedType TypeCache::FindType(const char* pTypeName) const noexcept
    {
        for (CachedType i = 0; i < m_types.size(); i++)
        {
            if (m_types[i] == pTypeName)
                return i;
        }
        return sc_invalidType;
    }
    TypeCache::CachedType TypeCache::FindOrCreateType(const char* pTypeName)
    {
        CachedType type = FindType(pTypeName);
        if (type != sc_invalidType)
            return type;
        m_types.emplace_back(pTypeName);
        return m_types.size() - 1;
    }
}