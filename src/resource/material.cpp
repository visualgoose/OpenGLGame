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
        m_value(std::move(texturePath)) { }

    Material::Material(std::filesystem::path filePath, ResourceIndex resourceIndex) :
        m_path(std::move(filePath)),
        m_resourceIndex(resourceIndex)
    {
        auto fileData = FS::ReadTxtFile(m_path);
        if (!fileData)
        {
            g_log.Error("Failed to open material file \"{}\" with following error:", m_path.string())
                .NextLine("{}", fileData.error().GetName());
            return;
        }
        json materialJSON = json::parse((*fileData).begin(), (*fileData).end(), nullptr,
            false, true);
        if (materialJSON.is_null())
        {
            g_log.Error("Failed to parse material file \"{}\"", m_path.string());
            return;
        }

        const json& shaderPathJSON = materialJSON["shaderPath"];
        if (!shaderPathJSON.is_string())
        {
            g_log.Error("Failed to parse material file \"{}\":", m_path.string())
                .NextLine("\"shaderPath\" is not a string");
            return;
        }

        std::string shaderPathStr;
        shaderPathJSON.get_to(shaderPathStr);
        std::filesystem::path shaderAbsPath = m_path.parent_path() / shaderPathStr;

        const ResourceSystem& resourceSystem = Client::S_GetInstance().GetResourceSystem();
        ResourceSystem::ResourceID shaderID = resourceSystem.GetResourceID(shaderAbsPath);
        m_shaderIndex = shaderID.m_resourceIndex;
        if (m_shaderIndex == ResourceSystem::c_invalidResourceIndex ||
            shaderID.m_resourceType != ResourceSystem::ResourceType_shader)
        {
            std::filesystem::path shaderPath = shaderPathStr;
            shaderID = resourceSystem.GetResourceID(shaderPath);
            m_shaderIndex = shaderID.m_resourceIndex;
        }

        if (m_shaderIndex == ResourceSystem::c_invalidResourceIndex ||
            shaderID.m_resourceType != ResourceSystem::ResourceType_shader)
        {
            g_log.Error("Failed to load material file \"{}\":", m_path.string())
                .NextLine("Could not find ResourceID for shader path \"{}\" or \"{}\" or the path pointed to a non shader file",
                    shaderAbsPath.string(), shaderPathStr);
            return;
        }

        const Shader& shader = resourceSystem.GetShader(m_shaderIndex);
        const std::vector<Shader::Property> shaderProperties = shader.GetProperties();

        const json& propertyValuesJSON = materialJSON["properties"];
        m_propertyValues.resize(propertyValuesJSON.size());
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
                g_log.Error("Failed to load material file \"{}\":", m_path.string())
                    .NextLine("Could not find property id \"{}\" in shader \"{}\"", propertyName, shaderPathStr);
                return;
            }
            std::string propertyValueString;
            propertyValueJSON.value().get_to(propertyValueString);
            m_propertyValues[foundIndex] = PropertyValue(std::move(propertyValueString));
        }
        m_valid = true;
    }

    void Material::AddRef()
    {
        //variables have to be here, because of some switch statement limitations
        uint32_t uint;
        bool boolean;
        std::filesystem::path path;
        std::filesystem::path path2;
        std::string string;
        ResourceSystem& resourceSystem = Client::S_GetInstance()
                            .GetResourceSystem();
        for (auto& propertyValue : m_propertyValues)
        {
            switch (propertyValue.m_type)
            {
                case Shader::PropertyType_tex2D:
                    if (propertyValue.m_resourceIndex != c_invalidResourceIndex)
                        Client::S_GetInstance()
                            .GetResourceSystem()
                            .TextureAddRef(propertyValue.m_resourceIndex);
                    else
                    {
                        string = std::get<std::string>(propertyValue.m_value); //get tex2D path
                        path = m_path.parent_path() / string; //path relative to material path
                        uint = 0; //the first path might be valid
                        boolean = FS::MakePathRelativeToGamePath(path) && std::filesystem::is_regular_file(path); //path relative to game path
                        if (!boolean) //path relative to material was invalid
                        {
                            uint = 1; //second path might be valid
                            path2 = string;
                            boolean = FS::MakePathRelativeToGamePath(path2) && std::filesystem::is_regular_file(path2);
                        }
                        if (!boolean) //both paths were invalid
                        {
                            g_log.Error("Failed to find texture \"{}\" or \"{}\" for material \"{}\"",
                                path.string(), string, m_path.string())
                                .NextLine("Note: All resource files have to be in the game directory");
                        }
                        else
                        {
                            ResourceIndex textureIndex;
                            if (uint == 0)
                                textureIndex = resourceSystem.TextureAddRef(path);
                            else
                                textureIndex = resourceSystem.TextureAddRef(path2);
                            propertyValue.m_resourceIndex = textureIndex;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }

    void Material::Release()
    {
        for (const auto& propertyValue : m_propertyValues)
        {
            switch (propertyValue.m_type)
            {
                case Shader::PropertyType_tex2D:
                    Client::S_GetInstance()
                        .GetResourceSystem()
                        .TextureRelease(propertyValue.m_resourceIndex);
                    break;
                default:
                    break;
            }
        }
    }
}