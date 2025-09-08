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

    public: //static functions
        static GameObject* AllocGameObject();
        static void RemoveGameObject(GameObject* pGameObject, bool removeChildren);
        static void RemoveGameObject(size_t id, bool removeChildren);

        static void Tick(double deltaTime);
        static void Frame(double deltaTime);

        [[nodiscard]] static size_t GetGameObjectCount();
        [[nodiscard]] static size_t GetGameObjectLimit();
        [[nodiscard]] static GameObjectHolder* GetGameObjects();
        [[nodiscard]] static GameObject* GetGameObject(GameObject::GameObjectID gameObjectID);

    private: //member variables
        size_t m_gameObjectCount = 0;
        size_t m_gameObjectLimit = 0;
        GameObjectHolder* m_pGameObjects = nullptr;

    public: //constructors
        explicit Scene(size_t baseGameObjectLimit = 128);
        ~Scene();

    public: //member functions
        GameObject* M_AllocGameObject();
        void M_RemoveGameObject(const GameObject* pGameObject, bool removeChildren);
        void M_RemoveGameObject(GameObject::GameObjectID id, bool removeChildren);

        void M_Tick(double deltaTime);
        void M_Frame(double deltaTime);

        [[nodiscard]] size_t M_GetGameObjectCount() const { return m_gameObjectCount; }
        [[nodiscard]] size_t M_GetGameObjectLimit() const { return m_gameObjectLimit; }
        [[nodiscard]] GameObjectHolder* M_GetGameObjects() const { return m_pGameObjects; }
        [[nodiscard]] GameObject* M_GetGameObject(GameObject::GameObjectID gameObjectID) const;
    };
}