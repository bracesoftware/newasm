// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm
{
    namespace hardware
    {
        template<typename T>
        class IOPort final
        {
            private:
            mutable int address;
            mutable T value;

            public:
            IOPort(int addr, T val)
            : address(addr), value(val){}

            inline int get_addr() const
            {
                return this->address;
            }
            inline T get_value() const
            {
                return this->value;
            }
            inline void set_value(T val)
            {
                this->value = val;
            }
        };
        namespace IO_ports
        {
            //screen
            newasm::hardware::IOPort<int> txtcol(1, 0);
            // disk
            newasm::hardware::IOPort<int> dskfmat(10, 0); 
            newasm::hardware::IOPort<int> dskw(11, 0);
            newasm::hardware::IOPort<int> dskr(12, 0);
        }

        inline void outIOPOrt(int id) // write to port
        {
            if(id == newasm::hardware::IO_ports::txtcol.get_addr()) // text color on screen
            {
                auto& i = newasm::hardware::IO_ports::txtcol;
                if(!newasm::header::functions::isnumeric(newasm::mem::regs::tlr.get_value()))
                {
                    i.set_value(0);
                    return;
                }
                int signal = std::stoi(newasm::mem::regs::tlr.get_value());
                i.set_value(signal);
                if(i.get_value() == 1) std::cout << newasm::header::col::red;
                if(i.get_value() == 2) std::cout << newasm::header::col::yellow;
                if(i.get_value() == 3) std::cout << newasm::header::col::green;
                if(i.get_value() == 4) std::cout << newasm::header::col::blue;
                if(i.get_value() == 5) std::cout << newasm::header::col::magenta;
                if(i.get_value() == 6) std::cout << newasm::header::col::cyan;
                if(i.get_value() == 7) std::cout << newasm::header::col::gray;
                if(i.get_value() == 8) std::cout << newasm::header::col::reset;
                else if(i.get_value() < 1 || i.get_value() > 8)
                {
                    i.set_value(0); //invalid request
                }
                return;
            }
            if(id == newasm::hardware::IO_ports::dskfmat.get_addr()) // disk formatting
            {
                newasm::hardware::Disk.format();
                auto& i = newasm::hardware::IO_ports::dskfmat;
                i.set_value(1);
            }
            if(id == newasm::hardware::IO_ports::dskw.get_addr()) // disk writing
            {
                auto& i = newasm::hardware::IO_ports::dskw;
                if(!newasm::header::functions::istext(newasm::mem::regs::tlr))
                {
                    i.set_value(0);
                    return;
                }
                int startByte = std::stoi(newasm::mem::regs::stl.get_value());
                int endByte = startByte + newasm::header::functions::remq(newasm::mem::regs::tlr.get_value()).size();
                std::string content = newasm::header::functions::remq(newasm::mem::regs::tlr.get_value());
                if(!newasm::header::functions::isnumeric(newasm::mem::regs::stl))
                {
                    i.set_value(0);
                    return;
                }

                newasm::hardware::Disk.writeToDisk(startByte, endByte, content);
                i.set_value(1);
            }
            if(id == newasm::hardware::IO_ports::dskr.get_addr()) // disk reading
            {
                auto& i = newasm::hardware::IO_ports::dskr;
                if(!newasm::header::functions::isnumeric(newasm::mem::regs::tlr))
                {
                    i.set_value(0);
                    return;
                }
                if(!newasm::header::functions::isnumeric(newasm::mem::regs::stl))
                {
                    i.set_value(0);
                    return;
                }

                int startByte = std::stoi(newasm::mem::regs::tlr);
                int endByte = std::stoi(newasm::mem::regs::stl);
                newasm::hardware::Disk.readDisk(startByte, endByte);
                i.set_value(1);
            }
            return;
        }
        inline std::string inIOPort(int id) // read from port
        {
            std::string output;
            if(id == newasm::hardware::IO_ports::txtcol.get_addr())
            {
                output = std::to_string(newasm::hardware::IO_ports::txtcol.get_value());
            }
            if(id == newasm::hardware::IO_ports::dskfmat.get_addr())
            {
                output = std::to_string(newasm::hardware::IO_ports::dskfmat.get_value());
            }
            if(id == newasm::hardware::IO_ports::dskw.get_addr())
            {
                output = std::to_string(newasm::hardware::IO_ports::dskw.get_value());
            }
            if(id == newasm::hardware::IO_ports::dskr.get_addr())
            {
                if(newasm::hardware::IO_ports::dskr.get_value() == 1)
                {
                    output = "\"" + (newasm::hardware::Disk.data) + "\"";
                }
                else
                {
                    output = "\"" + static_cast<std::string>("err") + "\"";
                }
            }

            //std::cout << "output = `" << output << "`\n";
            return output;
        }
    }
}