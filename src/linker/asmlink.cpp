// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm
{
    class Linker
    {
        private static inline void writeFile(const std::string& filename, const std::vector<std::string>& lines)
        {
            std::ofstream file(filename, std::ios::trunc);

            if(!file.is_open())
            {
                newasm::header::functions::linkinfo("System has encountered an error while opening `" + filename + "`.");
                return;
            }

            for(const auto& line : lines)
            {
                file << line << '\n';
            }

            file.close();
            return;
        }

        template<typename T>
        private static inline void replaceVectorElement(std::vector<T>& v, std::vector<T>& n, T value)
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
        private static inline void replaceVectorElement(std::vector<T>& v, std::vector<T>& n, int index)
        {
            if(index < v.size())
            {
                v.erase(v.begin() + index);
                v.insert(v.begin() + index, n.begin(), n.end());
            }
            return;
        }
        template<typename T>
        private static inline void replaceVectorElement__NEW(std::vector<T>& v, const std::vector<T>& n, std::size_t index)
        {
            if (index >= v.size()) return;

            v.insert(v.erase(v.begin() + index), n.begin(), n.end());
        }

        template<bool anyExt>
        private static inline std::pair<bool, std::vector<std::string>> readFile(const std::string& filename)
        {
            std::string filename_;
            if constexpr(anyExt == false)
            {
                filename_ = filename;
            }
            if constexpr(anyExt == true)
            {
                static const std::vector<std::string> extensions = {
                    "",
                    ".newasm",
                    ".asm",
                    ".nasm"
                };
                for(int i = 0; i < extensions.size(); ++i)
                {
                    if(std::filesystem::exists(filename + extensions.at(i)))
                    {
                        filename_ = filename + extensions.at(i);
                        break;
                    }
                }
            }
            std::ifstream file(filename_);
            std::vector<std::string> lines;

            if(!file.is_open())
            {
                newasm::header::functions::linkinfo("System has encountered an error while opening `" + filename + "` with extension `" + filename_ + "`.");
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
        private static inline std::pair<bool, std::string> linkFile__A(const std::string& line_)
        {
            std::string line = newasm::header::functions::remc(line_);
            static const std::regex pattern("^\\s*link\\s+\"([^\"]+)\"\\s*$");
            std::smatch match;
            if(std::regex_match(line, match, pattern))
            {
                //std::cout << "linkFile__A -> aj idi u pizd mat" << std::endl;
                return {true, match[1].str()}; // matchan, vraća tekst
            }

            return {false, ""}; // nije match
        }

        public static inline void link(const std::string& filename, const std::string& outputfilename)
        {
            newasm::header::functions::linkinfo("Processing entry file `" + filename + "`...");
            auto P = newasm::Linker::readFile<false>(filename);
            if(!P.first)
            {
                return;
            }
            auto v = P.second;
            int files = 0;
            newasm::forLinker__OLD::files[1] = filename;
            newasm::forLinker__OLD::file_sizes[filename] = v.size();

            for(int i = 0; i < v.size(); ++i)
            {
                __newasmDBG(std::cout << i << ": " << v.at(i) << "->" << filename << ":" << i+1 << std::endl)
                newasm::forLinker::lineData.push_back({filename, i + 1});
            }

            int linetemp;
            std::string temp;
            std::vector<std::string> v2;
            std::vector<std::pair<std::string, int>> lineDataLocal;
            bool temp2;
            for(int i = 0; i < v.size(); ++i)
            {
                temp.clear();
                v2.clear();
                lineDataLocal.clear();
                auto p = newasm::Linker::linkFile__A(v[i]);
                if(p.first)
                {
                    auto c = newasm::Linker::readFile<true>(p.second);
                    v2 = c.second;
                    temp2 = c.first;
                    if(!c.first)
                    {
                        continue;
                    }
                    std::cout << newasm::header::col::gray;
                    newasm::header::functions::nullprint("\t Linking `../" + p.second + "`...");
                    newasm::Linker::replaceVectorElement__NEW(v, v2, i);

                    for(int j = 0; j < v2.size(); ++j)
                    {
                        __newasmDBG(std::cout << j << ": " << v2.at(j) << "->" << p.second << ":" << j+1 << std::endl)
                        lineDataLocal.push_back({p.second, j + 1});
                    }

                    newasm::Linker::replaceVectorElement__NEW(newasm::forLinker::lineData, lineDataLocal, i);

                    newasm::forLinker__OLD::file_sizes[p.second] = v2.size();

                    newasm::forLinker__OLD::files[i] = p.second;
                    newasm::forLinker__OLD::files[i + v2.size() + 1] = filename;
                    files++;
                }
            }

            std::cout << newasm::header::col::reset;

            newasm::header::functions::nullprint("\tSuccessfully linked " + std::to_string(files) + " files.");
            newasm::Linker::writeFile(outputfilename, v);
            newasm::forLinker__OLD::linked_size = v.size();
            return;
        }
    };
}