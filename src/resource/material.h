#pragma once

#include <string>
#include <vector>

#include <variant>

#include "shader.h"

namespace OGLGAME
{
    class Material
    {
    private: //data types and constants
        using ResourceIndex = size_t;
        static constexpr ResourceIndex c_invalidResourceIndex = -1;

    public:
        struct PropertyValue
        {
        public: //member variables
            Shader::PropertyType m_type = Shader::PropertyType_invalid;
            ResourceIndex m_resourceIndex = -1; //for values that are a resource
            std::variant<std::string> m_value;

        public: //constructors
            PropertyValue() = default;
            explicit PropertyValue(std::string texturePath);
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
        void AddRef();
        void Release();

        [[nodiscard]] bool IsValid() const { return m_valid; }
        [[nodiscard]] ResourceIndex GetResourceIndex() const { return m_resourceIndex; }
        [[nodiscard]] const std::filesystem::path& GetPath() const { return m_path; }
        [[nodiscard]] ResourceIndex GetShaderIndex() const { return m_resourceIndex; }
        [[nodiscard]] const std::vector<PropertyValue>& GetProperties() const { return m_propertyValues; }
    };
}