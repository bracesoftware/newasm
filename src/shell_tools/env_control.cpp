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
            
            newasm::core::env_vars::functions::mod_env(key, value);
            return 1;
        }
    }
}