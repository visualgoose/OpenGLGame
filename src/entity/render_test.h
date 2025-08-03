#pragma once

#include "entity.h"
#include "interface/renderable.h"

namespace OGLGAME::Entities
{
    class RenderTest : IEntity, IRenderable
    {
    private:
        static TypeHierarchy s_typeHierarchy;
    private:
        static void S_CacheType();
    public:
        static const TypeHierarchy& S_GetTypeHierarchy() noexcept { return s_typeHierarchy; }
    public:
        RenderTest();
        ~RenderTest() override = default;
    public:
        const TypeHierarchy& GetTypeHierarchy() const noexcept override { return s_typeHierarchy; }
        friend class Client;
    };
}