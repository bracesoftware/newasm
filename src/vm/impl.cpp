// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#define __newasm_MEMORY_SIZE 10 // MiB
#define __newasm_DISK_SIZE 20 // MiB
#define __newasm_CACHE_LINES 1024 //8KiB since line is 8bytes

#define __newasm_MAX_FILES 64
#define __newasm_MAX_FILENAME_LEN 128

// funcs
#define __newasm_B_TO_KB(val)   ((val) / 1024.0)
#define __newasm_B_TO_MB(val)   ((val) / (1024.0 * 1024.0))
#define __newasm_B_TO_GB(val)   ((val) / (1024.0 * 1024.0 * 1024.0))

#define __newasm_KB_TO_B(val)   ((val) * 1024)
#define __newasm_MB_TO_B(val)   ((val) * 1024 * 1024)
#define __newasm_GB_TO_B(val)   ((val) * 1024 * 1024 * 1024)

#define __newasm_KB_TO_MB(val)  ((val) / 1024.0)
#define __newasm_MB_TO_KB(val)  ((val) * 1024)

#define __newasm_MB_TO_GB(val)  ((val) / 1024.0)
#define __newasm_GB_TO_MB(val)  ((val) * 1024)

//stock

#define STOCK__ [[maybe_unused]]
#ifndef stock
    #define stock STOCK__
#endif

// other funcs
stock constexpr inline void _newasm_rem__24234() noexcept
{
    return;
}

#define __newasm_rem(comment)   _newasm_rem__24234()

// other bs
#define STR(x) #x
#define CONCAT(a, b) a##b
#define NIL_STR "nil"_str
#define NULL_STR "0"_str
#define OPEN_BRACE_STR "{"_str
#define INVALID_INS (-1)
#define INS_EXTERNAL (-2)
#define MAX_LOAD_FACTOR (0.5f)

static_assert(sizeof(int) == 4);
static_assert(sizeof(float) == 4);
static_assert(sizeof(char) == 1);

static_assert(__newasm_MB_TO_B(__newasm_MEMORY_SIZE) % __newasm_CACHE_LINES == 0);
#define __newasm_LINEBYTES (__newasm_MB_TO_B(__newasm_MEMORY_SIZE) / __newasm_CACHE_LINES)

// real bs
namespace newasm
{
    inline void __test__() noexcept
    {
        struct test
        {
            std::string buf;
            std::vector<int> gg;

            inline test() {}

            inline ~test() noexcept {}
        };

        void* raw = std::malloc(sizeof(test));
        test* myobj = new(raw) test();

        std::cout << "giga chad C daddy" << std::endl;

        myobj->~test();
        std::free(raw);
        return;
    }
    
    inline namespace foo
    {
        //inlinegg
    }
    namespace foo
    {
        //gg
    }
}

#if 0
namespace newasm
{
    class Utils
    {
        public:
        template<typename T>
        inline static void print(T what)
        {
            std::cout << what;
            return;
        }

        template<typename T>
        inline static void println(T what)
        {
            std::cout << what << std::endl;
            return;
        }
    };
}
#endif

#if NEWASM_DEBUG == 1
    #define __newasmDBG(f__) f__;
#elif NEWASM_DEBUG == 0
    #define __newasmDBG(f__)
#endif

#if NEWASM_DEBUG == 1
    #define __newasmDBG_COMPLEX(f__) struct\
        __newasm_COMPLEX_DEBUG##__LINE__ final {\
            explicit inline __newasm_COMPLEX_DEBUG##__LINE__()f__\
        };__newasm_COMPLEX_DEBUG##__LINE__ _NEWASM_COMPLEX_DEBUG##__LINE__
#elif NEWASM_DEBUG == 0
    #define __newasmDBG_COMPLEX(f__)
#endif
namespace newasm
{
    struct NullBuffer__ : std::streambuf //so we can mute unnecessary printing in module loading 
    {
        int overflow(int c) override
        {
            //im actually not using ts
            return c;
        }
    };
}
int __newasm__MODULEID = 1;
bool __newasm_GLOBAL_MODULE = true;
#define __newasm_CHECK_JUMP_PROPERLY if(newasm::header::data::repl and not newasm::header::data::proc_now){newasm::unsins(ins);return 1;}
#define __newasm_LOAD_PACKAGE_MODULE(name, func)        struct \
    __gMOD_INIT_##name final{\
    inline ~__gMOD_INIT_##name(){if(!__newasm_GLOBAL_MODULE)NewASM::Modules::__LIST__.pop_back();--__newasm__MODULEID;}\
    explicit inline __gMOD_INIT_##name() noexcept{\
        NewASM::Modules::__LIST__.push_back(std::string(#name));\
        using namespace std;vector<string> __NEWASM_BUFFER;\
        bool __NEWASM_MODULE_ERR=false;std::string __NEWASM_ERRTEXT;NewASM::BasicFunction __NEWASM_DESTRUCTOR;auto __err__NEWASM = [&](const std::string& errtext)->\
        void {__NEWASM_MODULE_ERR=true;__NEWASM_ERRTEXT=__NEWASM_ERRTEXT+std::string(" ")+errtext;return;};\
        auto __printline_NEWASM = [&](const std::string& text)->void{__NEWASM_BUFFER.push_back(text);return;};\
        auto __setdestruct_NEWASM = [&](NewASM::BasicFunction f)->void{__NEWASM_DESTRUCTOR=f;};\
        NewASM::Modules::SetError=__err__NEWASM;NewASM::Modules::PrintLine=__printline_NEWASM;\
        NewASM::Modules::SetDestructor=__setdestruct_NEWASM;\
        auto __##name = [&]() -> void {func};\
        std::cout << newasm::header::col::red << "[  Service " << __newasm__MODULEID << "  ]: " << newasm::header::col::gray << \
        "Virtual machine is setting up module `" << #name << "`... ";\
        NewASM::NullBuffer__ nil;std::streambuf* old_buf = std::cout.rdbuf(&nil);\
        __NEWASM_DESTRUCTOR=[]()->void{};__##name();std::cout.rdbuf(old_buf);\
        if(!__NEWASM_MODULE_ERR)cout << NewASM::header::col::green << "OK!\n"<<flush;\
        if(__NEWASM_MODULE_ERR)cout<<NewASM::header::col::magenta<< "ERROR!\n"<<flush;\
        if(__NEWASM_MODULE_ERR)cout<<NewASM::header::col::red<<"\terror message: "<<NewASM::header::col::gray<<__NEWASM_ERRTEXT<<endl;\
        for(int i=0; i < __NEWASM_BUFFER.size(); ++i)cout<<NewASM::header::col::light_red<<"\tlog: "<< NewASM::header::col::gray<<__NEWASM_BUFFER.at(i)<<endl;\
        cout<<flush<< newasm::header::col::reset;__newasm__MODULEID++;__NEWASM_DESTRUCTOR();\
    }\
};static __gMOD_INIT_##name NEWASM__MODULE__##name
#define module __newasm_LOAD_PACKAGE_MODULE

#define MAIN_FUNC_ARGS_SIG int argc, char** argv
#define MAIN_FUNC_ARGS_CALL argc, argv
int NEXT_MAIN_MODULES(MAIN_FUNC_ARGS_SIG);
int main(MAIN_FUNC_ARGS_SIG)
{
    __newasm_GLOBAL_MODULE = false;
    NEXT_MAIN_MODULES(MAIN_FUNC_ARGS_CALL);
    return 0;
}
#undef main
#define main NEXT_MAIN_MODULES

#define __NEWASM_DUMMY 0
#define NEWASM_JUMP_POINT "__NEWASM_COMPILER_JUMP_POINT"_str

struct __ final{
    public explicit inline __() {
        newasm::enable_ansi();
    }
};
__ ___;

//some compiler tweaks
#if defined(__GNUC__) || defined(__clang__)//they best buddies
    #define ATTR_HOT __attribute__((hot))
    #define FORCE_INLINE __attribute__((always_inline))
#elif defined(_MSC_VER)
    // MSVC nema hot atribut, oslanja se na PGO
    #define ATTR_HOT 
    #define FORCE_INLINE __forceinline
#else
    #define ATTR_HOT
    #define FORCE_INLINE
#endif

#if defined(__GNUC__) || defined(__clang__)
    #define ATTR_FLAT __attribute__((flatten))
#elif defined(_MSC_VER)
    #define ATTR_FLAT
#else
    #define ATTR_FLAT
#endif

class One
{
    public inline void hi() noexcept
    {
        std::cout << "One::hi() called" << std::endl;
        return;
    }
};

class Two
{
    public inline void hi() noexcept
    {
        std::cout << "Two::hi() called" << std::endl;
        return;
    }
};

class Three
{
    public inline void bye() noexcept
    {
        return;
    }
};

template<class T>
concept Numbers = (
    std::is_same_v<T, One> or
    std::is_same_v<T, Two>
);

template<class T>
concept NumbersHasMethod = requires(T v)
{
    {
        v.hi() //we check if T has T::hi()
    } noexcept;
};

template<class T>
concept ValidNumber = Numbers<T> and NumbersHasMethod<T>;

template<ValidNumber T> //works if we just do "typename T" or "Number T"
class ConstructorTest
{
    public T obj;
    explicit inline ConstructorTest() noexcept
    {
        obj.hi();//<-without ValidNumber concept, it just ASSUMES the class has the hi method
                //without the compiler warning us whatsoever
    }
};
namespace newasm
{
    namespace __
    {
        int main()
        {
            //C++ HAS TO BE JOKEEEE
            ConstructorTest<Two> omg;
            return 0;
        }
    }
}

#define NEWASM_BASIC_FUNCTION_SIG []()->void

//best thing i invented
namespace newasm
{
    //now i use these for temporary string literals (for printing text, comparsion, etc.)
    class SmartString final
    {
        public const char* data;
        std::size_t len;

        public inline SmartString(const char* d, std::size_t l) noexcept
            : data(d), len(l) {}

        //c++ searches fitting func the linear way :O
        FORCE_INLINE inline operator std::string_view() const
        { 
            return {data, len}; 
        }

        FORCE_INLINE inline operator std::string() const
        { 
            return {data, len}; 
        }
        FORCE_INLINE inline std::string string() const
        {
            return std::string(data, len);
        }
        //comparsion
        FORCE_INLINE friend inline bool operator==(const std::string& lhs, const SmartString& rhs)
        {
            return std::string_view(lhs) == std::string_view(rhs.data, rhs.len);
        }
        FORCE_INLINE friend inline bool operator!=(const std::string& lhs, const SmartString& rhs)
        {
            return !(lhs == rhs);
        }
        FORCE_INLINE friend inline bool operator==(const SmartString& lhs, const std::string& rhs)
        {
            return std::string_view(lhs.data, lhs.len) == std::string_view(rhs);
        }
        FORCE_INLINE friend inline bool operator!=(const SmartString& lhs, const std::string& rhs)
        {
            return !(lhs == rhs);
        }

        FORCE_INLINE friend inline bool operator==(const SmartString& lhs, const char* rhs)
        {
            return std::string_view(lhs.data, lhs.len) == std::string_view(rhs);
        }
        FORCE_INLINE friend inline bool operator!=(const SmartString& lhs, const char* rhs)
        {
            return !(lhs == rhs);
        }
        FORCE_INLINE friend inline bool operator==(const char* lhs, const SmartString& rhs)
        {
            return std::string_view(lhs) == std::string_view(rhs.data, rhs.len);
        }
        FORCE_INLINE friend inline bool operator!=(const char* lhs, const SmartString& rhs)
        {
            return !(lhs == rhs);
        }
        //math
        FORCE_INLINE friend inline std::string operator+(const std::string& lhs, const SmartString& rhs)
        {
            std::string res = lhs;
            res.append(rhs.data, rhs.len);
            return res;
        }
        FORCE_INLINE friend inline std::string operator+(const SmartString& lhs, const std::string& rhs)
        {
            std::string res;
            res.reserve(lhs.len + rhs.size());
            res.append(lhs.data, lhs.len);
            res.append(rhs);
            return res;
        }
        FORCE_INLINE friend inline std::string operator+(const SmartString& lhs, const SmartString& rhs)
        {
            std::string res;
            res.append(lhs.string());
            res.append(rhs.string());
            return res;
        }
        FORCE_INLINE friend inline std::string operator+(const char* lhs, const SmartString& rhs)
        {
            std::string res(lhs);
            res.append(rhs.data, rhs.len);
            return res;
        }
        FORCE_INLINE friend inline std::string operator+(const SmartString& lhs, const char* rhs)
        {
            std::string res;
            res.append(lhs.data, lhs.len);
            res.append(rhs);
            return res;
        }
    };

    FORCE_INLINE inline NewASM::SmartString operator ""_str(const char* str, std::size_t len)
    {
        return NewASM::SmartString(str, len);
    }

    FORCE_INLINE inline SmartString operator ""_str(char c)
    {
        static thread_local char buf[2] = {0, 0};
        buf[0] = c;
        return SmartString(buf, 1);
    }

    template<typename T>
    concept _NonStringTypes = (
        std::is_same_v<T, int> or
        std::is_same_v<T, float> or
        std::is_same_v<T, char> or
        std::is_same_v<T, long> or
        std::is_same_v<T, short> or
        std::is_same_v<T, double> or
        std::is_same_v<T, unsigned long long> or
        std::is_same_v<T, long double>
    );

    struct __str {};
    inline constexpr __str _strV;

    template<_NonStringTypes T>
    FORCE_INLINE inline SmartString ToSmart(T t)
    {
        static thread_local std::string static_t;
        static_t = std::to_string(t);
        return SmartString(static_t.c_str(), static_t.size());
    }

    FORCE_INLINE inline SmartString operator ""_str(unsigned long long n)
    {
        return ToSmart(n);
    }

    FORCE_INLINE inline SmartString operator ""_str(long double d)
    {
        return ToSmart(d);
    }

    template<_NonStringTypes T>
    FORCE_INLINE inline SmartString operator|(T t, __str)
    {
        return ToSmart(t);
    }
}

#define NEWASM_BROKEN_ACTIVE_THREAD_COUNTER false