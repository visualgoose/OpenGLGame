#pragma once

#include <filesystem>

#include "ecs/game_object.h"

#include "resource/resource_system.h"

namespace OGLGAME::Components
{
    class AudioPlayer : public EntityComponent
    {
    private: //static variables
        static bool s_typeRegistered;
        static TypeHierarchy s_typeHierarchy;

    public: //static functions
        static void RegisterType();
        [[nodiscard]] static const TypeHierarchy& S_GetTypeHierarchy() { return s_typeHierarchy; }

    public: //constructors
        AudioPlayer() = default;
        ~AudioPlayer() override;

    private: //member variables
        ResourceSystem::ResourceIndex m_audioIndex = ResourceSystem::c_invalidResourceIndex;

    public: //member functions
        [[nodiscard]] const TypeHierarchy& GetTypeHierarchy() const override { return s_typeHierarchy; }

        void SetAudio(ResourceSystem::ResourceIndex audioIndex);
        void SetAudio(const std::filesystem::path& audioPath);
        [[nodiscard]] ResourceSystem::ResourceIndex GetAudio() const { return m_audioIndex; }
    };
}
