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
    namespace _std
    {
        template<typename T>
        concept _WriteableType = (
            std::is_same_v<T, char*> or
            std::is_same_v<T, const char*> or
            std::is_same_v<T, std::string> or
            std::is_same_v<T, char> or
            std::is_same_v<T, int> or
            std::is_same_v<T, short> or
            std::is_same_v<T, long> or
            std::is_same_v<T, long long> or
            std::is_same_v<T, unsigned int> or
            std::is_same_v<T, unsigned short> or
            std::is_same_v<T, unsigned long> or
            std::is_same_v<T, unsigned long long> or
            std::is_same_v<T, unsigned char> or
            std::is_same_v<T, bool> or
            std::is_same_v<T, float> or
            std::is_same_v<T, double>
        );

        template<_WriteableType T>
        inline void write(const T& what) noexcept
        {
            if constexpr(std::is_integral_v<T>)
            {
                std::printf("%d", what);
            }
            else if constexpr(std::is_floating_point_v<T>)
            {
                std::printf("%f", what);
            }
            else if constexpr(std::is_same_v<T, const char*> or
            std::is_same_v<T, char*>)
            {
                std::printf("%s", what);
            }
            else if constexpr(std::is_same_v<T, std::string>)
            {
                std::printf("%s", what.c_str());
            }
            else
            {
                static_assert(!sizeof(T*), "UNSUPPORTED");
            }
            return;
        }
    }
}

#define WRITE(text) newasm::_std::write(text)