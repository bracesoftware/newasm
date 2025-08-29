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
    namespace chars
    {
        const std::string empty = "0c0"; //empty
        const std::string endl = "0c1"; //end of line
        const std::string carret = "0c2"; //carriage return
        const std::string tab = "0c3"; //tab
        const std::string alert = "0c4"; //alert
        const std::string backspace = "0c5"; //backspace

        const std::unordered_map<std::string, std::string> map = {
            {empty, ""},
            {endl, "\n"},
            {carret, "\r"},
            {tab, "\t"},
            {alert, "\a"},
            {backspace, "\b"}
        };
    }
}