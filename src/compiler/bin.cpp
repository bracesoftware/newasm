// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#define NEWASM_APP_SIGNATURE "__newasm_app"

namespace newasm
{
    namespace compiler
    {
        namespace bin
        {
            //mini helper funcs
            template<typename T>
            static void write_bin(std::ofstream& out, const T& v)
            {
                out.write(reinterpret_cast<const char*>(&v), sizeof(T));
                return;
            }

            template<typename T>
            static void read_bin(std::ifstream& in, T& v)
            {
                in.read(reinterpret_cast<char*>(&v), sizeof(T));
                return;
            }

            static void write_string(std::ofstream& out, const std::string& s)
            {
                uint32_t len = static_cast<uint32_t>(s.size());
                write_bin(out, len);
                out.write(s.data(), len);
                return;
            }

            static void read_string(std::ifstream& in, std::string& s)
            {
                uint32_t len;
                read_bin(in, len);
                s.resize(len);
                in.read(&s[0], len);
                return;
            }
            //real bytecode stuff
            void save_lineData(std::ofstream& out, const newasm::compiler::lineData& ld)
            {
                write_string(out, ld.raw);
                write_bin(out, ld.type);

                uint32_t tokCount = ld.tokens.size();
                write_bin(out, tokCount);
                for(auto& t : ld.tokens)
                {
                    write_string(out, t);
                }

                write_string(out, ld.other);

                write_bin(out, ld.priArgType);
                write_bin(out, ld.altArgType);

                write_bin(out, ld.whatAmIDoing);
                write_bin(out, ld.parsedType);
                write_bin(out, ld.whatCodeSection);
                write_bin(out, ld.letsDecorateVariables);
                write_bin(out, ld.whatTheFuckAreEvents);
                return;
            }
            void load_lineData(std::ifstream& in, newasm::compiler::lineData& ld)
            {
                read_string(in, ld.raw);
                read_bin(in, ld.type);

                uint32_t tokCount;
                read_bin(in, tokCount);
                ld.tokens.resize(tokCount);
                for(auto& t : ld.tokens)
                {
                    read_string(in, t);
                }

                read_string(in, ld.other);

                read_bin(in, ld.whatAmIDoing);
                read_bin(in, ld.parsedType);
                read_bin(in, ld.whatCodeSection);
                read_bin(in, ld.letsDecorateVariables);
                read_bin(in, ld.whatTheFuckAreEvents);
                return;
            }
            void save_labels(std::ofstream& out, const std::unordered_map<std::string, int>& labels)
            {
                uint32_t count = labels.size();
                write_bin(out, count);

                for(auto& [name, addr] : labels)
                {
                    write_string(out, name);
                    write_bin(out, addr);
                }
                return;
            }
            void load_labels(std::ifstream& in, std::unordered_map<std::string, int>& labels)
            {
                uint32_t count;
                read_bin(in, count);

                labels.clear();
                for(uint32_t i = 0; i < count; ++i)
                {
                    std::string name;
                    int addr;
                    read_string(in, name);
                    read_bin(in, addr);
                    labels.emplace(std::move(name), addr);
                }
                return;
            }
            void save_files(std::ofstream& out, const std::vector<std::pair<std::string, int>>& files)
            {
                uint32_t count = files.size();
                write_bin(out, count);

                for(auto& [name, line] : files)
                {
                    write_string(out, name);
                    write_bin(out, line);
                }
                return;
            }
            void load_files(std::ifstream& in, std::vector<std::pair<std::string, int>>& files)
            {
                uint32_t count;
                read_bin(in, count);

                files.resize(count);
                for(auto& [name, line] : files)
                {
                    read_string(in, name);
                    read_bin(in, line);
                }
                return;
            }
            //finally
            inline bool ASSEMBLE(
                const std::string& path,
                const std::vector<newasm::compiler::lineData>& lines,
                const std::unordered_map<std::string, int>& labels,
                const std::vector<std::pair<std::string, int>>& files)
            {
                std::ofstream out(path, std::ios::binary);
                if(!out)
                {
                    return false;
                }

                // MAGIC
                const char magic[] = NEWASM_APP_SIGNATURE;
                out.write(magic, sizeof(magic));

                // VERSION
                write_bin(out, newasm::BUILD_NUMBER);
                write_bin(out, newasm::RUNTIME_VERSION);
                write_bin(out, newasm::KERNEL_VERSION);

                // lineData
                uint32_t lineCount = lines.size();
                write_bin(out, lineCount);
                for(auto& l : lines)
                {
                    save_lineData(out, l);
                }

                // labels
                save_labels(out, labels);

                // files
                save_files(out, files);

                return true;
            }
            bool load_app(
                const std::string& path,
                std::vector<lineData>& lines,
                std::unordered_map<std::string, int>& labels,
                std::vector<std::pair<std::string, int>>& files)
            {
                std::ifstream in(path, std::ios::binary);
                if(!in)
                {
                    return false;
                }

                // MAGIC
                char magic[13] = {};
                in.read(magic, 12);
                if (std::string(magic) != NEWASM_APP_SIGNATURE)
                    return false;

                // VERSION
                uint8_t buildnum, runtimever, krnl;
                read_bin(in, buildnum);
                read_bin(in, runtimever);
                read_bin(in, krnl);

                if(buildnum != newasm::BUILD_NUMBER and runtimever != newasm::RUNTIME_VERSION and krnl != newasm::KERNEL_VERSION)
                {
                    return false;
                }

                // lineData
                uint32_t lineCount;
                read_bin(in, lineCount);
                lines.resize(lineCount);
                for(auto& l : lines)
                {
                    load_lineData(in, l);
                }

                // labels
                load_labels(in, labels);

                // files
                load_files(in, files);

                return true;
            }

        }
    }
}