#include "model.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "logging.h"

namespace OGLGAME
{
    Model::Model(std::filesystem::path path, ResourceIndex resourceIndex) :
        m_path(std::move(path)),
        m_resourceIndex(resourceIndex)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path.string(), aiProcess_Triangulate);
        if (!scene)
        {
            g_log.Error("Failed to open model \"{}\"", m_path.string());
            return;
        }

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        m_meshes.resize(scene->mNumMeshes);
        for (uint32_t i = 0; i < scene->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[i];

            scene->mMaterials[mesh->mMaterialIndex]->GetName().C_Str()

            uint32_t indexCount = 3 * mesh->mNumFaces;
            vertices.resize(mesh->mNumVertices);
            indices.resize(indexCount);

            for (uint32_t currentVertex = 0; currentVertex < mesh->mNumVertices; currentVertex++)
            {
                vertices[currentVertex] =
                {
                    mesh->mVertices[currentVertex].x, mesh->mVertices[currentVertex].y, mesh->mVertices[currentVertex].z,
                    mesh->mTextureCoords[0][currentVertex].x, mesh->mTextureCoords[0][currentVertex].y
                };
            }

            uint32_t currentIndexIndex = 0;
            for (uint32_t currentFace = 0; currentFace < mesh->mNumFaces; currentFace++)
            {
                for (uint32_t currentFaceIndex = 0; currentFaceIndex < 3; currentFaceIndex++)
                {
                    indices[currentIndexIndex] = mesh->mFaces[currentFace].mIndices[currentIndexIndex];
                    currentIndexIndex++;
                }
            }

            Mesh& outMesh = m_meshes[i];
            glGenVertexArrays(1, &outMesh.m_vao);
            glBindVertexArray(outMesh.m_vao);

            GLuint buffers[2];
            glGenBuffers(2, buffers);
            glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);

            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);


        }

        m_valid = true;
    }

    void Model::AddRef() noexcept
    {

    }

    void Model::Release() noexcept
    {

    }
}