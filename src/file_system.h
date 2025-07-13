#pragma once

#include <filesystem>

#include <vector>
#include "exception/file_open_ex.h"

namespace OGLGAME::FS
{
    //Open and read binary file, throws OGLGAME::Exceptions::FileOpenException if file can't be opened
    std::vector<uint8_t> ReadBinFile(const std::filesystem::path& filePath);

    //Open and read text file, throws OGLGAME::Exceptions::FileOpenException if file can't be opened
    std::string ReadTxtFile(const std::filesystem::path& filePath);

    std::filesystem::path GetExePath();
}