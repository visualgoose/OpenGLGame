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

    private: //member variables
        std::vector<std::string> m_types;
    public:
        TypeCache();
        TypeCache(const TypeCache&) = delete;
        TypeCache& operator=(const TypeCache&) = delete;

        TypeIndex FindType(const char* pTypeName) const;
        TypeIndex FindOrCreateType(const char* pTypeName);
    };
}