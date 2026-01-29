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
        template<typename T, typename U>
        class linear_map
        {
            private:
            std::vector<std::pair<T, U>> __map__;
            public:
            U& operator[](T index)
            {
                for(auto& p : __map__)
                {
                    if(p.first == index)
                    {
                        return p.second;
                    }
                }

                __map__.push_back({index, U{}});
                return __map__.at(__map__.size() - 1).second;
            }

            inline int size()
            {
                return __map__.size();
            }

            inline std::pair<T, U> operator()(int index)
            {
                if(index >= this->size())
                {
                    throw std::out_of_range("linear_map: Function `this::operator()` cannot reach index " + std::to_string(index) + ".");
                    return {__map__.at(0).first, __map__.at(0).second};
                }
                auto p = __map__.at(index);
                return p;
            }

            inline void debug__()
            {
                for(auto& p : __map__)
                {
                    std::cout << "["<<p.first<<"] = " << p.second << std::endl;
                }

                return;
            }

            const U& at(T index, int offset) const
            {
                for(int i = 0; i < __map__.size(); ++i)
                {
                    if(__map__[i].first == index)
                    {
                        auto pos = i - offset;
                        if(pos < 0 || pos >= __map__.size())
                        {
                            throw std::out_of_range("linear_map: Invalid offset (which is " + std::to_string(offset) + ").");
                        }
                        return __map__[pos].second;
                    }
                }
                std::stringstream ss;
                ss << "linear_map: Key not found in `::at(T, int)`.\n\tT = ";
                ss << index;
                ss << ", int = ";
                ss << offset;
                ss << "\n";
                throw std::out_of_range(ss.str());
            }

            inline const U& __(T index, int offset) const
            {
                for(int i = 0; i < __map__.size(); ++i)
                {
                    if(__map__[i].first == index)
                    {
                        auto pos = i - offset;
                        if(pos < 0 || pos >= __map__.size())
                        {
                            throw std::out_of_range("linear_map: Invalid offset (which is " + std::to_string(offset) + ").");
                        }
                        return __map__[pos].first;
                    }
                }
                throw std::out_of_range("linear_map: Key not found in `::__(T, int)`.");
            }

            inline void erase(T index)
            {
                for(int i = 0; i < __map__.size(); ++i)
                {
                    if(__map__[i].first == index)
                    {
                        __map__.erase(__map__.begin() + i);
                        return;
                    }
                }
            }

            inline const U& at(T index) const
            {
                for(auto& p : __map__)
                {
                    if(p.first == index)
                    {
                        return p.second;
                    }
                }
                throw std::out_of_range("linear_map: Key not found in `::at(T)`.");
            }
        };
    }
}