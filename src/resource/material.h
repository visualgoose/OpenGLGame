#pragma once

#include <string>
#include <vector>

#include "shader.h"

namespace OGLGAME
{
    class Material
    {
    private: //data types
        using ResourceIndex = size_t;
        struct PropertyValue
        {
        public: //member variables
            Shader::PropertyType m_type;
            union
            {
                std::string m_texturePath;
            };

        public: //constructors
            PropertyValue(std::string texturePath);
            PropertyValue(const PropertyValue& other);
            PropertyValue(PropertyValue&& other) noexcept;
            ~PropertyValue() noexcept;

        public: //operators
            PropertyValue& operator=(const PropertyValue& other);
            PropertyValue& operator=(PropertyValue&& other) noexcept;
        };

    private: //member variables
        bool m_valid = false;
        std::filesystem::path m_path;
        ResourceIndex m_shaderIndex;
        std::vector<PropertyValue> m_propertyValues;

    public: //constructors
        Material(const std::filesystem::path& filePath);

    public: //member functions
        bool IsValid() const noexcept { return m_valid; }
    };
}