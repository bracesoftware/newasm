/*

Version: MPL 1.1

The contents of this file are subject to the Mozilla Public License Version 
1.1 the "License"; you may not use this file except in compliance with 
the License. You may obtain a copy of the License at 
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the
License.

Portions created by the Initial Developer are Copyright (c) The COPYRIGHT YEAR
the Initial Developer. All Rights Reserved.

*/

#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif


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
    }
    template<typename T> class _register final
    {
        private:
        std::string name;
        //char name_cexpr[cexpr_size];
        T value;
        newasm::_std::map<std::string, T> thread_values;
        T initial_value;

        bool short_int_value = false;
        
        public:
        _register(std::string regname, T val)
            : name(regname), value(val), initial_value(val){}

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
        inline void reset()
        {
            this->value = this->initial_value;
            for(auto i = this->thread_values.begin(); i != this->thread_values.end(); ++i)
            {
                i->second = this->initial_value;
            }
        }
        inline void reset(std::string thread_name)
        {
            thread_values[thread_name] = initial_value;
        }
        T get_value() const
        {
            if(newasm::thread_line)
            {
                return this->thread_values.at(newasm::threads::now);
            }
            return this->value;
        }
        inline void set_value(const T& new_val_)
        {
            T new_val = new_val_;
            if constexpr(std::is_same_v<T, int>)
            {
                if(this->short_int_value)
                {
                    if(new_val > std::numeric_limits<short>::max() or new_val < std::numeric_limits<short>::min())
                    {
                        new_val = 0;
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
        inline _register<T>& operator=(const T& new_val_)
        {
            T new_val = new_val_;
            if constexpr(std::is_same_v<T, int>)
            {
                if(this->short_int_value)
                {
                    if(new_val > std::numeric_limits<short>::max() or new_val < std::numeric_limits<short>::min())
                    {
                        new_val = 0;
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
        operator T&()
        {
            if(newasm::thread_line)
            {
                return thread_values.at(newasm::threads::now);
            }
            return value;
        }

        operator T() const
        {
            if(newasm::thread_line)
            {
                return thread_values.at(newasm::threads::now);
            }
            return value;
        }
        operator const T&() const
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
                return lhs = rhs.thread_values.at(newasm::threads::now);
            }
            return lhs == rhs.value;
        }

        void add_end_(const std::string& str__)
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

    std::string operator+(const _register<std::string>& reg, const std::string& rhs)
    {
        newasm::internal::ss_.str("");
        newasm::internal::ss_.clear();
        newasm::internal::ss_ << reg.get_value();
        newasm::internal::ss_ << rhs;
        return newasm::internal::ss_.str();
    }
    std::string operator+(const _register<std::string>& reg, const _register<std::string>& reg2)
    {
        newasm::internal::ss_.str("");
        newasm::internal::ss_.clear();
        newasm::internal::ss_ << reg.get_value();
        newasm::internal::ss_ << reg2.get_value();
        return newasm::internal::ss_.str();
    }
    std::string operator+(const std::string& rhs, const _register<std::string>& reg)
    {
        newasm::internal::ss_.str("");
        newasm::internal::ss_.clear();
        newasm::internal::ss_ << rhs;
        newasm::internal::ss_ << reg.get_value();
        return newasm::internal::ss_.str();
    }

    std::string operator+(const std::string& lhs, int rhs)
    {
        newasm::internal::ss_.str("");
        newasm::internal::ss_.clear();
        newasm::internal::ss_ << lhs;
        newasm::internal::ss_ << rhs;
        return newasm::internal::ss_.str();
    }

    std::string operator+(int lhs, const std::string& rhs)
    {
        return std::to_string(lhs) + rhs;
    }

    std::string operator+(const char* lhs, const newasm::_register<std::string>& rhs)
    {
        newasm::internal::ss_.str("");
        newasm::internal::ss_.clear();
        newasm::internal::ss_ << std::string(lhs);
        newasm::internal::ss_ << rhs.get_value();
        return newasm::internal::ss_.str();
    }

    std::string operator+(const newasm::_register<std::string>& lhs, const char* rhs)
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
    auto operator+(newasm::_register<T> lhs, U rhs)
    {
        return lhs.get_value() + rhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator+(U lhs, newasm::_register<T> rhs)
    {
        return rhs.get_value() + lhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator+(newasm::_register<U> lhs, newasm::_register<T> rhs)
    {
        return rhs.get_value() + lhs.get_value();
    }
    //sub
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator-(newasm::_register<T> lhs, U rhs)
    {
        return lhs.get_value() - rhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator-(U lhs, newasm::_register<T> rhs)
    {
        return lhs.get_value() - rhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator-(newasm::_register<U> lhs, newasm::_register<T> rhs)
    {
        return rhs.get_value() - lhs.get_value();
    }
    //mul
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator*(newasm::_register<T> lhs, U rhs)
    {
        return lhs.get_value() * rhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator*(U lhs, newasm::_register<T> rhs)
    {
        return rhs.get_value() * lhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator*(newasm::_register<U> lhs, newasm::_register<T> rhs)
    {
        return rhs.get_value() * lhs.get_value();
    }

    //div
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator/(newasm::_register<T> lhs, U rhs)
    {
        return lhs.get_value() / rhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator/(U lhs, newasm::_register<T> rhs)
    {
        return lhs / rhs.get_value();
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator/(newasm::_register<T> lhs, newasm::_register<T> rhs)
    {
        return lhs.get_value() / rhs.get_value();
    }

    //bitwise ops
    // and
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator&(newasm::_register<T> lhs, U rhs)
    {
        return lhs.get_value() & rhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator&(U lhs, newasm::_register<T> rhs)
    {
        return lhs & rhs.get_value();
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator&(newasm::_register<U> lhs, newasm::_register<T> rhs)
    {
        return lhs.get_value() & rhs.get_value();
    }

    // or
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator|(newasm::_register<T> lhs, U rhs)
    {
        return lhs.get_value() | rhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator|(U lhs, newasm::_register<T> rhs)
    {
        return lhs | rhs.get_value();
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator|(newasm::_register<U> lhs, newasm::_register<T> rhs)
    {
        return lhs.get_value() | rhs.get_value();
    }

    // xor
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator^(newasm::_register<T> lhs, U rhs)
    {
        return lhs.get_value() ^ rhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator^(U lhs, newasm::_register<T> rhs)
    {
        return lhs ^ rhs.get_value();
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator^(newasm::_register<U> lhs, newasm::_register<T> rhs)
    {
        return lhs.get_value() ^ rhs.get_value();
    }

    // shift left
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator<<(newasm::_register<T> lhs, U rhs)
    {
        return lhs.get_value() << rhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator<<(U lhs, newasm::_register<T> rhs)
    {
        return lhs << rhs.get_value();
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator<<(newasm::_register<U> lhs, newasm::_register<T> rhs)
    {
        return lhs.get_value() << rhs.get_value();
    }

    // shift right
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator>>(newasm::_register<T> lhs, U rhs)
    {
        return lhs.get_value() >> rhs;
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator>>(U lhs, newasm::_register<T> rhs)
    {
        return lhs >> rhs.get_value();
    }
    template<newasm::concepts::number T, newasm::concepts::number U>
    auto operator>>(newasm::_register<U> lhs, newasm::_register<T> rhs)
    {
        return lhs.get_value() >> rhs.get_value();
    }

    //not
    template<newasm::concepts::number T>
    auto operator ~(newasm::_register<T> rhs)
    {
        return ~rhs.get_value();
    }

    //other
    template<newasm::concepts::number T>
    auto operator!(newasm::_register<T> rhs)
    {
        return !rhs.get_value();
    }


}