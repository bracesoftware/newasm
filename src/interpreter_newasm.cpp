/*

The ORIGINAL CODE is the `NewASM Virtual Machine` Source Code.
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

#pragma GCC diagnostic ignored "-Wunused-result"

#include "runtime/common/os.h"

#define EMPTYLINE std::cout<<"\n"
#include <iostream>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <string_view>
#include <stdexcept>
#include <string>
#include <fstream>
#include <cstdio>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <thread>
#include <atomic>
#include <map>
#include <cstring>
//LOl
#include <limits>
#include <cstdlib>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h> // for go
#include <deque>
//For idk
#include <functional>
#include <utility>
// For registers
#include <type_traits>
#include <concepts>

#include "sysext/maps.cpp"
#include "vm/impl.cpp"
#include "sysext/csimple.cpp"
#define __newasm_included
#include "newasm_stdex.cpp"
#include "col._compat.cpp"

#include "getch._compat.cpp"

// thread init
#include "kernel/threads/_flags.cpp"

// Resources (assets) used in the program
#include "runtime/assets/text.h"
#include "runtime/assets/def.h"

namespace newasm
{
    //newasm::_std::shared_memory sharedMem;
    const int BUILD_NUMBER = 8;
    bool vercheck = true;
    bool dwin = false;
    const std::string tab = "\t\t\t";
    std::unordered_map<std::string,std::vector<std::string>>* dyn_ins_set;
    std::vector<std::pair<std::string,std::string>>* env_vars;

    namespace global
    {
        const int MODE_INT = 0;
        const int MODE_SHELL = 1;
        int mode = newasm::global::MODE_INT;

        std::deque<std::string> event_codeblock;
        bool event_now = false;
    }

    std::unordered_map<std::string, int> inverted_ins;
    std::unordered_map<std::string, int> inverted_kernel;
    std::unordered_map<std::string, int> inverted_types;

    namespace perf
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto end = std::chrono::high_resolution_clock::now();
    }
}

/*
Essential stuff needed to run
is in the runtime
*/
#include "runtime/register.h"

#include "kernel/malloc.h"
#include "runtime/handlers.cpp"
#include "runtime/lang_inf.cpp"

#include "runtime/progwin_api.cpp"
#include "utils._compat.cpp"

#include "runtime/utils.cpp"
#include "runtime/common/opcodes.h"

#include "newasm_dynlib.cpp"
#include "newasm_header.cpp"

#include "runtime/common/tokenize.h"
#include "newasm_setup.cpp"
#include "runtime/virtual.h"

#include "runtime/expcfg/decorators.cpp"
#include "kernel/threads/impl.cpp"

#include "kernel/system_calls/io_stream.cpp"
#include "kernel/system_calls/file_stream.cpp"
#include "kernel/system_calls/exec_flow.cpp"
#include "kernel/system_calls/c_manip.cpp"
#include "kernel/system_calls/text_operations.cpp"
#include "kernel/system_calls/net.cpp"
#include "kernel/system_calls/mem.cpp"
#include "kernel/system_calls/chrono.cpp"
#include "kernel/system_calls/tuple.cpp"
#include "kernel/system_calls/tcp.cpp"
#include "kernel/system_calls/http.cpp"

#include "runtime/containers.cpp"
#include "runtime/env_vars.cpp"

#include "runtime/lambda/_entry.cpp"
#include "kernel/syscall_info.cpp"

#include "vm/hardware/disk.cpp"
#include "vm/hardware/io_ports.cpp"
//
#include "kernel/dynamic/commonlibs.cpp"
#include "libs._compat.cpp"
//
#include "kernel/krnlcfg.cpp"
#include "kernel/syscall_handle.cpp"

#include "runtime/namespaces.cpp"
#include "pp/directives.cpp"

#include "compiler/comptins.cpp"
#include "compiler/asmc.cpp"

#include "vm/hardware/absolut.cpp"
#include "runtime/memory_impl.cpp"
#include "newasm_exec.cpp"

#include "runtime/procline_insert.cpp"
#include "runtime/repl_mode.cpp"

#include "newasm_compexpr.cpp"
#include "shell_tools/user.cpp"
#include "shell_tools/env_control.cpp"
#include "newasm_shell.cpp"

#include "runtime/common/chars.h"
#include "runtime/_entry.h"

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

            //newasm::header::functions::info("Checking for updates...");
            newasm::utils::loading("Checking for updates...", newasm::utils::load_speed);
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
//#define NEWASM_STRICT_TEST

#include "kernel/_utils.cpp"

namespace newasm
{
    namespace GLOBAL
    {
        void cleanup()
        {
            //std::cout << "\n";
            newasm::header::functions::log("System cleaning up...");
            std::cout << newasm::header::col::gray;
            newasm::containers::functions::free_dyn_mem();
            printf("\t\tCleaning up containers...\n");
            newasm::stack::free_macro_mem();
            printf("\t\tCleaning up macro data...\n");

            printf("\t\tCleaning up DL data...\n");
            printf("\t\tCleaning up environment variable memory...\n");
            
            newasm::threads::functions::free_mem();
            printf("\t\tCleaning up thread data...\n");
            newasm::core::env_vars::functions::save_env();
            printf("\t\tSaving environment variables...\n");
            
            printf("\t\tCleaning up dynamic tuple data...\n");
            for(auto i = newasm::variables::ids.begin(); i != newasm::variables::ids.end(); ++i)
            {
                if(i->second.type == newasm::datatypes::tuple)
                {
                    if(i->second.tuple != nullptr)
                    {
                        delete i->second.tuple;
                    }
                }
            }
            std::cout << newasm::header::col::reset;
            std::cout << std::endl;
            return;
        }
    }
}
namespace newasm
{
    int entry(int argc, char *argv[])
    {
        for(auto i = newasm::core::lang_inf::instruction_set.begin(); i != newasm::core::lang_inf::instruction_set.end(); ++i)
        {
            newasm::inverted_ins[i->second] = i->first;
        }
        for(auto i = newasm::core::lang_inf::refs::identifiers__.begin(); i != newasm::core::lang_inf::refs::identifiers__.end(); ++i)
        {
            newasm::inverted_kernel[i->second] = i->first;
        }
        for(auto i = newasm::core::lang_inf::typenames::identifiers__.begin(); i != newasm::core::lang_inf::typenames::identifiers__.end(); ++i)
        {
            newasm::inverted_types[i->second] = i->first;
        }
        //newasm::sharedMem.create("__newasm_debug", 1024); // 1024bytes of virtual mapped memory
        newasm::_virtual::virtualMemory.init(512); //512 bytes of virtual memory that can be reallocated using "malloc <number>_"
        newasm::runtime::main();
        std::string cmd;

        fs::path data_folder = fs::path(newasm::core::constants::data_folder);
        fs::path cache_folder = fs::path(newasm::core::constants::data_folder+
            newasm::core::constants::separator+
            newasm::core::constants::cache_folder
        );
        fs::path user_folder = fs::path(newasm::core::constants::data_folder+
            newasm::core::constants::separator+
            newasm::core::constants::user_folder
        );
        if(!fs::exists(data_folder))
        {
            fs::create_directories(data_folder);
        }
        if(!fs::exists(cache_folder))
        {
            fs::create_directories(cache_folder);
        }
        if(!fs::exists(user_folder))
        {
            fs::create_directories(user_folder);
        }
        //newasm::_virtual::main();
        newasm::user::main();
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
        std::cout << "\a";
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
        //shell mode
        if(newasm::global::mode == newasm::global::MODE_SHELL)
        {
            EMPTYLINE;EMPTYLINE;
            newasm::header::functions::info("Loading the shell mode...");
            EMPTYLINE;
            newasm::core::env_vars::functions::setup_env();

            newasm::ctl::main();

            newasm::GLOBAL::cleanup();
            return 1;
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
        newasm::hardware::randAccessMem.init();
        newasm::header::functions::info("Sucessfully allocated 10 MB of memory.");
        newasm::header::functions::info(
            static_cast<std::string>("Preparing to execute: ") + newasm::header::col::yellow +
            newasm::project_data::name + static_cast<std::string>(" ") + newasm::project_data::version
            + newasm::header::col::reset);

        /*
            Before executing the file we need to open the program window.
        */
        newasm::dwin = true;
        if(newasm::dwin)
        {
            if(!std::filesystem::exists(newasm::core::constants::progwin))
            {
                newasm::header::functions::err("`progwin` (debugger) not found.");
                return 1;
            }

            newasm::runtime::start_program(newasm::core::constants::progwin);
            std::string text = "Loading...\n\n" + newasm::header::col::yellow + newasm::header::style::bold + "NewASM Debug Window\n" + newasm::header::col::gray + "\tBuilt for the Virtual Machine\n\n" + newasm::header::col::reset;
            newasm::progwin::api::cout(text);
            newasm::progwin::api::flush();
            newasm::header::functions::wait(4000);
        }
        /*
            Executing
        */
        newasm::header::functions::trim(newasm::header::settings::script_file);
        newasm::execute(newasm::header::settings::script_file, -1);
        std::chrono::duration<double, std::milli> elapsed = newasm::perf::end - newasm::perf::start;

        std::cout << newasm::header::col::gray << "\t\tTime elapsed: " << elapsed.count() << " ms\n";
        std::cout << newasm::header::col::reset;

        if(newasm::mem::functions::datavalid(newasm::handlers::exit_handler, newasm::mem::funcs))
        {
            newasm::global::event_now = true;
            newasm::copyproc(newasm::handlers::exit_handler);
            for(auto i = newasm::global::event_codeblock.begin(); i != newasm::global::event_codeblock.end(); ++i)
            {
                newasm::procline(*i);
            }
            newasm::global::event_now = false;
        }

        // Print the debug buffer
        newasm::progwin::api::cout("Cleaning up the buffer...............\n--logout--\n");
        newasm::progwin::api::flush();

        newasm::header::functions::pause();
        //newasm::header::functions::wait(4000);

        newasm::header::functions::info("Cleaning up...");

        newasm::GLOBAL::cleanup();

        newasm::progwin::api::exit();

        return 0;
    }
}

#include "_entry.cpp"

#if 0

namespace newasm
{
    namespace __
    {
int main(int argc, char *argv[])
{
    newasm::runtime::main();
    std::string cmd;

    fs::path data_folder = fs::path(newasm::core::constants::data_folder);
    fs::path cache_folder = fs::path(newasm::core::constants::data_folder+
        newasm::core::constants::separator+
        newasm::core::constants::cache_folder
    );
    if(!fs::exists(data_folder))
    {
        fs::create_directories(data_folder);
    }
    if(!fs::exists(cache_folder))
    {
        fs::create_directories(cache_folder);
    }
    newasm::_virtual::main();
    newasm::dyn_ins_set = &newasm::mem::instructions;
    //std::cout << "IDIOTISM" << std::endl;
    newasm::env_vars = &newasm::core::env_vars::priv_env_var;

    std::cout << std::endl; newasm::header::functions::vers_info();

    
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
    /*newasm::project_data::impl::setup_proj();
    newasm::header::functions::info(
        static_cast<std::string>("Preparing to execute: ") + newasm::header::col::yellow +
        newasm::project_data::name + static_cast<std::string>(" ") + newasm::project_data::version
        + newasm::header::col::reset);
*/
    /*
        Before executing the file we need to open the program window.
    */
    if(newasm::dwin)
    {
        if(!std::filesystem::exists(newasm::core::constants::progwin))
        {
            newasm::header::functions::err("`progwin` (debugger) not found.");
            return 1;
        }

        newasm::progwin::api::cout("Debug window ready.");

        newasm::runtime::start_program(newasm::core::constants::progwin);
    }
    /*
        NewASM Shell
    */
    std::string command;
    std::string username = "root";
    while(true)
    {
        std::cout << newasm::header::col::green << "shell@" << 
        newasm::header::col::yellow<<username<<newasm::header::col::green
        <<" $"
        << newasm::header::col::reset;
        std::cin >> command;
        newasm::shell::main(command);
        if(newasm::shell::data::terminated)
        {
            break;
        }
    }

    // File to analyze.
    
    if(newasm::header::data::exception)
    {
        newasm::header::functions::log("Process terminated...");
    }
    
    newasm::header::functions::info("Cleaning up...");

    newasm::handles::delete_handles();
    newasm::containers::functions::free_dyn_mem();
    newasm::stack::free_macro_mem();
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

    if(newasm::dwin)
    {
        newasm::progwin::api::exit();
    }

    return 0;
}}}

        #endif

namespace newasm
{
    void __init__()
    {
        #ifdef NEWASM_STRICT_TEST
        std::string line = "  mov   \"oh, no\"";
        std::string line2 = "   ret    tlr,             \"eh, sexy\"";
        std::string line3 = "   syscall    ";
        std::string line4 = "   zero    stl";
        std::cout << newasm::common::tokenize(line).size() << char(32) <<
        newasm::common::tokenize(line2).size() << char(32) <<
        newasm::common::tokenize(line3).size() << char(32) <<
        newasm::common::tokenize(line4).size() << char(32) << std::endl; 

        std::cout << newasm::common::tokenize(line).at(0) << '|' << newasm::common::tokenize(line).at(1) << '|'<< std::endl;
        std::cout << newasm::common::tokenize(line2).at(0) << '|' << newasm::common::tokenize(line2).at(1) << '|' << newasm::common::tokenize(line2).at(2) << '|'<< std::endl;
        std::cout << newasm::common::tokenize(line3).at(0) << '|'<<std::endl;
        std::cout << newasm::common::tokenize(line4).at(0) << '|' << newasm::common::tokenize(line4).at(1) << '|'<< std::endl;

        //line = newasm::header::functions::trim(line);
        

        return 1;
        

        newasm::_register<std::string> test("test", "xd");
        test = '-' +static_cast<std::string>("\"")+'e'+test+static_cast<std::string>("\"")+'-'+static_cast<std::string>("ahaha");
        std::cout << "Test :: " << test;
        return 1;
        #endif
    }
}