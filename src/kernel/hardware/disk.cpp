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
        const std::string disk_file = newasm::core::constants::data_folder + 
        newasm::core::constants::separator + newasm::core::constants::disk_file;
        class DISK_
        {
            private:
            std::string path;
            int size;

            public:
            std::string data;
            DISK_(std::string disk_file, int mb_size) : path(disk_file), size(mb_size)
            {
                if(!std::filesystem::exists(path))
                {
                    std::ofstream file(path, std::ios::binary | std::ios::trunc);
                    for(int i = 0; i < size*1024*1024; ++i) file.put(0);
                    file.close();
                }
            }

            void format(int size_)
            {
                std::ofstream file(path, std::ios::binary);
                for(int i = 0; i < size_*1024*1024; ++i) file.put(0);
                file.close();
                this->size = size_;
            }

            void format()
            {
                this->format(this->size);
            }

            std::string readDisk_(std::streampos start, std::streampos end)
            {
                std::ifstream file(this->path, std::ios::binary);
                if (!file) return std::string("");

                std::streamsize size = end - start;
                file.seekg(start);
                if (!file) return std::string("");
                std::string buffer(size, '\0');
                file.read(&buffer[0], size);
                if (!file) return std::string("");
                file.close();

                return buffer;
            }

            void readDisk(int start, int end)
            {
                this->data = this->readDisk_(start,end);
            }

            void writeToDisk(std::streampos start, std::streampos end, const std::string& content)
            {
                std::fstream file(this->path, std::ios::in | std::ios::out | std::ios::binary);

                std::streamsize size = end - start;
                file.seekp(start);
                file.write(content.data(), size);
                file.close();
            }
        };
        
        newasm::hardware::DISK_ Disk(newasm::hardware::disk_file, 10);
    }
}