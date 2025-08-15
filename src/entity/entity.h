#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>

#include "type_hierarchy.h"

namespace OGLGAME
{
    class Entity
    {
    public: //data types
        using EntityID = size_t;

    public: //constants
        static constexpr size_t c_invalidID = -1;

    private: //member variables
        EntityID m_id = c_invalidID;
    public:
        glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };
        glm::quat m_rotation = { 0.0f, 0.0f, 0.0f, 0.0f };

    public: //constructors
        Entity() = default;
        Entity(Entity& other) = delete;
        virtual ~Entity() noexcept = default;

    public: //operators
        Entity& operator=(Entity&) = delete;

    public: //member functions
        [[nodiscard]] EntityID GetID() const noexcept { return m_id; }
        [[nodiscard]] virtual const TypeHierarchy& GetTypeHierarchy() const noexcept = 0;
    private:
        virtual void Tick(double deltaTime) {}
        virtual void Frame(double deltaTime) {}
    public:
        //check if T is the base type of this, T must have a static function called "S_GetTypeHierarchy"
        //and inherit Entity
        template<class T> requires std::is_base_of_v<Entity, T> &&
            std::is_same_v<decltype(T::S_GetTypeHierarchy()), const TypeHierarchy&>
        [[nodiscard]] bool IsType() const noexcept
        {
            const TypeHierarchy& typeHierarchy = T::S_GetTypeHierarchy();
            return GetTypeHierarchy().Is(typeHierarchy);
        }

    friend class Scene;
    };
}