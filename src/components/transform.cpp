#include "transform.h"

namespace OGLGAME::Components
{
    bool Transform::s_typeRegistered = false;
    TypeHierarchy Transform::s_typeHierarchy = TypeHierarchy();

    void Transform::RegisterType()
    {
        if (s_typeRegistered)
            return;
        s_typeHierarchy.AddChild("transform");
        s_typeRegistered = true;
    }
}