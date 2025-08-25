#pragma once

#include <vector>
#include <string>

namespace OGLGAME
{
    class TypeCache
    {
    public: //data types and constants
        using CachedType = size_t;
        static constexpr CachedType c_invalidType = -1;

    private: //member variables
        std::vector<std::string> m_types;
    public:
        TypeCache();
        TypeCache(const TypeCache&) = delete;
        TypeCache& operator=(const TypeCache&) = delete;

        CachedType FindType(const char* pTypeName) const noexcept;
        CachedType FindOrCreateType(const char* pTypeName);
    };
}