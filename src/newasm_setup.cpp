// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

module(sys_setup, {
    //setup goes here
});

namespace newasm
{
    namespace code_stream
    {
        int jumpto = 1;
        int jump = 0;
        const int invalid_lnidx = -1;
        namespace sections
        {
            const int text = 1;
            const int data = 2;
            const int start = 3;
        }
    }
    namespace exit_codes
    {
        const int noterm_point = 0;
        const int invalid_section = 1;
        const int invalid_proc = 2;
        const int invalid_retn = 3;
        const int sysreq_fail = 4;
        const int stkhea_col = 5;
        const int data_overflow = 6;
        const int dtyp_mismatch = 7;
        const int label_redef = 8;
        const int bus_err = 9;
        const int invalid_ins = 10;
        const int mem_overflow = 11;
        const int mem_underflow = 12;
        const int proc_redef = 13;
        const int invalid_memacc = 14;
        const int invalid_syntax = 15;
        const int memory_leak = 16;
        const int invalid_config = 17;
        const int inline_proc = 18;
        const int unknown_fdx = 19;
        const int unknown_directive = 20;
        const int namespace_err = 21;
        const int uninptr_usage = 22;
        const int var_redef = 23;
        const int object_redef = 24;
        const int unexpected_cbrace = 25;
        const int undefined_object = 26;
        const int undefined_objectmem = 27;
        const int nested_object = 28;
        const int expected_cbrace = 29;
        const int empty_proc = 30;
        const int datastruct_redef = 31;
        const int unexpected_end = 32;
        const int improper_dynlib = 33;
        const int unknown_event = 34;
        const int invalid_evhndlr = 35;
        const int constant_modif = 36;
        const int os_error = 37;
        const int malloc_err = 38;
        const int seg_fault = 39;
        const int invalid_call = 40;
        const int hndl_reassign = 41;
        const int sysenter_fail = 42;
        const int macro_redef = 43;
        const int unexpected_hash = 44;
        const int undefined_macro = 45;
        const int invalid_sysint = 46;
        const int expected_await = 47;
        const int invalid_thread = 48;
        const int invalid_exp = 49;
        const int vmem_overflow = 50;
        const int tuple_redef = 51;
        const int invalid_tuple = 52;
        const int channel_deadlock = 53;
        const int invalid_alloc = 54;
        const int invalid_init = 55;
        const int linker_err = 56;
        const int jit_fail = 57;
        const int kernel_panic = 58;
        const int immovable_type = 59;

        const std::unordered_map<int, std::string> identifier = {
            {noterm_point, "NoTerminationPoint"},
            {invalid_section, "InvalidSection"},
            {invalid_proc, "InvalidProcedure"},
            {invalid_retn, "InvalidExitAttempt"},
            {sysreq_fail, "SystemRequirementsNotMet"},
            {stkhea_col, "StackHeapCollision"},
            {data_overflow, "DataOverflow"},
            {dtyp_mismatch, "DataTypeMismatch"},
            {label_redef, "LabelRedefinition"},
            {bus_err, "BusError"},
            {invalid_ins, "InvalidASMInstruction"},
            {mem_overflow, "ProgramMemoryOverflow"},
            {mem_underflow, "ProgramMemoryUnderflow"},
            {proc_redef, "ProcedureRedefinition"},
            {invalid_memacc, "InvalidMemoryAccess"},
            {invalid_syntax, "InvalidSyntax"},
            {memory_leak, "HeapMemoryLeak"},
            {invalid_config, "InvalidSysConfiguration"},
            {inline_proc, "InlineProcedure"},
            {unknown_fdx, "UnknownSystemCall"},
            {unknown_directive, "UnknownParserDirective"},
            {uninptr_usage, "UnassignedRefUsage"},
            {var_redef, "VariableRedefinition"},
            {object_redef, "ObjectRedefinition"},
            {unexpected_cbrace, "UnexpectedClosingBrace"},
            {undefined_object, "UndefinedObject"},
            {undefined_objectmem, "UndefinedObjectMember"},
            {nested_object, "NestedObject"},
            {expected_cbrace, "ExpectedClosingBrace"},
            {empty_proc, "EmptyProcedureBlock"},
            {datastruct_redef, "DataStructureRedefinition"},
            {unexpected_end, "UnexpectedProcedureEnd"},
            {improper_dynlib, "ImproperDynLibraryImplementation"},
            {unknown_event, "UnknownEventToHandle"},
            {invalid_evhndlr, "InvalidEventHandler"},
            {constant_modif, "ConstValModification"},
            {os_error, "OSErr"},
            {malloc_err, "MallocErr"},
            {seg_fault, "SegmentationFault"},
            {invalid_call, "InvalidProcCall"},
            {hndl_reassign, "HexReassignment"},
            {sysenter_fail, "SysenterFail"},
            {macro_redef, "MacroRedefinition"},
            {unexpected_hash, "UnexpectedHash"},
            {undefined_macro, "UndefinedMacro"},
            {invalid_sysint, "InvalidSysInterrupt"},
            {expected_await, "ExpectedAwait"},
            {invalid_thread, "InvalidThread"},
            {invalid_exp, "InvalidExpression"},
            {vmem_overflow, "VirtualMemOverflow"},
            {namespace_err, "NamespaceError"},
            {tuple_redef, "TupleRedefinition"},
            {invalid_tuple, "InvalidTuple"},
            {channel_deadlock, "ThreadChannelDeadlock"},
            {invalid_alloc, "InvalidAlloc"},
            {invalid_init, "InvalidVarInitializer"},
            {linker_err, "LinkerError"},
            {jit_fail, "JITCompilerFailure"},
            {kernel_panic, "KernelPanicOrSysCrash"},
            {immovable_type, "ImmovableType"}
        };
    }
    namespace cmp_results
    {
        const int equal = 1;
        const int less = 2;
        const int greater = 3;
    }
    
    namespace constv
    {
        const std::string pxstr = "Program finished with exit code: ";
    }
    namespace system
    {
        bool terminated = false;
        int section = 0;
        int stop = 0;
        int stoproc = 0;
        std::string cproc;
        int proclines = 0;
        std::string original_proc;
        bool mangled_proc;
        std::string processing_proc;
    }
    namespace mem
    {
        namespace inf
        {
            const int max_mem_size = 512;
            int mem_size = 64;
        }
        std::string program_memory[newasm::mem::inf::max_mem_size];
        std::vector<std::string> COD;
        namespace regs
        {
            // non accessible registers
            int exc = 0; // cant be used lol
            newasm::_register<int> lcx("lcx", 0); //controlled with jmp variants
            //mem registers
            newasm::_register<int> stk("stk", 10 * 1024 * 1024 - 1);
            newasm::_register<int> hea("hea", 0);
            //NORMAL REGISTERS
            #define __AX_NAME ("ax_______________@@##??") //experimental
            newasm::_register_FAST ax(__AX_NAME);

            newasm::_register<int> rax("rax", 0); //lmao i've ran out of names
            newasm::_register<float> rbx("rbx", 0);
            newasm::_register<std::string> tlr("tlr", newasm::header::constants::inv_reg_val);
            newasm::_register<std::string> stl("stl", newasm::header::constants::inv_reg_val);

            newasm::_register<std::string> psx("psx", newasm::header::constants::inv_reg_val);
            //newasm::_register<std::string> prp("prp", newasm::header::constants::inv_reg_val);
            newasm::_register<std::string> cpt("cpt", newasm::header::constants::inv_reg_val);

            newasm::_register<std::string> tr0("tr0", newasm::header::constants::inv_reg_val);
            newasm::_register<std::string> tr1("tr1", newasm::header::constants::inv_reg_val);

            newasm::_register<std::string> dlx("dlx", newasm::header::constants::inv_reg_val);
            
            newasm::_register<int> fdx("fdx", 0);
            newasm::_register<int> cpr("cpr", 0);
            newasm::_register<int> br0("br0", 0);
            newasm::_register<int> br1("br1", 0);
            newasm::_register<int> bos("bos", 0);

            newasm::_register<float> cr0("cr0", 0);
            newasm::_register<float> cr1("cr1", 0);
            newasm::_register<int> cr2("cr2", 0);
            newasm::_register<int> cr3("cr3", 0);

            //instruction mode modifier
            newasm::_register<int> imm("imm", 0);

            const int tlr__ = 1;
            const int stl__ = 2;
            const int psx__ = 3;
            //const int prp__ = 4;
            const int imm__ = 4;
            const int cpt__ = 5;

            const int tr0__ = 6;
            const int tr1__ = 7;
            const int dlx__ = 8;
            const int fdx__ = 9;
            const int cpr__ = 10;

            const int br0__ = 11;
            const int br1__ = 12;

            const int bos__ = 13;
            const int cr0__ = 14;
            const int cr1__ = 15;

            const int stk__ = 16;
            const int hea__ = 17;

            const int cr2__ = 18;
            const int cr3__ = 19;

            const int ax__ = 20;
            const int rax__ = 21;
            const int rbx__ = 22;

            std::unordered_map<std::string, int> identifiers = {
                {"tlr", tlr__},
                {"stl", stl__},
                {"psx", psx__},
                //{"prp", prp__},
                {"imm", imm__},
                {"cpt", cpt__},
                {"tr0", tr0__},
                {"tr1", tr1__},
                {"dlx", dlx__},
                {"fdx", fdx__},
                {"cpr", cpr__},
                {"br0", br0__},
                {"br1", br1__},
                {"bos", bos__},
                {"cr0", cr0__},
                {"cr1", cr1__},
                {"stk", stk__},
                {"hea", hea__},
                {"cr2", cr2__},
                {"cr3", cr3__},
                {__AX_NAME, ax__},
                {"rax", rax__},
                {"rbx", rbx__}
            };

            void resetRegisters()
            {
                newasm::mem::regs::stk.reset();
                newasm::mem::regs::hea.reset();
                newasm::mem::regs::imm.reset();

                //newasm::hardware::randAccessMem.init();

                newasm::mem::regs::tlr.reset();
                newasm::mem::regs::stl.reset();

                newasm::mem::regs::psx.reset();
                //newasm::mem::regs::prp.reset();
                newasm::mem::regs::cpt.reset();

                newasm::mem::regs::tr0.reset();
                newasm::mem::regs::tr1.reset();

                newasm::mem::regs::fdx.reset();
                newasm::mem::regs::cpr.reset();
                newasm::mem::regs::br0.reset();
                newasm::mem::regs::br1.reset();

                newasm::mem::regs::cr0.reset();
                newasm::mem::regs::cr1.reset();
                newasm::mem::regs::cr2.reset();
                newasm::mem::regs::cr3.reset();

                newasm::mem::regs::dlx.reset();
                newasm::mem::regs::rax.reset();

                //PRIVATE
                newasm::mem::regs::lcx.reset();
            }
            void resetRegisters(const std::string& thread_name)
            {
                newasm::mem::regs::stk.reset(thread_name);
                newasm::mem::regs::imm.reset(thread_name);
                newasm::mem::regs::hea.reset(thread_name);
                newasm::mem::regs::tlr.reset(thread_name);
                newasm::mem::regs::stl.reset(thread_name);

                newasm::mem::regs::psx.reset(thread_name);
                //newasm::mem::regs::prp.reset(thread_name);
                newasm::mem::regs::cpt.reset(thread_name);

                newasm::mem::regs::tr0.reset(thread_name);
                newasm::mem::regs::tr1.reset(thread_name);

                newasm::mem::regs::fdx.reset(thread_name);
                newasm::mem::regs::cpr.reset(thread_name);
                newasm::mem::regs::br0.reset(thread_name);
                newasm::mem::regs::br1.reset(thread_name);

                newasm::mem::regs::cr0.reset(thread_name);
                newasm::mem::regs::cr1.reset(thread_name);
                newasm::mem::regs::cr2.reset(thread_name);
                newasm::mem::regs::cr3.reset(thread_name);

                newasm::mem::regs::dlx.reset(thread_name);
                newasm::mem::regs::rax.reset(thread_name);

                //PRIVATE
                newasm::mem::regs::lcx.reset(thread_name);
            }
        }

        struct struct_member
        {
            int datatype;
            std::string name;
            std::string value;
        };
        ////////
        std::unordered_map<std::string, std::string> data;
        std::unordered_map<std::string, int> datatypes;
        struct attrib
        {
            std::vector<std::string> namespaces;
            bool locked = false;
        };
        std::unordered_map<std::string, newasm::mem::attrib> data_attrib;
        ///////////
        struct tuple_data
        {
            std::vector<std::string> contents;
        };
        std::unordered_map<std::string, newasm::mem::tuple_data> tuple;

        //std::map<std::string, std::vector<newasm::compiler::lineData>> funcs;
        struct procData
        {
            std::string original_name;
            bool mangled = false;
        };
        std::unordered_map<std::string, newasm::mem::procData> funcs_data;

        std::unordered_map<std::string, std::vector<std::string>> instructions;

        std::unordered_map<std::string, int> labels;

        std::unordered_map<std::string, std::vector<newasm::mem::struct_member>> structs;
        //std::vector<std::string> uninitialized_pointer;

        namespace functions
        {
            #if 0
            bool setup_memsize(int size)
            {
                if(size > newasm::mem::inf::max_mem_size)
                {
                    return false;
                }
                newasm::mem::inf::mem_size = size;
                newasm::mem::regs::hea = 0;
                newasm::mem::regs::heaptr = 0;
                newasm::mem::regs::stk = size-1;
                for(int i = 0; i < size; ++i)
                {
                    newasm::mem::program_memory[i] = newasm::header::constants::nullstr;
                }
                return true;
            }
            #endif
            inline bool check_stkhea_col() noexcept
            {
                return (!(newasm::mem::regs::hea < newasm::mem::regs::stk));
            }
            template<typename T>
            inline bool datavalid(const std::string& dataname, const T& dat)
            {
                return dat.find(dataname) != dat.end();
                /*for(auto it = dat.begin(); it != dat.end(); ++it)
                {
                    if(it->first == dataname)
                    {
                        return true;
                    }
                }
                return false;*/
            }
            inline bool islbln(int line)
            {
                for(std::unordered_map<std::string, int>::iterator i = labels.begin(); i != labels.end(); ++i)
                {
                    if(i->second == line)
                    {
                        return true;
                    }
                }
                return false;
            }
        }
    }
    // absolutely normal structure
    namespace variables
    {
        struct unionData final
        {
            int addr;
        };

        struct tupleData final
        {
            std::vector<int> addr;
            std::vector<int> type;
        };

        struct classData final
        {
            std::vector<int> addr; // i love C++
        };

        struct contextData final
        {
            std::vector<std::string> keys;
            std::vector<int> addr;
            std::vector<int> type;
        };

        struct eventData final
        {
            std::vector<int> addr;
        };

        struct staticObjectData final
        {
            short addr = 0;
        };

        class threadData final
        {
            public int addr = 0;
        };

        class procedureData final
        {
            private bool prepared = false;
            public std::vector<newasm::compiler::lineData> contents;
            std::unordered_map<std::string, int> labels;
            int idx;

            std::string original_name;
            bool mangled = false;

            public explicit inline procedureData() noexcept {}
            inline ~procedureData() noexcept {}

            public inline void JIT_compile()
            {
                if(this->prepared)
                {
                    return;
                }
                this->prepared = true;

                if(this->contents.empty())
                {
                    return;
                }

                this->labels.max_load_factor(MAX_LOAD_FACTOR);

                for(int i = 0; i < this->contents.size(); ++i)
                {
                    if(this->contents.at(i).type == newasm::compiler::sealedLabel)
                    {
                        //std::cout << "Successfully added label: `" << this->contents.at(i).other << "`" << std::endl;
                        auto k = this->contents.at(i).other;
                        this->labels[k] = i;
                        newasm::sealedLabels->push_back(k);
                        this->contents.at(i).type = newasm::compiler::empty;
                        continue;
                    }
                }
                for(int i = 0; i < this->contents.size(); ++i)
                {
                    auto& bytecode = this->contents.at(i);
                    if(
                        bytecode.whatAmIDoing == newasm::core::lang_inf::jmp or
                        bytecode.whatAmIDoing == newasm::core::lang_inf::jz or
                        bytecode.whatAmIDoing == newasm::core::lang_inf::jnz or
                        bytecode.whatAmIDoing == newasm::core::lang_inf::je or
                        bytecode.whatAmIDoing == newasm::core::lang_inf::jne or
                        bytecode.whatAmIDoing == newasm::core::lang_inf::jl or
                        bytecode.whatAmIDoing == newasm::core::lang_inf::jle or
                        bytecode.whatAmIDoing == newasm::core::lang_inf::jg or
                        bytecode.whatAmIDoing == newasm::core::lang_inf::jge
                    )
                    {
                        if(bytecode.tokens.size() != 2)
                        {
                            continue;
                        }
                        auto& label_name = bytecode.tokens[1];
                        if(this->labels.find(label_name) == this->labels.end())
                        {
                            newasm::terminate(newasm::exit_codes::bus_err);
                            break;
                        }
                        bytecode.jumpinTo = this->labels.at(label_name);
                        continue;
                    }
                    if(
                        bytecode.whatAmIDoing == newasm::core::lang_inf::loop
                    )
                    {
                        if(bytecode.tokens.size() != 3)
                        {
                            continue;
                        }
                        auto& label_name = bytecode.tokens[2];
                        if(this->labels.find(label_name) == this->labels.end())
                        {
                            newasm::terminate(newasm::exit_codes::bus_err);
                            break;
                        }
                        bytecode.jumpinTo = this->labels.at(label_name);
                        continue;
                    }
                }

                //std::cout << "JIT compiler successfully compiled " << this->labels.size() << " labels in proc " << this->original_name <<  "." << std::endl;
                return;
            }
        };

        VarTable ids;
    }
}

namespace newasm
{
    namespace header
    {
        namespace functions
        {
            @maybe_unused
            inline void parseRegDeref_EXPERIMENTAL(std::string& arg)
            {
                std::cout << "Called parseRegDeref -> " << (*newasm::PRC)->tokens.size() << "; ";
                std::cout << ((*newasm::PRC)->priArgType == newasm::datatypes::_regDeref) << "; " << ((*newasm::PRC)->altArgType == newasm::datatypes::_regDeref);
                std::cout << std::endl;
                bool priCheck = (priRegDeref && (*newasm::PRC)->tokens.size() == 2 && (*newasm::PRC)->priArgType == newasm::datatypes::_regDeref);
                bool altCheck = (!priRegDeref && (*newasm::PRC)->tokens.size() == 3 && (*newasm::PRC)->altArgType == newasm::datatypes::_regDeref);
                if(!(priCheck or altCheck))
                {
                    return;
                }
                std::cout << "priCheck = " << priCheck << "; altCheck = " << altCheck << std::endl;
                unsigned int RID = (-1);
                if(priCheck)
                {
                    RID = (*newasm::PRC)->priInt;
                }
                if(altCheck)
                {
                    RID = (*newasm::PRC)->altInt;
                }

                static auto RID__ = RID;
                static auto arg__ = arg;
                $defer
                    std::cout << "RID = " << RID__ << std::endl;
                    std::cout << "arg = `" << arg__ << "`" << std::endl;
                $

                if(true)
                {
                    switch(RID)
                    {
                        case newasm::mem::regs::ax__:
                        {
                            auto t = newasm::mem::regs::ax.get_type();
                            (*newasm::PRC)->resType = t;
                            if(t == newasm::datatypes::number)
                            {
                                (*newasm::PRC)->resInt = newasm::mem::regs::ax.get_value<int>();
                                break;
                            }
                            if(t == newasm::datatypes::decimal)
                            {
                                (*newasm::PRC)->resFloat = newasm::mem::regs::ax.get_value<float>();
                                break;
                            }
                            if(t == newasm::datatypes::character)
                            {
                                (*newasm::PRC)->resChar = newasm::mem::regs::ax.get_value<char>();
                                break;
                            }
                            if(t == newasm::datatypes::text)
                            {
                                (*newasm::PRC)->resString = newasm::mem::regs::ax.get_value<std::string>();
                                break;
                            }
                            break;
                        }
                        case newasm::mem::regs::tlr__:
                        {
                            arg = newasm::mem::regs::tlr.get_value();
                            //std::cout << "arg = `" << arg << "`" << std::endl;
                            break;
                        }
                        case newasm::mem::regs::stl__:
                        {
                            arg = newasm::mem::regs::stl.get_value();
                            break;
                        }
                        case newasm::mem::regs::psx__:
                        {
                            arg = newasm::mem::regs::psx.get_value();
                            break;
                        }
                        #if 0
                        case newasm::mem::regs::prp__:
                        {
                            arg = newasm::mem::regs::prp.get_value();
                            break;
                        }
                        #endif
                        case newasm::mem::regs::cpt__:
                        {
                            arg = newasm::mem::regs::cpt.get_value();
                            break;
                        }
                        case newasm::mem::regs::tr0__:
                        {
                            arg = newasm::mem::regs::tr0.get_value();
                            break;
                        }
                        case newasm::mem::regs::tr1__:
                        {
                            arg = newasm::mem::regs::tr1.get_value();
                            break;
                        }
                        case newasm::mem::regs::dlx__:
                        {
                            arg = newasm::mem::regs::dlx.get_value();
                            break;
                        }
                        case newasm::mem::regs::fdx__:
                        {
                            arg = std::to_string(newasm::mem::regs::fdx.get_value());
                            break;
                        }
                        case newasm::mem::regs::cpr__:
                        {
                            arg = std::to_string(newasm::mem::regs::cpr.get_value());
                            break;
                        }
                        case newasm::mem::regs::br0__:
                        {
                            arg = std::to_string(newasm::mem::regs::br0.get_value());
                            break;
                        }
                        case newasm::mem::regs::br1__:
                        {
                            arg = std::to_string(newasm::mem::regs::br1.get_value());
                            break;
                        }
                        case newasm::mem::regs::bos__:
                        {
                            arg = std::to_string(newasm::mem::regs::bos.get_value());
                            break;
                        }
                        case newasm::mem::regs::cr0__:
                        {
                            arg = std::to_string(newasm::mem::regs::cr0.get_value());
                            break;
                        }
                        case newasm::mem::regs::cr1__:
                        {
                            arg = std::to_string(newasm::mem::regs::cr1.get_value());
                            break;
                        }
                        case newasm::mem::regs::cr2__:
                        {
                            arg = std::to_string(newasm::mem::regs::cr2.get_value());
                            break;
                        }
                        case newasm::mem::regs::cr3__:
                        {
                            arg = std::to_string(newasm::mem::regs::cr3.get_value());
                            break;
                        }
                        case newasm::mem::regs::imm__:
                        {
                            arg = std::to_string(newasm::mem::regs::imm.get_value());
                            break;
                        }
                        case newasm::mem::regs::rax__:
                        {
                            arg = std::to_string(newasm::mem::regs::rax.get_value());
                            break;
                        }
                        case newasm::mem::regs::stk__:
                        {
                            arg = std::to_string(newasm::mem::regs::stk.get_value());
                            break;
                        }
                        case newasm::mem::regs::hea__:
                        {
                            arg = std::to_string(newasm::mem::regs::hea.get_value());
                            break;
                        }
                        default:
                        {
                            newasm::terminate(newasm::exit_codes::os_error);
                            break;
                        }
                    }
                }
                return;
            }

            ATTR_HOT inline void parseRegDeref(std::string& arg)
            {
                if(newasm::header::functions::trim(arg).at(0) == '*' && arg.find('/') == std::string::npos)
                {
                    auto it = newasm::mem::regs::identifiers.find(newasm::header::functions::trim(arg.substr(1)));
                    #if 0
                    if(it == newasm::mem::regs::identifiers.end())
                    {
                        newasm::terminate(newasm::exit_codes::bus_err);
                        return;
                    }
                    #endif

                    switch(it->second)
                    {
                        case newasm::mem::regs::ax__:
                        {
                            auto t = newasm::mem::regs::ax.get_type();
                            (*newasm::PRC)->resType = t;
                            if(t == newasm::datatypes::number)
                            {
                                (*newasm::PRC)->resInt = newasm::mem::regs::ax.get_value<int>();
                                break;
                            }
                            if(t == newasm::datatypes::decimal)
                            {
                                (*newasm::PRC)->resFloat = newasm::mem::regs::ax.get_value<float>();
                                break;
                            }
                            if(t == newasm::datatypes::character)
                            {
                                (*newasm::PRC)->resChar = newasm::mem::regs::ax.get_value<char>();
                                break;
                            }
                            if(t == newasm::datatypes::text)
                            {
                                (*newasm::PRC)->resString = newasm::mem::regs::ax.get_value<std::string>();
                                break;
                            }
                            break;
                        }
                        case newasm::mem::regs::tlr__:
                        {
                            arg = newasm::mem::regs::tlr.get_value();
                            //std::cout << "arg = `" << arg << "`" << std::endl;
                            break;
                        }
                        case newasm::mem::regs::stl__:
                        {
                            arg = newasm::mem::regs::stl.get_value();
                            break;
                        }
                        case newasm::mem::regs::psx__:
                        {
                            arg = newasm::mem::regs::psx.get_value();
                            break;
                        }
                        #if 0
                        case newasm::mem::regs::prp__:
                        {
                            arg = newasm::mem::regs::prp.get_value();
                            break;
                        }
                        #endif
                        case newasm::mem::regs::cpt__:
                        {
                            arg = newasm::mem::regs::cpt.get_value();
                            break;
                        }
                        case newasm::mem::regs::tr0__:
                        {
                            arg = newasm::mem::regs::tr0.get_value();
                            break;
                        }
                        case newasm::mem::regs::tr1__:
                        {
                            arg = newasm::mem::regs::tr1.get_value();
                            break;
                        }
                        case newasm::mem::regs::dlx__:
                        {
                            arg = newasm::mem::regs::dlx.get_value();
                            break;
                        }
                        case newasm::mem::regs::fdx__:
                        {
                            arg = std::to_string(newasm::mem::regs::fdx.get_value());
                            break;
                        }
                        case newasm::mem::regs::cpr__:
                        {
                            arg = std::to_string(newasm::mem::regs::cpr.get_value());
                            break;
                        }
                        case newasm::mem::regs::br0__:
                        {
                            arg = std::to_string(newasm::mem::regs::br0.get_value());
                            break;
                        }
                        case newasm::mem::regs::br1__:
                        {
                            arg = std::to_string(newasm::mem::regs::br1.get_value());
                            break;
                        }
                        case newasm::mem::regs::bos__:
                        {
                            arg = std::to_string(newasm::mem::regs::bos.get_value());
                            break;
                        }
                        case newasm::mem::regs::cr0__:
                        {
                            arg = std::to_string(newasm::mem::regs::cr0.get_value());
                            break;
                        }
                        case newasm::mem::regs::cr1__:
                        {
                            arg = std::to_string(newasm::mem::regs::cr1.get_value());
                            break;
                        }
                        case newasm::mem::regs::cr2__:
                        {
                            arg = std::to_string(newasm::mem::regs::cr2.get_value());
                            break;
                        }
                        case newasm::mem::regs::cr3__:
                        {
                            arg = std::to_string(newasm::mem::regs::cr3.get_value());
                            break;
                        }
                        case newasm::mem::regs::imm__:
                        {
                            arg = std::to_string(newasm::mem::regs::imm.get_value());
                            break;
                        }
                        case newasm::mem::regs::rax__:
                        {
                            arg = std::to_string(newasm::mem::regs::rax.get_value());
                            break;
                        }
                        case newasm::mem::regs::rbx__:
                        {
                            arg = std::to_string(newasm::mem::regs::rbx.get_value());
                            break;
                        }
                        case newasm::mem::regs::stk__:
                        {
                            arg = std::to_string(newasm::mem::regs::stk.get_value());
                            break;
                        }
                        case newasm::mem::regs::hea__:
                        {
                            arg = std::to_string(newasm::mem::regs::hea.get_value());
                            break;
                        }
                        default:
                        {
                            newasm::terminate(newasm::exit_codes::os_error);
                            break;
                        }
                    }
                }
                
                return;
            }

            void parseAddressOf(std::string& s)
            {
                if(s.empty())
                {
                    return;
                }
                if(s.at(0) != '#')
                {
                    return;
                }

                auto suf = "&" + s.substr(1);
                newasm::runtime::functions::parse(suf);
                auto it = newasm::variables::ids.find(suf.substr(1));
                if(it == newasm::variables::ids.end())
                {
                    newasm::terminate(newasm::exit_codes::invalid_memacc);
                    return;
                }
                
                s = std::to_string(it->second.addr);

                if(it->second.type == newasm::datatypes::yunion)
                {
                    s = std::to_string(it->second.yunion->addr);
                }
                if(it->second.type == newasm::datatypes::blueprint)
                {
                    s = std::to_string(it->second.blueprint->addr[0]);
                }
                if(it->second.type == newasm::datatypes::tuple)
                {
                    s = std::to_string(it->second.tuple->addr[0]);
                }
                if(it->second.type == newasm::datatypes::proc)
                {
                    s = std::to_string(it->second.addr);//procedures aren't saved in same ram segment as vars
                }
                if(it->second.type == newasm::datatypes::mycontext)
                {
                    s = std::to_string(it->second.context->addr[0]);
                }
                return;
            }
            
        }
    }
}