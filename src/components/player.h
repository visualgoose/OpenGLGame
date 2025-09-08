#pragma once

#include "ecs/game_object.h"

#include "glm/vec2.hpp"

#include "components/transform.h"

namespace OGLGAME::Components
{
    class Player final : public EntityComponent
    {
    private: //static variables
        static bool s_typeRegistered;
        static TypeHierarchy s_typeHierarchy;

    public: //static functions
        static void RegisterType();
        [[nodiscard]] static const TypeHierarchy& S_GetTypeHierarchy() { return s_typeHierarchy; }

    private: //member variables
        Transform* m_pTransform = nullptr;
        glm::vec2 m_movement = glm::vec2(0.0f);

    public: //member functions
        void Setup() override;

        void Frame(double deltaTime) override;

        [[nodiscard]] const TypeHierarchy& GetTypeHierarchy() const override { return s_typeHierarchy; }
    };
}