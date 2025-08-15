#include "material.h"

#include "file_system.h"
#include "shader.h"
#include "logging.h"

#include "client.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace OGLGAME
{
    Material::PropertyValue::PropertyValue(std::string texturePath)
        : m_type(Shader::PropertyType_tex2D),
        m_texturePath(std::move(texturePath)) { }
    Material::PropertyValue::PropertyValue(const PropertyValue& other)
    {
        m_type = other.m_type;
        switch (other.m_type)
        {
            case Shader::PropertyType_tex2D:
                m_texturePath = other.m_texturePath;
                break;
            default:
                break;
        }
    }
    Material::PropertyValue::PropertyValue(PropertyValue&& other) noexcept
    {
        m_type = other.m_type;
        switch (other.m_type)
        {
            case Shader::PropertyType_tex2D:
                m_texturePath = std::move(other.m_texturePath);
                break;
            default:
                break;
        }
    }
    Material::PropertyValue::~PropertyValue() noexcept
    {
        switch (m_type)
        {
            case Shader::PropertyType_tex2D:
                m_texturePath.~basic_string();
                break;
            default:
                break;
        }
    }
    Material::PropertyValue& Material::PropertyValue::operator=(const PropertyValue& other)
    {
        m_type = other.m_type;
        switch (other.m_type)
        {
            case Shader::PropertyType_tex2D:
                m_texturePath = other.m_texturePath;
                break;
            default:
                break;
        }
        return *this;
    }
    Material::PropertyValue& Material::PropertyValue::operator=(PropertyValue&& other) noexcept
    {
        m_type = other.m_type;
        switch (other.m_type)
        {
            case Shader::PropertyType_tex2D:
                m_texturePath = std::move(other.m_texturePath);
                break;
            default:
                break;
        }
        return *this;
    }

    Material::Material(std::filesystem::path filePath, ResourceIndex resourceIndex) :
        m_path(std::move(filePath)),
        m_resourceIndex(resourceIndex)
    {
        auto fileData = FS::ReadTxtFile(filePath);
        if (!fileData)
        {
            g_log.Error("Failed to open material file \"{}\" with following error:", filePath.string())
                .NextLine("{}", fileData.error().GetName());
            return;
        }
        json materialJSON = json::parse((*fileData).begin(), (*fileData).end(), nullptr,
            false, true);
        if (materialJSON.is_null())
        {
            g_log.Error("Failed to parse material file \"{}\"", filePath.string());
            return;
        }

        const json& shaderPathJSON = materialJSON["shaderPath"];
        if (!shaderPathJSON.is_string())
        {
            g_log.Error("Failed to parse material file \"{}\":", filePath.string())
                .NextLine("\"shaderPath\" is not a string");
            return;
        }

        std::string shaderPathStr;
        shaderPathJSON.get_to(shaderPathStr);
        std::filesystem::path shaderPath = shaderPathStr;
        FS::MakePathRelativeToGamePath(shaderPath);
        shaderPathStr = shaderPath.string();

        const ResourceSystem& resourceSystem = Client::S_GetInstance().GetResourceSystem();
        ResourceSystem::ResourceID shaderID = resourceSystem.GetResourceID(shaderPath);
        m_shaderIndex = shaderID.m_resourceIndex;
        if (m_shaderIndex == ResourceSystem::c_invalidResourceIndex ||
            shaderID.m_resourceType != ResourceSystem::ResourceType_shader)
        {
            g_log.Error("Failed to load material file \"{}\":", filePath.string())
                .NextLine("Could not find ResourceID for shader path \"{}\" or resource type wasn't shader",
                    shaderPathStr);
            return;
        }
        const Shader& shader = resourceSystem.GetShader(m_shaderIndex);
        const std::vector<Shader::Property> shaderProperties = shader.GetProperties();

        const json& propertyValuesJSON = materialJSON["properties"];
        m_propertyValues.reserve(propertyValuesJSON.size());
        for (const auto& propertyValueJSON : propertyValuesJSON.items())
        {
            size_t foundIndex = -1;
            const std::string& propertyName = propertyValueJSON.key();
            for (size_t i = 0; i < shaderProperties.size(); ++i)
            {
                if (propertyName == shaderProperties[i].m_id)
                    foundIndex = i;
            }
            if (foundIndex == -1)
            {
                g_log.Error("Failed to load material file \"{}\":", filePath.string())
                    .NextLine("Could not find property id \"{}\" in shader \"{}\"", propertyName, shaderPathStr);
                return;
            }
            std::string propertyValueString;
            propertyValueJSON.value().get_to(propertyValueString);
            m_propertyValues[foundIndex] = PropertyValue(propertyValueString);
        }

        m_valid = true;
    }
}