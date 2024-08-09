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

            const std::string scripts_folder = "nax_scripts/";
            const std::string output_folder = "nax_output/";
        }
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
        namespace settings
        {
            int debug = 1;
            std::string script_file = "index.nax";
        }
        namespace version
        {
            const int major = 0;
            const int minor = 0;
            const int patch = 3;
        }
        namespace functions
        {
            void nullprint(std::string text)
            {
                std::cout << text << std::endl;
            }
            void err(std::string text)
            {
                std::cout << newasm::header::col::red << "[newasm] PROGRAM THREAD @ System error: " << newasm::header::col::reset << text << std::endl;
            }
            void wrn(std::string text)
            {
                std::cout << newasm::header::col::yellow << "[newasm] PROGRAM THREAD @ System warning: " << newasm::header::col::reset << text << std::endl;
            }
            void info(std::string text)
            {
                if(newasm::header::settings::debug == 1)
                {
                    std::cout << newasm::header::col::cyan << "[newasm] PROGRAM THREAD @ System info: " << newasm::header::col::gray << text << newasm::header::col::reset << std::endl;
                }
            }
            /*void init()
            {
                newasm::header::functions::info("Init finished.");
            }*/
            bool check_args(std::string forarg, int argc, char *argv[], int &argid)
            {
                if(argc > 1) for(int i = 1; i < argc; i++)
                {
                    if(forarg == static_cast<std::string>(argv[i]))
                    {
                        argid = i;
                        //newasm::header::functions::info("Argument found.");
                        return true;
                    }
                }
                return false;
            }
            void vers_info()
            {
                std::cout << newasm::header::col::green;
                std::cout << "\tNew-Assembly eXecutor\t\t" << newasm::header::constants::nullstr;
                std::cout << newasm::header::col::reset;
                std::cout << newasm::header::version::major<<"."<<newasm::header::version::minor<<"."<<newasm::header::version::patch;
                std::cout << newasm::header::constants::nullstr;
                std::cout << newasm::header::col::green;
                std::cout << "\n\t(c) 2024 Brace Software Co.," << newasm::header::col::reset << " by DEntisT" << std::endl << std::endl;
                //std::cout << newasm::header::col::reset;
            }
            void help_info()
            {
                newasm::header::functions::nullprint("[newasm] Application arguments:\n");
                newasm::header::functions::nullprint(
                    newasm::header::col::reset + "\t-help" + 
                    newasm::header::col::gray + "\t\t\tDisplays this panel."
                );
                newasm::header::functions::nullprint(
                    newasm::header::col::reset + "\t-ver" + 
                    newasm::header::col::gray + "\t\t\tDisplays version information."
                );
                newasm::header::functions::nullprint(
                    newasm::header::col::reset + "\t-input <file>" + 
                    newasm::header::col::gray + "\t\tSets the input file."
                );
                std::cout << newasm::header::col::reset;
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
                return !str.empty() && std::all_of
                (
                    str.begin(), str.end(), [](unsigned char c) 
                    {
                        return std::isalnum(c) || c == '_';
                    }
                );
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
            bool isfloat(const std::string& str)
            {
                try
                {
                    size_t pos;
                    std::stof(str, &pos);
                    return pos == str.length();
                } 
                catch(std::exception&)
                {
                    return false;
                }
            }
            bool istext(const std::string& str)
            {
                int quocount = 0;
                for(int i = 0; i < str.size(); i++)
                {
                    if(str.at(i) == '\"')
                    {
                        quocount ++;
                    }
                }
                return !str.empty() && str.front() == '"' && str.back() == '"' && quocount == 2;
            }
            std::string remq(const std::string& str)
            {
                if(str.length() >= 2 && str.front() == '"' && str.back() == '"')
                {
                    return str.substr(1, str.length() - 2);
                }
                return str;
            }
        }
    }
}