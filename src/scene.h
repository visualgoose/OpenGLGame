#pragma once

#include <type_traits>

#include "entity/entity.h"

namespace OGLGAME
{
    class Scene
    {
    public: //data types
        using EntityID = size_t;

    public: //constants
        static constexpr size_t c_invalidEntityID = -1;

    private: //member variables
        size_t m_entityCount = 0;
        size_t m_entityLimit = 0;
        Entity** m_ppEntities = nullptr;

    public: //constructors
        explicit Scene(size_t baseEntityLimit = 128);
        ~Scene();

    private: //member variables
        Entity* SpawnEntityInternal(Entity* pEntity);
    public:
        template<class T> requires std::is_base_of_v<Entity, T>
        T* SpawnEntity(T* pEntity) {
            return reinterpret_cast<T*>(SpawnEntityInternal(reinterpret_cast<Entity*>(pEntity)));
        }
        void RemoveEntity(Entity* pEntity);
        void RemoveEntity(size_t id);

        void Tick(double deltaTime);
        void Frame(double deltaTime);
    };
}