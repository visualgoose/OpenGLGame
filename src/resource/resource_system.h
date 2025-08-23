#pragma once

#include <unordered_map>
#include <vector>

#include "model.h"
#include "texture.h"
#include "material.h"
#include "shader.h"

namespace OGLGAME
{
    class ResourceSystem
    {
    public: //data types
        using ResourceIndex = size_t;
        enum ResourceType : uint8_t
        {
            ResourceType_invalid,
            ResourceType_model,
            ResourceType_texture,
            ResourceType_material,
            ResourceType_shader
        };
        struct ResourceID
        {
        public: //member variables
            ResourceIndex m_resourceIndex = -1;
            ResourceType m_resourceType = ResourceType_invalid;

        public: //member functions
            [[nodiscard]] bool IsValid() const noexcept
            {
                return m_resourceIndex != c_invalidResourceIndex &&
                    m_resourceType != ResourceType_invalid;
            }

        public: //operators
            operator ResourceIndex() const noexcept { return m_resourceIndex; }
        };

    public: //constants
        static constexpr ResourceIndex c_invalidResourceIndex = -1;

    private: //member variables
        std::unordered_map<std::filesystem::path, ResourceID> m_path2ResourceID;
        std::vector<Model> m_models;
        std::vector<Texture> m_textures;
        std::vector<Material> m_materials;
        std::vector<Shader> m_shaders;

    private: //constructors and setup functions
        //is private, because only the Client class should be able to initialize the resource system
        ResourceSystem();
        void LoadShadersAndMaterials();

    public: //operators
        ResourceSystem(ResourceSystem&) = delete;
        ResourceSystem& operator=(ResourceSystem&) = delete;

    public: //member functions
        void ModelRelease(ResourceIndex modelIndex) noexcept;
        void ModelAddRef(ResourceIndex modelIndex) noexcept;
        ResourceIndex ModelAddRef(const std::filesystem::path& modelPath) noexcept;

        void MaterialRelease(ResourceIndex modelIndex) noexcept;
        void MaterialAddRef(ResourceIndex modelIndex) noexcept;
        ResourceIndex MaterialAddRef(const std::filesystem::path& modelPath) noexcept;

        void TextureRelease(ResourceIndex modelIndex) noexcept {}
        void TextureAddRef(ResourceIndex modelIndex) noexcept {}
        ResourceIndex TextureAddRef(const std::filesystem::path& modelPath) noexcept {}

        ResourceID GetResourceID(const std::filesystem::path& filePath) const noexcept;

        const Model& GetModel(ResourceIndex modelIndex) const noexcept;
        const Texture& GetTexture(ResourceIndex textureIndex) const noexcept;
        const Material& GetMaterial(ResourceIndex materialIndex) const noexcept;
        const Shader& GetShader(ResourceIndex shaderIndex) const noexcept;


    friend class Client;
    };
}