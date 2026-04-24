// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

//#error [NewASM] Android ain't supported.

namespace newasm
{
    inline void enable_ansi() noexcept {}
    namespace header
    {
        namespace col
        {
            const std::string red = "\033[31m";
            const std::string green = "\033[32m";
            const std::string yellow = "\033[33m";
            const std::string blue = "\033[34m";
            const std::string magenta = "\033[35m";
            const std::string cyan = "\033[36m";
            const std::string gray = "\033[90m";
            const std::string kernel = "\033[38;2;85;107;47m";

            const std::string light_red = "\x1b[38;5;210m";
            const std::string light_blue = "\x1b[38;5;117m";
            const std::string lime_teal = "\033[38;2;0;255;170m";
            const std::string orange = "\033[38;2;255;165;0m";

            const std::string aqua = "\033[38;2;41;204;193m";

            const std::string reset = "\033[0m";
        }
        namespace style
        {
            const std::string underline = "\033[4m";
            const std::string bold = "\033[1m";
        }
    }
}