// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

__newasm_LOAD_PACKAGE_MODULE(absolut_exe, {
    //setup goes here
});

namespace newasm
{
    namespace hardware
    {
        template<int memsize>
        class randAccessMem__ final
        {
            public static constexpr int MEM_SIZE = memsize * 1024 * 1024;
            public unsigned char __memory__[MEM_SIZE];
            public newasm::containers::bit_array<MEM_SIZE> __memory_free__;
            
            public explicit inline randAccessMem__() noexcept {}

            inline int get_size() noexcept
            {
                return MEM_SIZE;
            }

            // free = 0
            // occupied = 1

            @nodiscard
            public inline bool is_free(int addr) noexcept
            {
                return !(__memory_free__.get_at(addr));
            }

            inline int get_free_alloc(int bytes) noexcept
            {
                for(int i = 0; i <= MEM_SIZE - bytes; ++i)
                {
                    if(i % newasm::header::data::alignment != 0)
                    {
                        continue;
                    }
                    bool free_block = true;
                    if(__memory_free__.get_at(i) == 0)
                    {
                        for(int k = i; k < i + bytes; ++k)
                        {
                            if(__memory_free__.get_at(k) != 0)
                            {
                                free_block = false;
                                i = k;
                                break;
                            }
                        }
                        if(free_block)
                        {
                            return i;
                        }
                    }
                }
                return -1;
            }

            inline void delete__HEAP(int addr, int stopaddr) noexcept
            {
                for(int i = addr; i < stopaddr; ++i)
                {
                    __memory_free__.set_at(i, 0);
                }
                return;
            }

            inline int get_heap_end() noexcept
            {
                for(int i = MEM_SIZE - 1; i >= 0; i--)
                {
                    if(i >= newasm::mem::regs::stk.get_value())
                    {
                        continue;
                    }
                    if(__memory_free__.get_at(i) == 1)
                    {
                        return i + 1;
                    }
                }
                return -1;
            }

            inline void init() noexcept
            {
                for(int i = 0; i < MEM_SIZE; ++i)
                {
                    __memory_free__.set_at(i, 0);
                }

                newasm::mem::regs::hea.set_value(0);
                newasm::mem::regs::stk.set_value(MEM_SIZE - 1);
                return;
            }
            // for int, float, char
            template<typename T>
            public inline int write(T value) noexcept
            {
                if constexpr(std::is_same<T, std::string>::value)
                {
                    auto len = value.size();
                    int address = get_free_alloc(sizeof(int) + len);
                    if(address == -1)
                    {
                        newasm::terminate(newasm::exit_codes::mem_overflow);
                        return -1;
                    }
                    // Firstly write the header
                    std::memcpy(&__memory__[address], &len, sizeof(int));
                    // Then the value
                    std::memcpy(&__memory__[address + sizeof(int)], value.data(), len);

                    for(int i = address; i < address + sizeof(int) + len; ++i)
                    {
                        __memory_free__.set_at(i, 1); // tell the thing it is occupied
                    }

                    //newasm::mem::regs::hea.set_value(get_heap_end());
                    return address;
                }

                int address = get_free_alloc(sizeof(T));
                if(address == -1)
                {
                    newasm::terminate(newasm::exit_codes::mem_overflow);
                    return -1;
                }
                std::memcpy(&__memory__[address], &value, sizeof(T));
                for(int i = address; i < address + sizeof(T); ++i)
                {
                    __memory_free__.set_at(i, 1); // tell the thing it is occupied
                }

                if(!newasm::expcfg::volatilebool)
                {
                    newasm::hardware::cpuCache.cache_addr<T>(address, value);
                }
                //newasm::mem::regs::hea.set_value(get_heap_end());
                return address;
            }

            template<typename T>
            public inline int write(int address, T value) noexcept
            {
                if constexpr(std::is_same<T, std::string>::value)
                {
                    auto len = value.size();
                    // Firstly write the header
                    std::memcpy(&__memory__[address], &len, sizeof(int));
                    // Then the value
                    std::memcpy(&__memory__[address + sizeof(int)], value.data(), len);

                    for(int i = address; i < address + sizeof(int) + len; ++i)
                    {
                        __memory_free__.set_at(i, 1); // tell the thing it is occupied
                    }

                    //newasm::mem::regs::hea.set_value(get_heap_end());

                    return address;
                }

                std::memcpy(&__memory__[address], &value, sizeof(T));
                for(int i = address; i < address + sizeof(T); ++i)
                {
                    __memory_free__.set_at(i, 1); // tell the thing it is occupied
                }
                //newasm::mem::regs::hea.set_value(get_heap_end());

                if(!newasm::expcfg::volatilebool)
                {
                    newasm::hardware::cpuCache.cache_addr<T>(address, value);
                }
                return address;
            }

            template<typename T>
            public inline int overwrite(int addr, T value) noexcept
            {
                //newasm::progwin::api::cout("Overwriting address: " + std::to_string(addr));
                if(__memory_free__.get_at(addr) == 0)
                {
                    newasm::terminate(newasm::exit_codes::seg_fault);
                    return addr;
                }
                if constexpr(std::is_same<T, std::string>::value)
                {
                    int buffer_len = 0;
                    std::memcpy(&buffer_len, &__memory__[addr], sizeof(int));

                    if(value.size() > buffer_len) // we check if the new value is smaller
                    {
                        for(int i = addr; i < addr + sizeof(int) + buffer_len; ++i) // free the memory
                        {
                            __memory_free__.set_at(i, 0);
                        }

                        // do not duplicate the code and just write the new thing
                        return write<std::string>(value);
                    }
                    if(value.size() == buffer_len)
                    {
                        // if the new string is the same size as the old string,
                        // just overwrite it as we would with fixed-size data
                        std::memcpy(&__memory__[addr + sizeof(int)], value.data(), buffer_len);
                    }
                    if(value.size() < buffer_len)
                    {
                        // if the new string is smaller, then we want to free unused data
                        int buf_size = static_cast<int>(value.size());
                        std::memcpy(&__memory__[addr], &buf_size, sizeof(int));
                        std::memcpy(&__memory__[addr + sizeof(int)], value.data(), value.size());

                        for(int i = addr + sizeof(int) + value.size(); i < addr + sizeof(int) + buffer_len; ++i)
                        {
                            __memory_free__.set_at(i, 0); // free unused stuff
                        }
                    }
                    return addr;
                }
                // We're modifying an existing memory block, thus we just have to change it
                std::memcpy(&__memory__[addr], &value, sizeof(T));
                if(!newasm::expcfg::volatilebool)
                {
                    newasm::hardware::cpuCache.cache_addr<T>(addr, value);
                }
                return addr;
            }

            template<typename T>
            public inline T peek(int addr) noexcept
            {
                if constexpr(std::is_same<T, std::string>::value)
                {
                    int buffer_len = 0;
                    std::memcpy(&buffer_len, &__memory__[addr], sizeof(int));
                    std::string buffer(buffer_len, '\0');
                    std::memcpy(buffer.data(), &__memory__[addr + sizeof(int)], buffer_len);
                    return buffer;
                }

                if(!newasm::expcfg::volatilebool)
                {
                    void* cache = newasm::hardware::cpuCache.find_addr(addr);
                    if(cache != nullptr)
                    {
                        T value;
                        std::memcpy(&value, cache, sizeof(T));
                        return value;
                    }
                }

                T value;
                std::memcpy(&value, &__memory__[addr], sizeof(T));
                return value;
            }

            //malloc, free, heap, push, pop
            public inline int malloc(int bytes) noexcept
            {
                int addr = get_free_alloc(sizeof(int) + bytes);
                if(addr == -1)
                {
                    return -1;
                }
                int header = bytes;
                // copy the header
                std::memcpy(&__memory__[addr], &header, sizeof(int));
                // now actually fill the memory
                for(int i = addr; i < addr + sizeof(int) + bytes; ++i)
                {
                    __memory_free__.set_at(i, 1);
                }

                //newasm::mem::regs::hea.set_value(get_heap_end());
                return addr;
            }

            public inline void free(int addr) noexcept
            {
                int bytes = 0;
                std::memcpy(&bytes, &__memory__[addr], sizeof(int));
                for(int i = addr; i < addr + sizeof(int) + bytes; ++i)
                {
                    // just free
                    __memory_free__.set_at(i, 0);
                }

                //newasm::mem::regs::hea.set_value(get_heap_end());
                return;
            }

            template<typename T>
            public inline void push__STACK(T value) noexcept
            {
                if constexpr(std::is_same<T, std::string>::value)
                {
                    auto len = value.size();
                    int address = newasm::mem::regs::stk.get_value() - sizeof(int) - len;
                    newasm::mem::regs::stk.set_value(address);
                    // Firstly write the header
                    std::memcpy(&__memory__[address], &len, sizeof(int));
                    // Then the value
                    std::memcpy(&__memory__[address + sizeof(int)], value.data(), len);

                    for(int i = address; i < address + sizeof(int) + len; ++i)
                    {
                        __memory_free__.set_at(i, 1); // tell the thing it is occupied
                    }

                    newasm::malloc::types[address] = newasm::datatypes::text; // initialize metadata

                    return;
                }

                int address = newasm::mem::regs::stk.get_value() - sizeof(T);
                newasm::mem::regs::stk.set_value(address);

                std::memcpy(&__memory__[address], &value, sizeof(T));
                for(int i = address; i < address + sizeof(T); ++i)
                {
                    __memory_free__.set_at(i, 1); // tell the thing it is occupied
                }

                if constexpr(std::is_same<T, int>::value)
                {
                    newasm::malloc::types[address] = newasm::datatypes::number;
                }
                if constexpr(std::is_same<T, float>::value)
                {
                    newasm::malloc::types[address] = newasm::datatypes::decimal;
                }
                if constexpr(std::is_same<T, char>::value)
                {
                    newasm::malloc::types[address] = newasm::datatypes::character;
                }
                return;
            }

            template<typename T>
            public inline int pop__STACK(T& value) noexcept
            {
                if constexpr(std::is_same<T, std::string>::value)
                {
                    int buffer_len = 0;
                    int address = newasm::mem::regs::stk.get_value();
                    std::memcpy(&buffer_len, &__memory__[address], sizeof(int));
                    std::string buffer(buffer_len, '\0');
                    std::memcpy(buffer.data(), &__memory__[address + sizeof(int)], buffer_len);
                    value = buffer;

                    for(int i = newasm::mem::regs::stk; i < newasm::mem::regs::stk + sizeof(int) + buffer_len; ++i)
                    {
                        __memory_free__.set_at(i, 0);
                    }

                    newasm::mem::regs::stk.set_value(newasm::mem::regs::stk.get_value() + sizeof(int) + buffer_len);
                    return address;
                }

                T val;
                int address = newasm::mem::regs::stk.get_value();
                std::memcpy(&val, &__memory__[address], sizeof(T));
                value = val;

                for(int i = newasm::mem::regs::stk; i < newasm::mem::regs::stk + sizeof(T); ++i)
                {
                    __memory_free__.set_at(i, 0);
                }

                newasm::mem::regs::stk.set_value(newasm::mem::regs::stk.get_value() + sizeof(T));
                return address;
            }
            template<typename T>
            public inline int pop__STACK() noexcept
            {
                T value;
                return pop__STACK<T>(value);
            }
            //

            public inline auto get__A(int addr) noexcept
            {
                return &this->__memory__[addr];
            }
        };

        newasm::hardware::randAccessMem__<__newasm_MEMORY_SIZE> randAccessMem; // 10 MB OF MEMORY :D
    }
}