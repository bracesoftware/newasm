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
    namespace utils
    {
        void displaybar(const std::string &text, float progress)
        {
            int bar_width = 40;
            std::cout << newasm::header::col::reset << newasm::header::col::green << text << newasm::header::col::gray << "[";
            int pos = static_cast<int>(bar_width * progress);
            for (int i = 0; i < bar_width; ++i)
            {
                if (i < pos) std::cout << "=";
                else if (i == pos) std::cout << "*";
                else std::cout << " ";
            }
            std::cout << "] " << newasm::header::col::reset << int(progress * 100.0) << " %\r";
            std::cout.flush();
        }

        int loadingbar(const std::string &text)
        {
            for (int i = 0; i <= 100; i+=5)
            {
                newasm::utils::displaybar(text, i / 100.0f);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            std::cout << std::endl;
            return 0;
        }
    }
}