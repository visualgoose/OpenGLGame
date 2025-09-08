#include "resource_system.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "client.h"
#include "logging.h"

#include "vgassert.h"

#include "file_system.h"

namespace OGLGAME
{
    void ResourceSystem::ModelRelease(const ResourceIndex modelIndex)
    {
        Client::GetInstance().GetResourceSystem().M_ModelRelease(modelIndex);
    }

    void ResourceSystem::ModelAddRef(const ResourceIndex modelIndex)
    {
        Client::GetInstance().GetResourceSystem().M_ModelAddRef(modelIndex);
    }

    ResourceSystem::ResourceIndex ResourceSystem::ModelAddRef(const std::filesystem::path& modelPath)
    {
        return Client::GetInstance().GetResourceSystem().M_ModelAddRef(modelPath);
    }

    void ResourceSystem::MaterialRelease(const ResourceIndex materialIndex)
    {
        Client::GetInstance().GetResourceSystem().M_MaterialRelease(materialIndex);
    }

    void ResourceSystem::MaterialAddRef(const ResourceIndex materialIndex)
    {
        Client::GetInstance().GetResourceSystem().M_MaterialAddRef(materialIndex);
    }

    ResourceSystem::ResourceIndex ResourceSystem::MaterialAddRef(const std::filesystem::path& materialPath)
    {
        return Client::GetInstance().GetResourceSystem().M_MaterialAddRef(materialPath);
    }

    void ResourceSystem::TextureRelease(const ResourceIndex textureIndex)
    {
        Client::GetInstance().GetResourceSystem().M_TextureRelease(textureIndex);
    }

    void ResourceSystem::TextureAddRef(const ResourceIndex textureIndex)
    {
        Client::GetInstance().GetResourceSystem().M_TextureAddRef(textureIndex);
    }

    ResourceSystem::ResourceIndex ResourceSystem::TextureAddRef(const std::filesystem::path& texturePath)
    {
        return Client::GetInstance().GetResourceSystem().M_TextureAddRef(texturePath);
    }

    ResourceSystem::ResourceID ResourceSystem::GetResourceID(const std::filesystem::path& filePath)
    {
        return Client::GetInstance().GetResourceSystem().M_GetResourceID(filePath);
    }

    const Model& ResourceSystem::GetModel(const ResourceIndex modelIndex)
    {
        return Client::GetInstance().GetResourceSystem().M_GetModel(modelIndex);
    }

    const Texture& ResourceSystem::GetTexture(const ResourceIndex textureIndex)
    {
        return Client::GetInstance().GetResourceSystem().M_GetTexture(textureIndex);
    }

    const Material& ResourceSystem::GetMaterial(const ResourceIndex materialIndex)
    {
        return Client::GetInstance().GetResourceSystem().M_GetMaterial(materialIndex);
    }

    const Shader& ResourceSystem::GetShader(const ResourceIndex shaderIndex)
    {
        return Client::GetInstance().GetResourceSystem().M_GetShader(shaderIndex);
    }

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

    void ResourceSystem::M_ModelRelease(ResourceIndex modelIndex)
    {
        vgassert(modelIndex < m_models.size());

        m_models[modelIndex].Release();
        if (!m_models[modelIndex].IsValid())
            m_modelCount--;
    }

    void ResourceSystem::M_ModelAddRef(ResourceIndex modelIndex)
    {
        vgassert(modelIndex < m_models.size());

        m_models[modelIndex].AddRef();
    }

    ResourceSystem::ResourceIndex ResourceSystem::M_ModelAddRef(const std::filesystem::path& modelPath)
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

    void ResourceSystem::M_MaterialRelease(const ResourceIndex materialIndex)
    {
        vgassert(materialIndex < m_materials.size());

        m_materials[materialIndex].Release();
    }

    void ResourceSystem::M_MaterialAddRef(const ResourceIndex materialIndex)
    {
        vgassert(materialIndex < m_materials.size());

        m_materials[materialIndex].AddRef();
    }

    ResourceSystem::ResourceIndex ResourceSystem::M_MaterialAddRef(const std::filesystem::path& materialPath)
    {
        const auto materialIt = m_path2ResourceID.find(materialPath);
        if (materialIt == m_path2ResourceID.end())
            return c_invalidResourceIndex;

        vgassert(materialIt->second.m_resourceType == ResourceType_material);

        ResourceIndex materialIndex = materialIt->second.m_resourceIndex;
        m_materials[materialIndex].AddRef();
        return materialIndex;
    }

    void ResourceSystem::M_TextureRelease(const ResourceIndex textureIndex)
    {
        vgassert(textureIndex < m_textures.size());

        m_textures[textureIndex].Release();
        if (!m_textures[textureIndex].IsValid())
            m_textureCount--;
    }

    void ResourceSystem::M_TextureAddRef(const ResourceIndex textureIndex)
    {
        vgassert(textureIndex < m_textures.size());

        m_textures[textureIndex].AddRef();
    }

    ResourceSystem::ResourceIndex ResourceSystem::M_TextureAddRef(const std::filesystem::path& texturePath)
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

    ResourceSystem::ResourceID ResourceSystem::M_GetResourceID(const std::filesystem::path& filePath) const
    {
        const auto& resourceIDIt = m_path2ResourceID.find(filePath);
        if (resourceIDIt == m_path2ResourceID.end())
            return {};
        return resourceIDIt->second;
    }

    const Model& ResourceSystem::M_GetModel(const ResourceIndex modelIndex) const
    {
        vgassert(m_models.size() > modelIndex);

        return m_models[modelIndex];
    }

    const Texture& ResourceSystem::M_GetTexture(const ResourceIndex textureIndex) const
    {
        vgassert(m_textures.size() > textureIndex);

        return m_textures[textureIndex];
    }

    const Material& ResourceSystem::M_GetMaterial(const ResourceIndex materialIndex) const
    {
        vgassert(m_materials.size() > materialIndex);

        return m_materials[materialIndex];
    }

    const Shader& ResourceSystem::M_GetShader(const ResourceIndex shaderIndex) const
    {
        vgassert(m_shaders.size() > shaderIndex);

        return m_shaders[shaderIndex];
    }
}