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

<======================== DO NOT DISTRIBUTE! ========================>

*/

#pragma GCC diagnostic ignored "-Wunused-result" // used gcc for ts
#pragma unroll

namespace newasm
{
    const int BUILD_NUMBER = 16;
    const int RUNTIME_VERSION = 5;
    const int KERNEL_VERSION = 2;
}

#include "runtime/common/os.h"
#include "runtime/common/arch.h"

#define EMPTYLINE std::cout<<"\n"
#include <iostream>
#include <initializer_list>
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
#include <random>
#include <map>
#include <cstring>
#include <regex>
//LOl
#include <limits>
#include <cstdlib> //memcpy, rand
#include <iomanip>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h> // for golang compiler
#include <deque>
#include <new>
//For idk
#include <functional>
#include <utility>
// For registers
#include <type_traits>
#include <concepts>

#include "sysext/out.cpp"
#include "vm/external.cpp"
#include "sysext/maps.cpp"
#include "vm/impl.cpp"
#include "sysext/csimple.cpp"
#define __newasm_included
#include "runtime/alpha.cpp"
#include "newasm_stdex.cpp"
#include "sys._platformSpecific.cpp"

#include "taster._platformSpecific.cpp"

// thread init
#include "kernel/threads/_flags.cpp"
#include "vm/blueprint/class.cpp"
// hardware changes
#include "vm/hardware/multiproc.cpp"
extern "C"
{
    void free_string(char* str);
}
#include "vm/_console.cpp"
#include "vm/hardware/cpu_register.cpp"

// Resources (assets) used in the program
namespace newasm
{
    namespace constv
    {
        const std::string quote = "\"";
    }

    void callproc(std::string& name);
    int process_s_(bool &valid, std::string wholeline, std::string stat, std::string arg);
    int process_s(std::string& section);
    namespace user
    {
        int udb_hash(const std::string& input);
    }
    int terminate(int exit_code);
    void async(const std::string& name);
    namespace header
    {
        namespace functions
        {
            void krnl(std::string text);
            //void parseRegDeref(std::string& arg);
            std::pair<bool, int> issizeof(const std::string& str);
        }
    }
    namespace common
    {
        std::vector<std::string> tokenize(const std::string line);
    }
    namespace runtime
    {
        int version;
        namespace functions
        {
            void parse(std::string& suf);

            template<bool _procNameParse>
            void parse(std::string& suf);
        }
    }
    namespace compiler
    {
        struct lineData
        {
            std::string raw;
            int type;
            std::vector<std::string> tokens;
            std::string other;

            int priArgType = 0;
            int altArgType = 0;
        };

        std::string parse_def(std::string suf);
        void process_comptis(std::string ins, std::string arg1);
        void process_comptiso(std::string ins, std::string arg1, std::string arg2);
        bool iscomptins(std::string ins);
    }
    
    int procline(newasm::compiler::lineData& line);
    int procline(std::string& text);
    int procline(const char* line);

    void tokenize(std::string str);
    namespace impl
    {
        std::string eval(std::string str);
    }

    //--------------------------------------
    bool vercheck = true;
    bool dwin = true;
    const std::string tab = "\t\t\t";
    std::unordered_map<std::string,std::vector<std::string>>* dyn_ins_set;
    std::vector<std::pair<std::string,std::string>>* env_vars;

    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    std::vector<std::chrono::duration<double, std::milli>> runtime_deduction;
    std::vector<std::chrono::duration<double, std::milli>> wasted_deduction;
    std::vector<std::chrono::duration<double, std::milli>> network_deduction;

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
    //------------------------------------------------------
    namespace brace_stack
    {
        constinit const int object_block = 0;
        constinit const int thread_block = 1;
        constinit const int class_block = 2;
    }
    std::vector<int> brace_stack__;
    //------------------------------------------------------
    class timer
    {
        private:
        std::atomic<bool> running;
        std::atomic<double> elapsed_ms;
        std::thread t;

        public:
        timer() : running(false), elapsed_ms(0.0) {}

        inline void start()
        {
            if (running) return;
            running = true;
            elapsed_ms = 0.0;

            t = std::thread([this]() {
                auto last = std::chrono::high_resolution_clock::now();
                while (running) {
                    auto now = std::chrono::high_resolution_clock::now();
                    elapsed_ms += std::chrono::duration<double, std::milli>(now - last).count();
                    last = now;
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
            });
        }

        inline void stop()
        {
            running = false;
            if(t.joinable()) t.join();
        }

        inline double count() const
        {
            return elapsed_ms.load();
        }
    };

    namespace perf
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto end = std::chrono::high_resolution_clock::now();

        newasm::timer inputWasteTimer;
    }

    void* test = nullptr;

    class forLinker__OLD
    {
        public:
        static inline newasm::_std::linear_map<int, std::string> files;
        static inline newasm::_std::linear_map<std::string, int> file_sizes;
        static inline int linked_size;

        static inline std::vector<std::string> debug;

        static inline std::string getNextFile(int& line)
        {
            for(int i = files.size() - 1; i != 0; --i)
            {
                if(line < files(i).first)
                {
                    return files(i).second;
                }
            }
            //std::cout << "RETURNED `" << files[0] << "`" << std::endl;
            return files[0];
        }

        static inline int getNextFile__B(int& line)
        {
            for(int i = files.size() - 1; i != 0; --i)
            {
                //std::cout << "getNextFile__B->1(): files(i).first = " << files(i).first << "; files(i).second = " << files(i).second << std::endl;
                if(line < files(i).first)
                {
                    //std::cout << "getNextFile__B->2(): files(i).first = " << files(i).first << "; files(i).second = " << files(i).second << std::endl;
                    return files(i).first;
                }
            }
            return 0;
        }

        static inline int getNextFile__C(int& line)
        {
            for(int i = files.size() - 2; i != 0; --i)
            {
                if(line >= files(i).first)
                {
                    //std::cout << "1->RETURNED `" << files(i).second << "`" << std::endl;
                    return files(i + 1).first;
                }
            }
            //std::cout << "2->RETURNED `" << files[0] << "`" << std::endl;
            return 0;
        }

        static inline int getLine_SKLJ(int& lastlinedx)
        {
            int result = 0;
            result = std::abs(getNextFile__C(lastlinedx) - lastlinedx);
            return result;
        }

        static inline int getLine(int& line)
        {
            for(int i = files.size() - 1;; --i)
            {
                //std::cout << "getLine :: Comparing (" << i << "): line -> " << line << "; files(i).first -> " << files(i).first << std::endl;
                debug.push_back("getLine :: Comparing (" + std::to_string(i) + "): line -> " + std::to_string(line) + "; files(i).first -> " + std::to_string(files(i).first));
                if(line >= files(i).first)
                {
                    //std::cout << "1->RETURNED `" << files(i).second << "`" << std::endl;
                    return std::abs(line - files(i).first);
                }

                if(i == 0)
                {
                    break;
                }
            }
            //std::cout << "2->RETURNED `" << files[0] << "`" << std::endl;
            return line;
        }

        static inline std::string getFile(int& line)
        {
            for(int i = files.size() - 1;; --i)
            {
                debug.push_back("getFile :: Comparing (" + std::to_string(i) + "): line -> " + std::to_string(line) + "; files(i).first -> " + std::to_string(files(i).first));
                if(line >= files(i).first)
                {
                    //std::cout << "1->RETURNED `" << files(i).second << "`" << std::endl;
                    return files(i).second;
                }

                if(i == 0)
                {
                    break;
                }
            }
            //std::cout << "2->RETURNED `" << files[0] << "`" << std::endl;
            return files[0];
        }

        static inline std::string getFile__B(int& line)
        {
            for(int i = 0; i < files.size() - 1; ++i)
            {
                if(files(i).first <= line)
                {
                    //std::cout << "RETURNED `" << files(i).second << "`" << std::endl; 
                    return files(i).second;
                }
            }
            //std::cout << "RETURNED `" << files[0] << "`" << std::endl;
            return files[0];
        }
    };

    class forLinker
    {
        public:
        static inline std::vector<std::pair<std::string, int>> lineData;

        static inline std::string getFile(int line)
        {
            if(line < 0)
            {
                return lineData.at(0).first;
            }
            return lineData.at(line).first;
        }

        static inline int getLine(int lastlinedx)
        {
            if(lastlinedx < 0)
            {
                return 0;
            }
            return lineData.at(lastlinedx).second;
        }
    };
}

/*
Essential stuff needed to run
is in the runtime
*/

#include "kernel/malloc.h"
#include "runtime/handlers.cpp"
#include "runtime/lang_inf.cpp"

#include "runtime/progwin_api.cpp"
#include "utils._platformSpecific.cpp"

#include "runtime/utils.cpp"
#include "runtime/common/opcodes.h"

#include "newasm_dynlib.cpp"
#include "newasm_header.cpp"
#include "compiler/native_jit.cpp"
#include "linker/asmlink.cpp"

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
#include "kernel/system_calls/misc.cpp"
#include "kernel/system_calls/crypto.cpp"

#include "runtime/containers.cpp"
#include "runtime/env_vars.cpp"

#include "runtime/lambda/_entry.cpp"
#include "kernel/syscall_info.cpp"

#include "vm/hardware/disk.cpp"
#include "vm/hardware/io_ports.cpp"
//
#include "kernel/dynamic/commonlibs.cpp"
#include "libs._platformSpecific.cpp"
//
#include "kernel/krnlcfg.cpp"
#include "kernel/syscall_handle.cpp"

#include "runtime/namespaces.cpp"
#include "runtime/pp/directives.cpp"

#include "compiler/asmc.cpp"
#include "compiler/comptins.cpp"

#include "vm/hardware/cpu_cache.cpp"
#include "vm/hardware/absolut.cpp"
#include "runtime/memory_impl.cpp"

namespace newasm
{
    auto* RAM = &newasm::hardware::randAccessMem;
}

struct __global_newasm final
{
    explicit inline __global_newasm() noexcept
    {
        // THIS CODE IS CALLED BEFORE EVERYTHING ELSE!!!
        //std::ios::sync_with_stdio(false);
    }
};

static __global_newasm nG;

#include "runtime/garbage_collector.cpp"
#include "newasm_exec.cpp"

#include "runtime/procline_insert.cpp"
#include "runtime/repl_mode.cpp"

#include "newasm_compexpr.cpp"
#include "shell_tools/prompt.cpp"

#include "shell_tools/user.cpp"
#include "shell_tools/env_control.cpp"
#include "shell_tools/mount.cpp"
#include "newasm_shell.cpp"

#include "runtime/common/chars.h"
#include "runtime/_entry.h"

namespace fs = std::filesystem;

namespace newasm
{
    bool use_std = false;

    namespace vers
    {
        int main()
        {
            // download test
            std::string url = "https://bracesoftware.github.io/web/newasm_server/vers.txt";
            std::string url2 = "https://bracesoftware.github.io/web/newasm_server/runtime.txt";
            std::string output_path = newasm::core::constants::data_folder + newasm::core::constants::separator + newasm::core::constants::temp_vers;
            std::string output_path2 = newasm::core::constants::data_folder + newasm::core::constants::separator + newasm::core::constants::temp_runtime;

            //newasm::header::functions::info("Checking for updates...");
            newasm::utils::loading("Checking for updates...", newasm::utils::load_speed);
            bool checkres1 = newasm::net::download(url, output_path);
            bool checkres2 = newasm::net::download(url2, output_path2);
            bool checkres = checkres1 && checkres2;
            if(checkres)
            {
                std::cout << newasm::header::col::gray;
                newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Accessed the update server..."));
                std::cout << newasm::header::col::reset;
                std::ifstream file(output_path);
                if(!file)
                {
                    std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Error while checking the build version."));
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
                std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Running the latest version of the system!"));
                std::cout << newasm::header::col::reset;

                std::ifstream file2(output_path2);
                if(!file2)
                {
                    std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("Error while checking the runtime version."));
                    std::cout << newasm::header::col::reset;
                    return 1;
                }
                std::string runtimenumber;
                std::getline(file2, runtimenumber);
                if(newasm::RUNTIME_VERSION < std::stoi(newasm::header::functions::trim((runtimenumber))))
                {
                    std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("NewASM Runtime ") + newasm::header::functions::trim((runtimenumber)) + " update is available!");
                    std::cout << newasm::header::col::reset;
                    return 1;
                }
                std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + static_cast<std::string>("The runtime is running on the latest version."));
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
#include "runtime/async_thread.cpp"

namespace newasm
{
    namespace GLOBAL
    {
        void cleanup()
        {
            auto printf___ = [](const char* msg) -> void {
                std::cout << msg << std::flush;
                return;
            };
            //std::cout << "\n";
            newasm::header::functions::log("System cleaning up...");
            std::cout << newasm::header::col::gray;
            newasm::containers::functions::free_dyn_mem();
            printf___("\t\tCleaning up containers...\n");
            newasm::stack::free_macro_mem();
            printf___("\t\tCleaning up macro data...\n");

            printf___("\t\tCleaning up DL data...\n");
            printf___("\t\tCleaning up environment variable memory...\n");
            
            newasm::threads::functions::free_mem();
            printf___("\t\tCleaning up thread data...\n");
            newasm::core::env_vars::functions::save_env();
            printf___("\t\tSaving environment variables...\n");
            
            printf___("\t\tCleaning up dynamic tuple and context data...\n");
            for(auto i = newasm::variables::ids.begin(); i != newasm::variables::ids.end(); ++i)
            {
                if(i->second.type == newasm::datatypes::tuple)
                {
                    if(i->second.tuple != nullptr)
                    {
                        delete i->second.tuple;
                    }
                }
                if(i->second.type == newasm::datatypes::mycontext)
                {
                    if(i->second.context != nullptr)
                    {
                        delete i->second.context;
                    }
                }
            }
            printf___("\t\tCleaning up class data...\n");
            for(auto i = newasm::variables::ids.begin(); i != newasm::variables::ids.end(); ++i)
            {
                if(i->second.type == newasm::datatypes::blueprint)
                {
                    if(i->second.blueprint != nullptr)
                    {
                        delete i->second.blueprint;
                    }
                }
            }
            printf___("\t\tCleaning up union data...\n");
            for(auto i = newasm::variables::ids.begin(); i != newasm::variables::ids.end(); ++i)
            {
                if(i->second.type == newasm::datatypes::yunion)
                {
                    if(i->second.yunion != nullptr)
                    {
                        delete i->second.yunion;
                    }
                }
            }
            printf___("Closing...");
            std::cout << newasm::header::col::reset;
            std::cout << std::endl;
            return;
        }
    }
}

#include "vm/utilities/procfile.cpp"

namespace newasm
{
    int entry(int argc, char* argv[])
    {
        newasm::mem::regs::fdx.make_short(true);
        if constexpr(0) newasm::native_jit::print("Hello from JIT COMPILER!");
        newasm::forLinker__OLD::debug.reserve(100);
        auto get_time = [&]() -> std::string {
            auto now = std::chrono::system_clock::now();
            std::time_t t = std::chrono::system_clock::to_time_t(now);

            std::tm tm;
            #ifdef _WIN32
                localtime_s(&tm, &t);
            #else
                localtime_r(&t, &tm);
            #endif

            // Formatiraj u string
            std::ostringstream oss;
            oss << std::put_time(&tm, "%d/%m/%Y-%H:%M:%S");//<- wtf ovo je nova stvar?!?!
            return oss.str();
        };

        newasm::forLinker__OLD::linked_size = 0;
        fs::create_directory(newasm::data::getDataFolder() + newasm::core::constants::separator + newasm::core::constants::data_folder);
        //fs::create_directory(newasm::data::getDataFolder() + newasm::core::constants::separator + newasm::core::constants::linker);
        #define __LOG_FILE__ (newasm::data::getDataFolder() + newasm::core::constants::separator + newasm::core::constants::data_folder + newasm::core::constants::separator + "__newasm.log")
        std::ofstream __LOG_FILE(__LOG_FILE__, std::ios::app);
        __LOG_FILE << "[" << get_time() << "] System opened.\n";
        __LOG_FILE.close();

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
        newasm::hardware::cpuCache.init();

        fs::path data_folder = fs::path(newasm::core::constants::data_folder);
        fs::path cache_folder = fs::path(newasm::core::constants::data_folder+
            newasm::core::constants::separator+
            newasm::core::constants::cache_folder
        );

        fs::path linker_folder = fs::path(newasm::core::constants::data_folder+
            newasm::core::constants::separator+
            newasm::core::constants::linker
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
        if(!fs::exists(linker_folder))
        {
            fs::create_directories(linker_folder);
        }
        //newasm::_virtual::main();
        newasm::user::main();
        newasm::dyn_ins_set = &newasm::mem::instructions;
        //std::cout << "IDIOTISM" << std::endl;
        newasm::env_vars = &newasm::core::env_vars::priv_env_var;

        /*
        ARGS PROCESSOR
        */
        newasm::header::functions::vers_info(); // main output
        newasm::header::settings::create_new_projfile = true;
        newasm::dwin = true;
        
        std::string args;
        char* args__ = nullptr;
        args__ = std::getenv("newasm_args");
        if(args__ == nullptr) // on Windows
        {
            args__ = std::getenv("newasm_args "); // Allow spaces
        }
        args = args__;
        if(args__ == nullptr)
        {
            EMPTYLINE;
            newasm::header::functions::err("Arguments not provided.\n\tUse the `"+newasm::header::style::underline+
            "newasm_args"+newasm::header::col::reset+"` environment variable.");
            newasm::header::functions::info("Using default values...");
            EMPTYLINE;
            EMPTYLINE;
            args = "h,std,l,nodbg";
        }
        
        newasm::header::functions::trim(args);
        auto arguments = newasm::header::functions::split(args, ',');
        for(int i = 0; i < arguments.size(); ++i)
        {
            newasm::header::functions::trim(arguments[i]);
            if(arguments[i] == newasm::args::help) // help argument
            {
                newasm::header::functions::help_info();
                continue;
            }
            if(arguments[i] == newasm::args::logging) //logging
            {
                newasm::header::settings::logging = true;
                continue;
            }
            if(arguments[i] == newasm::args::nover_check) // nover check
            {
                newasm::vercheck = false;
                continue;
            }
            if(arguments[i] == newasm::args::use_std) // use standard lib
            {
                newasm::header::settings::use_std = true;
                continue;
            }
            if(arguments[i] == newasm::args::nodbg) // disable progwin
            {
                newasm::dwin = false;
                continue;
            }

            newasm::header::functions::err("Invalid environment argument or format: " + newasm::header::col::gray + arguments[i]);
            std::cout << newasm::header::col::reset;
            EMPTYLINE;
            return 1;
        }
        
        auto load_std = []() -> bool {
            if(newasm::header::settings::use_std)
            {
                if(!std::filesystem::exists(newasm::header::constants::std_library))
                {
                    newasm::header::functions::err("Standard library not found.\n\t`" + newasm::header::constants::std_library + "` is missing.");
                    return false;
                }
                newasm::procfile(newasm::header::constants::std_library); // firstly get the stl goin
            }
            return true;
        };

        //shell mode
        if(argc == 1)//(newasm::global::mode == newasm::global::MODE_SHELL)
        {
            EMPTYLINE;
            newasm::header::functions::info("Loading the shell mode...");
            newasm::core::env_vars::functions::setup_env();

            bool result = load_std();
            if(!result)
            {
                return 1;
            }
            EMPTYLINE;

            newasm::header::functions::wait(1500);

            newasm::ctl::main();

            newasm::GLOBAL::cleanup();
            return 1;
        }
        //if excess stuff is provided, fuck it up
        if(argc != 2)
        {
            newasm::header::functions::err("Application usage: " + newasm::header::col::gray + "newasm <filename>");
            std::cout << newasm::header::col::reset;
            EMPTYLINE;
            return 1;
        }

        //if the file is provided, do this
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
        newasm::header::settings::script_file = argv[1];
        newasm::header::functions::trim(newasm::header::settings::script_file);
        newasm::header::settings::script_file_LINKED = linker_folder.string() + newasm::core::constants::separator + newasm::header::settings::script_file;

        newasm::Linker::link(newasm::header::settings::script_file, newasm::header::settings::script_file_LINKED);

        newasm::header::functions::wait(4000);

        if(!std::filesystem::exists(newasm::header::settings::script_file_LINKED))
        {
            newasm::header::functions::err("Cannot open the file: `" + newasm::header::settings::script_file + "`");
            return 1;
        }
        std::cout << std::endl;
        newasm::header::execution_flow::entry_exec = newasm::header::settings::script_file;

        newasm::core::env_vars::functions::setup_env();
        newasm::project_data::impl::setup_proj(newasm::header::settings::script_file);
        newasm::hardware::randAccessMem.init();
        newasm::header::functions::info("Sucessfully allocated 10 MB of memory.");
        newasm::header::functions::info(
            static_cast<std::string>("Preparing to execute: ") + newasm::header::col::yellow +
            newasm::project_data::name + static_cast<std::string>(" ") + newasm::project_data::version
            + newasm::header::col::reset);

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

            newasm::runtime::start_program(newasm::core::constants::progwin);
            std::string text = "Loading...\n\n" + newasm::header::col::yellow + newasm::header::style::bold + "NewASM Debug Window\n" + newasm::header::col::gray + "\tBuilt for the Virtual Machine\n\n" + newasm::header::col::reset;
            newasm::progwin::api::cout(text);
            newasm::progwin::api::flush();
            newasm::header::functions::wait(4000);
        }
        /*
            Executing
        */

        bool result = load_std();
        if(!result)
        {
            return 1;
        }

        newasm::async_thread::entry();

        newasm::Console::show("NewASM Application Window");
        //newasm::Console::out("Ide totalni gas");
        newasm::header::functions::trim(newasm::header::settings::script_file_LINKED);
        newasm::execute(newasm::header::settings::script_file_LINKED, -1);

        /*newasm::procline(".start");
        newasm::procline("mov tlr, \"hello from built-in\"");
        newasm::procline("mov stl, 0c1");
        newasm::procline("mov fdx, 1");
        newasm::procline("sysenter \"ios\"");
        newasm::procline("syscall");*/

        std::chrono::duration<double, std::milli> elapsed = newasm::perf::end - newasm::perf::start;
        //std::chrono::duration<double, std::milli> input_wasted = std::chrono::duration<double, std::milli>::zero();
        std::chrono::duration<double, std::milli> wait_wasted = std::chrono::duration<double, std::milli>::zero();
        std::chrono::duration<double, std::milli> network_wasted = std::chrono::duration<double, std::milli>::zero();

        /*for(int i = 0; i < newasm::runtime_deduction.size(); ++i)
        {
            input_wasted = input_wasted + newasm::runtime_deduction.at(i);
        }*/

        for(int i = 0; i < newasm::wasted_deduction.size(); ++i)
        {
            wait_wasted = wait_wasted + newasm::wasted_deduction.at(i);
        }

        for(int i = 0; i < newasm::network_deduction.size(); ++i)
        {
            network_wasted = network_wasted + newasm::network_deduction.at(i);
        }

        std::cout << newasm::header::col::gray << "\t\tTime elapsed: " << elapsed.count() << " ms\n";
        std::cout << newasm::header::col::gray << "\t\t\t" << newasm::perf::inputWasteTimer.count() << " ms wasted on user input\n";
        std::cout << newasm::header::col::gray << "\t\t\t" << network_wasted.count() << " ms wasted on network latency\n";
        //
        std::cout << newasm::header::style::underline;
        std::cout << newasm::header::col::gray << "\t\t\t" << wait_wasted.count() << " ms wasted on `wait`\n";
        std::cout << newasm::header::col::reset;
        std::cout << newasm::header::col::gray << "\t\t\tTotal: ";
        std::cout << (elapsed.count() - newasm::perf::inputWasteTimer.count() - wait_wasted.count() - network_wasted.count()) << " ms\n";
        std::cout << newasm::header::col::reset;

        auto& dbg = newasm::forLinker__OLD::debug;
        for(int i = 0; i < dbg.size(); ++i)
        {
            newasm::progwin::api::cout(dbg.at(i) + "\n");
        }

        newasm::async_thread::running = false;

        // Print the debug buffer
        newasm::progwin::api::cout("Cleaning up the buffer..............." + newasm::header::col::light_red + "\n\tsys -> ..\n--logout--\n");
        newasm::progwin::api::flush();

        newasm::header::functions::pause();

        newasm::header::functions::info("Cleaning up...");

        newasm::GLOBAL::cleanup();

        newasm::progwin::api::exit();

        __LOG_FILE.open(__LOG_FILE__, std::ios::app);
        __LOG_FILE << "[" << get_time() << "] System exited.\n";
        __LOG_FILE.close();

        newasm::Console::close();
        return 0;
    }
}

 #if 0
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
        #endif

#include "sys_boot__.cpp"

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