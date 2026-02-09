// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm::_std
{
    template<typename F>
    class scope_exit final
    {
        F func;
        bool active = true;

        public:
        explicit inline scope_exit(F&& f) noexcept
            : func(std::forward<F>(f)) {}

        inline ~scope_exit() noexcept
        {
            if(active)
            {
                func();
            }
        }

        inline scope_exit(const scope_exit&) = delete;
        inline scope_exit& operator=(const scope_exit&) = delete;

        inline scope_exit(scope_exit&& other) noexcept
            : func(std::move(other.func)), active(other.active)
        {
            other.active = false;
        }

        inline void dismiss() noexcept
        {
            active = false;
        }
    };
}
