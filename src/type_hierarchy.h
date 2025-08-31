#pragma once

#include "type_cache.h"
#include <vector>

//get offset of an interface in a class
#define INTERFACE_OFFSET(Class, Interface) reinterpret_cast<char*>(static_cast<Interface*>(reinterpret_cast<Class*>(1))) - reinterpret_cast<char*>(1)
//have to use "1", because otherwise static_cast<Interface*>() will only return the correct value on a non-zero value ^^^

namespace OGLGAME
{
    class TypeHierarchy
    {
    public: //data types and constants
        static constexpr std::ptrdiff_t c_invalidInterfaceOffset = -1;
        struct Interface
        {
            TypeCache::TypeIndex m_index = TypeCache::c_invalidType;
            std::ptrdiff_t offset = c_invalidInterfaceOffset;
        };

    private: //member variables
        std::vector<Interface> m_interfaces;
        std::vector<TypeCache::TypeIndex> m_hierarchy;

    public: //constructors
        TypeHierarchy();

    public: //member functions
        [[nodiscard]] std::ptrdiff_t GetInterfaceOffset(TypeCache::TypeIndex interfaceType) const;
        //check if "other" is the base type of this
        [[nodiscard]] bool Is(const TypeHierarchy& other) const;

        //use INTERFACE_OFFSET macro for offset
        void AddInterface(TypeCache::TypeIndex interfaceType, std::ptrdiff_t offset);
        void AddChild(const char* pChildName);

        [[nodiscard]] bool IsTop(const TypeHierarchy& other) const;
    };
}