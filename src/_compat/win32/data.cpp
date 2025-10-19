/*

Version: MPL 1.1

The contents of this file are subject to the Mozilla Public License Version 
1.1 the "License"; you may not use this file except in compliance with 
the License. You may obtain a copy of the License at 
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the
License.

Portions created by the Initial Developer are Copyright (c) The COPYRIGHT YEAR
the Initial Developer. All Rights Reserved.

*/

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