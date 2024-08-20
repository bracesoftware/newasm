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

#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

#define __newasm__integer_bits (sizeof(int)*8)


#include <iostream>

namespace newasm
{
    namespace containers
    {
        template<const unsigned int t_bitarraysize> class bit_array
        {
        private:
            int bitarraysize = t_bitarraysize;
            int bitarrayvalue[(t_bitarraysize/(__newasm__integer_bits))+1];
        public:
            bit_array()
            {
                //std::cout << "Created a bitarray with a size of " << sizeof(bitarrayvalue)/sizeof(int) << std::endl;
                if(!(0 < this->bitarraysize))
                {
                    this->bitarraysize = 32;
                }
                for(int i = 0; i < sizeof(bitarrayvalue)/sizeof(int); i++)
                {
                    bitarrayvalue[i] = 0;
                }
            }
            int get_size()
            {
                return this->bitarraysize;
            }
            void clear()
            {
                for(int i; i < sizeof(bitarrayvalue); i++)
                {
                    bitarrayvalue[i] = 0;
                }
            }
            int get_at(int index)
            {
                if(index >= this->bitarraysize)
                {
                    throw (index);
                    return 0;
                }
                return (bitarrayvalue[index/(__newasm__integer_bits)]) & (1 << (index % (__newasm__integer_bits))) ? 1 : 0;
            }
            int set_at(int index, int value)
            {
                if(value != 0 && value != 1)
                {
                    throw (value);
                    return 1;
                }
                if(!(0 <= index < this->bitarraysize))
                {
                    throw(index);
                    return 1;
                }

                int arridx = index/(__newasm__integer_bits);
                int bitidx = index % (__newasm__integer_bits);

                bitarrayvalue[arridx] = value ?
                    bitarrayvalue[arridx] | (1 << bitidx) :
                    bitarrayvalue[arridx] & ~(1 << bitidx);
                return 0;
            }
            int flip()
            {
                for(int i = 0; i < this->get_size(); i++)
                {
                    if(this->get_at(i) == 1)
                    {
                        this->set_at(i, 0);
                    }
                    else
                    {
                        this->set_at(i, 1);
                    }
                }
                return 0;
            }
            int reverse()
            {
                newasm::containers::bit_array<t_bitarraysize> temporary_bitarray;
                for(int i = 0; i < this->bitarraysize; i++)
                {
                    temporary_bitarray.set_at(i, this->get_at(i));
                }

                for(int i = 0, j = this->bitarraysize - 1; i < this->bitarraysize; i++, j--)
                {
                    this->set_at(i, temporary_bitarray.get_at(j));
                }
                return 0;
            }
        };
    }
}