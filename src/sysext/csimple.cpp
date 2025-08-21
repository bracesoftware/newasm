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
    namespace csimple
    {
        void* memcpy(void* dest, const void* source, int bytes)
        {
            if(bytes < 0)
            {
                throw std::invalid_argument("newasm::csimple::memcpy: Size `bytes` is negative.");
                return nullptr;
            }
            unsigned char* dst = (unsigned char*)dest;
            const unsigned char* src = (const unsigned char*)source;

            for(int i = 0; i < bytes; ++i)
            {
                *dst = *src;
                dst++;
                src++;
            }
            return dest;
        }
    }
}