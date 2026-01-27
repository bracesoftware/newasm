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
        static inline std::vector<std::string> readFile(const std::string& filename)
        {
            std::ifstream file(filename);
            std::vector<std::string> lines;

            if(!file.is_open())
            {
                newasm::header::functions::linkinfo("System has encountered an error while opening `" + filename + "`.");
                return lines;
            }

            std::string line;
            while(std::getline(file, line))
            {
                lines.push_back(line);
            }

            file.close();
            return lines;
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
            auto v = newasm::Linker::readFile(filename);
            int files = 0;

            std::string temp;
            std::vector<std::string> v2;
            for(int i = 0; i < v.size(); ++i)
            {
                temp.clear();
                v2.clear();
                auto p = newasm::Linker::linkFile__A(v[i]);
                if(p.first)
                {
                    files++;
                    v2 = newasm::Linker::readFile(p.second);
                    newasm::header::functions::nullprint("\tLinking `../" + p.second + "`...");
                    newasm::Linker::replaceVectorElement(v, v2, i);
                }
            }

            newasm::header::functions::nullprint("\tSuccessfully linked " + std::to_string(files) + " files.");
            newasm::Linker::writeFile(outputfilename, v);
            return;
        }
    };
}