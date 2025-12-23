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
        class DISK_ final
        {
            private:
            std::string path;
            int size;
            std::string volume_label;
            int sectors;

            public:
            std::string data;
            DISK_(std::string disk_file, int mb_size, std::string label)
            : path(disk_file), size(mb_size), volume_label(label)
            {
                if(!std::filesystem::exists(path))
                {
                    std::ofstream file(path, std::ios::binary | std::ios::trunc);
                    for(int i = 0; i < size * 1024 * 1024; ++i)
                    {
                        file.put(0);
                    }
                    file.close();
                }
            }

            std::string get_label()
            {
                return this->volume_label;
            }

            inline void format(int size_)
            {
                std::ofstream file(path, std::ios::binary);
                //newasm::header::functions::info("Formatting the disk...");
                
                auto size__ = size_ * 1024 * 1024;
                //float percent;

                for(int i = 0; i < size__; ++i)
                {
                    //percent = (i / size__) * 100;
                    //std::cout << "\r\tFormatting: " << static_cast<int>(percent) << std::flush;
                    file.put(0);
                }
                //std::cout << "\tDisk formatted successfully." << std::endl;

                file.close();
                this->size = size_;
            }

            inline void format()
            {
                this->format(this->size);
            }

            inline std::string readDisk_(std::streampos start, std::streampos end)
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

            inline void readDisk(int start, int end)
            {
                this->data = this->readDisk_(start, end);
            }

            inline void writeToDisk(std::streampos start, std::streampos end, const std::string& content)
            {
                std::fstream file(this->path, std::ios::in | std::ios::out | std::ios::binary);

                std::streamsize size = end - start;
                file.seekp(start);
                file.write(content.data(), size);
                file.close();
            }

            inline char read_raw(int pos)
            {
                std::ifstream file(this->path, std::ios::binary);
                if(!file) return 0;

                int start = pos;
                int end = pos;
                char ret;

                std::streamsize size = end - start;
                file.seekg(start);

                file.read(&ret, size);
                
                file.close();

                return ret;
            }
        };
        
        newasm::hardware::DISK_ Disk(newasm::hardware::disk_file, __newasm_DISK_SIZE, "main_disk");
    }
}