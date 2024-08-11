/*

The ORIGINAL CODE is the `New-Assembly eXecutor` Source Code.
The INITIAL DEVELOPER is Brace Software Co., DEntisT.
The COPYRIGHT YEAR is 2024.

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

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <cstdio>
#include <sstream>
#include <filesystem>

#include <algorithm>
#include <cctype>

#define __newasm_included
//#include "newasm_syscall.hpp"
#include "newasm_header.hpp"
#include "newasm_mem.hpp"

#include "newasm_exec.hpp"

int main(int argc, char *argv[])
{
    int argid = 0;
    if(newasm::header::functions::check_args("-ver",argc,argv,argid))
    {
        newasm::header::functions::vers_info();
        return 0;
    }
    std::cout << std::endl; newasm::header::functions::vers_info();
    if(newasm::header::functions::check_args("-input",argc,argv,argid))
    {
        if(argid < argc-1)
        {
            newasm::header::settings::script_file = static_cast<std::string>(argv[argid+1]);
        }
        else
        {
            newasm::header::functions::err("Wrong application usage!\n\t\t\t\t\t" + newasm::header::col::gray + " newasm -input <filename in nax_scripts> -other_options");
            newasm::header::functions::wrn("Input file is set to `index.nax`.");
            newasm::header::settings::script_file = "index.nax";
        }
    }
    //other funny options
    if(newasm::header::functions::check_args("-help",argc,argv,argid))
    {
        newasm::header::functions::help_info();
    }

    std::cout << std::endl;

    // File to analyze.
    newasm::header::functions::trim(newasm::header::settings::script_file);
    newasm::execute
    (
        newasm::header::settings::script_file,
        newasm::code_stream::invalid_lnidx,
        newasm::analyze
        (
            newasm::header::settings::script_file
        )
    );
    return 0;
}
