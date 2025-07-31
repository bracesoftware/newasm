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
                    throw std::out_of_range("Key not found in const `::at`.");
                }
                return it->second;
            }
        };
    }
}
