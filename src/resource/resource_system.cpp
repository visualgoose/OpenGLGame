#include "resource_system.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "logging.h"

#include "vgassert.h"

namespace OGLGAME
{
    ResourceSystem::ResourceSystem()
    {
        m_string2ResourceID.reserve(16);
        m_models.reserve(16);
        m_textures.reserve(16);
        m_materials.reserve(16);
        m_shaders.reserve(16);
    }
    void ResourceSystem::LoadShadersAndMaterials()
    {
        for (const auto& dirEntry
            : std::filesystem::recursive_directory_iterator(std::filesystem::current_path()))
        {
            std::filesystem::path entrypath = dirEntry.path();
            if (entrypath.extension() != ".shader.json")
                continue;
            size_t index = m_shaders.size();
            m_shaders.emplace_back(entrypath);
            if (!m_shaders[index].IsValid())
                m_shaders.erase(m_shaders.begin() + index);
            m_string2ResourceID[entrypath.string()] = ResourceID(index, ResourceType_shader);
        }
        for (const auto& dirEntry
            : std::filesystem::recursive_directory_iterator(std::filesystem::current_path()))
        {
            std::filesystem::path entrypath = dirEntry.path();
            if (entrypath.extension() != ".mat.json")
                continue;
            size_t index = m_materials.size();
            m_materials.emplace_back(entrypath);
            if (!m_materials[index].IsValid())
                m_materials.erase(m_materials.begin() + index);
            m_string2ResourceID[entrypath.string()] = ResourceID(index, ResourceType_material);
        }
    }

    bool ResourceSystem::AddRefModel(const ResourceSystem::ResourceIndex model) noexcept
    {
        //ModelResource& model = GetModelInfoNC(model);
        //model.m_refCount++;
        return true;
    }
    const ResourceSystem::ResourceIndex ResourceSystem::AddRefModel(const std::string& modelPath)
    {
        ResourceIndex ResourceIndex = m_string2ResourceID[modelPath];
        if (ResourceIndex != sc_invalidResourceIndex)
        {
            AddRefModel(ResourceIndex);
            return ResourceIndex;
        }

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate);
        if (!scene)
        {
            g_log.Error("Failed to read file: {}", modelPath);
            return sc_invalidResourceIndex;
        }

        aiMesh* mesh = scene->mMeshes[0];
        uint32_t numFaces = mesh->mNumFaces;
        std::vector<uint32_t> indicies;
        indicies.reserve(3 * numFaces);
        size_t currentIndexIndex = 0; //wow, amazing name isn't it?
        for (uint32_t i = 0; i < numFaces; i++)
        {
            for (uint32_t j = 0; j < 3; j++)
            {
                indicies[currentIndexIndex] = mesh->mFaces[i].mIndices[j];
                currentIndexIndex++;
            }
        }
        /*
        ModelResource modelResource;
        modelResource.m_refCount = 1;
        modelResource.m_path = modelPath;
        glGenVertexArrays(1, &modelResource.m_VAO);
        //because of the structure of ModelResource allocating 2 buffers for m_VBO will also allocate a buffer for m_EBO
        //maybe bad practice, but convenient and idc
        glGenBuffers(2, &modelResource.m_VBO);

        glBindVertexArray(modelResource.m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, modelResource.m_VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelResource.m_EBO);

        glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * sizeof(aiVector3D), mesh->mVertices, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(uint32_t), indicies.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(aiVector3D), reinterpret_cast<const void*>(0));

        glBindVertexArray(0);

        importer.FreeScene();

        ResourceIndex cachedResourceIndex = sc_invalidResourceIndex;
        if (m_models.size() == m_string2ResourceID.size())
        {
            m_models.resize(2 * m_string2ResourceID.size());
            cachedResourceIndex = m_string2ResourceID.size();
        }
        else
        {
            for (size_t i = 0; i < m_models.size(); i++)
            {
                if (m_models[i].m_refCount == 0)
                {
                    cachedResourceIndex = i;
                    break;
                }
            }
        }
        ModelResource& modelResourceRef = m_models[cachedResourceIndex] = std::move(modelResource);
        m_string2ResourceID[modelResourceRef.m_path] = cachedResourceIndex;
        */
        return sc_invalidResourceIndex;
    }
    void ResourceSystem::ReleaseModel(const ResourceIndex model) noexcept
    {
        /*
        Model& model = GetModelInfoNC(model);
        if (model.m_refCount == 1)
        {
            //because of the structure of ModelResource deleting 2 m_VBO buffers will also delete m_EBO
            //maybe bad practice, but convenient and idc
            glDeleteBuffers(2, &model.m_VBO);
            model.m_VBO = 0;
            model.m_EBO = 0;
            glDeleteVertexArrays(1, &model.m_VAO);
            model.m_VAO = 0;
        }
        model.m_refCount--;
        */
    }
    void ResourceSystem::ReleaseModel(const std::string& modelPath) noexcept
    {
        ResourceIndex model = GetModel(modelPath);
        if (model != sc_invalidResourceIndex)
        {
            ReleaseModel(model);
        }
    }
    const ResourceSystem::ResourceIndex ResourceSystem::GetModel(const std::string& modelPath) const noexcept
    {
        const auto& modelIterator = m_string2ResourceID.find(modelPath);
        if (modelIterator != m_string2ResourceID.end())
        {
            return modelIterator->second.m_resourceIndex;
        }
        return sc_invalidResourceIndex;
    }
    const Model& ResourceSystem::GetModelInfo(
        const ResourceSystem::ResourceIndex model) const noexcept
    {
        vgassert(model != sc_invalidResourceIndex);
        vgassert(model >= m_models.size());
        //vgassert(m_models[model].m_refCount != 0);

        return m_models[model];
    }
    Model& ResourceSystem::GetModelInfoNC(
        const ResourceSystem::ResourceIndex model) noexcept
    {
        
        vgassert(model != sc_invalidResourceIndex);
        vgassert(model >= m_models.size());
        //vgassert(m_models[model].m_refCount != 0);

        return m_models[model];
    }
}