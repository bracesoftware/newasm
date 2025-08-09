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
    namespace ctl
    {
        const std::string download_server = "https://bracesoftware.github.io/web/newasm_server/dynlibs/";
        const int first_column = 20;
        const int second_column = 20;
        const int third_column = 20;
        const std::string tabs = "\t";
        const std::vector<std::pair<std::string, std::pair<std::string,std::string>>> help_table_data = {
            {"help",      {"/",             "Displays the help panel."}},
            {"exit",      {"/",             "Exits the application."}},
            {"repl",      {"/",             "Load the read-eval-print console."}},
            {"install",   {"<lib>",         "Installs a dynamic library."}},
            {"login",     {"/",             "Login into your local account."}},
            {"logout",    {"/",             "Log out of your local account."}},
            {"addenv",    {"/",             "Add an environment variable."}},
            {"remenv",    {"/",             "Remove an environment variable."}},
            {"modenv",    {"/",             "Modify an environment variable."}}
        };
        void help_info()
        {
            std::cout << "\n" << newasm::header::col::reset;
            std::cout
                    << std::setw(first_column) << std::left<<newasm::header::style::underline + newasm::header::style::bold+tabs+"Command"
                    << std::setw(second_column) << std::left<<"Arguments" 
                    << std::setw(third_column) << std::left<<"Description"
            << "\n" << newasm::header::col::reset;

            std::string argument, params, description;

            for(auto i = help_table_data.begin(); i != help_table_data.end(); ++i)
            {
                argument = i->first;
                params = i->second.first;
                description = i->second.second;
                std::cout 
                        << std::setw(first_column) << std::left<<tabs+newasm::header::col::reset + argument
                        << std::setw(second_column) << std::left<<params
                        << std::setw(third_column) << std::left<<newasm::header::col::gray + description
                << "\n";
            }

            std::cout << "\n";
            return;
        }

        namespace data
        {
            std::string cmd;
            bool finish = false;
            bool progwin = false;
        }

        namespace install
        {
            namespace const_
            {
                const int NO = -1;
                const int DIRECT_DYN = 0;
                const int SETUP = 1;
            }

            namespace setup_
            {
                const int GARBAGE = 0;
                const int DLNAME = 1;
                const int MSG = 2;
                const int IMPL = 3;
            }

            int type = newasm::ctl::install::const_::NO;
            int setup = newasm::ctl::install::setup_::GARBAGE;
            std::string dynlib_name;
            std::string download_msg;
            std::vector<std::string> dynlib_content;
        }
        
        namespace impl
        {
            int process_c(std::string cmd)
            {
                auto tokenize = [](std::string str) -> std::vector<std::string> {
                    std::vector<std::string> tokens;
                    std::string token;
                    std::istringstream stream(str);

                    while(stream >> token)
                    {
                        tokens.push_back(token);
                    }

                    return tokens;
                };
                auto repl = []() -> void {
                    newasm::header::data::repl_end = false;
                    newasm::header::functions::nullprint_wnm(
                        static_cast<std::string>("Read-Eval-Print Loop mode loaded; to leave, use the `")+
                        newasm::header::style::underline+static_cast<std::string>("exit")+newasm::header::col::reset+
                        static_cast<std::string>("` instruction."));
                    newasm::header::data::lastlndx = 0;
                    newasm::header::data::repl = true;
                    
                    newasm::repl();
                };
                auto dwin = []() -> void {
                    if(newasm::ctl::data::progwin)
                    {
                        newasm::header::functions::err("`progwin` (debugger) already running.");
                        return;
                    }
                    if(!std::filesystem::exists(newasm::core::constants::progwin))
                    {
                        newasm::header::functions::err("`progwin` (debugger) not found.");
                        return;
                    }

                    newasm::runtime::start_program(newasm::core::constants::progwin);
                    std::string text = newasm::header::col::yellow + newasm::header::style::bold + "NewASM Debug Window loaded...\n" + newasm::header::col::reset;
                    newasm::progwin::api::cout(text);

                    newasm::ctl::data::progwin = true;
                    return;
                };
                std::vector<std::string> tokens = tokenize(cmd);
                for(int i = 0; i < tokens.size(); ++i)
                {
                    tokens[i] = newasm::header::functions::trim(tokens[i]);
                }
                if(tokens.size() == 0)
                {
                    return 1;
                }
                if(tokens.size() == 1)
                {
                    if(tokens[0] == newasm::core::lang_inf::cmds::identifiers__.at(newasm::core::lang_inf::cmds::exit__))
                    {
                        newasm::ctl::data::finish = true;
                        newasm::header::functions::info("Cleaning up...");
                        return 1;
                    }
                    if(tokens[0] == newasm::core::lang_inf::cmds::identifiers__.at(newasm::core::lang_inf::cmds::login__))
                    {
                        newasm::user::impl::login();
                        return 1;
                    }
                    if(tokens[0] == newasm::core::lang_inf::cmds::identifiers__.at(newasm::core::lang_inf::cmds::logout__))
                    {
                        newasm::user::main();
                        return 1;
                    }
                    /*
                        NEED LOGIN
                    */
                    if(tokens[0] == newasm::core::lang_inf::cmds::identifiers__.at(newasm::core::lang_inf::cmds::help__))
                    {
                        _newasm_CHECKLOGIN;
                        newasm::ctl::help_info();
                        return 1;
                    }
                    if(tokens[0] == newasm::core::lang_inf::cmds::identifiers__.at(newasm::core::lang_inf::cmds::repl__))
                    {
                        _newasm_CHECKLOGIN;
                        repl();
                        return 1;
                    }
                    if(tokens[0] == newasm::core::lang_inf::cmds::identifiers__.at(newasm::core::lang_inf::cmds::addenv__))
                    {
                        _newasm_CHECKLOGIN;
                        newasm::env_control::addenv();
                        return 1;
                    }
                    if(tokens[0] == newasm::core::lang_inf::cmds::identifiers__.at(newasm::core::lang_inf::cmds::modenv__))
                    {
                        _newasm_CHECKLOGIN;
                        newasm::env_control::modenv();
                        return 1;
                    }
                    if(tokens[0] == newasm::core::lang_inf::cmds::identifiers__.at(newasm::core::lang_inf::cmds::remenv__))
                    {
                        _newasm_CHECKLOGIN;
                        newasm::env_control::remenv();
                        return 1;
                    }
                }
                if(tokens.size() == 2)
                {
                    _newasm_CHECKLOGIN;
                    if(tokens[0] == newasm::core::lang_inf::cmds::identifiers__.at(newasm::core::lang_inf::cmds::install__))
                    {
                        newasm::header::functions::info(static_cast<std::string>("Attempting to install the \"") + tokens[1] + static_cast<std::string>("\" package."));
                        newasm::ctl::install::dynlib_content.clear();
                        newasm::ctl::install::type = newasm::ctl::install::const_::NO;
                        std::string output_path = tokens[1] + newasm::core::constants::dynlib_ex;
                        /*if(std::filesystem::exists(output_path))
                        {
                            std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Library with the same name has already been installed!"));
                            std::cout << newasm::header::col::reset;
                            return 1;
                        }*/
                        newasm::utils::loadingbar("\t* Progress:        ");
                        if(newasm::net::download(newasm::ctl::download_server + tokens[1] + newasm::core::constants::dynlib_ex, output_path))
                        {
                            newasm::ctl::install::type = newasm::ctl::install::const_::DIRECT_DYN;
                        }
                        if(newasm::net::download(newasm::ctl::download_server + tokens[1] + newasm::core::constants::setup_ex,
                            newasm::core::constants::data_folder + newasm::core::constants::separator + tokens[1] + newasm::core::constants::setup_ex))
                        {
                            output_path = newasm::core::constants::data_folder + newasm::core::constants::separator + tokens[1] + newasm::core::constants::setup_ex;
                            newasm::ctl::install::type = newasm::ctl::install::const_::SETUP;
                        }
                        
                        if(newasm::ctl::install::type == newasm::ctl::install::const_::DIRECT_DYN) //if dynamic lib
                        {
                            std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Accessed the download server..."));
                            std::cout << newasm::header::col::reset;
                            if(!std::filesystem::exists(output_path))
                            {
                                std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Error while downloading the library."));
                                std::cout << newasm::header::col::reset;
                                return 1;
                            }
                           
                            std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Successfully downloaded the library!"));
                            std::cout << newasm::header::col::reset;

                            return 1;
                        }
                        if(newasm::ctl::install::type == newasm::ctl::install::const_::SETUP) //if setup
                        {
                            std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Accessed the download server..."));
                            std::cout << newasm::header::col::reset;
                            //std::cout << "debug - '" + output_path << "'" << std::endl;
                            if(!std::filesystem::exists(output_path))
                            {
                                std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Error while downloading the setup."));
                                std::cout << newasm::header::col::reset;
                                return 1;
                            }
                           
                            std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Successfully downloaded the setup."));
                            std::cout << newasm::header::col::reset;

                            std::ifstream file(output_path);
                            std::vector<std::string> tokens2;
                            newasm::ctl::install::dynlib_name.clear();
                            newasm::ctl::install::dynlib_content.clear();
                            newasm::ctl::install::download_msg.clear();
                            if(file)
                            {
                                std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Preparing to run the setup..."));
                                std::cout << newasm::header::col::reset;

                                newasm::ctl::install::setup = newasm::ctl::install::setup_::GARBAGE;
                            }
                            if(file)
                            {
                                //newasm::header::functions::wrn("DEBUG 1");
                                std::string line;
                                while(std::getline(file, line))
                                {
                                    tokens2.clear();
                                    if(newasm::header::functions::strfind(line, ':'))
                                    {
                                        //newasm::header::functions::wrn(static_cast<std::string>("PASSED : ") + line);
                                        tokens2 = newasm::header::functions::split_fixed(line, ':');
                                        tokens2[0] = newasm::header::functions::trim(tokens2[0]);
                                        tokens2[1] = newasm::header::functions::trim(tokens2[1]);
                                        //newasm::header::functions::wrn(static_cast<std::string>("PASSED 2 : ") + line);
                                    }
                                    //newasm::header::functions::wrn("DEBUG 2");
                                    if(tokens2.size() > 1) if(tokens2[0] == static_cast<std::string>("~setup"))
                                    {
                                        if(tokens2[1] == static_cast<std::string>("dlname"))
                                        {
                                            newasm::ctl::install::setup = newasm::ctl::install::setup_::DLNAME;
                                            continue;
                                        }
                                        if(tokens2[1] == static_cast<std::string>("msg"))
                                        {
                                            newasm::ctl::install::setup = newasm::ctl::install::setup_::MSG;
                                            continue;
                                        }
                                        if(tokens2[1] == static_cast<std::string>("impl"))
                                        {
                                            newasm::ctl::install::setup = newasm::ctl::install::setup_::IMPL;
                                            continue;
                                        }
                                    }
                                    /*if(newasm::ctl::install::setup == newasm::ctl::install::setup_::DLNAME)
                                        newasm::header::functions::wrn(static_cast<std::string>("PASSED LINE - dlname : ") + line);
                                    if(newasm::ctl::install::setup == newasm::ctl::install::setup_::IMPL)
                                        newasm::header::functions::wrn(static_cast<std::string>("PASSED LINE - impl : ") + line);
                                    if(newasm::ctl::install::setup == newasm::ctl::install::setup_::MSG)
                                        newasm::header::functions::wrn(static_cast<std::string>("PASSED LINE - msg : ") + line);*/

                                    if(newasm::ctl::install::setup == newasm::ctl::install::setup_::GARBAGE)
                                    {
                                        continue;
                                    }
                                    if(newasm::ctl::install::setup == newasm::ctl::install::setup_::DLNAME)
                                    {
                                        newasm::ctl::install::dynlib_name = newasm::header::functions::trim(line);
                                        /*std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("* Imported the dynamic library name..."));
                                        std::cout << newasm::header::col::reset;
                                        newasm::header::functions::wrn(static_cast<std::string>("PASSED DLNAME : ") + line);*/
                                    }
                                    if(newasm::ctl::install::setup == newasm::ctl::install::setup_::MSG)
                                    {
                                        newasm::ctl::install::download_msg = newasm::header::functions::trim(line);
                                        /*std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("* Imported the dynamic library install message..."));
                                        std::cout << newasm::header::col::reset;
                                        newasm::header::functions::wrn(static_cast<std::string>("PASSED MSG : ") + line);*/
                                    }
                                    if(newasm::ctl::install::setup == newasm::ctl::install::setup_::IMPL)
                                    {
                                        newasm::ctl::install::dynlib_content.push_back(newasm::header::functions::trim(line));
                                        /*std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("* Imported the dynamic library implementation..."));
                                        std::cout << newasm::header::col::reset;
                                        newasm::header::functions::wrn(static_cast<std::string>("PASSED IMPL : ") + line);*/
                                    }
                                }
                                file.close();
                            }

                            if(!newasm::ctl::install::dynlib_name.empty())
                            {
                                std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("* Imported the dynamic library name..."));
                                std::cout << newasm::header::col::reset;
                            }
                            if(!newasm::ctl::install::download_msg.empty())
                            {
                                std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("* Imported the dynamic library install message..."));
                                std::cout << newasm::header::col::reset;
                            }
                            if(!newasm::ctl::install::dynlib_content.empty())
                            {
                                std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("* Imported the dynamic library implementation..."));
                                std::cout << newasm::header::col::reset;
                            }

                            /*if(!file)
                            {
                                std::cout << newasm::header::col::red;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Failed to read the setup."));
                                std::cout << newasm::header::col::reset;
                                return 1;
                            }*/
                            //

                            if(!newasm::header::functions::isalphanum(newasm::ctl::install::dynlib_name))
                            {
                                std::cout << newasm::header::col::red;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Invalid dynamic library name: ") + newasm::ctl::install::dynlib_name);
                                std::cout << newasm::header::col::reset;
                                if(std::filesystem::exists(output_path)) std::filesystem::remove(output_path);
                                return 1;
                            }
                            if(newasm::ctl::install::dynlib_content.empty())
                            {
                                std::cout << newasm::header::col::red;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Dynamic library is empty."));
                                std::cout << newasm::header::col::reset;
                                if(std::filesystem::exists(output_path)) std::filesystem::remove(output_path);
                                return 1;
                            }

                            std::string final_output = newasm::ctl::install::dynlib_name + newasm::core::constants::dynlib_ex;
                            std::ofstream final_file(final_output);
                            if(final_file)
                            {
                                for(auto i = newasm::ctl::install::dynlib_content.begin(); i != newasm::ctl::install::dynlib_content.end(); ++i)
                                {
                                    final_file << *i << "\n";
                                }
                                final_file.close();
                            }
                            if(!final_file)
                            {
                                std::cout << newasm::header::col::red;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Failed to install the dynamic library."));
                                std::cout << newasm::header::col::reset;
                                if(std::filesystem::exists(output_path)) std::filesystem::remove(output_path);
                                return 1;
                            }
                            
                            ///////////////
                            std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Successfully finished the setup."));
                            std::cout << newasm::header::col::reset;
                            if(!newasm::ctl::install::download_msg.empty())
                            {
                                std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Info: ") + newasm::header::col::reset + newasm::ctl::install::download_msg);
                                std::cout << newasm::header::col::reset;
                            }
                            if(std::filesystem::exists(output_path)) std::filesystem::remove(output_path);
                            return 1;
                        }
                        if(newasm::ctl::install::type == newasm::ctl::install::const_::NO) //if it doesnt exist online
                        {
                            std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Didn't find a such library or a setup!"));
                            std::cout << newasm::header::col::reset;
                            return 1;
                        }
                        return 1;
                    }
                }
                newasm::header::functions::err("Invalid command or command usage.");
                return 1;
            }
        }
        void main()
        {
            while(true)
            {
                if(newasm::ctl::data::finish)
                {
                    return;
                }
                std::string command;
                std::string username = newasm::user::global::username;
                std::cout << newasm::header::col::green << "shell@" << 
                newasm::header::col::yellow<<username<<newasm::header::col::green
                <<"-$"
                << newasm::header::col::reset;
                
                std::getline(std::cin, command);

                newasm::ctl::impl::process_c(command);
            }

            return;
        }
    }
}