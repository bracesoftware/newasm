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
    namespace drivers
    {
        namespace vmfs
        {
            class filesystem final
            {
                private:
                const int end_of_ft = 1024 * 1024; // 1mb of ft

                newasm::hardware::DISK_& disk;
                std::vector<int> file_addr;

                filesystem(newasm::hardware::DISK_& vdisk)
                    : this->disk(vdisk)
                {
                    this->file_addr.clear();
                }

                // return the address of the content
                inline int get_file(std::string name)
                {
                    int pos, i;
                    int files = std::stoi(disk.readDisk_(0, sizeof(int)));
                    int addr, header;
                    std::string temp;

                    for(pos = sizeof(int); pos < this->end_of_ft; pos = pos + sizeof(int))
                    {
                        if(i == files)
                        {
                            break;
                        }

                        addr = std::stoi(disk.readDisk_(pos, pos + sizeof(int)));
                        header = std::stoi(disk.readDisk_(addr, addr + sizeof(int)));
                        temp = disk.readDisk_(addr + sizeof(int), addr + sizeof(int) + header);

                        if(name == temp)
                        {
                            return (addr + sizeof(int) + header);
                        }

                        ++i;
                    }
                    return 0;
                }

                public:
                inline void write(std::string name, std::string content)
                {
                    return;
                }
                inline void erase(std::string name, std::string content)
                {
                    return;
                }
                inline std::string read(std::string name)
                {
                    int addr = this->get_file(name);
                    std::string content;
                    if(addr == 0)
                    {
                        content = "nil";
                        return content;
                    }

                    int header = disk.readDisk_(addr, addr + sizeof(int));
                    
                    return content;
                }
            };
        }
        vmfs::filesystem fileSystem(newasm::hardware::Disk);
    }
}