#pragma once

#include <filesystem>

#include "sndfile.h"


namespace OGLGAME
{
    class Audio
    {
    public: //data types and constants
        using ResourceIndex = size_t;
        static constexpr ResourceIndex c_invalidResourceIndex = -1;

    public:
        static sf_count_t GetByteDepth(int32_t subType);

    private: //member variables
        bool m_valid = false;
        std::filesystem::path m_path;
        ResourceIndex m_resourceIndex = c_invalidResourceIndex;
        size_t m_refCount = 0;

        int32_t m_sampleRate;
        int32_t m_numChannels;
        int32_t m_subType;

    private:
        sf_count_t m_frameCount;
        sf_count_t m_sampleCount; //frameCount * channelCount
        sf_count_t m_byteSize;
        void* m_pBytes;

    public: //constructors
        Audio() = default;

    public: //member functions
        void Load(std::filesystem::path path, ResourceIndex resourceIndex);
        void AddRef();

        void Release();
        void CleanUp();

        [[nodiscard]] bool IsValid() const { return m_valid; }
        [[nodiscard]] const std::filesystem::path& GetPath() const { return m_path; }
        [[nodiscard]] ResourceIndex GetResourceIndex() const { return m_resourceIndex; }
        [[nodiscard]] size_t GetRefCount() const { return m_refCount; }
        [[nodiscard]] int32_t GetSampleRate() const { return m_sampleRate; }
        [[nodiscard]] int32_t GetNumChannels() const { return m_numChannels; }
        [[nodiscard]] int32_t GetSubType() const { return m_subType; }
        [[nodiscard]] sf_count_t GetFrameCount() const { return m_frameCount; }
        [[nodiscard]] sf_count_t GetSampleCount() const { return m_sampleCount; }
        [[nodiscard]] sf_count_t GetByteSize() const { return m_byteSize; }
        [[nodiscard]] const void* GetAudioTrackBytes() const { return m_pBytes; }
    };
}