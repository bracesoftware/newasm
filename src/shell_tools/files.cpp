// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

__newasm_LOAD_PACKAGE_MODULE(shell_tools__FILES, {
    //setup goes here
});

namespace newasm
{
    namespace files
    {
        inline void ListFiles(std::vector<std::string> path)//list all files for a specific path
        {
            auto format_path = [&]() -> std::string {
                std::stringstream p;
                for(int i = 0; i < path.size(); ++i)
                {
                    p << path[i];
                    if(i + 1 == path.size())
                    {
                        continue;
                    }
                    p << '/';
                }
                return p.str();
            };
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
            auto GetTotalDiskUsage__L = [&]() -> DISK_POS {
                auto& disk = NewASM::hardware::Disk;
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
                if(pairs.empty())
                {
                    return FILE_TABLE_POS;
                }
                return pairs.back().second;
            };
            auto PATH = format_path();
            unsigned int found_files = 0;
            FILE_TABLE table = GetFileTable(NewASM::hardware::Disk);
            EMPTYLINE;
            if(GetFiles__L(NewASM::hardware::Disk) == 0)
            {
                NewASM::header::functions::err("No accessible files found on the disk.");
            }
            if(GetFiles__L(NewASM::hardware::Disk) > 0) if(path.size() == 1)
            {
                unsigned int file_usage = 0;
                for(int i = 0; i < table.size(); ++i)
                {
                    if(table[i].pos == FILE_TABLE_POS) continue;
                    file_usage += table[i].size;
                }
                file_usage += GetMaxFiles() * sizeof(FILE);
                static unsigned int table_size = GetMaxFiles() * sizeof(FILE);
                std::cout << NewASM::header::col::gray;
                std::cout << "\t" << "Files: " << file_usage - table_size << " B" << std::endl;
                std::cout << "\t" << "FT size: " << table_size << " B" << std::endl;
                std::cout << "\t" << "Fragmentation waste: " << GetTotalDiskUsage__L() - file_usage << " B" << std::endl;
                std::cout << "\t" << NewASM::header::style::underline << "Total disk usage: " << GetTotalDiskUsage__L() << " B" << std::endl;
                std::cout << NewASM::header::col::reset;
            }
            if(GetFiles__L(NewASM::hardware::Disk) > 0) for(int i = 0; i < table.size(); ++i)
            {
                if(path.size() > 1) if(std::string(table[i].name).starts_with(PATH) && std::string(table[i].name).size() > PATH.size())
                {
                    if(table[i].pos == FILE_TABLE_POS) continue;
                    ++found_files;
                    std::cout << NewASM::header::col::gray;
                    std::cout << "\t" << std::string(table[i].name).substr(PATH.size() + 1) << "\t\t" << NewASM::header::col::yellow << table[i].size << " B\n";
                }
                if(path.size() == 1)
                {
                    if(table[i].pos == FILE_TABLE_POS) continue;
                    std::cout << NewASM::header::col::gray;
                    std::cout << "\t" << table[i].name << "\t\t" << NewASM::header::col::yellow << table[i].size << " B\n";
                }
            }
            if(GetFiles__L(NewASM::hardware::Disk) > 0) if(found_files == 0 and path.size() > 1)
            {
                NewASM::header::functions::err("No accessible files found on the disk.");
            }
            std::cout << NewASM::header::col::reset;
            EMPTYLINE;
            return;
        }

        inline void CD(const std::string& dir)
        {
            if(dir == "..") //go back
            {
                if(newasm::ctl::data::path.size() == 1)
                {
                    return;
                }
                newasm::ctl::data::path.pop_back();
                return;
            }
            if(!NewASM::header::functions::isalphanum(dir))
            {
                NewASM::header::functions::err("Name of the directory has to be alphanumeric.");
                return;
            }
            NewASM::ctl::data::path.push_back(dir);
            return;
        }
    }
}