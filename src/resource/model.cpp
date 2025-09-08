#include "model.h"

#include <cstddef>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "client.h"
#include "file_system.h"
#include "logging.h"
#include "resource_system.h"

namespace OGLGAME
{
    void Model::Load(std::filesystem::path path, const ResourceIndex resourceIndex)
    {
        vgassert(!m_valid);

        m_path = std::move(path);
        m_resourceIndex = resourceIndex;

        Assimp::Importer importer;
        const aiScene* pScene = importer.ReadFile(m_path.string(), aiProcess_Triangulate);
        if (!pScene)
        {
            g_log.Error("Failed to open model \"{}\"", m_path.string());
            return;
        }

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        m_meshes.resize(pScene->mNumMeshes);
        std::filesystem::path materialPath;
        std::filesystem::path materialAbsPath;
        for (uint32_t i = 0; i < pScene->mNumMeshes; i++)
        {
            aiMesh* pMesh = pScene->mMeshes[i];

            const char* pMaterialPathAssimp = pScene->mMaterials[pMesh->mMaterialIndex]->GetName().C_Str();
            materialAbsPath = m_path.parent_path() / pMaterialPathAssimp;

            ResourceIndex materialIndex = ResourceSystem::MaterialAddRef(materialAbsPath);
            if (materialIndex == c_invalidResourceIndex)
            {
                materialPath = pMaterialPathAssimp;
                materialIndex = ResourceSystem::MaterialAddRef(materialPath);
            }
            if (materialIndex == c_invalidResourceIndex)
            {
                g_log.Warning("Failed to find material \"{}\" or \"{}\" for model \"{}\"", materialAbsPath.string(),
                    materialPath.string(), m_path.string());
            }

            uint32_t indexCount = 3 * pMesh->mNumFaces;
            vertices.resize(pMesh->mNumVertices);
            indices.resize(indexCount);

            for (uint32_t currentVertex = 0; currentVertex < pMesh->mNumVertices; currentVertex++)
            {
                vertices[currentVertex] =
                {
                    { pMesh->mVertices[currentVertex].x, pMesh->mVertices[currentVertex].y, pMesh->mVertices[currentVertex].z },
                    { pMesh->mTextureCoords[0][currentVertex].x, pMesh->mTextureCoords[0][currentVertex].y },
                    { pMesh->mNormals[currentVertex].x, pMesh->mNormals[currentVertex].y, pMesh->mNormals[currentVertex].z },
                    { -1, -1, -1, -1 },
                    { 0.0f, 0.0f, 0.0f, 0.0f }
                };
            }

            uint32_t currentIndexIndex = 0;
            for (uint32_t currentFace = 0; currentFace < pMesh->mNumFaces; currentFace++)
            {
                for (uint32_t currentFaceIndex = 0; currentFaceIndex < 3; currentFaceIndex++)
                {
                    indices[currentIndexIndex] = pMesh->mFaces[currentFace].mIndices[currentFaceIndex];
                    currentIndexIndex++;
                }
            }

            Mesh& outMesh = m_meshes[i];
            glGenVertexArrays(1, &outMesh.m_vao);
            glBindVertexArray(outMesh.m_vao);

            GLuint pBuffers[2];
            glGenBuffers(2, pBuffers);
            glBindBuffer(GL_ARRAY_BUFFER, pBuffers[0]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pBuffers[1]);

            outMesh.m_vbo = pBuffers[0];
            outMesh.m_ebo = pBuffers[1];
            outMesh.m_materialIndex = materialIndex;
            outMesh.m_indexCount = indexCount;

            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

            const ResourceIndex shaderIndex = ResourceSystem::GetMaterial(materialIndex).GetShaderIndex();
            const Shader& shader = ResourceSystem::GetShader(shaderIndex);

            Shader::VertexAttribute pVertexAttributes[Shader::VertexAttribute_Count] = { Shader::VertexAttribute_none };
            shader.GetVertexAttributes(pVertexAttributes, Shader::VertexAttribute_Count);
            for (GLuint vertexAttrIdx = 0; vertexAttrIdx < Shader::VertexAttribute_Count; vertexAttrIdx++)
            {
                if (pVertexAttributes[vertexAttrIdx] == Shader::VertexAttribute_none)
                    break;
                switch (pVertexAttributes[vertexAttrIdx])
                {
                    case Shader::VertexAttribute_position:
                        glVertexAttribPointer(vertexAttrIdx, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            reinterpret_cast<void*>(offsetof(Vertex, m_position)));
                        break;
                    case Shader::VertexAttribute_texCoord:
                        glVertexAttribPointer(vertexAttrIdx, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            reinterpret_cast<void*>(offsetof(Vertex, m_texCoord)));
                        break;
                    case Shader::VertexAttribute_normal:
                        glVertexAttribPointer(vertexAttrIdx, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            reinterpret_cast<void*>(offsetof(Vertex, m_normal)));
                        break;
                    case Shader::VertexAttribute_boneIDs:
                        glVertexAttribPointer(vertexAttrIdx, 4, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex),
                            reinterpret_cast<void*>(offsetof(Vertex, m_boneIDs)));
                        break;
                    case Shader::VertexAttribute_weight:
                        glVertexAttribPointer(vertexAttrIdx, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            reinterpret_cast<void*>(offsetof(Vertex, m_weights)));
                        break;
                    default:
                        break;
                }
                glEnableVertexAttribArray(vertexAttrIdx);
            }
            glBindVertexArray(0);
        }
        m_valid = true;
        m_refCount = 1;
    }

    void Model::AddRef()
    {
        vgassert(m_valid);

        m_refCount++;
    }

    void Model::Release()
    {
        vgassert(m_valid);

        m_refCount--;
        if (m_refCount == 0)
            CleanUp();
    }

    void Model::CleanUp()
    {
        vgassert(m_valid);

        m_valid = false;
        for (const auto& mesh : m_meshes)
        {
            const GLuint pBuffers[2] =
            {
                mesh.m_vbo,
                mesh.m_ebo
            };
            glDeleteBuffers(2, pBuffers);
            glDeleteVertexArrays(1, &mesh.m_vao);
            if (mesh.m_materialIndex != c_invalidResourceIndex)
                ResourceSystem::MaterialRelease(mesh.m_materialIndex);
        }
    }

}