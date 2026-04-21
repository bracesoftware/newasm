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
                    std::cout << "Line: " << std::to_string(loc.line()) << std::endl;
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
    }
}
