#pragma once

#define OGLGAME_GAME_OBJECT_H

#include <vector>
#include <utility>

#include "entity_component.h"

#include "logging.h"

namespace OGLGAME
{
    class GameObject
    {
    public: //data types and constant variables
        using GameObjectID = size_t;
        static constexpr GameObjectID c_invalidGOID = -1;

    private: //member variables
        GameObjectID m_id = c_invalidGOID;

        std::vector<EntityComponent*> m_components;

        GameObjectID m_parentID = c_invalidGOID;
        std::vector<GameObjectID> m_children;

    private: //constructors
        //use Scene::AllocGameObject()
        GameObject();
        ~GameObject();
    public:
        GameObject(GameObject&) = delete;

    public: //operators
        GameObject& operator=(GameObject&) = delete;
    private:
        GameObject& operator=(GameObject&& other);

    public: //member functions
        void Tick(double deltaTime);
        void Frame(double deltaTime);

        void SetParent(GameObject* pParent);
        void SetParent(GameObjectID parentID);

        template<class T, typename... Args>
        T* AddComponent(Args&&... args) requires (std::is_base_of_v<EntityComponent, T> &&
            !std::is_same_v<EntityComponent, T> && std::is_same_v<decltype(T::S_GetTypeHierarchy()), const TypeHierarchy&>)
        {
            for (const auto component : m_components)
            {
                if (component->GetTypeHierarchy().IsTop(T::S_GetTypeHierarchy()))
                {
                    g_log.Error("Failed to add component to game object, because the base of the component\n"
                        "is the same of another already existing component on the game object");
                    return nullptr;
                }
            }

            auto* pEntityComponent = reinterpret_cast<EntityComponent*>(
                new T(std::forward<Args>(args)...));
            m_components.push_back(pEntityComponent);
            pEntityComponent->m_pGameObject = this;
            pEntityComponent->Setup();
            return reinterpret_cast<T*>(pEntityComponent);
        }
        template<class T> requires (std::is_base_of_v<EntityComponent, T> &&
            !std::is_same_v<EntityComponent, T> && std::is_same_v<decltype(T::S_GetTypeHierarchy()), const TypeHierarchy&>)
        void RemoveComponent()
        {
            for (auto componentIt = m_components.begin(); componentIt != m_components.end(); ++componentIt)
            {
                if ((*componentIt)->GetTypeHierarchy().IsTop(T::S_GetTypeHierarchy()))
                {
                    m_components.erase(componentIt);
                    return;
                }
            }
        }
        template<class T>
        T* GetComponent() requires (std::is_base_of_v<EntityComponent, T> &&
            !std::is_same_v<EntityComponent, T> && std::is_same_v<decltype(T::S_GetTypeHierarchy()), const TypeHierarchy&>)
        {
            for (auto component : m_components)
            {
                if (T* TComponent = component->GetECType<T>())
                    return TComponent;
            }
            return nullptr;
        }

    friend class Scene;
    };
}