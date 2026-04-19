/*

// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

<=================== READ LICENCE.md FOR MORE DETAILS ===================>

*/

#pragma GCC diagnostic ignored "-Wunused-result" // used gcc for ts
#pragma unroll

#define NEWASM_DEBUG 0
namespace newasm
{
    inline void enable_ansi() noexcept;
    inline constinit const int BUILD_NUMBER = 30;
    inline constinit const int RUNTIME_VERSION = 13;
    inline constinit const int KERNEL_VERSION = 8;
}

namespace SYS = newasm;
namespace NewASM = newasm;
namespace llsq = newasm;

link "runtime/common/os";
link "runtime/common/arch";
#pragma GCC optimize ("inline-functions,unroll-loops")
#define EMPTYLINE std::cout<<"\n"
#include <iostream>
#include <initializer_list>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <string_view>
#include <source_location>
#include <stdexcept>
#include <ostream>
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
#include <optional>
//LOl
#include <limits>
#include <variant>
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
fetch link "https://bracesoftware.github.io/web/newasm_server/bcxx_test";
link "runtime/common/chars";
namespace newasm
{
    using BasicFunction = std::function<void()>;
    namespace Modules
    {
        inline std::function<void(const std::string&)> SetError;
        inline std::function<void(const std::string&)> PrintLine;
        inline std::function<void(NewASM::BasicFunction)> SetDestructor;
        typedef std::vector<std::string> ModuleList;
        ModuleList __LIST__;
        inline ModuleList GetLoadedModules() noexcept
        {
            return __LIST__;
        }
    }

    namespace compiler
    {
        struct lineData;
    }
    namespace ExceptionHandling
    {
        newasm::compiler::lineData* Line = nullptr;
    }

    namespace Namespaces
    {
        constinit const short Construction = 1 << 2;
        constinit const short Destruction = 1 << 3;
    }
}
link "vm/impl";
link "runtime/common/attrib";
namespace newasm
{
    namespace CapturedData
    {
        int* ExitCodeInvalidMemacc = nullptr;
    }
    namespace RamChip
    {
        inline std::function<std::string(int)> PeekString = nullptr;
    }

    namespace variables
    {
        struct tupleData;
        struct classData;
        struct unionData;
        struct contextData;
        class procedureData;
        class threadData;
        struct eventData;
        struct staticObjectData;

        struct varData final
        {
            int addr; // address where it is stored
            int type; // type
            tupleData* tuple = nullptr; // if it is a tuple, we use this instead of addr
            classData* blueprint = nullptr; // if it is a class, we use this
            unionData* yunion = nullptr; // if it is an union, we use this
            contextData* context = nullptr; // if it is a context, use this
            procedureData* proc = nullptr; //if it is a proc, use this

            //just for checks
            threadData* thrd = nullptr;
            eventData* event = nullptr;
            staticObjectData* obj = nullptr;

            //decorator data
            bool locked = false;
            bool transient__ = false;
            //attrib
            int attrib = 0;
        };
    }
    typedef std::unordered_map<std::string, newasm::variables::varData> VarTable;
    typedef newasm::variables::varData* VarPtr;
    VarTable* VAR_TABLE_PTR = nullptr;
    VarPtr CurrentProc = nullptr;
    VarPtr CurrentProcA = nullptr;
    VarPtr CurrentClass = nullptr;

    constinit int CYCLE_COUNT = 0;
    std::string CONST__ = NIL_STR;
    std::string& real_line = CONST__;
    std::vector<std::string>* sealedLabels = nullptr;

    constinit bool priRegDeref = false;

    namespace lambda
    {
        constinit bool process = false; // if lambda contents is being processed
    }
    namespace header
    {
        namespace data
        {
            constinit bool repl = false;
            constinit bool LogCompilerOptimizations = true;
        }
    }
    namespace project_data
    {
        std::string name = "";
        std::string version = "";
    }
    namespace datatypes
    {
        const int symbol_name = 0;
        const int number = 1;
        const int decimal = 2;
        const int text = 3;
        const int reference = 4;
        const int character = 5;
        const int tuple = 6;
        const int blueprint = 7;
        const int yunion = 8;
        const int mycontext = 9;
        const int proc = 10;
        
        const int static_objz = 11;
        const int threadz = 12;
        const int event = 100;
        const int _regDeref = 101;
        constinit const int NIL = 102;
        constinit const int tokenOpenBrace = 103;
    }
    namespace runtime
    {
        namespace evalModes
        {
            const short regDeref = 1;
            const short addressOf = 2;
            const short valueOfNamespacedVar = 3;//-> if(newasm::header::functions::parseNamespaceSegments(suf).first)
            const short valueOfNamespacedTupleOrContext = 4; 
            const short referenceOfNamespacedVar = 5;//-> isref
            const short environmentVariable = 6;//-> check */ at the front of a string
            const short valueOf = 7;
            const short sizeOf = 8;
        }
    }

    namespace kernel
    {
        namespace cfg
        {
            bool IOStream = false;
            bool Extensions = false;
            bool Thread = false;
            bool Chrono = false;
            bool Network = false;
            bool Memory = false;
            bool TextOperations = false;
            bool ContainerManipulation = false;
            bool FileStream = false;
            bool FileStream_VDSK = false;
            bool Tuple = false;
            bool TCProtocol = false;
            bool HTTP = false;
            bool Math = false;
            bool Misc = false;
            bool Crypto = false;
            bool Context = false;
        }
    }
}

link "sysext/scope_exit";
link "sysext/out";
link "vm/external";
link "sysext/maps";

link "sysext/csimple";
#define __newasm_included "NEWASM"_str
link "runtime/alpha";
link "sys._platformSpecific";
//<- UNDER THIS ALL MODULES CAN LOAD
module(vm_impl, {
    //empty
});
link "newasm_stdex";
link "taster._platformSpecific";
module(hostos_detect, {});
// thread init
link "kernel/threads/_flags";
link "vm/blueprint/class";
// hardware changes
link "vm/hardware/multiproc";
extern "C"
{
    void free_string(char* str);
}
link "vm/hardware/cpu_register";

// Resources (assets) used in the program
namespace newasm
{
    namespace files
    {
        inline void CD(const std::string& dir);
        inline std::string ResolveName(const std::string& name);
        constinit const std::string GetEmptyDir__C = "..";
    }
    namespace ctl
    {
        namespace data
        {
            std::vector<std::string> path;
        }
    }
    namespace flags
    {
        bool loaded_std = false;
        bool perf_available = false;
    }

    template<bool what>
    inline void execute();

    const bool DO_HANDLE_EXIT = true;
    
    namespace kernel
    {
        constexpr auto makeHash(unsigned short a, unsigned short b) noexcept -> unsigned int {
            return (static_cast<unsigned int>(a) << 16) | b;
            //return (uint32_t(uint16_t(a)) << 16) | uint32_t(uint16_t(b));
        }
    }

    namespace GLOBAL
    {
        typedef bool (*__LOAD_STD)();
        typedef void (*__SHOW_PERF)();
        inline void cleanup();
        __LOAD_STD global_load_std = nullptr;
        __SHOW_PERF global_showPerf = nullptr;

        bool showed_perf = false;
    }

    namespace compiler
    {
        const int empty = 0;
        const int directive = 1;
        const int decorator = 2;
        const int namespace__ = 3;
        const int closingBrace = 4;
        const int macroTerminator = 5;
        const int lambdaTerminator = 6;
        const int sectionModifier = 7;
        const int sealedLabel = 8;//const int handleModifier = 8;
        const int macroDecl = 9;
        const int dataDecl = 10;
        const int macroCall = 11;
        const int conditional = 12;
        const int instruction = 13;
        const int classInstance = 14;
        const int labelJumpPoint = 15;//for optimization
        const int attribute = 16;
    }

    namespace decorators
    {
        constinit const short DESTRUCTIVE = 1;
        constinit const short CONSTRUCTIVE = 2;

        namespace id
        {
            constinit const short TRANSIENT = 1;
            constinit const short VOLATILE = 2;
            constinit const short LOCK = 3;
        }
    }
    //////////EVENTS
    ///exit
    inline void handle_exit();
    bool exit_handled = false;
    bool handling_exit = false;
    std::string exit_handler__;
    ////////////
    namespace constv
    {
        const std::string quote = "\"";
    }

    inline void process_hndl(int tohandle, const std::string& procedure);
    inline void callproc(const std::string& name);
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
            inline void krnl(std::string text);
            //void parseRegDeref(std::string& arg);
            std::pair<bool, int> issizeof(const std::string& str);

            template<bool _Force>
            inline std::string parseBackslash(const std::string& s);
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
        class dataObject final
        {
            using string = std::string;

            private int type = 0;
            private int objInt = 0;
            private float objFloat = 0;
            private char objChar = 0;
            private string objString = "";

            //funcs
            public inline std::variant<int, float, char, std::string> get()
            {
                if(this->type == newasm::datatypes::number)
                {
                    return this->objInt;
                }
                if(this->type == newasm::datatypes::decimal)
                {
                    return this->objFloat;
                }
                if(this->type == newasm::datatypes::character)
                {
                    return this->objChar;
                }
                if(this->type == newasm::datatypes::text)
                {
                    return this->objString;
                }
                return 0;
            }
        };
    }
    namespace compiler
    {
        struct argumentData
        {
            int type;
            int addr;
        };

        struct EvalMode final
        {
            typedef std::string string;

            int type = 0;
            int argType = INVALID_INS;
            signed int argInt = 0;
            float argFloat = 0;
            char argChar = 0;
            string argString = "";
            string argString2 = "";
        };

        struct lineData final
        {
            //some definitions, typedef where we can, using where we must!
            typedef std::string string;
            template<typename T>
            using vec = std::vector<T>;

            //actual code
            string raw;
            int type;
            vec<string> tokens;
            string other;

            int priArgType = 0;
            EvalMode priEvalMode;
            int priInt = 0;
            float priFloat = 0;
            char priChar = 0;
            string priString = "";
            
            int altArgType = 0;
            EvalMode altEvalMode;
            int altInt = 0;
            float altFloat = 0;
            char altChar = 0;
            string altString = "";

            int caseLineArgType = 0;

            //newasm::compiler::argumentData suffixLiteral;
            //newasm::compiler::argumentData operandLiteral;

            int whatAmIDoing = INVALID_INS;
            short parsedType = INVALID_INS;
            short whatCodeSection = INVALID_INS;
            int letsDecorateVariables = INVALID_INS; //kernel'z makeHash returns an int
            short whatTheFuckAreEvents = INVALID_INS;
            int attribute = INVALID_INS;
            int whatAreRegistersLol = INVALID_INS;
            unsigned short krnlMod = INVALID_INS;
            int jumpinTo = INVALID_INS;
            bool VirtualMemoryAccess = false;
            unsigned int caseTableAddress = 0;
            int returninTo = INVALID_INS;
            //for runtime analysis and better error messages
            bool MacroComponent = false;
            string SourceMacroName = "";

            //---------------------------------------------
            //stuff not included in the binary:
            unsigned int resType = 0;
            int resInt = 0;
            float resFloat = 0;
            char resChar = 0;
            string resString = "";
            
            explicit inline lineData() noexcept {}
            ~lineData() noexcept {}

            inline bool operator==(const lineData& rhs) const noexcept
            {
                return (
                    this->tokens == rhs.tokens and
                    this->type == rhs.type and
                    this->whatAmIDoing == rhs.whatAmIDoing and
                    this->whatAreRegistersLol == rhs.whatAreRegistersLol and

                    this->priArgType == rhs.priArgType and
                    this->priEvalMode.type == rhs.priEvalMode.type and
                    this->priInt == rhs.priInt and
                    this->priFloat == rhs.priFloat and
                    this->priChar == rhs.priChar and
                    this->priString == rhs.priString and

                    this->altArgType == rhs.altArgType and
                    this->altEvalMode.type == rhs.altEvalMode.type and
                    this->altInt == rhs.altInt and
                    this->altFloat == rhs.altFloat and
                    this->altChar == rhs.altChar and
                    this->altString == rhs.altString and

                    this->VirtualMemoryAccess == rhs.VirtualMemoryAccess and
                    this->attribute == rhs.attribute and
                    this->whatCodeSection == rhs.whatCodeSection and
                    this->krnlMod == rhs.krnlMod and
                    this->letsDecorateVariables == rhs.letsDecorateVariables and
                    this->whatTheFuckAreEvents == rhs.whatTheFuckAreEvents
                );
            }
        };

        inline std::string parse_def(std::string suf);
        inline void process_comptis(std::string ins, std::string arg1);
        inline void process_comptiso(std::string ins, std::string arg1, std::string arg2);
        inline bool iscomptins(std::string ins);
    }

    namespace runtime
    {
        namespace functions
        {
            inline void eval(std::string& s, newasm::compiler::EvalMode& mode);
        }
    }

    struct rawData final
    {
        typedef std::string string;

        unsigned int rawType = 0;
        signed int rawInt = 0;
        float rawFloat = 0;
        char rawChar = 0;
        string rawString = "";
    };

    using execBytecode = newasm::compiler::lineData;
    newasm::kernel::thread_safe<newasm::compiler::lineData*> PRC;
    
    int procline(newasm::compiler::lineData& line);
    int procline(std::string& text);
    int procline(const char* line);

    typedef void (TokenizeFunc)(std::string str);
    inline TokenizeFunc tokenize;
    namespace impl
    {
        std::string eval(std::string str);
    }

    //--------------------------------------
    bool vercheck = true;
    bool dwin = true;
    std::string bin_out_name = "a.out";
    const std::string tab = "\t\t\t";
    std::unordered_map<std::string,std::vector<std::string>>* dyn_ins_set;
    std::vector<std::pair<std::string,std::string>>* env_vars;

    auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();
    std::vector<std::chrono::duration<double, std::milli>> runtime_deduction;
    std::vector<std::chrono::duration<double, std::milli>> wasted_deduction;
    std::vector<std::chrono::duration<double, std::milli>> network_deduction;

    namespace global
    {
        constinit const int MODE_INT = 0;
        constinit const int MODE_SHELL = 1;
        constinit int mode = newasm::global::MODE_INT;

        std::deque<std::string> event_codeblock;
        constinit bool event_now = false;
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
        constinit const int event_block = 3;
    }
    std::vector<int> brace_stack__;
    //------------------------------------------------------
    class TIMER__
    {
        private std::atomic<bool> running;
        private std::atomic<double> elapsed_ms;
        private std::thread t;

        public explicit inline TIMER__() : running(false), elapsed_ms(0.0) {}

        public inline void start()
        {
            if(running)
            {
                return;
            }

            running = true;
            elapsed_ms = 0.0;

            t = std::thread([this]()
            {
                auto last = std::chrono::steady_clock::now();
                while(running)
                {
                    auto now = std::chrono::steady_clock::now();
                    elapsed_ms += std::chrono::duration<double, std::milli>(now - last).count();
                    last = now;
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
            });
        }

        inline void stop()
        {
            running = false;
            if(t.joinable())
            {
                //std::cout << std::endl;
                t.join();
            }
        }

        inline double count() const
        {
            return elapsed_ms.load();
        }

        inline void clear()
        {
            elapsed_ms = 0.0;
            return;
        }
    };

    class timer
    {
        using clock = std::chrono::steady_clock;
        private clock::time_point start_time;
        private double accumulated_ms = 0.0;
        private bool running = false;

        public void start()
        {
            if(!running) 
            {
                start_time = clock::now();
                running = true;
            }
        }

        public void stop()
        {
            if(running)
            {
                auto end_time = clock::now();
                accumulated_ms += std::chrono::duration<double, std::milli>(end_time - start_time).count();
                running = false;
            }
            return;
        }

        public double count() const
        {
            if(!running)
            {
                return accumulated_ms;
            }
            
            auto current_duration = std::chrono::duration<double, std::milli>(clock::now() - start_time).count();
            return accumulated_ms + current_duration;
        }

        public void clear()
        {
            accumulated_ms = 0.0;
            if(running)
            {
                start_time = clock::now();
                return;
            }
        }
    };

    namespace perf
    {
        auto start = std::chrono::steady_clock::now();
        auto end = std::chrono::steady_clock::now();

        newasm::timer inputWasteTimer;
        newasm::timer heavyHostServices;
    }

    void* test = nullptr;

    class forLinker__OLD
    {
        public static inline newasm::_std::linear_map<int, std::string> files;
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
    using lineSource = std::pair<std::string, int>;
    class forLinker
    {
        public static inline std::vector<lineSource> lineData;

        public static inline std::string getFile(int line)
        {
            if(line < 0)
            {
                return lineData.front().first;
            }
            return lineData.at(line).first;
        }

        public static inline int getLine(int lastlinedx)
        {
            if(lastlinedx < 0)
            {
                return lineData.front().second;
            }
            return lineData.at(lastlinedx).second;
        }
    };
}

link "vm/ConsoleEmu";

namespace newasm
{
    namespace events
    {
        bool parsing_now = false;
        int current = INVALID_INS;
        struct Handler final
        {
            std::vector<int> addr;
        };
        //exit event
        newasm::events::Handler exitHandler;
        constinit const int exitId = 1;
        bool exitNow = false;
        //other events

        const std::unordered_map<std::string, int> eventNames = {
            {"'termination'", newasm::events::exitId}
        };
    }
}

/*
Essential stuff needed to run
is in the runtime
*/

link "runtime/lang_inf";
link "runtime/handlers";

link "runtime/progwin_api";
link "utils._platformSpecific";

link "runtime/utils";
link "runtime/common/opcodes";

link "newasm_dynlib";
link "newasm_header";
link "toolchain/compiler/native_jit";
link "toolchain/linker/asmlink";

link "runtime/common/tokenize";
link "newasm_setup";
link "runtime/virtual";

link "runtime/expcfg/decorators";
link "kernel/threads/impl";

link "kernel/system_calls/io_stream";
link "kernel/system_calls/file_stream";
link "kernel/system_calls/exec_flow";
link "kernel/system_calls/c_manip";
link "kernel/system_calls/text_operations";
link "kernel/system_calls/net";
link "kernel/system_calls/mem";
link "kernel/system_calls/chrono";
link "kernel/system_calls/tuple";
link "kernel/system_calls/tcp";
link "kernel/system_calls/http";
link "kernel/system_calls/misc";
link "kernel/system_calls/crypto";

link "runtime/containers";
link "runtime/env_vars";

link "runtime/lambda/_entry";
link "kernel/syscall_info";

link "vm/hardware/disk";
link "kernel/drivers/vmfs";
link "vm/hardware/io_ports";
//
link "kernel/dynamic/commonlibs";
link "libs._platformSpecific";
//
link "vm/hardware/cpu_cache";
link "vm/hardware/absolut";
link "kernel/syscall_handle";

link "runtime/namespaces";
link "runtime/pp/directives";

link "toolchain/compiler/asmc";
link "toolchain/compiler/comptins";

//link "runtime/memory_impl";

namespace newasm
{
    auto* RAM = &newasm::hardware::randAccessMem;
    using RamPointer = decltype(RAM);
}

struct __global_newasm final
{
    public explicit inline __global_newasm() noexcept
    {
        // THIS CODE IS CALLED BEFORE EVERYTHING ELSE!!!
        //std::ios::sync_with_stdio(false);
    }
};

static __global_newasm nG;

link "runtime/garbage_collector";
link "toolchain/compiler/bin";
link "newasm_exec";
link "runtime/repl_mode";

link "newasm_compexpr";
link "shell_tools/prompt";

link "shell_tools/user";
link "shell_tools/env_control";
link "shell_tools/mount";
link "shell_tools/files";
link "newasm_shell";

link "runtime/_entry";

namespace fs = std::filesystem;

namespace newasm
{
    constinit bool use_std = false;

    namespace vers
    {
        inline int main()
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
                newasm::header::functions::nullprint(newasm::tab + "Accessed the update server..."_str);
                std::cout << newasm::header::col::reset;
                std::ifstream file(output_path);
                if(!file)
                {
                    std::cout << newasm::header::col::gray;
                    newasm::header::functions::nullprint(newasm::tab + "Error while checking the build version."_str);
                    std::cout << newasm::header::col::reset;
                    return 1;
                }
                std::string vernumber;
                std::getline(file, vernumber);
                if(newasm::BUILD_NUMBER < std::stoi(newasm::header::functions::trim((vernumber))))
                {
                    std::cout << newasm::header::col::gray;
                    newasm::header::functions::nullprint(newasm::tab + "Latest build "_str + newasm::header::functions::trim((vernumber)) + " is available!"_str);
                    std::cout << newasm::header::col::reset;
                    return 1;
                }
                std::cout << newasm::header::col::gray;
                newasm::header::functions::nullprint(newasm::tab + "Running the latest version of the system!"_str);
                std::cout << newasm::header::col::reset;

                std::ifstream file2(output_path2);
                if(!file2)
                {
                    std::cout << newasm::header::col::gray;
                    newasm::header::functions::nullprint(newasm::tab + "Error while checking the runtime version."_str);
                    std::cout << newasm::header::col::reset;
                    return 1;
                }
                std::string runtimenumber;
                std::getline(file2, runtimenumber);
                if(newasm::RUNTIME_VERSION < std::stoi(newasm::header::functions::trim((runtimenumber))))
                {
                    std::cout << newasm::header::col::gray;
                    newasm::header::functions::nullprint(newasm::tab + "NewASM Runtime "_str + newasm::header::functions::trim((runtimenumber)) + " update is available!"_str);
                    std::cout << newasm::header::col::reset;
                    return 1;
                }
                std::cout << newasm::header::col::gray;
                newasm::header::functions::nullprint(newasm::tab + "The runtime is running on the latest version."_str);
                std::cout << newasm::header::col::reset;
                return 1;
            }
            std::cout << newasm::header::col::gray;
            newasm::header::functions::nullprint(newasm::tab + "Cannot access the update server!"_str);
            std::cout << newasm::header::col::reset;
            return 1;
        }
    }
}
// MAIN
//#define NEWASM_STRICT_TEST

link "kernel/_utils";
link "runtime/async_thread";

namespace newasm
{
    namespace GLOBAL
    {
        inline void cleanup()
        {
            NewASM::compiler::data::MacroTable.clear();
            NewASM::header::data::CallCStack->clear();
            newasm::RAM->set_sot(sizeof(int));
            NewASM::header::data::EnableThreads = true;
            newasm::header::data::offlineMode = false;
            newasm::exit_handled = false;
            newasm::lambda::GLOBAL.reset_forKernel();
            newasm::header::flags::autobos = false;
            newasm::GLOBAL::showed_perf = false;
            newasm::header::data::exception = true;
            newasm::stack::events.clear();

            newasm::compiler::caseJumpTable.clear();

            newasm::events::exitHandler.addr.clear();

            newasm::mem::instructions.clear();
            newasm::flags::loaded_std = false;

            newasm::nms::stack.clear();
            newasm::nms::count = 0;

            newasm::kernel::cfg::IOStream = false;
            newasm::kernel::cfg::Extensions = false;
            newasm::kernel::cfg::Thread = false;
            newasm::kernel::cfg::Chrono = false;
            newasm::kernel::cfg::Network = false;
            newasm::kernel::cfg::Memory = false;
            newasm::kernel::cfg::TextOperations = false;
            newasm::kernel::cfg::ContainerManipulation = false;
            newasm::kernel::cfg::FileStream = false;
            newasm::kernel::cfg::Tuple = false;
            newasm::kernel::cfg::TCProtocol = false;
            newasm::kernel::cfg::HTTP = false;
            newasm::kernel::cfg::Math = false;
            newasm::kernel::cfg::Misc = false;
            newasm::kernel::cfg::Crypto = false;
            newasm::kernel::cfg::Context = false;
            NewASM::kernel::cfg::FileStream_VDSK = false;

            newasm::mem::datatypes.clear();
            newasm::mem::data.clear();
            newasm::mem::data_attrib.clear();

            newasm::mem::structs.clear();
            //newasm::mem::funcs.clear();

            newasm::containers::functions::free_dyn_mem();
            newasm::stack::free_macro_mem();

            newasm::threads::functions::free_mem();
            newasm::core::env_vars::functions::save_env();

            newasm::compiler::data::sealed_labels.clear();

            //reset attributes after each program
            newasm::runtime::currentAttributes = 0;
         
            for(auto i = newasm::variables::ids.begin(); i != newasm::variables::ids.end(); ++i)
            {
                if(i->second.type == newasm::datatypes::event)
                {
                    if(i->second.event != nullptr)
                    {
                        delete i->second.event;
                    }
                }
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
                if(i->second.type == newasm::datatypes::blueprint)
                {
                    if(i->second.blueprint != nullptr)
                    {
                        delete i->second.blueprint;
                    }
                }
                if(i->second.type == newasm::datatypes::static_objz)
                {
                    if(i->second.obj != nullptr)
                    {
                        delete i->second.obj;
                    }
                }
                if(i->second.type == newasm::datatypes::yunion)
                {
                    if(i->second.yunion != nullptr)
                    {
                        delete i->second.yunion;
                    }
                }
                if(i->second.type == newasm::datatypes::proc)
                {
                    if(i->second.proc != nullptr)
                    {
                        delete i->second.proc;
                    }
                }
                if(i->second.type == newasm::datatypes::threadz)
                {
                    if(i->second.thrd != nullptr)
                    {
                        delete i->second.thrd;
                    }
                }
            }
            newasm::variables::ids.clear();

            newasm::system::terminated = false;

            newasm::expcfg::transientbool = false;
            newasm::expcfg::volatilebool = false;
            newasm::expcfg::lockbool = false;

            //cleanup compiler data for REPL in shell
            newasm::compiler::data::symbol_map.clear();
            newasm::compiler::data::namespace_stack.clear();
            newasm::compiler::data::sealed_labels.clear();

            NewASM::header::data::ActiveThreads = 0;

            NewASM::CurrentProc = nullptr;
            NewASM::CurrentClass = nullptr;
            return;
        }
    }
}

link "vm/utilities/procfile";

module(main_module, {
    NewASM::CapturedData::ExitCodeInvalidMemacc = const_cast<decltype(NewASM::CapturedData::ExitCodeInvalidMemacc)>(&newasm::exit_codes::invalid_memacc);
    newasm::mem::labels.max_load_factor(MAX_LOAD_FACTOR);
    NewASM::header::data::LogCompilerOptimizations = true;
    NewASM::header::data::CallCStack->reserve(1000);
    return;
});
namespace newasm
{
    inline signed int entry(signed int argc, char* argv[])
    {
        NewASM::RamChip::PeekString = [](int addr) -> std::string {
            return NewASM::RAM->peek<std::string>(addr);
        };
        NewASM::VAR_TABLE_PTR = &newasm::variables::ids;
        auto s = 69_str;
        //newasm::real_line.reserve(500);
        newasm::mem::regs::fdx.make_short(true);
        newasm::mem::regs::fdx.log_things(false);
        newasm::variables::ids.reserve(1000); // for funsies
        newasm::variables::ids.max_load_factor(MAX_LOAD_FACTOR);
        NewASM::stack::macros.max_load_factor(MAX_LOAD_FACTOR);
        
        newasm::execBytecode test = newasm::compiler::DO("mov tlr, 3"_str.string());
        
        if constexpr(0) newasm::native_jit::print("Hello from JIT COMPILER!");
        newasm::forLinker__OLD::debug.reserve(100);
        auto get_time = [&]() -> std::string {
            auto now = std::chrono::system_clock::now();
            std::time_t t = std::chrono::system_clock::to_time_t(now);

            std::tm tm;
            #if _NEWASM_OS == _NEWASM_OS_windows or _NEWASM_OS == _NEWASM_OS_windows_old
                localtime_s(&tm, &t);
            #elif _NEWASM_OS == _NEWASM_OS_linux
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
        if(args__)
        {
            args = args__;
        }
        if(args__ == nullptr)
        {
            EMPTYLINE;
            newasm::header::functions::err("Arguments not provided.\n\tUse the `" + newasm::header::style::underline +
                "newasm_args" + newasm::header::col::reset + "` environment variable.");
            newasm::header::functions::info("Using default values...\t| 'h,l,nodbg'");
            EMPTYLINE;
            EMPTYLINE;
            args = "h,l,nodbg";
        }
        
        newasm::header::functions::trim(args);
        auto arguments = newasm::header::functions::split(args, ',');
        std::string value;
        auto parseArgValue = [&](std::string& string) -> void {
            auto eqsign_pos = string.find('=');
            if(eqsign_pos != std::string::npos)
            {
                auto temp = newasm::header::functions::split_fixed(string, '=');
                string = newasm::header::functions::trim(temp[0]);
                value = newasm::header::functions::trim(temp[1]);
                return;
            }
            value.clear();
            return;
        };
        for(int i = 0; i < arguments.size(); ++i)
        {
            newasm::header::functions::trim(arguments[i]);
            parseArgValue(arguments[i]);
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
            if(arguments[i] == newasm::args::nolco) // disable logging of compiler optimizations
            {
                NewASM::header::data::LogCompilerOptimizations = false;
                continue;
            }
            if(arguments[i] == newasm::args::out) // disable progwin
            {
                if(value.empty())
                {
                    newasm::header::functions::wrn("Using standard binary name: " + newasm::bin_out_name);
                    continue;
                }
                newasm::bin_out_name = value;
                value.clear();
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
                if(newasm::flags::loaded_std)
                {
                    return false;
                }
                if(!std::filesystem::exists(newasm::header::constants::std_library))
                {
                    newasm::header::functions::err("Standard library not found.\n\t`" + newasm::header::constants::std_library + "` is missing.");
                    return false;
                }
                newasm::procfile(newasm::header::constants::std_library); // firstly get the stl goin
                newasm::flags::loaded_std = true;
            }
            return true;
        };

        newasm::GLOBAL::global_load_std = load_std;

        auto ver_check = []() -> void {
            if(newasm::vercheck)
            {
                newasm::vers::main();
                if(std::filesystem::exists(newasm::core::constants::data_folder + newasm::core::constants::separator + newasm::core::constants::temp_vers))
                {
                    std::filesystem::remove(newasm::core::constants::data_folder + newasm::core::constants::separator + newasm::core::constants::temp_vers);
                }
            }
        };

        auto showPerf = []() -> void {
            if(!newasm::flags::perf_available)
            {
                newasm::header::functions::err("No performance information is available.");
                return;
            }
            newasm::flags::perf_available = false;
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

            std::cout << newasm::header::col::gray << "  Profiler info for: " << newasm::header::col::yellow << newasm::project_data::name << " " << newasm::project_data::version << std::endl;
            std::cout << newasm::header::col::gray << "\t\tTime elapsed: " << elapsed.count() << " ms\n";
            std::cout << newasm::header::col::gray << "\t\t\t" << newasm::perf::inputWasteTimer.count() << " ms wasted on user input\n";
            std::cout << newasm::header::col::gray << "\t\t\t" << newasm::perf::heavyHostServices.count() << " ms used on heavy host services\n";
            std::cout << newasm::header::col::gray << "\t\t\t" << network_wasted.count() << " ms wasted on network latency\n";
            //
            std::cout << newasm::header::style::underline;
            std::cout << newasm::header::col::gray << "\t\t\t" << wait_wasted.count() << " ms wasted on `wait`\n";
            std::cout << newasm::header::col::reset;
            std::cout << newasm::header::col::gray << "\t\t\tTotal: ";
            std::cout << (elapsed.count() - newasm::perf::heavyHostServices.count() - newasm::perf::inputWasteTimer.count() - wait_wasted.count() - network_wasted.count()) << " ms\n";
            std::cout << newasm::header::col::gray << "\t\t\tCycle count: " << newasm::CYCLE_COUNT << '\n';
            std::cout << newasm::header::col::reset;
            return;
        };

        newasm::GLOBAL::global_showPerf = showPerf;

        //shell mode
        if(argc == 1)//(newasm::global::mode == newasm::global::MODE_SHELL)
        {
            ver_check();
            EMPTYLINE;
            newasm::header::functions::wait(2000);
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
        NewASM::header::functions::log("System loading...");

        ver_check();
        NewASM::flags::perf_available = true;

        EMPTYLINE;
        NewASM::header::settings::script_file = argv[1];
        NewASM::header::functions::trim(NewASM::header::settings::script_file);
        NewASM::header::settings::script_file_LINKED = linker_folder.string() + NewASM::core::constants::separator + NewASM::header::settings::script_file;

        NewASM::Linker::link(NewASM::header::settings::script_file, NewASM::header::settings::script_file_LINKED);

        NewASM::header::functions::wait(4000);

        if(!std::filesystem::exists(NewASM::header::settings::script_file_LINKED))
        {
            NewASM::header::functions::err("Cannot open the file: `" + NewASM::header::settings::script_file + "`");
            return 1;
        }
        EMPTYLINE;
        NewASM::header::execution_flow::entry_exec = NewASM::header::settings::script_file;

        NewASM::core::env_vars::functions::setup_env();
        NewASM::project_data::impl::setup_proj(NewASM::header::settings::script_file);
        NewASM::hardware::randAccessMem.init();
        EMPTYLINE;
        NewASM::header::functions::finfo("Sucessfully allocated %i MiB of random access memory.", __newasm_MEMORY_SIZE);
        NewASM::hardware::Disk.init();
        NewASM::header::functions::finfo("Sucessfully allocated %i MiB of virtual disk space.", __newasm_DISK_SIZE);
 
        NewASM::header::functions::finfo("Preparing to execute: %s%s %s%s",
            NewASM::header::col::yellow,
            NewASM::project_data::name,
            NewASM::project_data::version,
            NewASM::header::col::reset
        );

        EMPTYLINE;

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
        newasm::async_thread::entry();

        newasm::Console::show("NewASM Application Window");
        //newasm::Console::out("Ide totalni gas");
        newasm::header::functions::trim(newasm::header::settings::script_file_LINKED);
        newasm::compile_and_exec(newasm::header::settings::script_file_LINKED, -1);

        EMPTYLINE;
        newasm::header::functions::info(newasm::constv::pxstr + std::to_string(newasm::mem::regs::exc));

        /*newasm::procline(".start");
        newasm::procline("mov tlr, \"hello from built-in\"");
        newasm::procline("mov stl, 0c1");
        newasm::procline("mov fdx, 1");
        newasm::procline("sysenter \"ios\"");
        newasm::procline("syscall");*/

        newasm::GLOBAL::global_showPerf();

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

        newasm::header::functions::info("Cleaning up memory...");

        newasm::GLOBAL::cleanup();
        std::cout << newasm::header::col::gray;
        newasm::header::functions::nullprint("\n\tShutting down...");
        std::cout << newasm::header::col::reset;
        std::cout << std::endl;

        newasm::progwin::api::exit();

        __LOG_FILE.open(__LOG_FILE__, std::ios::app);
        __LOG_FILE << "[" << get_time() << "] System exited.\n";
        __LOG_FILE.close();

        newasm::Console::close();
        //newasm::header::functions::wait(2000);
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

link "sys_boot__";

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