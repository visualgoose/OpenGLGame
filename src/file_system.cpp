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
    std::vector<uint8_t> ReadBinFile(const fs::path& filePath)
    {
        std::ifstream file(filePath, std::ios::binary | std::ios::ate);
        if (!file.is_open())
            throw Exceptions::FileOpenException(filePath);

        size_t fileSize = file.tellg(); //tellg returns file size, because std::ios::ate was used in std::ifstream
        std::vector<uint8_t> fileBuff(fileSize);

        file.seekg(0); //set pointer to 0, so file reads properly
        file.read(reinterpret_cast<char*>(fileBuff.data()), fileSize);

        file.close();
        return fileBuff;
    }

    std::string ReadTxtFile(const fs::path& filePath)
    {
        std::ifstream file(filePath);
        if (!file.is_open())
            throw Exceptions::FileOpenException(filePath);

        std::string fileBuff(
            (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()
        );

        file.close();
        return fileBuff;
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