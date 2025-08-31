#pragma once

#include <filesystem>

#include "ecs/game_object.h"

#include "resource/resource_system.h"

namespace OGLGAME::Components
{
    class ModelFilter : public EntityComponent
    {
    private: //static variables
        static bool s_typeRegistered;
        static TypeHierarchy s_typeHierarchy;

    public: //static functions
        static void RegisterType();
        [[nodiscard]] static const TypeHierarchy& S_GetTypeHierarchy() { return s_typeHierarchy; }

    public: //constructors
        ModelFilter() = default;
        ~ModelFilter() override;

    private: //member variables
        ResourceSystem::ResourceIndex m_modelIndex = ResourceSystem::c_invalidResourceIndex;

    public: //member functions
        [[nodiscard]] const TypeHierarchy& GetTypeHierarchy() const override { return s_typeHierarchy; }

        void SetModel(ResourceSystem::ResourceIndex modelIndex);
        void SetModel(const std::filesystem::path& modelPath);
        [[nodiscard]] ResourceSystem::ResourceIndex GetModel() const { return m_modelIndex; }
    };
}
