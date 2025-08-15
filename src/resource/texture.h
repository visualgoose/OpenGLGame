#pragma once

#include <filesystem>

#include <glad/glad.h>

namespace OGLGAME
{
    class Texture
    {
    public: //data types
        using ResourceIndex = size_t;

    public: //constants
        static constexpr ResourceIndex c_invalidResourceIndex = -1;

    private: //member variables
        bool m_valid = false;
        std::filesystem::path m_path;
        ResourceIndex m_resourceIndex = c_invalidResourceIndex;
        size_t m_refCount = 0;

        GLuint m_texture = 0;

    public: //constructors
        Texture() = default;
        explicit Texture(std::filesystem::path path, ResourceIndex resourceIndex);
    };
}