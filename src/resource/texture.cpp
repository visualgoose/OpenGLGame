#include "texture.h"

#include <SDL3_image/SDL_image.h>

#include "file_system.h"
#include "logging.h"

#include <debug/debug.h>

namespace OGLGAME
{
    Texture::Texture(std::filesystem::path path, const ResourceIndex resourceIndex) :
        m_path(std::move(path)),
        m_resourceIndex(resourceIndex)
    {
        GLenum glTextureFormat;

        SDL_Surface* texData = IMG_Load(m_path.string().c_str());
        if (!texData)
        {
            g_log.Error("Failed to load texture \"{}\"", m_path.string());
            return;
        }

        switch (texData->format)
        {
            case SDL_PIXELFORMAT_RGBA32:
                glTextureFormat = GL_RGBA;
                break;
            case SDL_PIXELFORMAT_RGB24:
                glTextureFormat = GL_RGB;
                break;
            case SDL_PIXELFORMAT_BGRA32:
                glTextureFormat = GL_BGRA;
                break;
            case SDL_PIXELFORMAT_BGR24:
                glTextureFormat = GL_BGR;
                break;
            default:
                SDL_Surface* tempTexData = SDL_ConvertSurface(texData, SDL_PIXELFORMAT_RGBA32);
                if (!tempTexData)
                {
                    g_log.Error("Failed to load texture \"{}\":", m_path.string())
                        .NextLine("Image's format wasn't supported and converting failed");
                    return;
                }
                SDL_DestroySurface(texData);
                texData = tempTexData;
                glTextureFormat = GL_RGBA;
                break;
        }

        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D,
            0,
            static_cast<GLint>(glTextureFormat),
            texData->w, texData->h,
            0,
            glTextureFormat,
            GL_UNSIGNED_BYTE,
            texData->pixels);
        glGenerateMipmap(GL_TEXTURE_2D);

        SDL_DestroySurface(texData);

        m_valid = true;
    }

    void Texture::AddRef() noexcept
    {
        vgassert(m_valid);

        m_refCount++;
    }

    void Texture::Release() noexcept
    {
        vgassert(m_valid);

        m_refCount--;
    }
}