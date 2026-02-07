// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm
{
    inline void procfile(std::string filename) // process simple scripts
    {
        std::ifstream file(filename);
        if(!file.is_open())
        {
            std::cout << newasm::header::col::light_red << filename << " not found." << std::endl; 
            return;
        }

        std::string line;
        int idx = 0;
        while(std::getline(file, line))
        {
            //std::cout << "Processing line " << idx << " -> " << line << std::endl;
            try
            {
                newasm::procline(line);
            }
            catch(std::exception& e)
            {
                std::cerr << "stdlib te zajebucnuo stdlib:" << idx << " -> " << e.what() << std::endl;
            }
            idx ++;
        }

        file.close();
        return;
    }
}