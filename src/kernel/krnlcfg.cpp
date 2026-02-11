// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

__newasm_LOAD_PACKAGE_MODULE(KRNLCFG, {
    //setup goes here
});

namespace newasm
{
    namespace kernel
    {
        namespace cfg
        {
            bool IOStream = false;
            bool Extensions = false;
            bool Thread = false;
            bool Chrono = false;
            bool Network = false;
            bool Memory = false;
            bool TextOperations = false;
            bool ContainerManipulation = false;
            bool FileStream = false;
            bool Tuple = false;
            bool TCProtocol = false;
            bool HTTP = false;
            bool Math = false;
            bool Misc = false;
            bool Crypto = false;
            bool Context = false;
        }
    }
}