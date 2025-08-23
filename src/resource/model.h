#pragma once

#include <filesystem>
#include <vector>

#include <glad/glad.h>

namespace OGLGAME
{
    class Model
    {
    public: //data types
        struct Vertex
        {
            float x, y, z;
            float u, v;
        };
        using ResourceIndex = size_t;
        struct Mesh
        {
            ResourceIndex m_materialIndex = -1;
            GLuint m_vao = 0;
            GLuint m_vbo = 0;
            GLuint m_ebo = 0;
            uint32_t m_indexCount = 0;
        };

    public: //constants
        static constexpr ResourceIndex c_invalidResourceIndex = -1;

    private: //member variables
        bool m_valid = false;
        std::filesystem::path m_path;
        ResourceIndex m_resourceIndex = c_invalidResourceIndex;
        size_t m_refCount = 1;
        std::vector<Mesh> m_meshes;

    public: //constructors
        Model() = default;

    public: //member functions
        void Load(std::filesystem::path path, ResourceIndex resourceIndex);
        void AddRef() noexcept;

        void Release() noexcept;
        void CleanUp() noexcept;

        [[nodiscard]] bool IsValid() const noexcept { return m_valid; }
        [[nodiscard]] const std::filesystem::path& GetPath() const noexcept { return m_path; }
        [[nodiscard]] ResourceIndex GetResourceIndex() const noexcept { return m_resourceIndex; }
        [[nodiscard]] size_t GetRefCount() const noexcept { return m_refCount; }
        [[nodiscard]] std::vector<Mesh> GetMeshes() const noexcept { return m_meshes; }
    };
}
