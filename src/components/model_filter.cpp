#include "model_filter.h"

#include "client.h"
#include "logging.h"

namespace OGLGAME::Components
{
    bool ModelFilter::s_typeRegistered = false;
    TypeHierarchy ModelFilter::s_typeHierarchy = TypeHierarchy();

    void ModelFilter::RegisterType()
    {
        if (s_typeRegistered)
            return;
        s_typeHierarchy.AddChild("model_filter");
        s_typeRegistered = true;
    }

    ModelFilter::~ModelFilter()
    {
        ResourceSystem& resourceSystem = Client::S_GetInstance().GetResourceSystem();
        if (m_modelIndex != ResourceSystem::c_invalidResourceIndex)
            resourceSystem.ModelRelease(m_modelIndex);
    }

    void ModelFilter::SetModel(const ResourceSystem::ResourceIndex modelIndex)
    {
        if (m_modelIndex == modelIndex)
            return;
        ResourceSystem& resourceSystem = Client::S_GetInstance().GetResourceSystem();
        if (m_modelIndex != ResourceSystem::c_invalidResourceIndex)
            resourceSystem.ModelRelease(m_modelIndex);
        resourceSystem.ModelAddRef(modelIndex);
        m_modelIndex = modelIndex;
    }

    void ModelFilter::SetModel(const std::filesystem::path& modelPath)
    {
        ResourceSystem& resourceSystem = Client::S_GetInstance().GetResourceSystem();
        auto [modelIndex, resourceType] = resourceSystem.GetResourceID(modelPath);
        if (resourceType != ResourceSystem::ResourceType_model && resourceType != ResourceSystem::ResourceType_invalid)
        {
            g_log.Error("Tried setting model path to a non model path");
            return;
        }
        if (modelIndex != ResourceSystem::c_invalidResourceIndex && modelIndex == m_modelIndex)
            return;
        if (m_modelIndex != ResourceSystem::c_invalidResourceIndex)
            resourceSystem.ModelRelease(m_modelIndex);

        if (modelIndex == ResourceSystem::c_invalidResourceIndex)
            m_modelIndex = resourceSystem.ModelAddRef(modelPath);
        else
        {
            m_modelIndex = modelIndex;
            resourceSystem.ModelAddRef(modelIndex);
        }
    }
}