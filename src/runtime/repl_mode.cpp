// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm
{
    newasm::compiler::lineData ReplLine;
    int repl()
    {
        std::string line;
        std::cout << "\n";
        //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        newasm::header::data::LastLine = &ReplLine;
        while(true)
        {
            line.clear();
            std::cout << std::endl;
            std::cout << newasm::header::col::gray << ">> ";
            std::getline(std::cin,line);
            
            std::cout << newasm::header::col::reset;
            
            newasm::header::data::lastlndx++;
            
            ReplLine = newasm::compiler::DO(line);
            newasm::procline(ReplLine);

            if(newasm::header::data::repl_end)
            {
                break;
            }
        }
        return 0;
    }
}