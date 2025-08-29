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
        template<typename _Type>
        concept _CachableType = (
            std::is_same_v<_Type, int> or
            std::is_same_v<_Type, char> or
            std::is_same_v<_Type, float>
        );

        template<int t_cachesize>
        class CPU_CACHE__ final
        {
            private:
            const int invalid_address = (-1);
            struct __line__
            {
                int addr;
                char value[4];
            }; // 8 bytes per line

            mutable __line__ __cache__[t_cachesize];

            public:
            inline void init() noexcept
            {
                for(int i = 0; i < t_cachesize; ++i)
                {
                    __cache__[i].addr = invalid_address;
                }
                return;
            }

            template<newasm::hardware::_CachableType T>
            inline void* find(int address)
            {
                for(int i = 0; i < t_cachesize; ++i)
                {
                    if(__cache__[i].addr == address)
                    {
                        return (void*)__cache__[i].value;
                    }
                }
                return nullptr;
            }

            template<newasm::hardware::_CachableType T>
            inline void add(int address, T value)
            {
                T temp;
                for(int i = 0; i < t_cachesize; ++i)
                {
                    if(__cache__[i].addr != invalid_address)
                    {
                        __cache__[i].addr = address;
                        temp = value;
                        std::memcpy(&__cache__[i].value, &temp, sizeof(T));
                        return;
                    }
                }
                return;
            }

            inline void remove(int address)
            {
                for(int i = 0; i < t_cachesize; ++i)
                {
                    if(__cache__[i].addr == address)
                    {
                        __cache__[i].addr = invalid_address;
                        return;
                    }
                }
                return;
            }
        };

        newasm::hardware::CPU_CACHE__<1024> cpu_cache;
    }
}