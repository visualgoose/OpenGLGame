#include "render_test.h"

namespace OGLGAME::Entities
{
    TypeHierarchy RenderTest::s_typeHierarchy = TypeHierarchy();

    void RenderTest::S_CacheType()
    {
        s_typeHierarchy.AddInterface(Renderable::S_GetType());
    }

    RenderTest::RenderTest()
    {
        SetModel("models/test.obj");
        std::hash<std::string> xd = std::hash<std::string>();
    }
}