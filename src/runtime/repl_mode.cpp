// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm
{
    int repl()
    {
        std::string line;
        std::cout << "\n";
        //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        while(true)
        {
            line.clear();
            std::cout << newasm::header::col::gray << ">> ";
            std::getline(std::cin,line);
            
            newasm::header::data::lastln = line;
            std::cout << newasm::header::col::reset;
            
            newasm::header::data::lastlndx++;
            newasm::process << line;

            if(newasm::header::data::repl_end)
            {
                break;
            }
        }
        return 0;
    }
}