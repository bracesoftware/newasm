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

#define __newasm_MEMORY_SIZE 10 // mb
#define __newasm_DISK_SIZE 20 // mb
#define __newasm_CACHE_LINES 1024 //8KB since line is 8bytes

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

#if defined(__GNUC__) || defined(__clang__)
    #define STOCK__ [[maybe_unused]] inline
#else
    #define STOCK__ [[maybe_unused]]
#endif

// other funcs
STOCK__ constexpr inline void _newasm_rem__24234() noexcept
{
    return;
}

#define __newasm_rem(comment)   _newasm_rem__24234()

// other bs
#define STR(x) #x
#define CONCAT(a, b) a##b
#define NIL_STR "nil"

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