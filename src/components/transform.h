#pragma once

#include "ecs/game_object.h"

#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>

namespace OGLGAME::Components
{
    class Transform : public EntityComponent
    {
    private: //static variables
        static bool s_typeRegistered;
        static TypeHierarchy s_typeHierarchy;

    public: //static functions
        static void RegisterType();
        [[nodiscard]] static const TypeHierarchy& S_GetTypeHierarchy() { return s_typeHierarchy; }

    public:
        glm::vec3 m_position = glm::vec3(0.0f);
        glm::vec3 m_scale = glm::vec3(1.0f);
        glm::quat m_rotation = glm::quat();

    public: //constructors
        Transform() = default;

    public: //member functions
        [[nodiscard]] const TypeHierarchy& GetTypeHierarchy() const override { return s_typeHierarchy; }
    };
}