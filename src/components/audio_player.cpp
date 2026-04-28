#include "audio_player.h"

#include "client.h"
#include "logging.h"

namespace OGLGAME::Components
{
    bool AudioPlayer::s_typeRegistered = false;
    TypeHierarchy AudioPlayer::s_typeHierarchy = TypeHierarchy();

    void AudioPlayer::RegisterType()
    {
        if (s_typeRegistered)
            return;
        s_typeHierarchy.AddChild("audio_player");
        s_typeRegistered = true;
    }

    AudioPlayer::~AudioPlayer()
    {
        if (m_audioIndex != ResourceSystem::c_invalidResourceIndex)
            ResourceSystem::AudioRelease(m_audioIndex);
    }

    void AudioPlayer::SetAudio(const ResourceSystem::ResourceIndex audioIndex)
    {
        if (m_audioIndex == audioIndex)
            return;
        if (m_audioIndex != ResourceSystem::c_invalidResourceIndex)
            ResourceSystem::ModelRelease(m_audioIndex);
        ResourceSystem::ModelAddRef(audioIndex);
        m_audioIndex = audioIndex;
    }

    void AudioPlayer::SetAudio(const std::filesystem::path& audioPath)
    {
        auto [audioIndex, resourceType] = ResourceSystem::GetResourceID(audioPath);
        if (resourceType != ResourceSystem::ResourceType_audioTrack && resourceType != ResourceSystem::ResourceType_invalid)
        {
            g_log.Error("Tried setting model path to a non model path");
            return;
        }
        if (audioIndex != ResourceSystem::c_invalidResourceIndex && audioIndex == m_audioIndex)
            return;
        if (m_audioIndex != ResourceSystem::c_invalidResourceIndex)
            ResourceSystem::AudioRelease(m_audioIndex);

        if (audioIndex == ResourceSystem::c_invalidResourceIndex)
            m_audioIndex = ResourceSystem::AudioAddRef(audioPath);
        else
        {
            m_audioIndex = audioIndex;
            ResourceSystem::AudioAddRef(audioIndex);
        }
    }

}