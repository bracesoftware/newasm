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
#define NIL_STR "nil"
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
#define __newasm_CHECK_JUMP_PROPERLY if(newasm::header::data::repl and not newasm::header::data::proc_now){newasm::unsins(ins);return 1;}
#define __newasm_LOAD_PACKAGE_MODULE(name, func)        struct \
    __gMOD_INIT_##name final{\
    explicit inline __gMOD_INIT_##name() noexcept{\
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

#define __NEWASM_DUMMY 0
#define NEWASM_JUMP_POINT "__NEWASM_COMPILER_JUMP_POINT"

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