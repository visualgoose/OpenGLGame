#include "shader.h"

#include "file_system.h"
#include "logging.h"

#include <fmt/format.h>

namespace OGLGAME
{
    //OGLGAME::VertexLayout implementation

    VertexLayout::VertexLayout(const VertexAttributeBits* pVertexAttributes) noexcept
    {
        uint8_t vertexAttributeCount = 0;
        for (uint8_t i = 0;; i++)
        {
            if (i > (8 * sizeof(VertexAttributeBits)) / sc_bitsPerAttribute)
                break;
            if (pVertexAttributes[i] == VertexAttributeBits_none)
                break;
            vertexAttributeCount++;
        }
        Init(pVertexAttributes, vertexAttributeCount);
    }
    VertexLayout::VertexLayout(const VertexAttributeBits* pVertexAttributes, uint8_t vertexAttributeCount) noexcept
    {
        Init(pVertexAttributes, vertexAttributeCount);
    }

    void VertexLayout::Init(const VertexAttributeBits* pVertexAttributes, uint8_t vertexAttributeCount) noexcept
    {
        for (uint8_t i = 0; i < vertexAttributeCount; i++)
        {
            m_vertexAttributes |= (static_cast<VertexAttributes>(pVertexAttributes[i]) >> (i * sc_bitsPerAttribute));
        }
    }

    constexpr VertexLayout::VertexAttributeBitMask VertexLayout::GetVertexAttributeBitMask() const noexcept
    {
        VertexAttributeBitMask bitMask = 0;
        for (size_t i = 0; i < sc_bitsPerAttribute; i++)
        {
            bitMask |= 1 >> i;
        }
        return bitMask;
    }
    uint8_t VertexLayout::GetVertexAttributeIndex(VertexAttributeBits vertexAttribute) const noexcept
    {
        VertexAttributeBits curAttributeBit = static_cast<VertexAttributeBits>(m_vertexAttributes);
        for (uint8_t i = 0; i < (8 * sizeof(VertexAttributeBits)) / sc_bitsPerAttribute; i++)
        {
            if ((curAttributeBit & GetVertexAttributeBitMask()) == vertexAttribute)
                return i;
            curAttributeBit = static_cast<VertexAttributeBits>(curAttributeBit << sc_bitsPerAttribute);
        }
        return -1;
    }

    //OGLGAME::Shader implementation

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
    NLOHMANN_JSON_SERIALIZE_ENUM(VertexLayout::VertexAttributeBits,
        {
            {VertexLayout::VertexAttributeBits_none, nullptr},
            {VertexLayout::VertexAttributeBits_position, "position"},
            {VertexLayout::VertexAttributeBits_texCoord, "texCoord"}
        })

    using json = nlohmann::json;

    Shader::Shader(const std::filesystem::path& filePath) : m_path(filePath)
    {
        auto fileData = FS::ReadTxtFile(filePath);
        if (!fileData)
        {
            g_log.Error("Failed to open shader file \"{}\" with following error:", filePath.string())
                .NextLine("{}", fileData.error().GetName());
            return;
        }
        json shaderJSON = json::parse((*fileData).begin(), (*fileData).end(), nullptr,
            false, true);
        if (shaderJSON.is_null())
        {
            g_log.Error("Failed to parse shader file \"{}\":", filePath.string());
            return;
        }

        if (!CreateShaders(shaderJSON))
            return;
        if (!CreateProgram(shaderJSON))
            return;
        if (!LoadProperties(shaderJSON))
            return;
        if (!LoadFeatures(shaderJSON))
            return;

        m_valid = true;
    }
    bool Shader::CreateShaders(const json& shaderJSON)
    {
        json shaderJSONVertex = shaderJSON["vertex"];
        if (!shaderJSONVertex.is_object())
        {
            g_log.Error("Failed to parse shader file \"{}\":", m_path.string())
                .NextLine("\"vertex\" isn't a json object");
            return false;
        }
        json shaderJSONVertexPath = shaderJSONVertex["path"];
        if (!shaderJSONVertexPath.is_string())
        {
            g_log.Error("Failed to parse shader file \"{}\":", m_path.string())
                .NextLine("\"path\" in \"vertex\" isn't a string");
            return false;
        }
        std::string shaderSourcePath;
        shaderJSONVertexPath.get_to(shaderSourcePath);
        std::filesystem::path relativePath = m_path.parent_path() / shaderSourcePath;
        std::expected<std::string, FS::FileOpenError> shaderSource = FS::ReadTxtFile(relativePath)
            .or_else([&](FS::FileOpenError relativeError)
                {
                    shaderSource = FS::ReadTxtFile(std::filesystem::path(shaderSourcePath));
                    if (!shaderSource)
                    {
                        g_log.Error("Failed to open vertex shader source from shader file \"{}\":", m_path.string())
                            .NextLine("Relative path: {}", relativePath.string())
                            .NextLine("Relative error: {}", relativeError.GetName())
                            .NextLine("Not relative path: {}", shaderSourcePath)
                            .NextLine("Not realtive error: {}", shaderSource.error().GetName());
                    }
                    return shaderSource;
                });
        if (!shaderSource)
            return false;
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

        json shaderJSONVertexLayout = shaderJSONVertex["vertexLayout"];
        if (shaderJSONVertexLayout.size() > VertexLayout::VertexAttributeBits_Count)
        {
            g_log.Error("Failed to parse shader file \"{}\":", m_path.string())
                .NextLine("\"vertexLayout\" in \"vertex\" is either null or "
                    "its size is bigger than the count of all possible vertex attributes");
            return false;
        }
        VertexLayout::VertexAttributeBits pVertexAttributes[VertexLayout::VertexAttributeBits_Count]
            = { VertexLayout::VertexAttributeBits_none };
        shaderJSONVertexLayout.get_to(pVertexAttributes);
        m_vertexLayout = VertexLayout(pVertexAttributes);

        json shaderJSONFragment = shaderJSON["fragment"];
        if (!shaderJSONFragment.is_object())
        {
            g_log.Error("Failed to parse shader file \"{}\":", m_path.string())
                .NextLine("\"fragment\" isn't a json object");
            return false;
        }
        json shaderJSONFragmentPath = shaderJSONFragment["path"];
        if (!shaderJSONFragmentPath.is_string())
        {
            g_log.Error("Failed to parse shader file \"{}\":", m_path.string())
                .NextLine("\"path\" in \"fragment\" isn't a string");
            return false;
        }
        shaderJSONFragmentPath.get_to(shaderSourcePath);
        relativePath = m_path.parent_path() / shaderSourcePath;
        shaderSource = FS::ReadTxtFile(relativePath)
            .or_else([&](FS::FileOpenError relativeError)
                {
                    shaderSource = FS::ReadTxtFile(std::filesystem::path(shaderSourcePath));
                    if (!shaderSource)
                    {
                        g_log.Error("Failed to open fragment shader source from shader file \"{}\":", m_path.string())
                            .NextLine("Relative path: {}", relativePath.string())
                            .NextLine("Relative error: {}", relativeError.GetName())
                            .NextLine("Not relative path: {}", shaderSourcePath)
                            .NextLine("Not realtive error: {}", shaderSource.error().GetName());
                    }
                    return shaderSource;
                });
        if (!shaderSource)
            return false;
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
    bool Shader::CreateProgram(const json& shaderJSON)
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
        json shaderJSONProperties = shaderJSON["properties"];
        if (!shaderJSONProperties.is_null() && !shaderJSONProperties.is_array())
        {
            g_log.Error("Failed to parse shader file \"{}\":", m_path.string())
                .NextLine("\"properties\" isn't null and isn't an array");
        }
        m_properties.reserve(shaderJSONProperties.size());
        for (const auto& shaderJSONPropertyIt : shaderJSONProperties.items())
        {
            const std::string& shaderJSONPropertyKey = shaderJSONPropertyIt.key();
            json shaderJSONProperty = shaderJSONPropertyIt.value();

            PropertyType propertyType;
            shaderJSONProperty["type"].get_to(propertyType);
            if (propertyType == PropertyType_invalid)
            {
                g_log.Error("Failed to parse shader file \"{}\":", m_path.string())
                    .NextLine("\"type\" at key \"{}\" in \"properties\" is invalid or null", shaderJSONPropertyKey);
                return;
            }

            json shaderJSONPropertyID = shaderJSONProperty["id"];
            if (!shaderJSONPropertyID.is_string())
            {
                g_log.Error("Failed to parse shader file \"{}\":", m_path.string())
                    .NextLine("\"id\" at key \"{}\" in \"properties\" isn't a string", shaderJSONPropertyKey);
                return;
            }
            std::string propertyID;
            shaderJSONPropertyID.get_to(propertyID);

            GLint uniformLocation = glGetUniformLocation(m_shaderProgram, propertyID.c_str());
            if (uniformLocation == -1)
            {
                g_log.Error("Failed to get uniform location of id \"{}\" at key \"{}\" in \"properties\"",
                    propertyID, shaderJSONPropertyKey);
                return;
            }

            json shaderJSONPropertyDisplayName = shaderJSONProperty["displayName"];
            if (!shaderJSONPropertyDisplayName.is_string())
            {
                g_log.Error("Failed to parse shader file \"{}\":", m_path.string())
                    .NextLine("\"displayName\" at key \"{}\" in \"properties\" isn't a string", shaderJSONPropertyKey);
                return;
            }
            std::string displayName;
            shaderJSONPropertyDisplayName.get_to(displayName);

            m_properties.emplace_back(uniformLocation, propertyType, std::move(propertyID), std::move(displayName));
        }
    }
    bool Shader::LoadFeatures(const json& shaderJSON)
    {
        json shaderJSONFeatures = shaderJSON["features"];
        if (shaderJSONFeatures.is_null() || shaderJSONFeatures.size() > Feature_Count)
        {
            g_log.Error("Failed to parse shader file \"{}\":", m_path.string())
                .NextLine("\"features\" is either null or its size is bigger than the count of all possible features");
            return;
        }
        shaderJSONFeatures.get_to(m_features);
        for (uint8_t i = 0; i < Feature_Count; i++)
        {
            if (m_features[i] == Feature_invalid)
            {
                g_log.Error("Failed to parse shader file \"{}\":", m_path.string())
                    .NextLine("value at index \"{}\" in \"features\" isn't a valid feature", i);
                return;
            }
        }
    }
    Shader::~Shader() noexcept
    {
        glDeleteProgram(m_shaderProgram);
        glDeleteShader(m_vertexShader);
        glDeleteShader(m_fragmentShader);
    }
}