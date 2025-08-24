#include "model.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "client.h"
#include "file_system.h"
#include "logging.h"
#include "resource_system.h"

namespace OGLGAME
{
    void Model::Load(std::filesystem::path path, ResourceIndex resourceIndex)
    {
        vgassert(!m_valid);

        ResourceSystem& resourceSystem = Client::S_GetInstance().GetResourceSystem();

        m_path = std::move(path);
        m_resourceIndex = resourceIndex;

        Assimp::Importer importer;
        const aiScene* pScene = importer.ReadFile(path.string(), aiProcess_Triangulate);
        if (!pScene)
        {
            g_log.Error("Failed to open model \"{}\"", m_path.string());
            return;
        }

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        m_meshes.resize(pScene->mNumMeshes);
        std::filesystem::path materialPath;
        for (uint32_t i = 0; i < pScene->mNumMeshes; i++)
        {
            aiMesh* pMesh = pScene->mMeshes[i];

            materialPath = m_path / pScene->mMaterials[pMesh->mMaterialIndex]->GetName().C_Str();

            ResourceIndex materialIndex = resourceSystem.MaterialAddRef(materialPath);
            if (materialIndex == c_invalidResourceIndex)
            {
                g_log.Warning(R"(Failed to find material index for material "{}" for model "{}")", materialPath.string(), m_path.string());
            }

            uint32_t indexCount = 3 * pMesh->mNumFaces;
            vertices.resize(pMesh->mNumVertices);
            indices.resize(indexCount);

            for (uint32_t currentVertex = 0; currentVertex < pMesh->mNumVertices; currentVertex++)
            {
                vertices[currentVertex] =
                {
                    pMesh->mVertices[currentVertex].x, pMesh->mVertices[currentVertex].y, pMesh->mVertices[currentVertex].z,
                    pMesh->mTextureCoords[0][currentVertex].x, pMesh->mTextureCoords[0][currentVertex].y
                };
            }

            uint32_t currentIndexIndex = 0;
            for (uint32_t currentFace = 0; currentFace < pMesh->mNumFaces; currentFace++)
            {
                for (uint32_t currentFaceIndex = 0; currentFaceIndex < 3; currentFaceIndex++)
                {
                    indices[currentIndexIndex] = pMesh->mFaces[currentFace].mIndices[currentIndexIndex];
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

            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

            const ResourceIndex shaderIndex = resourceSystem.GetMaterial(materialIndex).GetShaderIndex();
            const Shader& shader = resourceSystem.GetShader(shaderIndex);

            Shader::VertexAttribute pVertexAttributes[Shader::VertexAttribute_Count] = { Shader::VertexAttribute_none };
            shader.GetVertexAttributes(pVertexAttributes, Shader::VertexAttribute_Count);
            GLsizei stride = shader.GetVertexStride();
            uint8_t* vertexOffset = 0;
            for (GLuint vertexAttrIdx = 0; vertexAttrIdx < Shader::VertexAttribute_Count; vertexAttrIdx++)
            {
                if (pVertexAttributes[vertexAttrIdx] == Shader::VertexAttribute_none)
                    break;
                switch (pVertexAttributes[vertexAttrIdx])
                {
                    case Shader::VertexAttribute_position:
                        glVertexAttribPointer(vertexAttrIdx, 3, GL_FLOAT, GL_FALSE, stride, vertexOffset);
                        vertexOffset += 3 * sizeof(float);
                        break;
                    case Shader::VertexAttribute_texCoord:
                        glVertexAttribPointer(vertexAttrIdx, 2, GL_FLOAT, GL_FALSE, stride, vertexOffset);
                        vertexOffset += 2 * sizeof(float);
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

    void Model::AddRef() noexcept
    {
        vgassert(m_valid);

        m_refCount++;
    }

    void Model::Release() noexcept
    {
        vgassert(m_valid);

        m_refCount--;
        if (m_refCount == 0)
            CleanUp();
    }

    void Model::CleanUp() noexcept
    {
        vgassert(!m_valid);

        m_valid = false;
        ResourceSystem& resourceSystem = Client::S_GetInstance().GetResourceSystem();
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
                resourceSystem.MaterialRelease(mesh.m_materialIndex);
        }
    }

}