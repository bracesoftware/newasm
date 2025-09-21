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
    namespace env_control
    {
        int addenv()
        {
            std::string key, value;
            std::cout << newasm::header::col::gray << "\tInput environment variable name: " << newasm::header::col::reset;
            std::getline(std::cin, key);
            key = newasm::header::functions::trim(key);
            if(!newasm::header::functions::isalphanum(key))
            {
                newasm::header::functions::err("Environment variable name must consist of only alphanumeric characters.");
                return 1;
            }
            if(newasm::core::env_vars::functions::env_exists(key))
            {
                newasm::header::functions::err("Environment variable already exists.");
                return 1;
            }
            std::cout << newasm::header::col::gray << "\tInput environment variable value: " << newasm::header::col::reset;
            std::getline(std::cin, value);
            value = newasm::header::functions::trim(value);
            
            newasm::core::env_vars::functions::add_env(key, value);
            return 1;
        }
        int remenv()
        {
            std::string key;
            std::cout << newasm::header::col::gray << "\tInput environment variable name: " << newasm::header::col::reset;
            std::getline(std::cin, key);
            key = newasm::header::functions::trim(key);
            
            if(!newasm::core::env_vars::functions::env_exists(key))
            {
                newasm::header::functions::err("Environment variable doesn't exist.");
                return 1;
            }

            bool result = newasm::shell_tools::prompt("Are you sure you want to delete the variable?");
            if(!result) return 1;

            newasm::core::env_vars::functions::rem_env(key);
            return 1;
        }
        int modenv()
        {
            std::string key, value;
            std::cout << newasm::header::col::gray << "\tInput environment variable name: " << newasm::header::col::reset;
            std::getline(std::cin, key);
            key = newasm::header::functions::trim(key);
            if(!newasm::core::env_vars::functions::env_exists(key))
            {
                newasm::header::functions::err("Environment variable doesn't exist.");
                return 1;
            }
            std::cout << newasm::header::col::gray << "\tInput new environment variable value: " << newasm::header::col::reset;
            std::getline(std::cin, value);
            value = newasm::header::functions::trim(value);

            bool result = newasm::shell_tools::prompt("Are you sure you want to modify the variable?");
            if(!result) return 1;
            
            newasm::core::env_vars::functions::mod_env(key, value);
            return 1;
        }
        int renenv()
        {
            std::string oldname, newname;
            std::cout << newasm::header::col::gray << "\tInput environment variable name: " << newasm::header::col::reset;
            std::getline(std::cin, oldname);
            oldname = newasm::header::functions::trim(oldname);
            if(!newasm::core::env_vars::functions::env_exists(oldname))
            {
                newasm::header::functions::err("Environment variable doesn't exist.");
                return 1;
            }
            std::cout << newasm::header::col::gray << "\tInput new environment variable name: " << newasm::header::col::reset;
            std::getline(std::cin, newname);
            newname = newasm::header::functions::trim(newname);

            bool result = newasm::shell_tools::prompt("Are you sure you want to rename the variable?");
            if(!result) return 1;
            
            newasm::core::env_vars::functions::rename_env(oldname, newname);
            return 1;
        }

        inline int printenv()
        {
            std::cout << std::endl;
            auto& vec = newasm::core::env_vars::priv_env_var;

            if(vec.size() == 0)
            {
                newasm::header::functions::info("No environment variables found.");
                std::cout << std::endl;
                return 1;
            }

            const int row_el = 2;
            const int col1_width = 20;
            const int col2_width = 30;

            for(size_t i = 0; i < vec.size(); i += row_el)
            {
                for(size_t j = 0; j < row_el && (i + j) < vec.size(); ++j)
                {
                    auto& p = vec[i + j];
                    std::cout << newasm::header::col::kernel
                        << std::setw(col1_width) << std::left << p.first
                        << newasm::header::col::gray
                        << std::setw(col2_width) << std::left << p.second
                        << newasm::header::col::reset;
                }
                std::cout << std::endl;
            }

            std::cout << std::endl;

            return 1;
        }

        inline int printenv_()
        {
            std::cout << std::endl;
            auto& vec = newasm::core::env_vars::priv_env_var;

            int remaining = vec.size();
            int idx = 0;
            const int row_el = 3;

            const int column_size = 15;

            for(int i = 0; i < vec.size(); i += row_el)
            {
                if(remaining < 5)
                {
                    for(int j = 0; j < remaining; ++j)
                    {
                        idx = i + j;
                        std::cout << newasm::header::col::red << vec[idx].first << ": " << newasm::header::col::gray << vec[idx].second;

                        std::cout << "\t\t";
                        
                        std::cout << newasm::header::col::reset;
                    }
                    std::cout << std::endl;
                    break;
                }
                remaining -= row_el;
                for(int j = 0; j < row_el; ++j)
                {
                    idx = i + j;
                    std::cout << newasm::header::col::red << vec[idx].first << ": " << newasm::header::col::gray << vec[idx].second;

                    std::cout << "\t\t";

                    std::cout << newasm::header::col::reset;
                }
                std::cout << std::endl;
            }
            std::cout << newasm::header::col::reset;
            std::cout << std::endl;
            return 1;
        }
    }
}