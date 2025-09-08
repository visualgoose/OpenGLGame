#include <type_hierarchy.h>

#include "vgassert.h"

#include "type_cache.h"

namespace OGLGAME
{
    TypeHierarchy::TypeHierarchy()
    {
        m_hierarchy.reserve(4);
        m_interfaces.reserve(2);
    }

    std::ptrdiff_t TypeHierarchy::GetInterfaceOffset(const TypeCache::TypeIndex interfaceType) const
    {
        vgassert(interfaceType != TypeCache::c_invalidType);

        for (const auto&[m_index, offset] : m_interfaces)
        {
            if (m_index == interfaceType)
                return offset;
        }
        return c_invalidInterfaceOffset;
    }

    bool TypeHierarchy::Is(const TypeHierarchy& other) const
    {
        vgassert(!m_hierarchy.empty() && "Tried calling TypeHierarchy::Is on empty TypeHierarchy");
        vgassert(!other.m_hierarchy.empty() && "Tried calling TypeHierarchy::Is on empty TypeHierarchy");

        if (m_hierarchy.size() < other.m_hierarchy.size())
            return false;
        const size_t bottomChildIndex = other.m_hierarchy.size() - 1;
        return m_hierarchy[bottomChildIndex] == other.m_hierarchy[bottomChildIndex];
    }

    void TypeHierarchy::AddInterface(const TypeCache::TypeIndex interfaceType, const std::ptrdiff_t offset)
    {
        vgassert(interfaceType != TypeCache::c_invalidType);
        m_interfaces.emplace_back(interfaceType, offset);
    }

    void TypeHierarchy::AddChild(const char* pChildName)
    {
        m_hierarchy.emplace_back(TypeCache::FindOrCreateType(pChildName));
    }

    bool TypeHierarchy::IsTop(const TypeHierarchy& other) const
    {
        vgassert(!m_hierarchy.empty() && "Tried calling TypeHierarchy::IsTop on empty TypeHierarchy");
        vgassert(!other.m_hierarchy.empty() && "Tried calling TypeHierarchy::IsTop on empty TypeHierarchy");

        return m_hierarchy[0] == other.m_hierarchy[0];
    }
}