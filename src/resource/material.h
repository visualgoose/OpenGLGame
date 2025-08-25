#pragma once

#include <string>
#include <vector>

#include "shader.h"

namespace OGLGAME
{
    class Material
    {
    private: //data types and constants
        using ResourceIndex = size_t;
        static constexpr ResourceIndex c_invalidResourceIndex = -1;

        struct PropertyValue
        {
        public: //member variables
            Shader::PropertyType m_type;
            ResourceIndex m_resourceIndex = -1; //for values that are a resource
            union
            {
                std::string m_texturePath;
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

    private: //member variables
        bool m_valid = false;

        std::filesystem::path m_path;
        ResourceIndex m_resourceIndex = c_invalidResourceIndex;

        ResourceIndex m_shaderIndex = c_invalidResourceIndex;
        std::vector<PropertyValue> m_propertyValues;

    public: //constructors
        Material(std::filesystem::path filePath, ResourceIndex resourceIndex);

    public: //member functions
        void AddRef() const noexcept;
        void Release() const noexcept;

        [[nodiscard]] bool IsValid() const noexcept { return m_valid; }
        [[nodiscard]] ResourceIndex GetResourceIndex() const noexcept { return m_resourceIndex; }
        [[nodiscard]] const std::filesystem::path& GetPath() const noexcept { return m_path; }
        [[nodiscard]] ResourceIndex GetShaderIndex() const noexcept { return m_resourceIndex; }
        [[nodiscard]] const std::vector<PropertyValue>& GetProperties() const noexcept { return m_propertyValues; }
    };
}