// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#define VERSION_______________ 0

#if VERSION_______________ == 1
#pragma once
#include <windows.h>
#include <iostream>
#include <vector>
#include <string_view>

namespace newasm
{
    using manip = std::ostream& (*)(std::ostream&);

    struct fmt
    {
        std::vector<manip> mods;
        std::string_view  text{};
    };

    inline fmt operator+(manip a, manip b)
    {
        return {{a, b}, {}};
    }

    inline fmt operator+(fmt f, manip m)
    {
        f.mods.push_back(m);
        return f;
    }

    inline fmt operator+(manip m, std::string_view txt)
    {
        return {{m}, txt};
    }

    inline fmt operator+(fmt f, std::string_view txt)
    {
        f.text = txt;
        return f;
    }

    inline std::ostream& operator<<(std::ostream& os, const fmt& f)
    {
        for (auto m : f.mods)
            m(os);

        os << f.text;
        return os;
    }

    inline HANDLE console()
    {
        static HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        return h;
    }

    inline std::ostream& set(std::ostream& os, WORD attr)
    {
        SetConsoleTextAttribute(console(), attr);
        return os;
    }

    namespace header
    {
        namespace col
        {
            inline std::ostream& red(std::ostream& os)
            {
                return set(os, FOREGROUND_RED | FOREGROUND_INTENSITY);
            }

            inline std::ostream& green(std::ostream& os)
            {
                return set(os, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            }

            inline std::ostream& yellow(std::ostream& os)
            {
                return set(os,
                    FOREGROUND_RED |
                    FOREGROUND_GREEN |
                    FOREGROUND_INTENSITY);
            }

            inline std::ostream& blue(std::ostream& os)
            {
                return set(os, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            }

            inline std::ostream& magenta(std::ostream& os)
            {
                return set(os,
                    FOREGROUND_RED |
                    FOREGROUND_BLUE |
                    FOREGROUND_INTENSITY);
            }

            inline std::ostream& cyan(std::ostream& os)
            {
                return set(os,
                    FOREGROUND_GREEN |
                    FOREGROUND_BLUE |
                    FOREGROUND_INTENSITY);
            }

            inline std::ostream& gray(std::ostream& os)
            {
                return set(os,
                    FOREGROUND_RED |
                    FOREGROUND_GREEN |
                    FOREGROUND_BLUE);
            }

            // ANSI: 38;2;85;107;47  (olive / kernel-like)
            inline std::ostream& kernel(std::ostream& os)
            {
                return set(os,
                    FOREGROUND_GREEN |
                    FOREGROUND_RED);
            }

            // ANSI: 38;5;210 (light red / pink)
            inline std::ostream& light_red(std::ostream& os)
            {
                return set(os,
                    FOREGROUND_RED |
                    FOREGROUND_INTENSITY);
            }

            // ANSI: 38;5;117 (light blue)
            inline std::ostream& light_blue(std::ostream& os)
            {
                return set(os,
                    FOREGROUND_BLUE |
                    FOREGROUND_GREEN |
                    FOREGROUND_INTENSITY);
            }

            //ANSI: 38;2;0;255;170 (lime teal)
            inline std::ostream& lime_teal(std::ostream& os)
            {
                return set(os,
                    FOREGROUND_GREEN |
                    FOREGROUND_BLUE |
                    FOREGROUND_INTENSITY);
            }

            //ansy: 38;2;41;204;193 (aqua)
            inline std::ostream& aqua(std::ostream& os)
            {
                return set(os,
                    FOREGROUND_GREEN |
                    FOREGROUND_BLUE);
            }

            inline std::ostream& reset(std::ostream& os)
            {
                return set(os,
                    FOREGROUND_RED |
                    FOREGROUND_GREEN |
                    FOREGROUND_BLUE);
            }
        }

        namespace style
        {
            inline std::ostream& bold(std::ostream& os)
            {
                return set(os,
                    FOREGROUND_RED |
                    FOREGROUND_GREEN |
                    FOREGROUND_BLUE |
                    FOREGROUND_INTENSITY);
            }

            inline std::ostream& underline(std::ostream& os)
            {
                return os;
            }
        }
    }
}

#endif
#if VERSION_______________ == 0
namespace newasm
{
    inline void enable_ansi() noexcept {}
    namespace header
    {
        namespace col
        {
            const std::string red = "";
            const std::string green = "";
            const std::string yellow = "";
            const std::string blue = "";
            const std::string magenta = "";
            const std::string cyan = "";
            const std::string gray = "";
            const std::string kernel = "";

            const std::string light_red = "";
            const std::string light_blue = "";
            const std::string lime_teal = "";
            const std::string orange = "";
            const std::string aqua = "";
            const std::string electric_purple = "";
            const std::string hot_pink = "";
            const std::string gold = "";
            const std::string ghost_white = "";

            const std::string reset = "";
        }
        namespace style
        {
            const std::string underline = "";
            const std::string bold = "";
            const std::string italic = "";
            const std::string dim = "";
            const std::string blink = "";
            const std::string reverse = "";
            const std::string strikethrough = "";
        }
        namespace bg
        {
            const std::string err_red = "";
            const std::string red = "";
            const std::string green = "";
            const std::string night_blue = "";
        }
    }
}
#endif