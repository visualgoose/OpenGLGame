#pragma once

#include <vector>
#include <string>
#include <filesystem>

#include <glad/glad.h>

#include <nlohmann/json.hpp>

namespace OGLGAME
{
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

        enum VertexAttribute
        {
            VertexAttribute_none,
            VertexAttribute_position,
            VertexAttribute_texCoord,
            VertexAttribute_EndOfEnum
        };
        static constexpr uint8_t VertexAttribute_Count = VertexAttribute_EndOfEnum - 1;

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
        Feature m_pFeatures[Feature_Count] = { Feature_invalid };
        GLint m_pFeatureUniformLocations[Feature_Count] = { 0 };
        //is terminated by "VertexAttribute_none"
        VertexAttribute m_pVertexAttributes[VertexAttribute_Count] = { VertexAttribute_none };
        GLsizei m_vertexStride = 0;

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
        void GetVertexAttributes(VertexAttribute* pOutAttributes, uint8_t outAttributesSize) const noexcept;
        [[nodiscard]] GLsizei GetVertexStride() const noexcept { return m_vertexStride; }

        [[nodiscard]] GLuint GetVertexShader() const noexcept { return m_vertexShader; }
        [[nodiscard]] GLuint GetFragmentShader() const noexcept { return m_fragmentShader; }
        [[nodiscard]] GLuint GetShaderProgram() const noexcept { return m_shaderProgram; }
    };
}