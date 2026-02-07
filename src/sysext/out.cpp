// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm
{
    namespace _std
    {
        template<typename T>
        concept _WriteableType = (
            std::is_same_v<T, char*> or
            std::is_same_v<T, const char*> or
            std::is_same_v<T, std::string> or
            std::is_same_v<T, char> or
            std::is_same_v<T, int> or
            std::is_same_v<T, short> or
            std::is_same_v<T, long> or
            std::is_same_v<T, long long> or
            std::is_same_v<T, unsigned int> or
            std::is_same_v<T, unsigned short> or
            std::is_same_v<T, unsigned long> or
            std::is_same_v<T, unsigned long long> or
            std::is_same_v<T, unsigned char> or
            std::is_same_v<T, bool> or
            std::is_same_v<T, float> or
            std::is_same_v<T, double>
        );

        template<_WriteableType T>
        inline void write(const T& what) noexcept
        {
            if constexpr(std::is_integral_v<T>)
            {
                std::printf("%d", what);
            }
            else if constexpr(std::is_floating_point_v<T>)
            {
                std::printf("%f", what);
            }
            else if constexpr(std::is_same_v<T, const char*> or
            std::is_same_v<T, char*>)
            {
                std::printf("%s", what);
            }
            else if constexpr(std::is_same_v<T, std::string>)
            {
                std::printf("%s", what.c_str());
            }
            else
            {
                static_assert(!sizeof(T*), "unsupported_argument_in_write()");
            }
            return;
        }
    }
}

#define WRITE(text) newasm::_std::write(text)