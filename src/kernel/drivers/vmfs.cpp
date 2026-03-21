// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm::Drivers::FileSystem_V
{
    const int MAX_FILES = 64;
    const int MAX_FILENAME = 128;

    struct FileEntry final
    {
        char name[MAX_FILENAME];
        int start_pos;
        int size;
        bool exists;
    };

    inline std::vector<FileEntry> get_file_table(newasm::hardware::DISK_& disk)
    {
        std::vector<FileEntry> table(newasm::Drivers::FileSystem_V::MAX_FILES);
        std::string raw = disk.readDisk_(0, sizeof(FileEntry) * newasm::Drivers::FileSystem_V::MAX_FILES);
        if(!raw.empty())
        {
            std::memcpy(table.data(), raw.data(), raw.size());
        }
        return table;
    }
    inline void save_file_table(newasm::hardware::DISK_& disk, const std::vector<FileEntry>& table)
    {
        std::string buffer(sizeof(FileEntry) * newasm::Drivers::FileSystem_V::MAX_FILES, '\0');
        std::memcpy(&buffer[0], table.data(), buffer.size());
        disk.writeToDisk(0, buffer.size(), buffer);
        return;
    }
    inline bool EXISTS(newasm::hardware::DISK_& disk, const std::string& name)
    {
        auto table = get_file_table(disk);
        for(const auto& entry : table)
        {
            if(entry.exists && std::string(entry.name) == name)
            {
                return true;
            }
        }
        return false;
    }
    inline void MKFILE(newasm::hardware::DISK_& disk, const std::string& name, const std::string& content)
    {
        auto table = get_file_table(disk);
        int table_size_bytes = sizeof(FileEntry) * newasm::Drivers::FileSystem_V::MAX_FILES;
        
        for(int i = 0; i < newasm::Drivers::FileSystem_V::MAX_FILES; ++i)
        {
            if(!table[i].exists)
            {
                int start_offset = table_size_bytes;
                if(i > 0)
                {
                    for(int j = 0; j < newasm::Drivers::FileSystem_V::MAX_FILES; j++)
                    {
                        if(table[j].exists)
                        {
                            start_offset = std::max(start_offset, table[j].start_pos + table[j].size);
                        }
                    }
                }

                table[i].exists = true;
                table[i].size = content.size();
                table[i].start_pos = start_offset;
                std::strncpy(table[i].name, name.c_str(), NewASM::Drivers::FileSystem_V::MAX_FILENAME - 1);

                disk.writeToDisk(table[i].start_pos, table[i].start_pos + table[i].size, content);
                
                save_file_table(disk, table);
                return;
            }
        }
        return;
    }
    void RMFILE(newasm::hardware::DISK_& disk, const std::string& name)
    {
        auto table = get_file_table(disk);
        for(int i = 0; i < newasm::Drivers::FileSystem_V::MAX_FILES; ++i)
        {
            if(table[i].exists && std::string(table[i].name) == name)
            {
                table[i].exists = false;
                save_file_table(disk, table);
                return;
            }
        }
    }
    inline std::string READFILE(newasm::hardware::DISK_& disk, const std::string& name)
    {
        auto table = get_file_table(disk);
        
        for(const auto& entry : table)
        {
            if(entry.exists && std::string(entry.name) == name)
            {
                return disk.readDisk_(entry.start_pos, entry.start_pos + entry.size);
            }
        }
        
        return "";
    }
    inline void MODFILE(newasm::hardware::DISK_& disk, const std::string& name, const std::string& content)
    {
        auto table = get_file_table(disk);
        
        for(int i = 0; i < newasm::Drivers::FileSystem_V::MAX_FILES; ++i)
        {
            if(table[i].exists && std::string(table[i].name) == name)
            {
                if(new_content.size() <= (size_t)table[i].size)
                {
                    disk.writeToDisk(table[i].start_pos, table[i].start_pos + new_content.size(), new_content);
                    
                    table[i].size = new_content.size();
                    save_file_table(disk, table);
                    return;
                }
                else
                {
                    deleteFile(disk, name);
                    makeFile(disk, name, new_content);
                    return;
                }
            }
        }
        return;
    }
    inline void APPTOFILE(newasm::hardware::DISK_& disk, const std::string& name, const std::string& content)
    {
        std::string old = READFILE(disk, name);
        MODFILE(disk, name, old + content);
        return;
    }
}

#if 0
namespace newasm
{
    namespace drivers
    {
        namespace vmfs
        {
            class filesystem final
            {
                private const int end_of_ft = 1024 * 1024; // 1mb of ft

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

                public inline void write(std::string name, std::string content)
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
#endif