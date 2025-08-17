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

namespace newasm
{
    namespace hardware
    {
        template<int memsize> class randAccessMem__
        {
            private:
            static constexpr int MEM_SIZE = memsize * 1024 * 1024;
            char __memory__[MEM_SIZE];
            newasm::containers::bit_array<MEM_SIZE> __memory_free__;
            public:
            int get_free_alloc(int bytes)
            {
                for(int i = 0; i <= MEM_SIZE - bytes; ++i)
                {
                    bool free_block = true;
                    if(__memory_free__.get_at(i) == 0)
                    {
                        for(int k = i; k < i + bytes; ++k)
                        {
                            if(__memory_free__.get_at(k) != 0)
                            {
                                free_block = false;
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

            void init()
            {
                for(int i = 0; i < MEM_SIZE; ++i)
                {
                    __memory_free__.set_at(i, 0);
                }
                return;
            }
            // for int, float, char
            template<typename T> int write(T value)
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
                return address;
            }

            template<typename T> int overwrite(int addr, T value)
            {
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
                // We're modifying an existing memory block, thus we just have to modify it
                std::memcpy(&__memory__[addr], &value, sizeof(T));
                return addr;
            }

            template<typename T> T peek(int addr)
            {
                if constexpr(std::is_same<T, std::string>::value)
                {
                    int buffer_len = 0;
                    std::memcpy(&buffer_len, &__memory__[addr], sizeof(int));
                    std::string buffer(buffer_len, '\0');
                    std::memcpy(buffer.data(), &__memory__[addr + sizeof(int)], buffer_len);
                    return buffer;
                }
                T value;
                std::memcpy(&value, &__memory__[addr], sizeof(T));
                return value;
            }
        };

        newasm::hardware::randAccessMem__<10> randAccessMem; // 10 MB OF MEMORY :D
    }
}