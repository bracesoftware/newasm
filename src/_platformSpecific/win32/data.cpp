// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm
{
    namespace data
    {
        std::string getDataFolder()
        {
            char path[MAX_PATH];
            if(SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path)))
            {
                return std::string(path);
            }
            // fallback na USERPROFILE
            const char* userProfile = std::getenv("USERPROFILE");
            if(userProfile)
            {
                return std::string(userProfile) + "\\AppData\\Roaming";
            }
            return "C:\\Temp"; // fallback
        }
    }
}