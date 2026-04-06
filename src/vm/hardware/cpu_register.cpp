// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

module(cpu_register_emulator, {
    //setup goes here
});

namespace newasm
{
    namespace kernel
    {
        template<typename T>
        class ValueTracker final
        {
            private T value;

            inline void log(T v)
            {
                std::cout << NewASM::header::col::red;
                std::cout << "\n=====================================================\n\n";
                std::cout << "ValueTracker: " << v << '\n' << std::endl;
                std::cout << "=====================================================\n";
                std::cout << NewASM::header::col::reset;
            }

            inline T& get() { return value; }
            inline const T& get() const { return value; }

            public inline ValueTracker(T&& val) { this->value = std::move(val); }
            public inline ValueTracker(const T& val) { this->value = val; }

            inline ValueTracker<T>& operator=(const T& new_val)
            {
                get() = new_val;
                this->log(new_val);
                return *this;
            }
            inline ValueTracker<T>& operator=(T&& new_val)
            {
                get() = std::move(new_val);
                this->log(new_val);
                return *this;
            }

            inline operator T&()
            {
                return get();
            }
            inline operator const T&() const
            {
                return get();
            }

            inline T* operator->()
            {
                return &get();
            }
            inline const T* operator->() const
            {
                return &get();
            }

            inline T& operator*()
            {
                return get();
            }
            inline const T& operator*() const
            {
                return get();
            }
            inline ValueTracker<T>& operator++()
            {
                ++get();
                this->log(get());
                return *this;
            }

            inline T operator++(int)
            {
                T old = get();
                ++get();
                this->log(get());
                return old;
            }
        };
        template<typename T>
        class thread_safe final
        {
            #if 0
            inline thread_safe<T>& operator=(const T& new_val)
            {
                if(newasm::thread_line)
                {
                    thread_values.at(newasm::threads::now) = new_val;
                    return *this;
                }
                value = new_val;
                return *this;
            }

            operator T&()
            {
                if(newasm::thread_line)
                {
                    return thread_values.at(newasm::threads::now);
                }
                return value;
            }
            #endif
            
            private T value;
            private newasm::_std::map<std::string, T> thread_values;

            inline T& get()
            {
                if(newasm::thread_line)
                {
                    return thread_values.at(newasm::threads::now);
                }
                return value;
            }

            inline const T& get() const
            {
                if(newasm::thread_line)
                {
                    return thread_values.at(newasm::threads::now);
                }
                return value;
            }

            //implementation of the thread safe proxy
            //so all things we can do with T,we can do
            //with thread_safe<T> the same way
            inline void global_init()
            {
                thread_values.max_load_factor(MAX_LOAD_FACTOR);
            }
            
            public inline thread_safe(T&& val)
            {
                global_init();
                this->value = std::move(val);
            }
            public inline thread_safe(const T& val)
            {
                global_init();
                this->value = val;
            }
            public inline thread_safe()
            {
                global_init();
            }

            inline thread_safe<T>& operator=(const T& new_val)
            {
                get() = new_val;
                return *this;
            }

            //some mov semantics hihi
            inline thread_safe<T>& operator=(T&& new_val)
            {
                get() = std::move(new_val);
                return *this;
            }

            inline operator T&()
            {
                return get();
            }
            inline operator const T&() const
            {
                return get();
            }

            inline T* operator->()
            {
                return &get();
            }
            inline const T* operator->() const
            {
                return &get();
            }

            inline T& operator*()
            {
                return get();
            }
            inline const T& operator*() const
            {
                return get();
            }
        };
    }
}

template<typename T>
bool operator==(const newasm::kernel::thread_safe<T>& a, const T& b)
{
    return static_cast<const T&>(a) == b;
}

template<typename T>
bool operator==(const T& a, const newasm::kernel::thread_safe<T>& b)
{
    return a == static_cast<const T&>(b);
}

////actual registers

namespace newasm
{
    namespace internal
    {
        std::stringstream ss_;
    }
    namespace concepts
    {
        template<typename T>
        concept number = std::is_arithmetic_v<T>;

        template<typename T>
        concept _CanBeReg = (
            std::is_same<T, int>::value or
            std::is_same<T, float>::value or
            std::is_same<T, char>::value or
            std::is_same<T, std::string>::value
        );
    }
    class _register_FAST final
    {
        //main info
        private std::string name;
        private newasm::kernel::thread_safe<int> REG_TYPE__;
        //for context switching
        private newasm::kernel::thread_safe<int> regInt;
        private newasm::kernel::thread_safe<float> regFloat;
        private newasm::kernel::thread_safe<char> regChar;
        private newasm::kernel::thread_safe<std::string> regString;

        public explicit inline _register_FAST(std::string regname)
            : name(regname)
            {
                __newasmDBG(std::cout << "REG CREATED" << std::endl;)
            }

        public inline int get_type()
        {
            return REG_TYPE__;
        }

        template<newasm::concepts::_CanBeReg T>
        public inline void set_value(const T& val)
        {
            if constexpr(std::is_same_v<T, int>)
            {
                this->regInt = val;
                this->REG_TYPE__ = newasm::datatypes::number;
                return;
            }
            if constexpr(std::is_same_v<T, float>)
            {
                this->regFloat = val;
                this->REG_TYPE__ = newasm::datatypes::decimal;
                return;
            }
            if constexpr(std::is_same_v<T, char>)
            {
                this->regChar = val;
                this->REG_TYPE__ = newasm::datatypes::character;
                return;
            }
            if constexpr(std::is_same_v<T, std::string>)
            {
                this->regString = val;
                this->REG_TYPE__ = newasm::datatypes::text;
                return;
            }
        }

        template<newasm::concepts::_CanBeReg T>
        public inline T get_value()
        {
            if constexpr(std::is_same_v<T, int>)
            {
                return this->regInt;
            }
            if constexpr(std::is_same_v<T, float>)
            {
                return this->regFloat;
            }
            if constexpr(std::is_same_v<T, char>)
            {
                return this->regChar;
            }
            if constexpr(std::is_same_v<T, std::string>)
            {
                return this->regString;
            }
        }
    };
    template<typename T>
    class _register final
    {
        private std::string name;
        //char name_cexpr[cexpr_size];
        private T value;
        newasm::_std::map<std::string, T> thread_values;
        T initial_value;

        bool short_int_value = false;
        bool log = false;
        
        public explicit inline _register(std::string regname, T val)
            : name(regname), value(val), initial_value(val)
            {
                thread_values.max_load_factor(MAX_LOAD_FACTOR);
            }


        public inline void log_change() const
        {
            if(this->log == false)
            {
                return;
            }
            std::cout << newasm::header::col::aqua;
            std::cout << std::endl;
            std::cout << "newasm::real_line -> `" << newasm::real_line << "`\n";
            std::cout << "newasm::lambda::process -> " << newasm::lambda::process << "\n";
            std::cout << "newasm::header::data::repl -> " << newasm::header::data::repl << "\n";
            std::cout << "newasm::thread_line -> " << newasm::thread_line << "\n";
            std::cout << "value -> " << this->value << "\n";
            if(newasm::thread_line)
            {
                //this->thread_values.at(newasm::threads::now);
                std::cout << "thread value -> " << this->thread_values.at(newasm::threads::now) << "\n";
            }
            std::cout << newasm::header::col::reset;
            return;
        }

        inline void log_things(bool set) noexcept
        {
            this->log = set;
            return;
        }
        inline void make_short(bool set) noexcept
        {
            if constexpr(std::is_same_v<T, int>)
            {
                short_int_value = set;
            }
            return;
        }

        inline std::string identifier() const
        {
            return name;
        }
        FORCE_INLINE inline void reset()
        {
            this->value = this->initial_value;
            for(auto i = this->thread_values.begin(); i != this->thread_values.end(); ++i)
            {
                i->second = this->initial_value;
            }
        }
        FORCE_INLINE inline void reset(std::string thread_name)
        {
            thread_values[thread_name] = initial_value;
        }
        FORCE_INLINE inline T get_value() const
        {
            if(newasm::thread_line)
            {
                return this->thread_values.at(newasm::threads::now);
            }
            return this->value;
        }
        FORCE_INLINE inline void set_value(const T& new_val)
        {
            newasm::_std::scope_exit a([this]() {
                this->log_change();
            });
            //T new_val = new_val_;
            if constexpr(std::is_same_v<T, int>)
            {
                if(this->short_int_value)
                {
                    if(new_val > std::numeric_limits<short>::max() or new_val < std::numeric_limits<short>::min())
                    {
                        if(newasm::thread_line)
                        {
                            //std::cout << "THIS IS AN ERROR ! \n";
                            this->thread_values.at(newasm::threads::now) = 0;
                            return;
                        }
                        this->value = 0;
                        return;
                    }
                }
            }
            if(newasm::thread_line)
            {
                //std::cout << "THIS IS AN ERROR ! \n";
                this->thread_values.at(newasm::threads::now) = new_val;
                return;
            }
            this->value = new_val;
            return;
        }
        
        inline T& ref_value()
        {
            if(newasm::thread_line)
            {
                return this->thread_values.at(newasm::threads::now);
            }
            return value;
        }
        inline _register<T>& operator=(const T& new_val)
        {
            newasm::_std::scope_exit a([this]() {
                this->log_change();
            });
            //T new_val = new_val_;
            if constexpr(std::is_same_v<T, int>)
            {
                if(this->short_int_value)
                {
                    if(new_val > std::numeric_limits<short>::max() or new_val < std::numeric_limits<short>::min())
                    {
                        if(newasm::thread_line)
                        {
                            thread_values.at(newasm::threads::now) = 0;
                            return *this;
                        }
                        value = 0;
                        return *this;
                    }
                }
            }
            if(newasm::thread_line)
            {
                thread_values.at(newasm::threads::now) = new_val;
                return *this;
            }
            value = new_val;
            return *this;
        }
        inline operator T&()
        {
            if(newasm::thread_line)
            {
                return thread_values.at(newasm::threads::now);
            }
            return value;
        }

        inline operator T() const
        {
            if(newasm::thread_line)
            {
                return thread_values.at(newasm::threads::now);
            }
            return value;
        }
        inline operator const T&() const
        {
            if(newasm::thread_line)
            {
                return thread_values.at(newasm::threads::now);
            }
            return value;
        }
        inline friend std::ostream& operator<<(std::ostream& os, const _register<T>& r)
        {
            if(newasm::thread_line)
            {
                os << r.thread_values.at(newasm::threads::now);
                return os;
            }
            os << r.value;
            return os;
        }
        inline friend std::istream& operator>>(std::istream& is, _register<T>& r)
        {
            if(newasm::thread_line)
            {
                is >> r.thread_values.at(newasm::threads::now);
                return is;
            }
            is >> r.value;
            return is;
        }

        bool operator==(const T& other) const
        {
            if(newasm::thread_line)
            {
                return thread_values.at(newasm::threads::now) == other;
            }
            return value == other;
        }
        friend bool operator==(const T& lhs, const _register<T>& rhs)
        {
            if(newasm::thread_line)
            {
                return lhs == rhs.thread_values.at(newasm::threads::now);
            }
            return lhs == rhs.value;
        }

        inline void add_end_(const std::string& str__)
        {
            if constexpr(!std::is_same_v<T, std::string>)
            {
                return;
            }
            if(newasm::thread_line)
            {
                std::stringstream ss;
                ss << str__;
                ss << thread_values.at(newasm::threads::now);
                ss << str__;
                thread_values.at(newasm::threads::now) = ss.str();
                return;
            }
            std::stringstream ss;
            ss << str__;
            ss << value;
            ss << str__;
            value = ss.str();
            return;
        }
        inline void SetStringValue(const std::string& value)
        {
            if constexpr(std::is_same_v<T, std::string>)
            {
                this->set_value(value);
                this->add_end_("\"");
            }
            return;
        }
    };

/*
  /$$$$$$   /$$               /$$                    
 /$$__  $$ | $$              |__/                    
| $$  \__//$$$$$$    /$$$$$$  /$$ /$$$$$$$   /$$$$$$ 
|  $$$$$$|_  $$_/   /$$__  $$| $$| $$__  $$ /$$__  $$
 \____  $$ | $$    | $$  \__/| $$| $$  \ $$| $$  \ $$
 /$$  \ $$ | $$ /$$| $$      | $$| $$  | $$| $$  | $$
|  $$$$$$/ |  $$$$/| $$      | $$| $$  | $$|  $$$$$$$
 \______/   \___/  |__/      |__/|__/  |__/ \____  $$
                                            /$$  \ $$
                                           |  $$$$$$/
                                            \______/ 
*/

    inline std::string operator+(const _register<std::string>& reg, const std::string& rhs)
    {
        newasm::internal::ss_.str("");
        newasm::internal::ss_.clear();
        newasm::internal::ss_ << reg.get_value();
        newasm::internal::ss_ << rhs;
        return newasm::internal::ss_.str();
    }
    inline std::string operator+(const _register<std::string>& reg, const _register<std::string>& reg2)
    {
        newasm::internal::ss_.str("");
        newasm::internal::ss_.clear();
        newasm::internal::ss_ << reg.get_value();
        newasm::internal::ss_ << reg2.get_value();
        return newasm::internal::ss_.str();
    }
    inline std::string operator+(const std::string& rhs, const _register<std::string>& reg)
    {
        newasm::internal::ss_.str("");
        newasm::internal::ss_.clear();
        newasm::internal::ss_ << rhs;
        newasm::internal::ss_ << reg.get_value();
        return newasm::internal::ss_.str();
    }

    inline std::string operator+(const std::string& lhs, int rhs)
    {
        newasm::internal::ss_.str("");
        newasm::internal::ss_.clear();
        newasm::internal::ss_ << lhs;
        newasm::internal::ss_ << rhs;
        return newasm::internal::ss_.str();
    }

    inline std::string operator+(int lhs, const std::string& rhs)
    {
        return std::to_string(lhs) + rhs;
    }

    inline std::string operator+(const char* lhs, const newasm::_register<std::string>& rhs)
    {
        newasm::internal::ss_.str("");
        newasm::internal::ss_.clear();
        newasm::internal::ss_ << std::string(lhs);
        newasm::internal::ss_ << rhs.get_value();
        return newasm::internal::ss_.str();
    }

    inline std::string operator+(const newasm::_register<std::string>& lhs, const char* rhs)
    {
        newasm::internal::ss_.str("");
        newasm::internal::ss_.clear();
        newasm::internal::ss_ << lhs.get_value();
        newasm::internal::ss_ << std::string(rhs);
        return newasm::internal::ss_.str();
    }


    /*
     
 /$$   /$$                         /$$                                    
| $$$ | $$                        | $$                                    
| $$$$| $$ /$$   /$$ /$$$$$$/$$$$ | $$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$$
| $$ $$ $$| $$  | $$| $$_  $$_  $$| $$__  $$ /$$__  $$ /$$__  $$ /$$_____/
| $$  $$$$| $$  | $$| $$ \ $$ \ $$| $$  \ $$| $$$$$$$$| $$  \__/|  $$$$$$ 
| $$\  $$$| $$  | $$| $$ | $$ | $$| $$  | $$| $$_____/| $$       \____  $$
| $$ \  $$|  $$$$$$/| $$ | $$ | $$| $$$$$$$/|  $$$$$$$| $$       /$$$$$$$/
|__/  \__/ \______/ |__/ |__/ |__/|_______/  \_______/|__/      |_______/ 
                                                                          
                                                                          
                                                                          
*/

    // add
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator+(newasm::_register<T> lhs, U rhs)
    {
        return lhs.get_value() + rhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator+(U lhs, newasm::_register<T> rhs)
    {
        return rhs.get_value() + lhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator+(newasm::_register<U> lhs, newasm::_register<T> rhs)
    {
        return rhs.get_value() + lhs.get_value();
    }
    //sub
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator-(newasm::_register<T> lhs, U rhs)
    {
        return lhs.get_value() - rhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator-(U lhs, newasm::_register<T> rhs)
    {
        return lhs - rhs.get_value();
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator-(newasm::_register<U> lhs, newasm::_register<T> rhs)
    {
        return rhs.get_value() - lhs.get_value();
    }
    //mul
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator*(newasm::_register<T> lhs, U rhs)
    {
        return lhs.get_value() * rhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator*(U lhs, newasm::_register<T> rhs)
    {
        return rhs.get_value() * lhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator*(newasm::_register<U> lhs, newasm::_register<T> rhs)
    {
        return rhs.get_value() * lhs.get_value();
    }

    //div
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator/(newasm::_register<T> lhs, U rhs)
    {
        return lhs.get_value() / rhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator/(U lhs, newasm::_register<T> rhs)
    {
        return lhs / rhs.get_value();
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator/(newasm::_register<T> lhs, newasm::_register<T> rhs)
    {
        return lhs.get_value() / rhs.get_value();
    }

    //bitwise ops
    // and
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator&(newasm::_register<T> lhs, U rhs)
    {
        return lhs.get_value() & rhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator&(U lhs, newasm::_register<T> rhs)
    {
        return lhs & rhs.get_value();
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator&(newasm::_register<U> lhs, newasm::_register<T> rhs)
    {
        return lhs.get_value() & rhs.get_value();
    }

    // or
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator|(newasm::_register<T> lhs, U rhs)
    {
        return lhs.get_value() | rhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator|(U lhs, newasm::_register<T> rhs)
    {
        return lhs | rhs.get_value();
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator|(newasm::_register<U> lhs, newasm::_register<T> rhs)
    {
        return lhs.get_value() | rhs.get_value();
    }

    // xor
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator^(newasm::_register<T> lhs, U rhs)
    {
        return lhs.get_value() ^ rhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator^(U lhs, newasm::_register<T> rhs)
    {
        return lhs ^ rhs.get_value();
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator^(newasm::_register<U> lhs, newasm::_register<T> rhs)
    {
        return lhs.get_value() ^ rhs.get_value();
    }

    // shift left
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator<<(newasm::_register<T> lhs, U rhs)
    {
        return lhs.get_value() << rhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator<<(U lhs, newasm::_register<T> rhs)
    {
        return lhs << rhs.get_value();
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator<<(newasm::_register<U> lhs, newasm::_register<T> rhs)
    {
        return lhs.get_value() << rhs.get_value();
    }

    // shift right
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator>>(newasm::_register<T> lhs, U rhs)
    {
        return lhs.get_value() >> rhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator>>(U lhs, newasm::_register<T> rhs)
    {
        return lhs >> rhs.get_value();
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    inline auto operator>>(newasm::_register<U> lhs, newasm::_register<T> rhs)
    {
        return lhs.get_value() >> rhs.get_value();
    }

    //not
    template<newasm::concepts::number T>
    inline auto operator ~(newasm::_register<T> rhs)
    {
        return ~rhs.get_value();
    }

    //other
    template<newasm::concepts::number T>
    inline auto operator!(newasm::_register<T> rhs)
    {
        return !rhs.get_value();
    }
}