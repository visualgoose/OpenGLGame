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
        TypeHierarchy(const TypeHierarchy& other);
        TypeHierarchy(TypeHierarchy&& other) noexcept;

    public: //member functions
        bool HasInterface(const TypeCache::CachedType interfaceType) const noexcept;
        //check if "other" is the base type of this
        bool Is(const TypeHierarchy& other) const noexcept;

        void AddInterface(const TypeCache::CachedType interfaceType);
        void AddChild(const char* pChildName);
    };
}