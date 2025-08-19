#pragma once

#include "type_cache.h"
#include <vector>

namespace OGLGAME
{
    class TypeHierarchy
    {
    private: //member variables
        std::vector<TypeCache::CachedType> m_interfaces;
        std::vector<TypeCache::CachedType> m_hierarchy;

    public: //constructors
        TypeHierarchy();

    public: //member functions
        [[nodiscard]] bool HasInterface(TypeCache::CachedType interfaceType) const noexcept;
        //check if "other" is the base type of this
        [[nodiscard]] bool Is(const TypeHierarchy& other) const noexcept;

        void AddInterface(TypeCache::CachedType interfaceType);
        void AddChild(const char* pChildName);
    };
}