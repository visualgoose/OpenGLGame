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
    public: //data types and constants
        using ResourceIndex = size_t;
        static constexpr ResourceIndex c_invalidResourceIndex = -1;
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
            ResourceIndex m_resourceIndex = c_invalidResourceIndex;
            ResourceType m_resourceType = ResourceType_invalid;

        public: //member functions
            [[nodiscard]] bool IsValid() const
            {
                return m_resourceIndex != c_invalidResourceIndex &&
                    m_resourceType != ResourceType_invalid;
            }
        };

    public: //static functions
        static void ModelRelease(ResourceIndex modelIndex);
        static void ModelAddRef(ResourceIndex modelIndex);
        static ResourceIndex ModelAddRef(const std::filesystem::path& modelPath);

        static void MaterialRelease(ResourceIndex materialIndex);
        static void MaterialAddRef(ResourceIndex materialIndex);
        static ResourceIndex MaterialAddRef(const std::filesystem::path& materialPath);

        static void TextureRelease(ResourceIndex textureIndex);
        static void TextureAddRef(ResourceIndex textureIndex);
        static ResourceIndex TextureAddRef(const std::filesystem::path& texturePath);

        static ResourceID GetResourceID(const std::filesystem::path& filePath);

        static const Model& GetModel(ResourceIndex modelIndex);
        static const Texture& GetTexture(ResourceIndex textureIndex);
        static const Material& GetMaterial(ResourceIndex materialIndex);
        static const Shader& GetShader(ResourceIndex shaderIndex);

    private: //member variables
        std::unordered_map<std::filesystem::path, ResourceID> m_path2ResourceID;
        size_t m_modelCount = 0;
        std::vector<Model> m_models;
        size_t m_textureCount = 0;
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
        void M_ModelRelease(ResourceIndex modelIndex);
        void M_ModelAddRef(ResourceIndex modelIndex);
        ResourceIndex M_ModelAddRef(const std::filesystem::path& modelPath);

        void M_MaterialRelease(ResourceIndex materialIndex);
        void M_MaterialAddRef(ResourceIndex materialIndex);
        ResourceIndex M_MaterialAddRef(const std::filesystem::path& materialPath);

        void M_TextureRelease(ResourceIndex textureIndex);
        void M_TextureAddRef(ResourceIndex textureIndex);
        ResourceIndex M_TextureAddRef(const std::filesystem::path& texturePath);

        ResourceID M_GetResourceID(const std::filesystem::path& filePath) const;

        const Model& M_GetModel(ResourceIndex modelIndex) const;
        const Texture& M_GetTexture(ResourceIndex textureIndex) const;
        const Material& M_GetMaterial(ResourceIndex materialIndex) const;
        const Shader& M_GetShader(ResourceIndex shaderIndex) const;

    friend class Client;
    };
}