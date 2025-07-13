#include "logger.h"

#include <sstream>

namespace OGLGAME
{
    LogContinue::LogContinue()
    {
        memcpy(m_color, ANSI_COLOR_RESET, sizeof(m_color) - 1);
    }

    void LogContinue::ChangeIndentSize(size_t indentSize)
    {
        m_indent.resize(indentSize, ' ');
    }
    void LogContinue::ChangeColor(const char* color)
    {
        vgassert(color != nullptr);
        memcpy(m_color, color, sizeof(m_color) - 1);
    }

    std::string Logger::LF2Indent(const std::string& string)
    {
        std::stringstream ss(string);

        size_t stringsSize = 0;
        size_t offset = 0;
        while (true)
        {
            size_t pos = string.find('\n', offset);
            if (pos != std::string::npos)
            {
                stringsSize++;
                offset += pos;
            }
            else
                break;
        }

        std::vector<std::string> strings;
        strings.reserve(stringsSize);
        std::string temp;
        while (std::getline(ss, temp, '\n'))
            strings.push_back(temp);

        //size of indents + LFs
        size_t stringSize = (strings.size() - 1) * (m_logContinue.m_indent.size() + 1);
        for (const auto& string : strings)
        {
            stringSize += string.size();
        }
        std::string out;
        out.reserve(stringSize);
        for (size_t i = 0; i < strings.size(); i++)
        {
            out.append(strings[i]);
            if (i == strings.size() - 1 ||
                (i == strings.size() - 2 &&
                (strings[i + 1].empty() || strings[i + 1][0] == '\0')))
            {
                break;
            }
            else
                out.append("\n").append(m_logContinue.m_indent);
        }
        return out;
    }
}