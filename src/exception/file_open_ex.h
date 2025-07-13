#pragma once

#include <exception>

#include <filesystem>

namespace OGLGAME::Exceptions
{
    class FileOpenException : public std::exception
    {
    private:
        std::string m_err;
    public:
        FileOpenException(const std::filesystem::path& filePath);
    public:
        const char* what() const override;
    };
}