// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm
{
    namespace _std
    {
        template<typename K, typename V>
        class map : public std::unordered_map<K, V>
        {
            public:
            V& at(const K& key) {
                auto it = std::unordered_map<K, V>::find(key);
                if(it == std::unordered_map<K, V>::end())
                {
                    return (*this)[key];
                }
                return it->second;
            }
            const V& at(const K& key) const
            {
                auto it = std::unordered_map<K, V>::find(key);
                if(it == std::unordered_map<K, V>::end())
                {
                    throw std::out_of_range("Key \"" + key + "\" not found in const `::at`.");
                }
                return it->second;
            }
        };
    }
}
