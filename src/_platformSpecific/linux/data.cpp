// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm
{
    namespace data
    {
        std::string getDataFolder()
        {
            const char* xdgDataHome = std::getenv("XDG_DATA_HOME");
            if(xdgDataHome && *xdgDataHome != '\0')
            {
                return std::string(xdgDataHome);
            }

            const char* home = std::getenv("HOME");
            if(home)
            {
                return std::string(home) + "/.local/share";
            }

            return "/tmp"; // fallback
        }
    }
}