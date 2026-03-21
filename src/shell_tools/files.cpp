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
                unsigned int usage = 0;
                for(int i = 0; i < table.size(); ++i)
                {
                    usage += table[i].size;
                }
                usage += GetMaxFiles() * sizeof(FILE);
                std::cout << NewASM::header::col::gray;
                std::cout << "\t" << NewASM::header::style::underline << "Total disk usage: " << usage << " B" << std::endl;
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