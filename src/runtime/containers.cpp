// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

__newasm_LOAD_PACKAGE_MODULE(containers_api, {
    //setup goes here
});

#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

#define __newasm__integer_bits (sizeof(int)*8)

namespace newasm
{
    namespace containers
    {
        constexpr int default_size = 512;

        template<int t_bintreesize>
        class binary_tree final
        {
            private int contents[t_bintreesize];
            public inline int get_at(int index)
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
            inline void set_at_parent_of(int index, int value)
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
            inline void set_at_left_child_of(int index, int value)
            {
                if(index * 2 >= t_bintreesize)
                {
                    return;
                }
                contents[index * 2] = value;
                return;
            }
            inline void set_at_right_child_of(int index, int value)
            {
                if(index * 2 + 1 >= t_bintreesize)
                {
                    return;
                }
                contents[index * 2 + 1] = value;
                return;
            }
            inline void set_at__(int index, int value)
            {
                this->contents[index] = value;
                return;
            }
        };

        template<int t_bitarraysize>
        class bit_array final
        {
            private:
            int bitarraysize = t_bitarraysize;
            int bitarrayvalue[(t_bitarraysize/(__newasm__integer_bits)) + 1];
            public explicit inline bit_array()
            {
                //std::cout << "Created a bitarray with a size of " << sizeof(bitarrayvalue)/sizeof(int) << std::endl;
                if(!(0 < this->bitarraysize))
                {
                    this->bitarraysize = 32;
                }
                for(int i = 0; i < sizeof(bitarrayvalue)/sizeof(int); ++i)
                {
                    bitarrayvalue[i] = 0;
                }
            }

            inline int get_size() noexcept
            {
                return this->bitarraysize;
            }

            inline void clear() noexcept
            {
                #if 0
                for(int i = 0; i < sizeof(bitarrayvalue); i++)
                {
                    bitarrayvalue[i] = 0;
                }
                #endif
                std::memset(bitarrayvalue, 0, sizeof(bitarrayvalue));

                return;
            }

            inline int get_at(int index)
            {
                if(index >= this->bitarraysize)
                {
                    throw (index);
                    return 0;
                }
                return (bitarrayvalue[index/(__newasm__integer_bits)]) & (1 << (index % (__newasm__integer_bits))) ? 1 : 0;
            }

            inline int set_at(int index, int value)
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

            inline int flip() noexcept
            {
                for(int i = 0; i < this->get_size(); ++i)
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

            inline int reverse() noexcept
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

        class thread_channel__ final
        {
            public:
            bool empty = true;
            std::string data;
        };

        std::unordered_map<std::string, newasm::containers::bit_array<newasm::containers::default_size>*> bit_arrays;
        std::unordered_map<std::string, newasm::containers::binary_tree<newasm::containers::default_size>*> binary_trees;
        std::unordered_map<std::string, newasm::containers::thread_channel__*> thread_channels;

        namespace functions
        {
            inline void free_dyn_mem()
            {
                #if 0
                if(bit_arrays.empty())
                {
                    return;
                }
                if(binary_trees.empty())
                {
                    return;
                }
                if(thread_channels.empty())
                {
                    return;
                }
                #endif
                for(
                    std::unordered_map<std::string, newasm::containers::bit_array<newasm::containers::default_size>*>::iterator i = newasm::containers::bit_arrays.begin();
                    i != newasm::containers::bit_arrays.end(); ++i
                )
                {
                    if(i->second != nullptr)
                    {
                        delete i->second;
                    }
                }
                for(
                    std::unordered_map<std::string, newasm::containers::binary_tree<newasm::containers::default_size>*>::iterator i = newasm::containers::binary_trees.begin();
                    i != newasm::containers::binary_trees.end(); ++i
                )
                {
                    if(i->second != nullptr)
                    {
                        delete i->second;
                    }
                }
                for(
                    std::unordered_map<std::string, newasm::containers::thread_channel__*>::iterator i = newasm::containers::thread_channels.begin();
                    i != newasm::containers::thread_channels.end(); ++i
                )
                {
                    if(i->second != nullptr)
                    {
                        delete i->second;
                    }
                }

                newasm::containers::bit_arrays.clear();
                newasm::containers::binary_trees.clear();
                newasm::containers::thread_channels.clear();
                return;
            }
        }
    }
}