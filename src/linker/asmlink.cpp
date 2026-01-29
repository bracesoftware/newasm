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
    class Linker
    {
        private:
        static inline void writeFile(const std::string& filename, const std::vector<std::string>& lines)
        {
            std::ofstream file(filename, std::ios::trunc);

            if(!file.is_open())
            {
                newasm::header::functions::linkinfo("System has encountered an error while opening `" + filename + "`.");
                return;
            }

            for (const auto& line : lines) {
                file << line << '\n';
            }

            file.close();
            return;
        }

        template<typename T>
        static inline void replaceVectorElement(std::vector<T>& v, std::vector<T>& n, T value)
        {
            auto it = std::find(v.begin(), v.end(), value);

            if(it != v.end())
            {
                it = v.erase(it);
                v.insert(it, n.begin(), n.end());
            }
            return;
        }
        template<typename T>
        static inline void replaceVectorElement(std::vector<T>& v, std::vector<T>& n, int index)
        {
            if(index < v.size())
            {
                v.erase(v.begin() + index);
                v.insert(v.begin() + index, n.begin(), n.end());
            }
            return;
        }
        static inline std::pair<bool, std::vector<std::string>> readFile(const std::string& filename)
        {
            std::ifstream file(filename);
            std::vector<std::string> lines;

            if(!file.is_open())
            {
                newasm::header::functions::linkinfo("System has encountered an error while opening `" + filename + "`.");
                return {false, lines};
            }

            std::string line;
            while(std::getline(file, line))
            {
                lines.push_back(line);
            }

            file.close();
            return {true, lines};
        }
        static inline std::pair<bool, std::string> linkFile__A(const std::string& line_)
        {
            std::string line = newasm::header::functions::remc(line_);
            static const std::regex pattern("^\\s*link\\s+\"([^\"]+)\"\\s*$");
            std::smatch match;
            if(std::regex_match(line, match, pattern))
            {
                return {true, match[1].str()}; // matchan, vraća tekst
            }

            return {false, ""}; // nije match
        }

        public:
        static inline void link(const std::string& filename, const std::string& outputfilename)
        {
            newasm::header::functions::linkinfo("Processing entry file `" + filename + "`...");
            auto P = newasm::Linker::readFile(filename);
            if(!P.first)
            {
                return;
            }
            auto v = P.second;
            int files = 0;

            std::string temp;
            std::vector<std::string> v2;
            bool temp2;
            for(int i = 0; i < v.size(); ++i)
            {
                temp.clear();
                v2.clear();
                auto p = newasm::Linker::linkFile__A(v[i]);
                if(p.first)
                {
                    auto c = newasm::Linker::readFile(p.second);
                    v2 = c.second;
                    temp2 = c.first;
                    if(!c.first)
                    {
                        continue;
                    }
                    std::cout << newasm::header::col::gray;
                    newasm::header::functions::nullprint("\t Linking `../" + p.second + "`...");
                    newasm::Linker::replaceVectorElement(v, v2, i);

                    files++;
                }
            }

            std::cout << newasm::header::col::reset;

            newasm::header::functions::nullprint("\tSuccessfully linked " + std::to_string(files) + " files.");
            newasm::Linker::writeFile(outputfilename, v);
            return;
        }
    };
}