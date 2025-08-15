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
    public: //member variables
        FileOpenErrorCode m_errorCode = FileOpenErrorCode_unknown;

    public: //constructors
        FileOpenError() noexcept = default;
        explicit FileOpenError(const FileOpenErrorCode errorCode) noexcept : m_errorCode(errorCode) {}

    public: //operators
        explicit operator FileOpenErrorCode() const noexcept { return m_errorCode; }
        explicit operator const char* () const noexcept { return GetName(); }

    public: //member functions
        [[nodiscard]] const char* GetName() const noexcept;
    };

    std::expected<std::vector<uint8_t>, FileOpenError> ReadBinFile(const std::filesystem::path& filePath);
    std::expected<std::string, FileOpenError> ReadTxtFile(const std::filesystem::path& filePath);

    /*
    * \param path the path that will be made relative
    * 
    * \returns is resulting path in the game directory
    */
    bool MakePathRelativeToGamePath(std::filesystem::path& path);

    std::filesystem::path GetExePath();
}