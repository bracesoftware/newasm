// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#define VERSION_______________ 0

#if VERSION_______________ == 1
#include <windows.h>

namespace newasm::winold
{
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

    // ===== osnovne boje =====

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
        return set(os, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    }

    inline std::ostream& blue(std::ostream& os)
    {
        return set(os, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }

    inline std::ostream& magenta(std::ostream& os)
    {
        return set(os, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }

    inline std::ostream& cyan(std::ostream& os)
    {
        return set(os, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }

    inline std::ostream& gray(std::ostream& os)
    {
        return set(os, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }

    inline std::ostream& kernel(std::ostream& os)
    {
        return set(os, FOREGROUND_GREEN);
    }

    inline std::ostream& light_red(std::ostream& os)
    {
        return set(os, FOREGROUND_RED | FOREGROUND_INTENSITY);
    }

    inline std::ostream& light_blue(std::ostream& os)
    {
        return set(os, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }

    inline std::ostream& lime_teal(std::ostream& os)
    {
        return set(os, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }

    inline std::ostream& aqua(std::ostream& os)
    {
        return set(os, FOREGROUND_GREEN | FOREGROUND_BLUE);
    }


    inline std::ostream& reset(std::ostream& os)
    {
        return set(os,
            FOREGROUND_RED |
            FOREGROUND_GREEN |
            FOREGROUND_BLUE);
    }


    inline std::ostream& bold(std::ostream& os)
    {
        return os;
    }

    inline std::ostream& underline(std::ostream& os)
    {
        return os;
    }
}



namespace newasm::header
{
    namespace col
    {
        inline auto red                 = newasm::winold::red;
        inline auto green               = newasm::winold::green;
        inline auto yellow              = newasm::winold::yellow;
        inline auto blue                = newasm::winold::blue;
        inline auto magenta             = newasm::winold::magenta;
        inline auto cyan                = newasm::winold::cyan;
        inline auto gray                = newasm::winold::gray;
        inline auto kernel              = newasm::winold::kernel;
        inline auto light_red           = newasm::winold::light_red;
        inline auto light_blue          = newasm::winold::light_blue;
        inline auto lime_teal           = newasm::winold::lime_teal;
        inline auto aqua                = newasm::winold::aqua;
        inline auto reset               = newasm::winold::reset;
    }
    namespace style
    {
        inline auto bold                = newasm::winold::bold;
        inline auto underline           = newasm::winold::underline;
    }
}
#endif
#if VERSION_______________ == 0
namespace newasm
{
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
            const std::string aqua = "";

            const std::string reset = "";
        }
        namespace style
        {
            const std::string underline = "";
            const std::string bold = "";
        }
    }
}
#endif