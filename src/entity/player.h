#pragma once

#include "entity.h"

namespace OGLGAME::Entities
{
    class Player : public IEntity
    {
    private:
        static TypeHierarchy s_typeHierarchy;
    public:
        static void CacheType();
    public:
        static const TypeHierarchy& S_GetTypeHierarchy() noexcept { return s_typeHierarchy; }
    public:
        const TypeHierarchy& GetTypeHierarchy() const noexcept override { return s_typeHierarchy; }
    private:
        void Tick(double deltaTime) override;
        void Frame(double deltaTime) override;
    };
}