#pragma once

#include <filesystem>

#include <glad/glad.h>

namespace OGLGAME
{
    class Texture
    {
    public: //data types and constants
        using ResourceIndex = size_t;
        static constexpr ResourceIndex c_invalidResourceIndex = -1;

    private: //member variables
        bool m_valid = false;
        std::filesystem::path m_path;
        ResourceIndex m_resourceIndex = c_invalidResourceIndex;
        size_t m_refCount = 0;
        GLuint m_texture = 0;

    public: //constructors
        Texture() = default;

    public: //member functions
        void Load(std::filesystem::path path, ResourceIndex resourceIndex);
        void AddRef();

        void Release();
        void CleanUp();

        [[nodiscard]] bool IsValid() const { return m_valid; }
        [[nodiscard]] const std::filesystem::path& GetPath() const { return m_path; }
        [[nodiscard]] ResourceIndex GetResourceIndex() const { return m_resourceIndex; }
        [[nodiscard]] size_t GetRefCount() const { return m_refCount; }
        [[nodiscard]] GLuint GetTexture() const { return m_texture; }
    };
}