#include "type_cache.h"

namespace OGLGAME
{
    TypeCache::TypeCache()
    {
        m_types.reserve(16);
    }

    TypeCache::TypeIndex TypeCache::FindType(const char* pTypeName) const
    {
        for (TypeIndex i = 0; i < m_types.size(); i++)
        {
            if (m_types[i] == pTypeName)
                return i;
        }
        return c_invalidType;
    }
    TypeCache::TypeIndex TypeCache::FindOrCreateType(const char* pTypeName)
    {
        if (const TypeIndex type = FindType(pTypeName); type != c_invalidType)
            return type;
        m_types.emplace_back(pTypeName);
        return m_types.size() - 1;
    }
}