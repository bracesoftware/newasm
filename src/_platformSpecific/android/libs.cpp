// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm
{
    namespace kernel
    {
        namespace dynamic
        {
            int CALL(std::string libname, std::string func)
            {
                std::cout << "Android doesn't yet support dynamic library (extension) loading...\n";
                return 1;
            }
        }
    }
}