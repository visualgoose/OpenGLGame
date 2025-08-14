#pragma once

#include "entity.h"
#include "interface/renderable.h"

namespace OGLGAME::Entities
{
    class RenderTest : Entity, Renderable
    {
    private: //static variables
        static TypeHierarchy s_typeHierarchy;

    public: //static functions
        static void S_CacheType();
        static const TypeHierarchy& S_GetTypeHierarchy() noexcept { return s_typeHierarchy; }

    public: //constructors
        RenderTest();
        ~RenderTest() override = default;

    public: //member functions
        const TypeHierarchy& GetTypeHierarchy() const noexcept override { return s_typeHierarchy; }
    };
}