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
            int getFreeMemIdx_(int bytes)
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
                int address = getFreeMemIdx_(sizeof(T));
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

            template<typename T> void overwrite(int addr, T value)
            {
                // We're modifying an existing memory block, thus we just have to modify it
                std::memcpy(&__memory__[addr], &value, sizeof(T));
                return;
            }

            template<typename T> T peek(int addr)
            {
                T value;
                std::memcpy(&value, &__memory__[addr], sizeof(T));
                return value;
            }

            // for strings
            // coming soon
        };

        newasm::hardware::randAccessMem__<10> randAccessMem; // 10 MB OF MEMORY :D
    }
}