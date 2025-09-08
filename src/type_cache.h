#pragma once

#include <vector>
#include <string>

namespace OGLGAME
{
    class TypeCache
    {
    public: //data types and constants
        using TypeIndex = size_t;
        static constexpr TypeIndex c_invalidType = -1;

    public: //static functions
        static TypeIndex FindType(const char* pTypeName);
        static TypeIndex FindOrCreateType(const char* pTypeName);

    private: //member variables
        std::vector<std::string> m_types;
    public:
        TypeCache();
        TypeCache(const TypeCache&) = delete;
        TypeCache& operator=(const TypeCache&) = delete;

        TypeIndex M_FindType(const char* pTypeName) const;
        TypeIndex M_FindOrCreateType(const char* pTypeName);
    };
}