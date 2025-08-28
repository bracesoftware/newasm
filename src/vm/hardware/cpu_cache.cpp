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
        #define CACHE_SIZE (t_cachesize * 1024)
        template<int t_cachesize, int t_linesize>
        class CPU_CACHE__ final
        {
            private:
            //constexpr int cache_size__ = t_cachesize * 1024;
            mutable unsigned char __cache__[CACHE_SIZE]; // KILOBYTES
            mutable unsigned char __temp__[t_linesize];

            newasm::containers::bit_array<CACHE_SIZE> free_cache__;
            // free = 0
            // occupied = 1
            public:
            inline void init() noexcept
            {
                //eh
                return;
            }

            inline bool find(int addr, char data[])
            {
                int temp;
                for(int i = 0; i <= CACHE_SIZE - t_linesize; i = i + t_linesize)
                {
                    if(free_cache__.get_at(i) == 0)
                    {
                        continue;
                    }
                    std::memcpy(&temp, &__cache__[i], sizeof(int));
                    if(temp == addr)
                    {
                        std::memcpy(data, &__cache__[i], t_linesize);
                        return true;
                    }
                }
                return false;
            }

            inline void add(int addr, int data)
            {
                for(int i = 0; i <= CACHE_SIZE - t_linesize; i = i + t_linesize)
                {
                    if(free_cache__.get_at(i) == 0)
                    {
                        std::memcpy(&__cache__[i], &addr, sizeof(int));
                        std::memcpy(&__cache__[i + sizeof(int)], &data, sizeof(int));
                        free_cache__.set_at(i, 1);
                        return;
                    }
                }
            }

            inline void remove(int addr)
            {
                int temp;
                for(int i = 0; i <= CACHE_SIZE - t_linesize; i = i + t_linesize)
                {
                    if(free_cache__.get_at(i) == 1)
                    {
                        std::memcpy(&temp, &__cache__[i], sizeof(int));
                        if(temp == addr)
                        {
                            free_cache__.set_at(i, 0);
                            return;
                        }
                    }
                }
                return;
            }
        };

        newasm::hardware::CPU_CACHE__<2, 8> cpu_cache;
    }
}