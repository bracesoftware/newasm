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

Portions created by the Initial Developer are Copyright (c) 2022
the Initial Developer. All Rights Reserved.

*/

namespace newasm
{
    namespace header
    {
        namespace constants
        {
            int nullint = 0;
            bool nullbool = false;
            std::string nullstr = " ";
            char nullchar = '0';
            float nullfloat = 0.0;

            std::string scripts_folder = "nax_scripts/";
        }
        namespace settings
        {
            int debug = 1;
        }
        namespace version
        {
            const int major = 0;
            const int minor = 0;
            const int patch = 1;
        }
        namespace functions
        {
            void err(std::string text)
            {
                std::cout << "[newasm] PROGRAM THREAD @ System error: " << text << std::endl;
            }
            void wrn(std::string text)
            {
                std::cout << "[newasm] PROGRAM THREAD @ System warning: " << text << std::endl;
            }
            void info(std::string text)
            {
                if(newasm::header::settings::debug == 1)
                {
                    std::cout << "[newasm] PROGRAM THREAD @ System info: " << text << std::endl;
                }
            }
            /*void init()
            {
                newasm::header::functions::info("Init finished.");
            }*/
            bool check_args(std::string forarg, int argc, char *argv[])
            {
                if(argc > 1) for(int i = 1; i < argc; i++)
                {
                    if(forarg == static_cast<std::string>(argv[i]))
                    {
                        newasm::header::functions::info("Argument found.");
                        return true;
                    }
                }
                return false;
            }
            void vers_info()
            {
                std::cout << "New-Assembly eXecutor" << newasm::header::constants::nullstr;
                std::cout << newasm::header::version::major<<"."<<newasm::header::version::minor<<"."<<newasm::header::version::patch;
                std::cout << newasm::header::constants::nullstr;
                std::cout << "(c) 2024 Brace Software Co., by DEntisT" << std::endl << std::endl;
            }
            std::vector<std::string> split(const std::string &str, char delimiter)
            {
                std::vector<std::string> tokens;
                std::string token;
                std::stringstream ss(str);

                while (std::getline(ss, token, delimiter))
                {
                    tokens.push_back(token);
                }

                return tokens;
            }
            std::string trim(const std::string &str)
            {
                auto start = str.begin();
                auto end = str.end();

                while(start != end && std::isspace(static_cast<unsigned char>(*start)))
                {
                    ++start;
                }

                if(start == end)
                {
                    return static_cast<std::string>("");
                }
                auto it = end;
                do
                {
                    --it;
                }
                while(it != start && std::isspace(static_cast<unsigned char>(*it)));

                return std::string(start, it + 1);
            }
            bool isnumeric(const std::string &str)
            {
                return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
            }
            bool strfind(const std::string& str, char c)
            {
                return str.find(c) != std::string::npos;
            }
            bool isalphanum(const std::string& str)
            {
                return !str.empty() && std::all_of(str.begin(), str.end(), ::isalnum);
            }
            void parseopr(std::string &opr,std::unordered_map<std::string, std::string> &data)
            {
                for(auto it = data.begin(); it != data.end(); it++)
                {
                    if(opr == it->first)
                    {
                        opr.clear();
                        opr = it->second;
                        break;
                    }
                }
            }
        }
    }
}