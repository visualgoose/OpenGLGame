#pragma once

#include <filesystem>

#include <vector>
#include <string>

#include <expected>

namespace OGLGAME::FS
{
    enum FileOpenErrorCode
    {
        FileOpenErrorCode_unknown,
        FileOpenErrorCode_notFound,
        FileOpenErrorCode_permissionDenied,
        FileOpenErrorCode_notAFile,
        FileOpenErrorCode_readFailure
    };
    struct FileOpenError
    {
    public:
        FileOpenErrorCode m_errorCode = FileOpenErrorCode_unknown;
    public:
        FileOpenError() noexcept = default;
        FileOpenError(const FileOpenErrorCode errorCode) noexcept : m_errorCode(errorCode) {}
    public:
        operator FileOpenErrorCode() const noexcept { return m_errorCode; }
        operator const char* () const noexcept { return GetName(); }
    public:
        const char* GetName() const noexcept;
    };

    std::expected<std::vector<uint8_t>, FileOpenError> ReadBinFile(const std::filesystem::path& filePath);

    std::expected<std::string, FileOpenError> ReadTxtFile(const std::filesystem::path& filePath);

    std::filesystem::path GetExePath();
}