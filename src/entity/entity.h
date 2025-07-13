#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>

#include "type_hierarchy.h"

namespace OGLGAME
{
    class IEntity
    {
    public:
        static constexpr size_t sc_invalidID = -1;
    private:
        size_t m_id = sc_invalidID;
    public:
        glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };
        glm::quat m_rotation = { 0.0f, 0.0f, 0.0f, 0.0f };
    public:
        virtual ~IEntity() noexcept {}
    public:
        size_t GetID() const noexcept { return m_id; }
        virtual const TypeHierarchy& GetTypeHierarchy() const noexcept = 0;
    private:
        virtual void Tick(double deltaTime) {}
        virtual void Frame(double deltaTime) {}
    friend class Scene;
    };
}