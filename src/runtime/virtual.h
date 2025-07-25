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
            std::ofstream file(newasm::_virtual::vmem_file, std::ios::app);
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
            std::ofstream file(newasm::_virtual::vmem_file, std::ios::trunc); // trunc briše fajl prije pisanja
            if (!file.is_open())
            {
                std::cerr << "WRITE VMEM ERROR" << std::endl;
                return false;
            }

            for(const auto& cell : newasm::_virtual::virtual_mem)
            {
                file << cell << "\n";
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
                newasm::terminate(newasm::exit_codes::invalid_memacc);
                return;
            }
            if(addr < 0)
            {
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
            int old_size = newasm::_virtual::virtual_mem.size();

            if(size > newasm::_virtual::max_size)
            {
                newasm::terminate(newasm::exit_codes::invalid_memacc);
                return;
            }
            std::stringstream cell_ss;
            if(old_size < size) for(int i = old_size - 1; i < size; ++i)
            {
                cell_ss.clear();
                cell_ss << 0 << newasm::_virtual::separator_1 << i << newasm::_virtual::separator_2 << 0;
                newasm::_virtual::virtual_mem.push_back(cell_ss.str());
            }
            if(old_size > size) for(int i = 0; i < size; ++i)
            {
                cell_ss.clear();
                cell_ss << 0 << newasm::_virtual::separator_1 << i << newasm::_virtual::separator_2 << 0;
                newasm::_virtual::virtual_mem.push_back(cell_ss.str());
            }
            newasm::_virtual::size = size;
            newasm::_virtual::write_vmem();
            if(old_size == size)
            {
                return;
            }
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