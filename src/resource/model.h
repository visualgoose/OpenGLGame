#pragma once

#include <filesystem>
#include <vector>

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <glad/glad.h>

namespace OGLGAME
{
    class Model
    {
    public: //data types and constants
        using ResourceIndex = size_t;
        static constexpr ResourceIndex c_invalidResourceIndex = -1;

        struct Vertex
        {
            glm::vec3 m_position;
            glm::vec2 m_texCoord;
            glm::vec3 m_normal;
            glm::ivec4 m_boneIDs;
            glm::vec4 m_weights;
        };
        struct Mesh
        {
            ResourceIndex m_materialIndex = -1;
            GLuint m_vao = 0;
            GLuint m_vbo = 0;
            GLuint m_ebo = 0;
            uint32_t m_indexCount = 0;
        };

    private: //member variables
        bool m_valid = false;
        std::filesystem::path m_path;
        ResourceIndex m_resourceIndex = c_invalidResourceIndex;
        size_t m_refCount = 0;
        std::vector<Mesh> m_meshes;

    public: //constructors
        Model() = default;

    public: //member functions
        void Load(std::filesystem::path path, ResourceIndex resourceIndex);
        void AddRef();

        void Release();
        void CleanUp();

        [[nodiscard]] bool IsValid() const { return m_valid; }
        [[nodiscard]] const std::filesystem::path& GetPath() const { return m_path; }
        [[nodiscard]] ResourceIndex GetResourceIndex() const { return m_resourceIndex; }
        [[nodiscard]] size_t GetRefCount() const { return m_refCount; }
        [[nodiscard]] const std::vector<Mesh>& GetMeshes() const { return m_meshes; }
    };
}
