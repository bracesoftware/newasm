// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain


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
            const std::string electric_purple = "\033[38;2;191;0;255m";
            const std::string hot_pink = "\033[38;2;255;105;180m";
            const std::string gold = "\033[38;2;255;215;0m";
            const std::string ghost_white = "\033[38;2;248;248;255m";

            const std::string reset = "\033[0m";
        }
        namespace style
        {
            const std::string underline = "\033[4m";
            const std::string bold = "\033[1m";
            const std::string italic = "\033[3m";
            const std::string dim = "\033[2m";
            const std::string blink = "\033[5m";
            const std::string reverse = "\033[7m";
            const std::string strikethrough = "\033[9m";
        }
        namespace bg
        {
            const std::string err_red = "\033[48;5;88m";
            const std::string red = "\033[41m";
            const std::string green = "\033[42m";
            const std::string night_blue = "\033[48;2;25;25;112m";
        }
    }
}