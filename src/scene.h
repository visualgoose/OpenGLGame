#pragma once

#include <type_traits>

#include "ecs/game_object.h"

namespace OGLGAME
{
    class Scene
    {
    public: //data types
        struct GameObjectHolder
        {
            bool m_allocated = false;
            GameObject m_gameObject;
        };

    private: //member variables
        size_t m_gameObjectCount = 0;
        size_t m_gameObjectLimit = 0;
        GameObjectHolder* m_pGameObjects = nullptr;

    public: //constructors
        explicit Scene(size_t baseGameObjectLimit = 128);
        ~Scene();

    public:
        GameObject* AllocGameObject();
        void RemoveGameObject(GameObject* pGameObject, bool removeChildren);
        void RemoveGameObject(size_t id, bool removeChildren);

        void Tick(double deltaTime);
        void Frame(double deltaTime);

        [[nodiscard]] size_t GetGameObjectCount() const { return m_gameObjectCount; }
        [[nodiscard]] size_t GetGameObjectLimit() const { return m_gameObjectLimit; }
        [[nodiscard]] GameObjectHolder* GetGameObjects() const { return m_pGameObjects; }
        [[nodiscard]] GameObject* GetGameObject(GameObject::GameObjectID gameObjectID) const;
    };
}