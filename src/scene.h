#pragma once

#include <type_traits>

#include "entity/entity.h"

namespace OGLGAME
{
    class Scene
    {
    public:
        static constexpr size_t sc_invalidEntityID = -1;
    private:
        size_t m_entityCount = 0;
        size_t m_entityLimit = 0;
        IEntity** m_ppEntities = nullptr;
    public:
        Scene(size_t baseEntityLimit = 128);
        ~Scene();
    private:
        IEntity* SpawnEntityInternal(IEntity* pEntity);
    public:
        template<class T> requires std::is_base_of<IEntity, T>::value
        T* SpawnEntity(T* pEntity) {
            return reinterpret_cast<T*>(SpawnEntityInternal(reinterpret_cast<IEntity*>(pEntity)));
        }
        void RemoveEntity(IEntity* pEntity);
        void RemoveEntity(size_t id);
    public:
        void Tick(double deltaTime);
        void Frame(double deltaTime);
    };
}