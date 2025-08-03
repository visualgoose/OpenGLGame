#include "file_system.h"

#include <fstream>
#include <iostream>
#include <string>

#if defined(OGLGAME_PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#elif defined(OGLGAME_PLATFORM_LINUX)
#include <unistd.h>
#endif

namespace fs = std::filesystem;

namespace OGLGAME::FS
{
    const char* FileOpenError::GetName() const noexcept
    {
        switch (m_errorCode)
        {
        case FileOpenErrorCode_notFound:
            return "FileOpenErrorCode_notFound";
        case FileOpenErrorCode_permissionDenied:
            return "FileOpenErrorCode_permissionDenied";
        case FileOpenErrorCode_notAFile:
            return "FileOpenErrorCode_notAFile";
        case FileOpenErrorCode_readFailure:
            return "FileOpenErrorCode_notAFile";
        default:
            return "FileOpenErrorCode_unknown";
        }
    }

    std::expected<std::vector<uint8_t>, FileOpenError> ReadBinFile(const std::filesystem::path& filePath)
    {
        if (!std::filesystem::exists(filePath))
            return std::unexpected(FileOpenErrorCode_notFound);

        if (!std::filesystem::is_regular_file(filePath))
            return std::unexpected(FileOpenErrorCode_notAFile);

        std::ifstream file(filePath, std::ios::binary | std::ios::ate);
        if (!file.is_open())
            return std::unexpected(FileOpenErrorCode_permissionDenied);

        size_t fileSize = file.tellg(); //tellg returns file size, because std::ios::ate was used in std::ifstream
        std::vector<uint8_t> fileData(fileSize);

        file.seekg(0); //set pointer to 0, so file reads properly
        if (!file.read(reinterpret_cast<char*>(fileData.data()), fileSize))
            return std::unexpected(FileOpenErrorCode_readFailure);

        file.close();
        return fileData;
    }

    std::expected<std::string, FileOpenError> ReadTxtFile(const std::filesystem::path& filePath)
    {
        if (!std::filesystem::exists(filePath))
            return std::unexpected(FileOpenErrorCode_notFound);

        if (!std::filesystem::is_regular_file(filePath))
            return std::unexpected(FileOpenErrorCode_notAFile);

        std::ifstream file(filePath);
        if (!file.is_open())
            return std::unexpected(FileOpenErrorCode_permissionDenied);

        std::string fileData = std::string(
            (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()
        );

        file.close();
        return fileData;
    }

    fs::path GetExePath()
    {
#if defined(OGLGAME_PLATFORM_WINDOWS)
        char exePath[MAX_PATH];
        GetModuleFileName(nullptr, exePath, MAX_PATH);
        return exePath;
#elif defined(OGLGAME_PLATFORM_LINUX)
        return fs::read_symlink("/proc/self/exe");
#endif
    }
}