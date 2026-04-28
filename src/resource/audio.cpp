#include "audio.h"

#include "file_system.h"
#include "logging.h"

#include <sndfile.hh>

namespace OGLGAME
{
    sf_count_t Audio::GetByteDepth(const int32_t subType)
    {
        switch (subType) {
            case SF_FORMAT_PCM_16: return 2;
            case SF_FORMAT_PCM_24: return 3;
            case SF_FORMAT_PCM_32:
            case SF_FORMAT_FLOAT:  return 4;
            case SF_FORMAT_DOUBLE: return 8;
            default: return 0;
        }
    }

    void Audio::Load(std::filesystem::path path, const ResourceIndex resourceIndex)
    {
        m_path = std::move(path);
        m_resourceIndex = resourceIndex;

        SndfileHandle handle(m_path.c_str());
        if (handle.error())
        {
            g_log.Error("Failed to load audio track \"{}\":", m_path.string())
                .NextLine("{}", handle.strError());
            return;
        }
        m_sampleRate = handle.samplerate();
        m_numChannels = handle.channels();
        m_subType = handle.format() & SF_FORMAT_SUBMASK;
        m_frameCount = handle.frames();

        m_sampleCount = m_numChannels * m_frameCount;
        m_byteSize = m_sampleCount * GetByteDepth(m_subType);
        m_pBytes = new uint8_t[m_byteSize];
        handle.readRaw(m_pBytes, m_byteSize);

        m_refCount = 1;
        m_valid = true;
    }

    void Audio::AddRef()
    {
        vgassert(m_valid);

        m_refCount++;
    }

    void Audio::Release()
    {
        vgassert(m_valid);

        m_refCount--;
        if (m_refCount == 0)
            CleanUp();
    }

    void Audio::CleanUp()
    {
        m_valid = false;
    }
}