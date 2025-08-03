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
    public:
        using ResourceIndex = size_t;
        enum ResourceType
        {
            ResourceType_invalid,
            ResourceType_model,
            ResourceType_texture,
            ResourceType_material,
            ResourceType_shader
        };
        struct ResourceID
        {
            ResourceIndex m_resourceIndex = -1;
            ResourceType m_resourceType = ResourceType_invalid;

            operator ResourceIndex() { return m_resourceIndex; }
        };
    public:
        static constexpr ResourceIndex sc_invalidResourceIndex = -1;
    private:
        std::unordered_map<std::string, ResourceID> m_string2ResourceID;
        std::vector<Model> m_models;
        std::vector<Texture> m_textures;
        std::vector<Material> m_materials;
        std::vector<Shader> m_shaders;
    public:
        ResourceSystem();
    private:
        void LoadShadersAndMaterials();
    public:
        ResourceSystem(ResourceSystem&) = delete;
        ResourceSystem& operator=(ResourceSystem&) = delete;
    public:
        bool AddRefModel(const ResourceIndex model) noexcept;
        const ResourceIndex AddRefModel(const std::string& modelPath);
        void ReleaseModel(const ResourceIndex model) noexcept;
        void ReleaseModel(const std::string& modelPath) noexcept;
        const ResourceIndex GetModel(const std::string& modelPath) const noexcept;
        const Model& GetModelInfo(const ResourceIndex model) const noexcept;
    private:
        Model& GetModelInfoNC(const ResourceIndex model) noexcept;
        friend class Client;
    };
}