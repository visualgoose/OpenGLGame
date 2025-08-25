#include "resource_system.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "logging.h"

#include "vgassert.h"

#include "file_system.h"

namespace OGLGAME {
    ResourceSystem::ResourceSystem()
    {
        m_path2ResourceID.reserve(16);
        m_models.reserve(16);
        m_textures.reserve(16);
        m_materials.reserve(16);
        m_shaders.reserve(16);
    }
    void ResourceSystem::LoadShadersAndMaterials()
    {
        std::filesystem::path curPath = std::filesystem::current_path();
        for (const auto& dirEntry :
            std::filesystem::recursive_directory_iterator(curPath))
        {
            std::filesystem::path entrypath = dirEntry.path();
            std::string entrypathString = entrypath.string();
            if (!entrypathString.ends_with(".shader.json"))
                continue;

            FS::MakePathRelativeToGamePath(entrypath);

            size_t index = m_shaders.size();
            m_shaders.emplace_back(entrypath, index);
            if (!m_shaders[index].IsValid())
            {
                m_shaders.erase(m_shaders.begin() + index);
                continue;
            }
            m_path2ResourceID[entrypath] = ResourceID(index, ResourceType_shader);
        }
        for (const auto& dirEntry :
            std::filesystem::recursive_directory_iterator(std::filesystem::current_path()))
        {
            std::filesystem::path entrypath = dirEntry.path();
            std::string entrypathString = entrypath.string();
            if (!entrypathString.ends_with(".mat.json"))
                continue;

            FS::MakePathRelativeToGamePath(entrypath);

            size_t index = m_materials.size();
            m_materials.emplace_back(entrypath, index);
            if (!m_materials[index].IsValid())
            {
                m_materials.erase(m_materials.begin() + index);
                continue;
            }
            m_path2ResourceID[entrypath] = ResourceID(index, ResourceType_material);
        }
    }

    void ResourceSystem::ModelRelease(ResourceIndex modelIndex) noexcept
    {
        vgassert(modelIndex < m_models.size());

        m_models[modelIndex].Release();
        if (!m_models[modelIndex].IsValid())
            m_modelCount--;
    }

    void ResourceSystem::ModelAddRef(ResourceIndex modelIndex) noexcept
    {
        vgassert(modelIndex < m_models.size());

        m_models[modelIndex].AddRef();
    }

    ResourceSystem::ResourceIndex ResourceSystem::ModelAddRef(const std::filesystem::path& modelPath) noexcept
    {
        if(const auto modelIt = m_path2ResourceID.find(modelPath); modelIt != m_path2ResourceID.end())
        {
            vgassert(modelIt->second.m_resourceType == ResourceType_model);

            const ResourceIndex modelIndex = modelIt->second.m_resourceIndex;
            m_models[modelIndex].AddRef();
            return modelIndex;
        }

        ResourceIndex modelIndex;
        if (m_modelCount == m_models.size())
        {
            modelIndex = m_models.size();
            m_models.resize(m_models.size() + 1);
        }
        else
        {
            for (modelIndex = 0; modelIndex < m_models.size(); modelIndex++)
            {
                if (!m_models[modelIndex].IsValid())
                    break;
            }
        }
        m_models[modelIndex].Load(modelPath, modelIndex);
        if (!m_models[modelIndex].IsValid())
            return c_invalidResourceIndex;
        m_path2ResourceID[modelPath] = ResourceID(modelIndex, ResourceType_model);
        m_modelCount++;
        return modelIndex;
    }

    void ResourceSystem::MaterialRelease(const ResourceIndex materialIndex) noexcept
    {
        vgassert(materialIndex < m_materials.size());

        m_materials[materialIndex].Release();
    }

    void ResourceSystem::MaterialAddRef(const ResourceIndex materialIndex) noexcept
    {
        vgassert(materialIndex < m_materials.size());

        m_materials[materialIndex].AddRef();
    }

    ResourceSystem::ResourceIndex ResourceSystem::MaterialAddRef(const std::filesystem::path& materialPath) noexcept
    {
        const auto materialIt = m_path2ResourceID.find(materialPath);
        if (materialIt == m_path2ResourceID.end())
            return c_invalidResourceIndex;

        vgassert(materialIt->second.m_resourceType == ResourceType_material);

        ResourceIndex materialIndex = materialIt->second.m_resourceIndex;
        m_materials[materialIndex].AddRef();
        return materialIndex;
    }

    void ResourceSystem::TextureRelease(const ResourceIndex textureIndex) noexcept
    {
        vgassert(textureIndex < m_textures.size());

        m_textures[textureIndex].Release();
        if (!m_textures[textureIndex].IsValid())
            m_textureCount--;
    }

    void ResourceSystem::TextureAddRef(const ResourceIndex textureIndex) noexcept
    {
        vgassert(textureIndex < m_textures.size());

        m_textures[textureIndex].AddRef();
    }

    ResourceSystem::ResourceIndex ResourceSystem::TextureAddRef(const std::filesystem::path& texturePath) noexcept
    {
        if(const auto textureIt = m_path2ResourceID.find(texturePath); textureIt != m_path2ResourceID.end())
        {
            vgassert(textureIt->second.m_resourceType == ResourceType_texture);

            const ResourceIndex textureIndex = textureIt->second.m_resourceIndex;
            m_textures[textureIndex].AddRef();
            return textureIndex;
        }

        ResourceIndex textureIndex;
        if (m_textureCount == m_textures.size())
        {
            textureIndex = m_textures.size();
            m_textures.resize(m_textures.size() + 1);
        }
        else
        {
            for (textureIndex = 0; textureIndex < m_textures.size(); textureIndex++)
            {
                if (!m_textures[textureIndex].IsValid())
                    break;
            }
        }
        m_textures[textureIndex].Load(texturePath, textureIndex);
        if (!m_textures[textureIndex].IsValid())
            return c_invalidResourceIndex;
        m_path2ResourceID[texturePath] = ResourceID(textureIndex, ResourceType_texture);
        m_textureCount++;
        return textureIndex;
    }

    ResourceSystem::ResourceID ResourceSystem::GetResourceID(const std::filesystem::path& filePath) const noexcept
    {
        const auto& resourceIDIt = m_path2ResourceID.find(filePath);
        if (resourceIDIt == m_path2ResourceID.end())
            return {};
        return resourceIDIt->second;
    }

    const Model& ResourceSystem::GetModel(const ResourceIndex modelIndex) const noexcept
    {
        vgassert(m_models.size() > modelIndex);

        return m_models[modelIndex];
    }

    const Texture& ResourceSystem::GetTexture(const ResourceIndex textureIndex) const noexcept
    {
        vgassert(m_textures.size() > textureIndex);

        return m_textures[textureIndex];
    }

    const Material& ResourceSystem::GetMaterial(const ResourceIndex materialIndex) const noexcept
    {
        vgassert(m_materials.size() > materialIndex);

        return m_materials[materialIndex];
    }

    const Shader& ResourceSystem::GetShader(const ResourceIndex shaderIndex) const noexcept
    {
        vgassert(m_shaders.size() > shaderIndex);

        return m_shaders[shaderIndex];
    }
}