#include <type_hierarchy.h>

#include "vgassert.h"

#include "client.h"

namespace OGLGAME
{
    TypeHierarchy::TypeHierarchy()
    {
        m_hierarchy.reserve(4);
        m_interfaces.reserve(2);
    }
    TypeHierarchy::TypeHierarchy(const TypeHierarchy& other)
    {
        m_hierarchy = other.m_hierarchy;
        m_interfaces = other.m_interfaces;
    }
    TypeHierarchy::TypeHierarchy(TypeHierarchy&& other) noexcept
    {
        m_hierarchy = std::move(other.m_hierarchy);
        m_interfaces = std::move(other.m_interfaces);
    }

    bool TypeHierarchy::HasInterface(const TypeCache::CachedType interfaceType) const noexcept
    {
        vgassert(interfaceType != TypeCache::c_invalidType);

        for (const auto& interface : m_interfaces)
        {
            if (interface == interfaceType)
                return true;
        }
        return false;
    }
    bool TypeHierarchy::Is(const TypeHierarchy& other) const noexcept
    {
        vgassert(!m_hierarchy.empty() && "Tried calling TypeHierarchy::Is on empty Entity");
        vgassert(!other.m_hierarchy.empty() && "Tried calling TypeHierarchy::Is on empty Entity");

        if (m_hierarchy.size() < other.m_hierarchy.size())
            return false;
        size_t bottomChildIndex = other.m_hierarchy.size() - 1;
        return m_hierarchy[bottomChildIndex] == other.m_hierarchy[bottomChildIndex];
    }
    void TypeHierarchy::AddInterface(const TypeCache::CachedType interfaceType)
    {
        vgassert(interfaceType != TypeCache::c_invalidType);
        m_interfaces.push_back(interfaceType);
    }
    void TypeHierarchy::AddChild(const char* pChildName)
    {
        m_hierarchy.push_back(Client::S_GetInstance()
            .GetTypeCache()
            .FindOrCreateType(pChildName));
    }
}