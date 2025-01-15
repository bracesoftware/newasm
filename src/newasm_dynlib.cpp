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

#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

namespace newasm
{
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

            const std::string reset = "\033[0m";
        }
        namespace style
        {
            const std::string underline = "\033[4m";
            const std::string bold = "\033[1m";
        }
    }
    namespace dynlib
    {
        namespace err_codes
        {
            const int labels_unsupported = 1;
            const int unsupported_instruction = 2;

            const std::unordered_map<int, std::string> identifier = {
                {labels_unsupported, "LabelsUnsupported"},
                {unsupported_instruction, "UnsupportedInstruction"}
            };
        }
        namespace mem
        {
            std::vector<std::string> invalid_dynlibs;
        }
        namespace settings
        {
            std::string analyzed_dynlib;
            std::string lastline;
            int lastlinedx;
        }
        namespace functions
        {
            void abort(int error_code)
            {
                newasm::dynlib::mem::invalid_dynlibs.push_back(newasm::dynlib::settings::analyzed_dynlib);
                std::cout << newasm::header::col::reset
                << newasm::header::col::red << "\t\t\t\t"
                << "Aborted dynamic library implementation.\n"<< "\t\t\t\t"
                << "Runtime error " << error_code << " \"" << newasm::header::col::gray << newasm::header::style::underline
                << newasm::dynlib::err_codes::identifier.at(error_code) << newasm::header::col::reset
                << newasm::header::col::red << "\" in " << newasm::header::col::gray << newasm::header::style::underline
                << newasm::dynlib::settings::analyzed_dynlib << ":" << newasm::dynlib::settings::lastlinedx << newasm::header::col::reset
                << newasm::header::col::red << " >> " << newasm::header::col::gray << newasm::dynlib::settings::lastline << "\n";
                return;
            }
        }
    }
}