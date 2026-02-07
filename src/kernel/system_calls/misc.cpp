// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm
{
    namespace syscalls
    {
        namespace misc
        {
            inline int rand_old(int min, int max) noexcept
            {
                return min + (std::rand() % (max - min + 1));
            }

            inline int rand(int min, int max) noexcept
            {
                if(min > max)
                {
                    return min;
                }
                static std::mt19937 rng(std::random_device{}());
                std::uniform_int_distribution<int> dist(min, max);
                return dist(rng);
            }
        }
    }
}