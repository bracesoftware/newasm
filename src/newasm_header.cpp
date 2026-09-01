// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

#ifndef __cplusplus
    #error WHAT
#endif

module(header, {
    //setup goes here
});

namespace newasm::header
{
    namespace definitions
    {
        typedef std::vector<std::pair<std::string, std::pair<std::string, std::string>>> HelpTable;
    }
    namespace flags
    {
        bool compexpr = false;
        newasm::kernel::thread_safe<bool> autobos = false;
    }
    namespace system_info
    {
        constinit const char* name = "[NewASM]";
        constinit const char* copyright_claim = "(c) 2026 Brace Software Co.";
        constinit const char* fullname = "NewASM Virtual Machine";
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
        //std::string& lastln = CONST__;
        constinit int lastlndx = 0;
        constinit bool exception = true;
        #if 0
            #ifndef newasm::header::data::repl
                bool repl = false;
            #endif
        #endif
        constinit bool repl_end = false;
        constinit bool proc_now = false;
        constinit bool struct_now = false;
        std::string struct_decl = "";

        constinit bool blueprint_now = false;
        std::string blueprint_decl = "";

        newasm::kernel::thread_safe<int> argc = 0;
        newasm::kernel::thread_safe<int> callstkidx = 0;

        constinit int module = 0;

        std::string temp = "temp";
        //macros
        constinit bool macro_now = false;
        std::string macro_decl = "";
        //switch-case
        newasm::kernel::thread_safe<std::string> switched_value;
        newasm::kernel::thread_safe<bool> case_matched = false;
        std::string case_line;
        //tuples
        newasm::kernel::thread_safe<int> tupleIndex = -1;
        //lists
        newasm::kernel::thread_safe<int> listIndex = -1;

        //for calling funcs
        std::string temp_;

        //lol
        constinit bool std_now__ = false;

        //movas instruction
        newasm::kernel::thread_safe<int> movas_type = newasm::core::lang_inf::typenames::num;
        //malloc block
        newasm::kernel::thread_safe<int> malloc_block_used = 0;

        constinit int alignment = 1;
        //offline mode (kernel module for networking,http,tcp is off)
        constinit bool offlineMode = false;

        constinit bool EnableThreads = true;
        #if NEWASM_BROKEN_ACTIVE_THREAD_COUNTER
        NewASM::kernel::ValueTracker<unsigned int> ActiveThreads = 0;
        #else
        constinit unsigned int ActiveThreads = 0;
        #endif

        NewASM::kernel::thread_safe<std::vector<int>> CallCStack;
        #if 0
        NewASM::kernel::thread_safe<bool> TryBlock;
        NewASM::kernel::thread_safe<int> TryJump;
        NewASM::kernel::thread_safe<bool> TryCatched;
        #endif
        bool TryBlock = false;
        int TryJump = -1;
        bool TryCatched = false;
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
        const int inv_ireg_val = -99'999;
        const float inv_freg_val = static_cast<float>(-99'999);

        const std::string std_library = "std.newasm";
    }
    namespace settings
    {
        int debug = 1;
        std::string script_file = newasm::header::constants::default_input;
        std::string script_file_LINKED = newasm::header::constants::default_input;
        bool extra = false;
        bool create_new_projfile = false;
        bool logging = false;
        bool lazy_evhndlr = false;
        bool use_std = false;
    }
    namespace functions
    {
        inline NewASM::header::definitions::HelpTable DescriptionWordWrap(const NewASM::header::definitions::HelpTable& vec)
        {
            auto v = vec;
            for(size_t i = 0; i < v.size(); ++i)
            {
                std::string& description = v[i].second.second;
                std::stringstream ss(description);
                std::string word;
                std::vector<std::string> words;
                
                while(ss >> word)
                {
                    words.push_back(word);
                }

                if(words.size() <= 5)
                {
                    continue;
                }

                if(words.size() > 5)
                {
                    std::string first_part = "";
                    std::string second_part = "";

                    for(size_t j = 0; j < words.size(); ++j)
                    {
                        if(j < 5)
                        {
                            first_part += words[j] + (j == 4 ? "" : " ");
                        }
                        else
                        {
                            second_part += words[j] + (j == words.size() - 1 ? "" : " ");
                        }
                    }

                    v[i].second.second = first_part;
                    v.insert(v.begin() + i + 1, {"", {"", second_part}});
                    //i++; 
                }
            }
            return v;
        }
        
        inline void getreleasetype(std::string& dest)
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
        inline void getsnapshot(std::string& dest)
        {
            std::function<std::string(const std::string&)> hash = [](const std::string& input) -> std::string {
                std::hash<std::string> hasher;
                size_t h = hasher(input);

                static const char charset[] =
                    "0123456789"
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyz";

                const size_t base = sizeof(charset) - 1;

                std::string result;
                while(h > 0)
                {
                    result.push_back(charset[h % base]);
                    h /= base;
                }

                if(result.empty())
                {
                    result = "0";
                }

                return result;
            };

            std::string release__type;
            std::string date = __DATE__;
            std::string time = __TIME__;
            newasm::header::functions::getreleasetype(release__type);

            date = hash(date);
            time = hash(time);

            dest.clear();
            
            dest = date + ":"_str +
                    time + "-"_str +
                    release__type;
            return;
        }
        inline void getversion(std::string& dest)
        {
            dest = (newasm::BUILD_NUMBER | _strV);
            return;
        }
        inline void getos(std::string& dest)
        {
            #if _NEWASM_OS == _NEWASM_OS_windows
                dest = "windows";
            #elif _NEWASM_OS == _NEWASM_OS_windows_old
                dest = "windows_old";
            #elif _NEWASM_OS == _NEWASM_OS_linux
                dest = "linux";
            #elif _NEWASM_OS == _NEWASM_OS_android
                dest = "android";
            #elif _NEWASM_OS == _NEWASM_OS_macos
                dest = "macos";
            #endif
        }
        inline void getarch(std::string& dest)
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

        inline void nullprint(const std::string& text) noexcept
        {
            std::cout << text << '\n';
        }
        inline int nullprint_wnm(const std::string& text) noexcept
        {
            std::cout << newasm::header::system_info::name << " " << text << '\n';
            return 0;
        }
        inline void err(const std::string& text)
        {
            std::cout << newasm::header::col::light_red << " *** "<<newasm::header::style::dim<<"Error:  " << newasm::header::col::gray << text <<newasm::header::col::reset<< '\n';
        }
        inline void wrn(const std::string& text)
        {
            std::cout << newasm::header::col::yellow << " ! "<<newasm::header::style::dim<<"Warning:  " << newasm::header::col::gray << text<<newasm::header::col::reset << '\n';
        }
        inline void info(const std::string& text)
        {
            std::cout << newasm::header::col::light_blue << " Info:  " << newasm::header::col::gray << text << newasm::header::col::reset << '\n';
        }
        //formatted info overloads
        inline void f__(const char* format)
        {
            std::cout << format;
        }
        template<typename T, typename... A>
        void f__(const char* format, T value, A... a) noexcept
        {
            for(; *format != '\0'; format++)
            {
                if(*format == '%' && *(format + 1) != '\0')
                {
                    if(*(format + 1) == '%')
                    {
                        std::cout << '%';
                        format++;
                        continue;
                    }
                    std::cout << value;
                    f__(format + 2, a...); 
                    return;
                }
                std::cout << *format;
            }
        }
        template<typename... A>
        inline void f__(const std::string& format, A... a)
        {
            f__(format.c_str(), a...);
        }

        template<typename... A>
        inline void finfo(A... a) noexcept //main func used
        {
            std::cout << newasm::header::col::light_blue << " Info:  " << newasm::header::col::gray;
            //formatting:
            f__(a...);
            std::cout << newasm::header::col::reset << std::endl;
        }
        //other stuff
        inline void dbg(const std::string& text) noexcept
        {
            if(newasm::header::settings::debug == 1)
                std::cout << newasm::header::col::gray << " Debug:  " << newasm::header::col::reset << text << std::endl;
        }
        inline void linkinfo(const std::string& text) noexcept
        {
            std::cout << newasm::header::col::aqua << " Linker:  " << newasm::header::col::reset << text << std::endl;
        }

        inline void compilerinfo(const std::string& text) noexcept
        {
            std::cout << newasm::header::col::aqua << " Compiler:  " << newasm::header::col::reset << text << std::endl;
        }

        inline void caution(const std::string& text) noexcept
        {
            std::cout << newasm::header::col::orange << " * "<<""<<"Caution: " <<newasm::header::col::reset<< newasm::header::col::gray << text << newasm::header::col::reset<<std::endl;
        }
        
        /*void init()
        {
            newasm::header::functions::info("Init finished.");
        }*/
        //std::string trim(const std::string &str);
        //absolute bs
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
        inline void vers_info(bool showglitch = true) noexcept
        {
            std::string arch;
            std::string os;
            std::string version;
            std::string snapshot;

            newasm::header::functions::getarch(arch);
            newasm::header::functions::getos(os);
            newasm::header::functions::getversion(version);
            newasm::header::functions::getsnapshot(snapshot);

            if(showglitch) std::cout << std::endl;

            if(showglitch) std::cout << newasm::header::col::light_blue << newasm::header::style::bold << newasm::header::style::reverse << newasm::header::style::underline;
            if(showglitch) newasm::utils::glitch_text(newasm::header::system_info::fullname);
            std::cout << newasm::header::col::reset << newasm::header::col::gray;
            std::cout << newasm::header::col::light_blue << "\n  Build: " << newasm::header::col::gray << version << "-" << os << "_" << arch;
            std::cout << newasm::header::col::light_blue << "\n  Snapshot: " << newasm::header::col::gray << snapshot;
            std::cout << newasm::header::col::light_blue << "\n  Compiled with:"<< newasm::header::col::gray << " C++" << __cplusplus << " on " << __DATE__ << " at " << __TIME__;
            
            std::cout << "\n\n  Runtime version: v" << newasm::runtime::version;
            std::cout << "\t\tKernel version: v" << newasm::KERNEL_VERSION;
            
            std::cout << "\n  " << newasm::header::system_info::copyright_claim <<
            newasm::header::col::reset << std::endl;
            
            std::cout << "\a";
            return;
        }
        const int first_column = 20;
        const int second_column = 20;
        const int third_column = 20;
        const std::string tabs = "\t";
        const NewASM::header::definitions::HelpTable help_table_data = {
            {"h",           {"/",               "Displays the help panel."}},
            {"l",           {"/",               "Enables the logging system."}},
            {"nv",          {"/",               "Disables the version check feature."}},
            {"std",         {"/",               "Load the standard library before staring the application."}},
            {"nodbg",       {"/",               "Disables the debugger window. May increase performance."}},
            {"out",         {"<binary name>",   "Provide binary name for the compiler, default is `a.out`."}},
            {newasm::args::nolco,       {"/",   "Tells the compiler not to log any optimization information into the console."}}
        };

        inline void help_info() noexcept
        {
            auto vec = NewASM::header::functions::DescriptionWordWrap(help_table_data);
            int longest_text = 0;
            for(int i = 0; i < vec.size(); ++i)
            {
                int l = vec.at(i).second.second.size();
                if(l > longest_text)
                {
                    longest_text = l;
                }
            }
            std::cout << "\n";

            std::cout << newasm::header::col::light_blue << newasm::header::style::underline;

            std::cout << '\t'
                << std::setw(first_column) << std::left << "Argument"
                << std::setw(second_column) << std::left << "Params"
                << std::setw(longest_text)  << std::left << "Description"
                << "\n";

            std::cout << newasm::header::col::reset;

            for(const auto& row : vec)
            {
                std::cout << newasm::header::col::gray;
                std::cout << '\t'
                    << std::setw(first_column)  << std::left << row.first
                    << std::setw(second_column) << std::left << row.second.first
                    << std::setw(third_column)  << std::left << row.second.second
                    << "\n";
            }

            std::cout << newasm::header::col::reset;

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
        inline int wait(int milliseconds) noexcept
        {
            if (milliseconds < 0) return -1;
            std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
            return 0;
        }
        const std::vector<std::string> lambda_ = {
            "proc", "end"
        };

        @nodiscard
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

        [[maybe_unused]]
        std::string form_iso(std::string ins, std::string suf, std::string opr)
        {
            std::stringstream result;
            result << ins << " " << suf << "," << opr;
            return result.str();
        }

        @nodiscard
        inline std::vector<std::string> split___(const std::string &str, char delimiter)//ORIDZINAL
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
        ATTR_FLAT inline std::vector<std::string> split(const std::string &str, char delimiter)
        {
            std::vector<std::string> tokens;
            size_t start = 0, end = 0;
            while((end = str.find(delimiter, start)) != std::string::npos)
            {
                std::string token = str.substr(start, end - start);
                if(!newasm::header::functions::trim(token).empty()) 
                {
                    tokens.push_back(token);
                }
                start = end + 1;
            }
            std::string last = str.substr(start);
            if(!newasm::header::functions::trim(last).empty()) tokens.push_back(last);
            return tokens;
        }

        @nodiscard
        inline std::vector<std::string> split_fixed(const std::string &str, char delimiter)
        {
            if constexpr(0)
            {
                std::cout << "split_fixed -> str: `" << str << "`, delimiter: `" << delimiter << "`\n";
            }
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

        @nodiscard
        inline std::string trim__(const std::string& str) //ORIDZINAL
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

            //returns trimmed string
            return std::string(start, it + 1);
        }
        ATTR_FLAT inline std::string trim(const std::string& str)
        {
            const auto start = str.find_first_not_of(" \t\n\r\f\v");
            if(start == std::string::npos) return "";
            const auto end = str.find_last_not_of(" \t\n\r\f\v");
            return str.substr(start, end - start + 1);
        }
        inline bool isnumeric_(const std::string& str)
        {
            std::string copy;
            for(char i : str)
            {
                if(!std::isspace(i))
                {
                    copy = copy + i;
                }
            }
            if(copy.empty())
            {
                return false;
            }
            if(copy[0] != '-' && !std::isdigit(copy[0]))
            {
                return false;
            }
            return std::all_of(copy.begin() + 1, copy.end(), ::isdigit);
        }
        inline bool isnumeric(const std::string& str)//ORIDZINAL
        {
            bool first = true;
            for(char c : str)
            {
                if(std::isspace(static_cast<unsigned char>(c))) continue;
                if(first)
                {
                    if(c != '-' && !std::isdigit(static_cast<unsigned char>(c)))
                    {
                        return false;
                    }
                    first = false;
                }
                else if(!std::isdigit(static_cast<unsigned char>(c)))
                {
                    return false;
                }
            }
            return !first; // barem jedan validan char
        }
        //optimization
        inline bool isnumeric__(const std::string& str)
        {
            if(str.empty()) return false;
            size_t start = 0;
            if(str[0] == '-')
            {
                if(str.size() == 1) return false;
                start = 1;
            }
            return std::all_of(str.begin() + start, str.end(), [](unsigned char c) {
                return std::isdigit(c);
            });
        }
        inline bool strfind(const std::string& str, const char c)
        {
            return str.find(c) != std::string::npos;
        }
        inline bool isalphanum(const std::string& str)
        {
            return !str.empty() && std::all_of
            (
                str.begin(), str.end(), [](unsigned char c) 
                {
                    return std::isalnum(c) || c == '_';
                }
            );
        }
        inline void parseopr(std::string &opr,std::unordered_map<std::string, std::string> &data)
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

        inline bool CheckFloatingPoint(const std::string& s)
        {
            int dotCount = 0;
            int minusSign = 0;

            if(s.empty()) return false;

            for(size_t i = 0; i < s.size(); ++i)
            {
                char c = s[i];
                if('0' <= c && c <= '9')//if(std::isdigit(c))
                {
                    continue;
                }
                else if(c == '.')
                {
                    dotCount++;
                    if(dotCount > 1)
                    {
                        return false;
                    }
                    if(i == 0 || i == s.size() - 1)
                    {
                        return false;
                    }
                } 
                else if(c == '-')
                {
                    minusSign++;
                    if(minusSign > 1)
                    {
                        return false;
                    }
                }
                else 
                {
                    return false;
                }
            }

            if(minusSign == 1)
            {
                if(s.at(0) != '-')
                {
                    return false;
                }
            }

            return dotCount == 1;
        }

        ATTR_FLAT inline bool isfloat(const std::string& str)
        {
            return CheckFloatingPoint(str);// or isnumeric(str);
        }

        inline bool istext_(const std::string& str)
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

        inline bool istext(const std::string& s)
        {
            return (s.size() >= 2 && s.front() == '"' && s.back() == '"');
        }

        inline bool istext__(const std::string& s)
        {
            return (s.size() >= 2 && s.front() == '"' && s.back() == '"' && std::count(s.begin(), s.end(), '"') == 2);
        }

        inline bool isref_(const std::string& str)
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
        inline bool isref(const std::string& s)
        {
            return s.size() > 1 && s.front() == '&' && std::count(s.begin(), s.end(), '&') == 1;
        }

        inline bool ischar(const std::string& s)
        {
            return (s.size() == 3 or (s.size() == 4 && s.at(1) == '\\')) && s.front() == '\'' && s.back() == '\'';
        }

        inline bool ischar_(const std::string &str)
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
        inline std::string remq(const std::string& str)
        {
            if(str.length() >= 2 && str.front() == '"' && str.back() == '"')
            {
                return str.substr(1, str.length() - 2);
            }
            return str;
        }
        inline std::string remsq(const std::string& str)
        {
            if(str.length() >= 2 && str.front() == '\'' && str.back() == '\'')
            {
                return str.substr(1, str.length() - 2);
            }
            return str;
        }
        inline std::string remc(const std::string &line)
        {
            std::size_t pos = line.find(';');
            if(pos != std::string::npos)
            {
                return line.substr(0, pos);
            }
            return line;
        }
        inline std::string remamp(const std::string &line)
        {
            std::size_t pos = line.find('&');
            if(pos != std::string::npos)
            {
                return newasm::header::functions::trim(line.substr(pos+1));
            }
            return line;
        }

        template<typename _Arg>
        @nodiscard
        inline std::string tohex(const _Arg& value)
        {
            std::stringstream ss;
            ss << std::hex << value;
            return ss.str();
        }

        @nodiscard
        inline int strdist(const std::string& a, const std::string& b)
        {
            if(a == b)
            {
                return 0;
            }
            int size_a = a.size();
            int size_b = b.size();
            int result = 0;

            for(int i = 0; i < (size_a > size_b) ? size_b : size_a; ++i)
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

        //credits stackoverflow
        int hextoi(const std::string& hex_str)
        {
            size_t start = 0;
            int result = 0;

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
            if(str.size() < 3) return false;
            if(str.at(0) != '0' || str.at(1) != 'x') return false;
            
            for(int i = 2; i < str.size(); ++i)
            {
                char c = str.at(i);
                if(!(('0' <= c && c <= '9') || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F')))
                {
                    return false;
                }
            }
            return true;
        }
        bool isbin(const std::string& str)
        {
            if(str.size() < 3)
            {
                return false;
            }
            if(str.at(0) != '0' || str.at(1) != 'b')
            {
                return false;
            }
            
            for(int i = 2; i < str.size(); ++i)
            {
                char c = str.at(i);
                if(c != '0' && c != '1')
                {
                    return false;
                }
            }
            return true;
        }
        int bintoi(const std::string& str)
        {
            std::string s = str;
            if(s.substr(0, 2) == "0b")
            {
                s = s.substr(2);
            }
            return std::stoi(s, nullptr, 2);
        }
        bool hasnull(std::string str, char &delim, int &pos)
        {
            for(int i = 0; i < str.size(); ++i)
            {
                if(
                    (str.at(i) == '\t') ||
                    (str.at(i) == (char)32)
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
        inline std::pair<bool, int> isallocref(std::string text)
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

        inline std::pair<bool, int> isargref(std::string text)
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
        inline std::pair<bool, int> isvmemsize(std::string& text)
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
        inline std::pair<bool, int> isvmemref(std::string text)
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
        inline std::pair<bool, std::pair<int, int>> isrange(const std::string& input)
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

        inline bool case_typename(std::string& switched_val, std::string& suf)
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

        inline std::pair<bool, std::string> isdeco(std::string text)
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

        inline void inputListener()
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

        inline void pause()
        {
            std::cout << newasm::header::col::gray << "\tPress `q` to terminate the session..." << newasm::header::col::reset << std::flush;
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
            while(i < line.length() && std::isspace(line[i])) i++;
            if(i >= line.length() || line[i] != '.')
            {
                //std::cout << "THIS HAPPENED" << std::endl;
                return {false, {"", ""}};
            }
            i++;
            while(i < line.length() && std::isspace(line[i])) i++;
            if(i >= line.length() || line[i] != '$')
            {
                //std::cout << line.length <<
                return {false, {"", ""}};
            }
            i++;
            while(i < line.length() && std::isspace(line[i])) i++;
            std::string directive;
            while(i < line.length() && !std::isspace(line[i]))
            {
                directive += line[i++];
            }
            while(i < line.length() && std::isspace(line[i])) i++;
            std::string argument;
            while(i < line.length() && !std::isspace(line[i]))
            {
                argument += line[i++];
            }

            //i now fuckin hate spaces after if/while/for
            if(!directive.empty())
            {
                //std::cout << directive << '-' << argument << std::endl;
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
            if(line.empty())
            {
                return {false, {}};
            }
            
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

            if(segments.empty())
            {
                return {false, {}};
            }
            
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

        inline std::string demangleName(const std::vector<std::string>& namespaces, const std::string& symbol_name)
        {
            std::stringstream ss;
            for(int i = 0; i < namespaces.size(); ++i)
            {
                ss << namespaces[i] << "::";
            }
            ss << symbol_name;
            return ss.str();
        }

        inline std::string mangleName__1(const std::vector<std::string>& namespaces, const std::string& symbol_name)
        {
            std::string fullpath;

            for(const auto& ns : namespaces)
            {
                fullpath += ns + "::";
            }
            fullpath += symbol_name + static_cast<std::string>(__TIME__) + static_cast<std::string>(__DATE__); // prevent bad code

            size_t hash_val = std::hash<std::string>{}(fullpath);

            char buffer[17];
            snprintf(buffer, sizeof(buffer), "%016zx", hash_val); //turn to hexadecimal format

            return std::string("__newasm_symbol") + buffer;
        }
        #define MAGIC_NUMBER__ 15
        inline std::string mangleName(const std::vector<std::string>& namespaces, const std::string& symbol_name)
        {
            size_t total_size = symbol_name.size() + sizeof(__TIME__) - 1 + sizeof(__DATE__) - 1;
            for(const auto& ns : namespaces)
            {
                total_size += ns.size() + 2; //"::" oper
            }
            std::string fullpath;
            fullpath.reserve(total_size);

            for(const auto& ns : namespaces)
            {
                fullpath += ns;
                fullpath += "::";
            }

            fullpath += symbol_name;
            fullpath += __TIME__;
            fullpath += __DATE__;

            size_t hash_val = std::hash<std::string>{}(fullpath);
            char buffer[16];
            static constexpr char hex[] = "0123456789abcdef";

            for(int i = MAGIC_NUMBER__; i >= 0; --i)
            {
                buffer[i] = hex[hash_val & 0xF];
                hash_val >>= 4;
            }
            std::string result;
            result.reserve(2 * MAGIC_NUMBER__ + 3); // "__newasm_symbol" + hash

            result += "__newasm_symbol";
            result.append(buffer, MAGIC_NUMBER__ + 1);

            return result;
        }

        typedef std::pair<bool, std::pair<std::vector<std::string>, std::string>> NamespaceDetectionResult;
        inline NamespaceDetectionResult DetectNamespace(std::string str)
        {
            NamespaceDetectionResult result;
            str = NewASM::header::functions::trim(str);
            std::string temp;
            result.first = true;
            if(str.find("::") == std::string::npos)
            {
                result.first = false;
                return result;
            }
            for(unsigned int i = 0; i < str.size(); ++i)
            {
                auto c = str.at(i);
                auto next_c = i + 1 >= str.size() ? 0 : str.at(i + 1);
                if(c == ':' and next_c == ':')
                {
                    result.second.first.push_back(temp);
                    temp.clear();
                    ++i;
                    continue;
                }
                temp += c;
                continue;
            }
            result.second.second = temp;

            auto& l = result.second.second;
            if(l.empty() or !newasm::header::functions::isalphanum(l))
            {
                result.first = false;
            }

            auto& k = result.second.first;
            for(int i = 0; i < k.size(); ++i)
            {
                if(k.at(i).empty() or !newasm::header::functions::isalphanum(k.at(i)))
                {
                    result.first = false;
                    break;
                }
            }

            return result;
        }
        
        inline bool istuple(const std::string& str)
        {
            if(str.front() == '(' && str.back() == ')')
            {
                return true;
            }
            return false;
        }

        inline bool isTupleOrContext(const std::string& str)
        {
            return istuple(str);
        }
        
        inline std::vector<std::string> parseTuple(const std::string& line)
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
        inline std::vector<std::string> parseTupleOrContext(const std::string& str)
        {
            return parseTuple(str);
        }
        inline std::vector<std::string> parseContext(const std::string& input)
        {
            std::vector<std::string> result;
            if(input == "()")
            {
                return result;
            }

            size_t start = input.find('(');
            size_t end = input.rfind(')');

            auto isWhitespaces = [](const std::string& str) -> bool {
                if(str.empty())
                {
                    return true;
                }
                for(unsigned char c : str)
                {
                    if(!std::isspace((unsigned char)c))
                    {
                        return false;
                    }
                }
                return true;
            };

            if(start == std::string::npos || end == std::string::npos || start >= end)
            {
                return result;
            }

            if(isWhitespaces(input.substr(start + 1, end - start - 1)))
            {
                return result;
            }

            std::string inner = input.substr(start + 1, end - start - 1);

            size_t pos = 0;
            while(true)
            {
                size_t comma = inner.find(',', pos);
                if(comma == std::string::npos)
                {
                    result.push_back(inner.substr(pos));
                    break;
                }

                result.push_back(inner.substr(pos, comma - pos));
                pos = comma + 1;
            }

            return result;
        }

        inline std::pair<bool, std::pair<std::string, std::string>> checkTupleFormat(const std::string& input)
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

        template<typename T>
        inline int getIndex(const std::vector<T>& v, const T& target)
        {
            auto it = std::find(v.begin(), v.end(), target);
            if(it != v.end())
            {
                return std::distance(v.begin(), it);
            }
            return (-1);
        }

        inline auto checkTupleOrContextFormat(const std::string& input)
        {
            return checkTupleFormat(input);
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

        inline bool parseHandleModifier(const std::string& s)
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
        #if 0 //old func i modified from chatgpt in 2022, broke stuff
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
            if(text2.empty())
            {
                return {0, {}};
            }
            i++; // preskoči ':'
            skipSpaces(s, i);
            std::string text3 = newasm::header::functions::trim(s.substr(i));
            if(text3.empty())
            {
                return {0, {}};
            }

            return {2, {text1, text2, text3}};
        }
        #endif

        std::pair<bool, std::string> parseContainerType(const std::string& s)
        {
            if(newasm::header::functions::trim(s).at(0) == '?')
            {
                return {true, newasm::header::functions::trim(s.substr(1))};
            }
            return {false, ""};
        }

        inline std::vector<std::string> tokenize__2(std::string s)
        {
            std::vector<std::string> tokens;
            std::istringstream iss(s);
            std::string token;

            while(iss >> token)
            {
                tokens.push_back(token);
            }

            std::string first = tokens.at(0);
            std::string second = "null";

            if(tokens.size() == 1)
            {
                tokens.clear();
                tokens.push_back(first);
                tokens.push_back(second);
                return tokens;
            }

            second.clear();

            for(int i = 1; i < tokens.size(); ++i)
            {
                second = second + tokens.at(i);
            }

            if(tokens.size() != 2)
            {
                tokens.clear();
                tokens.push_back(first);
                tokens.push_back(second);
            }

            return tokens;
        }

        //////
        template<bool _Force = false>
        inline std::string parseBackslash(const std::string& s) // RETURNS WHAT WE NEED, IT DOESN'T PASS BY REFERENCE
        {
            std::string final_str = s;

            if constexpr(!_Force)
            {
                if((!newasm::header::functions::istext(final_str)) && (!newasm::header::functions::ischar(final_str)))
                {
                    //std::cout << "s: ``" << final_str << "`` is NOT text" << std::endl;
                    return final_str;
                }
            }

            std::string before_slash, after_slash;
            char character;
            int idx = 0;

            while(true)
            {
                idx = final_str.find('\\');

                if(idx == std::string::npos)
                {
                    return final_str;
                }

                if(idx + 1 >= final_str.size())
                {
                    return final_str;
                }

                character = final_str[idx + 1];

                // support for \n, \t, \r, \a etc.
                before_slash = final_str.substr(0, idx);
                after_slash = final_str.substr(idx + 2, final_str.size() - idx - 2);

                static const std::unordered_map<char, char> escape_map = {
                    {'n', '\n'},
                    {'r', '\r'},
                    {'t', '\t'},
                    {'v', '\v'},
                    {'f', '\f'},
                    {'a', '\a'},
                    {'b', '\b'},
                    {'\\', '\\'},
                    {'\'', '\''},
                    {'"', '\"'},
                    {'0', '\0'},
                    {'?', '?'}
                };

                auto charry = escape_map.find(character);
                if(charry != escape_map.end())
                {
                    final_str = before_slash + charry->second + after_slash;
                }
                if(charry == escape_map.end())
                {
                    final_str = before_slash + "?" + after_slash;
                }
            }
            //std::cout << "s: ``" << final_str << "`` is FULLY parsed!" << std::endl;
            return final_str;
        }
        inline std::pair<bool, int> issizeof(const std::string& str)
        {
            std::string opr;
            //std::cout << "\t" << "STR IN ISSIZEOF : `" << str << "`\n";
            if(str.size() > 2 && str[0] == '$' && str[1] == '-')
            {
                opr = newasm::header::functions::trim(str.substr(2));

                // dobijamo &lol
                newasm::runtime::functions::parse(opr);

                //newasm::progwin::api::cout("issizeof()::we_got .. -> " + opr);
                if(newasm::header::functions::isref(opr))
                {
                    opr = newasm::header::functions::remamp(opr);
                
                    auto it = NewASM::VAR_TABLE_PTR->find(opr);
                    if(it == NewASM::VAR_TABLE_PTR->end())
                    {
                        //std::cout << "JEL SE RADI O OVOM? -----> " <<opr<< std::endl;
                        NewASM::terminate(*NewASM::CapturedData::ExitCodeInvalidMemacc);
                        return {false, 0};
                    }
                    if(it->second.type == newasm::datatypes::number) return {true, 4};
                    if(it->second.type == newasm::datatypes::decimal) return {true, 4};
                    if(it->second.type == newasm::datatypes::character) return {true, 1};
                    if(it->second.type == newasm::datatypes::text)
                    {
                        std::string buf;
                        buf = NewASM::RamChip::PeekString(it->second.addr);
                        return {true, buf.size() + 4};
                    }
                }
                else
                {
                    if(newasm::header::functions::istext(opr))
                    {
                        std::string buf;
                        buf = newasm::header::functions::remq(opr);
                        return {true, buf.size()};
                    }
                }

            }
            //std::cout << "\t" << "fail : `" << str << opr << "`\n";
            return {false, 0};
        }
        /////////////////////

        inline std::pair<bool, std::string> parseSealedLabel(const std::string& s)
        {
            if(s.empty() or s.size() < 2)
            {
                return {false, NIL_STR};
            }
            if(s.front() != '{' and s.back() != '}')
            {
                return {false, NIL_STR};
            }
            std::string str = newasm::header::functions::trim(s.substr(1, s.size() - 2));
            if(str.at(0) != ':')
            {
                return {false, NIL_STR};
            }

            str = newasm::header::functions::trim(str.substr(1));

            if(!newasm::header::functions::isalphanum(str))
            {
                return {false, NIL_STR};
            }

            return {true, str};
        }
        @nodiscard
        inline std::pair<bool, std::vector<std::string>> parseAttribute(const std::string& line)
        {
            if(line.empty())
            {
                return {false, {}};
            }
            if(line.at(0) != '@')
            {
                return {false, {}};
            }

            std::string s = line.substr(1);
            auto vec = NewASM::header::functions::split(s, ',');
            for(int i = 0; i < vec.size(); ++i)
            {
                vec[i] = NewASM::header::functions::trim(vec[i]);
                if(!NewASM::header::functions::isalphanum(vec[i]) || vec.at(i).empty())
                {
                    return {false, {}};
                }
            }
            return {true, vec};
        }
    }
    //------------------------------------------------------
}
namespace newasm::project_data
{
    std::vector<std::string> dynamic_libs;
    std::vector<std::string> tokens;

    namespace impl
    {
        inline int parse_proj_settings(const std::string& key, const std::string& value)
        {
            if(key == "name"_str)
            {
                newasm::project_data::name = value;
                return 1;
            }
            if(key == "version"_str)
            {
                newasm::project_data::version = value;
                return 1;
            }
            if(key == "dlibs"_str)
            {
                if(value == NIL_STR)
                {
                    return 1;
                }
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
                        std::cout << newasm::header::col::gray << "\tSuccessfully loaded dynamic library: "_str +
                            newasm::header::col::gray + dynamic_libs[i] << "\n";

                        std::string line;
                        newasm::dynlib::settings::analyzed_dynlib = dynamic_libs[i];
                        newasm::dynlib::settings::lastlinedx = 0;
                        bool dynlib_errno = true;
                        //std::cout << "Debugging MEGA CRASH " << "3\n";
                        while(std::getline(file, line))
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
                                ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::jz) ||
                                ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::jnz) ||
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
                        std::cout << newasm::header::col::red << " \tFailed to load dynamic library: "_str + 
                            newasm::header::col::gray + 
                            dynamic_libs[i] << "\n";
                            //std::cout << "Debugging MEGA CRASH " << "8\n";
                    }
                }
            }
            return 1;
        }
        inline int setup_proj(const std::string& script_file)
        {
            static const auto PROJ_FILE_EXT = ".newasm_proj"_str;
            std::ifstream internal_fileobject(script_file + PROJ_FILE_EXT);
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
                    std::ofstream internal_fileobject(newasm::header::settings::script_file + PROJ_FILE_EXT, std::ios::app);

                    if(internal_fileobject.is_open())
                    {
                        internal_fileobject << "name = Unnamed project" << "\n";
                        internal_fileobject << "version = 0.0.1" << "\n";
                        internal_fileobject << "dlibs = nil" << "\n";
                        internal_fileobject.close();
                    }
                }
            }
            return 1;
        }
    } 
}
