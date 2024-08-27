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
        class binary_tree__
        {
            virtual int get_at(int index)
            {
                return -1;
            }
            virtual void set_at_parent_of(int index, int value)
            {
                return;
            }
            virtual void set_at_left_child_of(int index, int value)
            {
                return;
            }
            virtual void set_at_right_child_of(int index, int value)
            {
                return;
            }
        };
        template<int t_bintreesize> class binary_tree : public binary_tree__
        {
            private:
                int contents[t_bintreesize];
            public:
                int get_at(int index) override
                {
                    if(index >= t_bintreesize)
                    {
                        return -1;
                    }
                    if(index == 0)
                    {
                        return -1;
                    }
                    return contents[index];
                }
                void set_at_parent_of(int index, int value) override
                {
                    if(index == 0 || index == 1)
                    {
                        return;
                    }
                    if(index >= t_bintreesize)
                    {
                        return;
                    }
                    contents[index % 2 == 0 ? index / 2 : (index - 1) / 2] = value;
                    return;
                }
                void set_at_left_child_of(int index, int value) override
                {
                    if(index * 2 >= t_bintreesize)
                    {
                        return;
                    }
                    contents[index * 2] = value;
                    return;
                }
                void set_at_right_child_of(int index, int value) override
                {
                    if(index * 2 + 1 >= t_bintreesize)
                    {
                        return;
                    }
                    contents[index * 2 + 1] = value;
                    return;
                }
        };
        class bit_array__
        {
            public:
                virtual int get_size();
                virtual void clear();
                virtual int get_at(int index);
                virtual int set_at(int index, int value);
                virtual int flip();
                virtual int reverse();
        };

        int bit_array__::get_size()
        {
            return 1;
        }
        void bit_array__::clear()
        {
            return;
        }
        int bit_array__::get_at(int index)
        {
            return index;
        }
        int bit_array__::set_at(int index, int value)
        {
            return value;
        }
        int bit_array__::flip()
        {
            return 0;
        }
        int bit_array__::reverse()
        {
            return 0;
        }

        template<int t_bitarraysize> class bit_array : public bit_array__
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
            int get_size() override
            {
                return this->bitarraysize;
            }
            void clear() override
            {
                for(int i = 0; i < sizeof(bitarrayvalue); i++)
                {
                    bitarrayvalue[i] = 0;
                }
            }
            int get_at(int index) override
            {
                if(index >= this->bitarraysize)
                {
                    throw (index);
                    return 0;
                }
                return (bitarrayvalue[index/(__newasm__integer_bits)]) & (1 << (index % (__newasm__integer_bits))) ? 1 : 0;
            }
            int set_at(int index, int value) override
            {
                if(value != 0 && value != 1)
                {
                    throw (value);
                    return 1;
                }
                if(!(0 <= index && index < this->bitarraysize))
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
            int flip() override
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
            int reverse() override
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

        std::unordered_map<std::string, newasm::containers::bit_array__*> bit_arrays;

        namespace functions
        {
            void free_dyn_mem()
            {
                for(std::unordered_map<std::string, newasm::containers::bit_array__*>::iterator i = newasm::containers::bit_arrays.begin(); i != newasm::containers::bit_arrays.end(); i++)
                {
                    delete i->second;
                }
                return;
            }
        }
    }
}