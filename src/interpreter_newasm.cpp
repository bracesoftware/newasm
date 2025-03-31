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

#include "common/os.cpp"

#define EMPTYLINE std::cout<<"\n"
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

#include <iomanip>
#include <stdio.h>
#include <deque>

#define __newasm_included

namespace newasm
{
    const int BUILD_NUMBER = 0;
    bool vercheck = true;
    const std::string tab = "\t\t\t";
    std::unordered_map<std::string,std::vector<std::string>>* dyn_ins_set;
    std::vector<std::pair<std::string,std::string>>* env_vars;

    namespace global
    {
        const int MODE_INT = 0;
        const int MODE_CTL = 1;
        int mode = newasm::global::MODE_INT;

        std::deque<std::string> event_codeblock;
        bool event_now = false;
    }
}

#include "core/handlers.cpp"
#include "core/lang_inf.cpp"
#if __newasm_os == newasm__common__os__win32
    #include "3rd.net.win32.cpp"
#elif __newasm_os == newasm__common__os__linux
    #include "3rd.net.linux.cpp"
#endif

#include "newasm_dynlib.cpp"
#include "newasm_header.cpp"
#include "newasm_setup.cpp"

#include "threads/impl.cpp"

#include "system_calls/io_stream.cpp"
#include "system_calls/file_stream.cpp"
#include "system_calls/exec_flow.cpp"
#include "system_calls/c_manip.cpp"
#include "system_calls/text_operations.cpp"
#include "system_calls/net.cpp"
#include "system_calls/mem.cpp"

#include "core/containers.cpp"
#include "core/env_vars.cpp"

#include "newasm_exec.cpp"

#include "extra/procline_insert.cpp"
#include "extra/repl_mode.cpp"
#include "extra/utils.cpp"

#include "newasm_tests.cpp"
#include "newasm_ctl.cpp"

namespace fs = std::filesystem;

namespace newasm
{
    namespace vers
    {
        int main()
        {
            // download test
            std::string url = "https://bracesoftware.github.io/web/newasm_server/vers.txt";
            std::string output_path = newasm::core::constants::data_folder + newasm::core::constants::separator + newasm::core::constants::temp_vers;

            newasm::header::functions::info("Checking for updates...");
            newasm::utils::loadingbar("\t* Progress:        ");
            bool checkres = newasm::net::download(url, output_path);
            if(checkres)
            {
                std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Accessed the update server..."));
                std::cout << newasm::header::col::reset;std::ifstream file(output_path);
                if(!file)
                {
                    std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Error while checking the version."));
                    std::cout << newasm::header::col::reset;
                    return 1;
                }
                std::string vernumber;
                std::getline(file, vernumber);
                if(newasm::BUILD_NUMBER < std::stoi(newasm::header::functions::trim((vernumber))))
                {
                    std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Latest build ") + newasm::header::functions::trim((vernumber)) + " is available!");
                    std::cout << newasm::header::col::reset;
                    return 1;
                }
                std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Running the latest version of the runtime environment!"));
                std::cout << newasm::header::col::reset;
                return 1;
            }
            std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Cannot access the update server!"));
            std::cout << newasm::header::col::reset;
            return 1;
        }
    }
}

// MAIN

int main(int argc, char *argv[])
{
    fs::path data_folder = fs::path(newasm::core::constants::data_folder);
    if(!fs::exists(data_folder))
    {
        fs::create_directories(data_folder);
    }
    newasm::dyn_ins_set = &newasm::mem::instructions;
    //std::cout << "IDIOTISM" << std::endl;
    newasm::env_vars = &newasm::core::env_vars::priv_env_var;
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
    //mode stuff
    if(newasm::header::functions::check_args(newasm::setup::args::arg_map.at(newasm::setup::args::mode),argc,argv,argid))
    {
        if(argid < argc - 1)
        {
            newasm::global::mode = std::stoi(newasm::header::functions::trim(static_cast<std::string>(argv[argid+1])));
            /*else
            {
                newasm::header::functions::wrn("Invalid mode, resetting to default.");
                newasm::global::mode = newasm::global::MODE_INT;
            }*/
        }
        else
        {
            newasm::header::functions::err("Wrong application usage!\n\t\t\t\t\t" + newasm::header::col::gray + " newasm -mode <mode ID> -other_options");
        }
    }
    //control mode
    if(newasm::global::mode == newasm::global::MODE_CTL)
    {
        EMPTYLINE;EMPTYLINE;
        newasm::header::functions::info("Loading the CTL mode...");
        EMPTYLINE;
        newasm::ctl::main();
        return 1;
    }

    if(!newasm::header::functions::check_args(newasm::setup::args::arg_map.at(newasm::setup::args::input),argc,argv,argid) &&
    newasm::header::functions::check_args(newasm::setup::args::arg_map.at(newasm::setup::args::repl),argc,argv,argid))
    {
        newasm::header::data::exception = false;
        goto repl_label;
    }
    
    if(newasm::header::functions::check_args(newasm::setup::args::arg_map.at(newasm::setup::args::input),argc,argv,argid))
    {
        if(argid < argc - 1)
        {
            newasm::header::settings::script_file = newasm::header::functions::trim(static_cast<std::string>(argv[argid+1]));
        }
        else
        {
            newasm::header::functions::err("Wrong application usage!\n\t\t\t\t\t" + newasm::header::col::gray + " newasm -input <filename> -other_options");
            newasm::header::functions::wrn("Input file is set to `"+newasm::header::constants::default_input+"`.");
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
    if(newasm::header::functions::check_args(newasm::setup::args::arg_map.at(newasm::setup::args::tests),argc,argv,argid))
    {
        newasm::tests::main();
    }
    if(newasm::header::functions::check_args(newasm::setup::args::arg_map.at(newasm::setup::args::nover),argc,argv,argid))
    {
        newasm::vercheck = false;
    }
    if(newasm::header::functions::check_args(newasm::setup::args::arg_map.at(newasm::setup::args::log),argc,argv,argid))
    {
        newasm::header::settings::logging = true;
        //newasm::header::functions::log("yo");
    }
    newasm::header::functions::log("System loading...");

    if(newasm::vercheck)
    {
        newasm::vers::main();
        if(std::filesystem::exists(newasm::core::constants::data_folder + newasm::core::constants::separator + newasm::core::constants::temp_vers))
        {
            std::filesystem::remove(newasm::core::constants::data_folder + newasm::core::constants::separator + newasm::core::constants::temp_vers);
        }
    }

    std::cout << std::endl;
    newasm::header::execution_flow::entry_exec = newasm::header::settings::script_file;

    newasm::core::env_vars::functions::setup_env();
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
    if(newasm::header::data::exception)
    {
        newasm::header::functions::log("Process terminated...");
    }
    
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
    newasm::containers::functions::free_dyn_mem();
    newasm::header::functions::log("System unloading...");

    if(newasm::dyn_ins_set != nullptr)
    {
        delete newasm::dyn_ins_set;
        newasm::dyn_ins_set = nullptr;
    }
    if(newasm::env_vars != nullptr)
    {
        delete newasm::env_vars;
        newasm::env_vars = nullptr;
    }
    
    newasm::threads::functions::free_mem();

    return 0;
}