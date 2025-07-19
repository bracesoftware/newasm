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
    template<typename T> class _register
    {
        private:
        std::string name;
        T value;
        public:
        _register(std::string regname, T val)
            : name(regname), value(val)
        {
        }
        std::string identifier() const
        {
            return name;
        }
        T get_value() const
        {
            return this->value;
        }
        void set_value(T new_val)
        {
            this->value = new_val;
        }
        T& ref_value()
        {
            return value;
        }
        _register<T>& operator=(const T& new_val)
        {
            value = new_val;
            return *this;
        }
        operator T&()
        {
            return value;
        }

        operator T() const
        {
            return value;
        }
        operator const T&() const
        {
            return value;
        }
        friend std::ostream& operator<<(std::ostream& os, const _register<T>& r)
        {
            os << r.value;
            return os;
        }
        friend std::istream& operator>>(std::istream& is, _register<T>& r)
        {
            is >> r.value;
            return is;
        }

    };
    template<typename T> std::string operator+(const _register<T>& reg, const std::string& rhs)
    {
        return reg.get_value() + rhs;
    }
    template<typename T> std::string operator+(const std::string& rhs, const _register<T>& reg)
    {
        return reg.get_value() + rhs;
    }

    std::string operator+(const std::string& lhs, int rhs)
    {
        return lhs + std::to_string(rhs);
    }

    // int + string
    std::string operator+(int lhs, const std::string& rhs)
    {
        return std::to_string(lhs) + rhs;
    }

    template<typename T> std::string operator+(const char* lhs, const newasm::_register<T>& rhs)
    {
        return std::string(lhs) + rhs.get_value();
    }

    template<typename T> std::string operator+(const newasm::_register<T>& lhs, const char* rhs)
    {
        return lhs.get_value() + std::string(rhs);
    }

}