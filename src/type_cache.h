#pragma once

#include <vector>
#include <string>

namespace OGLGAME
{
    class TypeCache
    {
    public:
        using CachedType = size_t;
    public:
        static constexpr CachedType sc_invalidType = -1;
    private:
        std::vector<std::string> m_types;
    public:
        TypeCache();
        TypeCache(const TypeCache&) = delete;
        TypeCache& operator=(const TypeCache&) = delete;
    public:
        CachedType FindType(const char* pTypeName) const noexcept;
        CachedType FindOrCreateType(const char* pTypeName);
    };
}