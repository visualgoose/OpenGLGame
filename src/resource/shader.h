#pragma once

#include <vector>
#include <string>
#include <filesystem>

#include <glad/glad.h>

#include <nlohmann/json.hpp>

namespace OGLGAME
{
    struct VertexLayout
    {
    public:
        using VertexAttributes = uint16_t;
        using VertexAttributeBitMask = VertexAttributes;
        enum VertexAttributeBits : VertexAttributes
        {
            VertexAttributeBits_none,
            VertexAttributeBits_position,
            VertexAttributeBits_texCoord,
            VertexAttributeBits_EndOfEnum
        };
        static constexpr VertexAttributes VertexAttributeBits_Count = VertexAttributeBits_EndOfEnum - 1;
    public:
        static constexpr size_t sc_bitsPerAttribute = 4;
        static constexpr uint8_t sc_invalidVAIndex = -1;
    private:
        VertexAttributes m_vertexAttributes = 0;
    public:
        VertexLayout() = default;
        /*
        * \param pVertexAttributes must be terminated using VertexAttributeBits_none
        */
        VertexLayout(const VertexAttributeBits* pVertexAttributes) noexcept;
        VertexLayout(const VertexAttributeBits* pVertexAttributes, uint8_t vertexAttributeCount) noexcept;
        VertexLayout(VertexAttributes vertexAttributes) noexcept : m_vertexAttributes(vertexAttributes) {}
    private:
        void Init(const VertexAttributeBits* pVertexAttributes, uint8_t vertexAttributeCount) noexcept;
    public:
        operator VertexAttributes() const noexcept { return m_vertexAttributes; }
    public:
        VertexAttributes GetVertexAttributes() const noexcept { return m_vertexAttributes; }
    public:
        //returns bit mask of sc_bitsPerAttribute, so if sc_bitsPerAttribute is 4 then
        //return value is "1111 0000 0000 0000" in binary
        constexpr VertexAttributeBitMask GetVertexAttributeBitMask() const noexcept;
        uint8_t GetVertexAttributeIndex(VertexAttributeBits vertexAttribute) const noexcept;
    };

    class Shader
    {
    public:
        using ResourceIndex = size_t;
        enum PropertyType : uint8_t
        {
            PropertyType_invalid,
            PropertyType_tex2D,
            PropertyType_Count
        };
        struct Property
        {
            GLint m_uniformLocation = -1;
            PropertyType m_propertyType = PropertyType_invalid;
            std::string m_id;
            std::string m_displayName;
        };
        enum Feature : uint8_t
        {
            Feature_invalid,
            Feature_mvp,
            Feature_EndOfEnum
        };
        static constexpr uint8_t Feature_Count = Feature_EndOfEnum - 1;
    public:
        static constexpr ResourceIndex sc_invalidResourceIndex = -1;
    private:
        bool m_valid = false;
        std::filesystem::path m_path;
        ResourceIndex m_resourceIndex = sc_invalidResourceIndex;
        std::vector<Property> m_properties;
        Feature m_features[Feature_Count] = { Feature_invalid };
        VertexLayout m_vertexLayout;
    private:
        GLuint m_vertexShader = 0;
        GLuint m_fragmentShader = 0;
        GLuint m_shaderProgram = 0;
    public:
        Shader(const std::filesystem::path& filePath);
        Shader(Shader&) = delete;
        Shader& operator=(Shader&) = delete;
    private:
        //also loads vertex layout
        bool CreateShaders(const nlohmann::json& shaderJSON);
        bool CreateProgram(const nlohmann::json& shaderJSON);
        bool LoadProperties(const nlohmann::json& shaderJSON);
        bool LoadFeatures(const nlohmann::json& shaderJSON);
    public:
        ~Shader() noexcept;
    public:
        bool IsValid() const noexcept { return m_valid; }
    };
}