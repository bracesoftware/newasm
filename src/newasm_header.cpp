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

#ifndef __cplusplus
    #error WHAT
#endif

namespace newasm
{
    namespace header
    {
        namespace system_info
        {
            const std::string name = "[NewASM]";
            const std::string copyright_claim = "(c) 2024 Brace Software Co.";
            const std::string fullname = "NewASM Runtime Environment";
        }
        namespace version
        {
            namespace release_types
            {
                const int unstable = 1;
                const int stable = 2;
                const int test = 3;
            }
            const int major = 0;
            const int minor = 1;
            const int patch = 1;
            const int release = 2;
            const int release_type = newasm::header::version::release_types::test;
        }
        namespace data
        {
            std::string lastln = "";
            int lastlndx = 0;
            bool exception = true;
            bool repl = false;
            bool repl_end = false;
            bool proc_now = false;
            bool struct_now = false;
            std::string struct_decl = "";
        }
        namespace constants
        {
            int nullint = 0;
            bool nullbool = false;
            std::string nullstr = " ";
            std::string nullstring = "null";
            char nullchar = '0';
            float nullfloat = 0.0;

            const int max_tokens = 2;

            //const std::string scripts_folder = "nax_scripts/";
            //const std::string output_folder = "nax_output/";
            const std::string default_input = "index.asm";

            const std::string endl = "\n";

            const std::string inv_reg_val = "[newasm] __ptr -> Segmentation fault.";
            const int inv_ireg_val = -999999;
            const float inv_freg_val = static_cast<float>(-999999);
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
        namespace style
        {
            const std::string underline = "\033[4m";
            const std::string bold = "\033[1m";
        }
        namespace settings
        {
            int debug = 1;
            std::string script_file = "index.nax";
            bool extra = false;
            bool create_new_projfile = false;
        }
        namespace functions
        {
            void getreleasetype(std::string &dest)
            {
                if(newasm::header::version::release_type == newasm::header::version::release_types::unstable)
                {
                    dest = "unstable";
                }
                if(newasm::header::version::release_type == newasm::header::version::release_types::stable)
                {
                    dest = "stable";
                }
                if(newasm::header::version::release_type == newasm::header::version::release_types::test)
                {
                    dest = "test";
                }
            }
            void getversion(std::string &dest)
            {
                std::string release__type;
                newasm::header::functions::getreleasetype(release__type);
                dest.clear();
                dest =  std::to_string(newasm::header::version::major) + static_cast<std::string>(".") +
                        std::to_string(newasm::header::version::minor) + static_cast<std::string>(".") +
                        std::to_string(newasm::header::version::patch) + static_cast<std::string>("-R") +
                        std::to_string(newasm::header::version::release) + static_cast<std::string>("-") + 
                        release__type;
            }
            void getos(std::string &dest)
            {
                #ifdef _WIN32
                    dest = "windows";
                #elif defined(__linux__)
                    dest = "linux";
                #endif
            }
            void getarch(std::string &dest)
            {
                #if defined(__x86_64__) || defined(__amd64__)
                    dest = "x86-64bit";//std::cout << "64-bit x86 architecture" << std::endl;
                #elif defined(__i386__)
                    dest = "x86-32bit";//std::cout << "32-bit x86 architecture" << std::endl;
                #elif defined(__aarch64__)
                    dest = "ARM-64bit";//std::cout << "64-bit ARM architecture" << std::endl;
                #elif defined(__arm__)
                    dest = "ARM-32bit";//std::cout << "32-bit ARM architecture" << std::endl;
                #else
                    dest = "u-Arch";//std::cout << "Unknown architecture" << std::endl;
                #endif
            }
            void nullprint(std::string text)
            {
                std::cout << text << std::endl;
            }
            int nullprint_wnm(std::string text)
            {
                std::cout << newasm::header::system_info::name << " " << text << std::endl;
                return 0;
            }
            void err(std::string text)
            {
                std::cout << newasm::header::col::red << newasm::header::system_info::name << " PROGRAM THREAD @ System error | " << newasm::header::col::reset << text << std::endl;
            }
            void wrn(std::string text)
            {
                std::cout << newasm::header::col::yellow << newasm::header::system_info::name << "  PROGRAM THREAD @ System warning | " << newasm::header::col::reset << text << std::endl;
            }
            void info(std::string text)
            {
                if(newasm::header::settings::debug == 1)
                {
                    std::cout << newasm::header::col::cyan << newasm::header::system_info::name << "  PROGRAM THREAD @ System info | " << newasm::header::col::gray << text << newasm::header::col::reset << std::endl;
                }
            }
            /*void init()
            {
                newasm::header::functions::info("Init finished.");
            }*/
            //std::string trim(const std::string &str);
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
                std::string arch;
                std::string os;
                std::string version;

                newasm::header::functions::getarch(arch);
                newasm::header::functions::getos(os);
                newasm::header::functions::getversion(version);

                std::cout << newasm::header::col::yellow << newasm::header::style::bold << newasm::header::style::underline;
                std::cout << "\n" << newasm::header::system_info::fullname;
                std::cout << newasm::header::col::reset << newasm::header::col::gray;
                std::cout << "\n  Build: " << version << "-" << os << "_" << arch;
                std::cout << "\n  Compiled with: C++" << __cplusplus;
                
                std::cout << "\n  " << newasm::header::system_info::copyright_claim <<
                newasm::header::col::reset << std::endl;
                
            }
            void help_info()
            {
                newasm::header::functions::nullprint_wnm("Application arguments:\n");
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
                std::cout << "\n";
            }
            std::vector<std::string> split(const std::string &str, char delimiter)
            {
                std::vector<std::string> tokens;
                std::string token;
                std::stringstream ss(str);

                while(std::getline(ss, token, delimiter))
                {
                    tokens.push_back(token);
                }

                return tokens;
            }
            std::vector<std::string> split_fixed(const std::string &str, char delimiter)
            {
                size_t pos = str.find(delimiter);
                std::vector<std::string> tokens;

                if(pos != std::string::npos)
                {
                    tokens.push_back(str.substr(0, pos));
                    tokens.push_back(str.substr(pos + 1));
                }
                else
                {
                    tokens.push_back(str);
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
                std::string copy;
                for(char i : str)
                {
                    if(!std::isspace(i))
                    {
                        copy = copy + i;
                    }
                }
                if (copy.empty()) return false;
                if (copy[0] != '-' && !std::isdigit(copy[0])) return false;
                return std::all_of(copy.begin() + 1, copy.end(), ::isdigit);
            }
            bool strfind(const std::string& str, const char c)
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
            bool isref(const std::string& str)
            {
                int amp_count = 0;
                for(int i = 0; i < str.size(); i++)
                {
                    if(str.at(i) == '&')
                    {
                        amp_count ++;
                    }
                }
                if(amp_count != 1)
                {
                    return false;
                }
                if(str.at(0) != '&')
                {
                    return false;
                }
                if(str.at(0) == '&' && !std::isalnum(str.at(1)) && str.at(1) != '_')
                {
                    return false;
                }
                return true;
            }
            bool isrefat(const std::string &data)
            {
                return (data.find("@") != std::string::npos);
            }
            std::string remq(const std::string& str)
            {
                if(str.length() >= 2 && str.front() == '"' && str.back() == '"')
                {
                    return str.substr(1, str.length() - 2);
                }
                return str;
            }
            std::string remc(const std::string &line)
            {
                std::size_t pos = line.find(';');
                if(pos != std::string::npos)
                {
                    return line.substr(0, pos);
                }
                return line;
            }
            std::string remamp(const std::string &line)
            {
                std::size_t pos = line.find('&');
                if(pos != std::string::npos)
                {
                    return line.substr(pos+1);
                }
                return line;
            }
            int strdist(const std::string &a, const std::string &b)
            {
                if(a == b)
                {
                    return 0;
                }
                int size_a = a.size();
                int size_b = b.size();
                int result = 0;

                for(int i = 0; i < size_a > size_b ? size_b : size_a; i++)
                {
                    if(a.at(i) != b.at(i))
                    {
                        result++;
                    }
                }

                result = result + size_a > size_b ? size_a - size_b : size_b - size_a;
                return result;
            }
            bool ishex(const std::string &str)
            {
                if (str.size() < 3) return false;
                if (str.at(0) != '0' || str.at(1) != 'x') return false;
                
                for (int i = 2; i < str.size(); i++)
                {
                    char c = str.at(i);
                    if(!(('0' <= c && c <= '9') || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F')))
                    {
                        return false;
                    }
                }
                return true;
            }
            bool isbin(const std::string &str)
            {
                if (str.size() < 3) return false;
                if (str.at(0) != '0' || str.at(1) != 'b') return false;
                
                for (int i = 2; i < str.size(); i++)
                {
                    char c = str.at(i);
                    if(c != '0' && c != '1')
                    {
                        return false;
                    }
                }
                return true;
            }
        }
    }

    namespace project_data
    {
        std::string name = "";
        std::string version = "";

        namespace impl
        {
            int parse_proj_settings(const std::string &key, const std::string &value)
            {
                if(key == static_cast<std::string>("name"))
                {
                    newasm::project_data::name = value;
                }
                if(key == static_cast<std::string>("version"))
                {
                    newasm::project_data::version = value;
                }
                return 1;
            }
            int setup_proj()
            {
                std::ifstream internal_fileobject(newasm::header::settings::script_file + static_cast<std::string>(".newasm_proj"));
                if(internal_fileobject.is_open())
                {
                    std::string line,key,value;
                    std::vector<std::string> tokens;

                    while(std::getline(internal_fileobject, line))
                    {
                        tokens = newasm::header::functions::split_fixed(line, '=');
                        key = tokens[0];
                        key = newasm::header::functions::trim(key);
                        value = tokens[1];
                        value = newasm::header::functions::trim(value);
                        newasm::project_data::impl::parse_proj_settings(key, value);
                    }
                    internal_fileobject.close();
                    return 1;
                }
                else
                {
                    newasm::header::functions::wrn("Cannot find the project file.");
                    newasm::project_data::name = "Unnamed project";
                    newasm::project_data::version = "0.0.1";
                    if(newasm::header::settings::create_new_projfile)
                    {
                        std::ofstream internal_fileobject(newasm::header::settings::script_file + static_cast<std::string>(".newasm_proj"), std::ios::app);

                        if(internal_fileobject.is_open())
                        {
                            internal_fileobject << "name = Unnamed project" << "\n" << "version = 0.0.1" << "\n";
                            internal_fileobject.close();
                        }
                    }
                }
                return 1;
            }
        } 
    }
}