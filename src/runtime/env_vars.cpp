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
    namespace core
    {
        namespace env_vars
        {
            std::vector<std::pair<std::string,std::string>> priv_env_var; 
            namespace functions
            {
                int setup_env()
                {
                    /*std::fstream launch_file(newasm::core::constants::data_folder + newasm::core::constants::separator + "launch.ini",std::ios::app|std::ios::out|std::ios::in);
                     launch_file << "no";
                      launch_file.close();*/
                    std::string env_vars_file = newasm::core::constants::data_folder + newasm::core::constants::separator + "env_vars.ini";
                    std::ifstream internal_fileobject(env_vars_file);
                    if(internal_fileobject.is_open())
                    {
                        std::string line,key,value;
                        std::vector<std::string> tokens;

                        while(std::getline(internal_fileobject, line))
                        {
                            if(line.empty())
                            {
                                continue;
                            }
                            if(!newasm::header::functions::strfind(line,'='))
                            {
                                continue;
                            }
                            tokens = newasm::header::functions::split_fixed(line, '=');
                            key = tokens[0];
                            key = newasm::header::functions::trim(key);
                            value = tokens[1];
                            value = newasm::header::functions::trim(value);

                            for(auto i = newasm::core::env_vars::priv_env_var.begin(); i < newasm::core::env_vars::priv_env_var.end(); ++i)
                            {
                                if(i->first == key)
                                {
                                    continue;
                                }
                            }
                            
                            newasm::core::env_vars::priv_env_var.push_back({key,value});
                        }
                        newasm::header::functions::info("Successfully analyzed the environment variables.");
                        internal_fileobject.close();
                        return 1;
                    }
                    else
                    {
                        newasm::header::functions::wrn("Cannot find the environment variable file.");
                        std::ofstream internal_fileobject(env_vars_file, std::ios::app);

                        if(internal_fileobject.is_open())
                        {
                            internal_fileobject << "\n";
                            internal_fileobject.close();
                        }
                    }
                    return 1;
                }
            }
        }
    }
}