// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm
{
    namespace hardware
    {
        class process final
        {
            private:
            std::string source;
            std::vector<std::pair<std::string, int>> jump_table;
            //std::vector<newasm::compiler::lineData> contents;

            public:
            inline process(const std::string& filename)
            : source(filename)
            {
                // do smth
            }

            inline ~process() noexcept {}
        };
        // we use the `process` class to spawn new processes
    }
}