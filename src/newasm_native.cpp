// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

module(native_connector, {
    NewASM::Modules::PrintLine("Native functions connected to the VM.");
});

namespace newasm
{
    namespace Const
    {
        namespace SupportedNatives
        {
            constinit const int PRINT = 0;

            const std::unordered_map<std::string, int> Identifiers = {
                {"print", PRINT}
            };
        }
    }
}