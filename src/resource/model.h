#pragma once

#include <filesystem>

#include <tuple>

namespace OGLGAME
{
    class Model
    {
    public: //data types
        using ResourceIndex = size_t;
        struct Mesh
        {
        };

    public: //constants
        static constexpr ResourceIndex c_invalidResourceIndex = -1;

    private: //member variables
        bool m_valid = false;
        std::filesystem::path m_path;
        ResourceIndex m_resourceIndex = c_invalidResourceIndex;
        size_t m_refCount = 0;
    };
}