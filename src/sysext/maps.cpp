// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm
{
    namespace _std
    {
        template <typename T>
        std::string TYPENAME()
        {
            return typeid(T).name();
        }

        template<typename K, typename V>
        class map extends std::unordered_map<K, V>
        {
            public V& at(const K& key)
            {
                auto it = std::unordered_map<K, V>::find(key);
                if(it == std::unordered_map<K, V>::end())
                {
                    return (*this)[key];
                }
                return it->second;
            }
            public const V& at(const K& key
                #if NEWASM_DBG_SOURCE_LOC == true
                , const std::source_location loc = std::source_location::current()
                #endif
            ) const
            {
                auto it = std::unordered_map<K, V>::find(key);
                if(it == std::unordered_map<K, V>::end())
                {
                    //std::cout << "realline->`" << newasm::real_line << "`\n";
                    #if NEWASM_DBG_SOURCE_LOC == true
                    std::cout << std::endl;
                    std::cout << "==========================================" << std::endl;
                    std::cout << "File: " << loc.file_name() << std::endl;
                    std::cout << "Line: " << newasm::_std::to_string(loc.line()) << std::endl;
                    std::cout << "Func: " << loc.function_name() << std::endl;
                    std::cout << "==========================================" << std::endl;

                    std::cout << std::endl;
                    #endif
                    std::cout << "Key is type: " << TYPENAME<K>() << std::endl;
                    throw std::out_of_range("Key \"" + key + "\" not found in const `::at`.");
                }
                return it->second;
            }
        };

        template<typename T>
        class vec final
        {
            using _T = typename std::conditional<std::is_same<T, bool>::value, unsigned char, T>::type;
            public mutable std::vector<_T> v;

            public explicit inline vec()
            {
                v.resize(100);
            }

            public ATTR_FLAT inline void debug() const
            {
                for(size_t i = 0; i < this->v.size(); ++i)
                {
                    std::cout
                        << "\033[38;2;191;0;255m"
                        << "vec[" << i << "] : ";

                    if constexpr(requires(std::ostream& os, const T& value)
                    {
                        os << value;
                    })
                    {
                        std::cout << v[i];
                    }
                    else
                    {
                        std::cout << "<not streamable>";
                    }

                    std::cout << std::endl;
                }
            }

            public ATTR_FLAT inline void debug()
            {
                for(size_t i = 0; i < this->v.size(); ++i)
                {
                    std::cout
                        << "\033[38;2;191;0;255m"
                        << "vec[" << i << "] : ";

                    if constexpr(requires(std::ostream& os, const T& value)
                    {
                        os << value;
                    })
                    {
                        std::cout << v[i];
                    }
                    else
                    {
                        std::cout << "<not streamable>";
                    }

                    std::cout << std::endl;
                }
            }

            private ATTR_FLAT inline void ensure_capacity(unsigned int index)
            {
                if(index >= this->v.size())
                {
                    this->v.resize(index + 10);
                }
            }

            private ATTR_FLAT inline void ensure_capacity(unsigned int index) const
            {
                if(index >= this->v.size())
                {
                    this->v.resize(index + 10);
                }
            }

            public FORCE_INLINE inline unsigned int size()
            {
                return v.size();
            }

            public FORCE_INLINE inline T& operator[](unsigned int index)
            {
                if constexpr(NEWASM_BUG_CRISIS) std::cout << "================= OPERATOR [] ===================" << std::endl;
                if constexpr(NEWASM_BUG_CRISIS) std::cout << "\033[38;2;255;105;180m" << "index: "<< index << std::endl;
                ensure_capacity(index);
                return reinterpret_cast<T&>(v[index]);
            }

            FORCE_INLINE inline T& at(unsigned int index)
            {
                if constexpr(NEWASM_BUG_CRISIS) std::cout << "================= AT ===================" << std::endl;
                if constexpr(NEWASM_BUG_CRISIS) std::cout << "\033[38;2;255;105;180m" << "index: "<< index << std::endl;
                ensure_capacity(index);
                return reinterpret_cast<T&>(v.at(index));
            }

            FORCE_INLINE inline const T& operator[](unsigned int index) const
            {
                if constexpr(NEWASM_BUG_CRISIS) std::cout << "================= OPERATOR [] CONST ===================" << std::endl;
                if constexpr(NEWASM_BUG_CRISIS) std::cout << "\033[38;2;255;105;180m" << "index: "<< index << std::endl;
                ensure_capacity(index);
                return reinterpret_cast<const T&>(v[index]);
            }

            FORCE_INLINE inline const T& at(unsigned int index) const
            {
                if constexpr(NEWASM_BUG_CRISIS) std::cout << "================= AT CONST ===================" << std::endl;
                if constexpr(NEWASM_BUG_CRISIS) std::cout << "\033[38;2;255;105;180m" << "index: "<< index << std::endl;
                ensure_capacity(index);
                return reinterpret_cast<const T&>(v.at(index));
            }

            FORCE_INLINE inline auto begin()
            {
                return v.begin();
            }

            FORCE_INLINE inline auto end()
            {
                return v.end();
            }

            FORCE_INLINE inline auto clear()
            {
                return v.clear();
            }
        };
    }
}
