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
                struct
                {
                    std::string m_texturePath;
                    ResourceIndex resource_index = 0;
                };
            };

        public: //constructors
            explicit PropertyValue(std::string texturePath);
            PropertyValue(const PropertyValue& other);
            PropertyValue(PropertyValue&& other) noexcept;
            ~PropertyValue() noexcept;

        public: //operators
            PropertyValue& operator=(const PropertyValue& other);
            PropertyValue& operator=(PropertyValue&& other) noexcept;
        };

    public: //constants
        static constexpr ResourceIndex c_invalidResourceIndex = -1;

    private: //member variables
        bool m_valid = false;

        std::filesystem::path m_path;
        ResourceIndex m_resourceIndex;

        ResourceIndex m_shaderIndex = c_invalidResourceIndex;
        std::vector<PropertyValue> m_propertyValues;

    public: //constructors
        Material(std::filesystem::path filePath, ResourceIndex resourceIndex);

    public: //member functions
        [[nodiscard]] bool IsValid() const noexcept { return m_valid; }
    };
}