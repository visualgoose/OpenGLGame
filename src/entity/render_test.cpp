#include "render_test.h"

namespace OGLGAME::Entities
{
    bool RenderTest::s_typeHierarchyInitialized = false;
    TypeHierarchy RenderTest::s_typeHierarchy = TypeHierarchy();

    void RenderTest::S_CacheType()
    {
        if (s_typeHierarchyInitialized)
            return;
        s_typeHierarchy.AddInterface(Renderable::S_GetType());
    }

    RenderTest::RenderTest()
    {
        SetModel("models/test.obj");
    }
}