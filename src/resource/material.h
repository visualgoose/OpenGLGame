#pragma once

#include <string>
#include <vector>

#include "shader.h"

namespace OGLGAME
{
    class Material
    {
    private:
        struct PropertyValue
        {
            Shader::PropertyType m_type;
            union
            {
                std::string m_texturePath;
            };
        };
    private:
        bool m_valid = false;
        std::filesystem::path m_path;
        std::vector<PropertyValue> m_propertyValues;
    public:
        Material(const std::filesystem::path& filePath);
        Material(Material&) = delete;
        Material& operator=(Material&) = delete;
    public:
        bool IsValid() const noexcept { return m_valid; }
    };
}