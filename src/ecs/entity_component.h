#pragma once

#if !defined(OGLGAME_GAME_OBJECT_H) && !defined(ENTITY_COMPONENT_CPP)
#error "include header game_object.h instead of entity_component"
#endif

#include "type_hierarchy.h"

namespace OGLGAME
{
    class GameObject;

    class EntityComponent
    {
    private: //member variables
        GameObject* m_pGameObject = nullptr;

    protected: //constructors
        EntityComponent() = default;
    public:
        virtual ~EntityComponent() = default;

    public: //member functions
        virtual void Setup() {}

        virtual void Tick(double deltaTime) {}
        virtual void Frame(double deltaTime) {}

        [[nodiscard]] virtual const TypeHierarchy& GetTypeHierarchy() const = 0;

        [[nodiscard]] GameObject* GetGameObject() const { return m_pGameObject; }

        template<class T> requires std::is_same_v<decltype(T::S_GetType()), TypeCache::TypeIndex>
        [[nodiscard]] T* GetInterface()
        {
            const std::ptrdiff_t interfaceOffset = GetTypeHierarchy().GetInterfaceOffset(T::S_GetType());
            if (interfaceOffset == TypeHierarchy::c_invalidInterfaceOffset)
                return nullptr;
            return reinterpret_cast<T*>(reinterpret_cast<char*>(this) + interfaceOffset);
        }
        template<class T> requires (std::is_base_of_v<EntityComponent, T> &&
            std::is_same_v<decltype(T::S_GetTypeHierarchy()), const TypeHierarchy&>)
        [[nodiscard]] T* GetECType()
        {
            const TypeHierarchy& TTypeHierarchy = T::S_GetTypeHierarchy();
            if (GetTypeHierarchy().Is(TTypeHierarchy))
                return reinterpret_cast<T*>(this);
            return nullptr;
        }

    friend class GameObject;
    };
}