// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

module(virtual_cpu_cache, {
    //setup goes here
});

namespace newasm
{
    namespace hardware
    {
        template<typename _Type>
        concept _CachableType = (
            std::is_same_v<_Type, int> or
            std::is_same_v<_Type, char> or
            std::is_same_v<_Type, float> or
            std::is_same_v<_Type, std::string> // so ram is happy
        );

        template<int t_cachesize>
        class CPU_CACHE__ final
        {
            private constexpr static int invalid_address = (-1);
            private constexpr static int __newasm_LINEBYTES_ = __newasm_LINEBYTES;
            
            struct __line__ final
            {
                int addr;
                char value[4];
            }; // 8 bytes per line

            private mutable __line__ __cache__[t_cachesize];

            // A block of RAM memory links to a specific cache line index
            // Doesn't require linear search, and thus cache is way faster than RAM,
            // cuz we have a direct memory access

            // Amount of addresses that map to a single cache line is __newasm_LINEBYTES
            private constexpr inline int __minusIPlus(int address) noexcept
            {
                // a simple mathematical algorithm
                return (
                    (((((address + 1) + __newasm_LINEBYTES_) % __newasm_LINEBYTES_) == 0 ? (address + 1) : 
                    (address + 1) + (__newasm_LINEBYTES_ - (((address + 1) + __newasm_LINEBYTES_) % __newasm_LINEBYTES_)))
                    / __newasm_LINEBYTES_) - 1
                );
            }

            public constexpr inline void init() noexcept
            {
                for(int i = 0; i < t_cachesize; ++i)
                {
                    __cache__[i].addr = invalid_address;
                }
                return;
            }

            @nodiscard
            public inline void* find_addr(int address) noexcept
            {
                if(__cache__[__minusIPlus(address)].addr == address)
                {
                    return (void*)__cache__[__minusIPlus(address)].value;
                }
                return nullptr;
            }

            template<newasm::hardware::_CachableType T>
            inline void cache_addr(int address, T value) noexcept
            {
                T temp;

                __cache__[__minusIPlus(address)].addr = address;
                
                temp = value;
                std::memcpy(&__cache__[__minusIPlus(address)].value, &temp, sizeof(T));
                return;
            }

            explicit inline CPU_CACHE__() noexcept
            {
                this->init();
            }
            
            inline ~CPU_CACHE__() noexcept {}
        };

        newasm::hardware::CPU_CACHE__<__newasm_CACHE_LINES> cpuCache;
    }
}