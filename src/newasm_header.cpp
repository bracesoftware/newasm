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
        namespace flags
        {
            bool compexpr = false;
            bool autobos = false;
        }
        namespace system_info
        {
            const std::string name = "[NewASM]";
            const std::string copyright_claim = "(c) 2025 Brace Software Co.";
            const std::string fullname = "NewASM Virtual Machine";
        }
        namespace version
        {
            namespace release_types
            {
                const int unstable = 1; //has many major bugs
                const int stable = 2; //has no known bugs
                const int test = 3; //not tested enough
                const int beta = 4; //has some bugs
            }
            const int major = 2;
            const int minor = 1;
            const int patch = 0;
            const int release = 1;
            const int release_type = newasm::header::version::release_types::beta;
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

            int argc = 0;
            int callstkidx = 0;

            int module = 0;

            std::string temp = "temp";
            //macros
            bool macro_now = false;
            std::string macro_decl = "";
            //switch-case
            std::string switched_value;
            bool case_matched = false;
            std::string case_line;
            //tuples
            int tupleIndex = -1; 
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
            const std::string log_file = newasm::core::constants::data_folder + newasm::core::constants::separator + "log.txt";

            const std::string endl = "\n";

            const std::string inv_reg_val = "[newasm] __ptr -> Segmentation fault.";
            const int inv_ireg_val = -999999;
            const float inv_freg_val = static_cast<float>(-999999);
        }
        namespace settings
        {
            int debug = 1;
            std::string script_file = newasm::header::constants::default_input;
            bool extra = false;
            bool create_new_projfile = false;
            bool logging = false;
            bool lazy_evhndlr = false;
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
                if(newasm::header::version::release_type == newasm::header::version::release_types::beta)
                {
                    dest = "beta";
                }
            }
            void getversion(std::string &dest)
            {
                std::string release__type;
                std::string date = __DATE__;
                std::string time = __TIME__;
                newasm::header::functions::getreleasetype(release__type);
                dest.clear();
                dest =  static_cast<std::string>("b") +
                        std::to_string(newasm::BUILD_NUMBER)+static_cast<std::string>(".") +
                        std::to_string(newasm::user::udb_hash(date)) +static_cast<std::string>(".") +
                        std::to_string(newasm::user::udb_hash(time))  +
                        static_cast<std::string>("-") + release__type;
            }
            void getos(std::string &dest)
            {
                #if _NEWASM_OS == _NEWASM_OS_windows
                    dest = "windows";
                #elif _NEWASM_OS == _NEWASM_OS_windows_old
                    dest = "windows_old";
                #elif _NEWASM_OS == _NEWASM_OS_linux
                    dest = "linux";
                #elif _NEWASM_OS == _NEWASM_OS_android
                    dest = "android";
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
                std::cout << newasm::header::col::blue << newasm::header::system_info::name << "  PROGRAM THREAD @ System info | " << newasm::header::col::gray << text << newasm::header::col::reset << std::endl;
            }
            void dbg(std::string text)
            {
                if(newasm::header::settings::debug == 1)
                    std::cout << newasm::header::col::gray << newasm::header::system_info::name << "   PROGRAM THREAD @ Debug | " << newasm::header::col::reset << text << std::endl;
            }
            
            /*void init()
            {
                newasm::header::functions::info("Init finished.");
            }*/
            //std::string trim(const std::string &str);
            bool check_args(std::string forarg, int argc, char *argv[], int &argid)
            {
                if(argc > 1) for(int i = 1; i < argc; ++i)
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
                newasm::utils::glitch_text(newasm::header::system_info::fullname);
                std::cout << newasm::header::col::reset << newasm::header::col::gray;
                std::cout << "\n  Build: " << version << "-" << os << "_" << arch;
                std::cout << "\n  Compiled with: C++" << __cplusplus << " on " << __DATE__ << " at " << __TIME__;
                
                std::cout << "\n\n  Runtime version: v" << newasm::runtime::version;
                
                std::cout << "\n  " << newasm::header::system_info::copyright_claim <<
                newasm::header::col::reset << std::endl;
                
            }
            const int first_column = 20;
            const int second_column = 20;
            const int third_column = 20;
            const std::string tabs = "\t";
            const std::vector<std::pair<std::string, std::pair<std::string,std::string>>> help_table_data = {
                {"-help",      {"/",           "Displays the help panel."}},
                {"-ver",       {"/",           "Displays the version information."}},
                {"-input",     {"<file>",      "Sets the input file."}},
                {"-repl",      {"/",           "Enter the read-evaluate-print mode."}},
                {"-newproj",   {"/",           "Create a new project file if one doesn't already exist."}},
                {"-tests",     {"/",           "Loads the test function."}},
                {"-log",       {"/",           "Enables the logging system."}},
                {"-mode",      {"<mode ID>",   "Changes the app launch mode."}},
                {"-nover",     {"/",           "Disables the version check feature."}}
            };
            void help_info()
            {
                std::cout << "\n" << newasm::header::col::reset;
                std::cout
                        << std::setw(first_column) << std::left<<newasm::header::style::underline + newasm::header::style::bold+tabs+"Argument"
                        << std::setw(second_column) << std::left<<"Parameters" 
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
            
       
            
            std::string trim(const std::string &str);
            bool isbetween(const std::string& str, char delim, char what)
            {
                int pos1 = -1,pos2 = -1,delpos = -1;
                for(int i = 0; i < str.size(); ++i)
                {
                    if(str.at(i) == delim)
                    {
                        if(delpos == -1)
                        {
                            delpos = i;
                            //std::cout << "found delpos " << delpos << std::endl;
                            continue;
                        }
                    }
                    if(str.at(i) == what)
                    {
                        if(pos1 == -1)
                        {
                            pos1 = i;
                            //std::cout << "found pos1 " << pos1 << std::endl;
                            continue;
                        }
                        if(pos2 == -1)
                        {
                            pos2 = i;
                            //std::cout << "found pos2 " << pos2 << std::endl;
                            continue;
                        }
                    }
                }
                if((delpos != -1) && (pos1 != -1) && (pos2 != -1))
                {
                    //std::cout << "pos1 " << pos1<< "delpos " << delpos << "pos2" << pos2 << std::endl;
                    if(pos1 < delpos && delpos < pos2)
                    {
                        return true;
                    }
                }
                return false;
            }
            std::vector<std::string> tokenize2(const std::string& input)
            {
                std::vector<std::string> tokens;

                size_t i = 0;
                while(i < input.size() && !std::isspace(static_cast<unsigned char>(input[i])))
                {
                    ++i;
                }

                if(i == input.size())
                {
                    // Nema razmaka (nema whitespace)
                    tokens.push_back(input);
                }
                else
                {
                    // Razdvoji u dva dijela
                    std::string first = input.substr(0, i);

                    // preskoči sve whitespace karaktere nakon prvog
                    size_t j = i;
                    while(j < input.size() && std::isspace(static_cast<unsigned char>(input[j])))
                    {
                        j++;
                    }

                    std::string second = input.substr(j);
                    tokens.push_back(first);
                    tokens.push_back(second);
                }

                return tokens;
            }
            int wait(int milliseconds)
            {
                if (milliseconds < 0) return -1; // negativno vrijeme nema smisla
                std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
                return 0; // sve ok
            }
            const std::vector<std::string> lambda_ = {
                "proc", "end"
            };
            std::pair<bool, std::string> is_lambda(std::string text)
            {
                std::string newstr = newasm::header::functions::trim(text);

                if(newstr.size() >= 3 && newstr.front() == '(' && newstr.back() == ')')
                {
                    auto lambda_proc = (newstr.substr(1, newstr.size() - 2));
                    for(const auto& keyword : lambda_)
                    {
                        if(keyword == lambda_proc)
                        {
                            return {true, lambda_proc};
                        }
                    }
                    return {false, "err"};
                }

                return {false, "err"};
            }

            std::string form_iso(std::string ins, std::string suf, std::string opr)
            {
                std::stringstream result;
                result << ins << " " << suf << "," << opr;
                return result.str();
            }

            std::vector<std::string> split(const std::string &str, char delimiter)
            {
                std::vector<std::string> tokens;
                std::string token;
                std::stringstream ss(str);

                while(std::getline(ss, token, delimiter))
                {
                    if(newasm::header::functions::trim(token).empty()) continue;
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
                for(auto it = data.begin(); it != data.end(); ++it)
                {
                    if(opr == it->first)
                    {
                        opr.clear();
                        opr = it->second;
                        break;
                    }
                }
            }
            static bool isfloat_(const std::string& str)
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

            bool isfloat(const std::string& s)
            {
                int dotCount = 0;

                if (s.empty()) return false;

                for (size_t i = 0; i < s.size(); ++i)
                {
                    char c = s[i];
                    if(std::isdigit(c))
                    {
                        continue;
                    }
                    else if(c == '.')
                    {
                        dotCount++;
                        if (dotCount > 1) return false;
                        if (i == 0 || i == s.size() - 1) return false;
                    } 
                    else 
                    {
                        return false; // nevažeći karakter
                    }
                }

                return dotCount == 1; // mora biti točno jedna točka
            }

            bool istext(const std::string& str)
            {
                int quocount = 0;
                for(int i = 0; i < str.size(); ++i)
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
                for(int i = 0; i < str.size(); ++i)
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
                /*if(str.at(0) == '&' && !std::isalnum(str.at(1)) && str.at(1) != '_')
                {
                    return false;
                }*/
                return true;
            }
            bool ischar(const std::string &str)
            {
                int singlequo_count = 0;
                for(int i = 0; i < str.size(); ++i)
                {
                    if(str.at(i) == '\'')
                    {
                        singlequo_count ++;
                    }
                }
                return !str.empty() && str.front() == '\'' && str.back() == '\'' && singlequo_count == 2 && str.size() == 3;
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
            std::string remsq(const std::string& str)
            {
                if(str.length() >= 2 && str.front() == '\'' && str.back() == '\'')
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
                    return newasm::header::functions::trim(line.substr(pos+1));
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

                for(int i = 0; i < size_a > size_b ? size_b : size_a; ++i)
                {
                    if(a.at(i) != b.at(i))
                    {
                        result++;
                    }
                }

                result = result + size_a > size_b ? size_a - size_b : size_b - size_a;
                return result;
            }
            int hex_char_to_int(char c)
            {
                if (c >= '0' && c <= '9')
                    return c - '0';
                else if (c >= 'a' && c <= 'f')
                    return 10 + (c - 'a');
                else if (c >= 'A' && c <= 'F')
                    return 10 + (c - 'A');
                else
                    return 0;
            }

            int hextoi(const std::string& hex_str)
            {
                size_t start = 0;
                int result = 0;

                // Ako počinje s "0x" ili "0X", preskoči prefiks
                if(hex_str.size() >= 2 && hex_str[0] == '0' && (hex_str[1] == 'x' || hex_str[1] == 'X'))
                {
                    start = 2;
                }

                for(size_t i = start; i < hex_str.length(); ++i)
                {
                    char c = hex_str[i];
                    if(!std::isxdigit(c))
                    {
                        return -1;
                    }
                    result = result * 16 + hex_char_to_int(c);
                }

                return result;
            }
            bool ishex(const std::string &str)
            {
                if (str.size() < 3) return false;
                if (str.at(0) != '0' || str.at(1) != 'x') return false;
                
                for (int i = 2; i < str.size(); ++i)
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
                
                for (int i = 2; i < str.size(); ++i)
                {
                    char c = str.at(i);
                    if(c != '0' && c != '1')
                    {
                        return false;
                    }
                }
                return true;
            }
            bool hasnull(std::string str, char &delim, int &pos)
            {
                for(int i = 0; i < str.size(); ++i)
                {
                    if(
                        (str.at(i) == '\t') ||
                        (str.at(i) == 32)
                    )
                    {
                        pos = i;
                        delim = str.at(i);
                        return true;
                    }
                }
                return false;
            }
            int log(std::string text)
            {
                if(newasm::header::settings::logging == false)
                {
                    return 1;
                }

                #ifdef _NEWASM_BOOM
                    #ifdef _WIN32
                        const char separator = '\\';
                        std::string defaultDir = "C:\\Users\\Default\\Documents";
                    #else
                        const char separator = '/';
                        std::string defaultDir = "/home/user/documents";
                    #endif

                    std::filesystem::path fullPath = std::filesystem::path(defaultDir+separator+"NewASM");
                    if(!std::filesystem::exists(fullPath))
                    {
                        std::filesystem::create_directories(fullPath);
                    }

                    std::ofstream global_log(defaultDir+separator+"NewASM"+separator+newasm::header::constants::log_file,std::ios::app|std::ios::out);
                    global_log << "_";
                    global_log.close();
                #endif

                std::ofstream log_file_object(newasm::header::constants::log_file, std::ios::app | std::ios::out);
                auto now = std::chrono::system_clock::now(); // gets time
                std::time_t now_time = std::chrono::system_clock::to_time_t(now);
                if(log_file_object.is_open())
                {
                    log_file_object << std::put_time(std::localtime(&now_time), "[%d/%m/%Y - %H:%M:%S] ")
                    << newasm::header::system_info::name << " Log | " << text << "\n";
                    log_file_object.close();
                    return 1;
                }
                newasm::header::functions::err("Unable to open the log file.");
                //newasm::header::functions::info("IDIOT");
                return 1;
            }
            std::pair<bool, int> isallocref(std::string text)
            {
                std::string newstr = newasm::header::functions::trim(text);

                if(newstr.size() >= 3 && newstr.front() == '[' && newstr.back() == ']')
                {
                    std::string number_part = newasm::header::functions::trim(newstr.substr(1, newstr.size() - 2));
                    if(!newasm::header::functions::isnumeric(number_part))
                    {
                        newasm::runtime::functions::parse(number_part);
                    }
                    if(newasm::header::functions::isnumeric(number_part))
                    {
                        return {true, std::stoi(number_part)};
                    }
                }
                return {false, 0};
            }

            std::pair<bool, int> isargref(std::string text)
            {
                std::string newstr = newasm::header::functions::trim(text);

                if(newstr.size() >= 3 && newstr.front() == '<' && newstr.back() == '>')
                {
                    std::string number_part = newstr.substr(1, newstr.size() - 2);
                    if(newasm::header::functions::isnumeric(number_part))
                    {
                        return {true, std::stoi(number_part)};
                    }
                }

                return {false, 0};
            }
            std::pair<bool, int> isvmemsize(std::string& text)
            {
                std::string newstr = newasm::header::functions::trim(text);

                if(newstr.size() >= 2 && newstr.back() == '_')
                {
                    std::string number_part = newasm::header::functions::trim(newstr.substr(0, newstr.size() - 1));
                    if(!newasm::header::functions::isnumeric(number_part))
                    {
                        newasm::runtime::functions::parse(number_part);
                    }
                    if(newasm::header::functions::isnumeric(number_part))
                    {
                        return {true, std::stoi(number_part)};
                    }
                }
                return {false, 0};
            }
            std::pair<bool, int> isvmemref(std::string text)
            {
                std::string newstr = newasm::header::functions::trim(text);

                if(newstr.size() >= 3 && newstr.front() == '{' && newstr.back() == '}')
                {
                    std::string number_part = newasm::header::functions::trim(newstr.substr(1, newstr.size() - 2));
                    if(!newasm::header::functions::isnumeric(number_part))
                    {
                        newasm::runtime::functions::parse(number_part);
                    }
                    if(newasm::header::functions::isnumeric(number_part))
                    {
                        return {true, std::stoi(number_part)};
                    }
                }
                return {false, 0};
            }
            std::pair<bool, std::pair<int, int>> isrange(const std::string& input)
            {
                size_t dots = input.find("..");
                if (dots == std::string::npos) return {false, {0, 0}};

                std::string lhs = input.substr(0, dots);
                std::string rhs = input.substr(dots + 2);

                lhs = newasm::header::functions::trim(lhs);
                rhs = newasm::header::functions::trim(rhs);

                if(!newasm::header::functions::isnumeric(lhs))
                {
                    newasm::runtime::functions::parse(lhs);
                }
                if(!newasm::header::functions::isnumeric(rhs))
                {
                    newasm::runtime::functions::parse(rhs);
                }

                if(!newasm::header::functions::isnumeric(lhs))
                {
                    //newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return {false, {0, 0}};
                }
                if(!newasm::header::functions::isnumeric(rhs))
                {
                    //newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return {false, {0, 0}};
                }

                int start = std::stoi(lhs);
                int end = std::stoi(rhs);
                return {true, {start, end}};
            }

            bool case_typename(std::string& switched_val, std::string& suf)
            {
                if(newasm::header::functions::isnumeric(switched_val))
                if(suf == newasm::core::lang_inf::typenames::identifiers__.at(
                        newasm::core::lang_inf::typenames::num
                )) return true;

                if(newasm::header::functions::isfloat(switched_val))
                if(suf == newasm::core::lang_inf::typenames::identifiers__.at(
                        newasm::core::lang_inf::typenames::decm
                )) return true;

                if(newasm::header::functions::istext(switched_val))
                if(suf == newasm::core::lang_inf::typenames::identifiers__.at(
                        newasm::core::lang_inf::typenames::txt
                )) return true;

                if(newasm::header::functions::ischar(switched_val))
                if(suf == newasm::core::lang_inf::typenames::identifiers__.at(
                        newasm::core::lang_inf::typenames::char__
                )) return true;

                if(newasm::header::functions::isref(switched_val))
                if(suf == newasm::core::lang_inf::typenames::identifiers__.at(
                        newasm::core::lang_inf::typenames::ref
                )) return true;
                return false;
            }

            std::pair<bool, int> issizeof(const std::string& str)
            {
                std::string opr;
                //std::cout << "\t" << "STR IN ISSIZEOF : `" << str << "`\n";
                if(str.size() > 2 && str[0] == '$' && str[1] == '-')
                {
                    opr = newasm::header::functions::trim(str.substr(2));
                    newasm::mem::functions::parse(opr);
                    opr = newasm::header::functions::remamp(opr);
                    
                    auto it = newasm::variables::ids.find(opr);
                    if(it->second.type == newasm::datatypes::number) return {true, 4};
                    if(it->second.type == newasm::datatypes::decimal) return {true, 4};
                    if(it->second.type == newasm::datatypes::character) return {true, 1};
                    if(it->second.type == newasm::datatypes::text)
                    {
                        std::string buf;
                        buf = newasm::hardware::randAccessMem.peek<std::string>(it->second.addr);
                        return {true, buf.size()};
                    }

                }
                //std::cout << "\t" << "fail : `" << str << opr << "`\n";
                return {false, 0};
            }

            std::pair<bool, std::string> isdeco(std::string text)
            {
                std::string _text_ = newasm::header::functions::trim(text);
                std::string deco;
                if(_text_.size() >= 3 && _text_.front() == '[' && _text_.back() == ']')
                {
                    deco = _text_.substr(1, _text_.size() - 2);
                    deco = newasm::header::functions::trim(deco);
                    //newasm::progwin::api::cout("Decorator " + text + " valid.");
                    return {true, deco};
                }
                //newasm::progwin::api::cout("Decorator " + text + " not valid.");
                return {false, ""};
            }

            std::atomic<char> pressed('?');
            bool killInputListener = false;

            void inputListener()
            {
                while(true)
                {
                    if(killInputListener)
                    {
                        return;
                    }
                    pressed = newasm::_compat::getch();

                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }


            void pause()
            {
                std::cout << newasm::header::col::gray << "\tPress `q` to terminate the session..." << newasm::header::col::reset;
                std::thread listener(inputListener);
                while(true)
                {
                    if(pressed == 'q')
                    {
                        killInputListener = true;
                        break;
                    }
                }
                listener.join();
                std::cout << std::endl;
            }

            std::pair<bool, std::pair<std::string, std::string>> parseDirective(const std::string& line)
            {
                size_t i = 0;
                while (i < line.length() && std::isspace(line[i])) i++;
                if (i >= line.length() || line[i] != '.') return {false, {"", ""}};
                i++;
                while (i < line.length() && std::isspace(line[i])) i++;
                if (i >= line.length() || line[i] != '$') return {false, {"", ""}};
                i++;
                while (i < line.length() && std::isspace(line[i])) i++;
                std::string directive;
                while (i < line.length() && !std::isspace(line[i]))
                {
                    directive += line[i++];
                }
                while (i < line.length() && std::isspace(line[i])) i++;
                std::string argument;
                while (i < line.length() && !std::isspace(line[i]))
                {
                    argument += line[i++];
                }

                if (!directive.empty())
                {
                    return {true, {directive, argument}};
                }

                return {false, {"", ""}};
            }
			std::pair<bool, std::string> parseNamespace(const std::string& line)
			{
				size_t i = 0;
				while (i < line.length() && std::isspace(line[i])) i++;
				if (i >= line.length() || line[i] != '.') return {false, ""};
				i++;
				while (i < line.length() && std::isspace(line[i])) i++;
				if (i >= line.length() || line[i] != '/') return {false, ""};
				i++;
				while (i < line.length() && std::isspace(line[i])) i++;

				std::string namespace_name;
				while (i < line.length() && !std::isspace(line[i]))
				{
					namespace_name += line[i++];
				}
				if (namespace_name.empty()) return {false, ""};

				return {true, namespace_name};
			}
			
			std::pair<bool, std::vector<std::string>> parseNamespaceSegments(const std::string& line)
			{
				if(line.at(0) == ':')
				{
					return {false, {}};
				}
				
				if(line.back() == ':')
				{
					return {false, {}};
				}
				
				if(line.find("::") == std::string::npos)
				{
					return {false, {}};
				}
				
				std::vector<std::string> segments;
				size_t pos = 0;
				size_t len = line.length();

				while (pos < len)
				{
					size_t next_sep = line.find("::", pos);
					if (next_sep == std::string::npos)
					{
						// Zadnji segment (ili jedini)
						std::string segment = line.substr(pos);
						if (!segment.empty())
							segments.push_back(segment);
						break;
					}
					else
					{
						std::string segment = line.substr(pos, next_sep - pos);
						if (segment.empty())
						{
							// Dvostruki separator sa praznim segmentom nije validan
							return {false, {}};
						}
						segments.push_back(segment);
						pos = next_sep + 2; // preskoči "::"
					}
				}

				if (segments.empty())
					return {false, {}};
				
				for(int i = 0; i < segments.size(); ++i)
				{
					if(!newasm::header::functions::isalphanum(segments.at(i)))
					{
						return {false, {}};
					}
					segments.at(i) = newasm::header::functions::trim(segments.at(i));
				}

				return {true, segments};
			}

			std::string mangleName(const std::vector<std::string>& namespaces, const std::string& symbol_name)
			{
				std::string fullpath;

				// Spoji sve namespaceove sa separatorom
				for (const auto& ns : namespaces)
				{
					fullpath += ns + "::";
				}
				fullpath += symbol_name + static_cast<std::string>(__TIME__) + static_cast<std::string>(__DATE__); // prevent bad code

				// Izračunaj hash
				size_t hash_val = std::hash<std::string>{}(fullpath);

				// Pretvori hash u heksadecimalni string (za čitljivost)
				char buffer[17];
				snprintf(buffer, sizeof(buffer), "%016zx", hash_val);

				return std::string("var_") + buffer;
			}
			
			bool istuple(std::string str)
			{
				if(str.front() == '(' && str.back() == ')')
				{
					return true;
				}
				return false;
			}
			
			std::vector<std::string> parseTuple(const std::string& line)
			{
				std::vector<std::string> result;
				size_t i = 0;
				size_t n = line.size();

				while(i < n && std::isspace(line[i])) i++;
				if(i < n && line[i] == '(') i++;

				while(i < n)
				{
					while( i < n && std::isspace(line[i])) i++;

					if(i >= n || line[i] == ')') break;

					if(line[i] == '"')
					{
						std::string val;
						val += line[i++];
						while (i < n)
						{
							val += line[i];
							if (line[i] == '"' && val.back() != '\\')
							{
								i++;
								break;
							}
							i++;
						}
						result.push_back(val);
					}
					else
					{
						std::string val;
						while(i < n && line[i] != ',' && line[i] != ')')
						{
							if(!std::isspace(line[i]))
								val += line[i];
							else if(!val.empty())
								break;
							i++;
						}
						result.push_back(val);
					}
					while (i < n && (std::isspace(line[i]) || line[i] == ',')) i++;
				}
				return result;
			}
			std::pair<bool, std::pair<std::string, std::string>> checkTupleFormat(const std::string& input)
			{
				size_t i = 0;
				size_t n = input.size();

				auto skipSpaces = [&](size_t& pos)
				{
					while(pos < n && std::isspace(static_cast<unsigned char>(input[pos])))
					{
						++pos;
					}
				};

				skipSpaces(i);
				size_t startText = i;

				while(i < n && !std::isspace(static_cast<unsigned char>(input[i])) && input[i] != '(')
				{
					++i;
				}

				if (startText == i)
				{
					return {false, {"", ""}};
				}

				std::string text = input.substr(startText, i - startText);

				skipSpaces(i);

				if (i >= n || input[i] != '(')
				{
					return {false, {"", ""}};
				}
				++i;

				skipSpaces(i);

				size_t startText2 = i;
				while(i < n && input[i] != ')')
				{
					++i;
				}
				if(i >= n)
				{
					return {false, {"", ""}};
				}
				if(startText2 == i)
				{
					return {false, {"", ""}};
				}

				std::string text2 = input.substr(startText2, i - startText2);

				++i;
				skipSpaces(i);

				if(i != n)
				{
					return {false, {"", ""}};
				}

				return {true, {text, text2}};
			}
			std::pair<bool, std::pair<std::string, std::string>> checkTupleFormat2(const std::string& input)
			{
				size_t i = 0;
				size_t n = input.size();

				auto skipSpaces = [&](size_t& pos)
				{
					while(pos < n && std::isspace(static_cast<unsigned char>(input[pos])))
					{
						++pos;
					}
				};
				skipSpaces(i);
				size_t startText = i;

				while(i < n && (std::isalnum(static_cast<unsigned char>(input[i])) || input[i] == '_'))
				{
					++i;
				}

				if (startText == i)
				{
					return {false, {"", ""}};
				}

				std::string text = input.substr(startText, i - startText);

				skipSpaces(i);

				if (i >= n || input[i] != '(')
				{
					return {false, {"", ""}};
				}
				++i;

				skipSpaces(i);

				size_t startText2 = i;
				while (i < n && input[i] != ')') {
					++i;
				}
				if (i >= n)
				{
					return {false, {"", ""}};
				}
				if (startText2 == i)
				{
					return {false, {"", ""}};
				}

				std::string text2 = input.substr(startText2, i - startText2);

				++i;
				skipSpaces(i);

				if (i != n)
				{
					return {false, {"", ""}};
				}

				return {true, {text, text2}};
			}

            std::pair<bool, std::pair<std::string, std::string>> parseObject(const std::string &input_)
            {
                std::string input = newasm::header::functions::trim(input_);
                if(input.empty())
                {
                    return {false, {"", ""}};
                }
                if(input.front() == '{')
                {
                    return {false, {"", ""}};
                }

                size_t openPos = input.find('{');
                size_t closePos = input.find('}');

                if(openPos == std::string::npos || closePos == std::string::npos || closePos < openPos)
                {
                    return { false, { "", "" } };
                }

                std::string before = newasm::header::functions::trim(input.substr(0, openPos));

                std::string inside = newasm::header::functions::trim(input.substr(openPos + 1));
                inside.pop_back(); // get rid of }
                inside = newasm::header::functions::trim(inside);

                if(before.size() == 0)
                {
                    return {false, {"", ""}};
                }
                if(inside.size() == 0)
                {
                    return {false, {"", ""}};
                }

                return { true, { before, inside } };
            }

            bool parseHandleModifier(const std::string& s)
            {
                size_t i = 0;
                size_t n = s.size();

                while (i < n && std::isspace(s[i])) i++;

                size_t start1 = i;
                while (i < n && s[i] != '=') i++; // sve do zareza
                if (i == start1) return false; // prvi TEXT ne smije biti prazan

                std::string text1 = s.substr(start1, i - start1);

                size_t comma = i;
                while (comma < n && (s[comma] == '=' || std::isspace(s[comma]))) comma++;
                if (comma == n) return false;

                std::string text2 = s.substr(comma);

                if (text2.empty() || text2.find('=') != std::string::npos) return false;
                return true;
            }
            static bool parseDataMacroDecl_2(const std::string& s)
            {
                size_t i = 0;
                size_t n = s.size();

                while (i < n && std::isspace(s[i])) i++;

                size_t start1 = i;
                while (i < n && s[i] != ':') i++; // sve do zareza
                if (i == start1) return false; // prvi TEXT ne smije biti prazan

                std::string text1 = s.substr(start1, i - start1);

                size_t comma = i;
                while (comma < n && (s[comma] == ':' || std::isspace(s[comma]))) comma++;
                if (comma == n) return false;

                std::string text2 = s.substr(comma);

                if (text2.empty() || text2.find(':') != std::string::npos) return false;
                return true;
            }

            static void skipSpaces(const std::string& s, size_t& i)
            {
                while (i < s.size() && std::isspace(s[i])) i++;
            }


            std::pair<int, std::vector<std::string>> parseDataMacroDecl(const std::string& s)
            {
                int pos = s.find(':');
                int pos2 = s.find("::");
                if(pos == std::string::npos)
                {
                    return {0, {}};
                }
                if(isbetween(s, ':', '"') || isbetween(s, ':', '\''))
                {
                    return {0, {}};
                }
                if(pos == pos2)
                {
                    return {0, {}};
                }

                auto parse_tokens = [&](const std::string& str) -> std::vector<std::string> {
                    std::vector<std::string> tokens;
                    std::istringstream iss(str);
                    std::string token;
                    while(iss >> token)
                    {
                        tokens.push_back(token);
                    }
                    return tokens;
                };

                std::vector<std::string> parser = newasm::header::functions::split_fixed(s, ':');
                parser[0] = newasm::header::functions::trim(parser[0]);
                parser[1] = newasm::header::functions::trim(parser[1]);

                if(parser.at(0).empty())
                {
                    return {0, {}};
                }
                if(parser.at(1).empty())
                {
                    return {0, {}};
                }

                auto left = parse_tokens(parser.at(0));

                for(int i = 0; i < parser.size(); ++i)
                {
                    if(parser.at(i).find("::") != std::string::npos)
                    {
                        if(parser.at(i).find("::") == pos)
                        {
                            return {0, {}};
                        }
                    }
                }

                // all checks passed
                if(left.size() == 1)
                {
                    return {1, {left.at(0), parser.at(1)}};
                }
                if(left.size() == 2)
                {
                    return {2, {left.at(0), left.at(1), parser.at(1)}};
                }
                return {0, {}};
            }
            static std::pair<int, std::vector<std::string>> parseDataMacroDecl_(const std::string& s)
            {
                size_t n = s.size();
                size_t i = 0;
                skipSpaces(s, i);

                // Prvi TEXT
                size_t start1 = i;
                if(isbetween(s, ':', '"') || isbetween(s, ':', '\''))
                {
                    return {0, {}};
                }
                while (i < n && s[i] != ':' && !std::isspace(s[i])) i++;
                if (i == start1) return {0, {}};
                std::string text1 = newasm::header::functions::trim(s.substr(start1, i - start1));

                skipSpaces(s, i);

                // FORMAT1: TEXT1 : TEXT2
                if (i < n && s[i] == ':')
                {
                    i++; // preskoči ':'
                    skipSpaces(s, i);
                    std::string text2 = newasm::header::functions::trim(s.substr(i));
                    if (text2.empty()) return {0, {}};
                    return {1, {text1, text2}};
                }

                // FORMAT2: TEXT1 TEXT2 : TEXT3
                size_t start2 = i;
                while (i < n && s[i] != ':') i++;
                if (i == n) return {0, {}}; // nema ':', invalid
                std::string text2 = newasm::header::functions::trim(s.substr(start2, i - start2));
                if (text2.empty()) return {0, {}};

                i++; // preskoči ':'
                skipSpaces(s, i);
                std::string text3 = newasm::header::functions::trim(s.substr(i));
                if (text3.empty()) return {0, {}};

                return {2, {text1, text2, text3}};
            }

            std::pair<bool, std::string> parseContainerType(const std::string& s)
            {
                if(newasm::header::functions::trim(s).at(0) == '?')
                {
                    return {true, newasm::header::functions::trim(s.substr(1))};
                }
                return {false, ""};
            }
        }
    }
    ////////////////////////////

    namespace project_data
    {
        std::string name = "";
        std::string version = "";
        std::vector<std::string> dynamic_libs;
        std::vector<std::string> tokens;

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
                if(key == static_cast<std::string>("dlibs"))
                {
                    dynamic_libs.clear();
                    newasm::header::functions::info("Loading the dynamic libraries...");
                    //std::cout << "Debugging MEGA CRASH " << "1\n";
                    dynamic_libs = newasm::header::functions::split(value,',');
                    for(int i = 0; i < dynamic_libs.size(); ++i)
                    {
                        dynamic_libs[i] = newasm::header::functions::trim(dynamic_libs[i]);
                        //std::cout << "dynamic_libs [" << i << "] : '" << dynamic_libs[i] << "'\n";
                        if(std::filesystem::exists(dynamic_libs[i] + newasm::core::constants::dynlib_ex))
                        {
                            std::ifstream file(dynamic_libs[i] + newasm::core::constants::dynlib_ex);
                            std::vector<std::string> parser,parser2;
                            std::string ins,suf,op;
                            //std::cout << "Debugging MEGA CRASH " << "2\n";
                            //std::vector<std::string> lines;
                            std::cout << newasm::header::col::reset << "\t\t\tSuccessfully loaded dynamic library: " + 
                                static_cast<std::string>(newasm::header::col::gray) + 
                                dynamic_libs[i] << "\n";

                            std::string line;
                            newasm::dynlib::settings::analyzed_dynlib = dynamic_libs[i];
                            newasm::dynlib::settings::lastlinedx = 0;
                            bool dynlib_errno = true;
                            //std::cout << "Debugging MEGA CRASH " << "3\n";
                            while (std::getline(file, line))
                            {
                                line = newasm::header::functions::trim(line);
                                tokens = newasm::common::tokenize(line);
                                (*newasm::dyn_ins_set)[dynamic_libs[i]].push_back(line);
                                newasm::dynlib::settings::lastline = line;
                                newasm::dynlib::settings::lastlinedx += 1;
                                //std::cout << "Debugging MEGA CRASH " << "4\n";

                                if(newasm::header::functions::strfind(line,'!'))
                                {
                                    parser = newasm::header::functions::split_fixed(line,'!');
                                    parser[0] = newasm::header::functions::trim(parser[0]);
                                    parser[1] = newasm::header::functions::trim(parser[1]);
                                }
                                //std::cout << "Debugging MEGA CRASH " << "5\n";
                                
                                if(!parser.empty()) if(!parser[0].empty() && parser[0] == "_")
                                {
                                    dynlib_errno = false;
                                    newasm::dynlib::functions::abort(newasm::dynlib::err_codes::labels_unsupported);
                                    //std::cout << "Debugging MEGA CRASH " << "5.1\n";
                                }
                                //std::cout << "Debugging MEGA CRASH " << "5.2\n";
                                parser.clear();
                                /*if(newasm::header::functions::strfind(line,'.') && newasm::header::functions::strfind(line,','))
                                {
                                    parser = newasm::header::functions::split_fixed(line,',');
                                    op = newasm::header::functions::trim(parser[1]);
                                    parser2 = newasm::header::functions::split_fixed(parser[0],'.');
                                    ins = newasm::header::functions::trim(parser2[0]);
                                    suf = newasm::header::functions::trim(parser2[1]);
                                }*/
                                tokens.clear();
                                if(tokens.size() == 1)
                                {
                                    ins = tokens[0];
                                }
                                if(tokens.size() == 2)
                                {
                                    ins = tokens.at(0);
                                    suf = tokens.at(1);
                                }
                                if(tokens.size() == 3)
                                {
                                    ins = tokens.at(0);
                                    suf = tokens.at(1);
                                    op = tokens.at(2);
                                }
                                //std::cout << "Debugging MEGA CRASH " << "6\n";
                                bool tried_retry_label = false;
                                parser_retry_label:
                                if(!parser.empty()) if(!ins.empty() && (
                                    ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::jmp) ||
                                    ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::je) ||
                                    ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::jne) ||
                                    ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::jl) ||
                                    ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::jle) ||
                                    ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::jg) ||
                                    ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::jge)
                                ))
                                {
                                    dynlib_errno = false;
                                    newasm::dynlib::functions::abort(newasm::dynlib::err_codes::unsupported_instruction);
                                    //std::cout << "Debugging MEGA CRASH " << "6.1\n";
                                }
                                if(dynlib_errno && !tried_retry_label)
                                {
                                    tried_retry_label = true;
                                    ins = line;
                                    parser.push_back("random");
                                    goto parser_retry_label;
                                }
                            }

                            file.close();
                                //std::cout << "Debugging MEGA CRASH " << "7\n";
                        }
                        if(!std::filesystem::exists(dynamic_libs[i] + newasm::core::constants::dynlib_ex))
                        {
                            std::cout << newasm::header::col::red << "\t\t\tFailed to load dynamic library: " + 
                                static_cast<std::string>(newasm::header::col::gray) + 
                                dynamic_libs[i] << "\n";
                                //std::cout << "Debugging MEGA CRASH " << "8\n";
                        }
                    }
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