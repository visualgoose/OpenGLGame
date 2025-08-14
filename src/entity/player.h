#pragma once

#include "entity.h"

namespace OGLGAME::Entities
{
    class Player : public Entity
    {
    private: //static variables
        static TypeHierarchy s_typeHierarchy;

    public: //static functions
        static void S_CacheType();
        static const TypeHierarchy& S_GetTypeHierarchy() noexcept { return s_typeHierarchy; }

    public: //member functions
        const TypeHierarchy& GetTypeHierarchy() const noexcept override { return s_typeHierarchy; }
    private:
        void Tick(double deltaTime) override;
        void Frame(double deltaTime) override;
    };
}