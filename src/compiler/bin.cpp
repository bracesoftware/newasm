// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

__newasm_LOAD_PACKAGE_MODULE(binary_gen, {
    //setup goes here
});

#define NEWASM_APP_SIGNATURE "__newasm_app"
#define NEWASM_SIGNATURE_SIZE 12

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

                write_bin(out, ld.priEvalMode.type);
                write_bin(out, ld.priEvalMode.argInt);
                write_bin(out, ld.priEvalMode.argFloat);
                write_bin(out, ld.priEvalMode.argChar);
                write_string(out, ld.priEvalMode.argString);
                write_string(out, ld.priEvalMode.argString2);

                write_bin(out, ld.priInt);
                write_bin(out, ld.priFloat);
                write_bin(out, ld.priChar);
                write_string(out, ld.priString);

                write_bin(out, ld.altArgType);

                write_bin(out, ld.altEvalMode.type);
                write_bin(out, ld.altEvalMode.argInt);
                write_bin(out, ld.altEvalMode.argFloat);
                write_bin(out, ld.altEvalMode.argChar);
                write_string(out, ld.altEvalMode.argString);
                write_string(out, ld.altEvalMode.argString2);

                write_bin(out, ld.altInt);
                write_bin(out, ld.altFloat);
                write_bin(out, ld.altChar);
                write_string(out, ld.altString);

                write_bin(out, ld.caseLineArgType);

                write_bin(out, ld.whatAmIDoing);
                write_bin(out, ld.parsedType);
                write_bin(out, ld.whatCodeSection);
                write_bin(out, ld.letsDecorateVariables);
                write_bin(out, ld.whatTheFuckAreEvents);
                write_bin(out, ld.attribute);
                write_bin(out, ld.whatAreRegistersLol);

                write_bin(out, ld.krnlMod);
                write_bin(out, ld.jumpinTo);

                write_bin(out, ld.VirtualMemoryAccess);
                write_bin(out, ld.caseTableAddress);

                write_bin(out, ld.priEvalMode.argType);
                write_bin(out, ld.altEvalMode.argType);
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

                read_bin(in, ld.priArgType);
                
                read_bin(in, ld.priEvalMode.type);
                read_bin(in, ld.priEvalMode.argInt);
                read_bin(in, ld.priEvalMode.argFloat);
                read_bin(in, ld.priEvalMode.argChar);
                read_string(in, ld.priEvalMode.argString);
                read_string(in, ld.priEvalMode.argString2);

                read_bin(in, ld.priInt);
                read_bin(in, ld.priFloat);
                read_bin(in, ld.priChar);
                read_string(in, ld.priString);

                read_bin(in, ld.altArgType);

                read_bin(in, ld.altEvalMode.type);
                read_bin(in, ld.altEvalMode.argInt);
                read_bin(in, ld.altEvalMode.argFloat);
                read_bin(in, ld.altEvalMode.argChar);
                read_string(in, ld.altEvalMode.argString);
                read_string(in, ld.altEvalMode.argString2);

                read_bin(in, ld.altInt);
                read_bin(in, ld.altFloat);
                read_bin(in, ld.altChar);
                read_string(in, ld.altString);

                read_bin(in, ld.caseLineArgType);

                read_bin(in, ld.whatAmIDoing);
                read_bin(in, ld.parsedType);
                read_bin(in, ld.whatCodeSection);
                read_bin(in, ld.letsDecorateVariables);
                read_bin(in, ld.whatTheFuckAreEvents);
                read_bin(in, ld.attribute);
                read_bin(in, ld.whatAreRegistersLol);

                read_bin(in, ld.krnlMod);
                read_bin(in, ld.jumpinTo);

                read_bin(in, ld.VirtualMemoryAccess);
                read_bin(in, ld.caseTableAddress);

                read_bin(in, ld.priEvalMode.argType);
                read_bin(in, ld.altEvalMode.argType);
                return;
            }
            //label addresses
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
            //file data for each line
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
            //newasm_dl
            void save_dynlibs(std::ofstream& out, const std::unordered_map<std::string, std::vector<std::string>>& labels)
            {
                uint32_t count = labels.size();
                write_bin(out, count);

                for(auto& [name, vec] : labels)
                {
                    write_string(out, name);
                    uint32_t vec_size = vec.size();
                    write_bin(out, vec_size);
                    for(uint32_t i = 0; i < vec_size; ++i)
                    {
                        write_string(out, vec.at(i));
                    }
                }
                return;
            }
            void load_dynlibs(std::ifstream& in, std::unordered_map<std::string, std::vector<std::string>>& labels)
            {
                uint32_t count;
                read_bin(in, count);

                labels.clear();
                for(uint32_t i = 0; i < count; ++i)
                {
                    std::string temp;
                    std::string name;
                    uint32_t content_size;
                    std::vector<std::string> content;
                    read_string(in, name);
                    read_bin(in, content_size);
                    for(uint32_t i = 0; i < content_size; ++i)
                    {
                        read_string(in, temp);
                        content.push_back(temp);
                    }
                    labels.emplace(std::move(name), std::move(content));
                }
                return;
            }
            //finally
            inline bool ASSEMBLE(
                const std::string& path,
                const std::vector<newasm::compiler::lineData>& lines,
                const std::unordered_map<std::string, int>& labels,
                const std::vector<std::pair<std::string, int>>& files,
                const std::unordered_map<std::string, std::vector<std::string>>& ins,
                const std::vector<newasm::compiler::lineData>& jumptable
            )
            {
                std::ofstream out(path, std::ios::binary);
                if(!out)
                {
                    return false;
                }

                // MAGIC
                out.write(NEWASM_APP_SIGNATURE, NEWASM_SIGNATURE_SIZE);

                // VERSION
                int buildnum = newasm::BUILD_NUMBER;
                int runtimever = newasm::RUNTIME_VERSION;
                int krnl = newasm::KERNEL_VERSION;

                write_bin(out, buildnum);
                write_bin(out, runtimever);
                write_bin(out, krnl);

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

                //used kernel modules
                write_bin(out, newasm::kernel::cfg::IOStream);
                write_bin(out, newasm::kernel::cfg::Extensions);
                write_bin(out, newasm::kernel::cfg::Thread);
                write_bin(out, newasm::kernel::cfg::Chrono);
                write_bin(out, newasm::kernel::cfg::Network);
                write_bin(out, newasm::kernel::cfg::Memory);
                write_bin(out, newasm::kernel::cfg::TextOperations);
                write_bin(out, newasm::kernel::cfg::ContainerManipulation);
                write_bin(out, newasm::kernel::cfg::FileStream);
                write_bin(out, newasm::kernel::cfg::Tuple);
                write_bin(out, newasm::kernel::cfg::TCProtocol);
                write_bin(out, newasm::kernel::cfg::HTTP);
                write_bin(out, newasm::kernel::cfg::Math);
                write_bin(out, newasm::kernel::cfg::Misc);
                write_bin(out, newasm::kernel::cfg::Crypto);
                write_bin(out, newasm::kernel::cfg::Context);
                write_bin(out, newasm::kernel::cfg::FileStream_VDSK);

                //dynamic libs (*.newasm_dl files)
                save_dynlibs(out, ins);

                //project data
                write_string(out, newasm::project_data::name);
                write_string(out, newasm::project_data::version);

                //jump table
                uint32_t jumpTableSize = jumptable.size();
                write_bin(out, jumpTableSize);
                for(auto& l : jumptable)
                {
                    save_lineData(out, l);
                }
                return true;
            }

            constinit const int UNKNOWN_ERROR = 1;
            constinit const int INVALID_APP = 2;
            constinit const int INCOMPATIBLE_APP = 3;

            void exit_load(const std::string& path, int exit_code)
            {
                static const std::unordered_map<int, std::string> loading_errors = {
                    {UNKNOWN_ERROR, "unknown error or no such file found"},
                    {INVALID_APP, "not a valid NewASM application"},
                    {INCOMPATIBLE_APP, "not a compatible NewASM binary format"}
                };

                newasm::header::functions::err("Failed to load the application.\n");

                if(loading_errors.find(exit_code) == loading_errors.end())
                {
                    return;
                }

                newasm::header::functions::nullprint(newasm::header::col::gray + "\t\t" + path + ": " + loading_errors.at(exit_code) + "\n\n");
                return;
            }

            bool load_app(
                const std::string& path,
                std::vector<lineData>& lines,
                std::unordered_map<std::string, int>& labels,
                std::vector<std::pair<std::string, int>>& files,
                std::unordered_map<std::string, std::vector<std::string>>& ins,
                std::vector<lineData>& jumptable
            )
            {
                std::ifstream in(path, std::ios::binary);
                if(!in)
                {
                    newasm::compiler::bin::exit_load(path, newasm::compiler::bin::UNKNOWN_ERROR);
                    return false;
                }

                // MAGIC
                char magic[NEWASM_SIGNATURE_SIZE];
                in.read(magic, NEWASM_SIGNATURE_SIZE);
                if(std::memcmp(magic, NEWASM_APP_SIGNATURE, NEWASM_SIGNATURE_SIZE) != 0)
                {
                    newasm::compiler::bin::exit_load(path, newasm::compiler::bin::INVALID_APP);
                    return false;
                }

                // VERSION
                int buildnum, runtimever, krnl;
                read_bin(in, buildnum);
                read_bin(in, runtimever);
                read_bin(in, krnl);

                if((buildnum != newasm::BUILD_NUMBER) or (runtimever != newasm::RUNTIME_VERSION) or (krnl != newasm::KERNEL_VERSION))
                {
                    __newasmDBG_COMPLEX({
                        std::cout << "buildnum: " << buildnum << std::endl;
                        std::cout << "runtimever: " << runtimever << std::endl;
                        std::cout << "krnl: " << krnl << std::endl;
                    });
                    newasm::compiler::bin::exit_load(path, newasm::compiler::bin::INCOMPATIBLE_APP);
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

                //used kernel modules
                read_bin(in, newasm::kernel::cfg::IOStream);
                read_bin(in, newasm::kernel::cfg::Extensions);
                read_bin(in, newasm::kernel::cfg::Thread);
                read_bin(in, newasm::kernel::cfg::Chrono);
                read_bin(in, newasm::kernel::cfg::Network);
                read_bin(in, newasm::kernel::cfg::Memory);
                read_bin(in, newasm::kernel::cfg::TextOperations);
                read_bin(in, newasm::kernel::cfg::ContainerManipulation);
                read_bin(in, newasm::kernel::cfg::FileStream);
                read_bin(in, newasm::kernel::cfg::Tuple);
                read_bin(in, newasm::kernel::cfg::TCProtocol);
                read_bin(in, newasm::kernel::cfg::HTTP);
                read_bin(in, newasm::kernel::cfg::Math);
                read_bin(in, newasm::kernel::cfg::Misc);
                read_bin(in, newasm::kernel::cfg::Crypto);
                read_bin(in, newasm::kernel::cfg::Context);
                read_bin(in, newasm::kernel::cfg::FileStream_VDSK);
                
                //dynamic libs (*.newasm_dl files)
                load_dynlibs(in, ins);

                //project data
                read_string(in, newasm::project_data::name);
                read_string(in, newasm::project_data::version);

                //jump table
                uint32_t jumpTableSize;
                read_bin(in, jumpTableSize);
                jumptable.resize(jumpTableSize);
                for(auto& l : jumptable)
                {
                    load_lineData(in, l);
                }
                return true;
            }

        }
    }
}