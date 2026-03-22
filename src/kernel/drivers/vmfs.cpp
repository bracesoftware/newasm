// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

__newasm_LOAD_PACKAGE_MODULE(VirtualDiskDriver, {
    //empty
});

#define __newasm_VMFS_VER_ORIGINAL 1
#define __newasm_VMFS_VER_COMMUNITY 2 //version made by collecting resources available online, WAS NOT WORKING

#define __newasm_VMFS __newasm_VMFS_VER_ORIGINAL

#if __newasm_VMFS == __newasm_VMFS_VER_ORIGINAL
//stackoverflow file system did not work
//so i had to improvise..and reinvent the wheel...
namespace newasm::Drivers::FileSystem_V
{
    constinit const unsigned int FILE_TABLE_POS = 0;
    using DISK_POS = unsigned long;
    constexpr inline unsigned int GetMaxFiles()
    {
        return __newasm_MAX_FILES;
    }
    constexpr inline unsigned int GetMaxFileNameLen()
    {
        return __newasm_MAX_FILENAME_LEN;
    }
    struct FILE final
    {
        char name[__newasm_MAX_FILENAME_LEN];
        DISK_POS pos; //we don't use bool exists, but 0 as a start pos if it doesnt exist
                                //cuz 0 is a position for file table,so no file can be on 0
        DISK_POS size; //size can't be < 0
    };
    constexpr inline unsigned int GetFileTableEnd()
    {
        return (sizeof(FILE) * NewASM::Drivers::FileSystem_V::GetMaxFiles());
    }
    using FILE_TABLE = std::vector<FILE>;
    using FILE_NAME = const std::string&;
    using DISK = NewASM::hardware::DISK_;
    FORCE_INLINE inline FILE_TABLE GetFileTable(DISK& disk)
    {
        FILE_TABLE table(NewASM::Drivers::FileSystem_V::GetMaxFiles());
        std::string DATA = disk.READ_DSK(FILE_TABLE_POS, sizeof(FILE) * NewASM::Drivers::FileSystem_V::GetMaxFiles());
        if(!DATA.empty())
        {
            std::memcpy(table.data(), DATA.data(), DATA.size());
        }
        return table;
    }
    FORCE_INLINE inline void SaveFileTable(DISK& disk, const FILE_TABLE& table)
    {
        std::string buf(sizeof(FILE) * NewASM::Drivers::FileSystem_V::GetMaxFiles(), '\0');
        std::memcpy(&buf[0], table.data(), buf.size());
        disk.WRITE_DSK(0, buf);
        return;
    }
    inline void FormatDisk(DISK& disk)
    {
        disk.ERASE_DSK();
        unsigned const int files = NewASM::Drivers::FileSystem_V::GetMaxFiles();
        FILE_TABLE table(files);
        for(int i = 0; i < files; ++i) //just to be sure lmao
        {
            table[i].pos = FILE_TABLE_POS;
            table[i].size = FILE_TABLE_POS;
        }
        NewASM::Drivers::FileSystem_V::SaveFileTable(disk, table);
        return;
    }
    inline DISK_POS GetFreePos(DISK& disk, DISK_POS size)
    {
        using namespace NewASM::Drivers::FileSystem_V;
        auto GetFiles__L = [&](DISK& dsk) -> DISK_POS {
            DISK_POS files = 0;
            FILE_TABLE table = GetFileTable(dsk);
            for(int i = 0; i < table.size(); ++i) //just to be sure lmao
            {
                if(table[i].pos != FILE_TABLE_POS)
                {
                    ++files;
                }
            }
            return files;
        };
        DISK_POS files = GetFiles__L(disk);
        if(files == 0)
        {
            return NewASM::Drivers::FileSystem_V::GetFileTableEnd();
        }
        if(files > 0)
        {
            std::vector<std::pair<DISK_POS, DISK_POS>> pairs;
            auto table = GetFileTable(disk);
            for(int i = 0; i < table.size(); ++i)
            {
                if(table[i].pos == FILE_TABLE_POS) continue;
                pairs.push_back({table[i].pos, table[i].pos + table[i].size});
            }
            std::sort(pairs.begin(), pairs.end(), [](const auto& a, const auto& b) -> bool {
                return a.first < b.first;
            });
            if(pairs.size() == 1)
            {
                return pairs.front().second;
            }
            int PairSize = pairs.size();
            for(int i = 0; i < PairSize; ++i)
            {
                if(i == 0)
                {
                    if(pairs.front().first - NewASM::Drivers::FileSystem_V::GetFileTableEnd() >= size)
                    {
                        return NewASM::Drivers::FileSystem_V::GetFileTableEnd();
                    }
                    continue;
                }
                if(i == PairSize - 1)
                {
                    if((__newasm_DISK_SIZE * 1024 * 1024) - pairs.back().second >= size)
                    {
                        return pairs.back().second;
                    }
                    continue;
                }
                if(pairs.at(i + 1).first - pairs.at(i).second >= size)
                {
                    return pairs.at(i).second;
                }
                continue;
            }
        }
        return FILE_TABLE_POS;
    }
    inline bool EXISTS(DISK& disk, FILE_NAME name)
    {
        using namespace NewASM::Drivers::FileSystem_V;
        FILE_TABLE table = GetFileTable(disk);
        for(int i = 0; i < table.size(); ++i)
        {
            if(table[i].pos != FILE_TABLE_POS && std::string(table[i].name) == name)
            {
                return true;
            }
        }
        return false;
    }
    inline void MKFILE(DISK& disk, FILE_NAME name, const std::string& content)
    {
        using namespace NewASM::Drivers::FileSystem_V;
        if(name.size() >= __newasm_MAX_FILENAME_LEN)
        {
            return;
        }
        FILE_TABLE table = GetFileTable(disk);
        for(int i = 0; i < table.size(); ++i)
        {
            if(table[i].pos == FILE_TABLE_POS)//it does NOT exist
            {
                std::memset(table[i].name, 0, __newasm_MAX_FILENAME_LEN);
                std::memcpy(table[i].name, name.data(), name.size());
                table[i].size = content.size();
                table[i].pos = NewASM::Drivers::FileSystem_V::GetFreePos(disk, content.size());
                disk.WRITE_DSK(table[i].pos, content);
                break;
            }
        }
        SaveFileTable(disk, table);
        return;
    }
    inline void RMFILE(DISK& disk, FILE_NAME name)
    {
        using namespace NewASM::Drivers::FileSystem_V;
        FILE_TABLE table = GetFileTable(disk);
        for(int i = 0; i < table.size(); ++i)
        {
            if(std::string(table[i].name) == name && table[i].pos != FILE_TABLE_POS)
            {
                table[i].pos = FILE_TABLE_POS;
                table[i].size = FILE_TABLE_POS;
                break;
            }
        }
        SaveFileTable(disk, table);
        return;
    }
    inline std::string READFILE(DISK& disk, FILE_NAME name)
    {
        using namespace NewASM::Drivers::FileSystem_V;
        FILE_TABLE table = GetFileTable(disk);
        std::string buf("");
        for(int i = 0; i < table.size(); ++i)
        {
            if(std::string(table[i].name) == name && table[i].pos != FILE_TABLE_POS)
            {
                buf = disk.READ_DSK(table[i].pos, table[i].size);
                break;
            }
        }
        return buf;
    }
    inline void MODFILE(DISK& disk, FILE_NAME name, const std::string& content)
    {
        using namespace NewASM::Drivers::FileSystem_V;
        FILE_TABLE table = GetFileTable(disk);
        std::string buf("");
        for(int i = 0; i < table.size(); ++i)
        {
            if(std::string(table[i].name) == name && table[i].pos != FILE_TABLE_POS)
            {
                if(content.size() <= table[i].size)
                {
                    disk.WRITE_DSK(table[i].pos, content);
                    table[i].size = content.size();
                    break;
                }
                RMFILE(disk, name);
                MKFILE(disk, name, content);
                return;
            }
        }
        SaveFileTable(disk, table);
        return;
    }
    inline void APPTOFILE(DISK& disk, FILE_NAME name, const std::string& content)
    {
        using namespace NewASM::Drivers::FileSystem_V;
        std::string old = READFILE(disk, name);
        MODFILE(disk, name, old + content);
        return;
    }
    inline void DEFRAG(DISK& disk)
    {
        return;
    }
}
#elif __newasm_VMFS == __newasm_VMFS_VER_COMMUNITY
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
    inline void MKFILE_OLD(newasm::hardware::DISK_& disk, const std::string& name, const std::string& content)//creditz stackoverflow
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
    //DO NOT search free filesystems on stackoverflow
    inline void MKFILE(newasm::hardware::DISK_& disk, const std::string& name, const std::string& content)
    {
        auto table = get_file_table(disk);
        int table_size_bytes = sizeof(FileEntry) * newasm::Drivers::FileSystem_V::MAX_FILES;
        
        for(int i = 0; i < newasm::Drivers::FileSystem_V::MAX_FILES; ++i)
        {
            if(!table[i].exists)
            {
                int start_offset = table_size_bytes;
                
                for(int j = 0; j < newasm::Drivers::FileSystem_V::MAX_FILES; j++)
                {
                    if(table[j].exists)
                    {
                        start_offset = std::max(start_offset, table[j].start_pos + table[j].size);
                    }
                }

                table[i].exists = true;
                table[i].size = content.size();
                table[i].start_pos = start_offset;
                std::strncpy(table[i].name, name.c_str(), MAX_FILENAME - 1);

                disk.writeToDisk(table[i].start_pos, table[i].start_pos + table[i].size, content);
                save_file_table(disk, table);
                return;
            }
        }
    }
    inline void RMFILE(newasm::hardware::DISK_& disk, const std::string& name)
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
    inline void MODFILE(newasm::hardware::DISK_& disk, const std::string& name, const std::string& content)//credits stackoverflow
    {
        auto table = get_file_table(disk);
        
        for(int i = 0; i < newasm::Drivers::FileSystem_V::MAX_FILES; ++i)
        {
            if(table[i].exists && std::string(table[i].name) == name)
            {
                if(content.size() <= (size_t)table[i].size)
                {
                    disk.writeToDisk(table[i].start_pos, table[i].start_pos + content.size(), content);
                    
                    table[i].size = content.size();
                    save_file_table(disk, table);
                    return;
                }
                else
                {
                    RMFILE(disk, name);
                    MKFILE(disk, name, content);
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
    //I FOUND DEFRAGMENTATION ON STACKOVERFLOW;LETS TEST IT
    inline void DEFRAG(newasm::hardware::DISK_& disk)
    {
        auto table = get_file_table(disk);
        int table_size_bytes = sizeof(FileEntry) * MAX_FILES;
        
        struct TempEntry final
        {
            int original_index;
            FileEntry data;
        };
        std::vector<TempEntry> active_files;
        
        for(int i = 0; i < MAX_FILES; ++i)
        {
            if(table[i].exists && table[i].size > 0)
            {
                active_files.push_back({i, table[i]});
            }
        }

        std::sort(active_files.begin(), active_files.end(), [](const TempEntry& a, const TempEntry& b) {
            return a.data.start_pos < b.data.start_pos;
        });

        int current_offset = table_size_bytes;

        for(auto& temp : active_files)
        {
            std::string content = disk.readDisk_(temp.data.start_pos, temp.data.start_pos + temp.data.size);
            disk.writeToDisk(current_offset, current_offset + temp.data.size, content);
            table[temp.original_index].start_pos = current_offset;
            current_offset += temp.data.size;
        }

        save_file_table(disk, table);
    }
}
#endif

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