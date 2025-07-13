#include "file_open_ex.h"

#include <fmt/format.h>

namespace fs = std::filesystem;

namespace OGLGAME::Exceptions
{
    FileOpenException::FileOpenException(const fs::path& filePath)
    {
        fs::path absolute = fs::absolute(filePath);
        std::string fileName;
        if (fs::exists(absolute))
            fileName = absolute.string();
        else
            fileName = filePath.string();

        m_err = fmt::format("Failed to open file: {}", fileName);
    }

    const char* FileOpenException::what() const
    {
        return m_err.c_str();
    }
}