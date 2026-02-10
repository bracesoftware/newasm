// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm
{
    namespace csimple
    {
        void* memcpy(void* dest, const void* source, int bytes)
        {
            if(bytes < 0)
            {
                throw std::invalid_argument("newasm::csimple::memcpy: Size `bytes` is negative.");
                return nullptr;
            }
            unsigned char* dst = (unsigned char*)dest;
            const unsigned char* src = (const unsigned char*)source;

            for(int i = 0; i < bytes; ++i)
            {
                *dst = *src;
                dst++;
                src++;
            }
            return dest;
        }

        int ipow(int base, int exp)
        {
            int r = 1;
            while(exp > 0)
            {
                if(exp & 1)
                {
                    r *= base;
                }
                base *= base;
                exp >>= 1;
            }
            return r;
        }
    }
}