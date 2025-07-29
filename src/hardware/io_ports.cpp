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
        template<typename T> class IOPort
        {
            private:
                int address;
                T value;
            public:
            IOPort(int addr, T val)
            : address(addr), value(val){}

            int get_addr() const
            {
                return this->address;
            }
            T get_value() const
            {
                return this->value;
            }
            void set_value(T val)
            {
                this->value = val;
            }
        };
        namespace IO_ports
        {
            newasm::hardware::IOPort<int> txtcol(1, 0);
        }

        void outIOPOrt(int id, int signal) // write to port
        {
            if(id == newasm::hardware::IO_ports::txtcol.get_addr()) // text color on screen
            {
                auto i = newasm::hardware::IO_ports::txtcol;
                i.set_value(id);
                if(i.get_value() == 1) std::cout << newasm::header::col::red;
                if(i.get_value() == 2) std::cout << newasm::header::col::yellow;
                if(i.get_value() == 3) std::cout << newasm::header::col::green;
                if(i.get_value() == 4) std::cout << newasm::header::col::blue;
                if(i.get_value() == 5) std::cout << newasm::header::col::magenta;
                if(i.get_value() == 6) std::cout << newasm::header::col::cyan;
                if(i.get_value() == 7) std::cout << newasm::header::col::gray;
                if(i.get_value() == 8) std::cout << newasm::header::col::reset;
                return;
            }
            return;
        }
        std::string inIOPort(int id) // read from port
        {
            std::string output;
            if(id == newasm::hardware::IO_ports::txtcol.get_addr())
            {
                output = std::to_string(newasm::hardware::IO_ports::txtcol.get_value());
            }
            return output;
        }
    }
}