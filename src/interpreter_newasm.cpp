/*

The ORIGINAL CODE is the `NewASM Runtime Environment` Source Code.
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
#include <map>
#include <string>
#include <fstream>
#include <cstdio>
#include <sstream>
#include <filesystem>
#include <cmath>

#include <algorithm>
#include <cctype>
#include <chrono>
#include <thread>

#define __newasm_included

#include "newasm_header.cpp"
#include "newasm_setup.cpp"

#include "system_calls/io_stream.cpp"
#include "system_calls/file_stream.cpp"
#include "system_calls/exec_flow.cpp"

#include "core/lang_inf.cpp"
#include "core/containers.cpp"

#include "newasm_exec.cpp"

#include "extra/procline_insert.cpp"
#include "extra/repl_mode.cpp"
#include "extra/utils.cpp"

// MAIN

int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        newasm::header::functions::vers_info();
        std::cout << "Use `" << newasm::header::style::underline <<
        newasm::setup::args::arg_map.at(newasm::setup::args::help)
        << newasm::header::col::reset << "` for more information." << std::endl;
        std::cout << "\n";
        std::cout << "\n";
        return 1;
    }

    int argid = 0;

    if(newasm::header::functions::check_args(newasm::setup::args::arg_map.at(newasm::setup::args::ver),argc,argv,argid))
    {
        newasm::header::functions::vers_info();
        return 0;
    }

    std::cout << std::endl; newasm::header::functions::vers_info();

    if(!newasm::header::functions::check_args(newasm::setup::args::arg_map.at(newasm::setup::args::input),argc,argv,argid) &&
    newasm::header::functions::check_args(newasm::setup::args::arg_map.at(newasm::setup::args::repl),argc,argv,argid))
    {
        newasm::header::data::exception = false;
        goto repl_label;
    }
    
    if(newasm::header::functions::check_args(newasm::setup::args::arg_map.at(newasm::setup::args::input),argc,argv,argid))
    {
        if(argid < argc-1)
        {
            newasm::header::settings::script_file = newasm::header::functions::trim(static_cast<std::string>(argv[argid+1]));
        }
        else
        {
            newasm::header::functions::err("Wrong application usage!\n\t\t\t\t\t" + newasm::header::col::gray + " newasm -input <filename in nax_scripts> -other_options");
            newasm::header::functions::wrn("Input file is set to `index.nax`.");
            newasm::header::settings::script_file = newasm::header::constants::default_input;
        }
    }
    
    //other funny options
    if(newasm::header::functions::check_args(newasm::setup::args::arg_map.at(newasm::setup::args::help),argc,argv,argid))
    {
        newasm::header::functions::help_info();
    }
    if(newasm::header::functions::check_args(newasm::setup::args::arg_map.at(newasm::setup::args::extra),argc,argv,argid))
    {
        newasm::header::settings::extra = true;
    }
    if(newasm::header::functions::check_args(newasm::setup::args::arg_map.at(newasm::setup::args::cnpf),argc,argv,argid))
    {
        newasm::header::settings::create_new_projfile = true;
    }

    std::cout << std::endl;
    newasm::header::execution_flow::entry_exec = newasm::header::settings::script_file;

    newasm::project_data::impl::setup_proj();
    newasm::header::functions::info(
        static_cast<std::string>("Preparing to execute: ") + newasm::header::col::yellow +
        newasm::project_data::name + static_cast<std::string>(" ") + newasm::project_data::version
        + newasm::header::col::reset);

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
    
    repl_label:
    if(newasm::header::functions::check_args(newasm::setup::args::arg_map.at(newasm::setup::args::repl),argc,argv,argid))
    {
        if(newasm::header::data::exception)
        {
            return 0;
        }
        if(newasm::header::settings::extra)
        {
            newasm::utils::loadingbar("\t* Preparing REPL:        ");
            //newasm::utils::loadingbar("\t* Sexy:                  ");
        }
        std::cout << std::endl;
        newasm::header::functions::nullprint_wnm(
            static_cast<std::string>("Read-Eval-Print Loop mode loaded; to leave, use the `")+
            newasm::header::style::underline+static_cast<std::string>("exit")+newasm::header::col::reset+
            static_cast<std::string>("` instruction."));
        newasm::header::data::lastlndx = 0;
        newasm::header::data::repl = true;
        
        newasm::repl();
    }

    newasm::handles::delete_handles();
    return 0;
}