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
    namespace _virtual
    {
        const std::string vmem_file = newasm::core::constants::data_folder + 
        newasm::core::constants::separator + newasm::core::constants::virtual_mem;
        class RAM
        {
        private:
            std::string path;
            int size;
        public:
            RAM(std::string filename, int bytes) : path(filename), size(bytes) {}

            void init(int bytes)
            {
                std::ofstream file(path, std::ios::binary | std::ios::trunc);
                for (int i = 0; i < bytes; ++i) file.put(0);
                file.close();
                //std::cout << "Inicijaliziran ram.bin sa " << bytes << " bajtova.\n";
            }

            void writebyteat(int index, std::string character)
            {
                std::fstream file(path, std::ios::binary | std::ios::in | std::ios::out);
                if (!file.is_open())
                {
                    //std::cerr << "Greška pri otvaranju ram.bin!\n";
                    return;
                }

                file.seekp(index);
                file.put(character[0]);
                file.close();
                //std::cout << "Zapisano '" << character[0] << "' na poziciju " << index << ".\n";
            }

            std::string readbyteat(int index)
            {
                std::ifstream file(path, std::ios::binary);
                if (!file.is_open())
                {
                    //std::cerr << "Greška pri otvaranju ram.bin!\n";
                    return "[ERROR]";
                }

                file.seekg(index);
                char c;
                file.get(c);
                file.close();

                if (std::isprint(c)) return std::string(1, c);
                else
                {
                    std::stringstream ss;
                    ss << "\\x" << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)c;
                    return ss.str();
                }
            }

            void writeintat(int index, int value)
            {
                std::fstream file(path, std::ios::binary | std::ios::in | std::ios::out);
                if (!file.is_open())
                {
                    return;
                }

                file.seekp(index);
                for (int i = 0; i < 4; ++i)
                {
                    file.put((value >> (i * 8)) & 0xFF);
                }
                file.close();
                //std::cout << "Zapisano int: " << value << " na poziciju " << index << ".\n";
            }

            int readintat(int index)
            {
                std::ifstream file(path, std::ios::binary);
                if (!file.is_open())
                {
                    return -1;
                }

                file.seekg(index);
                int value = 0;
                for (int i = 0; i < 4; ++i)
                {
                    char byte;
                    file.get(byte);
                    value |= ((unsigned char)byte << (i * 8));
                }
                file.close();
                return value;
            }
            void writefloatat(int index, float value)
            {
                std::fstream file(path, std::ios::binary | std::ios::in | std::ios::out);
                if (!file.is_open())
                {
                    return;
                }

                file.seekp(index);
                char* bytes = reinterpret_cast<char*>(&value);
                file.write(bytes, sizeof(float));
                file.close();

                //std::cout << "Zapisano float: " << value << " na poziciju " << index << ".\n";
            }

            float readfloatat(int index)
            {
                std::ifstream file(path, std::ios::binary);
                if (!file.is_open())
                {
                    return -1.0f;
                }

                file.seekg(index);
                float value;
                file.read(reinterpret_cast<char*>(&value), sizeof(float));
                file.close();

                return value;
            }
            void writestringat(int index, const std::string& text)
            {
                std::fstream file(path, std::ios::binary | std::ios::in | std::ios::out);
                if (!file.is_open())
                {
                    return;
                }

                file.seekp(index);

                int length = text.size();
                file.write(reinterpret_cast<char*>(&length), sizeof(int));
                file.write(text.c_str(), length);

                file.close();
                //std::cout << "Zapisano string: \"" << text << "\" na poziciju " << index << ".\n";
            }

            std::string readstringat(int index)
            {
                std::ifstream file(path, std::ios::binary);
                if (!file.is_open())
                {
                    return "[ERROR]";
                }

                file.seekg(index);

                int length;
                file.read(reinterpret_cast<char*>(&length), sizeof(int));

                std::string result(length, '\0');
                file.read(&result[0], length);

                file.close();
                return result;
            }
        };

        newasm::_virtual::RAM virtualMemory(newasm::_virtual::vmem_file, 64);

        const int readmode_num = 1;
        const int readmode_decm = 2;
        const int readmode_char = 3;
        const int readmode_txt = 4;

        int readMode = readmode_char;

        std::string readData(int index)
        {
            if(newasm::_virtual::readMode == readmode_num)
            {
                return std::to_string(newasm::_virtual::virtualMemory.readintat(index));
            }
            if(newasm::_virtual::readMode == readmode_decm)
            {
                return std::to_string(newasm::_virtual::virtualMemory.readfloatat(index));
            }
            if(newasm::_virtual::readMode == readmode_char)
            {
                return std::string("'")+(newasm::_virtual::virtualMemory.readbyteat(index))+std::string("'");
            }
            if(newasm::_virtual::readMode == readmode_txt)
            {
                return (std::string("\"") + newasm::_virtual::virtualMemory.readstringat(index) + std::string("\""));
            }
            return "err";
        }
    }
}

#ifdef __ERR__
namespace newasm
{
    namespace _virtual
    {
        template<typename T>
        concept simple_types = std::same_as<std::remove_cvref_t<T>, int> || 
            std::same_as<std::remove_cvref_t<T>, float> ||
            std::same_as<std::remove_cvref_t<T>, std::string>;
        template<newasm::_virtual::simple_types T> class var_
        {
            public:
            T value;
            bool constant = false;
        };
        ////////////////////////////////////
        const int max_size = 512;
        const std::string vmem_file = newasm::core::constants::data_folder + newasm::core::constants::separator + newasm::core::constants::virtual_mem;
        int size = 0;
        std::vector<std::string> virtual_mem;
        newasm::_virtual::var_<std::string> CELL;
        const char separator_1 = '?';
        const char separator_2 = '>';

        void main()
        {
            std::ofstream file(newasm::_virtual::vmem_file, std::ios::out | std::ios::trunc);
            file.close();
            return;
        }

        //getters and setters
        void read_vmem()
        {
            std::ifstream file(newasm::_virtual::vmem_file);
            if (!file.is_open())
            {
                std::cerr << "READ VMEM ERROR" << std::endl;
                return; // Prazan vector ako nije uspjelo
            }

            std::string cell;
            while(std::getline(file, cell))
            {
                newasm::_virtual::virtual_mem.push_back(cell);
            }
            file.close();
            return;
        }

        // Overwrite memory.
        bool write_vmem()
        {
            std::remove(newasm::_virtual::vmem_file.c_str());
            std::ofstream file(newasm::_virtual::vmem_file, std::ios::out | std::ios::trunc);
            if(!file.is_open())
            {
                std::cerr << "WRITE VMEM ERROR" << std::endl;
                return false;
            }

            for(auto i = newasm::_virtual::virtual_mem.begin(); i != newasm::_virtual::virtual_mem.end(); ++i)
            {
                file << *i + std::string("\n");
            }
            file.close();
            return true;
        }


        //actually working
        void get_at(int addr)
        {
            newasm::_virtual::virtual_mem.clear();
            newasm::_virtual::read_vmem();

            if(newasm::_virtual::virtual_mem.size() <= addr)
            {
                //std::cout << "newasm::_virtual::virtual_mem.size() <= addr :::::: " <<
                //newasm::_virtual::virtual_mem.size() << " :::::::: " << addr << std::endl;
                newasm::terminate(newasm::exit_codes::invalid_memacc);
                return;
            }
            if(addr < 0)
            {
                //std::cout << "addr < 0 :::::: " << addr << std::endl;
                newasm::terminate(newasm::exit_codes::invalid_memacc);
                return;
            }

            std::vector<std::string> tokens;

            bool constant = false;
            std::string value;
            //const§addr$value
            for(auto i = newasm::_virtual::virtual_mem.begin(); i != newasm::_virtual::virtual_mem.end(); ++i)
            {
                tokens = newasm::header::functions::split_fixed(*i, newasm::_virtual::separator_1);
                constant = (std::stoi(tokens[0]) == 0) ? false : true;
                tokens = newasm::header::functions::split_fixed(tokens[1], newasm::_virtual::separator_2);

                if(addr == std::stoi(tokens[0]))
                {
                    newasm::_virtual::CELL.constant = constant;
                    newasm::_virtual::CELL.value = value;
                    return;
                }
            }
            newasm::_virtual::CELL.constant = false;
            newasm::_virtual::CELL.value = "err";
            newasm::_virtual::virtual_mem.clear();
            return;
        }
        void push_addr()
        {
            newasm::_virtual::virtual_mem.clear();
            newasm::_virtual::read_vmem();

            if(newasm::_virtual::virtual_mem.size() >= newasm::_virtual::size)
            {
                newasm::terminate(newasm::exit_codes::vmem_overflow);
                return;
            }

            std::stringstream cell_ss;
            cell_ss << int(newasm::_virtual::CELL.constant) << newasm::_virtual::separator_1;
            cell_ss << newasm::_virtual::virtual_mem.size() << newasm::_virtual::separator_2;
            cell_ss << newasm::_virtual::CELL.value;

            newasm::_virtual::virtual_mem.push_back(cell_ss.str());
            newasm::_virtual::write_vmem();

            newasm::_virtual::virtual_mem.clear();
            return;
        }

        void realloc(int size)
        {
            newasm::_virtual::virtual_mem.clear();
            newasm::_virtual::read_vmem();

            if(size > newasm::_virtual::max_size)
            {
                newasm::terminate(newasm::exit_codes::invalid_memacc);
                return;
            }

            std::stringstream cell_ss;
            for(int i = 0; i < size; ++i)
            {
                cell_ss.str("");
                cell_ss.clear();
                cell_ss << 0 << newasm::_virtual::separator_1 << i << newasm::_virtual::separator_2 << 0;
                newasm::_virtual::virtual_mem.push_back(cell_ss.str());
            }
            newasm::_virtual::size = size;
            newasm::_virtual::write_vmem();
            return;
        }

        void set_at(int addr)
        {
            if(addr >= newasm::_virtual::size)
            {
                newasm::terminate(newasm::exit_codes::invalid_memacc);
                return;
            }
            if(addr < 0)
            {
                newasm::terminate(newasm::exit_codes::invalid_memacc);
                return;
            }
            newasm::_virtual::read_vmem();
            std::stringstream cell_ss;
            cell_ss << int(newasm::_virtual::CELL.constant) << newasm::_virtual::separator_1;
            cell_ss << addr << newasm::_virtual::separator_2;
            cell_ss << newasm::_virtual::CELL.value;
            newasm::_virtual::virtual_mem.at(addr) = cell_ss.str();
            newasm::_virtual::write_vmem();
            return;
        }
    }
}
#endif