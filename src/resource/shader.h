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
    public: //data types and related constants
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

    public: //constants
        static constexpr size_t c_bitsPerAttribute = 4;
        static constexpr uint8_t c_invalidVAIndex = -1;

    private: //member variables
        VertexAttributes m_vertexAttributes = 0;

    public: //constructors, setup functions
        VertexLayout() = default;
        /*
        * \param pVertexAttributes must be terminated using VertexAttributeBits_none
        */
        explicit VertexLayout(const VertexAttributeBits* pVertexAttributes) noexcept;
        VertexLayout(const VertexAttributeBits* pVertexAttributes, uint8_t vertexAttributeCount) noexcept;
        explicit VertexLayout(const VertexAttributes vertexAttributes) noexcept : m_vertexAttributes(vertexAttributes) {}
    private:
        void Init(const VertexAttributeBits* pVertexAttributes, uint8_t vertexAttributeCount) noexcept;

    public: //operators
        explicit operator VertexAttributes() const noexcept { return m_vertexAttributes; }

    public: //member functions
        [[nodiscard]] VertexAttributes GetVertexAttributes() const noexcept { return m_vertexAttributes; }

        //returns bit mask of sc_bitsPerAttribute, so if sc_bitsPerAttribute is 4 then
        //return value is "1111 0000 0000 0000" in binary
        [[nodiscard]] constexpr VertexAttributeBitMask GetVertexAttributeBitMask() const noexcept;
        [[nodiscard]] size_t GetVertexAttributeIndex(VertexAttributeBits vertexAttribute) const noexcept;
    };

    class Shader
    {
    public: //data types and constants related to data types
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

    public: //constants
        static constexpr ResourceIndex c_invalidResourceIndex = -1;
        static constexpr const char* c_featureUniformNames[Feature_EndOfEnum] =
        {
            "invalid",
            "u_oglgame_mvp"
        };

    private: //member variables
        bool m_valid = false;

        std::filesystem::path m_path;
        ResourceIndex m_resourceIndex = c_invalidResourceIndex;

        std::vector<Property> m_properties;
        Feature m_features[Feature_Count] = { Feature_invalid };
        GLint m_featureUniformLocations[Feature_Count] = { 0 };
        VertexLayout m_vertexLayout;

        //opengl handles
        GLuint m_vertexShader = 0;
        GLuint m_fragmentShader = 0;
        GLuint m_shaderProgram = 0;

    public: //constructors and setup functions
        Shader() = default;
        Shader(std::filesystem::path filePath, ResourceIndex resourceIndex);
        ~Shader() noexcept;

    private:
        //also loads vertex layout
        bool CreateShaders(const nlohmann::json& shaderJSON);
        bool CreateProgram();
        bool LoadProperties(const nlohmann::json& shaderJSON);
        bool LoadFeatures(const nlohmann::json& shaderJSON);

    public: //member functions
        [[nodiscard]] bool IsValid() const noexcept { return m_valid; }

        [[nodiscard]] const std::filesystem::path& GetPath() const noexcept { return m_path; }
        [[nodiscard]] ResourceIndex GetIndex() const noexcept { return m_resourceIndex; }

        [[nodiscard]] const std::vector<Property>& GetProperties() const noexcept { return m_properties; }
        void GetFeatures(Feature* pOutFeatures, uint8_t pOutFeaturesMaxSize) const noexcept;
        void GetFeatureUniformLocations(GLint* pOutLocations, uint8_t outLocationsSize) const noexcept;
        [[nodiscard]] const VertexLayout& GetVertexLayout() const noexcept { return m_vertexLayout; }

        [[nodiscard]] GLuint GetVertexShader() const noexcept { return m_vertexShader; }
        [[nodiscard]] GLuint GetFragmentShader() const noexcept { return m_fragmentShader; }
        [[nodiscard]] GLuint GetShaderProgram() const noexcept { return m_shaderProgram; }
    };
}