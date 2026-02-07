// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#ifdef RANGE_INCLUDED
    #error ahh
#endif

#define RANGE_INCLUDED 23

namespace newasm
{
    namespace _std
    {
        template<class _Type>
        class range final
        {
            private:
            mutable std::vector<_Type> vec;

            public:
            inline range(std::initializer_list<_Type> list)
            : vec(list) {}

            inline ~range() noexcept {}

            range& operator=(std::initializer_list<_Type> list)
            {
                vec = list;
                return *this;
            }

            range& operator=(const _Type& value)
            {
                vec.clear();
                vec.push_back(value);
                return *this;
            }

            friend bool operator>>(const _Type& value, const range& r)
            {
                return std::find(r.vec.begin(), r.vec.end(), value) != r.vec.end();
            }
        };
    }
}