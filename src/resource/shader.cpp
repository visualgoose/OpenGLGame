#include "shader.h"

#include "file_system.h"
#include "logging.h"

#include <fmt/format.h>

namespace OGLGAME
{
    NLOHMANN_JSON_SERIALIZE_ENUM(Shader::PropertyType,
        {
            {Shader::PropertyType_invalid, nullptr},
            {Shader::PropertyType_tex2D, "tex2D"}
        })
    NLOHMANN_JSON_SERIALIZE_ENUM(Shader::Feature,
        {
            {Shader::Feature_invalid, nullptr},
            {Shader::Feature_mvp, "oglgame_MVP"}
        })
    NLOHMANN_JSON_SERIALIZE_ENUM(Shader::VertexAttribute,
        {
            {Shader::VertexAttribute_none, nullptr},
            {Shader::VertexAttribute_position, "position"},
            {Shader::VertexAttribute_texCoord, "texCoord"},
            {Shader::VertexAttribute_normal, "normal"},
            {Shader::VertexAttribute_boneIDs, "boneIDs"},
            {Shader::VertexAttribute_weight, "weight"},
        })

    using json = nlohmann::json;

    Shader::Shader(std::filesystem::path filePath, ResourceIndex resourceIndex) :
        m_path(std::move(filePath)),
        m_resourceIndex(resourceIndex)
    {
        auto fileData = FS::ReadTxtFile(filePath);
        if (!fileData)
        {
            g_log.Error("Failed to open shader file \"{}\" with following error:", m_path.string())
                .NextLine("{}", fileData.error().GetName());
            return;
        }
        json shaderJSON = json::parse((*fileData).begin(), (*fileData).end(), nullptr,
            false, true);
        if (shaderJSON.is_null())
        {
            g_log.Error("Failed to parse shader file \"{}\":", m_path.string());
            return;
        }

        if (!CreateShaders(shaderJSON))
            return;
        if (!CreateProgram())
            return;
        if (!LoadProperties(shaderJSON))
            return;
        if (!LoadFeatures(shaderJSON))
            return;

        m_valid = true;
    }

    Shader::~Shader() noexcept
    {
        glDeleteProgram(m_shaderProgram);
        glDeleteShader(m_vertexShader);
        glDeleteShader(m_fragmentShader);
    }

    bool Shader::CreateShaders(const json& shaderJSON)
    {
        const json& shaderJSONVertex = shaderJSON["vertex"];
        if (!shaderJSONVertex.is_object())
        {
            g_log.Error("Failed to parse shader file \"{}\":", m_path.string())
                .NextLine("\"vertex\" isn't a json object");
            return false;
        }

        const json& shaderJSONVertexPath = shaderJSONVertex["path"];
        if (!shaderJSONVertexPath.is_string())
        {
            g_log.Error("Failed to parse shader file \"{}\":", m_path.string())
                .NextLine("\"path\" in \"vertex\" isn't a string");
            return false;
        }
        std::string shaderSrcPathStr;
        shaderJSONVertexPath.get_to(shaderSrcPathStr);

        std::filesystem::path shaderSrcPath = m_path.parent_path() / shaderSrcPathStr;
        shaderSrcPathStr = shaderSrcPath.string();
        if (!FS::MakePathRelativeToGamePath(shaderSrcPath))
        {
            g_log.Error("Failed to load shader file \"{}\":", m_path.string())
                .NextLine("Vertex shader path \"{}\" isn't in the game directory", shaderSrcPathStr);
            return false;
        }
        std::expected<std::string, FS::FileOpenError> shaderSource = FS::ReadTxtFile(shaderSrcPath);
        if (!shaderSource)
        {
            g_log.Error("Failed to load shader file \"{}\":", m_path.string())
                .NextLine("Failed to read vertex shader source with error {}", shaderSource.error().GetName());
            return false;
        }
        const GLchar* pShaderSource = (*shaderSource).c_str();
        GLint shaderSourceLen = static_cast<GLint>((*shaderSource).size());

        GLint success = 0;
        GLint infoLogLen;
        std::string infoLog;

        m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(m_vertexShader, 1, &pShaderSource, &shaderSourceLen);
        glCompileShader(m_vertexShader);
        glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderiv(m_vertexShader, GL_INFO_LOG_LENGTH, &infoLogLen);
            infoLog.resize(static_cast<size_t>(infoLogLen));
            glGetShaderInfoLog(m_vertexShader, infoLogLen, nullptr, infoLog.data());
            g_log.Error("Failed to compile vertex shader from shader file \"{}\":", m_path.string())
                .NextLine("{}", infoLog);
            return false;
        }

        const json& shaderJSONVertexLayout = shaderJSONVertex["vertexLayout"];
        if (shaderJSONVertexLayout.size() > VertexAttribute_Count)
        {
            g_log.Error("Failed to parse shader file \"{}\":", m_path.string())
                .NextLine("\"vertexLayout\" in \"vertex\" is either null or "
                    "its size is bigger than the count of all possible vertex attributes");
            return false;
        }
        shaderJSONVertexLayout.get_to(m_pVertexAttributes);

        const json& shaderJSONFragment = shaderJSON["fragment"];
        if (!shaderJSONFragment.is_object())
        {
            g_log.Error("Failed to parse shader file \"{}\":", m_path.string())
                .NextLine("\"fragment\" isn't a json object");
            return false;
        }
        const json& shaderJSONFragmentPath = shaderJSONFragment["path"];
        if (!shaderJSONFragmentPath.is_string())
        {
            g_log.Error("Failed to parse shader file \"{}\":", m_path.string())
                .NextLine("\"path\" in \"fragment\" isn't a string");
            return false;
        }
        shaderJSONFragmentPath.get_to(shaderSrcPathStr);

        shaderSrcPath = m_path.parent_path() / shaderSrcPathStr;
        shaderSrcPathStr = shaderSrcPath.string();
        if (!FS::MakePathRelativeToGamePath(shaderSrcPath))
        {
            g_log.Error("Failed to load shader file \"{}\":", m_path.string())
                .NextLine("Vertex shader path \"{}\" isn't in the game directory", shaderSrcPathStr);
            return false;
        }
        shaderSource = FS::ReadTxtFile(shaderSrcPath);
        if (!shaderSource)
        {
            g_log.Error("Failed to load shader file \"{}\":", m_path.string())
                .NextLine("Failed to read vertex shader source with error {}", shaderSource.error().GetName());
            return false;
        }
        pShaderSource = (*shaderSource).c_str();
        shaderSourceLen = static_cast<GLint>((*shaderSource).size());

        m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(m_fragmentShader, 1, &pShaderSource, &shaderSourceLen);
        glCompileShader(m_fragmentShader);
        glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderiv(m_fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLen);
            infoLog.resize(static_cast<size_t>(infoLogLen));
            glGetShaderInfoLog(m_fragmentShader, infoLogLen, nullptr, infoLog.data());
            g_log.Error("Failed to compile fragment shader from shader file \"{}\":", m_path.string())
                .NextLine("{}", infoLog);
            return false;
        }
        return true;
    }

    bool Shader::CreateProgram()
    {
        GLint success;
        GLint infoLogLen;
        std::string infoLog;

        m_shaderProgram = glCreateProgram();
        glAttachShader(m_shaderProgram, m_vertexShader);
        glAttachShader(m_shaderProgram, m_fragmentShader);
        glLinkProgram(m_shaderProgram);
        glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetShaderiv(m_shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLen);
            infoLog.resize(static_cast<size_t>(infoLogLen));
            glGetProgramInfoLog(m_shaderProgram, infoLogLen, nullptr, infoLog.data());
            g_log.Error("Failed to link shader program from shader file \"{}\":", m_path.string())
                .NextLine("{}", infoLog);
            return false;
        }
        return true;
    }

    bool Shader::LoadProperties(const nlohmann::json& shaderJSON)
    {
        const json& shaderJSONProperties = shaderJSON["properties"];
        if (!shaderJSONProperties.is_null() && !shaderJSONProperties.is_array())
        {
            g_log.Error("Failed to parse shader file \"{}\":", m_path.string())
                .NextLine("\"properties\" isn't null and isn't an array");
        }
        m_properties.reserve(shaderJSONProperties.size());
        for (const auto& shaderJSONPropertyIt : shaderJSONProperties.items())
        {
            const std::string& shaderJSONPropertyKey = shaderJSONPropertyIt.key();
            const json& shaderJSONProperty = shaderJSONPropertyIt.value();

            PropertyType propertyType;
            shaderJSONProperty["type"].get_to(propertyType);
            if (propertyType == PropertyType_invalid)
            {
                g_log.Error("Failed to parse shader file \"{}\":", m_path.string())
                    .NextLine("\"type\" at key \"{}\" in \"properties\" is invalid or null", shaderJSONPropertyKey);
                return false;
            }

            const json& shaderJSONPropertyID = shaderJSONProperty["id"];
            if (!shaderJSONPropertyID.is_string())
            {
                g_log.Error("Failed to parse shader file \"{}\":", m_path.string())
                    .NextLine("\"id\" at key \"{}\" in \"properties\" isn't a string", shaderJSONPropertyKey);
                return false;
            }
            std::string propertyID;
            shaderJSONPropertyID.get_to(propertyID);

            GLint uniformLocation = glGetUniformLocation(m_shaderProgram, propertyID.c_str());
            if (uniformLocation == -1)
            {
                g_log.Error("Failed to get uniform location of id \"{}\" at key \"{}\" in \"properties\"",
                    propertyID, shaderJSONPropertyKey);
                return false;
            }

            const json& shaderJSONPropertyDisplayName = shaderJSONProperty["displayName"];
            if (!shaderJSONPropertyDisplayName.is_string())
            {
                g_log.Error("Failed to parse shader file \"{}\":", m_path.string())
                    .NextLine("\"displayName\" at key \"{}\" in \"properties\" isn't a string", shaderJSONPropertyKey);
                return false;
            }
            std::string displayName;
            shaderJSONPropertyDisplayName.get_to(displayName);

            m_properties.emplace_back(uniformLocation, propertyType, std::move(propertyID), std::move(displayName));
        }
        return true;
    }

    bool Shader::LoadFeatures(const json& shaderJSON)
    {
        const json& shaderJSONFeatures = shaderJSON["features"];
        if (shaderJSONFeatures.is_null() || shaderJSONFeatures.size() > Feature_Count)
        {
            g_log.Error("Failed to parse shader file \"{}\":", m_path.string())
                .NextLine("\"features\" is either null or its size is bigger than the count of all possible features");
            return false;
        }
        shaderJSONFeatures.get_to(m_pFeatures);
        for (uint8_t i = 0; i < Feature_Count; i++)
        {
            if (m_pFeatures[i] == Feature_invalid)
            {
                g_log.Error("Failed to parse shader file \"{}\":", m_path.string())
                    .NextLine("Value at index \"{}\" in \"features\" isn't a valid feature", i);
                return false;
            }
            m_pFeatureUniformLocations[i] = glGetUniformLocation(m_shaderProgram,
                c_featureUniformNames[m_pFeatures[i]]);
            if (m_pFeatureUniformLocations[i] == -1)
            {
                g_log.Error("Failed to get uniform location \"{}\" for feature",
                    c_featureUniformNames[m_pFeatures[i]]);
                return false;
            }
        }
        return true;
    }

    void Shader::GetFeatures(Feature* pOutFeatures, uint8_t outFeaturesSize) const noexcept
    {
        for (uint8_t i = 0; i < Feature_Count; i++)
        {
            if (i == outFeaturesSize)
                break;
            pOutFeatures[i] = m_pFeatures[i];
        }
    }

    void Shader::GetFeatureUniformLocations(GLint* pOutLocations,
        const uint8_t outLocationsSize) const noexcept
    {
        for (uint8_t i = 0; i < Feature_Count; i++)
        {
            if (i == outLocationsSize)
                break;
            pOutLocations[i] = m_pFeatureUniformLocations[i];
        }
    }

    void Shader::GetVertexAttributes(VertexAttribute *pOutAttributes, uint8_t outAttributesSize) const noexcept
    {
        for (uint8_t i = 0; i < VertexAttribute_Count; i++)
        {
            if (i == outAttributesSize)
                break;
            pOutAttributes[i] = m_pVertexAttributes[i];
        }
    }

}