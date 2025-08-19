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

            if (!FS::MakePathRelativeToGamePath(entrypath))
            {
                g_log.Error("Shader file {} wasn't loaded, because the file wasn't inside the game path",
                    entrypathString);
                continue;
            }

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

            if (!FS::MakePathRelativeToGamePath(entrypath))
            {
                g_log.Error("Material file {} wasn't loaded, because the file wasn't inside the game path",
                    entrypathString);
                continue;
            }

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

    ResourceSystem::ResourceID ResourceSystem::GetResourceID(const std::filesystem::path& filePath) const noexcept
    {
        const auto& resourceIDIt = m_path2ResourceID.find(filePath);
        if (resourceIDIt == m_path2ResourceID.end())
            return {};
        return resourceIDIt->second;
    }

    const Model& ResourceSystem::GetModel(ResourceIndex modelIndex) const noexcept
    {
        vgassert(m_models.size() > modelIndex);

        return m_models[modelIndex];
    }

    const Texture& ResourceSystem::GetTexture(ResourceIndex textureIndex) const noexcept
    {
        vgassert(m_textures.size() > textureIndex);

        return m_textures[textureIndex];
    }

    const Material& ResourceSystem::GetMaterial(ResourceIndex materialIndex) const noexcept
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