#pragma once

#include <glad/glad.h>

#include <unordered_map>
#include <vector>
#include <string>

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
            bool IsValid()
            {
                return m_resourceIndex != sc_invalidResourceIndex &&
                    m_resourceType != ResourceType_invalid;
            }

        public: //operators
            operator ResourceIndex() { return m_resourceIndex; }
        };

    public: //constants
        static constexpr ResourceIndex sc_invalidResourceIndex = -1;

    private: //member variabales
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
        ResourceID GetResourceID(const std::filesystem::path& filePath) const noexcept;

        const Shader& GetShader(ResourceIndex shaderIndex) const noexcept;

    friend class Client;
    };
}