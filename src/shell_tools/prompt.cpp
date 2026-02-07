// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm
{
    namespace shell_tools
    {
        inline bool prompt(std::string text)
        {
            std::string input;
            std::cout << newasm::header::col::light_blue << ":: " << newasm::header::col::gray << text << newasm::header::col::light_blue << " [y/n] ";
            std::cout << newasm::header::col::reset;
            std::getline(std::cin, input);
            //std::cout << std::endl;

            if((input == "Y") or (input == "y"))
            {
                return true;
            }
            if((input == "N") or (input == "n"))
            {
                newasm::header::functions::err("Operation cancelled successfully.");
                return false;
            }
            newasm::header::functions::err("Unknown option. Operation cancelled.");
            return false;
        }
    }
}