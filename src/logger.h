#pragma once

#include <iostream>
#include <fmt/base.h>
#include <fmt/args.h>

#include "vgassert.h"

#define ANSI_COLOR_BLACK     "\x1b[30m"
#define ANSI_COLOR_RED       "\x1b[31m"
#define ANSI_COLOR_GREEN     "\x1b[32m"
#define ANSI_COLOR_YELLOW    "\x1b[33m"
#define ANSI_COLOR_BLUE      "\x1b[34m"
#define ANSI_COLOR_MAGENTA   "\x1b[35m"
#define ANSI_COLOR_CYAN      "\x1b[36m"
#define ANSI_COLOR_WHITE     "\x1b[37m"

#define ANSI_COLOR_BRIGHT_BLACK   "\x1b[90m"
#define ANSI_COLOR_BRIGHT_RED     "\x1b[91m"
#define ANSI_COLOR_BRIGHT_GREEN   "\x1b[92m"
#define ANSI_COLOR_BRIGHT_YELLOW  "\x1b[93m"
#define ANSI_COLOR_BRIGHT_BLUE    "\x1b[94m"
#define ANSI_COLOR_BRIGHT_MAGENTA "\x1b[95m"
#define ANSI_COLOR_BRIGHT_CYAN    "\x1b[96m"
#define ANSI_COLOR_BRIGHT_WHITE   "\x1b[97m"

#define ANSI_COLOR_RESET   "\x1b[0m"

namespace OGLGAME
{
    class LogContinue
    {
    private:
        std::string m_indent;
        char m_color[6] = { '\0' };
    private:
        LogContinue();
    private:
        void ChangeIndentSize(size_t indentSize);
        void ChangeColor(const char* color);
    public:
        template<typename... T>
        LogContinue& NextLine(const fmt::v11::format_string<T...>& fmt, T&&... args)
        {
            fmt::print("{}{}{}\n" ANSI_COLOR_RESET, m_indent, m_color, fmt::format(fmt, std::forward<T>(args)...));
            return *this;
        }
    friend class Logger;
    };

    class Logger
    {
    private:
        LogContinue m_logContinue;
        std::string m_modid;
        std::string indent;
    public:
        Logger(const std::string& modid) : m_modid(modid), m_logContinue() { }

        Logger(const Logger&) = delete;
        Logger& operator=(Logger&) = delete;
    public:
        enum LogType : uint8_t
        {
            LOG_TYPE_INFO,
            LOG_TYPE_WARNING,
            LOG_TYPE_ERROR,
            LOG_TYPE_FATAL
        };
    public:
        std::string LF2Indent(const std::string& string);
    private:
        template<typename... T>
        LogContinue& Print(const char* color, const char* prefix, const fmt::v11::format_string<T...>& fmt, T&&... args)
        {
            fmt::print("{}[{}@{}] {}\n" ANSI_COLOR_RESET, color, prefix, m_modid.c_str(), fmt::format(fmt, std::forward<T>(args)...));
            m_logContinue.ChangeColor(color);
            m_logContinue.ChangeIndentSize(1 + // "["
                std::strlen(prefix) + //"{prefix}"
                1 + //"@"
                m_modid.size() + //"{modid}"
                2); //"] "
            return m_logContinue;
        }
    public:
        template<typename... T>
        LogContinue& PrintType(LogType type, const fmt::v11::format_string<T...>& fmt, T&&... args)
        {
            switch (type)
            {
            case LOG_TYPE_WARNING:
                return Print(ANSI_COLOR_BRIGHT_YELLOW, "Warning", fmt, std::forward<T>(args)...);
            case LOG_TYPE_ERROR:
                return Print(ANSI_COLOR_BRIGHT_RED, "Error", fmt, std::forward<T>(args)...);
            case LOG_TYPE_FATAL:
                return Print(ANSI_COLOR_BRIGHT_YELLOW, "Warning", fmt, std::forward<T>(args)...);
            default:
                return Print(ANSI_COLOR_RESET, "Info", fmt, std::forward<T>(args)...);
            }
        }
        template<typename... T>
        LogContinue& Info(const fmt::v11::format_string<T...>& fmt, T&&... args)
        {
            return Print(ANSI_COLOR_RESET, "Info", fmt, std::forward<T>(args)...);
        }
        template<typename... T>
        LogContinue& Warning(const fmt::v11::format_string<T...>& fmt, T&&... args)
        {
            return Print(ANSI_COLOR_BRIGHT_YELLOW, "Warning", fmt, std::forward<T>(args)...);
        }
        template<typename... T>
        LogContinue& Error(const fmt::v11::format_string<T...>& fmt, T&&... args)
        {
            return Print(ANSI_COLOR_BRIGHT_RED, "Error", fmt, std::forward<T>(args)...);
        }
        template<typename... T>
        LogContinue& Fatal(const fmt::v11::format_string<T...>& fmt, T&&... args)
        {
            return Print(ANSI_COLOR_RED, "FATAL", fmt, std::forward<T>(args)...);
        }
    };
}