// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain


#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

module(virtual_cpu, {
    NewASM::Modules::PrintLine("Virtual CPU successfully plugged in...");
});

namespace newasm
{
    void handle_threads();
    int compile_and_exec(std::string file, int lineidx_____);
    int compile(const std::string& file);
    static inline int terminate_(int exit_code, const std::source_location& loc);

    inline void unsins(std::string ins)
    {
        newasm::header::functions::wrn(
            "Instruction `"_str +
            newasm::header::style::underline +
            ins + newasm::header::col::reset +
            "` is not supported in the REPL mode."_str
        );
        return;
    }
    inline void unsins_repl(std::string ins)
    {
        newasm::header::functions::wrn(
            "Instruction `"_str +
            newasm::header::style::underline +
            ins + newasm::header::col::reset +
            "` is not supported outside the REPL mode."_str
        );
        return;
    }
    inline void __CRASH__()
    {
        newasm::header::functions::err("Application crashed.");
        return;
    }
    inline int terminate(int exit_code, const std::source_location loc)
    {
        //std::cout << "\nTERMINATE CALLED -> lmao -> " << exit_code << std::endl;
        if constexpr(0) if(newasm::handling_exit)
        {
            newasm::__CRASH__();
            return 1;
        }

        NewASM::perf::heavyHostServices.start();
        $defer
            NewASM::perf::heavyHostServices.stop();
        $

        NewASM::variables::procedureData* p = nullptr;
        if(newasm::header::data::proc_now) p = NewASM::CurrentProcA->proc;
        else if(newasm::LambdaDispatch::LambdaLine) p = &(*newasm::LambdaDispatch::ThreadSafePtr);
        if(p)
        {
            if(p->TryBlock)
            {
                p->TryBlock = false;
                p->TryCatched = true;
                p->idx = p->TryJump - 1;
                newasm::mem::regs::rax.set_value(exit_code);
                NewASM::RemoveExceptionComment();
                return 1;
            }
        }

        if(newasm::thread_line)
        {
            auto& mmap = newasm::threads::memory.at(newasm::threads::now);
            if(mmap->TryBlock)
            {
                //std::cout << "Error catched in thread -> " << exit_code << std::endl;
                mmap->TryBlock = false;
                mmap->TryCatched = true;
                mmap->lcx = mmap->TryJump - 1;
                //std::cout << "Type -> " << mmap->contents.at(mmap->TryJump).whatAmIDoing << std::endl;
                newasm::mem::regs::rax.set_value(exit_code);

                if(p) p->forceShutdown();
                NewASM::RemoveExceptionComment();
                return 1;
            }
        }

        if(NewASM::header::data::TryBlock)
        {
            NewASM::header::data::TryBlock = false;
            NewASM::header::data::TryCatched = true;
            NEWASM_JMP__(newasm::header::data::TryJump)
            newasm::mem::regs::rax.set_value(exit_code);

            if(p) p->forceShutdown();
            NewASM::RemoveExceptionComment();
            return 1;
        }

        newasm::terminate_(exit_code, loc);
        if constexpr(0) if(newasm::header::functions::trim(NewASM::ExceptionHandling::Line->raw) == std::string("syscall"))
        {
            newasm::header::functions::krnl("Kernel crashed.");
        }
        return 1;
    }
    inline int terminate(const std::string& exit_code, const std::source_location loc = std::source_location::current())
    {
        if(NewASM::header::functions::isnumeric(exit_code))
        {
            NewASM::terminate((int)std::stoi(exit_code), loc);
            return 1;
        }
        if(NewASM::header::functions::isfloat(exit_code))
        {
            NewASM::terminate(static_cast<int>(std::round(std::stof(exit_code))), loc);
            return 1;
        }
        NewASM::terminate(NewASM::exit_codes::invalid_retn, loc);
        return 1;
    }
    inline int terminate(float exit_code, const std::source_location loc = std::source_location::current())
    {
        NewASM::terminate(static_cast<int>(std::round(exit_code)), loc);
        return 1;
    }
    //int redirect_exec(std::string filename);
    static inline int terminate_(int exit_code, const std::source_location& loc)//, std::string line)
    {
        const std::string Insomnia = "\t\t\t  ";
        auto LogExceptionSourceLoc = <:loc, Insomnia:>() -> void {
            if(NewASM::MutableConfig::DisplaySourceInformation)
            {
                std::cout << Insomnia << newasm::header::col::reset << newasm::header::col::light_red;
                std::cout << "^ exception source information -> " << newasm::header::col::gray << newasm::header::style::underline;
                std::cout << loc.file_name() << ":" << loc.line() << ":" << loc.column() << newasm::header::col::reset << "\n\t" << Insomnia;
                std::cout << newasm::header::col::light_red << '`' << newasm::header::col::gray;
                std::cout << loc.function_name() << newasm::header::col::light_red << '`';
                std::cout << newasm::header::col::reset << newasm::header::col::gray;
                std::cout << std::endl;
                std::cout << newasm::header::col::reset;
            }
        };
        auto LogComment = <:Insomnia:>() -> void {
            if(NewASM::header::data::ExceptionComment)
            {
                NewASM::header::data::ExceptionComment = false;
                std::cout << Insomnia << newasm::header::col::reset << newasm::header::col::orange;
                std::cout << "^ comment -> \"" << newasm::header::col::gray;
                std::cout << newasm::GetExceptionComment();
                std::cout << newasm::header::col::orange << '"';
                std::cout << std::endl;
                std::cout << newasm::header::col::reset;
            }
        };
        auto LogDeclarationSource = <:Insomnia:>(const std::string& name, int idx) -> void {
            std::cout << Insomnia << newasm::header::col::reset << newasm::header::col::red;
            std::cout << newasm::header::style::dim;
            std::cout << "^ `" << name << "` declared in: " << newasm::header::style::underline;
            std::cout << newasm::header::col::gray <<
                    newasm::forLinker::getFile(idx) <<//(newasm::header::data::lastlndx) << //newasm::header::settings::script_file <<
                    ":" <<
                    newasm::forLinker::getLine(idx);
            std::cout << std::endl;
            std::cout << newasm::header::col::reset;
        };
        bool temp_proc = false;
        std::cout << std::endl;
        //std::cout << "TERMINATEEE" << std::endl;
        if(newasm::header::data::repl)
        {
            newasm::header::data::exception = true;
            if(newasm::header::data::exception)
            {
                std::cout <<
                "\t\t" <<
                newasm::header::col::red <<
                "Exception \"" <<
                newasm::header::col::gray<<
                newasm::header::style::underline<<
                newasm::exit_codes::identifier.at(exit_code)<<
                newasm::header::col::reset<<
                newasm::header::col::red << "\" occured >> "<<
                newasm::header::col::gray<<
                newasm::header::data::LastLine->raw << std::endl;
                LogExceptionSourceLoc();
                LogComment();

                std::cout << newasm::header::col::reset << std::endl;
            }
            return 1;
        }

        if(!newasm::system::terminated)
        {
            newasm::mem::regs::exc = exit_code;
        }

        newasm::system::terminated = true;

        if(newasm::header::data::exception)
        {
            std::cout <<
            "\t\t" <<
            newasm::header::col::red <<
            "Exception \"" <<
            newasm::header::col::gray<<
            newasm::header::style::underline<<
            newasm::exit_codes::identifier.at(exit_code)<<
            newasm::header::col::reset <<
            newasm::header::col::red <<
            "\" in ";

            //if(newasm::header::execution_flow::exec_redirected == false)
            if(
                newasm::header::data::proc_now == false and
                newasm::thread_line == false and
                newasm::events::exitNow == false
            )
            {
                if(newasm::header::data::LoadingStdlib)
                {
                    std::cout << newasm::header::constants::std_library;
                }
                else
                {
                    std::cout <<
                    newasm::header::col::gray <<
                    newasm::header::style::bold <<
                    newasm::header::style::underline <<
                    newasm::forLinker::getFile(newasm::mem::regs::lcx.get_value()) <<//(newasm::header::data::lastlndx) << //newasm::header::settings::script_file <<
                    ":" <<
                    newasm::forLinker::getLine(newasm::mem::regs::lcx.get_value());//(newasm::header::data::lastlndx);
                }
            }
            if(newasm::header::data::proc_now == true)
            {
                std::cout << "procedure " <<
                newasm::header::col::gray <<
                newasm::header::style::bold <<
                newasm::header::style::underline <<
                newasm::system::processing_proc;
                std::cout << ':' << NewASM::CurrentProcA->proc->idx + 1;
                temp_proc = true;
            }
            if(newasm::thread_line == true)
            {
                if(temp_proc)
                {
                    std::cout << newasm::header::col::reset;
                    std::cout << newasm::header::col::red;
                    std::cout << "...\n";
                    std::cout << Insomnia << Insomnia;
                    std::cout << "in ";
                }
                auto& t = newasm::threads::now;
                auto& mmap = newasm::threads::memory.at(t);
                std::cout << "thread " <<
                newasm::header::col::gray <<
                newasm::header::style::bold <<
                newasm::header::style::underline <<
                t;
                std::cout << ':' << mmap->lcx + 1;
                //temp_proc = true;
            }
            if(newasm::events::exitNow == true)
            {
                std::cout << "event handler for " <<
                newasm::header::col::gray <<
                newasm::header::style::bold <<
                newasm::header::style::underline <<
                "'termination'";
                //temp_proc = true;
            }

            std::cout <<
            newasm::header::col::reset <<
            newasm::header::col::red <<
            " >> " <<
            newasm::header::col::gray <<
            newasm::header::data::LastLine->raw << 
            std::endl;

            std::cout << newasm::header::col::reset;

            if(newasm::thread_line)
            {
                auto& t = newasm::threads::now;
                auto& mmap = newasm::threads::memory.at(t);
                LogDeclarationSource(t, mmap->LCX);
            }
            if(temp_proc)
            {
                auto& k = NewASM::CurrentProcA->proc;
                if(k->mangled)
                {
                    std::cout << "\t\t\t  " << newasm::header::col::reset << newasm::header::col::light_blue;
                    std::cout << "^ original procedure name: \"" << newasm::header::col::gray << newasm::header::style::underline;
                    std::cout << k->original_name;
                    std::cout << newasm::header::col::reset << newasm::header::col::light_blue << "\"";
                    std::cout << std::endl;
                    std::cout << newasm::header::col::reset;
                }
                LogDeclarationSource(k->original_name, k->LCX);
            }

            if(NewASM::ExceptionHandling::Line->MacroComponent)
            {
                std::cout << "\t\t\t  " << newasm::header::col::reset << newasm::header::col::magenta;
                std::cout << "^ after expanding macro: \"" << newasm::header::col::gray << newasm::header::style::underline;
                std::cout << NewASM::ExceptionHandling::Line->SourceMacroName;
                std::cout << newasm::header::col::reset << newasm::header::col::magenta << "\"";
                std::cout << std::endl;
                std::cout << newasm::header::col::reset;
            }

            std::cout << "\t\t\t  " << newasm::header::col::reset << newasm::header::col::yellow;
            std::cout << "^ backtrace: \"" << newasm::header::col::gray << newasm::header::style::underline;
            std::cout << NewASM::ExceptionHandling::Line->raw;
            std::cout << newasm::header::col::reset << newasm::header::col::yellow << "\"";
            std::cout << std::endl;
            std::cout << newasm::header::col::reset;

            if(newasm::LambdaDispatch::LambdaLine)
            {
                std::cout << "\t\t\t  " << newasm::header::col::reset << newasm::header::col::magenta;
                std::cout << "^ in lambda/anonymous procedure";
                std::cout << std::endl;
                std::cout << newasm::header::col::reset;
            }
            LogExceptionSourceLoc();
            LogComment();
        }
        return 1;
    }

    inline int parseopr_struct(std::string& opr)
    {
        auto objectData = newasm::header::functions::parseObject(opr);
        if(objectData.first)
        {
            std::string member_name = newasm::header::functions::trim(objectData.second.second);
            std::string struct_name = newasm::header::functions::trim(objectData.second.first);

            if(newasm::header::functions::parseNamespaceSegments(struct_name).first)
            {
                newasm::progwin::api::cout("Object<yes> NMS -> " + struct_name);
                auto i = newasm::header::functions::parseNamespaceSegments(struct_name);
                std::string symbol_name = i.second.back();
                auto vec = i.second;
                vec.pop_back(); // namespace list
                
                struct_name = newasm::header::functions::mangleName(vec, symbol_name);
            }

            if(!newasm::header::functions::isalphanum(struct_name))
            {
                return 1;
            }

            if(!newasm::mem::functions::datavalid(struct_name, newasm::mem::structs))
            {
                //std::cout << "struct_name: " << struct_name << std::endl;
                newasm::terminate(newasm::exit_codes::undefined_object);
                return 1;
            }
            bool member_found = false;
            std::vector<newasm::mem::struct_member>::iterator i;
            for(i = newasm::mem::structs[struct_name].begin(); i !=newasm::mem::structs[struct_name].end(); ++i)
            {
                if(i->name == member_name)
                {
                    member_found = true;
                    break;
                }
            }
            if(!member_found)
            {
                newasm::terminate(newasm::exit_codes::undefined_objectmem);
                return 1;
            }
            opr = i->value;
            try {if(newasm::mem::data_attrib.at(struct_name).locked)
            {
                opr = "\"unknown??\"";
            }}
            catch(std::exception& e)
            {
                std::cerr << e.what() << std::endl;
                std::cout << "Zajebucnuo si se sa ovim!\n";
            }
            return 1;
        }
        return 1;
    }
    
    inline void process_s(std::string& section, newasm::compiler::lineData& line)
    {
        
        #if 0
        if(section == static_cast<std::string>("data"))
        {
            newasm::garbageCollector::DO();
            newasm::system::section = newasm::code_stream::sections::data;
            return;
        }
        if(section == static_cast<std::string>("start"))
        {
            newasm::garbageCollector::DO();
            newasm::system::section = newasm::code_stream::sections::start;
            return;
        }
        if(section == static_cast<std::string>("hndl"))
        {
            newasm::garbageCollector::DO();
            newasm::system::section = newasm::code_stream::sections::hndl;
            return;
        }
        if(section == static_cast<std::string>("text"))
        {
            newasm::garbageCollector::DO();
            newasm::system::section = newasm::code_stream::sections::text;
            return;
        }
        #endif
        return;
    }
    #if 0
    int process_s_(bool &valid, std::string wholeline, std::string stat, std::string arg)
    {
        if(newasm::system::stop == 1)
        {
            newasm::terminate(newasm::exit_codes::invalid_syntax);
            return 1;
        }
        valid = false;
        if(stat == static_cast<std::string>("_"))
        {
            if(arg == static_cast<std::string>("data"))
            {
                newasm::system::section = newasm::code_stream::sections::data;
                valid = true;
                return 1;
            }
            if(arg == static_cast<std::string>("start"))
            {
                newasm::system::section = newasm::code_stream::sections::start;
                valid = true;
                return 1;
            }
            if(arg == static_cast<std::string>("hndl"))
            {
                newasm::system::section = newasm::code_stream::sections::hndl;
                valid = true;
                return 1;
            }
            if(arg == static_cast<std::string>("text"))
            {
                newasm::system::section = newasm::code_stream::sections::text;
                valid = true;
                return 1;
            }
            newasm::terminate(newasm::exit_codes::invalid_section);
            return 1;
        }
        return 1;
    }
    #endif
    inline int process_l(std::string arg, int lineidx)
    {
        if(newasm::mem::functions::datavalid(arg, newasm::mem::labels))
        {
            //newasm::terminate(newasm::exit_codes::label_redef);//,wholeline);
            newasm::compiler::abort(newasm::compiler::fail::label_redef);
            return 1;
        }
        newasm::mem::labels[arg] = lineidx;
        return 1;
    }
    #if 0
    static bool namespaceCollision(std::string name)
    {
        if(newasm::nms::count != newasm::mem::data_attrib[name].namespaces.size())
        {
            return false;
        }
        if(newasm::nms::count == newasm::mem::data_attrib.at(name).namespaces.size())
        {
            for(int i = 0; i < newasm::nms::count; ++i)
            {
                if(newasm::nms::stack.at(i) != newasm::mem::data_attrib.at(name).namespaces.at(i))
                {
                    return false;
                }
            }
        }
        return true;
    }
    #endif
    int process_d(
        const std::string& wholeline, const std::string& dtyp, const std::string& _name,
        std::string& value, newasm::compiler::lineData& line
    )
    {
        if(line.priArgType == newasm::datatypes::symbol_name)
        {
            newasm::runtime::functions::eval(value, line.priEvalMode);
        }
        //std::cout << dtyp << ":" << name << ":" << value << std::endl;
        std::string name = _name;
        if(!newasm::header::data::struct_now) if(newasm::nms::count != 0)
        {
            //std::cout << "original name: `" << name << "`, ";
            name = newasm::header::functions::mangleName(newasm::nms::stack, name);
            //std::cout << "mangled name: `" << name << "`" << std::endl;
        }
        #if 0
        if(!newasm::header::data::struct_now) if(newasm::mem::functions::datavalid(name, newasm::mem::data))
        {
            if(true)
            {
                newasm::terminate(newasm::exit_codes::var_redef);
                return 1;
            }
        }
        if(!newasm::header::data::struct_now) if(newasm::mem::functions::datavalid(name, newasm::variables::ids))
        {
            if(true)
            {
                newasm::terminate(newasm::exit_codes::var_redef);
                return 1;
            }
        }
        if(!newasm::header::data::struct_now) if(newasm::mem::functions::datavalid(name, newasm::mem::structs))
        {
            if(true)//if(newasm::namespaceCollision(name))
            {
                newasm::terminate(newasm::exit_codes::object_redef);
                return 1;
            }
        }
        if(!newasm::header::data::struct_now) if(newasm::mem::functions::datavalid(name, newasm::threads::memory))
        {
            if(true)//if(newasm::namespaceCollision(name))
            {
                newasm::terminate(newasm::exit_codes::object_redef);
                return 1;
            }
        }
        if(!newasm::header::data::struct_now) if(newasm::mem::functions::datavalid(name, newasm::mem::tuple))
        {
            if(true)//if(newasm::namespaceCollision(name))
            {
                newasm::terminate(newasm::exit_codes::tuple_redef);
                return 1;
            }
        }
        #endif
        #if 0
        auto it = newasm::inverted_types.find(dtyp);
        if(it == newasm::inverted_types.end())
        {
            newasm::terminate(newasm::exit_codes::invalid_syntax);
            return 1;
        }
        #endif
        switch(line.parsedType)
        {
            //objects
            case newasm::core::lang_inf::typenames::obj:
            {
                if(!newasm::brace_stack__.empty())
                {
                    newasm::terminate(newasm::exit_codes::nested_object);
                    return 1;
                }
                
                if(value != OPEN_BRACE_STR)
                {
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }

                newasm::header::data::struct_now = true;
                newasm::header::data::struct_decl = name;
                newasm::mem::data_attrib[name].locked = newasm::expcfg::lockbool;

                newasm::variables::ids[name].type = newasm::datatypes::static_objz;
                auto& mmap = newasm::variables::ids.at(name);

                mmap.obj = new newasm::variables::staticObjectData;
                mmap.attrib = newasm::runtime::currentAttributes;

                newasm::runtime::currentAttributes = 0;
                
                newasm::brace_stack__.push_back(newasm::brace_stack::object_block);
                return 1;
            }
            // whole numbers
            case newasm::core::lang_inf::typenames::num:
            {
                if(newasm::header::data::struct_now)
                {
                    if(!newasm::header::functions::isnumeric(value))
                    {
                        //newasm::progwin::api::cout("IT HAS TO BE A NUMBER (ZajebucnuoSiSeException) -> " + value);
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    newasm::mem::structs[newasm::header::data::struct_decl].push_back({newasm::datatypes::number, name, value});
                    return 1;
                }

                if(line.priArgType == newasm::datatypes::symbol_name) if(!newasm::header::functions::isnumeric(value))
                {
                    //std::cout << "Imamo error -> `" << value << "`->" << line.priEvalMode.type << "\n";
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                if(line.priArgType != newasm::datatypes::symbol_name) if(line.priArgType != newasm::datatypes::number)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                
                //newasm::mem::datatypes[name] = newasm::datatypes::number;
                //newasm::mem::data[name] = value;
                //newasm::mem::data_attrib[name].locked = newasm::expcfg::lockbool;
                //_newasm_addnamespaces(name)

                newasm::variables::ids[name].type = newasm::datatypes::number;
                auto& mmap = newasm::variables::ids.at(name);
                mmap.locked = newasm::expcfg::lockbool;
                mmap.transient__ = newasm::expcfg::transientbool;
                mmap.attrib = newasm::runtime::currentAttributes;
                newasm::runtime::currentAttributes = 0;

                if(newasm::expcfg::transientbool)
                {
                    newasm::garbage::FLAG = 1;
                    newasm::garbage::addr__.push_back(name);
                }

                if(line.priArgType == newasm::datatypes::symbol_name)
                {
                    mmap.addr = newasm::hardware::randAccessMem.write<int>(std::stoi(value));
                }
                if(line.priArgType == newasm::datatypes::number)
                {
                    mmap.addr = newasm::hardware::randAccessMem.write<int>(line.priInt);
                }
                return 1;
            }
            //decimal numbers
            case newasm::core::lang_inf::typenames::decm:
            {
                if(newasm::header::data::struct_now)
                {
                    if(!newasm::header::functions::isfloat(value))
                    {
                        //newasm::progwin::api::cout("IT HAS TO BE A NUMBER (ZajebucnuoSiSeException) -> " + value);
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    newasm::mem::structs[newasm::header::data::struct_decl].push_back({newasm::datatypes::decimal, name, value});
                    return 1;
                }
                if(line.priArgType == newasm::datatypes::symbol_name) if(!newasm::header::functions::isfloat(value))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                if(line.priArgType != newasm::datatypes::symbol_name) if(line.priArgType != newasm::datatypes::decimal)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                //newasm::mem::datatypes[name] = newasm::datatypes::decimal;
                //newasm::mem::data[name] = value;
                //newasm::mem::data_attrib[name].locked = newasm::expcfg::lockbool;
                //_newasm_addnamespaces(name)

                newasm::variables::ids[name].type = newasm::datatypes::decimal;
                auto& mmap = newasm::variables::ids.at(name);
                mmap.locked = newasm::expcfg::lockbool;
                mmap.transient__ = newasm::expcfg::transientbool;
                mmap.attrib = newasm::runtime::currentAttributes;
                newasm::runtime::currentAttributes = 0;

                if(newasm::expcfg::transientbool)
                {
                    newasm::garbage::FLAG = 1;
                    newasm::garbage::addr__.push_back(name);
                }

                if(line.priArgType == newasm::datatypes::symbol_name)
                {
                    mmap.addr = newasm::hardware::randAccessMem.write<float>(std::stof(value));
                }
                if(line.priArgType == newasm::datatypes::decimal)
                {
                    mmap.addr = newasm::hardware::randAccessMem.write<float>(line.priFloat);
                }
                return 1;
            }
            // text
            case newasm::core::lang_inf::typenames::txt:
            {
                if(newasm::header::data::struct_now)
                {
                    if(!newasm::header::functions::istext(value))
                    {
                        //newasm::progwin::api::cout("IT HAS TO BE A NUMBER (ZajebucnuoSiSeException) -> " + value);
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    newasm::mem::structs[newasm::header::data::struct_decl].push_back({newasm::datatypes::text, name, value});
                    return 1;
                }
                if(line.priArgType == newasm::datatypes::symbol_name) if(!newasm::header::functions::istext(value))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                if(line.priArgType != newasm::datatypes::symbol_name) if(line.priArgType != newasm::datatypes::text)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                //value = newasm::header::functions::parseBackslash(value);
                //newasm::header::functions::remq(value);

                newasm::variables::ids[name].type = newasm::datatypes::text;
                auto& mmap = newasm::variables::ids.at(name);
                mmap.locked = newasm::expcfg::lockbool;
                mmap.transient__ = newasm::expcfg::transientbool;
                mmap.attrib = newasm::runtime::currentAttributes;
                newasm::runtime::currentAttributes = 0;

                if(newasm::expcfg::transientbool)
                {
                    newasm::garbage::FLAG = 1;
                    newasm::garbage::addr__.push_back(name);
                }

                if(line.priArgType == newasm::datatypes::symbol_name)
                {
                    mmap.addr = newasm::hardware::randAccessMem.write<std::string>(newasm::header::functions::remq(value));
                }
                if(line.priArgType == newasm::datatypes::text)
                {
                    mmap.addr = newasm::hardware::randAccessMem.write<std::string>(line.priString);
                }

                /*
                std::cout << "_____________BACKTRACE_____________\n";
                std::cout << "Writing a string......";
                std::cout << "Variable name: " << name << std::endl;
                std::cout << "Addess: " << newasm::variables::ids.at(name).addr << std::endl;
                std::cout << "Value: " << newasm::hardware::randAccessMem.peek<std::string>(newasm::variables::ids.at(name).addr) << std::endl;
                std::cout << "___________________________________\n";
                */
                return 1;
            }
            // references
            case newasm::core::lang_inf::typenames::ref:
            {
                if(!newasm::header::functions::isref(value) && value != NIL_STR)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                if(!newasm::mem::functions::datavalid(
                newasm::header::functions::remamp(value), newasm::mem::data)
                && !newasm::mem::functions::datavalid(
                newasm::header::functions::remamp(value), newasm::variables::ids)
                && value != NIL_STR)
                {
                    newasm::terminate(newasm::exit_codes::invalid_memacc);
                    return 1;
                }
                if(newasm::header::data::struct_now)
                {
                    newasm::mem::structs[newasm::header::data::struct_decl].push_back({newasm::datatypes::reference, name, value});
                    return 1;
                }

                newasm::mem::datatypes[name] = newasm::datatypes::reference;
                newasm::mem::data[name] = value;
                newasm::mem::data_attrib[name].locked = newasm::expcfg::lockbool;
                return 1;
            }
            // characters
            case newasm::core::lang_inf::typenames::char__:
            {
                if(newasm::header::data::struct_now)
                {
                    if(!newasm::header::functions::ischar(value))
                    {
                        //newasm::progwin::api::cout("IT HAS TO BE A NUMBER (ZajebucnuoSiSeException) -> " + value);
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    newasm::mem::structs[newasm::header::data::struct_decl].push_back({newasm::datatypes::character, name, value});
                    return 1;
                }
                if(line.priArgType == newasm::datatypes::symbol_name) if(!newasm::header::functions::ischar(value))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                if(line.priArgType != newasm::datatypes::symbol_name) if(line.priArgType != newasm::datatypes::character)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                //value = newasm::header::functions::parseBackslash(value);
                //newasm::header::functions::remsq(value);
                
                
                newasm::variables::ids[name].type = newasm::datatypes::character;
                auto& mmap = newasm::variables::ids.at(name);
                mmap.locked = newasm::expcfg::lockbool;
                mmap.transient__ = newasm::expcfg::transientbool;
                mmap.attrib = newasm::runtime::currentAttributes;
                newasm::runtime::currentAttributes = 0;

                if(newasm::expcfg::transientbool)
                {
                    newasm::garbage::FLAG = 1;
                    newasm::garbage::addr__.push_back(name);
                }

                if(line.priArgType == newasm::datatypes::symbol_name)
                {
                    mmap.addr = newasm::hardware::randAccessMem.write<char>(newasm::header::functions::remsq(value).at(0));
                }
                if(line.priArgType == newasm::datatypes::character)
                {
                    mmap.addr = newasm::hardware::randAccessMem.write<char>(line.priChar);
                }
                return 1;
            }
            // tuples
            case newasm::core::lang_inf::typenames::tuple:
            {
                if(!newasm::header::functions::istuple(value))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }

                auto contents = newasm::header::functions::parseTuple(value);

                newasm::variables::ids[name].type = newasm::datatypes::tuple;
                auto& mmap = newasm::variables::ids.at(name);

                mmap.tuple = new newasm::variables::tupleData;
                mmap.attrib = newasm::runtime::currentAttributes;
                newasm::runtime::currentAttributes = 0;
                for(int i = 0; i < contents.size(); ++i)
                {
                    int address;
                    std::string value_buf = contents.at(i);
                    newasm::runtime::functions::parse(value_buf);
                    //integerz
                    if(newasm::header::functions::isnumeric(value_buf))
                    {
                        address = newasm::hardware::randAccessMem.write<int>(std::stoi(value_buf));
                        mmap.tuple->addr.push_back(address);
                        mmap.tuple->type.push_back(newasm::datatypes::number);
                        continue;
                    }
                    //floatz
                    if(newasm::header::functions::isfloat(value_buf))
                    {
                        address = newasm::hardware::randAccessMem.write<float>(std::stof(value_buf));
                        mmap.tuple->addr.push_back(address);
                        mmap.tuple->type.push_back(newasm::datatypes::decimal);
                        continue;
                    }
                    //charz
                    if(newasm::header::functions::ischar(value_buf))
                    {
                        value_buf = newasm::header::functions::parseBackslash<true>(value_buf); //addin true means we're forcing backslash evaluation
                        address = newasm::hardware::randAccessMem.write<char>(newasm::header::functions::remsq(value_buf).at(0));
                        mmap.tuple->addr.push_back(address);
                        mmap.tuple->type.push_back(newasm::datatypes::character);
                        continue;
                    }
                    //stringz
                    if(newasm::header::functions::istext(value_buf))
                    {
                        value_buf = newasm::header::functions::parseBackslash<true>(value_buf);
                        address = newasm::hardware::randAccessMem.write<std::string>(newasm::header::functions::remq(value_buf));
                        mmap.tuple->addr.push_back(address);
                        mmap.tuple->type.push_back(newasm::datatypes::text);
                        continue;
                    }
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }
                //newasm::mem::tuple[name].contents = newasm::header::functions::parseTuple(value);
                return 1;
            }
            // contexts
            case newasm::core::lang_inf::typenames::context__:
            {
                if(!newasm::header::functions::isTupleOrContext(value))
                {
                    newasm::terminate(newasm::exit_codes::invalid_init);
                    return 1;
                }

                auto contents = newasm::header::functions::parseContext(value);

                newasm::variables::ids[name].type = newasm::datatypes::mycontext;
                auto& mmap = newasm::variables::ids.at(name);
                mmap.context = new newasm::variables::contextData;
                mmap.attrib = newasm::runtime::currentAttributes;
                newasm::runtime::currentAttributes = 0;

                //newasm::header::functions::info("Created context: " + name);

                if(contents.size() == 0)
                {
                    return 1;
                }
                //std::string key, value;
                std::vector<std::string> v;
                int addr_temp = 0;
                for(int i = 0; i < contents.size(); ++i)
                {
                    if(!contents.at(i).find(':'))
                    {
                        newasm::terminate(newasm::exit_codes::invalid_syntax);
                        return 1;
                    }
                    v.clear();
                    v = newasm::header::functions::split(contents.at(i), ':');
                    if(v.size() != 2)
                    {
                        newasm::terminate(newasm::exit_codes::invalid_syntax);
                        return 1;
                    }
                    std::string& key = v.at(0), & value__ = v.at(1);
                    key = newasm::header::functions::trim(key);
                    value__ = newasm::header::functions::trim(value__);
                    newasm::runtime::functions::parse(value__);

                    //newasm::header::functions::info("Adding stuff to `" + name + "` = " + key + ":" + value__ + "---" + value);

                    if(!newasm::header::functions::istext(key))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    
                    key = newasm::header::functions::remq(key);
                    if(newasm::header::functions::isnumeric(value__))
                    {
                        addr_temp = newasm::hardware::randAccessMem.write<int>(std::stoi(value__));
                        mmap.context->addr.push_back(addr_temp);
                        mmap.context->keys.push_back(key);
                        mmap.context->type.push_back(newasm::datatypes::number);
                        continue;
                    }
                    if(newasm::header::functions::isfloat(value__))
                    {
                        addr_temp = newasm::hardware::randAccessMem.write<float>(std::stof(value__));
                        mmap.context->addr.push_back(addr_temp);
                        mmap.context->keys.push_back(key);
                        mmap.context->type.push_back(newasm::datatypes::decimal);
                        continue;
                    }
                    if(newasm::header::functions::ischar(value__))
                    {
                        value__ = newasm::header::functions::parseBackslash<true>(value__);
                        addr_temp = newasm::hardware::randAccessMem.write<char>(newasm::header::functions::remsq(value__).at(0));
                        mmap.context->addr.push_back(addr_temp);
                        mmap.context->keys.push_back(key);
                        mmap.context->type.push_back(newasm::datatypes::character);
                        continue;
                    }
                    if(newasm::header::functions::istext(value__))
                    {
                        value__ = newasm::header::functions::parseBackslash<true>(value__);
                        addr_temp = newasm::hardware::randAccessMem.write<std::string>(newasm::header::functions::remq(value__));
                        mmap.context->addr.push_back(addr_temp);
                        mmap.context->keys.push_back(key);
                        mmap.context->type.push_back(newasm::datatypes::text);
                        continue;
                    }
                    newasm::terminate(newasm::exit_codes::invalid_init);
                    return 1;
                }
                #if 0
                for(int i = 0; i < contents.size(); ++i)
                {
                    int address;
                    std::string value_buf = contents.at(i);
                    newasm::runtime::functions::parse(value_buf);
                    //integerz
                    if(newasm::header::functions::isnumeric(value_buf))
                    {
                        address = newasm::hardware::randAccessMem.write<int>(std::stoi(value_buf));
                        newasm::variables::ids.at(name).tuple->addr.push_back(address);
                        newasm::variables::ids.at(name).tuple->type.push_back(newasm::datatypes::number);
                        continue;
                    }
                    //floatz
                    if(newasm::header::functions::isfloat(value_buf))
                    {
                        address = newasm::hardware::randAccessMem.write<float>(std::stof(value_buf));
                        newasm::variables::ids.at(name).tuple->addr.push_back(address);
                        newasm::variables::ids.at(name).tuple->type.push_back(newasm::datatypes::decimal);
                        continue;
                    }
                    //charz
                    if(newasm::header::functions::ischar(value_buf))
                    {
                        address = newasm::hardware::randAccessMem.write<char>(newasm::header::functions::remsq(value_buf).at(0));
                        newasm::variables::ids.at(name).tuple->addr.push_back(address);
                        newasm::variables::ids.at(name).tuple->type.push_back(newasm::datatypes::character);
                        continue;
                    }
                    //stringz
                    if(newasm::header::functions::istext(value_buf))
                    {
                        address = newasm::hardware::randAccessMem.write<std::string>(newasm::header::functions::remq(value_buf));
                        newasm::variables::ids.at(name).tuple->addr.push_back(address);
                        newasm::variables::ids.at(name).tuple->type.push_back(newasm::datatypes::text);
                        continue;
                    }
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }
                #endif
                return 1;
            }
            // class
            case newasm::core::lang_inf::typenames::class__:
            {
                if(!newasm::brace_stack__.empty())
                {
                    newasm::terminate(newasm::exit_codes::nested_object);
                    return 1;
                }
                
                if(value != OPEN_BRACE_STR)
                {
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }

                newasm::variables::ids[name].type = newasm::datatypes::blueprint;
                auto& mmap = newasm::variables::ids.at(name);
                mmap.blueprint = new newasm::variables::classData;

                newasm::header::data::blueprint_now = true;
                newasm::header::data::blueprint_decl = name;

                mmap.attrib = newasm::runtime::currentAttributes;
                newasm::runtime::currentAttributes = 0;

                newasm::brace_stack__.push_back(newasm::brace_stack::class_block);
                
                NewASM::CurrentClass = &mmap;
                return 1;
            }
            case newasm::core::lang_inf::typenames::union__:
            {
                if(!newasm::brace_stack__.empty())
                {
                    newasm::terminate(newasm::exit_codes::nested_object);
                    return 1;
                }

                newasm::variables::ids[name].type = newasm::datatypes::yunion;
                auto& mmap = newasm::variables::ids.at(name);
                mmap.yunion = new newasm::variables::unionData;
                mmap.attrib = newasm::runtime::currentAttributes;
                newasm::runtime::currentAttributes = 0;

                if(value == NIL_STR)
                {
                    mmap.yunion->addr = newasm::hardware::randAccessMem.write<int>(0);
                    return 1;
                }

                if(newasm::header::functions::isnumeric(value))
                {
                    mmap.yunion->addr = newasm::hardware::randAccessMem.write<int>(std::stoi(value));
                    return 1;
                }
                if(newasm::header::functions::isfloat(value))
                {
                    mmap.yunion->addr = newasm::hardware::randAccessMem.write<float>(std::stoi(value));
                    return 1;
                }
                if(newasm::header::functions::ischar(value))
                {
                    value = newasm::header::functions::parseBackslash<true>(value);
                    char val = newasm::header::functions::remsq(value).at(0);
                    mmap.yunion->addr = newasm::hardware::randAccessMem.write<char>(val);
                    return 1;
                }
                if(newasm::header::functions::istext(value))
                {
                    value = newasm::header::functions::parseBackslash<true>(value);
                    std::string val = newasm::header::functions::remq(value);
                    mmap.yunion->addr = newasm::hardware::randAccessMem.write<std::string>(val);
                    return 1;
                }
                if(newasm::header::functions::isref(value))
                {
                    mmap.yunion->addr = newasm::hardware::randAccessMem.write<int>(0);
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::invalid_syntax);
                return 1;
            }
            // containers
            case newasm::core::lang_inf::typenames::cont:
            {
                auto parser = newasm::header::functions::parseContainerType(value);
                if(parser.first)
                {
                    value = parser.second;
                }
                auto it2 = newasm::inverted_types.find(value);
                switch(it2->second)
                {
                    // bit arrays
                    case newasm::core::lang_inf::typenames::bit_arr:
                    {
                        if(newasm::mem::functions::datavalid(name, newasm::containers::bit_arrays))
                        {
                            newasm::terminate(newasm::exit_codes::datastruct_redef);
                            return 1;
                        }
                
                        newasm::containers::bit_arrays[name] = new newasm::containers::bit_array<newasm::containers::default_size>();
                        newasm::mem::data_attrib[name].locked = newasm::expcfg::lockbool;
                        return 1;
                    }
                    // binary trees
                    case newasm::core::lang_inf::typenames::bin_tree:
                    {
                        if(newasm::mem::functions::datavalid(name, newasm::containers::binary_trees))
                        {
                            newasm::terminate(newasm::exit_codes::datastruct_redef);
                            return 1;
                        }

                        newasm::containers::binary_trees[name] = new newasm::containers::binary_tree<newasm::containers::default_size>();
                        newasm::containers::binary_trees.at(name)->set_at__(0,0);
                        newasm::mem::data_attrib[name].locked = newasm::expcfg::lockbool;
                        return 1;
                    }
                    // thread channels
                    case newasm::core::lang_inf::typenames::chan:
                    {
                        if(newasm::mem::functions::datavalid(name, newasm::containers::thread_channels))
                        {
                            newasm::terminate(newasm::exit_codes::datastruct_redef);
                            return 1;
                        }

                        newasm::containers::thread_channels[name] = new newasm::containers::thread_channel__();
                        newasm::containers::thread_channels.at(name)->empty = true;
                        newasm::containers::thread_channels.at(name)->data = "??";
                        return 1;
                    }
                }
                return 1;
            }
            default:
            {
                newasm::terminate(newasm::exit_codes::invalid_syntax);
                return 1;
            }
        }


        return 1;
    }
    int stor_structmem(const std::string &suf, const std::string &into)
    {
        std::string member_name = newasm::header::functions::trim(newasm::header::functions::parseObject(into).second.second);
        std::string struct_name = newasm::header::functions::trim(newasm::header::functions::parseObject(into).second.first);

        if(!newasm::mem::functions::datavalid(struct_name, newasm::mem::structs))
        {
            //std::cout << "struct_name: " << struct_name << std::endl;
            newasm::terminate(newasm::exit_codes::undefined_object);
            return 1;
        }
        bool member_found = false;
        std::vector<newasm::mem::struct_member>::iterator struct_member_id;
        for(struct_member_id = newasm::mem::structs[struct_name].begin(); struct_member_id !=newasm::mem::structs[struct_name].end(); ++struct_member_id)
        {
            if(struct_member_id->name == member_name)
            {
                member_found = true;
                break;
            }
        }
        if(!member_found)
        {
            newasm::terminate(newasm::exit_codes::undefined_objectmem);
            return 1;
        }

        //std::cout << struct_member_id->datatype << " " << struct_member_id->name << " " << struct_member_id->value << std::endl;

        int intreg = newasm::header::constants::inv_ireg_val;
        int floatreg = newasm::header::constants::inv_freg_val;
        std::string strreg = newasm::header::constants::inv_reg_val;

        if(suf == newasm::mem::regs::fdx.identifier())
        {
            intreg = newasm::mem::regs::fdx;
        }
        if(suf == newasm::mem::regs::bos.identifier())
        {
            intreg = newasm::mem::regs::bos;
        }
        if(suf == newasm::mem::regs::stk.identifier())
        {
            intreg = newasm::mem::regs::stk;
        }
        if(suf == newasm::mem::regs::hea.identifier())
        {
            intreg = newasm::mem::regs::hea;
        }
        if(suf == newasm::mem::regs::cpr.identifier())
        {
            intreg = newasm::mem::regs::cpr;
        }
        if(suf == newasm::mem::regs::br0.identifier())
        {
            intreg = newasm::mem::regs::br0;
        }
        if(suf == newasm::mem::regs::br1.identifier())
        {
            intreg = newasm::mem::regs::br1;
        }

        if(suf == newasm::mem::regs::cr0.identifier())
        {
            floatreg = newasm::mem::regs::cr0;
        }
        if(suf == newasm::mem::regs::cr1.identifier())
        {
            floatreg = newasm::mem::regs::cr1;
        }

        if(suf == newasm::mem::regs::tlr.identifier())
        {
            strreg = newasm::mem::regs::tlr.get_value();
        }
        if(suf == newasm::mem::regs::dlx.identifier())
        {
            strreg = newasm::mem::regs::dlx.get_value();
        }
        if(suf == newasm::mem::regs::stl.identifier())
        {
            strreg = newasm::mem::regs::stl;
        }
        if(suf == newasm::mem::regs::psx.identifier())
        {
            strreg = newasm::mem::regs::psx;
        }
        /*
        if(suf == newasm::mem::regs::prp.identifier())
        {
            strreg = newasm::mem::regs::prp;
        }
        if(suf == newasm::mem::regs::cpt.identifier())
        {
            strreg = newasm::mem::regs::cpt;
        }*/

        if(suf == newasm::mem::regs::tr0.identifier())
        {
            strreg = newasm::mem::regs::tr0;
        }
        if(suf == newasm::mem::regs::tr1.identifier())
        {
            strreg = newasm::mem::regs::tr1;
        }

        const bool debugit = false;

        if(intreg != newasm::header::constants::inv_ireg_val)
        {
            if(struct_member_id->datatype != newasm::datatypes::number)
            {
                newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                return 1;
            }
            struct_member_id->value = newasm::_std::to_string(intreg);
            if(debugit) std::cout << struct_member_id->datatype << " " << struct_member_id->name << " " << struct_member_id->value << std::endl;
            return 1;
        }
        if(floatreg != newasm::header::constants::inv_freg_val)
        {
            if(struct_member_id->datatype != newasm::datatypes::decimal)
            {
                newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                return 1;
            }
            struct_member_id->value = newasm::_std::to_string(floatreg);
            if(debugit) std::cout << struct_member_id->datatype << " " << struct_member_id->name << " " << struct_member_id->value << std::endl;
            return 1;
        }
        if(strreg != newasm::header::constants::inv_reg_val)
        {
            if(newasm::header::functions::isnumeric(strreg))
            {
                if(struct_member_id->datatype != newasm::datatypes::number)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }

                struct_member_id->value = strreg;
                if(debugit) std::cout << struct_member_id->datatype << " " << struct_member_id->name << " " << struct_member_id->value << std::endl;
                return 1;
            }
            if(newasm::header::functions::isfloat(strreg))
            {
                if(struct_member_id->datatype != newasm::datatypes::decimal)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }

                struct_member_id->value = strreg;
                if(debugit) std::cout << struct_member_id->datatype << " " << struct_member_id->name << " " << struct_member_id->value << std::endl;
                return 1;
            }
            if(newasm::header::functions::istext(strreg))
            {
                if(struct_member_id->datatype != newasm::datatypes::text)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }

                //strreg = newasm::header::functions::remq(strreg);
                struct_member_id->value = strreg;
                if(debugit) std::cout << struct_member_id->datatype << " " << struct_member_id->name << " " << struct_member_id->value << std::endl;
                return 1;
            }
            if(newasm::header::functions::isref(strreg))
            {
                if(struct_member_id->datatype != newasm::datatypes::reference)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }

                //strreg = newasm::header::functions::remamp(strreg);
                struct_member_id->value = strreg;
                if(debugit) std::cout << struct_member_id->datatype << " " << struct_member_id->name << " " << struct_member_id->value << std::endl;
                return 1;
            }
            newasm::terminate(newasm::exit_codes::invalid_syntax);
            return 1;
        }

        return 1;
    }
    FORCE_INLINE inline int process_iso(std::string& wholeline, std::string& ins, std::string suf, std::string opr, newasm::compiler::lineData& lineInfo)
    {
        if(lineInfo.AltArgLambda) [[unlikely]]
        {
            if(newasm::header::data::proc_now)
            {
                newasm::terminate(newasm::exit_codes::jit_fail);
                return 1;
            }
            newasm::LambdaDispatch::LambdaHalt = false;
            newasm::LambdaDispatch::LambdaLine = false;
            newasm::LambdaDispatch::LambdaNow = true;
            newasm::LambdaDispatch::ThreadSafePtr->contents.clear();
            newasm::LambdaDispatch::ThreadSafePtr->labels.clear();
            newasm::LambdaDispatch::ThreadSafePtr->sysResetLambda();
            newasm::LambdaDispatch::ThreadSafePtr->CallCStack.clear();

            newasm::LambdaDispatch::JitLine = newasm::header::functions::form_iso(ins,suf,"");
            newasm::LambdaDispatch::ThreadSafePtr->mangled = false;
            newasm::LambdaDispatch::ThreadSafePtr->idx = 0;
            return 1;
        }

        //we got this stop flag under the instruction checking
        //so we can easily check what instructions are being added to a function,etc
        if(newasm::system::stop == 1)
        {
            #if 0
            newasm::system::proclines ++;
            std::string newline = ins + static_cast<std::string>(" ") + suf + static_cast<std::string>(",") + opr;
            // OLD -> newasm::mem::funcs[newasm::system::cproc].push_back(newline);
            //std::cout << newasm::system::cproc << " : " << newline << std::endl;
            newasm::mem::funcs[newasm::system::cproc].push_back(lineInfo.raw);
            #endif
            newasm::system::proclines ++;
            //newasm::variables::ids.at(newasm::system::cproc).
            NewASM::CurrentProc->proc->contents.push_back(lineInfo);
            //newasm::mem::funcs[newasm::system::cproc].push_back(lineInfo);
            return 1;
        }

        //parse the operand before execution
        if(lineInfo.altArgType == newasm::datatypes::symbol_name)
        {
            //newasm::runtime::functions::parse(opr); //avoid all shi parsin if the
                                                    //compiler already figured it out
            newasm::runtime::functions::eval(opr, lineInfo.altEvalMode);
        }
        switch(lineInfo.whatAmIDoing)//switch(it->second)
        {
            case INVALID_INS:
            {
                newasm::terminate(newasm::exit_codes::invalid_ins);
                return 1;
            }
            //send
            case newasm::core::lang_inf::send:
            {
                if(!newasm::header::functions::isref(suf))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }

                newasm::runtime::functions::parse(suf);
                suf = newasm::header::functions::remamp(suf);

                if(newasm::containers::thread_channels.find(suf) == newasm::containers::thread_channels.end())
                {
                    newasm::terminate(newasm::exit_codes::invalid_memacc);
                    return 1;
                }

                newasm::containers::thread_channels.at(suf)->empty = false;
                newasm::containers::thread_channels.at(suf)->data = opr;

                //std::cout << "Sent `" << newasm::containers::thread_channels.at(suf)->data << "` to channel " << suf << std::endl;
                return 1;
            }
            //__say
            case newasm::core::lang_inf::__say:
            {
                if(suf == NULL_STR)
                {
                    if(!newasm::header::functions::istext(opr))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    
                    newasm::Console::out(newasm::tab + newasm::header::functions::remq(opr));
                    return 1;
                }
            }
            //merge
            case newasm::core::lang_inf::merge:
            {
                VarPtr ptr = nullptr;
                if(lineInfo.priArgType == newasm::datatypes::ThisPtr)
                {
                    if(newasm::_this == nullptr)
                    {
                        newasm::SetExceptionComment("`this` is probably not initialized");
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    ptr = newasm::_this;
                }
                else if(lineInfo.priArgType != newasm::datatypes::ThisPtr)
                {
                    newasm::runtime::functions::parse(suf);
                    if(!newasm::header::functions::isref(suf))
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    
                    suf = newasm::header::functions::remamp(suf);
                    auto it = newasm::variables::ids.find(suf);
                    if(it == newasm::variables::ids.end())
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    ptr = &it->second;
                }

                if(ptr->type != newasm::datatypes::mycontext)
                {
                    newasm::SetExceptionComment("object is not a context");
                    newasm::terminate(newasm::exit_codes::seg_fault);
                    return 1;
                }

                auto contents = newasm::header::functions::parseContext(opr);

                if(contents.size() == 0)
                {
                    return 1;
                }

                std::vector<std::string> v;
                int addr;
                for(int i = 0; i < contents.size(); ++i)
                {
                    v.clear();
                    // This function guarantees v.size() to be 2!!!!
                    v = newasm::header::functions::split_fixed(contents.at(i), ':');
                    
                    std::string& key = v.at(0), & value = v.at(1);
                    key = newasm::header::functions::trim(key);
                    value = newasm::header::functions::trim(value);

                    if(!newasm::header::functions::istext(key))
                    {
                        newasm::SetExceptionComment("key has to contain textual data");
                        newasm::terminate(newasm::exit_codes::invalid_init);
                        return 1;
                    }
                    key = newasm::header::functions::remq(key);

                    auto& KEYS____ = ptr->context->keys;
                    auto key_ = std::find(KEYS____.begin(), KEYS____.end(), key);
                    if(key_ == ptr->context->keys.end()) //key doesn't exist, we add it
                    {
                        if(value == NIL_STR)
                        {
                            newasm::terminate(newasm::exit_codes::invalid_memacc);
                            return 1;
                        }
                        if(newasm::header::functions::isnumeric(value))
                        {
                            addr = newasm::hardware::randAccessMem.write<int>(std::stoi(value));
                            ptr->context->keys.push_back(key);
                            ptr->context->type.push_back(newasm::datatypes::number);
                            ptr->context->addr.push_back(addr);
                            continue;
                        }
                        if(newasm::header::functions::isfloat(value))
                        {
                            addr = newasm::hardware::randAccessMem.write<float>(std::stof(value));
                            ptr->context->keys.push_back(key);
                            ptr->context->type.push_back(newasm::datatypes::decimal);
                            ptr->context->addr.push_back(addr);
                            continue;
                        }
                        if(newasm::header::functions::ischar(value))
                        {
                            addr = newasm::hardware::randAccessMem.write<char>(newasm::header::functions::remsq(value).at(0));
                            ptr->context->keys.push_back(key);
                            ptr->context->type.push_back(newasm::datatypes::character);
                            ptr->context->addr.push_back(addr);
                            continue;
                        }
                        if(newasm::header::functions::istext(value))
                        {
                            addr = newasm::hardware::randAccessMem.write<std::string>(newasm::header::functions::remq(value));
                            ptr->context->keys.push_back(key);
                            ptr->context->type.push_back(newasm::datatypes::text);
                            ptr->context->addr.push_back(addr);
                            continue;
                        }
                    }
                    if(key_ != ptr->context->keys.end()) //key exists. we can modify it or delete it
                    {
                        int idx = newasm::header::functions::getIndex(ptr->context->keys, key);

                        addr = ptr->context->addr[idx];
                        //we delete everything from RAM anyway
                        // might be inefficient, so i'll look into optimizing a lil
                        if(ptr->context->type[idx] == newasm::datatypes::number)
                        {
                            newasm::hardware::randAccessMem.delete__HEAP(addr, addr + sizeof(int));
                        }
                        if(ptr->context->type[idx] == newasm::datatypes::decimal)
                        {
                            newasm::hardware::randAccessMem.delete__HEAP(addr, addr + sizeof(float));
                        }
                        if(ptr->context->type[idx] == newasm::datatypes::character)
                        {
                            newasm::hardware::randAccessMem.delete__HEAP(addr, addr + sizeof(char));
                        }
                        if(ptr->context->type[idx] == newasm::datatypes::text)
                        {
                            int buffer_len, bytes;
                            std::memcpy(&buffer_len, &newasm::hardware::randAccessMem.__memory__[addr], sizeof(int));
                            bytes = sizeof(int) + buffer_len;
                            newasm::hardware::randAccessMem.delete__HEAP(addr, addr + bytes);
                        }
                        //if the user wants to get rid of the key,
                        // we erase it completely from contextData
                        if(value == NIL_STR)
                        {
                            ptr->context->addr.erase(ptr->context->addr.begin() + idx);
                            ptr->context->type.erase(ptr->context->type.begin() + idx);
                            ptr->context->keys.erase(ptr->context->keys.begin() + idx);
                            continue;
                        }
                        
                        //now, if the user wants to keep the key,
                        //we just give the keys new values
                        if(newasm::header::functions::isnumeric(value))
                        {
                            addr = newasm::hardware::randAccessMem.write<int>(std::stoi(value));
                            ptr->context->addr[idx] = addr;
                            ptr->context->type[idx] = newasm::datatypes::number;
                            //it->second.context->keys[idx] --------> we DON'T touch this, we want the key to stay the same
                            continue;
                        }
                        if(newasm::header::functions::isfloat(value))
                        {
                            addr = newasm::hardware::randAccessMem.write<float>(std::stof(value));
                            ptr->context->addr[idx] = addr;
                            ptr->context->type[idx] = newasm::datatypes::decimal;
                            continue;
                        }
                        if(newasm::header::functions::ischar(value))
                        {
                            addr = newasm::hardware::randAccessMem.write<char>(newasm::header::functions::remsq(value).at(0));
                            ptr->context->addr[idx] = addr;
                            ptr->context->type[idx] = newasm::datatypes::character;
                            continue;
                        }
                        if(newasm::header::functions::istext(value))
                        {
                            addr = newasm::hardware::randAccessMem.write<std::string>(newasm::header::functions::remq(value));
                            ptr->context->addr[idx] = addr;
                            ptr->context->type[idx] = newasm::datatypes::text;
                            continue;
                        }
                        newasm::terminate(newasm::exit_codes::invalid_init);
                        return 1;
                    }
                }
                return 1;
            }
            //lea
            case newasm::core::lang_inf::lea:
            {
                #if 0
                if(lineInfo.altArgType == newasm::datatypes::symbol_name)
                {
                    if(newasm::header::functions::isnumeric(opr))
                    {
                        if(std::stoi(opr) == -1)
                        {
                            if(suf == NIL_STR)
                            {
                                newasm::header::data::tupleIndex = -1;
                                return 1;
                            }
                        }
                    }
                    if(!newasm::header::functions::isref(suf))
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    newasm::runtime::functions::parse(suf);
                    //std::cout << "lea -> suf is :: " << suf << std::endl;
                    
                    suf = newasm::header::functions::remamp(suf);
                    if(!newasm::mem::functions::datavalid(suf, newasm::variables::ids))
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    if(!newasm::header::functions::isnumeric(opr))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    if(newasm::variables::ids.at(suf).tuple == nullptr)
                    {
                        newasm::terminate(newasm::exit_codes::invalid_tuple);
                        return 1;
                    }

                    int localidx = std::stoi(opr);

                    if(localidx >= newasm::variables::ids.at(suf).tuple->addr.size())
                    {
                        newasm::terminate(newasm::exit_codes::seg_fault);
                        return 1;
                    }

                    newasm::header::data::tupleIndex = localidx;
                    return 1;
                }
                if(lineInfo.altArgType == newasm::datatypes::number)
                {
                    if(lineInfo.altInt == -1)
                    {
                        if(suf == NIL_STR)
                        {
                            newasm::header::data::tupleIndex = -1;
                            return 1;
                        }
                    }
                }
                if(!newasm::header::functions::isref(suf))
                {
                    newasm::terminate(newasm::exit_codes::invalid_memacc);
                    return 1;
                }
                newasm::runtime::functions::parse(suf);
                //std::cout << "lea -> suf is :: " << suf << std::endl;
                
                suf = newasm::header::functions::remamp(suf);
                if(!newasm::mem::functions::datavalid(suf, newasm::variables::ids))
                {
                    newasm::terminate(newasm::exit_codes::invalid_memacc);
                    return 1;
                }
                if(lineInfo.altArgType != newasm::datatypes::number)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }
                if(newasm::variables::ids.at(suf).tuple == nullptr)
                {
                    newasm::terminate(newasm::exit_codes::invalid_tuple);
                    return 1;
                }

                int localidx = lineInfo.altInt;

                if(localidx >= newasm::variables::ids.at(suf).tuple->addr.size())
                {
                    newasm::terminate(newasm::exit_codes::seg_fault);
                    return 1;
                }

                newasm::header::data::tupleIndex = localidx;
                #endif

                VarPtr ptr = nullptr;
                if(lineInfo.priArgType == newasm::datatypes::ThisPtr)
                {
                    if(newasm::_this == nullptr)
                    {
                        newasm::SetExceptionComment("`this` is probably not initialized");
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    ptr = newasm::_this;
                }
                else if(lineInfo.priArgType != newasm::datatypes::ThisPtr)
                {
                    if(!newasm::header::functions::isref(suf))
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }

                    newasm::runtime::functions::parse(suf);
                    suf = newasm::header::functions::remamp(suf);

                    auto it = newasm::variables::ids.find(suf);
                    if(it == newasm::variables::ids.end())
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }

                    ptr = &it->second;
                }

                if(ptr->type != newasm::datatypes::tuple)
                {
                    newasm::SetExceptionComment("object is not a tuple");
                    newasm::terminate(newasm::exit_codes::seg_fault);
                    return 1;
                }

                auto TupleSize = ptr->tuple->addr.size();

                int IDX = -1;
                if(lineInfo.altArgType == newasm::datatypes::symbol_name)
                {
                    newasm::runtime::functions::parse(opr);
                    if(newasm::header::functions::isnumeric(opr))
                    {
                        IDX = std::stoi(opr);
                    }
                }
                else if(lineInfo.altArgType == newasm::datatypes::number)
                {
                    IDX = lineInfo.altInt;
                }

                if(IDX < 0 or IDX >= TupleSize)
                {
                    newasm::terminate(newasm::exit_codes::seg_fault);
                    return 1;
                }

                newasm::header::data::tupleIndex = IDX;
                return 1;
            }
            //vmov
            case newasm::core::lang_inf::vmov:
            {
                if(!newasm::header::functions::isvmemref(suf).first)
                {
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }
                
                if(newasm::header::functions::isnumeric(opr))
                {
                    int value = std::stoi(opr);

                    newasm::_virtual::virtualMemory.writeintat(newasm::header::functions::isvmemref(suf).second, value);
                    return 1;
                }
                if(newasm::header::functions::isfloat(opr))
                {
                    float value = std::stof(opr);

                    newasm::_virtual::virtualMemory.writefloatat(newasm::header::functions::isvmemref(suf).second, value);
                    return 1;
                }
                if(newasm::header::functions::istext(opr))
                {
                    std::string value = newasm::header::functions::remq(opr);

                    newasm::_virtual::virtualMemory.writestringat(newasm::header::functions::isvmemref(suf).second, value);
                    return 1;
                }
                if(newasm::header::functions::ischar(opr))
                {
                    std::string value = newasm::header::functions::remsq(opr);

                    newasm::_virtual::virtualMemory.writebyteat(newasm::header::functions::isvmemref(suf).second, value);
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::invalid_syntax);
                return 1;
            }
            
            // STOR
            case newasm::core::lang_inf::stor:
            {
                if(!newasm::header::functions::isref(opr))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }
                opr = newasm::header::functions::trim(newasm::header::functions::remamp(opr));
    
                if(newasm::header::functions::parseObject(opr).first && !newasm::header::functions::istext(opr))
                {
                    newasm::stor_structmem(suf, opr);
                    return 1;
                }

                if(!newasm::mem::functions::datavalid(opr,newasm::mem::data))
                {
                    newasm::terminate(newasm::exit_codes::invalid_memacc);
                    return 1;
                }
                for(auto i = newasm::syscalls::mem::constvals.begin(); i != newasm::syscalls::mem::constvals.end(); ++i)
                {
                    if(*i == newasm::header::functions::trim(opr))
                    {
                        newasm::terminate(newasm::exit_codes::constant_modif);
                        return 1;
                    }
                }

                if(lineInfo.whatAreRegistersLol == INVALID_INS)
                {
                    newasm::terminate(newasm::exit_codes::bus_err);
                    return 1;
                }

                switch(lineInfo.whatAreRegistersLol)
                {
                    case newasm::mem::regs::fdx__:
                    {
                        if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::data[opr] = newasm::_std::to_string(newasm::mem::regs::fdx);
                        return 1;
                    }
                    case newasm::mem::regs::imm__:
                    {
                        if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::data[opr] = newasm::_std::to_string(newasm::mem::regs::imm);
                        return 1;
                    }
                    case newasm::mem::regs::rbx__:
                    {
                        if(newasm::mem::datatypes[opr] != newasm::datatypes::decimal)
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::data[opr] = newasm::_std::to_string(newasm::mem::regs::rbx);
                        return 1;
                    }
                    case newasm::mem::regs::rax__:
                    {
                        if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::data[opr] = newasm::_std::to_string(newasm::mem::regs::rax);
                        return 1;
                    }
                    case newasm::mem::regs::bos__:
                    {
                        if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::data[opr] = newasm::_std::to_string(newasm::mem::regs::bos);
                        return 1;
                    }
                    case newasm::mem::regs::tlr__:
                    {
                        if(newasm::header::functions::isnumeric(newasm::mem::regs::tlr))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::tlr;
                            return 1;
                        }
                        if(newasm::header::functions::isfloat(newasm::mem::regs::tlr))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::decimal)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::tlr;
                            return 1;
                        }
                        if(newasm::header::functions::istext(newasm::mem::regs::tlr))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::text)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::tlr;
                            return 1;
                        }
                        if(newasm::header::functions::isref(newasm::mem::regs::tlr))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::reference)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::tlr;
                            //auto i = std::find(newasm::mem::uninitialized_pointer.begin(), newasm::mem::uninitialized_pointer.end(), opr);
                            //if(i != newasm::mem::uninitialized_pointer.end()) newasm::mem::uninitialized_pointer.erase(i);
                            return 1;
                        }
                        if(newasm::header::functions::ischar(newasm::mem::regs::tlr))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::character)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::tlr;
                            return 1;
                        }
                        return 1;
                    }
                    case newasm::mem::regs::dlx__:
                    {
                        if(newasm::header::functions::isnumeric(newasm::mem::regs::dlx))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::dlx;
                            return 1;
                        }
                        if(newasm::header::functions::isfloat(newasm::mem::regs::dlx))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::decimal)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::dlx;
                            return 1;
                        }
                        if(newasm::header::functions::istext(newasm::mem::regs::dlx))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::text)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::dlx;
                            return 1;
                        }
                        if(newasm::header::functions::isref(newasm::mem::regs::dlx))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::reference)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::dlx;
                            //auto i = std::find(newasm::mem::uninitialized_pointer.begin(), newasm::mem::uninitialized_pointer.end(), opr);
                            //if(i != newasm::mem::uninitialized_pointer.end()) newasm::mem::uninitialized_pointer.erase(i);
                            return 1;
                        }
                        if(newasm::header::functions::ischar(newasm::mem::regs::dlx))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::character)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::dlx;
                            return 1;
                        }
                        return 1;
                    }
                    case newasm::mem::regs::tr0__:
                    {
                        if(newasm::header::functions::isnumeric(newasm::mem::regs::tr0))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::tr0;
                            return 1;
                        }
                        if(newasm::header::functions::isfloat(newasm::mem::regs::tr0))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::decimal)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::tr0;
                            return 1;
                        }
                        if(newasm::header::functions::istext(newasm::mem::regs::tr0))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::text)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::tr0;
                            return 1;
                        }
                        if(newasm::header::functions::isref(newasm::mem::regs::tr0))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::reference)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::tr0;
                            //auto i = std::find(newasm::mem::uninitialized_pointer.begin(), newasm::mem::uninitialized_pointer.end(), opr);
                            //if(i != newasm::mem::uninitialized_pointer.end()) newasm::mem::uninitialized_pointer.erase(i);
                            return 1;
                        }
                        if(newasm::header::functions::ischar(newasm::mem::regs::tr0))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::character)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::tr0;
                            return 1;
                        }
                        return 1;
                    }
                    case newasm::mem::regs::tr1__:
                    {
                        if(newasm::header::functions::isnumeric(newasm::mem::regs::tr1))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::tr1;
                            return 1;
                        }
                        if(newasm::header::functions::isfloat(newasm::mem::regs::tr1))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::decimal)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::tr1;
                            return 1;
                        }
                        if(newasm::header::functions::istext(newasm::mem::regs::tr1))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::text)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::tr1;
                            return 1;
                        }
                        if(newasm::header::functions::isref(newasm::mem::regs::tr1))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::reference)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::tr1;
                            //auto i = std::find(newasm::mem::uninitialized_pointer.begin(), newasm::mem::uninitialized_pointer.end(), opr);
                            //if(i != newasm::mem::uninitialized_pointer.end()) newasm::mem::uninitialized_pointer.erase(i);
                            return 1;
                        }
                        if(newasm::header::functions::ischar(newasm::mem::regs::tr1))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::character)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::tr1;
                            return 1;
                        }
                        return 1;
                    }
                    case newasm::mem::regs::stl__:
                    {
                        if(newasm::header::functions::isnumeric(newasm::mem::regs::stl))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::stl;
                            return 1;
                        }
                        if(newasm::header::functions::isfloat(newasm::mem::regs::stl))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::decimal)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::stl;
                            return 1;
                        }
                        if(newasm::header::functions::istext(newasm::mem::regs::stl))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::text)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::stl;
                            return 1;
                        }
                        if(newasm::header::functions::isref(newasm::mem::regs::stl))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::reference)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::stl;
                            //auto i = std::find(newasm::mem::uninitialized_pointer.begin(), newasm::mem::uninitialized_pointer.end(), opr);
                            //if(i != newasm::mem::uninitialized_pointer.end()) newasm::mem::uninitialized_pointer.erase(i);
                            return 1;
                        }
                        if(newasm::header::functions::ischar(newasm::mem::regs::stl))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::character)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::stl;
                            return 1;
                        }
                        return 1;
                    }
                    case newasm::mem::regs::psx__:
                    {
                        if(newasm::header::functions::isnumeric(newasm::mem::regs::psx))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::psx;
                            return 1;
                        }
                        if(newasm::header::functions::isfloat(newasm::mem::regs::psx))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::decimal)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::psx;
                            return 1;
                        }
                        if(newasm::header::functions::istext(newasm::mem::regs::psx))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::text)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::psx;
                        }
                        if(newasm::header::functions::isref(newasm::mem::regs::psx))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::reference)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::psx;
                            //auto i = std::find(newasm::mem::uninitialized_pointer.begin(), newasm::mem::uninitialized_pointer.end(), opr);
                            //if(i != newasm::mem::uninitialized_pointer.end()) newasm::mem::uninitialized_pointer.erase(i);
                            return 1;
                        }
                        if(newasm::header::functions::ischar(newasm::mem::regs::psx))
                        {
                            if(newasm::mem::datatypes[opr] != newasm::datatypes::character)
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                                return 1;
                            }
                            newasm::mem::data[opr] = newasm::mem::regs::psx;
                        }
                        return 1;
                    }
                    case newasm::mem::regs::stk__:
                    {
                        if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::data[opr] = newasm::_std::to_string(newasm::mem::regs::stk);
                        return 1;
                    }
                    case newasm::mem::regs::hea__:
                    {
                        if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::data[opr] = newasm::_std::to_string(newasm::mem::regs::hea);
                        return 1;
                    }
                    case newasm::mem::regs::cpt__:
                    {
                        if(newasm::mem::datatypes[opr] != newasm::datatypes::reference)
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::data[opr] = newasm::mem::regs::cpt;
                        //auto i = std::find(newasm::mem::uninitialized_pointer.begin(), newasm::mem::uninitialized_pointer.end(), opr);
                        //if(i != newasm::mem::uninitialized_pointer.end()) newasm::mem::uninitialized_pointer.erase(i);
                        return 1;
                    }
                    case newasm::mem::regs::cpr__:
                    {
                        if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::data[opr] = newasm::_std::to_string(newasm::mem::regs::cpr);
                        return 1;
                    }
                    case newasm::mem::regs::cr0__:
                    {
                        if(newasm::mem::datatypes[opr] == newasm::datatypes::number)
                        {
                            newasm::mem::data[opr] = newasm::_std::to_string(newasm::mem::regs::cr0.get_value());
                            return 1;
                        }
                        if(newasm::mem::datatypes[opr] == newasm::datatypes::decimal)
                        {
                            newasm::mem::data[opr] = newasm::_std::to_string(newasm::mem::regs::cr0);
                            return 1;
                        }
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    case newasm::mem::regs::cr1__:
                    {
                        if(newasm::mem::datatypes[opr] == newasm::datatypes::number)
                        {
                            newasm::mem::data[opr] = newasm::_std::to_string(newasm::mem::regs::cr1.get_value());
                            return 1;
                        }
                        if(newasm::mem::datatypes[opr] == newasm::datatypes::decimal)
                        {
                            newasm::mem::data[opr] = newasm::_std::to_string(newasm::mem::regs::cr1);
                            return 1;
                        }
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    case newasm::mem::regs::cr2__:
                    {
                        if(newasm::mem::datatypes[opr] == newasm::datatypes::number)
                        {
                            newasm::mem::data[opr] = newasm::_std::to_string(newasm::mem::regs::cr2);
                            return 1;
                        }
                        if(newasm::mem::datatypes[opr] == newasm::datatypes::decimal)
                        {
                            newasm::mem::data[opr] = newasm::_std::to_string(newasm::mem::regs::cr2.get_value());
                            return 1;
                        }
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    case newasm::mem::regs::cr3__:
                    {
                        if(newasm::mem::datatypes[opr] == newasm::datatypes::number)
                        {
                            newasm::mem::data[opr] = newasm::_std::to_string(newasm::mem::regs::cr3);
                            return 1;
                        }
                        if(newasm::mem::datatypes[opr] == newasm::datatypes::decimal)
                        {
                            newasm::mem::data[opr] = newasm::_std::to_string(newasm::mem::regs::cr3.get_value());
                            return 1;
                        }
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    case newasm::mem::regs::br0__:
                    {
                        if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::data[opr] = newasm::_std::to_string(newasm::mem::regs::br0);
                        return 1;
                    }
                    case newasm::mem::regs::br1__:
                    {
                        if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::data[opr] = newasm::_std::to_string(newasm::mem::regs::br1);
                        return 1;
                    }
                    default:
                    {
                        newasm::terminate(newasm::exit_codes::os_error);
                        return 1;
                    }
                }
                    
                return 1;
            }
            #if 0
            //sysreq
            case newasm::core::lang_inf::sysreq:
            {
                if(!newasm::header::functions::isref(opr))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }
                opr = newasm::header::functions::remamp(opr);
                if(suf == static_cast<std::string>("proc"))
                {
                    if(!newasm::mem::functions::datavalid(opr,newasm::mem::funcs))
                    {
                        //std::cout << opr << " not found <proc>" << std::endl;
                        newasm::terminate(newasm::exit_codes::sysreq_fail);//,wholeline);
                        return 1;
                    }
                    return 1;
                }
                if(suf == static_cast<std::string>("data"))
                {
                    if(!newasm::mem::functions::datavalid(opr,newasm::mem::data) && !newasm::mem::functions::datavalid(opr,newasm::variables::ids))
                    {
                        //std::cout << opr << " not found <data>" << std::endl;
                        newasm::terminate(newasm::exit_codes::sysreq_fail);//,wholeline);
                    }
                    return 1;
                }
            }
            #endif
            //movasx
            case newasm::core::lang_inf::movasx:
            {
                VarPtr ptr = nullptr;
                if(lineInfo.priArgType == newasm::datatypes::ThisPtr)
                {
                    if(newasm::_this == nullptr)
                    {
                        newasm::SetExceptionComment("`this` is probably not initialized");
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    ptr = newasm::_this;
                }
                else if(lineInfo.priArgType != newasm::datatypes::ThisPtr)
                {
                    newasm::runtime::functions::parse(suf);
                    if(!newasm::header::functions::isref(suf))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }

                    suf = newasm::header::functions::remamp(suf);
                    auto it = newasm::variables::ids.find(suf);

                    if(it == newasm::variables::ids.end())
                    {
                        newasm::terminate(newasm::exit_codes::invalid_tuple);
                        return 1;
                    }

                    ptr = &it->second;
                }

                if(newasm::header::data::tupleIndex == -1)
                {
                    newasm::terminate(newasm::exit_codes::invalid_memacc);
                    return 1;
                }

                if(ptr->type != newasm::datatypes::tuple)
                {
                    newasm::SetExceptionComment("object is not a tuple");
                    newasm::terminate(newasm::exit_codes::seg_fault);
                    return 1;
                }

                int value = -1;
                if(lineInfo.altArgType == newasm::datatypes::number)
                {
                    value = lineInfo.altInt;
                }
                else if(lineInfo.altArgType == newasm::datatypes::symbol_name)
                {
                    newasm::runtime::functions::parse(opr);
                    if(newasm::header::functions::isnumeric(opr))
                    {
                        value = std::stoi(opr);
                    }
                }

                if(!newasm::RAM->is_valid_addr(value))
                {
                    newasm::SetExceptionComment(
                        newasm::_std::to_string(value) +
                        " is not a valid accessible address"
                    );
                    newasm::terminate(newasm::exit_codes::seg_fault);
                    return 1;
                }

                ptr->tuple->addr[newasm::header::data::tupleIndex] = value;
                return 1;
            }
            // movaddr
            case newasm::core::lang_inf::movaddr:
            {
                VarPtr ptr = nullptr;
                if(lineInfo.priArgType == newasm::datatypes::ThisPtr)
                {
                    if(newasm::_this == nullptr)
                    {
                        newasm::SetExceptionComment("`this` is probably not initialized");
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    ptr = newasm::_this;
                }
                else if(lineInfo.priArgType != newasm::datatypes::ThisPtr)
                {
                    newasm::runtime::functions::parse(suf);
                    if(!newasm::header::functions::isref(suf))
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    suf = newasm::header::functions::remamp(suf);
                    auto it = newasm::variables::ids.find(suf);

                    if(it == newasm::variables::ids.end())
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }

                    ptr = &it->second;
                }

                if(
                    ptr->type != newasm::datatypes::number and
                    ptr->type != newasm::datatypes::decimal and
                    ptr->type != newasm::datatypes::text and
                    ptr->type != newasm::datatypes::character
                )
                {
                    newasm::SetExceptionComment("not a supported object type: intg, float, string, char");
                    newasm::terminate(newasm::exit_codes::seg_fault);
                    return 1;
                }

                int value = -1;
                if(lineInfo.altArgType == newasm::datatypes::number)
                {
                    value = lineInfo.altInt;
                }
                else if(lineInfo.altArgType == newasm::datatypes::symbol_name)
                {
                    newasm::runtime::functions::parse(opr);
                    if(newasm::header::functions::isnumeric(opr))
                    {
                        value = std::stoi(opr);
                    }
                }

                if(!newasm::RAM->is_valid_addr(value))
                {
                    newasm::SetExceptionComment(
                        newasm::_std::to_string(value) +
                        " is not a valid accessible address"
                    );
                    newasm::terminate(newasm::exit_codes::seg_fault);
                    return 1;
                }

                ptr->addr = value;
                return 1;
            }
            //loop
            case newasm::core::lang_inf::loop:
            {
                switch(lineInfo.whatAreRegistersLol)
                {
                    case newasm::mem::regs::rax__:
                    {
                        --newasm::mem::regs::rax;
                        if(newasm::mem::regs::rax.get_value() <= 0)
                        {
                            return 1;
                        }
                        break;
                    }
                    case newasm::mem::regs::imm__:
                    {
                        #if NEWASM_IMM_LOGS == false
                        --newasm::mem::regs::imm;
                        #elif NEWASM_IMM_LOGS == true
                        newasm::mem::regs::imm.set_value(newasm::mem::regs::imm.get_value() - 1);
                        #endif
                        if(newasm::mem::regs::imm.get_value() <= 0)
                        {
                            return 1;
                        }
                        break;
                    }
                    case newasm::mem::regs::cr2__:
                    {
                        --newasm::mem::regs::cr2;
                        if(newasm::mem::regs::cr2.get_value() <= 0)
                        {
                            return 1;
                        }
                        break;
                    }
                    case newasm::mem::regs::cr3__:
                    {
                        --newasm::mem::regs::cr3;
                        if(newasm::mem::regs::cr3.get_value() <= 0)
                        {
                            return 1;
                        }
                        break;
                    }
                    default:
                    {
                        newasm::terminate(newasm::exit_codes::bus_err);
                        return 1;
                    }
                }
                
                if(newasm::header::data::proc_now)
                {
                    NewASM::CurrentProcA->proc->idx = lineInfo.jumpinTo;//j.proc->labels.at(suf);
                    //std::cout << "Zasto ne skaces pizda ti mater'na? -> " << j.proc->idx << std::endl;
                    return 1;
                }
                else if(newasm::LambdaDispatch::LambdaLine)
                {
                    newasm::LambdaDispatch::ThreadSafePtr->idx = lineInfo.jumpinTo;
                    return 1;
                }

                if(newasm::thread_line)
                {
                    newasm::threads::memory.at(newasm::threads::now)->lcx = lineInfo.jumpinTo;//newasm::threads::memory.at(newasm::threads::now)->labels.at(suf);
                    return 1;
                }

                NEWASM_JMP__(lineInfo.jumpinTo)
                return 1;
            }
            //movx
            case newasm::core::lang_inf::movx:
            {
                //return 1;
                newasm::runtime::functions::parse(opr); // for namespaces
                opr = newasm::header::functions::trim(newasm::header::functions::remamp(opr));
                newasm::runtime::functions::parse(suf); // for namespaces
                suf = newasm::header::functions::trim(newasm::header::functions::remamp(suf));

                auto NewLocation_ = newasm::variables::ids.find(suf);
                auto OldLocation_ = newasm::variables::ids.find(opr);

                if(NewLocation_ == newasm::variables::ids.end() or OldLocation_ == newasm::variables::ids.end())
                {
                    NewASM::terminate(NewASM::exit_codes::invalid_memacc);
                    return 1;
                }

                auto& newloc = NewLocation_->second;
                auto& oldloc_s = OldLocation_;
                auto& oldloc = oldloc_s->second;

                if(newloc.type != oldloc.type)
                {
                    NewASM::terminate(NewASM::exit_codes::dtyp_mismatch);
                    return 1;
                }

                if(
                    oldloc.attrib & newasm::core::lang_inf::attributes::CONST__ or
                    newloc.attrib & newasm::core::lang_inf::attributes::CONST__
                )
                {
                    newasm::terminate(newasm::exit_codes::constant_modif);
                    return 1;
                }

                if(
                    oldloc.attrib & newasm::core::lang_inf::attributes::STATIC__ or
                    newloc.attrib & newasm::core::lang_inf::attributes::STATIC__
                )
                {
                    newasm::terminate(newasm::exit_codes::immovable_type);
                    return 1;
                }

                // i like using `or` on end of lines cuz it looks better and || in the middle
                if(
                    (newloc.type == newasm::datatypes::proc || oldloc.type == newasm::datatypes::proc) or
                    (newloc.type == newasm::datatypes::threadz || oldloc.type == newasm::datatypes::threadz) or
                    (newloc.type == newasm::datatypes::event || oldloc.type == newasm::datatypes::event) or
                    (newloc.type == newasm::datatypes::static_objz or oldloc.type == newasm::datatypes::static_objz) // OBJ CANNOT BE MOVED CUZ IT IS NOT IN MEMORY AT ALL
                )
                {
                    NewASM::terminate(NewASM::exit_codes::immovable_type);
                    return 1;
                }

                //firstly safely delete existing data in newloc
                if(newloc.type == newasm::datatypes::number)
                {
                    newasm::hardware::randAccessMem.delete__HEAP(newloc.addr, newloc.addr + sizeof(int));
                }
                else if(newloc.type == newasm::datatypes::decimal)
                {
                    newasm::hardware::randAccessMem.delete__HEAP(newloc.addr, newloc.addr + sizeof(float));
                }
                else if(newloc.type == newasm::datatypes::character)
                {
                    newasm::hardware::randAccessMem.delete__HEAP(newloc.addr, newloc.addr + sizeof(char));
                }
                else if(newloc.type == newasm::datatypes::text)
                {
                    int buffer_len = newasm::hardware::randAccessMem.peek<int>(newloc.addr);
                    newasm::hardware::randAccessMem.delete__HEAP(newloc.addr, newloc.addr + sizeof(int) + buffer_len);
                }
                else if(newloc.type == newasm::datatypes::tuple) if(newloc.tuple != nullptr)
                {
                    int size = newloc.tuple->addr.size();
                    int addr;
                    // firstly clean the whole tuple
                    for(int idx = 0; idx < size; ++idx)
                    {
                        addr = newloc.tuple->addr[idx];
                        if(newloc.tuple->type[idx] == newasm::datatypes::number)
                        {
                            newasm::hardware::randAccessMem.delete__HEAP(addr, addr + sizeof(int));
                            continue;
                        }
                        if(newloc.tuple->type[idx] == newasm::datatypes::decimal)
                        {
                            newasm::hardware::randAccessMem.delete__HEAP(addr, addr + sizeof(float));
                            continue;
                        }
                        if(newloc.tuple->type[idx] == newasm::datatypes::character)
                        {
                            newasm::hardware::randAccessMem.delete__HEAP(addr, addr + sizeof(char));
                            continue;
                        }
                        if(newloc.tuple->type[idx] == newasm::datatypes::text)
                        {
                            int buffer_size;
                            std::memcpy(&buffer_size, &newasm::hardware::randAccessMem.__memory__[addr], sizeof(int));
                            int bytes = sizeof(int) + buffer_size;
                            newasm::hardware::randAccessMem.delete__HEAP(addr, addr + bytes);
                            continue;
                        }
                    }
                    delete newloc.tuple;
                }
                else if(newloc.type == newasm::datatypes::blueprint) if(newloc.blueprint != nullptr)
                {
                    int size = newloc.blueprint->addr.size();
                    for(int i = 0; i < size; ++i)
                    {
                        int addr = newloc.blueprint->addr.at(i);
                        int buffer_size;
                        std::memcpy(&buffer_size, &newasm::hardware::randAccessMem.__memory__[addr], sizeof(int));
                        int bytes = sizeof(int) + buffer_size;
                        newasm::hardware::randAccessMem.delete__HEAP(addr, addr + bytes);
                        continue;
                    }
                    delete newloc.blueprint;
                }
                else if(newloc.type == newasm::datatypes::yunion) if(newloc.yunion != nullptr)
                {
                    int yunion_addr = newloc.yunion->addr;
                    if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::num)
                    {
                        newasm::hardware::randAccessMem.delete__HEAP(yunion_addr, yunion_addr + sizeof(int));
                    }
                    else if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::decm)
                    {
                        newasm::hardware::randAccessMem.delete__HEAP(yunion_addr, yunion_addr + sizeof(float));
                    }
                    else if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::char__)
                    {
                        newasm::hardware::randAccessMem.delete__HEAP(yunion_addr, yunion_addr + sizeof(char));
                    }
                    else if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::txt)
                    {
                        int buffer_len = newasm::hardware::randAccessMem.peek<int>(yunion_addr);
                        newasm::hardware::randAccessMem.delete__HEAP(yunion_addr, yunion_addr + sizeof(int) + buffer_len);
                    }
                    else
                    {
                        newasm::terminate(newasm::exit_codes::seg_fault);
                    }
                    delete newloc.yunion;
                }
                else if(newloc.type == newasm::datatypes::mycontext) if(newloc.context != nullptr)
                {
                    auto oldcontents_ = newloc.context->keys.size();
                    int addr = 0;
                    for(int idx = 0; idx < oldcontents_; ++idx)
                    {
                        addr = newloc.context->addr[idx];
                        if(newloc.context->type[idx] == newasm::datatypes::number)
                        {
                            newasm::hardware::randAccessMem.delete__HEAP(addr, addr + sizeof(int));
                            continue;
                        }
                        if(newloc.context->type[idx] == newasm::datatypes::decimal)
                        {
                            newasm::hardware::randAccessMem.delete__HEAP(addr, addr + sizeof(float));
                            continue;
                        }
                        if(newloc.context->type[idx] == newasm::datatypes::character)
                        {
                            newasm::hardware::randAccessMem.delete__HEAP(addr, addr + sizeof(char));
                            continue;
                        }
                        if(newloc.context->type[idx] == newasm::datatypes::text)
                        {
                            int buffer_len;
                            std::memcpy(&buffer_len, &newasm::hardware::randAccessMem.__memory__[addr], sizeof(int));
                            int bytes = sizeof(int) + buffer_len;
                            newasm::hardware::randAccessMem.delete__HEAP(addr, addr + bytes);
                            continue;
                        }
                    }
                    delete newloc.context;
                }
                else if(newloc.proc != nullptr)// it will never reach this point
                {
                    delete newloc.proc;
                }
                else if(newloc.thrd != nullptr)
                {
                    delete newloc.thrd;
                }
                else if(newloc.event != nullptr)
                {
                    delete newloc.event;
                }
                else if(newloc.obj != nullptr)
                {
                    delete newloc.obj;
                }
                //then  move
                newloc.addr = oldloc.addr;
                newloc.type = oldloc.type;

                newloc.tuple = oldloc.tuple;
                newloc.blueprint = oldloc.blueprint;
                newloc.yunion = oldloc.yunion;
                newloc.context = oldloc.context;
                newloc.proc = oldloc.proc;
                newloc.thrd = oldloc.thrd;
                newloc.event = oldloc.event;
                newloc.obj = oldloc.obj;

                newloc.locked = oldloc.locked;
                newloc.transient__ = oldloc.transient__;
                newloc.attrib = oldloc.attrib;

                oldloc.deleted = true;
                if((*newasm::_this)->deleted)
                {
                    newasm::_this = nullptr; //get rid of the pointer
                }

                //std::cout << "tryn to delete: `" << opr << "`\n";
                newasm::variables::ids.erase(opr);
                return 1;
            }
            //mov
            case newasm::core::lang_inf::mov:
            {
                /*
                if(newasm::header::functions::isvmemref(opr).first)
                {
                    opr = newasm::_virtual::readData(newasm::header::functions::isvmemref(opr).second);
                }
                */
                if(lineInfo.VirtualMemoryAccess)
                {
                    //std::cout << "UAAAA\n";
                    auto p = newasm::header::functions::isvmemref(opr);
                    if(p.first)
                    {
                        opr = newasm::_virtual::readData(p.second);
                    }
                }

                if(lineInfo.whatAreRegistersLol == INVALID_INS)
                {
                    NewASM::variables::varData* ptr = nullptr;
                    if(lineInfo.priArgType != newasm::datatypes::ThisPtr)
                    {
                        newasm::runtime::functions::parse(suf); // for namespaces
                        suf = newasm::header::functions::trim(newasm::header::functions::remamp(suf));
                        if(!newasm::mem::functions::datavalid(suf, newasm::variables::ids)) [[unlikely]]
                        {
                            //std::cout << "THIS HAPPENED!!!! -> "<< suf << std::endl;
                            newasm::terminate(newasm::exit_codes::invalid_memacc);
                            return 1;
                        }
                        ptr = &newasm::variables::ids.at(suf);
                    }
                    else// if(lineInfo.priArgType == newasm::datatypes::ThisPtr)
                    {
                        if(newasm::_this == nullptr) [[unlikely]]
                        {
                            newasm::SetExceptionComment("`this` is probably not initialized");
                            //std::cout << "THIS ACTUALLY HAPPENED!!!! -> "<< suf << std::endl;
                            newasm::terminate(newasm::exit_codes::invalid_memacc);
                            return 1;
                        }
                        if(
                            (*newasm::_this)->type != newasm::datatypes::number and
                            (*newasm::_this)->type != newasm::datatypes::decimal and
                            (*newasm::_this)->type != newasm::datatypes::character and
                            (*newasm::_this)->type != newasm::datatypes::text and
                            (*newasm::_this)->type != newasm::datatypes::yunion and
                            (*newasm::_this)->type != newasm::datatypes::mycontext and
                            (*newasm::_this)->type != newasm::datatypes::tuple
                        ) [[unlikely]]
                        {
                            newasm::terminate(newasm::exit_codes::seg_fault);
                            return 1;
                        }

                        ptr = newasm::_this;
                    }

                    auto& i = *ptr;
                    if(i.attrib & newasm::core::lang_inf::attributes::CONST__) [[unlikely]]
                    {
                        newasm::terminate(newasm::exit_codes::constant_modif);
                        return 1;
                    }
                    if(i.type == newasm::datatypes::number)
                    {
                        if(lineInfo.altArgType == newasm::datatypes::symbol_name)
                        {
                            if(!newasm::header::functions::isnumeric(opr)) [[unlikely]]
                            {
                                //std::cout << "opr is " << opr << std::endl;
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                                return 1;
                            }

                            newasm::hardware::randAccessMem.overwrite<int>(i.addr, std::stoi(opr));
                            return 1;
                        }
                        if(lineInfo.altArgType != i.type) [[unlikely]]
                        {
                            //std::cout << "opr 2 is " << opr << std::endl;
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                            return 1;
                        }
                        newasm::hardware::randAccessMem.overwrite<int>(i.addr, lineInfo.altInt);
                        return 1;
                    }
                    if(i.type == newasm::datatypes::decimal)
                    {
                        if(lineInfo.altArgType == newasm::datatypes::symbol_name)
                        {
                            //std::cout << "\nEvo ga error -> `" << opr << "`\n";
                            if(!newasm::header::functions::isfloat(opr))
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                                return 1;
                            }

                            newasm::hardware::randAccessMem.overwrite<float>(i.addr, std::stof(opr));
                            return 1;
                        }
                        if(lineInfo.altArgType != i.type)
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                            return 1;
                        }
                        newasm::hardware::randAccessMem.overwrite<float>(i.addr, lineInfo.altFloat);
                        return 1;
                    }
                    if(i.type == newasm::datatypes::character)
                    {
                        if(lineInfo.altArgType == newasm::datatypes::symbol_name)
                        {
                            if(!newasm::header::functions::ischar(opr))
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                                return 1;
                            }

                            newasm::hardware::randAccessMem.overwrite<char>(i.addr, newasm::header::functions::remsq(opr).at(0));
                            return 1;
                        }
                        if(lineInfo.altArgType != i.type)
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                            return 1;
                        }
                        newasm::hardware::randAccessMem.overwrite<char>(i.addr, lineInfo.altChar);
                        return 1;
                    }
                    if(i.type == newasm::datatypes::text)
                    {
                        if(lineInfo.altArgType == newasm::datatypes::symbol_name)
                        {
                            if(!newasm::header::functions::istext(opr))
                            {
                                //std::cout << "ovo se desilo->opr-> " << opr << std::endl;
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                                return 1;
                            }

                            // We have to change the pointer if the string gets reallocated
                            newasm::variables::ids.at(suf).addr = newasm::hardware::randAccessMem.overwrite<std::string>(i.addr, newasm::header::functions::remq(opr));;
                            return 1;
                        }
                        if(lineInfo.altArgType != i.type)
                        {
                            //std::cout << "ovo se desilo 2->opr-> " << opr << std::endl;
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                            return 1;
                        }
                        newasm::variables::ids.at(suf).addr = newasm::hardware::randAccessMem.overwrite<std::string>(i.addr, lineInfo.altString);
                        return 1;
                    }
                    if(i.type == newasm::datatypes::yunion)
                    {
                        if(!newasm::hardware::randAccessMem.is_free(i.yunion->addr))
                        {
                            newasm::terminate(newasm::exit_codes::memory_leak);
                            return 1;
                        }
                        if(
                            (newasm::header::data::movas_type == newasm::core::lang_inf::typenames::num) and
                            (newasm::header::functions::isnumeric(opr))
                        )
                        {
                            i.yunion->addr = newasm::hardware::randAccessMem.write<int>(std::stoi(opr));
                            return 1;
                        }
                        if(
                            (newasm::header::data::movas_type == newasm::core::lang_inf::typenames::decm) and
                            (newasm::header::functions::isfloat(opr))
                        )
                        {
                            i.yunion->addr = newasm::hardware::randAccessMem.write<float>(std::stof(opr));
                            return 1;
                        }
                        if(
                            (newasm::header::data::movas_type == newasm::core::lang_inf::typenames::char__) and
                            (newasm::header::functions::ischar(opr))
                        )
                        {
                            char value = newasm::header::functions::remsq(opr).at(0);
                            i.yunion->addr = newasm::hardware::randAccessMem.write<char>(value);
                            return 1;
                        }
                        if(
                            (newasm::header::data::movas_type == newasm::core::lang_inf::typenames::txt) and
                            (newasm::header::functions::istext(opr))
                        )
                        {
                            std::string value = newasm::header::functions::remq(opr);
                            i.yunion->addr = newasm::hardware::randAccessMem.write<std::string>(value);
                            return 1;
                        }

                        newasm::terminate(newasm::exit_codes::seg_fault);
                        return 1;
                    }
                    //////////CONTEXTS
                    if(i.type == newasm::datatypes::mycontext)
                    {
                        if(!newasm::header::functions::isTupleOrContext(opr))
                        {
                            newasm::terminate(newasm::exit_codes::invalid_init);
                            return 1;
                        }

                        //newasm::header::functions::info("Modifying context: " + suf);

                        auto oldcontents_ = i.context->keys.size();
                        //newasm::header::functions::info("Found keys: " + newasm::_std::to_string(oldcontents_));
                        int addr = 0;
                        for(int idx = 0; idx < oldcontents_; ++idx)
                        {
                            addr = i.context->addr[idx];
                            //newasm::header::functions::info("Deleting key: " + i.context->keys[idx]);
                            if(i.context->type[idx] == newasm::datatypes::number)
                            {
                                newasm::hardware::randAccessMem.delete__HEAP(addr, addr + sizeof(int));
                                continue;
                            }
                            if(i.context->type[idx] == newasm::datatypes::decimal)
                            {
                                newasm::hardware::randAccessMem.delete__HEAP(addr, addr + sizeof(float));
                                continue;
                            }
                            if(i.context->type[idx] == newasm::datatypes::character)
                            {
                                newasm::hardware::randAccessMem.delete__HEAP(addr, addr + sizeof(char));
                                continue;
                            }
                            if(i.context->type[idx] == newasm::datatypes::text)
                            {
                                int buffer_len;
                                std::memcpy(&buffer_len, &newasm::hardware::randAccessMem.__memory__[addr], sizeof(int));
                                int bytes = sizeof(int) + buffer_len;
                                newasm::hardware::randAccessMem.delete__HEAP(addr, addr + bytes);
                                continue;
                            }
                        }

                        auto contents = newasm::header::functions::parseContext(opr);

                        auto size = i.context->keys.size();
                        if(i.context->addr.size() != size)
                        {
                            //newasm::header::functions::err("ADDR.SIZE NIJE JEDNAK SIZE: " + newasm::_std::to_string(i.context->addr.size()));
                            newasm::header::functions::err("You should NOT have seen this message! Code: CTX1-" + newasm::_std::to_string(size) + "-" + newasm::_std::to_string(i.context->addr.size()));
                            newasm::terminate(newasm::exit_codes::invalid_alloc);
                            return 1;
                        }
                        if(i.context->type.size() != size)
                        {
                            //newasm::header::functions::err("TYPE.SIZE NIJE JEDNAK SIZE: " + newasm::_std::to_string(i.context->type.size()));
                            newasm::header::functions::err("You should NOT have seen this message! Code: CTX2-" + newasm::_std::to_string(size) + "-" + newasm::_std::to_string(i.context->type.size()));
                            newasm::terminate(newasm::exit_codes::invalid_alloc);
                            return 1;
                        }

                        i.context->addr.clear();
                        i.context->keys.clear();
                        i.context->type.clear();

                        std::vector<std::string> v;
                        int addr_temp = 0;
                        //newasm::header::functions::info("Adding to existing context: " + suf);
                        for(int idx = 0; idx < contents.size(); ++idx)
                        {
                            if(!contents.at(idx).find(':'))
                            {
                                //newasm::header::functions::err("EVO GA OVDJE NIGGA!");
                                newasm::terminate(newasm::exit_codes::invalid_syntax);
                                return 1;
                            }
                            v.clear();
                            v = newasm::header::functions::split_fixed(contents.at(idx), ':');
                            if(v.size() != 2)
                            {
                                //newasm::header::functions::err("IPAK JE OVDJE!");
                                //newasm::header::functions::err("ERROR BIO OVDJE -> Parsing: `" + contents.at(idx) + "`");
                                
                                newasm::terminate(newasm::exit_codes::invalid_syntax);
                                return 1;
                            }
                            std::string& key = v.at(0), & value__ = v.at(1);

                            key = newasm::header::functions::trim(key);
                            value__ = newasm::header::functions::trim(value__);
                            //newasm::header::functions::info("2: Adding keys to `" + suf + "` = " + key + ":" + value__);
                            newasm::runtime::functions::parse(value__);

                            if(!newasm::header::functions::istext(key))
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                                return 1;
                            }
                            key = newasm::header::functions::remq(key);
                            if(newasm::header::functions::isnumeric(value__))
                            {
                                addr_temp = newasm::hardware::randAccessMem.write<int>(std::stoi(value__));
                                i.context->addr.push_back(addr_temp);
                                i.context->keys.push_back(key);
                                i.context->type.push_back(newasm::datatypes::number);
                                continue;
                            }
                            if(newasm::header::functions::isfloat(value__))
                            {
                                addr_temp = newasm::hardware::randAccessMem.write<float>(std::stof(value__));
                                i.context->addr.push_back(addr_temp);
                                i.context->keys.push_back(key);
                                i.context->type.push_back(newasm::datatypes::decimal);
                                continue;
                            }
                            if(newasm::header::functions::ischar(value__))
                            {
                                addr_temp = newasm::hardware::randAccessMem.write<char>(newasm::header::functions::remsq(value__).at(0));
                                i.context->addr.push_back(addr_temp);
                                i.context->keys.push_back(key);
                                i.context->type.push_back(newasm::datatypes::character);
                                continue;
                            }
                            if(newasm::header::functions::istext(value__))
                            {
                                addr_temp = newasm::hardware::randAccessMem.write<std::string>(newasm::header::functions::remq(value__));
                                i.context->addr.push_back(addr_temp);
                                i.context->keys.push_back(key);
                                i.context->type.push_back(newasm::datatypes::text);
                                continue;
                            }
                            newasm::terminate(newasm::exit_codes::invalid_init);
                            return 1;
                        }
                        return 1;
                    }
                    //////////////tuples
                    if(i.type == newasm::datatypes::tuple)
                    {
                        if(newasm::header::data::tupleIndex != -1)
                        {
                            // INTEGERS
                            if(i.tuple->type[newasm::header::data::tupleIndex] == newasm::datatypes::number)
                            {
                                if(newasm::header::functions::isnumeric(opr))
                                {
                                    newasm::hardware::randAccessMem.overwrite<int>(i.tuple->addr[newasm::header::data::tupleIndex], std::stoi(opr));
                                    newasm::header::data::tupleIndex = -1;
                                    return 1;
                                }
                                if(newasm::header::functions::isfloat(opr))
                                {
                                    newasm::hardware::randAccessMem.delete__HEAP(i.tuple->addr[newasm::header::data::tupleIndex], i.tuple->addr[newasm::header::data::tupleIndex] + sizeof(int));
                                    i.tuple->addr[newasm::header::data::tupleIndex] = newasm::hardware::randAccessMem.write<float>(std::stof(opr));
                                    i.tuple->type[newasm::header::data::tupleIndex] = newasm::datatypes::decimal;
                                    newasm::header::data::tupleIndex = -1;
                                    return 1;
                                }
                                if(newasm::header::functions::ischar(opr))
                                {
                                    newasm::hardware::randAccessMem.delete__HEAP(i.tuple->addr[newasm::header::data::tupleIndex], i.tuple->addr[newasm::header::data::tupleIndex] + sizeof(int));
                                    i.tuple->addr[newasm::header::data::tupleIndex] = newasm::hardware::randAccessMem.write<char>(newasm::header::functions::remsq(opr).at(0));
                                    i.tuple->type[newasm::header::data::tupleIndex] = newasm::datatypes::character;
                                    newasm::header::data::tupleIndex = -1;
                                    return 1;
                                }
                                if(newasm::header::functions::istext(opr))
                                {
                                    newasm::hardware::randAccessMem.delete__HEAP(i.tuple->addr[newasm::header::data::tupleIndex], i.tuple->addr[newasm::header::data::tupleIndex] + sizeof(int));
                                    i.tuple->addr[newasm::header::data::tupleIndex] = newasm::hardware::randAccessMem.write<std::string>(newasm::header::functions::remq(opr));
                                    i.tuple->type[newasm::header::data::tupleIndex] = newasm::datatypes::text;
                                    newasm::header::data::tupleIndex = -1;
                                    return 1;
                                }
                            }
                            // FLOATZ
                            if(i.tuple->type[newasm::header::data::tupleIndex] == newasm::datatypes::decimal)
                            {
                                if(newasm::header::functions::isnumeric(opr))
                                {
                                    newasm::hardware::randAccessMem.delete__HEAP(i.tuple->addr[newasm::header::data::tupleIndex], i.tuple->addr[newasm::header::data::tupleIndex] + sizeof(float));
                                    i.tuple->addr[newasm::header::data::tupleIndex] = newasm::hardware::randAccessMem.write<int>(std::stoi(opr));
                                    i.tuple->type[newasm::header::data::tupleIndex] = newasm::datatypes::number;
                                    newasm::header::data::tupleIndex = -1;
                                    return 1;
                                }
                                if(newasm::header::functions::isfloat(opr))
                                {
                                    newasm::hardware::randAccessMem.overwrite<float>(i.tuple->addr[newasm::header::data::tupleIndex], std::stof(opr));
                                    newasm::header::data::tupleIndex = -1;
                                    return 1;
                                }
                                if(newasm::header::functions::ischar(opr))
                                {
                                    newasm::hardware::randAccessMem.delete__HEAP(i.tuple->addr[newasm::header::data::tupleIndex], i.tuple->addr[newasm::header::data::tupleIndex] + sizeof(float));
                                    i.tuple->addr[newasm::header::data::tupleIndex] = newasm::hardware::randAccessMem.write<char>(newasm::header::functions::remsq(opr).at(0));
                                    i.tuple->type[newasm::header::data::tupleIndex] = newasm::datatypes::character;
                                    newasm::header::data::tupleIndex = -1;
                                    return 1;
                                }
                                if(newasm::header::functions::istext(opr))
                                {
                                    newasm::hardware::randAccessMem.delete__HEAP(i.tuple->addr[newasm::header::data::tupleIndex], i.tuple->addr[newasm::header::data::tupleIndex] + sizeof(float));
                                    i.tuple->addr[newasm::header::data::tupleIndex] = newasm::hardware::randAccessMem.write<std::string>(newasm::header::functions::remq(opr));
                                    i.tuple->type[newasm::header::data::tupleIndex] = newasm::datatypes::text;
                                    newasm::header::data::tupleIndex = -1;
                                    return 1;
                                }
                            }
                            // CHARACTERZS
                            if(i.tuple->type[newasm::header::data::tupleIndex] == newasm::datatypes::character)
                            {
                                if(newasm::header::functions::isnumeric(opr))
                                {
                                    newasm::hardware::randAccessMem.delete__HEAP(i.tuple->addr[newasm::header::data::tupleIndex], i.tuple->addr[newasm::header::data::tupleIndex] + sizeof(char));
                                    i.tuple->addr[newasm::header::data::tupleIndex] = newasm::hardware::randAccessMem.write<int>(std::stoi(opr));
                                    i.tuple->type[newasm::header::data::tupleIndex] = newasm::datatypes::number;
                                    newasm::header::data::tupleIndex = -1;
                                    return 1;
                                }
                                if(newasm::header::functions::isfloat(opr))
                                {
                                    newasm::hardware::randAccessMem.delete__HEAP(i.tuple->addr[newasm::header::data::tupleIndex], i.tuple->addr[newasm::header::data::tupleIndex] + sizeof(char));
                                    i.tuple->addr[newasm::header::data::tupleIndex] = newasm::hardware::randAccessMem.write<float>(std::stof(opr));
                                    i.tuple->type[newasm::header::data::tupleIndex] = newasm::datatypes::decimal;
                                    newasm::header::data::tupleIndex = -1;
                                    return 1;
                                }
                                if(newasm::header::functions::ischar(opr))
                                {
                                    newasm::hardware::randAccessMem.overwrite<char>(i.tuple->addr[newasm::header::data::tupleIndex], newasm::header::functions::remsq(opr).at(0));
                                    newasm::header::data::tupleIndex = -1;
                                    return 1;
                                }
                                if(newasm::header::functions::istext(opr))
                                {
                                    newasm::hardware::randAccessMem.delete__HEAP(i.tuple->addr[newasm::header::data::tupleIndex], i.tuple->addr[newasm::header::data::tupleIndex] + sizeof(char));
                                    i.tuple->addr[newasm::header::data::tupleIndex] = newasm::hardware::randAccessMem.write<std::string>(newasm::header::functions::remq(opr));
                                    i.tuple->type[newasm::header::data::tupleIndex] = newasm::datatypes::text;
                                    newasm::header::data::tupleIndex = -1;
                                    return 1;
                                }
                            }
                            // TEXT
                            if(i.tuple->type[newasm::header::data::tupleIndex] == newasm::datatypes::text)
                            {
                                int buffer_len;
                                std::memcpy(&buffer_len, &newasm::hardware::randAccessMem.__memory__[i.tuple->addr[newasm::header::data::tupleIndex]], sizeof(int));
                                int bytes = buffer_len + sizeof(int);
                                if(newasm::header::functions::isnumeric(opr))
                                {
                                    newasm::hardware::randAccessMem.delete__HEAP(i.tuple->addr[newasm::header::data::tupleIndex], i.tuple->addr[newasm::header::data::tupleIndex] + bytes);
                                    i.tuple->addr[newasm::header::data::tupleIndex] = newasm::hardware::randAccessMem.write<int>(std::stoi(opr));
                                    i.tuple->type[newasm::header::data::tupleIndex] = newasm::datatypes::number;
                                    newasm::header::data::tupleIndex = -1;
                                    return 1;
                                }
                                if(newasm::header::functions::isfloat(opr))
                                {
                                    newasm::hardware::randAccessMem.delete__HEAP(i.tuple->addr[newasm::header::data::tupleIndex], i.tuple->addr[newasm::header::data::tupleIndex] + bytes);
                                    i.tuple->addr[newasm::header::data::tupleIndex] = newasm::hardware::randAccessMem.write<float>(std::stof(opr));
                                    i.tuple->type[newasm::header::data::tupleIndex] = newasm::datatypes::decimal;
                                    newasm::header::data::tupleIndex = -1;
                                    return 1;
                                }
                                if(newasm::header::functions::ischar(opr))
                                {
                                    newasm::hardware::randAccessMem.delete__HEAP(i.tuple->addr[newasm::header::data::tupleIndex], i.tuple->addr[newasm::header::data::tupleIndex] + bytes);
                                    i.tuple->addr[newasm::header::data::tupleIndex] = newasm::hardware::randAccessMem.write<char>(newasm::header::functions::remsq(opr).at(0));
                                    i.tuple->type[newasm::header::data::tupleIndex] = newasm::datatypes::character;
                                    newasm::header::data::tupleIndex = -1;
                                    return 1;
                                }
                                if(newasm::header::functions::istext(opr))
                                {
                                    i.tuple->addr[newasm::header::data::tupleIndex] = newasm::hardware::randAccessMem.overwrite<std::string>(i.tuple->addr[newasm::header::data::tupleIndex], newasm::header::functions::remq(opr));
                                    newasm::header::data::tupleIndex = -1;
                                    return 1;
                                }
                            }

                            //std::cout << "Ne valja :: suf -> " << suf << std::endl;
                            
                            newasm::terminate(newasm::exit_codes::seg_fault);
                            return 1;
                        }
                        if(newasm::header::data::tupleIndex == -1)
                        {
                            if(!newasm::header::functions::istuple(opr))
                            {
                                newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                                return 1;
                            }
                            int size = i.tuple->addr.size();
                            int addr;
                            // firstly clean the whole tuple
                            for(int idx = 0; idx < size; ++idx)
                            {
                                addr = i.tuple->addr[idx];
                                if(i.tuple->type[idx] == newasm::datatypes::number)
                                {
                                    newasm::hardware::randAccessMem.delete__HEAP(addr, addr + sizeof(int));
                                    continue;
                                }
                                if(i.tuple->type[idx] == newasm::datatypes::decimal)
                                {
                                    newasm::hardware::randAccessMem.delete__HEAP(addr, addr + sizeof(float));
                                    continue;
                                }
                                if(i.tuple->type[idx] == newasm::datatypes::character)
                                {
                                    newasm::hardware::randAccessMem.delete__HEAP(addr, addr + sizeof(char));
                                    continue;
                                }
                                if(i.tuple->type[idx] == newasm::datatypes::text)
                                {
                                    int buffer_size;
                                    std::memcpy(&buffer_size, &newasm::hardware::randAccessMem.__memory__[addr], sizeof(int));
                                    int bytes = sizeof(int) + buffer_size;
                                    newasm::hardware::randAccessMem.delete__HEAP(addr, addr + bytes);
                                    continue;
                                }
                            }

                            // now we reallocate a whole new tuple

                            i.tuple->type.clear();
                            i.tuple->addr.clear();

                            auto contents = newasm::header::functions::parseTuple(opr);

                            for(int idx = 0; idx < contents.size(); ++idx)
                            {
                                int address;
                                std::string value_buf = contents.at(idx);
                                newasm::runtime::functions::parse(value_buf);
                                //integerz
                                if(newasm::header::functions::isnumeric(value_buf))
                                {
                                    address = newasm::hardware::randAccessMem.write<int>(std::stoi(value_buf));
                                    i.tuple->addr.push_back(address);
                                    i.tuple->type.push_back(newasm::datatypes::number);
                                    continue;
                                }
                                //floatz
                                if(newasm::header::functions::isfloat(value_buf))
                                {
                                    address = newasm::hardware::randAccessMem.write<float>(std::stof(value_buf));
                                    i.tuple->addr.push_back(address);
                                    i.tuple->type.push_back(newasm::datatypes::decimal);
                                    continue;
                                }
                                //charz
                                if(newasm::header::functions::ischar(value_buf))
                                {
                                    address = newasm::hardware::randAccessMem.write<char>(newasm::header::functions::remsq(value_buf).at(0));
                                    i.tuple->addr.push_back(address);
                                    i.tuple->type.push_back(newasm::datatypes::character);
                                    continue;
                                }
                                //stringz
                                if(newasm::header::functions::istext(value_buf))
                                {
                                    address = newasm::hardware::randAccessMem.write<std::string>(newasm::header::functions::remq(value_buf));
                                    i.tuple->addr.push_back(address);
                                    i.tuple->type.push_back(newasm::datatypes::text);
                                    continue;
                                }
                                newasm::terminate(newasm::exit_codes::invalid_syntax);
                                return 1;
                            }
                            // after deallocating the whole tuple, we make a new one
                            return 1;
                        }
                    }
                    return 1;
                }

                if(lineInfo.whatAreRegistersLol == INVALID_INS)
                {
                    newasm::terminate(newasm::exit_codes::bus_err);
                    return 1;
                }

                switch(lineInfo.whatAreRegistersLol)
                {
                    //crazy shi
                    case newasm::mem::regs::ax__:
                    {
                        if(lineInfo.altArgType == newasm::datatypes::number)
                        {
                            newasm::mem::regs::ax.set_value<int>(lineInfo.altInt);
                            return 1;
                        }
                        if(lineInfo.altArgType == newasm::datatypes::decimal)
                        {
                            newasm::mem::regs::ax.set_value<float>(lineInfo.altFloat);
                            return 1;
                        }
                        if(lineInfo.altArgType == newasm::datatypes::character)
                        {
                            newasm::mem::regs::ax.set_value<char>(lineInfo.altChar);
                            return 1;
                        }
                        if(lineInfo.altArgType == newasm::datatypes::text)
                        {
                            newasm::mem::regs::ax.set_value<std::string>(lineInfo.altString);
                            return 1;
                        }
                        newasm::terminate(newasm::exit_codes::bus_err);
                        return 1;
                    }
                    //literal shi
                    case newasm::mem::regs::fdx__:
                    {
                        if(lineInfo.altArgType == newasm::datatypes::number)
                        {
                            newasm::mem::regs::fdx = lineInfo.altInt;
                            return 1;
                        }
                        if(!newasm::header::functions::isnumeric(opr))
                        {
                            //std::cout << "Dobili smo error ovdje -> `" << opr << "`" << std::endl;
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::regs::fdx = std::stoi(opr);
                        return 1;
                    }
                    case newasm::mem::regs::rax__:
                    {
                        if(lineInfo.altArgType == newasm::datatypes::number)
                        {
                            newasm::mem::regs::rax = lineInfo.altInt;
                            return 1;
                        }
                        if(!newasm::header::functions::isnumeric(opr))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::regs::rax = std::stoi(opr);
                        return 1;
                    }
                    case newasm::mem::regs::imm__:
                    {
                        if(lineInfo.altArgType == newasm::datatypes::number)
                        {
                            newasm::mem::regs::imm = lineInfo.altInt;
                            return 1;
                        }
                        if(!newasm::header::functions::isnumeric(opr))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::regs::imm = std::stoi(opr);
                        return 1;
                    }
                    case newasm::mem::regs::rbx__:
                    {
                        if(lineInfo.altArgType == newasm::datatypes::decimal)
                        {
                            newasm::mem::regs::rbx = lineInfo.altFloat;
                            return 1;
                        }
                        if(!newasm::header::functions::isfloat(opr))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::regs::rbx = std::stof(opr);
                        return 1;
                    }
                    case newasm::mem::regs::bos__:
                    {
                        if(lineInfo.altArgType == newasm::datatypes::number)
                        {
                            newasm::mem::regs::bos = lineInfo.altInt;
                            return 1;
                        }
                        if(!newasm::header::functions::isnumeric(opr))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::regs::bos = std::stoi(opr);
                        return 1;
                    }
                    case newasm::mem::regs::tlr__:
                    {
                        //using std::cout, std::endl;
                        //cout << "\n\t >> tlr set to: " << opr << endl;
                        newasm::mem::regs::tlr = (opr);
                        return 1;
                    }
                    case newasm::mem::regs::dlx__:
                    {
                        newasm::mem::regs::dlx = (opr);
                        return 1;
                    }
                    case newasm::mem::regs::tr0__:
                    {
                        //using std::cout, std::endl;
                        //cout << "\n\t >> tlr set to: " << opr << endl;
                        newasm::mem::regs::tr0 = (opr);
                        return 1;
                    }
                    case newasm::mem::regs::tr1__:
                    {
                        //using std::cout, std::endl;
                        //cout << "\n\t >> tlr set to: " << opr << endl;
                        newasm::mem::regs::tr1 = (opr);
                        return 1;
                    }
                    case newasm::mem::regs::stl__:
                    {
                        newasm::mem::regs::stl = (opr);
                        return 1;
                    }
                    case newasm::mem::regs::psx__:
                    {
                        newasm::mem::regs::psx = (opr);
                        return 1;
                    }
                    case newasm::mem::regs::stk__:
                    {
                        if(lineInfo.altArgType == newasm::datatypes::number)
                        {
                            newasm::mem::regs::stk = lineInfo.altInt;
                            return 1;
                        }
                        if(!newasm::header::functions::isnumeric(opr))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::regs::stk = std::stoi(opr);
                        return 1;
                    }
                    case newasm::mem::regs::hea__:
                    {
                        //malloc
                        auto i = newasm::header::functions::isallocref(opr);
                        if(i.first)
                        {
                            auto byte = i.second;
                            auto addr = newasm::header::data::malloc_block_used; //newasm::malloc::meta.back();

                            int malloc_size = 0;
                            std::memcpy(&malloc_size, newasm::RAM->get__A(addr), sizeof(int));
                            
                            if(byte >= malloc_size)
                            {
                                newasm::terminate(newasm::exit_codes::seg_fault);
                                return 1;
                            }

                            newasm::mem::regs::hea = addr + sizeof(int) + byte;
                            return 1;
                        }

                        //standard heap adr alloc
                        if(!newasm::header::functions::isnumeric(opr))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::regs::hea = std::stoi(opr) < 0 ? 0 : std::stoi(opr);
                        if(newasm::mem::regs::hea >= newasm::hardware::randAccessMem.get_heap_end())
                        {
                            newasm::terminate(newasm::exit_codes::seg_fault);
                        }
                        return 1;
                    }
                    #if 0
                    case newasm::mem::regs::prp__:
                    {
                        if(!newasm::header::functions::isref(opr))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        if(!newasm::mem::functions::datavalid(
                        newasm::header::functions::remamp(opr), newasm::mem::data) 
                        && !newasm::mem::functions::datavalid(
                        newasm::header::functions::remamp(opr), newasm::variables::ids))
                        {
                            newasm::terminate(newasm::exit_codes::invalid_memacc);
                            return 1;
                        }
                        newasm::mem::regs::prp = (opr);
                        return 1;
                    }
                    #endif
                    case newasm::mem::regs::cpt__: // container pointer
                    {
                        if(!newasm::header::functions::isref(opr))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        if(!newasm::mem::functions::datavalid(
                            newasm::header::functions::remamp(opr), newasm::containers::bit_arrays) &&
                        !newasm::mem::functions::datavalid(
                            newasm::header::functions::remamp(opr), newasm::containers::binary_trees))
                        {
                            newasm::terminate(newasm::exit_codes::invalid_memacc);
                            return 1;
                        }
                        newasm::mem::regs::cpt = (opr);
                        return 1;
                    }
                    case newasm::mem::regs::cpr__:
                    {
                        if(!newasm::header::functions::isnumeric(opr))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::regs::cpr = std::stoi(opr);
                        return 1;
                    }
                    case newasm::mem::regs::cr0__:
                    {
                        if(!newasm::header::functions::isnumeric(opr) && !newasm::header::functions::isfloat(opr))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::regs::cr0 = std::stof(opr);
                        return 1;
                    }
                    case newasm::mem::regs::cr1__:
                    {
                        if(!newasm::header::functions::isnumeric(opr) && !newasm::header::functions::isfloat(opr))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::regs::cr1 = std::stof(opr);
                        return 1;
                    }
                    case newasm::mem::regs::cr2__:
                    {
                        if(!newasm::header::functions::isnumeric(opr) && !newasm::header::functions::isfloat(opr))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::regs::cr2 = std::stoi(opr);
                        return 1;
                    }
                    case newasm::mem::regs::cr3__:
                    {
                        if(!newasm::header::functions::isnumeric(opr) && !newasm::header::functions::isfloat(opr))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::regs::cr3 = std::stoi(opr);
                        return 1;
                    }
                    case newasm::mem::regs::br0__:
                    {
                        if(!newasm::header::functions::isnumeric(opr))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        //std::cout << "Moved " << opr << " into br0" << std::endl;
                        newasm::mem::regs::br0 = std::stoi(opr);
                        return 1;
                    }
                    case newasm::mem::regs::br1__:
                    {
                        if(!newasm::header::functions::isnumeric(opr))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::regs::br1 = std::stoi(opr);
                        return 1;
                    }
                    default:
                    {
                        newasm::terminate(newasm::exit_codes::os_error);
                        return 1;
                    }
                }
                return 1;
            }
            
            
            
            //rem
            case newasm::core::lang_inf::rem:
            {
                //idk,lets do some stupidity
                //i have no idea tbh

                //optimizer just removes it from the binary
                return 1;
            }

            //cmp
            case newasm::core::lang_inf::cmp:
            {
                __newasm_CHECK_JUMP_PROPERLY

                //if(suf == newasm::mem::regs::fdx.identifier())
                int intreg = newasm::header::constants::inv_ireg_val;
                float floatreg = newasm::header::constants::inv_freg_val;
                std::string strreg = newasm::header::constants::inv_reg_val;

                if(lineInfo.whatAreRegistersLol == INVALID_INS)
                {
                    newasm::terminate(newasm::exit_codes::bus_err);
                    return 1;
                }
                switch(lineInfo.whatAreRegistersLol)
                {
                    case newasm::mem::regs::fdx__:
                    {
                        intreg = newasm::mem::regs::fdx;
                        break;
                    }
                    case newasm::mem::regs::imm__:
                    {
                        intreg = newasm::mem::regs::imm;
                        break;
                    }
                    case newasm::mem::regs::rax__:
                    {
                        intreg = newasm::mem::regs::rax;
                        break;
                    }
                    case newasm::mem::regs::bos__:
                    {
                        intreg = newasm::mem::regs::bos;
                        break;
                    }
                    case newasm::mem::regs::stk__:
                    {
                        intreg = newasm::mem::regs::stk;
                        break;
                    }
                    case newasm::mem::regs::hea__:
                    {
                        intreg = newasm::mem::regs::hea;
                        break;
                    }
                    case newasm::mem::regs::cpr__:
                    {
                        intreg = newasm::mem::regs::cpr;
                        break;
                    }
                    case newasm::mem::regs::br0__:
                    {
                        intreg = newasm::mem::regs::br0;
                        break;
                    }
                    case newasm::mem::regs::br1__:
                    {
                        intreg = newasm::mem::regs::br1;
                        break;
                    }

                    case newasm::mem::regs::cr0__:
                    {
                        floatreg = newasm::mem::regs::cr0;
                        break;
                    }
                    case newasm::mem::regs::rbx__:
                    {
                        floatreg = newasm::mem::regs::rbx.get_value();
                        break;
                    }
                    case newasm::mem::regs::cr1__:
                    {
                        floatreg = newasm::mem::regs::cr1;
                        break;
                    }
                    case newasm::mem::regs::cr2__:
                    {
                        intreg = newasm::mem::regs::cr2;
                        break;
                    }
                    case newasm::mem::regs::cr3__:
                    {
                        intreg = newasm::mem::regs::cr3;
                        break;
                    }

                    case newasm::mem::regs::tlr__:
                    {
                        strreg = newasm::mem::regs::tlr;
                        break;
                    }
                    case newasm::mem::regs::dlx__:
                    {
                        strreg = newasm::mem::regs::dlx;
                        break;
                    }
                    case newasm::mem::regs::tr0__:
                    {
                        strreg = newasm::mem::regs::tr0;
                        break;
                    }
                    case newasm::mem::regs::tr1__:
                    {
                        strreg = newasm::mem::regs::tr1;
                        break;
                    }
                    case newasm::mem::regs::stl__:
                    {
                        strreg = newasm::mem::regs::stl;
                        break;
                    }
                    case newasm::mem::regs::psx__:
                    {
                        strreg = newasm::mem::regs::psx;
                        break;
                    }
                    #if 0
                    case newasm::mem::regs::prp__:
                    {
                        strreg = newasm::mem::regs::prp;
                        break;
                    }
                    #endif
                    case newasm::mem::regs::cpt__:
                    {
                        strreg = newasm::mem::regs::cpt;
                        break;
                    }
                }
                
                #if 0
                $defer
                    std::cout << "CPR FLAG RESULT: " << newasm::mem::regs::cpr << std::endl;
                    std::cout << "FDX VALUE: " << newasm::mem::regs::fdx.get_value() << std::endl;
                $
                #endif
                
                if(intreg != newasm::header::constants::inv_ireg_val)
                {
                    if(lineInfo.altArgType == newasm::datatypes::symbol_name)
                    {
                        if(!newasm::header::functions::isnumeric(opr))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                            return 1;
                        }
                        //proceed to comparsion
                        if((intreg) == std::stoi(opr))
                        {
                            newasm::mem::regs::cpr = newasm::cmp_results::equal;
                        }
                        if((intreg) < std::stoi(opr))
                        {
                            newasm::mem::regs::cpr = newasm::cmp_results::less;
                        }
                        if((intreg) > std::stoi(opr))
                        {
                            newasm::mem::regs::cpr = newasm::cmp_results::greater;
                        }
                    }
                    else if(lineInfo.altArgType == newasm::datatypes::number)
                    {
                        if((intreg) == lineInfo.altInt)
                        {
                            newasm::mem::regs::cpr = newasm::cmp_results::equal;
                        }
                        if((intreg) < lineInfo.altInt)
                        {
                            newasm::mem::regs::cpr = newasm::cmp_results::less;
                        }
                        if((intreg) > lineInfo.altInt)
                        {
                            newasm::mem::regs::cpr = newasm::cmp_results::greater;
                        }
                    }
                    else
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    return 1;
                }
                if(floatreg != newasm::header::constants::inv_ireg_val)
                {
                    if(lineInfo.altArgType == newasm::datatypes::symbol_name)
                    {
                        if(!newasm::header::functions::isfloat(opr) && !newasm::header::functions::isnumeric(opr))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                            return 1;
                        }
                        //proceed to comparsion
                        if((floatreg) == std::stof(opr))
                        {
                            newasm::mem::regs::cpr = newasm::cmp_results::equal;
                        }
                        if((floatreg) < std::stof(opr))
                        {
                            newasm::mem::regs::cpr = newasm::cmp_results::less;
                        }
                        if((floatreg) > std::stof(opr))
                        {
                            newasm::mem::regs::cpr = newasm::cmp_results::greater;
                        }
                    }
                    else if(lineInfo.altArgType == newasm::datatypes::decimal)
                    {
                        if((floatreg) == lineInfo.altFloat)
                        {
                            newasm::mem::regs::cpr = newasm::cmp_results::equal;
                        }
                        if((floatreg) < lineInfo.altFloat)
                        {
                            newasm::mem::regs::cpr = newasm::cmp_results::less;
                        }
                        if((floatreg) > lineInfo.altFloat)
                        {
                            newasm::mem::regs::cpr = newasm::cmp_results::greater;
                        }
                    }
                    else if(lineInfo.altArgType == newasm::datatypes::number)
                    {
                        if((floatreg) == lineInfo.altInt)
                        {
                            newasm::mem::regs::cpr = newasm::cmp_results::equal;
                        }
                        if((floatreg) < lineInfo.altInt)
                        {
                            newasm::mem::regs::cpr = newasm::cmp_results::less;
                        }
                        if((floatreg) > lineInfo.altInt)
                        {
                            newasm::mem::regs::cpr = newasm::cmp_results::greater;
                        }
                    }
                    else
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    return 1;
                }
                if(strreg != newasm::header::constants::inv_reg_val)
                {
                    ///////////////////NUMERIC
                    if(newasm::header::functions::isnumeric(opr))
                    {
                        if(newasm::header::functions::isnumeric(strreg) || newasm::header::functions::isfloat(strreg))
                        {
                            //proceed to comparsion
                            if(std::stoi(strreg) == std::stoi(opr))
                            {
                                newasm::mem::regs::cpr = newasm::cmp_results::equal;
                            }
                            if(std::stoi(strreg) < std::stoi(opr))
                            {
                                newasm::mem::regs::cpr = newasm::cmp_results::less;
                            }
                            if(std::stoi(strreg) > std::stoi(opr))
                            {
                                newasm::mem::regs::cpr = newasm::cmp_results::greater;
                            }
                            return 1;
                        }
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    ///////////////////FLOAT
                    if(newasm::header::functions::isfloat(opr))
                    {
                        if(newasm::header::functions::isfloat(strreg) || newasm::header::functions::isnumeric(strreg))
                        {
                            //proceed to comparsion
                            if(std::stof(strreg) == std::stof(opr))
                            {
                                newasm::mem::regs::cpr = newasm::cmp_results::equal;
                            }
                            if(std::stof(strreg) < std::stof(opr))
                            {
                                newasm::mem::regs::cpr = newasm::cmp_results::less;
                            }
                            if(std::stof(strreg) > std::stof(opr))
                            {
                                newasm::mem::regs::cpr = newasm::cmp_results::greater;
                            }
                            return 1;
                        }
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    ///////////////////TEXT
                    if(newasm::header::functions::istext(opr))
                    {
                        if(newasm::header::functions::istext(strreg))
                        {
                            //proceed to comparsion
                            if((strreg) == (opr))
                            {
                                newasm::mem::regs::cpr = newasm::cmp_results::equal;
                            }
                            if((strreg).size() < (opr).size())
                            {
                                newasm::mem::regs::cpr = newasm::cmp_results::less;
                            }
                            if((strreg).size() > (opr).size())
                            {
                                newasm::mem::regs::cpr = newasm::cmp_results::greater;
                            }
                            return 1;
                        }
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    ///////////////////ref
                    if(newasm::header::functions::isref(opr))
                    {
                        if(newasm::header::functions::isref(strreg))
                        {
                            //proceed to comparsion
                            if((strreg) == (opr))
                            {
                                newasm::mem::regs::cpr = newasm::cmp_results::equal;
                            }
                            if((strreg).size() < (opr).size())
                            {
                                newasm::mem::regs::cpr = newasm::cmp_results::less;
                            }
                            if((strreg).size() > (opr).size())
                            {
                                newasm::mem::regs::cpr = newasm::cmp_results::greater;
                            }
                            return 1;
                        }
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    ///////////////////char
                    if(newasm::header::functions::ischar(opr))
                    {
                        if(newasm::header::functions::ischar(strreg))
                        {
                            //proceed to comparsion
                            if(strreg[1] == opr[1])
                            {
                                newasm::mem::regs::cpr = newasm::cmp_results::equal;
                            }
                            if(strreg[1] < opr[1])
                            {
                                newasm::mem::regs::cpr = newasm::cmp_results::less;
                            }
                            if(strreg[1] > opr[1])
                            {
                                newasm::mem::regs::cpr = newasm::cmp_results::greater;
                            }
                            return 1;
                        }
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::invalid_syntax);
                return 1;
            }
            
            default:
            {
                newasm::terminate(newasm::exit_codes::invalid_exp);
                return 1;
            }
        }
            
        return 1;
    }
    FORCE_INLINE inline int process_is(std::string& line, std::string& ins, std::string suf, newasm::compiler::lineData& lineInfo)
    {
        if(newasm::system::stop == 1)
        {
            #if 0
            newasm::system::proclines ++;
            std::string newline = ins + static_cast<std::string>(" ") + suf;
            // OLD -> newasm::mem::funcs[newasm::system::cproc].push_back(newline);
            //std::cout << newasm::system::cproc << " : " << newline << std::endl;
            newasm::mem::funcs[newasm::system::cproc].push_back(lineInfo.raw);
            #endif
            newasm::system::proclines ++;
            //newasm::variables::ids.at(newasm::system::cproc).
            NewASM::CurrentProc->proc->contents.push_back(lineInfo);
            //newasm::mem::funcs[newasm::system::cproc].push_back(lineInfo);
            return 1;
        }
        /*
        auto it = newasm::inverted_ins.find(ins);
        if(it == newasm::inverted_ins.end())
        {
            newasm::terminate(newasm::exit_codes::invalid_ins);//,wholeline);
            return 1;
        }*/

        newasm::rawData priArg;

        switch(lineInfo.whatAmIDoing)//switch(it->second)
        {
            case INVALID_INS:
            {
                newasm::terminate(newasm::exit_codes::invalid_ins);
                return 1;
            }
            case newasm::core::lang_inf::fetch__:
            {
                if(newasm::_this != nullptr)
                {
                    (*newasm::_this)->fetched = false;
                }
                if(lineInfo.priArgType == NewASM::datatypes::NIL)
                {
                    newasm::_this = nullptr;
                    return 1;
                }
                newasm::runtime::functions::parse<true>(suf);
                auto it = newasm::variables::ids.find(suf);
                if(it == newasm::variables::ids.end())
                {
                    newasm::terminate(newasm::exit_codes::invalid_memacc);
                    return 1;
                }
                if(
                    it->second.type == newasm::datatypes::static_objz or
                    it->second.type == newasm::datatypes::event or
                    it->second.type == newasm::datatypes::threadz
                )
                {
                    newasm::terminate(newasm::exit_codes::seg_fault);
                    return 1;
                }
                
                VarPtr ptr = &it->second;

                if(ptr->attrib & newasm::core::lang_inf::attributes::SAFE__) [[unlikely]]
                {
                    newasm::terminate(newasm::exit_codes::seg_fault);
                    return 1;
                }

                if(ptr->fetched and ptr->type != newasm::datatypes::proc)
                {
                    if(newasm::thread_line)
                    {
                        newasm::threads::memory.at(newasm::threads::now)->paused = true;
                        return 1;
                    }

                    newasm::SetExceptionComment("object is already fetched by another thread");
                    newasm::terminate(newasm::exit_codes::seg_fault);
                    return 1;
                }

                newasm::_this = ptr;
                ptr->fetched = true; //this shi prevents other threads from taking our pointer away
                return 1;
            }
            //LOAD.adr/ref
            case newasm::core::lang_inf::load:
            {
                auto& opr = suf;
                //addr
                if(newasm::mem::regs::imm == 1)//(suf == static_cast<std::string>("*"))
                {
                    newasm::runtime::functions::eval(opr, lineInfo.priEvalMode);
                    #if 0
                    if(!newasm::header::functions::isnumeric(opr) && !newasm::header::functions::isfloat(opr) &&
                    !newasm::header::functions::istext(opr) && !newasm::header::functions::isref(opr) &&
                    !newasm::header::functions::ischar(opr))
                    {
                        //std::cout << "opr is " << opr << std::endl;
                        newasm::terminate(newasm::exit_codes::invalid_syntax);
                        return 1;
                    }
                    newasm::mem::program_memory[newasm::mem::regs::hea] = opr;
                    #endif
                    if(lineInfo.priArgType == newasm::datatypes::symbol_name)
                    {
                        if(newasm::header::functions::isnumeric(opr))
                        {
                            auto value = std::stoi(opr);
                            // write at heap pointer
                            newasm::hardware::randAccessMem.write<int>(newasm::mem::regs::hea, value);
                            return 0;
                        }
                        if(newasm::header::functions::isfloat(opr))
                        {
                            auto value = std::stof(opr);
                            // write at heap pointer
                            newasm::hardware::randAccessMem.write<float>(newasm::mem::regs::hea, value);
                            return 0;
                        }
                        if(newasm::header::functions::ischar(opr))
                        {
                            auto value = newasm::header::functions::remsq(opr).at(0);
                            // write at heap pointer
                            newasm::hardware::randAccessMem.write<char>(newasm::mem::regs::hea, value);
                            return 0;
                        }
                        if(newasm::header::functions::istext(opr))
                        {
                            auto value = newasm::header::functions::remq(opr);
                            // write at heap pointer
                            //std::cout << "Writing text " << value << " into addr* " << newasm::mem::regs::hea << std::endl;
                            newasm::hardware::randAccessMem.write<std::string>(newasm::mem::regs::hea, value);
                            return 0;
                        }
                        newasm::terminate(newasm::exit_codes::invalid_syntax);
                        return 1;
                    }
                    if(lineInfo.priArgType == newasm::datatypes::number)
                    {
                        newasm::RAM->write<int>(newasm::mem::regs::hea.get_value(), lineInfo.priInt);
                        return 1;
                    }
                    if(lineInfo.priArgType == newasm::datatypes::decimal)
                    {
                        newasm::RAM->write<float>(newasm::mem::regs::hea.get_value(), lineInfo.priFloat);
                        return 1;
                    }
                    if(lineInfo.priArgType == newasm::datatypes::character)
                    {
                        newasm::RAM->write<char>(newasm::mem::regs::hea.get_value(), lineInfo.priChar);
                        return 1;
                    }
                    if(lineInfo.priArgType == newasm::datatypes::text)
                    {
                        newasm::RAM->write<std::string>(newasm::mem::regs::hea.get_value(), lineInfo.priString);
                        return 1;
                    }
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }
                //ref
                if(newasm::mem::regs::imm == 2)//(suf == static_cast<std::string>("&"))
                {
                    if(!newasm::header::functions::isref(opr))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    opr = newasm::header::functions::remamp(opr);

                    if(newasm::mem::functions::datavalid(opr, newasm::variables::ids))
                    {
                        auto it = newasm::variables::ids.find(opr);
                        if(it->second.type == newasm::datatypes::number)
                        {
                            auto value = newasm::hardware::randAccessMem.peek<int>(newasm::mem::regs::hea);
                            newasm::hardware::randAccessMem.overwrite<int>(it->second.addr, value);
                            return 1;
                        }
                        if(it->second.type == newasm::datatypes::decimal)
                        {
                            auto value = newasm::hardware::randAccessMem.peek<float>(newasm::mem::regs::hea);
                            newasm::hardware::randAccessMem.overwrite<float>(it->second.addr, value);
                            return 1;
                        }
                        if(it->second.type == newasm::datatypes::character)
                        {
                            auto value = newasm::hardware::randAccessMem.peek<char>(newasm::mem::regs::hea);
                            newasm::hardware::randAccessMem.overwrite<char>(it->second.addr, value);
                            return 1;
                        }
                        if(it->second.type == newasm::datatypes::text)
                        {
                            auto value = newasm::hardware::randAccessMem.peek<std::string>(newasm::mem::regs::hea);
                            //std::cout << "Writing text " << value << " into addr& " << it->second.addr << " from addr* " << newasm::mem::regs::hea << std::endl;
                            it->second.addr = newasm::hardware::randAccessMem.overwrite<std::string>(it->second.addr, value);
                            return 1;
                        }
                        return 1;
                    }

                    if(!newasm::mem::functions::datavalid(opr,newasm::mem::data))
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }

                    if(newasm::header::functions::isnumeric
                    (
                        newasm::mem::program_memory
                        [
                            newasm::mem::regs::hea
                        ]
                    ))
                    {
                        if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::data[opr] = newasm::mem::program_memory
                        [
                            newasm::mem::regs::hea
                        ];
                        return 1;
                    }
                    if(newasm::header::functions::isfloat
                    (
                        newasm::mem::program_memory
                        [
                            newasm::mem::regs::hea
                        ]
                    ))
                    {
                        if(newasm::mem::datatypes[opr] != newasm::datatypes::decimal)
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::data[opr] = newasm::mem::program_memory
                        [
                            newasm::mem::regs::hea
                        ];
                        return 1;
                    }
                    if(newasm::header::functions::istext
                    (
                        newasm::mem::program_memory
                        [
                            newasm::mem::regs::hea
                        ]
                    ))
                    {
                        if(newasm::mem::datatypes[opr] != newasm::datatypes::text)
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::data[opr] = newasm::mem::program_memory
                        [
                            newasm::mem::regs::hea
                        ];
                        return 1;
                    }
                    if(newasm::header::functions::isref
                    (
                        newasm::mem::program_memory
                        [
                            newasm::mem::regs::hea
                        ]
                    ))
                    {
                        if(newasm::mem::datatypes[opr] != newasm::datatypes::reference)
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::data[opr] = newasm::mem::program_memory
                        [
                            newasm::mem::regs::hea
                        ];
                        //auto i = std::find(newasm::mem::uninitialized_pointer.begin(), newasm::mem::uninitialized_pointer.end(), opr);
                        //if(i != newasm::mem::uninitialized_pointer.end()) newasm::mem::uninitialized_pointer.erase(i);
                        return 1;
                    }
                    if(newasm::header::functions::ischar
                    (
                        newasm::mem::program_memory
                        [
                            newasm::mem::regs::hea
                        ]
                    ))
                    {
                        if(newasm::mem::datatypes[opr] != newasm::datatypes::character)
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::data[opr] = newasm::mem::program_memory
                        [
                            newasm::mem::regs::hea
                        ];
                        return 1;
                    }
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::invalid_call);
                return 1;
            }
            //movas
            case newasm::core::lang_inf::movas:
            {
                auto it = newasm::inverted_types.find(suf);
                if(it == newasm::inverted_types.end())
                {
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }

                switch(it->second)
                {
                    case newasm::core::lang_inf::typenames::num:
                    {
                        newasm::header::data::movas_type = newasm::core::lang_inf::typenames::num;
                        return 1;
                    }
                    case newasm::core::lang_inf::typenames::decm:
                    {
                        newasm::header::data::movas_type = newasm::core::lang_inf::typenames::decm;
                        return 1;
                    }
                    case newasm::core::lang_inf::typenames::char__:
                    {
                        newasm::header::data::movas_type = newasm::core::lang_inf::typenames::char__;
                        return 1;
                    }
                    case newasm::core::lang_inf::typenames::txt:
                    {
                        newasm::header::data::movas_type = newasm::core::lang_inf::typenames::txt;
                        return 1;
                    }
                }
                newasm::terminate(newasm::exit_codes::invalid_syntax);
                return 1;
            }
            //halt
            case newasm::core::lang_inf::halt:
            {
                if(not newasm::header::data::proc_now and not newasm::LambdaDispatch::LambdaLine)
                {
                    newasm::terminate(newasm::exit_codes::unexpected_end);
                    return 1;
                }
                if(newasm::LambdaDispatch::LambdaLine)
                {
                    newasm::LambdaDispatch::Result = suf;
                    newasm::LambdaDispatch::LambdaHalt = true;
                    return 1;
                }
                newasm::system::stoproc = 1;
                newasm::mem::regs::psx = (suf);
                return 1;
            }
            //je //jz
            case newasm::core::lang_inf::jz:
            case newasm::core::lang_inf::je:
            {
                __newasm_CHECK_JUMP_PROPERLY
                #if 0
                if(newasm::header::data::proc_now)
                if(!newasm::mem::functions::datavalid(suf, newasm::variables::ids.at(newasm::system::processing_proc).proc->labels))
                {
                    #if 0
                    std::cout << "VOLIMO C++!" << std::endl;
                    for(auto it = newasm::variables::ids.at(newasm::system::processing_proc).proc->labels.begin(); it != newasm::variables::ids.at(newasm::system::processing_proc).proc->labels.end(); ++it)
                    {
                        std::cout << "it->first: `" << it->first << "`, it->second: `" << it->second << "`\n";
                    }
                    #endif
                    newasm::terminate(newasm::exit_codes::bus_err);
                    return 1;
                }
                if(!newasm::header::data::proc_now)
                {
                    #if 0
                    if(!newasm::thread_line) if(!newasm::mem::functions::datavalid(suf, newasm::mem::labels))
                    {
                        newasm::terminate(newasm::exit_codes::bus_err);//,wholeline);
                        return 1;
                    }
                    #endif
                    if(newasm::thread_line) if(!newasm::mem::functions::datavalid(suf, newasm::threads::memory.at(newasm::threads::now)->labels))
                    {
                        newasm::terminate(newasm::exit_codes::bus_err);//,wholeline);
                        return 1;
                    }
                }
                #endif
                if(newasm::mem::regs::cpr != newasm::cmp_results::equal)
                {
                    //std::cout << "\nnewasm::mem::regs::cpr = " << newasm::mem::regs::cpr << std::endl;
                    return 1;
                }
                if(newasm::header::data::proc_now)
                {
                    NewASM::CurrentProcA->proc->idx = lineInfo.jumpinTo;//j.proc->labels.at(suf);
                    //std::cout << "jumped to " << j.proc->idx << '\n';
                    return 1;
                }
                else if(newasm::LambdaDispatch::LambdaLine)
                {
                    newasm::LambdaDispatch::ThreadSafePtr->idx = lineInfo.jumpinTo;
                    return 1;
                }

                if(newasm::thread_line)
                {
                    newasm::threads::memory.at(newasm::threads::now)->lcx = lineInfo.jumpinTo;//newasm::threads::memory.at(newasm::threads::now)->labels.at(suf);
                    return 1;
                }

                NEWASM_JMP__(lineInfo.jumpinTo)
                return 1;
            }
            //jne //jnz
            case newasm::core::lang_inf::jnz:
            case newasm::core::lang_inf::jne:
            {
                __newasm_CHECK_JUMP_PROPERLY
                #if 0
                if(newasm::header::data::proc_now)
                if(!newasm::mem::functions::datavalid(suf, newasm::variables::ids.at(newasm::system::processing_proc).proc->labels))
                {
                    newasm::terminate(newasm::exit_codes::bus_err);
                    return 1;
                }
                if(!newasm::header::data::proc_now)
                {
                    #if 0
                    if(!newasm::thread_line) if(!newasm::mem::functions::datavalid(suf, newasm::mem::labels))
                    {
                        newasm::terminate(newasm::exit_codes::bus_err);//,wholeline);
                        return 1;
                    }
                    #endif
                    if(newasm::thread_line) if(!newasm::mem::functions::datavalid(suf, newasm::threads::memory.at(newasm::threads::now)->labels))
                    {
                        newasm::terminate(newasm::exit_codes::bus_err);//,wholeline);
                        return 1;
                    }
                }
                #endif
                if(newasm::mem::regs::cpr == newasm::cmp_results::equal)
                {
                    return 1;
                }
                if(newasm::header::data::proc_now)
                {
                    NewASM::CurrentProcA->proc->idx = lineInfo.jumpinTo;//j.proc->labels.at(suf);
                    return 1;
                }
                else if(newasm::LambdaDispatch::LambdaLine)
                {
                    newasm::LambdaDispatch::ThreadSafePtr->idx = lineInfo.jumpinTo;
                    return 1;
                }

                if(newasm::thread_line)
                {
                    newasm::threads::memory.at(newasm::threads::now)->lcx = lineInfo.jumpinTo;//newasm::threads::memory.at(newasm::threads::now)->labels.at(suf);
                    return 1;
                }

                NEWASM_JMP__(lineInfo.jumpinTo)
                return 1;
            }
            //jl
            case newasm::core::lang_inf::jl:
            {
                __newasm_CHECK_JUMP_PROPERLY
                #if 0
                if(newasm::header::data::proc_now)
                if(!newasm::mem::functions::datavalid(suf, newasm::variables::ids.at(newasm::system::processing_proc).proc->labels))
                {
                    newasm::terminate(newasm::exit_codes::bus_err);
                    return 1;
                }
                if(!newasm::header::data::proc_now)
                {
                    #if 0
                    if(!newasm::thread_line) if(!newasm::mem::functions::datavalid(suf, newasm::mem::labels))
                    {
                        newasm::terminate(newasm::exit_codes::bus_err);//,wholeline);
                        return 1;
                    }
                    #endif
                    if(newasm::thread_line) if(!newasm::mem::functions::datavalid(suf, newasm::threads::memory.at(newasm::threads::now)->labels))
                    {
                        newasm::terminate(newasm::exit_codes::bus_err);//,wholeline);
                        return 1;
                    }
                }
                #endif
                if(newasm::mem::regs::cpr != newasm::cmp_results::less)
                {
                    return 1;
                }

                if(newasm::header::data::proc_now)
                {
                    NewASM::CurrentProcA->proc->idx = lineInfo.jumpinTo;//j.proc->labels.at(suf);
                    return 1;
                }
                else if(newasm::LambdaDispatch::LambdaLine)
                {
                    newasm::LambdaDispatch::ThreadSafePtr->idx = lineInfo.jumpinTo;
                    return 1;
                }

                if(newasm::thread_line)
                {
                    newasm::threads::memory.at(newasm::threads::now)->lcx = lineInfo.jumpinTo;//newasm::threads::memory.at(newasm::threads::now)->labels.at(suf);
                    return 1;
                }

                NEWASM_JMP__(lineInfo.jumpinTo)
                return 1;
            }
            //jg
            case newasm::core::lang_inf::jg:
            {
                __newasm_CHECK_JUMP_PROPERLY

                #if 0
                if(newasm::header::data::proc_now)
                if(!newasm::mem::functions::datavalid(suf, newasm::variables::ids.at(newasm::system::processing_proc).proc->labels))
                {
                    newasm::terminate(newasm::exit_codes::bus_err);
                    return 1;
                }
                if(!newasm::header::data::proc_now)
                {
                    #if 0
                    if(!newasm::thread_line) if(!newasm::mem::functions::datavalid(suf, newasm::mem::labels))
                    {
                        newasm::terminate(newasm::exit_codes::bus_err);//,wholeline);
                        return 1;
                    }
                    #endif
                    if(newasm::thread_line) if(!newasm::mem::functions::datavalid(suf, newasm::threads::memory.at(newasm::threads::now)->labels))
                    {
                        newasm::terminate(newasm::exit_codes::bus_err);//,wholeline);
                        return 1;
                    }
                }
                #endif
                if(newasm::mem::regs::cpr != newasm::cmp_results::greater)
                {
                    return 1;
                }

                if(newasm::header::data::proc_now)
                {
                    NewASM::CurrentProcA->proc->idx = lineInfo.jumpinTo;
                    return 1;
                }
                else if(newasm::LambdaDispatch::LambdaLine)
                {
                    newasm::LambdaDispatch::ThreadSafePtr->idx = lineInfo.jumpinTo;
                    return 1;
                }
                
                if(newasm::thread_line)
                {
                    newasm::threads::memory.at(newasm::threads::now)->lcx = lineInfo.jumpinTo;//newasm::threads::memory.at(newasm::threads::now)->labels.at(suf);
                    return 1;
                }
                
                NEWASM_JMP__(lineInfo.jumpinTo)
                return 1;
            }
            //jle
            case newasm::core::lang_inf::jle:
            {
                __newasm_CHECK_JUMP_PROPERLY
                #if 0
                if(newasm::header::data::proc_now)
                if(!newasm::mem::functions::datavalid(suf, newasm::variables::ids.at(newasm::system::processing_proc).proc->labels))
                {
                    newasm::terminate(newasm::exit_codes::bus_err);
                    return 1;
                }
                if(!newasm::header::data::proc_now)
                {
                    #if 0
                    if(!newasm::thread_line) if(!newasm::mem::functions::datavalid(suf, newasm::mem::labels))
                    {
                        newasm::terminate(newasm::exit_codes::bus_err);//,wholeline);
                        return 1;
                    }
                    #endif
                    if(newasm::thread_line) if(!newasm::mem::functions::datavalid(suf, newasm::threads::memory.at(newasm::threads::now)->labels))
                    {
                        newasm::terminate(newasm::exit_codes::bus_err);//,wholeline);
                        return 1;
                    }
                }
                #endif
                if(newasm::mem::regs::cpr != newasm::cmp_results::less && newasm::mem::regs::cpr != newasm::cmp_results::equal)
                {
                    return 1;
                }

                if(newasm::header::data::proc_now)
                {
                    NewASM::CurrentProcA->proc->idx = lineInfo.jumpinTo;//j.proc->labels.at(suf);
                    return 1;
                }
                else if(newasm::LambdaDispatch::LambdaLine)
                {
                    newasm::LambdaDispatch::ThreadSafePtr->idx = lineInfo.jumpinTo;
                    return 1;
                }

                if(newasm::thread_line)
                {
                    newasm::threads::memory.at(newasm::threads::now)->lcx = lineInfo.jumpinTo;//newasm::threads::memory.at(newasm::threads::now)->labels.at(suf);
                    return 1;
                }
                
                NEWASM_JMP__(lineInfo.jumpinTo)
                return 1;
            }
            //jge
            case newasm::core::lang_inf::jge:
            {
                __newasm_CHECK_JUMP_PROPERLY
                #if 0
                if(newasm::header::data::proc_now)
                if(!newasm::mem::functions::datavalid(suf, newasm::variables::ids.at(newasm::system::processing_proc).proc->labels))
                {
                    newasm::terminate(newasm::exit_codes::bus_err);
                    return 1;
                }
                if(!newasm::header::data::proc_now)
                {
                    if(!newasm::thread_line) if(!newasm::mem::functions::datavalid(suf, newasm::mem::labels))
                    {
                        newasm::terminate(newasm::exit_codes::bus_err);//,wholeline);
                        return 1;
                    }
                    if(newasm::thread_line) if(!newasm::mem::functions::datavalid(suf, newasm::threads::memory.at(newasm::threads::now)->labels))
                    {
                        newasm::terminate(newasm::exit_codes::bus_err);//,wholeline);
                        return 1;
                    }
                }
                #endif
                if(newasm::mem::regs::cpr != newasm::cmp_results::greater && newasm::mem::regs::cpr != newasm::cmp_results::equal)
                {
                    return 1;
                }

                if(newasm::header::data::proc_now)
                {
                    NewASM::CurrentProcA->proc->idx = lineInfo.jumpinTo;//j.proc->labels.at(suf);
                    return 1;
                }
                else if(newasm::LambdaDispatch::LambdaLine)
                {
                    newasm::LambdaDispatch::ThreadSafePtr->idx = lineInfo.jumpinTo;
                    return 1;
                }

                if(newasm::thread_line)
                {
                    newasm::threads::memory.at(newasm::threads::now)->lcx = lineInfo.jumpinTo;//newasm::threads::memory.at(newasm::threads::now)->labels.at(suf);
                    return 1;
                }

                NEWASM_JMP__(lineInfo.jumpinTo)
                return 1;
            }
            //jmp
            case newasm::core::lang_inf::jmp:
            {
                __newasm_CHECK_JUMP_PROPERLY
                #if 0
                if(newasm::header::data::proc_now)
                if(!newasm::mem::functions::datavalid(suf, newasm::variables::ids.at(newasm::system::processing_proc).proc->labels))
                {
                    newasm::terminate(newasm::exit_codes::bus_err);
                    return 1;
                }
                if(!newasm::header::data::proc_now)
                {
                    if(!newasm::thread_line) if(!newasm::mem::functions::datavalid(suf, newasm::mem::labels))
                    {
                        newasm::terminate(newasm::exit_codes::bus_err);//,wholeline);
                        return 1;
                    }
                    if(newasm::thread_line) if(!newasm::mem::functions::datavalid(suf, newasm::threads::memory.at(newasm::threads::now)->labels))
                    {
                        newasm::terminate(newasm::exit_codes::bus_err);//,wholeline);
                        return 1;
                    }
                }
                #endif

                if(newasm::header::data::proc_now)
                {
                    NewASM::CurrentProcA->proc->idx = lineInfo.jumpinTo;//j.proc->labels.at(suf);
                    //std::cout << "Zasto ne skaces pizda ti mater'na? -> " << j.proc->idx << std::endl;
                    return 1;
                }
                else if(newasm::LambdaDispatch::LambdaLine)
                {
                    newasm::LambdaDispatch::ThreadSafePtr->idx = lineInfo.jumpinTo;
                    return 1;
                }

                if(newasm::thread_line)
                {
                    newasm::threads::memory.at(newasm::threads::now)->lcx = lineInfo.jumpinTo;//newasm::threads::memory.at(newasm::threads::now)->labels.at(suf);
                    return 1;
                }

                NEWASM_JMP__(lineInfo.jumpinTo)
                return 1;
            }
            //catch
            case newasm::core::lang_inf::catch__:
            {
                NewASM::variables::procedureData* p = nullptr;
                if(newasm::header::data::proc_now) p = NewASM::CurrentProcA->proc;
                else if(newasm::LambdaDispatch::LambdaLine) p = &(*newasm::LambdaDispatch::ThreadSafePtr);
                if(p)
                {
                    if(!p->TryCatched)
                    {
                        p->TryBlock = false;
                        return 1;
                    }
                    p->TryCatched = false;
                    if(lineInfo.priArgType != newasm::datatypes::NIL)
                    {
                        p->idx = lineInfo.jumpinTo;
                    }
                    return 1;
                }

                if(newasm::thread_line)
                {
                    auto& mmap = newasm::threads::memory.at(newasm::threads::now);
                    if(!mmap->TryCatched)
                    {
                        mmap->TryBlock = false;
                        return 1;
                    }
                    mmap->TryCatched = false;
                    if(lineInfo.priArgType != newasm::datatypes::NIL)
                    {
                        mmap->lcx = lineInfo.jumpinTo;
                    }
                    return 1;
                }

                using namespace NewASM::header::data;
                if(!TryCatched)
                {
                    TryBlock = false;
                    return 1;
                }

                TryCatched = false;
                if(lineInfo.priArgType != newasm::datatypes::NIL)
                {
                    NEWASM_JMP__(lineInfo.jumpinTo)
                }
                return 1;
            }
            //callc
            case newasm::core::lang_inf::callc:
            {
                if(newasm::header::data::proc_now)
                {
                    auto& j = NewASM::CurrentProcA;
                    j->proc->idx = lineInfo.jumpinTo;
                    j->proc->CallCStack.push_back(lineInfo.returninTo);
                    return 1;
                }
                else if(newasm::LambdaDispatch::LambdaLine)
                {
                    auto& j = newasm::LambdaDispatch::ThreadSafePtr;
                    j->idx = lineInfo.jumpinTo;
                    j->CallCStack.push_back(lineInfo.returninTo);
                    return 1;
                }

                if(newasm::thread_line)
                {
                    newasm::threads::memory.at(newasm::threads::now)->lcx = lineInfo.jumpinTo;
                    NewASM::header::data::CallCStack->push_back(lineInfo.returninTo);
                    return 1;
                }

                NEWASM_JMP__(lineInfo.jumpinTo)
                NewASM::header::data::CallCStack->push_back(lineInfo.returninTo);
                //std::cout << "CallCStack size = " << NewASM::header::data::CallCStack->size() << std::endl;
                //std::cout << "CallCStack pushed = " << NewASM::header::data::CallCStack->back() << std::endl;
                return 1;
            }
            //del
            case newasm::core::lang_inf::del:
            {
                VarPtr ptr = nullptr;
                if(lineInfo.priArgType == newasm::datatypes::ThisPtr)
                {
                    if(newasm::_this == nullptr)
                    {
                        newasm::SetExceptionComment("`this` is probably not initialized");
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    ptr = newasm::_this;
                }
                else if(lineInfo.priArgType != newasm::datatypes::ThisPtr)
                {
                    newasm::runtime::functions::eval(suf, lineInfo.priEvalMode);
                    if(!newasm::header::functions::isref(suf))
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }

                    suf = newasm::header::functions::remamp(suf);
                    auto it = newasm::variables::ids.find(suf);
                    if(it == newasm::variables::ids.end())
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    ptr = &it->second;
                }
                
                if(ptr->type == newasm::datatypes::number)
                {
                    newasm::hardware::randAccessMem.delete__HEAP(ptr->addr, ptr->addr + sizeof(int));
                    return 1;
                }
                if(ptr->type == newasm::datatypes::decimal)
                {
                    newasm::hardware::randAccessMem.delete__HEAP(ptr->addr, ptr->addr + sizeof(float));
                    return 1;
                }
                if(ptr->type == newasm::datatypes::character)
                {
                    newasm::hardware::randAccessMem.delete__HEAP(ptr->addr, ptr->addr + sizeof(char));
                    return 1;
                }
                if(ptr->type == newasm::datatypes::text)
                {
                    int buffer_len = newasm::hardware::randAccessMem.peek<int>(ptr->addr);
                    newasm::hardware::randAccessMem.delete__HEAP(ptr->addr, ptr->addr + sizeof(int) + buffer_len);
                    return 1;
                }
                if(ptr->type == newasm::datatypes::yunion)
                {
                    int yunion_addr = ptr->yunion->addr;
                    if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::num)
                    {
                        newasm::hardware::randAccessMem.delete__HEAP(yunion_addr, yunion_addr + sizeof(int));
                        return 1;
                    }
                    if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::decm)
                    {
                        newasm::hardware::randAccessMem.delete__HEAP(yunion_addr, yunion_addr + sizeof(float));
                        return 1;
                    }
                    if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::char__)
                    {
                        newasm::hardware::randAccessMem.delete__HEAP(yunion_addr, yunion_addr + sizeof(char));
                        return 1;
                    }
                    if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::txt)
                    {
                        int buffer_len = newasm::hardware::randAccessMem.peek<int>(yunion_addr);
                        newasm::hardware::randAccessMem.delete__HEAP(yunion_addr, yunion_addr + sizeof(int) + buffer_len);
                        return 1;
                    }
                    newasm::terminate(newasm::exit_codes::seg_fault);
                    return 1;
                }
                if(ptr->type == newasm::datatypes::tuple)
                {
                    if(newasm::header::data::tupleIndex == -1)
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }

                    int effectiveaddr = ptr->tuple->addr[newasm::header::data::tupleIndex];
                
                    if(ptr->tuple->type[newasm::header::data::tupleIndex] == newasm::datatypes::number)
                    {
                        newasm::hardware::randAccessMem.delete__HEAP(effectiveaddr, effectiveaddr + sizeof(int));
                        newasm::header::data::tupleIndex = -1;
                        return 1;
                    }
                    if(ptr->tuple->type[newasm::header::data::tupleIndex] == newasm::datatypes::decimal)
                    {
                        newasm::hardware::randAccessMem.delete__HEAP(effectiveaddr, effectiveaddr + sizeof(float));
                        newasm::header::data::tupleIndex = -1;
                        return 1;
                    }
                    if(ptr->tuple->type[newasm::header::data::tupleIndex] == newasm::datatypes::character)
                    {
                        newasm::hardware::randAccessMem.delete__HEAP(effectiveaddr, effectiveaddr + sizeof(char));
                        newasm::header::data::tupleIndex = -1;
                        return 1;
                    }
                    if(ptr->tuple->type[newasm::header::data::tupleIndex] == newasm::datatypes::text)
                    {
                        int buffer_len = newasm::hardware::randAccessMem.peek<int>(effectiveaddr);
                        newasm::hardware::randAccessMem.delete__HEAP(effectiveaddr, effectiveaddr + sizeof(int) + buffer_len);
                        newasm::header::data::tupleIndex = -1;
                        return 1;
                    }

                    newasm::terminate(newasm::exit_codes::os_error);
                    return 1;
                }

                newasm::terminate(newasm::exit_codes::seg_fault);
                return 1;
            }
            //pop
            case newasm::core::lang_inf::pop:
            {
                if(newasm::RAM->StackInfo.empty())
                {
                    newasm::terminate(newasm::exit_codes::os_error);
                    return 1;
                }
                int type = newasm::RAM->StackInfo.back().stkType;
                newasm::RAM->StackInfo.pop_back();
                if(lineInfo.priArgType == NewASM::datatypes::NIL)
                {
                    if(type == newasm::datatypes::number)
                    {
                        newasm::hardware::randAccessMem.pop__STACK<int>();
                        return 1;
                    }
                    if(type == newasm::datatypes::decimal)
                    {
                        newasm::hardware::randAccessMem.pop__STACK<float>();
                        return 1;
                    }
                    if(type == newasm::datatypes::character)
                    {
                        newasm::hardware::randAccessMem.pop__STACK<char>();
                        return 1;
                    }
                    if(type == newasm::datatypes::text)
                    {
                        newasm::hardware::randAccessMem.pop__STACK<std::string>();
                        return 1;
                    }
                    return 1;
                }

                VarPtr ptr = nullptr;
                if(lineInfo.priArgType == newasm::datatypes::ThisPtr)
                {
                    if(newasm::_this == nullptr)
                    {
                        newasm::SetExceptionComment("`this` is probably not initialized");
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    ptr = newasm::_this;
                }
                else if(lineInfo.priArgType != newasm::datatypes::ThisPtr)
                {
                    if(!newasm::header::functions::isref(suf))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    suf = newasm::header::functions::remamp(suf);
                    newasm::runtime::functions::parse<true>(suf);
    
                    auto it = newasm::variables::ids.find(suf);
                    if(it == newasm::variables::ids.end())
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    ptr = &it->second;
                }
                    
                // integers
                if(type == newasm::datatypes::number)
                {
                    if(ptr->type != newasm::datatypes::number)
                    {
                        newasm::SetExceptionComment("object is not an integer");
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }

                    int value;
                    newasm::hardware::randAccessMem.pop__STACK<int>(value);
                    ptr->addr = newasm::hardware::randAccessMem.overwrite<int>(ptr->addr, value);
                    return 1;
                }
                // floats
                if(type == newasm::datatypes::decimal)
                {
                    if(ptr->type != newasm::datatypes::decimal)
                    {
                        newasm::SetExceptionComment("object is not a float");
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }

                    float value;
                    newasm::hardware::randAccessMem.pop__STACK<float>(value);
                    ptr->addr = newasm::hardware::randAccessMem.overwrite<float>(ptr->addr, value);
                    return 1;
                }
                // char
                if(type == newasm::datatypes::character)
                {
                    if(ptr->type != newasm::datatypes::character)
                    {
                        newasm::SetExceptionComment("object is not a character");
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }

                    char value;
                    newasm::hardware::randAccessMem.pop__STACK<char>(value);
                    ptr->addr = newasm::hardware::randAccessMem.overwrite<char>(ptr->addr, value);
                    return 1;
                }
                // string
                if(type == newasm::datatypes::text)
                {
                    if(ptr->type != newasm::datatypes::text)
                    {
                        newasm::SetExceptionComment("object is not a string");
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }

                    std::string value;
                    newasm::hardware::randAccessMem.pop__STACK<std::string>(value);
                    ptr->addr = newasm::hardware::randAccessMem.overwrite<std::string>(ptr->addr, value);
                    return 1;
                }

                newasm::terminate(newasm::exit_codes::jit_fail);
                #if 0
                auto opr = suf;
                if(newasm::mem::regs::stk == newasm::mem::inf::mem_size - 1)
                {
                    newasm::terminate(newasm::exit_codes::mem_overflow);//,wholeline);
                    return 1;
                }
                if(opr == static_cast<std::string>("\%nl"))
                {
                    newasm::mem::regs::stk = newasm::mem::regs::stk + 1;
                    return 1;
                }
                if(!newasm::header::functions::isref(opr))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }
                opr = newasm::header::functions::remamp(opr);
                if(!newasm::mem::functions::datavalid(opr,newasm::mem::data))
                {
                    //std::cout << "opr :: '" << opr << "'" << std::endl;
                    newasm::terminate(newasm::exit_codes::data_overflow);//,wholeline);
                    return 1;
                }

                if(newasm::header::functions::isnumeric(newasm::mem::program_memory[newasm::mem::regs::stk + 1]))
                {
                    if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    newasm::mem::data[opr] = newasm::mem::program_memory[newasm::mem::regs::stk + 1];
                    newasm::mem::regs::stk = newasm::mem::regs::stk + 1;
                    return 1;
                }
                if(newasm::header::functions::isfloat(newasm::mem::program_memory[newasm::mem::regs::stk + 1]))
                {
                    if(newasm::mem::datatypes[opr] != newasm::datatypes::decimal)
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    newasm::mem::data[opr] = newasm::mem::program_memory[newasm::mem::regs::stk + 1];
                    newasm::mem::regs::stk = newasm::mem::regs::stk + 1;
                    return 1;
                }
                if(newasm::header::functions::istext(newasm::mem::program_memory[newasm::mem::regs::stk + 1]))
                {
                    if(newasm::mem::datatypes[opr] != newasm::datatypes::text)
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    newasm::mem::data[opr] = newasm::mem::program_memory[newasm::mem::regs::stk + 1];
                    newasm::mem::regs::stk = newasm::mem::regs::stk + 1;
                    return 1;
                }
                if(newasm::header::functions::isref(newasm::mem::program_memory[newasm::mem::regs::stk + 1]))
                {
                    if(newasm::mem::datatypes[opr] != newasm::datatypes::reference)
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    newasm::mem::data[opr] = newasm::mem::program_memory[newasm::mem::regs::stk + 1];
                    newasm::mem::regs::stk = newasm::mem::regs::stk + 1;

                    //auto i = std::find(newasm::mem::uninitialized_pointer.begin(), newasm::mem::uninitialized_pointer.end(), opr);
                    //if(i != newasm::mem::uninitialized_pointer.end()) newasm::mem::uninitialized_pointer.erase(i);
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                #endif

                return 1;
            }
            //pop
            case newasm::core::lang_inf::pop__THREAD:
            {
                //std::cout << "popt POZVAN -> " << lineInfo.raw << std::endl;
                auto& DedicatedStack = *newasm::DedicatedMemory;
                if(DedicatedStack.StackInfo.empty())
                {
                    newasm::terminate(newasm::exit_codes::os_error);
                    return 1;
                }
                int type = DedicatedStack.StackInfo.back().stkType;
                DedicatedStack.StackInfo.pop_back();
                if(lineInfo.priArgType == NewASM::datatypes::NIL)
                {
                    if(type == newasm::datatypes::number)
                    {
                        newasm::hardware::randAccessMem.pop__STACK<int, true>();
                        return 1;
                    }
                    if(type == newasm::datatypes::decimal)
                    {
                        newasm::hardware::randAccessMem.pop__STACK<float, true>();
                        return 1;
                    }
                    if(type == newasm::datatypes::character)
                    {
                        newasm::hardware::randAccessMem.pop__STACK<char, true>();
                        return 1;
                    }
                    if(type == newasm::datatypes::text)
                    {
                        newasm::hardware::randAccessMem.pop__STACK<std::string, true>();
                        return 1;
                    }
                    return 1;
                }

                VarPtr ptr = nullptr;
                if(lineInfo.priArgType == newasm::datatypes::ThisPtr)
                {
                    if(newasm::_this == nullptr)
                    {
                        newasm::SetExceptionComment("`this` is probably not initialized");
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    ptr = newasm::_this;
                }
                else if(lineInfo.priArgType != newasm::datatypes::ThisPtr)
                {
                    if(!newasm::header::functions::isref(suf))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    suf = newasm::header::functions::remamp(suf);
                    newasm::runtime::functions::parse<true>(suf);
    
                    auto it = newasm::variables::ids.find(suf);
                    if(it == newasm::variables::ids.end())
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    ptr = &it->second;
                }
                
                // integers
                if(type == newasm::datatypes::number)
                {
                    if(ptr->type != newasm::datatypes::number)
                    {
                        newasm::SetExceptionComment("object is not an integer");
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }

                    int value;
                    newasm::hardware::randAccessMem.pop__STACK<int, true>(value);
                    ptr->addr = newasm::hardware::randAccessMem.overwrite<int>(ptr->addr, value);
                    return 1;
                }
                // floats
                if(type == newasm::datatypes::decimal)
                {
                    if(ptr->type != newasm::datatypes::decimal)
                    {
                        newasm::SetExceptionComment("object is not a float");
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }

                    float value;
                    newasm::hardware::randAccessMem.pop__STACK<float, true>(value);
                    ptr->addr = newasm::hardware::randAccessMem.overwrite<float>(ptr->addr, value);
                    return 1;
                }
                // char
                if(type == newasm::datatypes::character)
                {
                    if(ptr->type != newasm::datatypes::character)
                    {
                        newasm::SetExceptionComment("object is not a character");
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }

                    char value;
                    newasm::hardware::randAccessMem.pop__STACK<char, true>(value);
                    ptr->addr = newasm::hardware::randAccessMem.overwrite<char>(ptr->addr, value);
                    return 1;
                }
                // string
                if(type == newasm::datatypes::text)
                {
                    if(ptr->type != newasm::datatypes::text)
                    {
                        newasm::SetExceptionComment("object is not a string");
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }

                    std::string value;
                    newasm::hardware::randAccessMem.pop__STACK<std::string, true>(value);
                    ptr->addr = newasm::hardware::randAccessMem.overwrite<std::string>(ptr->addr, value);
                    return 1;
                }

                newasm::terminate(newasm::exit_codes::jit_fail);
                return 1;
            }
            //push
            case newasm::core::lang_inf::push:
            {
                if(newasm::mem::regs::imm == 1 && lineInfo.priArgType == newasm::datatypes::number)
                {
                    newasm::hardware::randAccessMem.push__STACK<int>(lineInfo.priInt);

                    newasm::header::data::callstkidx = newasm::mem::regs::stk;
                    auto it = newasm::stack::events.find(lineInfo.priInt);
                    if(it != newasm::stack::events.end())
                    {
                        newasm::header::data::temp_ = it->second;
                        newasm::runtime::functions::parse<true>(newasm::header::data::temp_);
                        newasm::callproc(newasm::header::data::temp_);
                        return 1;
                    }
                    newasm::terminate(newasm::exit_codes::unknown_event);
                    return 1;
                }

                //we favorize raw binary data
                if(lineInfo.priArgType == newasm::datatypes::number)
                {
                    newasm::hardware::randAccessMem.push__STACK<int>(lineInfo.priInt);
                }
                else if(lineInfo.priArgType == newasm::datatypes::decimal)
                {
                    newasm::hardware::randAccessMem.push__STACK<float>(lineInfo.priFloat);
                }
                else if(lineInfo.priArgType == newasm::datatypes::character)
                {
                    newasm::hardware::randAccessMem.push__STACK<char>(lineInfo.priChar);
                }
                else if(lineInfo.priArgType == newasm::datatypes::text)
                {
                    newasm::hardware::randAccessMem.push__STACK<std::string>(lineInfo.priString);
                }
                //then fetching
                else if(lineInfo.priArgType == newasm::datatypes::symbol_name)
                {
                    newasm::runtime::functions::eval<true>(suf, lineInfo.priEvalMode, &priArg);
                    if(priArg.rawType == newasm::datatypes::number)
                    {
                        newasm::hardware::randAccessMem.push__STACK<int>(priArg.rawInt);
                    }
                    else if(priArg.rawType == newasm::datatypes::decimal)
                    {
                        newasm::hardware::randAccessMem.push__STACK<float>(priArg.rawFloat);
                    }
                    else if(priArg.rawType == newasm::datatypes::character)
                    {
                        newasm::hardware::randAccessMem.push__STACK<char>(priArg.rawChar);
                    }
                    else if(priArg.rawType == newasm::datatypes::text)
                    {
                        newasm::hardware::randAccessMem.push__STACK<std::string>(priArg.rawString);
                    }
                    else if(priArg.rawType == newasm::datatypes::symbol_name)
                    {
                        if(newasm::header::functions::isnumeric(suf))
                        {
                            newasm::hardware::randAccessMem.push__STACK<int>(std::stoi(suf));
                        }
                        else if(newasm::header::functions::isfloat(suf))
                        {
                            newasm::hardware::randAccessMem.push__STACK<float>(std::stof(suf));
                        }
                        else if(newasm::header::functions::ischar(suf))
                        {
                            char CHAR__ = newasm::header::functions::remsq(suf).at(0);
                            newasm::hardware::randAccessMem.push__STACK<char>(CHAR__);
                        }
                        else if(newasm::header::functions::istext(suf))
                        {
                            newasm::hardware::randAccessMem.push__STACK<std::string>(newasm::header::functions::remq(suf));
                        }
                        else
                        {
                            newasm::terminate(newasm::exit_codes::jit_fail);
                            return 1;
                        }
                    }
                    else
                    {
                        newasm::terminate(newasm::exit_codes::jit_fail);
                        return 1;
                    }
                }
                else
                {
                    newasm::terminate(newasm::exit_codes::jit_fail);
                    return 1;
                }

                if(newasm::mem::functions::check_stkhea_col())
                {
                    newasm::terminate(newasm::exit_codes::stkhea_col);//,wholeline);
                    return 1;
                }
                return 1;
            }
            //push THREAD
            case newasm::core::lang_inf::push__THREAD:
            {
                //std::cout << "pusht POZVAN -> " << lineInfo.raw << std::endl;
                if(newasm::mem::regs::imm == 1 && lineInfo.priArgType == newasm::datatypes::number)
                {
                    newasm::hardware::randAccessMem.push__STACK<int, true>(lineInfo.priInt);

                    newasm::header::data::callstkidx = newasm::mem::regs::stk;
                    auto it = newasm::stack::events.find(lineInfo.priInt);
                    if(it != newasm::stack::events.end())
                    {
                        newasm::header::data::temp_ = it->second;
                        newasm::runtime::functions::parse<true>(newasm::header::data::temp_);
                        newasm::callproc(newasm::header::data::temp_);
                        return 1;
                    }
                    newasm::terminate(newasm::exit_codes::unknown_event);
                    return 1;
                }

                if(!newasm::DedicatedMemory->Usable)
                {
                    newasm::SetExceptionComment("stack has to be dedicated with `resb`");
                    newasm::terminate(newasm::exit_codes::seg_fault);
                    return 1;
                }

                //we favorize raw binary data
                if(lineInfo.priArgType == newasm::datatypes::number)
                {
                    newasm::RAM->push__STACK<int, true>(lineInfo.priInt);
                }
                else if(lineInfo.priArgType == newasm::datatypes::decimal)
                {
                    newasm::RAM->push__STACK<float, true>(lineInfo.priFloat);
                }
                else if(lineInfo.priArgType == newasm::datatypes::character)
                {
                    newasm::RAM->push__STACK<char, true>(lineInfo.priChar);
                }
                else if(lineInfo.priArgType == newasm::datatypes::text)
                {
                    newasm::RAM->push__STACK<std::string, true>(lineInfo.priString);
                }
                //then fetching
                else if(lineInfo.priArgType == newasm::datatypes::symbol_name)
                {
                    newasm::runtime::functions::eval<true>(suf, lineInfo.priEvalMode, &priArg);
                    if(priArg.rawType == newasm::datatypes::number)
                    {
                        newasm::RAM->push__STACK<int, true>(priArg.rawInt);
                    }
                    else if(priArg.rawType == newasm::datatypes::decimal)
                    {
                        newasm::RAM->push__STACK<float, true>(priArg.rawFloat);
                    }
                    else if(priArg.rawType == newasm::datatypes::character)
                    {
                        newasm::RAM->push__STACK<char, true>(priArg.rawChar);
                    }
                    else if(priArg.rawType == newasm::datatypes::text)
                    {
                        newasm::RAM->push__STACK<std::string, true>(priArg.rawString);
                    }
                    else if(priArg.rawType == newasm::datatypes::symbol_name)
                    {
                        if(newasm::header::functions::isnumeric(suf))
                        {
                            newasm::RAM->push__STACK<int, true>(std::stoi(suf));
                        }
                        else if(newasm::header::functions::isfloat(suf))
                        {
                            newasm::RAM->push__STACK<float, true>(std::stof(suf));
                        }
                        else if(newasm::header::functions::ischar(suf))
                        {
                            char CHAR__ = newasm::header::functions::remsq(suf).at(0);
                            newasm::RAM->push__STACK<char, true>(CHAR__);
                        }
                        else if(newasm::header::functions::istext(suf))
                        {
                            newasm::RAM->push__STACK<std::string, true>(newasm::header::functions::remq(suf));
                        }
                        else
                        {
                            newasm::terminate(newasm::exit_codes::jit_fail);
                            return 1;
                        }
                    }
                    else
                    {
                        newasm::terminate(newasm::exit_codes::jit_fail);
                        return 1;
                    }
                }
                else
                {
                    newasm::terminate(newasm::exit_codes::jit_fail);
                    return 1;
                }

            
                if(newasm::RAM->getDedicatedStackHeapCollision(DedicatedMemory))
                {
                    newasm::terminate(newasm::exit_codes::stkhea_col);
                    return 1;
                }
                return 1;
            }
            //cast
            case newasm::core::lang_inf::cast__:
            {
                if(suf == newasm::core::lang_inf::typenames::identifiers__.at(
                    newasm::core::lang_inf::typenames::num
                ))
                {
                    newasm::_virtual::readMode = newasm::_virtual::readmode_num;
                    return 1;
                }
                if(suf == newasm::core::lang_inf::typenames::identifiers__.at(
                    newasm::core::lang_inf::typenames::decm
                ))
                {
                    newasm::_virtual::readMode = newasm::_virtual::readmode_decm;
                    return 1;
                }
                if(suf == newasm::core::lang_inf::typenames::identifiers__.at(
                    newasm::core::lang_inf::typenames::char__
                ))
                {
                    newasm::_virtual::readMode = newasm::_virtual::readmode_char;
                    return 1;
                }
                if(suf == newasm::core::lang_inf::typenames::identifiers__.at(
                    newasm::core::lang_inf::typenames::txt
                ))
                {
                    newasm::_virtual::readMode = newasm::_virtual::readmode_txt;
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::invalid_syntax);
                return 1;
            }
            //out
            case newasm::core::lang_inf::out__:
            {
                if(lineInfo.priArgType == newasm::datatypes::number)
                {
                    newasm::hardware::outIOPOrt(lineInfo.priInt);
                    return 1;
                }

                newasm::runtime::functions::eval<true>(suf, lineInfo.priEvalMode, &priArg);
                if(priArg.rawType != newasm::datatypes::number)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }

                newasm::hardware::outIOPOrt(priArg.rawInt);
                return 1;
            }
            //in
            case newasm::core::lang_inf::in__:
            {
                if(lineInfo.priArgType == newasm::datatypes::number)
                {
                    newasm::mem::regs::tlr.set_value(newasm::hardware::inIOPort(lineInfo.priInt));
                    return 1;
                }

                newasm::runtime::functions::eval<true>(suf, lineInfo.priEvalMode, &priArg);
                if(priArg.rawType != newasm::datatypes::number)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }

                newasm::mem::regs::tlr.set_value(newasm::hardware::inIOPort(priArg.rawInt));
                return 1;
            }
            //switch
            case newasm::core::lang_inf::switch__:
            {
                newasm::runtime::functions::eval(suf, lineInfo.priEvalMode);

                newasm::header::data::switched_value = suf;
                newasm::header::data::case_matched = false;

                //newasm::header::data::case_line = newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::nop);
                return 1;
            }
            //case
            case newasm::core::lang_inf::case__:
            {
                if(newasm::header::data::case_matched)
                {
                    return 1;
                }
                newasm::runtime::functions::parse(suf);
                
                if(suf == newasm::header::data::switched_value)
                {
                    newasm::header::data::case_matched = true;
                    //newasm::procline(newasm::header::data::case_line);
                    //newasm::header::data::case_line.clear();
                    newasm::procline(newasm::compiler::caseJumpTable.at(lineInfo.caseTableAddress));
                    return 1;
                }
                if(newasm::header::functions::isnumeric(newasm::header::data::switched_value))
                {
                    auto isrange = newasm::header::functions::isrange(suf);
                    //std::cout << "\tsuf = " << suf << std::endl;
                    //std::cout << "isrange.first = `" << isrange.first << "`\n";
                    //std::cout << "isrange.second.first = `" << isrange.second.first << "`\n";
                    //std::cout << "isrange.second.second = `" << isrange.second.second << "`\n";
                    if(isrange.first)
                    {
                        if(isrange.second.first <= std::stoi(suf) || std::stoi(suf) <= isrange.second.second)
                        {
                            newasm::header::data::case_matched = true;
                            newasm::procline(newasm::compiler::caseJumpTable.at(lineInfo.caseTableAddress));//newasm::procline(newasm::header::data::case_line);
                            //newasm::header::data::case_line.clear()
                            return 1;
                        }
                    }
                }
                if(!newasm::header::functions::isnumeric(suf))
                {
                    if(newasm::header::functions::case_typename(
                        newasm::header::data::switched_value, suf
                    ))
                    {
                        newasm::header::data::case_matched = true;
                        newasm::procline(newasm::compiler::caseJumpTable.at(lineInfo.caseTableAddress));//newasm::procline(newasm::header::data::case_line);
                        return 1;
                    }
                }
                return 1;
            }
            case newasm::core::lang_inf::evt:
            {
                #if 0
                auto objectData = newasm::header::functions::parseObject(newasm::header::data::case_line);
                if(objectData.first)
                {
                    std::string member_name = newasm::header::functions::trim(objectData.second.second);
                    std::string struct_name = newasm::header::functions::trim(objectData.second.first);

                    if(newasm::header::functions::parseNamespaceSegments(struct_name).first)
                    {
                        newasm::progwin::api::cout("Object<yes> NMS -> " + struct_name);
                        auto i = newasm::header::functions::parseNamespaceSegments(struct_name);
                        std::string symbol_name = i.second.back();
                        auto vec = i.second;
                        vec.pop_back(); // namespace list
                        
                        struct_name = newasm::header::functions::mangleName(vec, symbol_name);
                    }
                }
                #endif
                if(lineInfo.priArgType == newasm::datatypes::number)//if(newasm::header::functions::ishex(suf))
                {
                    std::string proc_name = newasm::header::data::case_line;
                    if(newasm::header::functions::parseNamespaceSegments(newasm::header::data::case_line).first)
                    {
                        newasm::progwin::api::cout("Object<yes> NMS -> " + newasm::header::data::case_line);
                        auto i = newasm::header::functions::parseNamespaceSegments(newasm::header::data::case_line);
                        std::string symbol_name = i.second.back();
                        auto vec = i.second;
                        vec.pop_back(); // namespace list
                        
                        proc_name = newasm::header::functions::mangleName(vec, symbol_name);
                    }
                    newasm::process_hndl(lineInfo.priInt, proc_name);
                    return 1;
                }
                if(lineInfo.caseLineArgType != NewASM::datatypes::tokenOpenBrace)//if(newasm::header::data::case_line != OPEN_BRACE_STR)
                {
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }
                switch(lineInfo.whatTheFuckAreEvents)
                {
                    case INVALID_INS:
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    case newasm::events::exitId:
                    {
                        newasm::events::current = newasm::events::exitId;
                        newasm::events::parsing_now = true;
                        newasm::brace_stack__.push_back(newasm::brace_stack::event_block);
                        return 1;
                    }
                }
                return 1;
            }
            //sysreq
            case newasm::core::lang_inf::sysreq:
            {
                if(!newasm::header::functions::isref(suf))
                {
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }
                suf = newasm::header::functions::remamp(suf);

                newasm::runtime::functions::parse<true>(suf);

                //std::cout << "sysreq suf: `" << suf << "`" << std::endl;

                auto& typ = newasm::header::data::case_line;
                auto it = newasm::inverted_types.find(typ);
                int type = 0;
                int real_type = 0;
                if(it == newasm::inverted_types.end())
                {
                    if(typ == "thread")
                    {
                        if(newasm::threads::memory.find(suf) == newasm::threads::memory.end())
                        {
                            //std::cout << "cant find thread: `" << suf << "`" << std::endl;
                            newasm::terminate(newasm::exit_codes::sysreq_fail);
                            return 1;
                        }
                        return 1;
                    }
                    
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }

                type = it->second;

                auto it2 = newasm::variables::ids.find(suf);
                if(it2 == newasm::variables::ids.end())
                {
                    //std::cout << "cant find var: `" << suf << "`" << std::endl;
                    newasm::terminate(newasm::exit_codes::sysreq_fail);
                    return 1;
                }

                real_type = it2->second.type;

                switch(type)
                {
                    case newasm::core::lang_inf::typenames::proc__:
                    {
                        if(real_type != newasm::datatypes::proc)
                        {
                            newasm::terminate(newasm::exit_codes::sysreq_fail);
                            return 1;
                        }
                        return 1;
                    }
                    case newasm::core::lang_inf::typenames::num:
                    {
                        if(real_type != newasm::datatypes::number)
                        {
                            newasm::terminate(newasm::exit_codes::sysreq_fail);
                            return 1;
                        }
                        return 1;
                    }
                    case newasm::core::lang_inf::typenames::decm:
                    {
                        if(real_type != newasm::datatypes::decimal)
                        {
                            newasm::terminate(newasm::exit_codes::sysreq_fail);
                            return 1;
                        }
                        return 1;
                    }
                    case newasm::core::lang_inf::typenames::char__:
                    {
                        if(real_type != newasm::datatypes::character)
                        {
                            newasm::terminate(newasm::exit_codes::sysreq_fail);
                            return 1;
                        }
                        return 1;
                    }
                    case newasm::core::lang_inf::typenames::txt:
                    {
                        if(real_type != newasm::datatypes::text)
                        {
                            newasm::terminate(newasm::exit_codes::sysreq_fail);
                            return 1;
                        }
                        return 1;
                    }
                    case newasm::core::lang_inf::typenames::union__:
                    {
                        if(real_type != newasm::datatypes::yunion)
                        {
                            newasm::terminate(newasm::exit_codes::sysreq_fail);
                            return 1;
                        }
                        return 1;
                    }
                    case newasm::core::lang_inf::typenames::class__:
                    {
                        if(real_type != newasm::datatypes::blueprint)
                        {
                            newasm::terminate(newasm::exit_codes::sysreq_fail);
                            return 1;
                        }
                        return 1;
                    }
                    case newasm::core::lang_inf::typenames::tuple:
                    {
                        if(real_type != newasm::datatypes::tuple)
                        {
                            newasm::terminate(newasm::exit_codes::sysreq_fail);
                            return 1;
                        }
                        return 1;
                    }
                    case newasm::core::lang_inf::typenames::context__:
                    {
                        if(real_type != newasm::datatypes::mycontext)
                        {
                            newasm::terminate(newasm::exit_codes::sysreq_fail);
                            return 1;
                        }
                        return 1;
                    }
                    case newasm::core::lang_inf::typenames::obj:
                    {
                        if(real_type != newasm::datatypes::static_objz)
                        {
                            newasm::terminate(newasm::exit_codes::sysreq_fail);
                            return 1;
                        }
                        return 1;
                    }
                }

                #if 0
                if(type != it2->second.type)
                {
                    //std::cout << "found but invalid type: `" << suf << "`" << std::endl;
                    newasm::terminate(newasm::exit_codes::sysreq_fail);
                    return 1;
                }
                #endif

                newasm::terminate(newasm::exit_codes::invalid_syntax);
                return 1;
            }
            //thread
            case newasm::core::lang_inf::thread__:
            {
                if(lineInfo.caseLineArgType != NewASM::datatypes::tokenOpenBrace)//if(newasm::header::data::case_line != OPEN_BRACE_STR)
                {
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }

                if(newasm::nms::count != 0)
                {
                    suf = newasm::header::functions::mangleName(newasm::nms::stack, suf);
                }

                newasm::variables::ids[suf].type = newasm::datatypes::threadz;
                auto& mmap = newasm::variables::ids.at(suf);
                mmap.thrd = new newasm::variables::threadData;
                mmap.thrd->addr = newasm::RAM->write<int>(1);
                newasm::threads::thread_now = true;
                newasm::threads::thread_decl = suf;
                newasm::threads::memory[suf] = new newasm::threads::object__();
                newasm::threads::valid_threads.push_back(suf);
                auto& mmap2 = newasm::threads::memory.at(suf);
                mmap2->paused = false;
                mmap2->id = newasm::kernel::ThreadCount;
                mmap2->LCX = newasm::mem::regs::lcx.get_value();
                newasm::kernel::ThreadCount++;
                newasm::threads::sys_module[mmap2->id] = 0;
                newasm::mem::regs::resetRegisters(mmap2->id);

                newasm::LambdaDispatch::LambdaNow.setThreadValue(mmap2->id, false);
                newasm::LambdaDispatch::LambdaHalt.setThreadValue(mmap2->id, false);
                newasm::LambdaDispatch::LambdaLine.setThreadValue(mmap2->id, false);
                newasm::_this.setThreadValue(mmap2->id, nullptr);

                newasm::brace_stack__.push_back(newasm::brace_stack::thread_block);
                
                //newasm::threads::thread_count++;
                //newasm::header::data::case_line.clear();
                return 1;
            }
            case newasm::core::lang_inf::recv:
            {
                if(!newasm::thread_line)
                {
                    newasm::terminate(newasm::exit_codes::invalid_exp);
                    return 1;
                }
                if(!newasm::header::functions::isref(suf))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }

                newasm::runtime::functions::parse(suf);
                suf = newasm::header::functions::remamp(suf);

                if(newasm::containers::thread_channels.find(suf) == newasm::containers::thread_channels.end())
                {
                    newasm::terminate(newasm::exit_codes::invalid_memacc);
                    return 1;
                }

                auto channel = newasm::containers::thread_channels.find(suf);

                //std::cout << "recv called in " << newasm::threads::now << std::endl;

                if(channel->second->empty)
                {
                    //std::cout << "CHANNEL " << suf << " IS EMPTY!" << std::endl;
                    newasm::threads::memory.at(newasm::threads::now)->paused = true;
                    return 1;
                }

                channel->second->empty = true;
                newasm::threads::memory.at(newasm::threads::now)->paused = false;
                newasm::mem::regs::tlr.set_value(channel->second->data);
                //std::cout << "channel->second->data is `" << channel->second->data << "`" << std::endl;
                return 1;
            }
            //retf
            case newasm::core::lang_inf::retf:
            {
                if(!newasm::thread_line)
                {
                    //std::cout << "THIS IS FUCKD UP!!" << std::endl;
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }
                std::string retf__v = <:suf:>() -> std::string {
                    std::string result = suf;
                    newasm::runtime::functions::parse(result);
                    return result;
                }();

                auto& mmap = newasm::threads::memory.at(newasm::threads::now);
                mmap->returned = true;
                mmap->returned_val = retf__v;
                if(newasm::_this != nullptr)
                {
                    (*newasm::_this)->fetched = false;
                    newasm::_this = nullptr;
                }
            
                return 1;
            }
            //async
            case newasm::core::lang_inf::async__:
            {
                if(newasm::header::data::proc_now)
                {
                    newasm::terminate(newasm::exit_codes::jit_fail);
                    return 1;
                }

                if(!newasm::header::functions::isref(suf))
                {
                    //std::cout << "suf - `" << suf << '`' << std::endl;
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }

                suf = newasm::header::functions::remamp(suf);
                newasm::runtime::functions::parse<true>(suf);

                if(!newasm::mem::functions::datavalid(
                    suf, newasm::variables::ids))
                {
                    newasm::terminate(newasm::exit_codes::invalid_proc);
                    return 1;
                }

                if(newasm::mem::functions::datavalid(suf, newasm::variables::ids))
                {
                    if(newasm::variables::ids.at(suf).type != newasm::datatypes::proc)
                    {
                        newasm::terminate(newasm::exit_codes::invalid_proc);
                        return 1;
                    }
                }

                newasm::async(suf);
                return 1;
            }
            //await
            case newasm::core::lang_inf::await__:
            {
                if(newasm::header::data::proc_now)
                {
                    newasm::terminate(newasm::exit_codes::jit_fail);
                    return 1;
                }
                
                if(!newasm::header::functions::isref(suf))
                {
                    //std::cout << "suf - `" << suf << '`' << std::endl;
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }
                #if 0
                if(!newasm::mem::functions::datavalid(
                    newasm::header::functions::remamp(suf), newasm::threads::memory))
                {
                    newasm::terminate(newasm::exit_codes::invalid_thread);
                    return 1;
                }
                #endif
                std::string thread__ = newasm::header::functions::remamp(suf);
                newasm::runtime::functions::parse<true>(thread__);
                auto it = newasm::threads::memory.find(thread__);
                if(it == newasm::threads::memory.end())
                {
                    newasm::terminate(newasm::exit_codes::invalid_thread);
                    return 1;
                }
                //if(0) newasm::threads::memory.at(thread__)->prepare_sys();
                try
                {
                    while(true)
                    {
                        #if 0
                        newasm::thread_line = true;
                        newasm::threads::now = (thread__);
                        newasm::procline(*newasm::threads::memory.at(thread__)->contents.begin());
                        newasm::thread_line = false;
                        if(newasm::threads::memory.at(thread__)->paused)
                        {
                            newasm::terminate(newasm::exit_codes::channel_deadlock);
                            return 1;
                        }
                        newasm::threads::memory.at(thread__)->contents.pop_front();
                        #endif
                        auto& mmap = it->second;
                        if(mmap->returned)
                        {
                            newasm::_this.setThreadValue(mmap->id, nullptr);
                            break;
                        }
                        if(mmap->contents.empty())
                        {
                            mmap->returned = true;
                            mmap->returned_val = "0";
                            continue;
                        }
                        auto& IDX = mmap->lcx;
                        newasm::thread_line = true;
                        newasm::threads::now = (thread__); // thread name
                        newasm::threads::id_now = mmap->id;
                        if(IDX < 0 or IDX >= mmap->contents.size())
                        {
                            mmap->returned = true;
                            mmap->returned_val = "0";
                            auto ptr = newasm::_this.getThreadValue(mmap->id);
                            if(ptr != nullptr)
                            {
                                ptr->fetched = false;
                                newasm::_this.setThreadValue(mmap->id, nullptr);
                            }
                        }
                        else newasm::procline(mmap->contents.at(IDX));
                        IDX++;
                        newasm::thread_line = false;
                        if(mmap->paused)
                        {
                            IDX--;
                            //std::cout << "Thread paused by channel: " << newasm::threads::now << std::endl;
                            mmap->paused = false;
                            newasm::terminate(newasm::exit_codes::channel_deadlock);
                            return 1;
                        }
                    }
                }
                catch(const std::exception& e)
                {
                    std::cerr << "Zajebucnuo si se thred->lmao :: " << thread__ << " -----> " << e.what() << '\n';
                    std::cout << "last line --> " << newasm::real_line << std::endl;
                }
                
                return 1;
            }
            //int
            case newasm::core::lang_inf::int__:
            {
                int SystemInterrupt = -1;
                //in newasm runtime there is 3 levels of fetching
                //1) firsly, we check if there is a rvalue inside the binary
                if(lineInfo.priArgType == newasm::datatypes::number)
                {
                    //this is the fastest check,
                    //we directly read from the binary
                    SystemInterrupt = lineInfo.priInt;
                }
                //2) we check if there is a variable and get its value
                else if(lineInfo.priArgType == newasm::datatypes::symbol_name) if(
                    newasm::runtime::functions::eval<true>(suf, lineInfo.priEvalMode, &priArg);
                    priArg.rawType == newasm::datatypes::number
                )
                {
                    //if there is a var, we take its value
                    SystemInterrupt = priArg.rawInt;
                }
                //3) if we by some miracle, have no more options,
                // we directly read the string from the raw segment of the binary
                else
                {
                    if(newasm::header::functions::isnumeric(suf))
                    {
                        //slowest, VERY slow
                        SystemInterrupt = std::stoi(suf);
                    }
                }

                //now if there is no hope..
                //crash
                if(SystemInterrupt == -1)
                {
                    newasm::terminate(newasm::exit_codes::invalid_sysint);
                    return 1;
                }

                switch(SystemInterrupt)
                {
                    case 3:
                    {
                        newasm::header::flags::autobos = !newasm::header::flags::autobos;
                        return 1;
                    }
                    case 4:
                    {
                        newasm::header::data::offlineMode = !newasm::header::data::offlineMode;
                        return 1;
                    }
                    case 5:
                    {
                        NewASM::header::data::EnableThreads = !NewASM::header::data::EnableThreads;
                        return 1;
                    }
                }
                newasm::terminate(newasm::exit_codes::invalid_sysint);
                return 1;
            }
            //sysenter
            case newasm::core::lang_inf::sysenter:
            {
                #if 0
                if(!newasm::header::functions::istext(suf))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }
                suf = newasm::header::functions::remq(suf);

                auto kernel_module = newasm::inverted_kernel.find(suf);
                if(kernel_module == newasm::inverted_kernel.end())
                {
                    newasm::terminate(newasm::exit_codes::sysenter_fail);
                    return 1;
                }
                #endif
                if(lineInfo.krnlMod == INVALID_INS)
                {
                    newasm::terminate(newasm::exit_codes::sysenter_fail);
                    return 1;
                }

                switch(lineInfo.krnlMod)//switch(kernel_module->second)
                {
                    case newasm::core::lang_inf::refs::ios:
                    {
                        if(!newasm::kernel::cfg::IOStream)
                        {
                            newasm::terminate(newasm::exit_codes::sysenter_fail);
                            return 1;
                        }
                        if(newasm::thread_line)
                        {
                            newasm::threads::sys_module.at(newasm::threads::id_now) = newasm::core::lang_inf::refs::ios;
                            return 1;
                        }
                        newasm::header::data::module = newasm::core::lang_inf::refs::ios;
                        return 1;
                    }
                    case newasm::core::lang_inf::refs::cfg:
                    {
                        if(!newasm::kernel::cfg::Configuration)
                        {
                            newasm::terminate(newasm::exit_codes::sysenter_fail);
                            return 1;
                        }
                        if(newasm::thread_line)
                        {
                            newasm::threads::sys_module.at(newasm::threads::id_now) = newasm::core::lang_inf::refs::cfg;
                            return 1;
                        }
                        newasm::header::data::module = newasm::core::lang_inf::refs::cfg;
                        return 1;
                    }
                    case newasm::core::lang_inf::refs::fs:
                    {
                        if(!newasm::kernel::cfg::FileStream)
                        {
                            newasm::terminate(newasm::exit_codes::sysenter_fail);
                            return 1;
                        }
                        if(newasm::thread_line)
                        {
                            newasm::threads::sys_module.at(newasm::threads::id_now) = newasm::core::lang_inf::refs::fs;
                            return 1;
                        }
                        newasm::header::data::module = newasm::core::lang_inf::refs::fs;
                        return 1;
                    }
                    case newasm::core::lang_inf::refs::ext:
                    {
                        if(!newasm::kernel::cfg::Extensions)
                        {
                            newasm::terminate(newasm::exit_codes::sysenter_fail);
                            return 1;
                        }
                        if(newasm::thread_line)
                        {
                            newasm::threads::sys_module.at(newasm::threads::id_now) = newasm::core::lang_inf::refs::ext;
                            return 1;
                        }
                        newasm::header::data::module = newasm::core::lang_inf::refs::ext;
                        return 1;
                    }
                    case newasm::core::lang_inf::refs::cmanip:
                    {
                        if(!newasm::kernel::cfg::ContainerManipulation)
                        {
                            newasm::terminate(newasm::exit_codes::sysenter_fail);
                            return 1;
                        }
                        if(newasm::thread_line)
                        {
                            newasm::threads::sys_module.at(newasm::threads::id_now) = newasm::core::lang_inf::refs::cmanip;
                            return 1;
                        }
                        newasm::header::data::module = newasm::core::lang_inf::refs::cmanip;
                        return 1;
                    }
                    case newasm::core::lang_inf::refs::txtop:
                    {
                        if(!newasm::kernel::cfg::TextOperations)
                        {
                            newasm::terminate(newasm::exit_codes::sysenter_fail);
                            return 1;
                        }
                        if(newasm::thread_line)
                        {
                            newasm::threads::sys_module.at(newasm::threads::id_now) = newasm::core::lang_inf::refs::txtop;
                            return 1;
                        }
                        newasm::header::data::module = newasm::core::lang_inf::refs::txtop;
                        return 1;
                    }
                    case newasm::core::lang_inf::refs::net:
                    {
                        if(!newasm::kernel::cfg::Network)
                        {
                            newasm::terminate(newasm::exit_codes::sysenter_fail);
                            return 1;
                        }
                        if(newasm::thread_line)
                        {
                            newasm::threads::sys_module.at(newasm::threads::id_now) = newasm::core::lang_inf::refs::net;
                            return 1;
                        }
                        newasm::header::data::module = newasm::core::lang_inf::refs::net;
                        return 1;
                    }
                    case newasm::core::lang_inf::refs::mem:
                    {
                        if(!newasm::kernel::cfg::Memory)
                        {
                            newasm::terminate(newasm::exit_codes::sysenter_fail);
                            return 1;
                        }
                        if(newasm::thread_line)
                        {
                            newasm::threads::sys_module.at(newasm::threads::id_now) = newasm::core::lang_inf::refs::mem;
                            return 1;
                        }
                        newasm::header::data::module = newasm::core::lang_inf::refs::mem;
                        return 1;
                    }
                    case newasm::core::lang_inf::refs::chrono:
                    {
                        if(!newasm::kernel::cfg::Chrono)
                        {
                            newasm::terminate(newasm::exit_codes::sysenter_fail);
                            return 1;
                        }
                        if(newasm::thread_line)
                        {
                            newasm::threads::sys_module.at(newasm::threads::id_now) = newasm::core::lang_inf::refs::chrono;
                            return 1;
                        }
                        newasm::header::data::module = newasm::core::lang_inf::refs::chrono;
                        return 1;
                    }
                    case newasm::core::lang_inf::refs::thread:
                    {
                        if(!newasm::kernel::cfg::Thread)
                        {
                            newasm::terminate(newasm::exit_codes::sysenter_fail);
                            return 1;
                        }
                        if(newasm::thread_line)
                        {
                            newasm::threads::sys_module.at(newasm::threads::id_now) = newasm::core::lang_inf::refs::thread;
                            return 1;
                        }
                        newasm::header::data::module = newasm::core::lang_inf::refs::thread;
                        return 1;
                    }
                    case newasm::core::lang_inf::refs::tuple:
                    {
                        if(!newasm::kernel::cfg::Tuple)
                        {
                            newasm::terminate(newasm::exit_codes::sysenter_fail);
                            return 1;
                        }
                        if(newasm::thread_line)
                        {
                            newasm::threads::sys_module.at(newasm::threads::id_now) = newasm::core::lang_inf::refs::tuple;
                            return 1;
                        }
                        newasm::header::data::module = newasm::core::lang_inf::refs::tuple;
                        return 1;
                    }
                    case newasm::core::lang_inf::refs::tcp:
                    {
                        if(!newasm::kernel::cfg::TCProtocol)
                        {
                            newasm::terminate(newasm::exit_codes::sysenter_fail);
                            return 1;
                        }
                        if(newasm::thread_line)
                        {
                            newasm::threads::sys_module.at(newasm::threads::id_now) = newasm::core::lang_inf::refs::tcp;
                            return 1;
                        }
                        newasm::header::data::module = newasm::core::lang_inf::refs::tcp;
                        return 1;
                    }
                    case newasm::core::lang_inf::refs::http:
                    {
                        if(!newasm::kernel::cfg::HTTP)
                        {
                            newasm::terminate(newasm::exit_codes::sysenter_fail);
                            return 1;
                        }
                        if(newasm::thread_line)
                        {
                            newasm::threads::sys_module.at(newasm::threads::id_now) = newasm::core::lang_inf::refs::http;
                            return 1;
                        }
                        newasm::header::data::module = newasm::core::lang_inf::refs::http;
                        return 1;
                    }
                    case newasm::core::lang_inf::refs::math:
                    {
                        if(!newasm::kernel::cfg::Math)
                        {
                            newasm::terminate(newasm::exit_codes::sysenter_fail);
                            return 1;
                        }
                        if(newasm::thread_line)
                        {
                            newasm::threads::sys_module.at(newasm::threads::id_now) = newasm::core::lang_inf::refs::math;
                            return 1;
                        }
                        newasm::header::data::module = newasm::core::lang_inf::refs::math;
                        return 1;
                    }
                    case newasm::core::lang_inf::refs::misc:
                    {
                        if(!newasm::kernel::cfg::Misc)
                        {
                            newasm::terminate(newasm::exit_codes::sysenter_fail);
                            return 1;
                        }
                        if(newasm::thread_line)
                        {
                            newasm::threads::sys_module.at(newasm::threads::id_now) = newasm::core::lang_inf::refs::misc;
                            return 1;
                        }
                        newasm::header::data::module = newasm::core::lang_inf::refs::misc;
                        return 1;
                    }
                    case newasm::core::lang_inf::refs::crypto:
                    {
                        if(!newasm::kernel::cfg::Crypto)
                        {
                            newasm::terminate(newasm::exit_codes::sysenter_fail);
                            return 1;
                        }
                        if(newasm::thread_line)
                        {
                            newasm::threads::sys_module.at(newasm::threads::id_now) = newasm::core::lang_inf::refs::crypto;
                            return 1;
                        }
                        newasm::header::data::module = newasm::core::lang_inf::refs::crypto;
                        return 1;
                    }
                    case newasm::core::lang_inf::refs::ctx:
                    {
                        if(!newasm::kernel::cfg::Context)
                        {
                            newasm::terminate(newasm::exit_codes::sysenter_fail);
                            return 1;
                        }
                        if(newasm::thread_line)
                        {
                            newasm::threads::sys_module.at(newasm::threads::id_now) = newasm::core::lang_inf::refs::ctx;
                            return 1;
                        }
                        newasm::header::data::module = newasm::core::lang_inf::refs::ctx;
                        return 1;
                    }
                    case newasm::core::lang_inf::refs::fs_vdsk:
                    {
                        if(!newasm::kernel::cfg::FileStream_VDSK)
                        {
                            newasm::terminate(newasm::exit_codes::sysenter_fail);
                            return 1;
                        }
                        if(newasm::thread_line)
                        {
                            newasm::threads::sys_module.at(newasm::threads::id_now) = newasm::core::lang_inf::refs::fs_vdsk;
                            return 1;
                        }
                        newasm::header::data::module = newasm::core::lang_inf::refs::fs_vdsk;
                        return 1;
                    }
                    default:
                    {
                        newasm::terminate(newasm::exit_codes::os_error);
                        return 1;
                    }
                }
                return 1;
            }
            //align
            case newasm::core::lang_inf::align:
            {
                int alignment = 0;
                if(lineInfo.priArgType == newasm::datatypes::number)
                {
                    alignment = lineInfo.priInt;
                }
                if(lineInfo.priArgType == newasm::datatypes::symbol_name)
                {
                    if(!newasm::header::functions::isnumeric(suf))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    alignment = std::stoi(suf);
                }

                if(alignment <= 0)
                {
                    newasm::terminate(newasm::exit_codes::seg_fault);
                    return 1;
                }

                newasm::header::data::alignment = alignment;
                return 1;
            }
            //wait
            case newasm::core::lang_inf::wait:
            {
                int time = -1;
                if(lineInfo.priArgType == newasm::datatypes::number)
                {
                    time = lineInfo.priInt;
                }
                else if(lineInfo.priArgType == newasm::datatypes::symbol_name)
                {
                    newasm::runtime::functions::eval<true>(suf, lineInfo.priEvalMode, &priArg);
                    if(priArg.rawType == newasm::datatypes::number)
                    {
                        time = priArg.rawInt;
                    }
                }

                if(time == -1)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }

                auto k = newasm::header::functions::wait(time);

                if(k != -1)
                {
                    newasm::wasted_deduction.push_back(std::chrono::duration<double, std::milli>(time));
                }

                if(k == -1)
                {
                    newasm::SetExceptionComment("`wait` handler returned -1");
                    newasm::terminate(newasm::exit_codes::os_error);
                }
                return 1;
            }
            //sel
            case newasm::core::lang_inf::sel:
            {
                int address = -1;
                if(lineInfo.priArgType == newasm::datatypes::number)
                {
                    address = lineInfo.priInt;
                }
                else if(lineInfo.priArgType == newasm::datatypes::symbol_name)
                {
                    newasm::runtime::functions::eval<true>(suf, lineInfo.priEvalMode, &priArg);
                    if(priArg.rawType == newasm::datatypes::number)
                    {
                        address = priArg.rawInt;
                    }
                }

                if(address == -1)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }

                if(std::find(newasm::malloc::meta.begin(), newasm::malloc::meta.end(), address) == newasm::malloc::meta.end())
                {
                    newasm::terminate(newasm::exit_codes::seg_fault);
                    return 1;
                }

                newasm::header::data::malloc_block_used = address;
                return 1;
            }
            case newasm::core::lang_inf::resb__:
            {
                //std::cout << "RESB POZVAN -> " << lineInfo.raw << std::endl;
                if(!newasm::thread_line)
                {
                    newasm::SetExceptionComment("`resb` can be used only within threads");
                    newasm::terminate(newasm::exit_codes::invalid_memacc);
                    return 1;
                }

                int bytes = -1;
                auto& DedicatedStack = *newasm::DedicatedMemory;
                if(lineInfo.priArgType == newasm::datatypes::NIL)
                {
                    if(DedicatedStack.Usable)
                    {
                        newasm::RAM->freeStack(DedicatedStack.addr);
                    }
                    DedicatedStack.Usable = false;
                    return 1;
                }
                else if(lineInfo.priArgType == newasm::datatypes::number)
                {
                    bytes = lineInfo.priInt;
                }
                else if(lineInfo.priArgType == newasm::datatypes::symbol_name)
                {
                    newasm::runtime::functions::eval<true>(suf, lineInfo.priEvalMode, &priArg);
                    if(priArg.rawType == newasm::datatypes::number)
                    {
                        bytes = priArg.rawInt;
                    }
                }

                if(bytes == -1)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }

                if(DedicatedStack.Usable) //prevent memory leaks
                {
                    newasm::RAM->freeStack(DedicatedStack.addr);
                }
                DedicatedStack.Usable = true;
                DedicatedStack.addr = newasm::RAM->dedicateStack(bytes);
                if(DedicatedStack.addr == -1)
                {
                    newasm::SetExceptionComment("no enough memory or invalid size");
                    newasm::terminate(newasm::exit_codes::seg_fault);
                    DedicatedStack.Usable = false;
                    return 1;
                }
                DedicatedStack.stackEnd = newasm::RAM->getDedicatedStackEnd(DedicatedStack.addr);
                DedicatedStack.StackInfo.clear();

                //context switch is on, so the stk is gonna change for the thread itself
                newasm::mem::regs::stk.set_value(newasm::RAM->getDedicatedStackEnd(DedicatedStack.addr));
                return 1;
            }
            //malloc
            case newasm::core::lang_inf::malloc__:
            {
                if(lineInfo.VirtualMemoryAccess)
                {
                    newasm::_virtual::virtualMemory.init(lineInfo.priInt);
                    return 1;
                }

                int bytes = -1;
                if(lineInfo.priArgType == newasm::datatypes::number)
                {
                    bytes = lineInfo.priInt;
                }
                else if(lineInfo.priArgType == newasm::datatypes::symbol_name)
                {
                    newasm::runtime::functions::eval<true>(suf, lineInfo.priEvalMode, &priArg);
                    if(priArg.rawType == newasm::datatypes::number)
                    {
                        bytes = priArg.rawInt;
                    }
                }

                if(bytes == -1)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }

                auto i = newasm::hardware::randAccessMem.malloc(bytes);
                if(i == -1)
                {
                    newasm::SetExceptionComment("no enough memory or invalid size");
                    newasm::terminate(newasm::exit_codes::malloc_err);
                    return 1;
                }
                newasm::malloc::meta.push_back(i);
                newasm::mem::regs::rax.set_value(i);

                newasm::header::data::malloc_block_used = newasm::malloc::meta.back();
                
                #if 0
                if(newasm::allocation_data != nullptr) // malloc je vec upotrebljen //NoAlloc
                {
                    newasm::terminate(newasm::exit_codes::malloc_err);
                    return 1;
                }
                if(!newasm::header::functions::isnumeric(suf))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }
                newasm::allocation_data = new newasm::mem::malloc_info();
                newasm::allocation_data->size = std::stoi(suf);
                newasm::mem::regs::hea += std::stoi(suf);
                newasm::allocation_data->heapsize_new = newasm::mem::regs::hea;
                // medjutim nece heap pointer biti promienjen, to cemo ostaviti za mov i free
                #endif

                return 1;
            }
            //call
            case newasm::core::lang_inf::call:
            {
                if(newasm::header::data::proc_now)
                {
                    newasm::SetExceptionComment("cannot call a procedure within procedure, use `callc`");
                    newasm::terminate(newasm::exit_codes::inline_proc);
                    return 1;
                }

                if(lineInfo.priArgType == newasm::datatypes::ThisPtr)
                {
                    if(newasm::_this == nullptr) [[unlikely]]
                    {
                        newasm::SetExceptionComment("`this` is probably not initialized");
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    if((*newasm::_this)->type != newasm::datatypes::proc) [[unlikely]]
                    {
                        newasm::SetExceptionComment("object is not a procedure");
                        newasm::terminate(newasm::exit_codes::seg_fault);
                        return 1;
                    }
                    newasm::callproc(newasm::_this);
                    return 1;
                }

                newasm::runtime::functions::parse<true>(suf);

                //std::cout << "Called a procedure -> " << suf << std::endl;

                if(newasm::header::functions::isalphanum(suf))
                {
                    auto p = newasm::mem::functions::datavalid(suf, newasm::variables::ids);
                    if(!p) [[unlikely]]
                    {
                        newasm::SetExceptionComment("procedure not found in object map");
                        newasm::terminate(newasm::exit_codes::invalid_proc);
                        return 1;
                    }

                    if(p)
                    {
                        if(newasm::variables::ids.at(suf).type != newasm::datatypes::proc) [[unlikely]]
                        {
                            newasm::SetExceptionComment("object is not a procedure");
                            newasm::terminate(newasm::exit_codes::invalid_proc);
                            return 1;
                        }
                    }

                    newasm::callproc(suf);
                    return 1;
                }

                newasm::SetExceptionComment("case not matched");
                newasm::terminate(newasm::exit_codes::invalid_syntax);
                return 1;
            }
            //free
            case newasm::core::lang_inf::free__:
            {
                if(lineInfo.priArgType == NewASM::datatypes::NIL)
                {
                    if(newasm::malloc::meta.size() == 0)
                    {
                        newasm::terminate(newasm::exit_codes::seg_fault);
                        return 1;
                    }

                    int addr = newasm::malloc::meta.back();
                    newasm::malloc::meta.pop_back();

                    newasm::hardware::randAccessMem.free(addr);
                    return 1;
                }

                int heapaddr;
                if(lineInfo.priArgType == newasm::datatypes::number)
                {
                    heapaddr = lineInfo.priInt;
                }
                else if(
                    newasm::runtime::functions::eval<true>(suf, lineInfo.priEvalMode, &priArg);
                    priArg.rawType == newasm::datatypes::number
                )
                {
                    heapaddr = priArg.rawInt;
                }
                else if(newasm::header::functions::isnumeric(suf))
                {
                    heapaddr = std::stoi(suf);
                }
                else
                {
                    newasm::terminate(newasm::exit_codes::invalid_alloc);
                    return 1;
                }

                auto& v = newasm::malloc::meta;
                auto element = std::find(v.begin(), v.end(), heapaddr);
                if(element == v.end())
                {
                    newasm::terminate(newasm::exit_codes::invalid_alloc);
                    return 1;
                }

                newasm::hardware::randAccessMem.free(*element);

                v.erase(element);
                return 1;
            }
            //proc
            case newasm::core::lang_inf::proc:
            {
                if(newasm::header::data::repl)
                {
                    newasm::unsins(ins); // incompatible instruction for repl
                    return 1;
                }
                if(newasm::thread_line)
                {
                    newasm::SetExceptionComment("cannot create a procedure within a thread, use `.., (proc)`");
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }

                newasm::system::mangled_proc = false;

                if(newasm::nms::count != 0)
                {
                    newasm::system::original_proc = newasm::header::functions::demangleName(newasm::nms::stack, suf);
                    suf = newasm::header::functions::mangleName(newasm::nms::stack, suf);
                    newasm::system::mangled_proc = true;
                }
                else
                {
                    newasm::system::original_proc = suf;
                }

                if(newasm::header::functions::isalphanum(suf))
                {
                    newasm::system::stop = 1;
                    newasm::system::cproc = suf;
                    newasm::system::proclines = 0;
                    //std::cout << "Creating proc: " << opr << std::endl;
                    newasm::variables::ids[newasm::system::cproc].proc = new newasm::variables::procedureData;
                    auto& mmap = newasm::variables::ids.at(newasm::system::cproc);
                    mmap.type = newasm::datatypes::proc;
                    mmap.proc->LCX = newasm::mem::regs::lcx.get_value();
                    NewASM::CurrentProc = &mmap;
                    return 1;
                }

                newasm::SetExceptionComment("procedure name is probably not alphanumeric");
                newasm::terminate(newasm::exit_codes::os_error);
                return 1;
            }
            //heap
            case newasm::core::lang_inf::heap:
            {
                int heapaddr;
                if(lineInfo.priArgType == newasm::datatypes::number)
                {
                    heapaddr = lineInfo.priInt;
                }
                else if(
                    newasm::runtime::functions::eval<true>(suf, lineInfo.priEvalMode, &priArg);
                    priArg.rawType == newasm::datatypes::number
                )
                {
                    heapaddr = priArg.rawInt;
                }
                else if(newasm::header::functions::isnumeric(suf))
                {
                    heapaddr = std::stoi(suf);
                }
                else
                {
                    newasm::SetExceptionComment("couln't fetch the value of the first operand");
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }

                newasm::mem::regs::hea = newasm::mem::regs::hea + heapaddr;
                if(newasm::mem::regs::hea > 10 * 1024 * 1024)
                {
                    newasm::terminate(newasm::exit_codes::mem_overflow);//,wholeline);
                    return 1;
                }
                if(newasm::mem::regs::hea < 0)
                {
                    newasm::terminate(newasm::exit_codes::mem_underflow);//,wholeline);
                    return 1;
                }
                if(newasm::mem::functions::check_stkhea_col())
                {
                    newasm::terminate(newasm::exit_codes::stkhea_col);//,wholeline);
                    return 1;
                }
                return 1;
            }
            //db - debug
            case newasm::core::lang_inf::db:
            {
                auto debugRegister = [](const std::string& str1, const std::string& str2)
                {
                    newasm::progwin::api::cout(str1 +
                        " = `"_str + 
                        str2 + 
                        "`"_str
                    );
                    //newasm::progwin::api::flush();
                };
                switch(lineInfo.whatAreRegistersLol)
                {
                    case newasm::mem::regs::fdx__://if(suf == newasm::mem::regs::fdx.identifier())
                    {
                        debugRegister(suf, newasm::_std::to_string(newasm::mem::regs::fdx));
                        return 1;
                    }
                    case newasm::mem::regs::bos__://if(suf == newasm::mem::regs::bos.identifier())
                    {
                        debugRegister(suf, newasm::_std::to_string(newasm::mem::regs::bos));
                        return 1;
                    }
                    case newasm::mem::regs::tlr__://if(suf == newasm::mem::regs::tlr.identifier())
                    {
                        debugRegister(suf, (newasm::mem::regs::tlr));
                        return 1;
                    }
                    case newasm::mem::regs::dlx__://if(suf == newasm::mem::regs::dlx.identifier())
                    {
                        debugRegister(suf, (newasm::mem::regs::dlx));
                        return 1;
                    }
                    case newasm::mem::regs::tr0__://if(suf == newasm::mem::regs::tr0.identifier())
                    {
                        debugRegister(suf, (newasm::mem::regs::tr0));
                        return 1;
                    }
                    case newasm::mem::regs::tr1__://if(suf == newasm::mem::regs::tr1.identifier())
                    {
                        debugRegister(suf, (newasm::mem::regs::tr1));
                        return 1;
                    }
                    case newasm::mem::regs::stl__://if(suf == newasm::mem::regs::stl.identifier())
                    {
                        debugRegister(suf, (newasm::mem::regs::stl));
                        return 1;
                    }
                    case newasm::mem::regs::psx__://if(suf == newasm::mem::regs::psx.identifier())
                    {
                        debugRegister(suf, (newasm::mem::regs::psx));
                        return 1;
                    }
                    case newasm::mem::regs::stk__://if(suf == newasm::mem::regs::stk.identifier())
                    {
                        debugRegister(suf, newasm::_std::to_string(newasm::mem::regs::stk));
                        return 1;
                    }
                    case newasm::mem::regs::hea__://if(suf == newasm::mem::regs::hea.identifier())
                    {
                        debugRegister(suf, newasm::_std::to_string(newasm::mem::regs::hea));
                        return 1;
                    }
                    case newasm::mem::regs::cpt__://if(suf == newasm::mem::regs::cpt.identifier())
                    {
                        debugRegister(suf, (newasm::mem::regs::cpt));
                        return 1;
                    }
                    
                    case newasm::mem::regs::cpr__://if(suf == newasm::mem::regs::cpr.identifier())
                    {
                        debugRegister(suf, newasm::_std::to_string(newasm::mem::regs::cpr));
                        return 1;
                    }
                    case newasm::mem::regs::cr0__://if(suf == newasm::mem::regs::cr0.identifier())
                    {
                        debugRegister(suf, newasm::_std::to_string(newasm::mem::regs::cr0));
                        return 1;
                    }
                    case newasm::mem::regs::cr1__://if(suf == newasm::mem::regs::cr1.identifier())
                    {
                        debugRegister(suf, newasm::_std::to_string(newasm::mem::regs::cr1));
                        return 1;
                    }
                    case newasm::mem::regs::br0__://if(suf == newasm::mem::regs::br0.identifier())
                    {
                        debugRegister(suf, newasm::_std::to_string(newasm::mem::regs::br0));
                        return 1;
                    }
                    case newasm::mem::regs::br1__://if(suf == newasm::mem::regs::br1.identifier())
                    {
                        debugRegister(suf, newasm::_std::to_string(newasm::mem::regs::br1));
                        return 1;
                    }
                    case newasm::mem::regs::rax__://if(suf == newasm::mem::regs::rax.identifier())
                    {
                        debugRegister(suf, newasm::_std::to_string(newasm::mem::regs::rax));
                        return 1;
                    }
                    case newasm::mem::regs::imm__://if(suf == newasm::mem::regs::imm.identifier())
                    {
                        debugRegister(suf, newasm::_std::to_string(newasm::mem::regs::imm));
                        return 1;
                    }
                    case newasm::mem::regs::rbx__://if(suf == newasm::mem::regs::rbx.identifier())
                    {
                        debugRegister(suf, newasm::_std::to_string(newasm::mem::regs::rbx));
                        return 1;
                    }
                    case newasm::mem::regs::cr2__:
                    {
                        debugRegister(suf, newasm::_std::to_string(newasm::mem::regs::cr2));
                        return 1;
                    }
                    case newasm::mem::regs::cr3__:
                    {
                        debugRegister(suf, newasm::_std::to_string(newasm::mem::regs::cr3));
                        return 1;
                    }
                }
                newasm::terminate(newasm::exit_codes::invalid_syntax);
                return 1;
            }
            // ret - classic return
            case newasm::core::lang_inf::ret:
            {
                if(newasm::header::data::repl)
                {
                    newasm::unsins(ins);
                    return 1;
                }
                if(newasm::malloc::meta.size() != 0)
                {
                    newasm::terminate(newasm::exit_codes::memory_leak); // Learn to clean after yourself.
                    return 1;
                }
                newasm::runtime::functions::parse(suf);
                if(newasm::header::functions::isnumeric(suf))
                {
                    newasm::mem::regs::exc = std::stoi(suf);
                    newasm::header::data::exception = false;
                    newasm::terminate(newasm::mem::regs::exc);//,wholeline);
                    return 1;
                }
                newasm::mem::regs::exc = newasm::exit_codes::invalid_retn;
                newasm::terminate(newasm::mem::regs::exc);//,wholeline);
                return 1;
            }
            //retn - return near, regs
            case newasm::core::lang_inf::retn:
            {
                if(newasm::header::data::repl)
                {
                    newasm::unsins(ins);
                    return 1;
                }
                if(newasm::malloc::meta.size() != 0)
                {
                    newasm::terminate(newasm::exit_codes::memory_leak); // Learn to clean after yourself.
                    return 1;
                }

                if(lineInfo.whatAreRegistersLol == INVALID_INS)
                {
                    newasm::terminate(newasm::exit_codes::invalid_retn);
                    return 1;
                }
                
                switch(lineInfo.whatAreRegistersLol)
                {
                    case NewASM::mem::regs::fdx__:
                    {
                        NewASM::header::data::exception = false;
                        NewASM::terminate(NewASM::mem::regs::fdx.get_value());
                        return 1;
                    }
                    case NewASM::mem::regs::bos__:
                    {
                        NewASM::header::data::exception = false;
                        NewASM::terminate(NewASM::mem::regs::bos.get_value());
                        return 1;
                    }
                    case NewASM::mem::regs::tlr__:
                    {
                        NewASM::header::data::exception = false;
                        NewASM::terminate(NewASM::mem::regs::tlr.get_value());
                        return 1;
                    }
                    case NewASM::mem::regs::dlx__:
                    {
                        NewASM::header::data::exception = false;
                        NewASM::terminate(NewASM::mem::regs::dlx.get_value());
                        return 1;
                    }
                    case NewASM::mem::regs::tr0__:
                    {
                        NewASM::header::data::exception = false;
                        NewASM::terminate(NewASM::mem::regs::tr0.get_value());
                        return 1;
                    }
                    case NewASM::mem::regs::tr1__:
                    {
                        NewASM::header::data::exception = false;
                        NewASM::terminate(NewASM::mem::regs::tr1.get_value());
                        return 1;
                    }
                    case NewASM::mem::regs::stl__:
                    {
                        NewASM::header::data::exception = false;
                        NewASM::terminate(NewASM::mem::regs::stl.get_value());
                        return 1;
                    }
                    case NewASM::mem::regs::psx__:
                    {
                        NewASM::header::data::exception = false;
                        NewASM::terminate(NewASM::mem::regs::psx.get_value());
                        return 1;
                    }
                    case NewASM::mem::regs::stk__:
                    {
                        NewASM::header::data::exception = false;
                        NewASM::terminate(NewASM::mem::regs::stk.get_value());
                        return 1;
                    }
                    case NewASM::mem::regs::hea__:
                    {
                        NewASM::header::data::exception = false;
                        NewASM::terminate(NewASM::mem::regs::hea.get_value());
                        return 1;
                    }
                    case NewASM::mem::regs::cpt__:
                    {
                        NewASM::header::data::exception = false;
                        NewASM::terminate(NewASM::mem::regs::cpt.get_value());
                        return 1;
                    }
                    case NewASM::mem::regs::cpr__:
                    {
                        NewASM::header::data::exception = false;
                        NewASM::terminate(NewASM::mem::regs::cpr.get_value());
                        return 1;
                    }
                    case NewASM::mem::regs::cr0__:
                    {
                        NewASM::header::data::exception = false;
                        NewASM::terminate(NewASM::mem::regs::cr0.get_value());
                        return 1;
                    }
                    case NewASM::mem::regs::cr1__:
                    {
                        NewASM::header::data::exception = false;
                        NewASM::terminate(NewASM::mem::regs::cr1.get_value());
                        return 1;
                    }
                    case NewASM::mem::regs::br0__:
                    {
                        NewASM::header::data::exception = false;
                        NewASM::terminate(NewASM::mem::regs::br0.get_value());
                        return 1;
                    }
                    case NewASM::mem::regs::br1__:
                    {
                        NewASM::header::data::exception = false;
                        NewASM::terminate(NewASM::mem::regs::br1.get_value());
                        return 1;
                    }
                    case NewASM::mem::regs::cr2__:
                    {
                        NewASM::header::data::exception = false;
                        NewASM::terminate(NewASM::mem::regs::cr2.get_value());
                        return 1;
                    }
                    case NewASM::mem::regs::cr3__:
                    {
                        NewASM::header::data::exception = false;
                        NewASM::terminate(NewASM::mem::regs::cr3.get_value());
                        return 1;
                    }
                    case NewASM::mem::regs::imm__:
                    {
                        NewASM::header::data::exception = false;
                        NewASM::terminate(NewASM::mem::regs::imm.get_value());
                        return 1;
                    }
                    case NewASM::mem::regs::rax__:
                    {
                        NewASM::header::data::exception = false;
                        NewASM::terminate(NewASM::mem::regs::rax.get_value());
                        return 1;
                    }
                    case NewASM::mem::regs::rbx__:
                    {
                        NewASM::header::data::exception = false;
                        NewASM::terminate(NewASM::mem::regs::rbx.get_value());
                        return 1;
                    }
                    default:
                    {
                        NewASM::terminate(NewASM::exit_codes::os_error);
                        return 1;
                    }
                }

                newasm::terminate(newasm::exit_codes::invalid_retn);//,wholeline);
                return 1;
            }
            
            //zero
            case newasm::core::lang_inf::zero:
            {
                if(lineInfo.whatAreRegistersLol == INVALID_INS)
                {
                    newasm::terminate(newasm::exit_codes::bus_err);
                    return 1;
                }

                switch(lineInfo.whatAreRegistersLol)
                {
                    case newasm::mem::regs::fdx__: newasm::mem::regs::fdx.reset(); break;
                    case newasm::mem::regs::imm__: newasm::mem::regs::imm.reset(); break;
                    case newasm::mem::regs::rax__: newasm::mem::regs::rax.reset(); break;
                    case newasm::mem::regs::rbx__: newasm::mem::regs::rbx.reset(); break;
                    case newasm::mem::regs::bos__: newasm::mem::regs::bos.reset(); break;
                    case newasm::mem::regs::tlr__: newasm::mem::regs::tlr.reset(); break;
                    case newasm::mem::regs::dlx__: newasm::mem::regs::dlx.reset(); break;
                    case newasm::mem::regs::tr0__: newasm::mem::regs::tr0.reset(); break;
                    case newasm::mem::regs::tr1__: newasm::mem::regs::tr1.reset(); break;
                    case newasm::mem::regs::stl__: newasm::mem::regs::stl.reset(); break;
                    case newasm::mem::regs::stk__:
                    {
                        newasm::mem::regs::stk.reset();
                        newasm::terminate(newasm::exit_codes::stkhea_col); // Why'd you touch STK in the first place?
                        return 1;
                    }
                    case newasm::mem::regs::hea__: newasm::mem::regs::hea.reset(); break;
                    case newasm::mem::regs::psx__: newasm::mem::regs::psx.reset(); break;
                    //case newasm::mem::regs::prp__: newasm::mem::regs::prp.reset(); break;
                    case newasm::mem::regs::cpt__: newasm::mem::regs::cpt.reset(); break;
                    case newasm::mem::regs::cpr__: newasm::mem::regs::cpr.reset(); break;
                    case newasm::mem::regs::cr0__: newasm::mem::regs::cr0.reset(); break;
                    case newasm::mem::regs::cr1__: newasm::mem::regs::cr1.reset(); break;
                    case newasm::mem::regs::cr2__: newasm::mem::regs::cr2.reset(); break;
                    case newasm::mem::regs::cr3__: newasm::mem::regs::cr3.reset(); break;
                    case newasm::mem::regs::br0__: newasm::mem::regs::br0.reset(); break;
                    case newasm::mem::regs::br1__: newasm::mem::regs::br1.reset(); break;
                    default:
                    {
                        newasm::terminate(newasm::exit_codes::os_error);
                        return 1;
                    }
                }
                return 1;
            }
            //inc
            case newasm::core::lang_inf::inc:
            {
                if(lineInfo.whatAreRegistersLol == INVALID_INS)
                {
                    //std::cout << "THIS ACTUALLY HAPPENED NIGZ" << std::endl;
                    newasm::terminate(newasm::exit_codes::bus_err);
                    return 1;
                }

                switch(lineInfo.whatAreRegistersLol)
                {
                    case newasm::mem::regs::fdx__:
                    {
                        newasm::mem::regs::fdx ++;
                        return 1;
                    }
                    case newasm::mem::regs::imm__:
                    {
                        newasm::mem::regs::imm ++;
                        return 1;
                    }
                    case newasm::mem::regs::rax__:
                    {
                        ++newasm::mem::regs::rax;
                        return 1;
                    }
                    case newasm::mem::regs::bos__:
                    {
                        newasm::mem::regs::bos ++;
                        return 1;
                    }
                    case newasm::mem::regs::stk__:
                    {
                        newasm::mem::regs::stk ++;
                        return 1;
                    }
                    case newasm::mem::regs::hea__:
                    {
                        newasm::mem::regs::hea ++;
                        return 1;
                    }
                    case newasm::mem::regs::cpr__:
                    {
                        newasm::mem::regs::cpr ++;
                        return 1;
                    }
                    case newasm::mem::regs::cr0__:
                    {
                        newasm::mem::regs::cr0 ++;
                        return 1;
                    }
                    case newasm::mem::regs::cr1__:
                    {
                        newasm::mem::regs::cr1 ++;
                        return 1;
                    }
                    case newasm::mem::regs::rbx__:
                    {
                        ++newasm::mem::regs::rbx;
                        return 1;
                    }
                    case newasm::mem::regs::cr2__:
                    {
                        newasm::mem::regs::cr2 ++;
                        return 1;
                    }
                    case newasm::mem::regs::cr3__:
                    {
                        newasm::mem::regs::cr3 ++;
                        return 1;
                    }
                    case newasm::mem::regs::br0__:
                    {
                        newasm::mem::regs::br0 ++;
                        return 1;
                    }
                    case newasm::mem::regs::br1__:
                    {
                        newasm::mem::regs::br1 ++;
                        return 1;
                    }

                    //typeless registers require a different approach
                    case newasm::mem::regs::tlr__:
                    {
                        if(!newasm::header::functions::isnumeric(newasm::mem::regs::tlr))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                            return 1;
                        }
                        int tmp = std::stoi(newasm::mem::regs::tlr)+1;
                        newasm::mem::regs::tlr = newasm::_std::to_string(tmp);
                        return 1;
                    }
                    case newasm::mem::regs::dlx__:
                    {
                        if(!newasm::header::functions::isnumeric(newasm::mem::regs::dlx))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                            return 1;
                        }
                        int tmp = std::stoi(newasm::mem::regs::dlx)+1;
                        newasm::mem::regs::dlx = newasm::_std::to_string(tmp);
                        return 1;
                    }
                    case newasm::mem::regs::stl__:
                    {
                        if(!newasm::header::functions::isnumeric(newasm::mem::regs::stl))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                            return 1;
                        }
                        int tmp = std::stoi(newasm::mem::regs::stl)+1;
                        newasm::mem::regs::stl = newasm::_std::to_string(tmp);
                        return 1;
                    }
                    case newasm::mem::regs::psx__:
                    {
                        if(!newasm::header::functions::isnumeric(newasm::mem::regs::psx))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                            return 1;
                        }
                        int tmp = std::stoi(newasm::mem::regs::psx)+1;
                        newasm::mem::regs::psx = newasm::_std::to_string(tmp);
                        return 1;
                    }

                    case newasm::mem::regs::tr0__:
                    {
                        if(!newasm::header::functions::isnumeric(newasm::mem::regs::tr0))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                            return 1;
                        }
                        int tmp = std::stoi(newasm::mem::regs::tr0)+1;
                        newasm::mem::regs::tr0 = newasm::_std::to_string(tmp);
                        return 1;
                    }
                    case newasm::mem::regs::tr1__:
                    {
                        if(!newasm::header::functions::isnumeric(newasm::mem::regs::tr1))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                            return 1;
                        }
                        int tmp = std::stoi(newasm::mem::regs::tr1)+1;
                        newasm::mem::regs::tr1 = newasm::_std::to_string(tmp);
                        return 1;
                    }
                    //and then, we have this beautiful procedure pointer...
                    //we'll just pickup the next procedure from the map memory
                    #if 0
                    case newasm::mem::regs::prp__:
                    {
                        auto prp_ = newasm::header::functions::remamp(newasm::mem::regs::prp);
                        auto z = newasm::mem::functions::datavalid(prp_, newasm::variables::ids);
                        if(!z)
                        {
                            newasm::terminate(newasm::exit_codes::invalid_proc);
                            return 1;
                        }
                        if(z)
                        {
                            if(newasm::variables::ids.at(prp_))
                        }
                        bool found = false;
                        for(decltype(newasm::mem::funcs)::iterator i = newasm::mem::funcs.begin(); i != newasm::mem::funcs.end(); ++i)
                        {
                            if(found)
                            {
                                newasm::mem::regs::prp = static_cast<std::string>("&") + i->first;
                                break;
                            }
                            if(i->first == newasm::header::functions::remamp(newasm::mem::regs::prp))
                            {
                                found = true;
                                if(std::next(i) == newasm::mem::funcs.end())
                                {
                                    newasm::terminate(newasm::exit_codes::mem_overflow);
                                    return 1;
                                }
                                continue;
                            }
                        }
                        return 1;
                    }
                    #endif
                    case newasm::mem::regs::cpt__:
                    {
                        //make later,
                        // or rather NEVER!
                        return 1;
                    }
                    default:
                    {
                        newasm::terminate(newasm::exit_codes::os_error);
                        return 1;
                    }
                }
                return 1;
            }
            //dec
            case newasm::core::lang_inf::dec:
            {
                if(lineInfo.whatAreRegistersLol == INVALID_INS)
                {
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }

                switch(lineInfo.whatAreRegistersLol)
                {
                    case newasm::mem::regs::fdx__:
                    {
                        --newasm::mem::regs::fdx;
                        return 1;
                    }
                    case newasm::mem::regs::imm__:
                    {
                        --newasm::mem::regs::imm;
                        return 1;
                    }
                    case newasm::mem::regs::rax__:
                    {
                        --newasm::mem::regs::rax;
                        return 1;
                    }
                    case newasm::mem::regs::rbx__:
                    {
                        --newasm::mem::regs::rbx;
                        return 1;
                    }
                    case newasm::mem::regs::bos__:
                    {
                        --newasm::mem::regs::bos;
                        return 1;
                    }
                    case newasm::mem::regs::stk__:
                    {
                        newasm::mem::regs::stk --;
                        return 1;
                    }
                    case newasm::mem::regs::hea__:
                    {
                        newasm::mem::regs::hea --;
                        return 1;
                    }
                    case newasm::mem::regs::cpr__:
                    {
                        newasm::mem::regs::cpr --;
                        return 1;
                    }
                    case newasm::mem::regs::cr0__:
                    {
                        newasm::mem::regs::cr0 --;
                        return 1;
                    }
                    case newasm::mem::regs::cr1__:
                    {
                        newasm::mem::regs::cr1 --;
                        return 1;
                    }
                    case newasm::mem::regs::cr2__:
                    {
                        newasm::mem::regs::cr2 --;
                        return 1;
                    }
                    case newasm::mem::regs::cr3__:
                    {
                        newasm::mem::regs::cr3 --;
                        return 1;
                    }
                    case newasm::mem::regs::br0__:
                    {
                        newasm::mem::regs::br0 --;
                        return 1;
                    }
                    case newasm::mem::regs::br1__:
                    {
                        newasm::mem::regs::br1 --;
                        return 1;
                    }

                    //typeless registers require a different approach
                    case newasm::mem::regs::tlr__:
                    {
                        if(!newasm::header::functions::isnumeric(newasm::mem::regs::tlr))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                            return 1;
                        }
                        int tmp = std::stoi(newasm::mem::regs::tlr)-1;
                        newasm::mem::regs::tlr = newasm::_std::to_string(tmp);
                        return 1;
                    }
                    case newasm::mem::regs::dlx__:
                    {
                        if(!newasm::header::functions::isnumeric(newasm::mem::regs::dlx))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                            return 1;
                        }
                        int tmp = std::stoi(newasm::mem::regs::dlx)-1;
                        newasm::mem::regs::dlx = newasm::_std::to_string(tmp);
                        return 1;
                    }
                    case newasm::mem::regs::stl__:
                    {
                        if(!newasm::header::functions::isnumeric(newasm::mem::regs::stl))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                            return 1;
                        }
                        int tmp = std::stoi(newasm::mem::regs::stl)-1;
                        newasm::mem::regs::stl = newasm::_std::to_string(tmp);
                        return 1;
                    }
                    case newasm::mem::regs::psx__:
                    {
                        if(!newasm::header::functions::isnumeric(newasm::mem::regs::psx))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                            return 1;
                        }
                        int tmp = std::stoi(newasm::mem::regs::psx)-1;
                        newasm::mem::regs::psx = newasm::_std::to_string(tmp);
                        return 1;
                    }

                    case newasm::mem::regs::tr1__:
                    {
                        if(!newasm::header::functions::isnumeric(newasm::mem::regs::tr1))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                            return 1;
                        }
                        int tmp = std::stoi(newasm::mem::regs::tr1)-1;
                        newasm::mem::regs::tr1 = newasm::_std::to_string(tmp);
                        return 1;
                    }
                    case newasm::mem::regs::tr0__:
                    {
                        if(!newasm::header::functions::isnumeric(newasm::mem::regs::tr0))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                            return 1;
                        }
                        int tmp = std::stoi(newasm::mem::regs::tr0)-1;
                        newasm::mem::regs::tr0 = newasm::_std::to_string(tmp);
                        return 1;
                    }
                    #if 0
                    //and then, we have this beautiful procedure pointer...
                    //we'll just pickup the last procedure from the map memory
                    case newasm::mem::regs::prp__:
                    {
                        if(!newasm::mem::functions::datavalid(newasm::header::functions::remamp(newasm::mem::regs::prp), newasm::mem::funcs))
                        {
                            newasm::terminate(newasm::exit_codes::invalid_proc);
                            return 1;
                        }
                        for(decltype(newasm::mem::funcs)::iterator i = newasm::mem::funcs.begin(); i != newasm::mem::funcs.end(); ++i)
                        {
                            if(std::next(i) == newasm::mem::funcs.end())
                            {
                                newasm::terminate(newasm::exit_codes::mem_underflow);
                                return 1;
                            }
                            if(std::next(i)->first == newasm::header::functions::remamp(newasm::mem::regs::prp))
                            {
                                newasm::mem::regs::prp = static_cast<std::string>("&") + i->first;
                                break;
                            }
                        }
                        return 1;
                    }
                    #endif
                    case newasm::mem::regs::cpt__:
                    {
                        //make later
                        //seems like never to me
                        return 1;
                    }
                    default:
                    {
                        newasm::terminate(newasm::exit_codes::invalid_exp);
                        return 1;
                    }
                }
                return 1;
            }
        }
        return 1;
    }
    FORCE_INLINE inline int process_i(std::string& line, std::string& ins, newasm::compiler::lineData& lineInfo)
    {
        /*auto it = newasm::inverted_ins.find(ins);
        if(it == newasm::inverted_ins.end())
        {
            newasm::terminate(newasm::exit_codes::invalid_ins);
            return 1;
        }*/

        switch(lineInfo.whatAmIDoing)//switch(it->second)
        {
            //exit
            case newasm::core::lang_inf::exit:
            {
                if(!newasm::header::data::repl)
                {
                    newasm::unsins_repl(ins);
                    return 1;
                }
                newasm::header::data::repl_end = true;
                newasm::header::functions::info("REPL mode exit.");
                return 1;
            }
            //end
            case newasm::core::lang_inf::end:
            {
                if(newasm::header::data::repl)
                {
                    newasm::unsins(ins);
                    return 1;
                }
                if(newasm::system::stop == 0)
                {
                    newasm::terminate(newasm::exit_codes::unexpected_end);
                    return 1;
                }
                if(newasm::system::proclines == 0)
                {
                    newasm::terminate(newasm::exit_codes::empty_proc);
                    return 1;
                }

                //newasm::mem::funcs_data[newasm::system::cproc].mangled = newasm::system::mangled_proc;
                //newasm::mem::funcs_data[newasm::system::cproc].original_name = newasm::system::original_proc;

                //auto& a = newasm::variables::ids.at(newasm::system::cproc);
                NewASM::CurrentProc->proc->mangled = newasm::system::mangled_proc;
                NewASM::CurrentProc->proc->original_name = newasm::system::original_proc;
                NewASM::CurrentProc->proc->JIT_compile();

                newasm::system::stop = 0;
                //std::cout << "Finished proc: " << newasm::system::cproc << std::endl;
                return 1;
            }
        }
        if(newasm::system::stop == 1)
        {
            #if 0
            newasm::system::proclines ++;
            std::string newline = ins;
            newasm::mem::funcs[newasm::system::cproc].push_back(lineInfo.raw);
            //std::cout << newasm::system::cproc << " : " << newline << std::endl;
            #endif
            newasm::system::proclines ++;
            //newasm::variables::ids.at(newasm::system::cproc).
            NewASM::CurrentProc->proc->contents.push_back(lineInfo);
            //newasm::mem::funcs[newasm::system::cproc].push_back(lineInfo);
            return 1;
        }
        switch(lineInfo.whatAmIDoing)//switch(it->second)
        {
            case INVALID_INS:
            {
                newasm::terminate(newasm::exit_codes::invalid_ins);
                return 1;
            }
            //throw
            case newasm::core::lang_inf::throw__:
            {
                newasm::SetExceptionComment("`throw` was used");
                newasm::terminate(newasm::exit_codes::user_error);
                return 1;
            }
            //try
            case newasm::core::lang_inf::try__:
            {
                if(newasm::header::data::repl)
                {
                    newasm::unsins(ins);
                    return 1;
                }

                NewASM::variables::procedureData* p = nullptr;
                if(newasm::header::data::proc_now) p = NewASM::CurrentProcA->proc;
                else if(newasm::LambdaDispatch::LambdaLine) p = &(*newasm::LambdaDispatch::ThreadSafePtr);
                
                if(p)
                {
                    if(p->TryBlock)
                    {
                        p->TryBlock = false;
                        newasm::SetExceptionComment("unusable try-catch block on same identation level");
                        newasm::terminate(newasm::exit_codes::jit_fail);
                        return 1;
                    }
                    p->TryBlock = true;
                    p->TryJump = lineInfo.jumpinTo;
                    return 1;
                }

                if(newasm::thread_line)
                {
                    auto& mmap = newasm::threads::memory.at(newasm::threads::now);
                    if(mmap->TryBlock)
                    {
                        mmap->TryBlock = false;
                        newasm::SetExceptionComment("unusable try-catch block on same identation level");
                        newasm::terminate(newasm::exit_codes::jit_fail);
                        return 1;
                    }
                    mmap->TryBlock = true;
                    mmap->TryJump = lineInfo.jumpinTo;
                    return 1;
                }

                using namespace NewASM::header::data;
                if(TryBlock)
                {
                    TryBlock = false;
                    newasm::SetExceptionComment("unusable try-catch block on same identation level");
                    newasm::terminate(newasm::exit_codes::jit_fail);
                    return 1;
                }
                TryBlock = true;
                TryJump = lineInfo.jumpinTo;
                return 1;
            }
            //nop
            case newasm::core::lang_inf::nop:
            {
                // do nothing
                return 1;
            }
            //retc
            case newasm::core::lang_inf::retc:
            {
                if(newasm::header::data::proc_now)
                {
                    auto& j = NewASM::CurrentProcA;
                    if(j->proc->CallCStack.empty())
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    int address = j->proc->CallCStack.back() + 1;
                    j->proc->CallCStack.pop_back();
                    j->proc->idx = address;
                    return 1;
                }
                else if(newasm::LambdaDispatch::LambdaLine)
                {
                    auto& j = newasm::LambdaDispatch::ThreadSafePtr;
                    if(j->CallCStack.empty())
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                    }

                    int address = j->CallCStack.back() + 1;
                    j->CallCStack.pop_back();
                    j->idx = address;
                    return 1;
                }

                if(newasm::thread_line)
                {
                    //CallCStack is thread_safe so we can do the same thing, the empty function will work for that specific virtual thread
                    if(NewASM::header::data::CallCStack->empty())
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    int address = NewASM::header::data::CallCStack->back() + 1;
                    NewASM::header::data::CallCStack->pop_back();
                    newasm::threads::memory.at(newasm::threads::now)->lcx = address;
                    return 1;
                }

                if(NewASM::header::data::CallCStack->empty())
                {
                    newasm::terminate(newasm::exit_codes::invalid_memacc);
                    return 1;
                }

                int address = NewASM::header::data::CallCStack->back() + 1;
                NewASM::header::data::CallCStack->pop_back();
                NEWASM_JMP__(address)
                //std::cout << "CallCStack size = " << NewASM::header::data::CallCStack->size() << std::endl;
                //std::cout << "CallCStack popped = " << address << std::endl;
                return 1;
            }

            //default
            case newasm::core::lang_inf::default__:
            {
                if(newasm::header::data::case_matched)
                {
                    return 1;
                }
                newasm::procline(newasm::header::data::case_line);
                newasm::header::data::case_matched = true;
                //newasm::header::data::case_line.clear();
                return 1;
            }
            //cls
            case newasm::core::lang_inf::cls:
            {
                newasm::Console::cls();
                return 1;
            }
            //xchg
            case newasm::core::lang_inf::xchg:
            {
                newasm::header::data::temp = newasm::mem::regs::tlr;
                //newasm::mem::regs::tlr = newasm::mem::regs::stl;
                newasm::mem::regs::tlr.set_value(newasm::mem::regs::stl.get_value());
                //newasm::mem::regs::stl = newasm::header::data::temp;
                newasm::mem::regs::stl.set_value(newasm::header::data::temp);
                return 1;
            }
            //syscall
            case newasm::core::lang_inf::syscall:
            {
                newasm::kernel::handleSysCall(); //call the kernel to do the handling
                return 1;
            }
      
            //stack
            case newasm::core::lang_inf::stack:
            {
                if(newasm::header::data::callstkidx == 0)
                {
                    newasm::terminate(newasm::exit_codes::seg_fault);
                    return 1;
                }

                int type;
                // firsly pop the function call
                if(newasm::RAM->StackInfo.empty())
                {
                    newasm::terminate(newasm::exit_codes::seg_fault);
                    return 1;
                }
                newasm::RAM->StackInfo.pop_back();
                newasm::hardware::randAccessMem.pop__STACK<int>(); // no ref

                // then the function arguments
                for(int i = 0; i < newasm::header::data::argc; ++i)
                {
                    int REAL_IDX = newasm::RAM->StackInfo.size() - i - 1;
                    if(
                        REAL_IDX < 0 or
                        REAL_IDX >= newasm::RAM->StackInfo.size()
                    )
                    {
                        newasm::SetExceptionComment(
                            "calculated index `" +
                            newasm::_std::to_string(REAL_IDX) +
                            "` doesn't align with dedicated memory info vectr"
                        );
                        newasm::terminate(newasm::exit_codes::seg_fault);
                        return 1;
                    }
                    type = newasm::RAM->StackInfo.at(REAL_IDX).stkType;
                    if(type == newasm::datatypes::number)
                    {
                        newasm::hardware::randAccessMem.pop__STACK<int>(); // no ref
                        continue;
                    }
                    if(type == newasm::datatypes::decimal)
                    {
                        newasm::hardware::randAccessMem.pop__STACK<float>(); // no ref
                        continue;
                    }
                    if(type == newasm::datatypes::character)
                    {
                        newasm::hardware::randAccessMem.pop__STACK<char>(); // no ref
                        continue;
                    }
                    if(type == newasm::datatypes::text)
                    {
                        newasm::hardware::randAccessMem.pop__STACK<std::string>(); // no ref
                        continue;
                    }
                }


                newasm::header::data::argc = 0;
                newasm::header::data::callstkidx = 0;
                return 1;
            }
            //stack
            case newasm::core::lang_inf::stack__THREAD:
            {
                //std::cout << "STACKT POZVAN -> " << lineInfo.raw << std::endl;
                if(newasm::header::data::callstkidx == 0)
                {
                    newasm::terminate(newasm::exit_codes::seg_fault);
                    return 1;
                }

                int type;
                auto& DedicatedStack = *newasm::DedicatedMemory;
                // firsly pop the function call
                if(DedicatedStack.StackInfo.empty())
                {
                    newasm::terminate(newasm::exit_codes::seg_fault);
                    return 1;
                }
                DedicatedStack.StackInfo.pop_back();
                newasm::hardware::randAccessMem.pop__STACK<int, true>(); // no ref

                // then the function arguments
                for(int i = 0; i < newasm::header::data::argc; ++i)
                {
                    int REAL_IDX = DedicatedStack.StackInfo.size() - i - 1;
                    if(
                        REAL_IDX < 0 or
                        REAL_IDX >= DedicatedStack.StackInfo.size()
                    )
                    {
                        newasm::SetExceptionComment(
                            "calculated index `" +
                            newasm::_std::to_string(REAL_IDX) +
                            "` doesn't align with dedicated memory info vectr"
                        );
                        newasm::terminate(newasm::exit_codes::seg_fault);
                        return 1;
                    }
                    type = DedicatedStack.StackInfo.at(REAL_IDX).stkType;
                    if(type == newasm::datatypes::number)
                    {
                        newasm::hardware::randAccessMem.pop__STACK<int, true>(); // no ref
                        continue;
                    }
                    if(type == newasm::datatypes::decimal)
                    {
                        newasm::hardware::randAccessMem.pop__STACK<float, true>(); // no ref
                        continue;
                    }
                    if(type == newasm::datatypes::character)
                    {
                        newasm::hardware::randAccessMem.pop__STACK<char, true>(); // no ref
                        continue;
                    }
                    if(type == newasm::datatypes::text)
                    {
                        newasm::hardware::randAccessMem.pop__STACK<std::string, true>(); // no ref
                        continue;
                    }
                }


                newasm::header::data::argc = 0;
                newasm::header::data::callstkidx = 0;
                return 1;
            }
            //MATH OPERATIONS
            case newasm::core::lang_inf::add:
            {
                if(newasm::mem::regs::imm == 1)
                {
                    newasm::mem::regs::cr2 = newasm::mem::regs::cr2 + newasm::mem::regs::cr3;
                    return 1;
                }
                newasm::mem::regs::cr0 = newasm::mem::regs::cr0 + newasm::mem::regs::cr1;
                return 1;
            }
            case newasm::core::lang_inf::sub:
            {
                if(newasm::mem::regs::imm == 1)
                {
                    //newasm::mem::regs::cr2 = newasm::mem::regs::cr2 - newasm::mem::regs::cr3;
                    newasm::mem::regs::cr2.set_value(newasm::mem::regs::cr2.get_value() - newasm::mem::regs::cr3.get_value());
                    return 1;
                }
                newasm::mem::regs::cr0.set_value(newasm::mem::regs::cr0.get_value() - newasm::mem::regs::cr1.get_value());
                return 1;
            }
            case newasm::core::lang_inf::div:
            {
                if(newasm::mem::regs::imm == 1)
                {
                    newasm::mem::regs::cr2.set_value(newasm::mem::regs::cr2.get_value() / newasm::mem::regs::cr3.get_value());
                    return 1;
                }
                newasm::mem::regs::cr0.set_value(newasm::mem::regs::cr0.get_value() / newasm::mem::regs::cr1.get_value());
                return 1;
            }
            case newasm::core::lang_inf::mul:
            {
                if(newasm::mem::regs::imm == 1)
                {
                    newasm::mem::regs::cr2 = newasm::mem::regs::cr2 * newasm::mem::regs::cr3;
                    return 1;
                }
                newasm::mem::regs::cr0 = newasm::mem::regs::cr0 * newasm::mem::regs::cr1;
                return 1;
            }
            case newasm::core::lang_inf::mod__:
            {
                if(newasm::mem::regs::imm == 1)
                {
                    newasm::mem::regs::cr2.set_value(newasm::mem::regs::cr2.get_value() % newasm::mem::regs::cr3.get_value());
                    return 1;
                }
                newasm::mem::regs::cr0 = 0.0;
                //newasm::mem::regs::cr0 = newasm::mem::regs::cr0 % newasm::mem::regs::cr1;
                return 1;
            }
            case newasm::core::lang_inf::exp:
            {
                if(newasm::mem::regs::imm == 1)
                {
                    newasm::mem::regs::cr2 = newasm::csimple::ipow(newasm::mem::regs::cr2,newasm::mem::regs::cr3);
                    return 1;
                }
                newasm::mem::regs::cr0 = std::pow(newasm::mem::regs::cr0,newasm::mem::regs::cr1);
                return 1;
            }
            case newasm::core::lang_inf::log:
            {
                if(newasm::mem::regs::imm == 1)
                {
                    newasm::mem::regs::cr2 = static_cast<int>(std::log10(newasm::mem::regs::cr2.get_value()) / std::log10(newasm::mem::regs::cr3.get_value()));
                    return 1;
                }
                newasm::mem::regs::cr0 = std::log10(newasm::mem::regs::cr0) / std::log10(newasm::mem::regs::cr1);
                return 1;
            }
            //BITWISE OPERATIONS
            case newasm::core::lang_inf::and__:
            {
                newasm::mem::regs::br0 = newasm::mem::regs::br0 & newasm::mem::regs::br1;
                return 1;
            }
            case newasm::core::lang_inf::or__:
            {
                newasm::mem::regs::br0 = newasm::mem::regs::br0 | newasm::mem::regs::br1;
                return 1;
            }
            case newasm::core::lang_inf::not__:
            {
                newasm::mem::regs::br0 = ~newasm::mem::regs::br1;
                return 1;
            }
            case newasm::core::lang_inf::xor__:
            {
                newasm::mem::regs::br0 = newasm::mem::regs::br0 ^ newasm::mem::regs::br1;
                return 1;
            }
            case newasm::core::lang_inf::shl:
            {
                newasm::mem::regs::br0 = newasm::mem::regs::br0 << newasm::mem::regs::br1;
                return 1;
            }
            case newasm::core::lang_inf::shr:
            {
                newasm::mem::regs::br0 = newasm::mem::regs::br0 >> newasm::mem::regs::br1;
                return 1;
            }
            default:
            {
                newasm::terminate(newasm::exit_codes::invalid_exp);
                return 1;
            }
        }
        return 1;
    }
    
    inline void process_hndl(int tohandle, const std::string& procedure)
    {
        if(!newasm::mem::functions::datavalid(procedure, newasm::variables::ids))
        {
            newasm::terminate(newasm::exit_codes::invalid_memacc);
            return;
        }
        if(newasm::mem::functions::datavalid(procedure, newasm::variables::ids))
        {
            if(newasm::variables::ids.at(procedure).type != newasm::datatypes::proc)
            {
                newasm::terminate(newasm::exit_codes::invalid_evhndlr);
                return;
            }
        }

        if(newasm::stack::events.find(tohandle) != newasm::stack::events.end())
        {
            newasm::terminate(newasm::exit_codes::hndl_reassign);
            return;
        }
        newasm::stack::events[tohandle] = procedure;
        return;
    }
    int process_text(std::string macroname, std::string symbol)
    {
        if(symbol != "#"_str)
        {
            newasm::terminate(newasm::exit_codes::invalid_syntax);
            return 1;
        }
        if(newasm::header::functions::isalphanum(macroname))
        {
            if(newasm::stack::macros.find(macroname) != newasm::stack::macros.end())
            {
                newasm::terminate(newasm::exit_codes::macro_redef);
                return 1;
            }
            newasm::stack::macros[macroname] = new newasm::stack::macro_data();
            newasm::header::data::macro_now = true;
            newasm::header::data::macro_decl = macroname;
            return 1;
        }
        newasm::terminate(newasm::exit_codes::invalid_syntax);
        return 1;
    }
    #if 0
    int proclineUncompiled(std::string &line)
    {
        line = newasm::header::functions::remc(line);
        line = newasm::header::functions::trim(line);
        if(line[0] == ';')
        {
            return 0;
        }
        if(line.empty())
        {
            return 0;
        }
        if(newasm::header::functions::parseDirective(line).first)
        {
            auto it = newasm::header::functions::parseDirective(line);
            newasm::pp::impl::processDirectives(it.second.first, it.second.second);
            return 0;
        }
        if(newasm::system::section == newasm::code_stream::sections::data)
        if(newasm::header::functions::isdeco(line).first)
        {
            newasm::expcfg::process_dec(newasm::header::functions::isdeco(line).second);
            return 1;
        }
        if(newasm::system::section == newasm::code_stream::sections::data)
        if(newasm::header::functions::parseNamespace(line).first)
        {
            newasm::nms::process_nms(newasm::header::functions::parseNamespace(line).second);
            return 1;
        }
        if(line == static_cast<std::string>("}"))
        {
            if(newasm::header::data::struct_now)
            {
                newasm::header::data::struct_now = false;
                return 1;
            }
            if(newasm::threads::thread_now)
            {
                newasm::threads::thread_now = false;
                return 1;
            }
            newasm::terminate(newasm::exit_codes::unexpected_cbrace);
            return 1;
        }
        if(line == static_cast<std::string>("#"))
        {
            if(newasm::header::data::macro_now)
            {
                newasm::header::data::macro_now = false;
                return 1;
            }
            newasm::terminate(newasm::exit_codes::unexpected_cbrace);
            return 1;
        }
        auto lambda = newasm::header::functions::is_lambda(line);
        if(lambda.first) if(lambda.second == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::end))
        {
            if(newasm::lambda::GLOBAL.contents.empty())
            {
                newasm::terminate(newasm::exit_codes::unexpected_end);
                return 1;
            }
            //std::cout << "Called (LAMBDA.INS) << end" << std::endl;
            newasm::lambda::lambda_now = false;
            newasm::lambda::process = true;
            for(auto i = newasm::lambda::GLOBAL.contents.begin(); i != newasm::lambda::GLOBAL.contents.end(); ++i)
            {
                //std::cout << "lambda line >> " << *i << std::endl;
                //std::cout << "vector size >> " << newasm::lambda::GLOBAL.contents.size() << std::endl;
                newasm::procline(*i);
                if(newasm::lambda::GLOBAL.ret)
                {
                    break;
                }
            }
            newasm::lambda::process = false;
            if(!newasm::lambda::GLOBAL.ret)
            {
                // Must return a value inside a lambda procedure
                newasm::terminate(newasm::exit_codes::invalid_exp);
                return 1;
            }
            try
            {
                //std::cout << "EVAL >> " << newasm::lambda::GLOBAL.line + newasm::lambda::GLOBAL.result << std::endl;
                std::string eval = newasm::lambda::GLOBAL.line + newasm::lambda::GLOBAL.result;
                //std::cout << "RETURNED >> " << newasm::lambda::GLOBAL.result << '\n';
                newasm::procline(eval);
            }
            catch (const std::out_of_range& e) {
                std::cerr << "Out of range: " << e.what() << '\n';
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument: " << e.what() << '\n';
            }
            catch (const std::exception& e) {
                std::cerr << "Standard exception: " << e.what() << '\n';
            }
            
            return 1;
        }

        if(newasm::lambda::lambda_now)
        {
            if(newasm::thread_line)
            {
                if(newasm::lambda::GLOBAL.thread)
                {
                    //std::cout << "Thread :: lambda >> " << line << std::endl;
                    newasm::lambda::GLOBAL.contents.push_back(line);
                }
            }
            if(!newasm::thread_line)
            {
                if(!newasm::lambda::GLOBAL.thread)
                {
                    //std::cout << "Thread.not :: lambda.not >> " << line << std::endl;
                    newasm::lambda::GLOBAL.contents.push_back(line);
                }
            }
            return 1;
        }

        //using namespace std;
        //cout << newasm::threads::thread_now << endl;
        if(newasm::threads::thread_now)
        {
            //std::cout << "FUCCCKKKKKKKKKK2\n";
            newasm::threads::memory.at(newasm::threads::thread_decl)->contents.push_back(line);
            //newasm::threads::memory.at(newasm::threads::thread_decl)->size++;
            return 1;
        }
        if(newasm::header::data::macro_now)
        {
            //std::cout << "FUCCCKKKKKKKKKK2\n";
            newasm::stack::macros.at(newasm::header::data::macro_decl)->contents.push_back(line);
            //newasm::threads::memory.at(newasm::threads::thread_decl)->size++;
            return 1;
        }
        if(newasm::thread_line)
        {
            if(newasm::threads::memory.at(newasm::threads::now)->returned)
            {
                //newasm::threads::memory.at(newasm::threads::now)->contents.pop_front(); //btw get rid of this
                return 1; //skip line
            }
        }
        std::string ins, suf, opr, stat, arg, dtyp, ev, pr;
        std::vector<std::string> tmp, tmp1, tmp2, tmp3;
        tmp.clear();
        tmp1.clear();
        tmp2.clear();
        tmp3.clear();

        ins.clear();
        suf.clear();
        opr.clear();
        
        stat.clear();
        arg.clear();
        dtyp.clear();

        if(line.at(0) == '.')
        {
            bool valid = false;
            std::string sec = newasm::header::functions::trim(line.substr(1,line.size()));
            newasm::process_s_(valid,line,"_",sec);
            if(valid)
            {
                return 1;
            }
        }

        if(newasm::system::section == newasm::code_stream::sections::hndl)
        {
            if(line.size() < 1 || line.find(',') == std::string::npos)
            {
                newasm::terminate(newasm::exit_codes::invalid_syntax);
                return 1;
            }
            tmp = newasm::header::functions::split_fixed(line, ',');
            ev = tmp[0];
            pr = tmp[1];
            
            ev = newasm::header::functions::trim(ev);
            pr = newasm::header::functions::trim(pr);

            return newasm::process_hndl(ev,pr);
        }
        if(newasm::system::section == newasm::code_stream::sections::text)
        {
            if(line.find(':') == std::string::npos)
            {
                newasm::terminate(newasm::exit_codes::invalid_syntax);
                return 1;
            }
            tmp = newasm::header::functions::split_fixed(line, ':');
            stat = tmp[0];
            arg = tmp[1];
            stat = newasm::header::functions::trim(stat);
            arg = newasm::header::functions::trim(arg);
            return newasm::process_text(stat,arg);
        }
        /*if(newasm::system::section == newasm::code_stream::sections::config)
        {
            if(line.find('~') == std::string::npos)
            {
                newasm::terminate(newasm::exit_codes::invalid_syntax);
                return 1;
            }
            tmp = newasm::header::functions::split_fixed(line, '~');
            stat = tmp[0];
            arg = tmp[1];
            stat = newasm::header::functions::trim(stat);
            arg = newasm::header::functions::trim(arg);
            return newasm::process_c(line,stat,arg);
        }*/
        if(newasm::system::section == newasm::code_stream::sections::data)
        {
            try
            {
                if(line.size() < 1 || line.find(':') == std::string::npos)
                {
                    //std::cout << "FUCKKKKKKKKKK\n";
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }
                std::vector<std::string> tokens = newasm::common::tokenize2(line);
                std::vector<std::string> tokens2 = newasm::header::functions::split_fixed(tokens[1],':');

                if(tokens.size() < 2) return newasm::terminate(newasm::exit_codes::os_error);
                if(tokens2.size() < 2) return newasm::terminate(newasm::exit_codes::os_error);

                tokens[0] = newasm::header::functions::trim(tokens[0]);
                tokens2[0] = newasm::header::functions::trim(tokens2[0]);
                tokens2[1] = newasm::header::functions::trim(tokens2[1]);

                //std::cout << tokens[0] << tokens2[0] << tokens2[1] << std::endl; 

                newasm::process_d(line,tokens[0],tokens2[0],tokens2[1]);
            }
            catch(std::exception& err)
            {
                std::cout << "DATA SEC :: " << err.what() << std::endl;
            }
            return 1;
        }
        if(newasm::header::data::struct_now)
        {
            newasm::terminate(newasm::exit_codes::expected_cbrace);
            return 1;
        }
        if(newasm::system::section == newasm::code_stream::sections::start)
        {
            if(newasm::thread_line == false)
            {
                newasm::handle_threads();
            }
            if(newasm::mem::functions::datavalid(line, newasm::mem::instructions))
            {
                for(int i = 0; i < newasm::dynlib::mem::invalid_dynlibs.size(); ++i)
                {
                    if(line == newasm::dynlib::mem::invalid_dynlibs.at(i))
                    {
                        newasm::terminate(newasm::exit_codes::improper_dynlib);
                        return 1;
                    }
                }
                for(int i = 0; i < newasm::mem::instructions[line].size(); ++i)
                {
                    if(newasm::system::terminated)
                    {
                        return 1;
                    }
                    newasm::procline(newasm::mem::instructions[line].at(i));
                }
                return 1;
            }
            if(line.at(0) == '$')
            {
                std::string macroname = newasm::header::functions::trim(line.substr(1));
                auto it = newasm::stack::macros; if(it.find(macroname) != it.end())
                {
                    for(int i = 0; i < it.at(macroname)->contents.size(); ++i)
                    {
                        newasm::procline(it.at(macroname)->contents.at(i));
                    }
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::undefined_macro);
                return 1;
            }
            //tokenizer starts here
            auto is_str_ = [](std::string text, int idx) -> bool {
                std::vector<std::pair<int,int>> positions;
                int first_p = -1, second_p = -1;
        
                for(int i = 0; i < text.size(); ++i)
                {
                    if(text.at(i) == '"')
                    {
                        if(first_p == -1)
                        {
                            first_p = i;
                            continue;
                        }
                        if(second_p == -1)
                        {
                            second_p = i;
                            continue;
                        }
                        if((first_p != -1) && (second_p != -1))
                        {
                            positions.push_back({first_p, second_p});
                            first_p = -1;
                            second_p = -1;
                            continue;
                        }
                    }
                }
                if(first_p == -1)
                {
                    //newasm::terminate(newasm::exit_codes::invalid_exp);
                    return false;
                }
                for(int i = 0; i < positions.size(); ++i)
                {
                    if(positions.at(i).first <= idx && idx <= positions.at(i).second)
                    {
                        return true;
                    }
                }
                return false;
            };
            int idx__ = line.find("->"); //conditionals
            if(idx__ != std::string::npos)
            {
                if(is_str_(line, idx__) == false && is_str_(line, idx__ + 1) == false)
                {
                    if(line.size() != idx__+2)
                    {
                        std::vector<std::string> linetokens_inline = newasm::common::tokenize(line.substr(0, idx__));
                        if(linetokens_inline.size() == 2)
                        {
                            //std::cout << linetokens_inline.at(0) << " + " << linetokens_inline.at(1) << std::endl;
                            newasm::header::data::case_line = line.substr(idx__+2);
                            return process_is(line, linetokens_inline.at(0), linetokens_inline.at(1));
                        }
                        if(linetokens_inline.size() == 1)
                        {
                            //std::cout << linetokens_inline.at(0) << " + " << linetokens_inline.at(1) << std::endl;
                            newasm::header::data::case_line = line.substr(idx__+2);
                            return process_i(line, linetokens_inline.at(0));
                        }
                    }
                }
            }
            std::vector<std::string> linetokens = newasm::common::tokenize(line);

            std::string instruction;
            if(!linetokens.empty())
            {
                instruction = linetokens.at(0);
            }
            if(newasm::header::functions::ishex(instruction))
            {
                for(std::unordered_map<int, std::string>::iterator i = newasm::opcodes::mem.begin(); i != newasm::opcodes::mem.end(); ++i)
                {
                    if(i->first == newasm::header::functions::hextoi(instruction))
                    {
                        /*using namespace std;
                        cout << instruction << endl;
                        cout << i->first << endl;
                        cout << i->second << endl;*/
                        instruction = i->second;
                        //cout << instruction << endl;
                    }
                }
            }               

            if(linetokens.size() == 1)
            {
                return newasm::process_i(line, instruction);
            }
            if(linetokens.size() == 2)
            {
                return newasm::process_is(line, instruction,linetokens.at(1));
            }
            if(linetokens.size() == 3)
            {
                std::string operand;
                operand = linetokens.at(2);
                if(newasm::header::data::proc_now)
                {
                    if(newasm::header::functions::isargref(linetokens.at(2)).first)
                    {
                        newasm::header::data::argc++;
                        operand = newasm::mem::program_memory[newasm::header::data::callstkidx + 2 + newasm::header::functions::isargref(linetokens.at(2)).second];
                    }
                }
                return newasm::process_iso(line, instruction,linetokens.at(1),operand);
            }
         
        }
       
        newasm::terminate(newasm::exit_codes::invalid_syntax);
        return 0;
    }
    #endif

    inline void process_cli(std::string name, decltype(name) classname)
    {
        newasm::runtime::functions::parse(classname);
        classname = newasm::header::functions::remamp(classname);
        auto it = newasm::variables::ids.find(classname);
        if(it == newasm::variables::ids.end())
        {
            newasm::terminate(newasm::exit_codes::invalid_memacc);
            return;
        }

        if(it->second.type != newasm::datatypes::blueprint)
        {
            newasm::terminate(newasm::exit_codes::invalid_memacc);
            return;
        }

        if(newasm::nms::count != 0)
        {
            name = newasm::header::functions::mangleName(newasm::nms::stack, name);
        }

        newasm::header::data::struct_now = true;
        newasm::header::data::struct_decl = name;
        newasm::mem::data_attrib[name].locked = newasm::expcfg::lockbool;

        std::string temp;

        for(int i = 0; i < it->second.blueprint->addr.size(); ++i)
        {
            temp = newasm::hardware::randAccessMem.peek<std::string>(it->second.blueprint->addr[i]);
            newasm::procline(temp);
        }

        newasm::header::data::struct_now = false;
        return;
    }
    
    inline int procline(std::string& line)
    {
        auto JIT_COMPILED = newasm::compiler::DO(line);
        newasm::procline(JIT_COMPILED);
        return 1;
    }

    inline int procline(const char* line)
    {
        std::string buf(line);
        newasm::procline(buf);
        return 1;
    }

    inline int procline(newasm::execBytecode& line)
    {
        if(
            newasm::system::terminated and
            !newasm::events::exitNow
        ) return 1;
        if constexpr(0)
        {
            std::cout << std::endl;
            std::cout << "Current line: " << line.raw << std::endl;
            std::cout << "Thread: " << newasm::thread_line << std::endl;
            std::cout << "Exit now: " << newasm::events::exitNow << std::endl;
            std::cout << std::endl;
        }

        NewASM::ExceptionHandling::Line = &line;
        ++newasm::CYCLE_COUNT;
        newasm::PRC = &line;

        switch(line.type)
        {
            // EMPTY
            case newasm::compiler::empty:
            {
                //although empty lines are not included
                //in the binary, we still need to keep this fr repl
                return 1;
            }
            //LABEL JUMP POINTS
            case newasm::compiler::labelJumpPoint:
            {
                //skip
                __newasmDBG_COMPLEX({
                    std::cout << "labelJumpPoint -> skip: `" << line.raw << '`' << std::endl;
                });
                return 1;
            }
            //ATTRIBUTES
            case newasm::compiler::attribute:
            {
                newasm::runtime::currentAttributes |= line.attribute;
                return 1;
            }
            // DIRECTIVES
            case newasm::compiler::directive:
            {
                if(newasm::header::data::std_now__)
                {
                    if(newasm::system::stop == 1)
                    {
                        NewASM::CurrentProc->proc->contents.push_back(line);
                        //newasm::mem::funcs[newasm::system::cproc].push_back(line);
                        return 1;
                    }
                }
                newasm::pp::impl::processDirectives(line.priInt, line.tokens.at(1));
                return 1;
            }
            // DECORATORS
            case newasm::compiler::decorator:
            {
                /*if(newasm::system::section != newasm::code_stream::sections::data)
                {
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }*/
                newasm::expcfg::process_dec(line);
                return 1;
            }
            // NAMESPACE
            case newasm::compiler::namespace__:
            {
                if constexpr(0) if(
                    newasm::system::section != newasm::code_stream::sections::data and
                    newasm::system::section != newasm::code_stream::sections::start
                )
                {
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }
                newasm::nms::ProcessNamespace(line);
                return 1;
            }
            // CLOSING BRACE
            case newasm::compiler::closingBrace:
            {
                if(newasm::brace_stack__.empty())
                {
                    newasm::terminate(newasm::exit_codes::unexpected_cbrace);
                    return 1;
                }

                signed int brace_purpose = newasm::brace_stack__.back();
                newasm::brace_stack__.pop_back();

                if(brace_purpose == newasm::brace_stack::thread_block)
                {
                    std::string& thread__ = newasm::threads::thread_decl;
                    newasm::threads::thread_now = false;
                    auto& mmap = newasm::threads::memory.at(thread__);
                    mmap->recompile_threadProc();
                    
                    ++NewASM::header::data::ActiveThreads;
                    return 1;
                }
                if(brace_purpose == newasm::brace_stack::object_block)
                {
                    newasm::header::data::struct_now = false;
                    return 1;
                }
                if(brace_purpose == newasm::brace_stack::class_block)
                {
                    //std::cout << "Terminated class -> " << newasm::header::data::blueprint_decl << std::endl;
                    newasm::header::data::blueprint_now = false;
                    return 1;
                }
                if(brace_purpose == newasm::brace_stack::event_block)
                {
                    newasm::events::parsing_now = false;
                    return 1;
                }
                
                return 1;
            }
            // MACRO TERMINATOR
            case newasm::compiler::macroTerminator: [[unlikely]]
            {
                #if 0
                if(newasm::header::data::macro_now)
                {
                    newasm::header::data::macro_now = false;
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::unexpected_hash);
                #endif
                if(newasm::header::data::repl)
                {
                    newasm::terminate(newasm::exit_codes::jit_fail);
                }
                return 1;
            }
            // LAMBDA TERMINATOR
            case newasm::compiler::lambdaTerminator:
            {
                //if we're declaring a thread..
                if(newasm::threads::thread_now)
                {
                    //..push bytecode to thread's data
                    newasm::threads::memory.at(newasm::threads::thread_decl)->contents.push_back(line);
                    return 1;
                }
                if constexpr(NEWASM_CPU_REG_LOG)
                {
                    if(newasm::thread_line)
                    {
                        std::cout << "|                                      |" << std::endl;
                        std::cout << "|---> we're inside a thread lambda <---|" << std::endl;
                        std::cout << "|                                      |" << std::endl;
                    }
                    else
                    {
                        std::cout << "|                                     |" << std::endl;
                        std::cout << "|---> we're inside a maint lambda <---|" << std::endl;
                        std::cout << "|                                     |" << std::endl;
                    }
                }
                auto& p = newasm::LambdaDispatch::ThreadSafePtr;
                if(
                    newasm::LambdaDispatch::LambdaHalt or
                    p->contents.empty()
                )
                {
                    newasm::terminate(newasm::exit_codes::unexpected_end); // if the lambda func was empty
                    return 1;
                }
                p->JIT_compile();
                newasm::LambdaDispatch::LambdaNow = false;

                newasm::LambdaDispatch::LambdaLine = true;
                p->idx = 0;
                while(true)
                {
                    if(newasm::LambdaDispatch::LambdaHalt)
                    {
                        break;
                    }
                    else if(
                        p->idx >= p->contents.size() or
                        p->idx < 0
                    )
                    {
                        break;
                    }
                    auto& lll = p->contents.at(p->idx);
                    newasm::header::data::LastLine = &lll;
                    newasm::procline(lll);
                    p->idx++;
                }
                newasm::LambdaDispatch::LambdaLine = false;
                if(!newasm::LambdaDispatch::LambdaHalt)
                {
                    // Must return a value inside a lambda procedure
                    newasm::terminate(newasm::exit_codes::invalid_exp);
                    return 1;
                }
               
                std::string eval = *newasm::LambdaDispatch::JitLine + *newasm::LambdaDispatch::Result;
                auto JIT_COMPILE = newasm::compiler::DO(eval);
                newasm::procline(JIT_COMPILE);
                if constexpr(NEWASM_CPU_REG_LOG)
                {
                    if(newasm::thread_line)
                    {
                        std::cout << "|                                       |" << std::endl;
                        std::cout << "|---> we're outside a thread lambda <---|" << std::endl;
                        std::cout << "|                                       |" << std::endl;
                    }
                    else
                    {
                        std::cout << "|                                      |" << std::endl;
                        std::cout << "|---> we're outside a maint lambda <---|" << std::endl;
                        std::cout << "|                                      |" << std::endl;
                    }
                }
                return 1;
            }
            //class instance
            case newasm::compiler::classInstance:
            {
                if(newasm::system::section != newasm::code_stream::sections::data)
                {
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }

                newasm::process_cli(line.tokens.at(1), line.tokens.at(2));
                return 1;
            }
        }

        if(newasm::events::parsing_now)
        {
            if(newasm::events::current == newasm::events::exitId)
            {
                newasm::events::exitHandler.GetBytecode.push_back(line);
                return 1;
            }
        }

        if(newasm::header::data::blueprint_now)
        {
            int address = newasm::hardware::randAccessMem.write<std::string>(line.raw);
            NewASM::CurrentClass->blueprint->addr.push_back(address);
            return 1;
        }

        if(newasm::LambdaDispatch::LambdaNow)
        {
            newasm::LambdaDispatch::ThreadSafePtr->contents.push_back(line);
            return 1;
        }

        if(newasm::threads::thread_now)
        {
            newasm::threads::memory.at(newasm::threads::thread_decl)->contents.push_back(line);
            return 1;
        }

        /*
        if(newasm::thread_line)
        {
            if(newasm::threads::memory.at(newasm::threads::now)->returned)
            {
                return 1;
            }
        }*/

        switch(line.type)
        {
            // SEALED LABEL
            case newasm::compiler::sealedLabel:
            {
                if(newasm::system::stop == 1)
                {
                    NewASM::CurrentProc->proc->contents.push_back(line);
                    return 1;
                }
                if(newasm::LambdaDispatch::LambdaLine)
                {
                    return 1;
                }
                //std::cout << "is this the error we're gettin" << std::endl;
                newasm::terminate(newasm::exit_codes::invalid_ins);
                return 1;
            }
            // SECTION MODIFIERS
            case newasm::compiler::sectionModifier:
            {
                //auto testbool = true;
                if constexpr(0) newasm::process_s(line.tokens.at(0), line);
                switch(line.whatCodeSection) // faster
                {
                    case INVALID_INS: [[unlikely]]
                    {
                        newasm::terminate(newasm::exit_codes::invalid_section);
                        return 1;
                    }
                    default: // exists
                    {
                        newasm::garbageCollector::DO();
                        newasm::system::section = line.whatCodeSection;
                        return 1;
                    }
                }
                return 1;
            }
            // HANDLE MODIFIERS
            #if 0
            case newasm::compiler::handleModifier:
            {
                if(newasm::system::section != newasm::code_stream::sections::hndl)
                {
                    newasm::terminate(newasm::exit_codes::invalid_syntax); // make compiler happy :D
                    return 1;
                }
                newasm::process_hndl(line.tokens.at(0), line.tokens.at(1));
                return 1;
            }
            #endif
            // MACRO DECL
            case newasm::compiler::macroDecl: [[unlikely]]
            {
                #if 0
                if(newasm::system::section != newasm::code_stream::sections::text)
                {
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }
                newasm::process_text(line.tokens.at(0), line.tokens.at(1));
                #endif
                if(newasm::header::data::repl)
                {
                    newasm::terminate(newasm::exit_codes::jit_fail);
                }
                return 1;
            }
            // DATA DECL
            case newasm::compiler::dataDecl:
            {
                if(newasm::system::section != newasm::code_stream::sections::data)
                {
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }
                newasm::process_d(line.raw, line.tokens.at(0),line.tokens.at(1),line.tokens.at(2), line);
                return 1;
            }
            // MACRO CALL
            case newasm::compiler::macroCall: [[unlikely]]
            {
                #if 0
                auto& it = newasm::stack::macros;
                if(it.find(line.tokens.at(0)) != it.end())
                {
                    for(int i = 0; i < it.at(line.tokens.at(0))->contents.size(); ++i)
                    {
                        auto JIT_COMPILE = newasm::compiler::DO(it.at(line.tokens.at(0))->contents.at(i));
                        newasm::procline(JIT_COMPILE);
                    }
                    return 1;
                }
                #endif
                if(newasm::header::data::repl)
                {
                    newasm::terminate(newasm::exit_codes::undefined_macro);
                    newasm::terminate(newasm::exit_codes::jit_fail);
                }
                return 1;
            }
        }

        if(newasm::header::data::struct_now)
        {
            newasm::terminate(newasm::exit_codes::expected_cbrace);
            return 1;
        }

        if(newasm::header::data::blueprint_now)
        {
            newasm::terminate(newasm::exit_codes::expected_cbrace);
            return 1;
        }

        if(newasm::system::section == newasm::code_stream::sections::start)
        {
            if(newasm::thread_line == false)
            {
                if(NewASM::header::data::ActiveThreads != 0) newasm::handle_threads();
            }
            std::string libname;
            
            switch(line.type)
            {
                case newasm::compiler::conditional:
                {
                    if(line.tokens.size() == 1)
                    {
                        newasm::header::data::case_line = line.other;
                        newasm::process_i(line.raw, line.tokens.at(0), line);
                        return 1;
                    }
                    if(line.tokens.size() == 2)
                    {
                        newasm::header::data::case_line = line.other;
                        newasm::process_is(line.raw, line.tokens.at(0), line.tokens.at(1), line);
                        return 1;
                    }
                    newasm::terminate(newasm::exit_codes::os_error);
                    return 1;
                }
                case newasm::compiler::instruction:
                {
                    if(line.tokens.size() == 1)
                    {
                        //if(newasm::mem::functions::datavalid(libname, newasm::mem::instructions))
                        if(line.whatAmIDoing == INS_EXTERNAL)
                        {
                            libname = line.tokens.at(0);
                            for(int i = 0; i < newasm::dynlib::mem::invalid_dynlibs.size(); ++i)
                            {
                                if(libname == newasm::dynlib::mem::invalid_dynlibs.at(i))
                                {
                                    newasm::terminate(newasm::exit_codes::improper_dynlib);
                                    return 1;
                                }
                            }
                            for(int i = 0; i < newasm::mem::instructions[libname].size(); ++i)
                            {
                                if(newasm::system::terminated)
                                {
                                    return 1;
                                }
                                auto JIT_COMPILE = newasm::compiler::DO(newasm::mem::instructions[libname].at(i));
                                newasm::procline(JIT_COMPILE);
                            }
                            return 1;
                        }
                        newasm::process_i(line.raw, line.tokens.at(0), line);
                        return 1;
                    }
                    if(line.tokens.size() == 2)
                    {
                        newasm::process_is(line.raw, line.tokens.at(0), line.tokens.at(1), line);
                        return 1;
                    }
                    if(line.tokens.size() == 3)
                    {
                        auto operand = line.tokens.at(2);
                        if(newasm::header::data::proc_now)
                        {
                            if(line.AltStackArg)
                            {
                                newasm::header::data::argc ++;                                
                                // If the address of the func handler is i,
                                // then we are looking for i-argid address
                                // that i is callstkidx
                                int argid = line.altInt;
                                if(!newasm::thread_line)
                                {
                                    int idx = newasm::RAM->StackInfo.size() - 2 - argid;
                                    if(
                                        idx < 0 or
                                        idx >= newasm::RAM->StackInfo.size()
                                    )
                                    {
                                        newasm::terminate(newasm::exit_codes::seg_fault);
                                        return 1;
                                    }
                                    auto& p = newasm::RAM->StackInfo.at(idx);
                                    int type = p.stkType;
                                    int argaddr = p.stkAddr;

                                    if(type == newasm::datatypes::number)
                                    {
                                        operand = newasm::_std::to_string(newasm::hardware::randAccessMem.peek<int>(argaddr));
                                    }
                                    else if(type == newasm::datatypes::decimal)
                                    {
                                        operand = newasm::_std::to_string(newasm::hardware::randAccessMem.peek<float>(argaddr));
                                    }
                                    else if(type == newasm::datatypes::character)
                                    {
                                        std::string buf(1, newasm::hardware::randAccessMem.peek<char>(argaddr));
                                        operand = "'";
                                        operand += buf;
                                        operand += "'";
                                    }
                                    else if(type == newasm::datatypes::text)
                                    {
                                        operand = "\"";
                                        operand += newasm::hardware::randAccessMem.peek<std::string>(argaddr);
                                        operand += "\"";
                                    }
                                }
                                else
                                {
                                    auto& DedicatedStack = *newasm::DedicatedMemory;
                                    int idx = DedicatedStack.StackInfo.size() - 2 - argid;
                                    if(
                                        idx < 0 or
                                        idx >= DedicatedStack.StackInfo.size()
                                    )
                                    {
                                        newasm::terminate(newasm::exit_codes::seg_fault);
                                        return 1;
                                    }
                                    auto& p = DedicatedStack.StackInfo.at(idx);
                                    int type = p.stkType;
                                    int argaddr = p.stkAddr;

                                    if(type == newasm::datatypes::number)
                                    {
                                        operand = newasm::_std::to_string(newasm::hardware::randAccessMem.peek<int>(argaddr));
                                    }
                                    else if(type == newasm::datatypes::decimal)
                                    {
                                        operand = newasm::_std::to_string(newasm::hardware::randAccessMem.peek<float>(argaddr));
                                    }
                                    else if(type == newasm::datatypes::character)
                                    {
                                        std::string buf(1, newasm::hardware::randAccessMem.peek<char>(argaddr));
                                        operand = "'";
                                        operand += buf;
                                        operand += "'";
                                    }
                                    else if(type == newasm::datatypes::text)
                                    {
                                        operand = "\"";
                                        operand += newasm::hardware::randAccessMem.peek<std::string>(argaddr);
                                        operand += "\"";
                                    }
                                }
                                //std::cout << "operand is `" << operand << "`" << std::endl;
                                //std::cout << "stk is " << newasm::mem::regs::stk.get_value() << std::endl;
                            }
                        }
                        newasm::process_iso(line.raw, line.tokens.at(0), line.tokens.at(1), operand, line);
                        return 1;
                    }
                    newasm::terminate(newasm::exit_codes::os_error);
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::os_error);
                return 1;
            }
        }
        
        newasm::terminate(newasm::exit_codes::invalid_syntax);
        return 0;
    }
  
    void analyzeline(std::string &line, int lineidx)
    {
        //using namespace std;
        //cout << "DEBUG:\t" << line << endl;
        
        try
        {
            std::string arg;

            if(line.size() <= 1 || line.find(':') == std::string::npos) return;

            line = newasm::header::functions::trim(line);

            if(lineidx != newasm::code_stream::invalid_lnidx) if(line.at(0) == ':')
            {
                arg = line.substr(1);
                arg = newasm::header::functions::trim(arg);
                newasm::process_l(arg, lineidx);
            }
        }
        catch(std::exception& err)
        {
            std::cout << "LABEL SEC :: " << err.what() << std::endl;
        }
        return;
    }
    FORCE_INLINE inline void callproc(VarPtr ptr)
    {
        newasm::system::stoproc = 0;
        newasm::header::data::proc_now = true;
        newasm::system::processing_proc = ptr->proc->original_name;
        NewASM::CurrentProcA = ptr;
        
        //std::cout << "Actually called -> " << ptr->proc->original_name << std::endl;

        ptr->proc->idx = 0;
        auto& proc_contents = ptr->proc->contents;
        while(true)//for(int i = 0; i < proc_contents.size(); ++i)
        {
            if(newasm::system::stoproc == 1)
            {
                newasm::system::stoproc = 0;
                newasm::header::data::proc_now = false;
                break;
            }
            if(
                ptr->proc->idx >= proc_contents.size() or
                ptr->proc->idx < 0
            )
            {
                break;
            }
            newasm::header::data::LastLine = &proc_contents.at(ptr->proc->idx);
            newasm::procline(proc_contents.at(ptr->proc->idx));
            ptr->proc->idx++;
        }
        //newasm::header::data::proc_now = false;
        newasm::header::data::proc_now = false;
        return;
    }
    inline void callproc(const std::string& name)
    {
        auto it = newasm::variables::ids.find(name);
        if(it != newasm::variables::ids.end())
        {
            callproc(&it->second);
        }
        return;
    }

    inline void async(const std::string& name)
    {
        auto it = newasm::variables::ids.find(name);
        if(it != newasm::variables::ids.end())
        {
            newasm::threads::memory[name] = new newasm::threads::object__();
            auto& mmap = newasm::threads::memory.at(name);
            auto& mmap2 = newasm::variables::ids.at(name);
            mmap->contents = mmap2.proc->contents;
            mmap->labels = mmap2.proc->labels;
            newasm::threads::valid_threads.push_back(name);
        }
        return;
    }

    #if 0
    [[maybe_unused]]
    inline void copyproc(std::string& name)
    {
        newasm::global::event_codeblock.clear();
        auto it = newasm::mem::funcs.find(name);
        if(it != newasm::mem::funcs.end())
        {
            for(auto& line : it->second)
            {
                newasm::global::event_codeblock.push_back(line.raw);
            }
        }
        return;
    }
    #endif
    int analyze(std::string file)
    {
        std::ifstream internal_fileobject(/*newasm::header::constants::scripts_folder + */file);
        if(internal_fileobject.is_open())
        {
            std::string line;
            int lineidx = 1;

            while(std::getline(internal_fileobject, line))
            {
                newasm::analyzeline(line, lineidx);
                lineidx++;
            }
            internal_fileobject.close();
            return 1;
        }
        else
        {
            newasm::header::functions::err(
                "Unable to open the file: `"_str +
                file +
                "`"_str
            );
        }
        return 0;
    }

    inline void handle_exit()
    {
        //std::cout << "newasm::events::exitHandler.GetBytecode.size() -> " << newasm::events::exitHandler.GetBytecode.size() << std::endl;
        //std::cout << "newasm::exit_handled -> " << newasm::exit_handled << std::endl;
        if(newasm::exit_handled)
        {
            //newasm::header::functions::err("Exit handler called again.");
            return;
        }
        newasm::exit_handled = true;
        newasm::perf::heavyHostServices.start();

        newasm::events::exitNow = true;
        for(int i = 0; i < newasm::events::exitHandler.GetBytecode.size(); ++i)
        {
            newasm::procline(newasm::events::exitHandler.GetBytecode.at(i));
        }
        newasm::events::exitNow = false;
        newasm::perf::heavyHostServices.stop();
        return;
    }

    #if 0
    [[deprecated]]
    inline void handle_exit_() //VERY MEMORY UNSAFE FUNCTION!
                                //beware C++ developers!
    {
        return;
        if(newasm::exit_handled)
        {
            return;
        }
        auto exec_exit_handle = []() -> void {
            newasm::runtime::functions::parse<true>(newasm::handlers::exit_handler);
            auto it = newasm::mem::funcs.find(newasm::handlers::exit_handler);
            //std::cout << it->first << " | " << bool(it != newasm::mem::funcs.end()) << std::endl;
            if(it != newasm::mem::funcs.end())
            {
                newasm::global::event_now = true;
                newasm::copyproc(newasm::handlers::exit_handler);
                //std::cout << "Size -> " << newasm::global::event_codeblock.size() << std::endl;
                for(auto i = newasm::global::event_codeblock.begin(); i != newasm::global::event_codeblock.end(); ++i)
                {
                    //std::cout << "Processing -> `" << *i << "`" << std::endl;
                    auto COMPILED = newasm::compiler::DO(*i);
                    newasm::procline(COMPILED);
                }
                newasm::global::event_now = false;
            }
            return;
        };
        exec_exit_handle();
        newasm::exit_handled = true;
        return;
    }
    #endif

    template<bool what>
    void execute()
    {
        //clean up for jit
        newasm::compiler::data::symbol_map.clear();
        newasm::compiler::data::JIT_mode = true;
        
        if constexpr(what == true)
        {
            newasm::mem::regs::resetRegisters();
            newasm::mem::regs::hea = 0;
            newasm::mem::regs::lcx.set_value(0);
        }
        //cleanup timers
        newasm::wasted_deduction.clear();
        newasm::network_deduction.clear();
        newasm::perf::inputWasteTimer.clear();
        newasm::perf::heavyHostServices.clear();
        newasm::perf::StandardLibLoading.clear();
        newasm::perf::DynLibLoading.clear();
        //now we load the standard lib after loading the case jump table
        newasm::CYCLE_COUNT = 0, newasm::perf::start = std::chrono::steady_clock::now();
        newasm::perf::StandardLibLoading.start();
        bool result = newasm::GLOBAL::global_load_std();
        if(!result)
        {
            newasm::perf::StandardLibLoading.stop();
            return;
        }
        newasm::perf::StandardLibLoading.stop();

        //we load other libs
        auto status = newasm::LoadDynamicLibraries();
        if(!status.first)
        {
            newasm::header::functions::linkinfo(
                "Couldn't load dynamic library: `" +
                status.second + "`"
            );
            return;
        }

        auto size = newasm::compiler::compiledCode.size();

        while(true)
        {
            if(newasm::system::terminated)
            {
                break;
            }

            if(
                newasm::mem::regs::lcx.get_value() >= size or
                newasm::mem::regs::lcx.get_value() < 0
            )
            {
                break;
            }

            auto& ll = newasm::compiler::compiledCode.at(newasm::mem::regs::lcx.get_value());

            newasm::header::data::LastLine = &ll;
            newasm::header::data::lastlndx = newasm::mem::regs::lcx.get_value();
            
            #if NEWASM_DEBUG == 1
            try{
            #endif
            newasm::procline(ll);
            #if NEWASM_DEBUG == 1
            }catch(const std::exception& e)
            {
                std::cerr << "Kompajler te zajebucnuo -> " << e.what() << '\n';
                std::cout << "LCX value -> " << newasm::mem::regs::lcx.get_value() << std::endl;
                std::cout << "CC size -> " << newasm::compiler::compiledCode.size() << std::endl;
                std::cout << "Line data -> " << newasm::compiler::compiledCode.at(newasm::mem::regs::lcx.get_value()).raw << std::endl;
            }
            #endif
            
            if(newasm::code_stream::jump)
            {
                newasm::code_stream::jump = 0;
                newasm::mem::regs::lcx.set_value(newasm::code_stream::jumpto);
            }
            else newasm::mem::regs::lcx.set_value(newasm::mem::regs::lcx.get_value() + 1);
        }

        if(
            newasm::system::terminated and
            !newasm::header::data::exception
        )
        {
            newasm::handle_exit();
        }

        if(!newasm::system::terminated)
        {
            newasm::terminate(newasm::exit_codes::noterm_point); // You got to end your programs.
        }
        newasm::perf::end = std::chrono::steady_clock::now();
        newasm::compiler::data::JIT_mode = false;
        newasm::UnloadDynamicLibraries();
        return;
    }
    #if 0 //this ain't comin
    inline int compile(const std::string& file)
    {
        newasm::mem::COD.clear();
        newasm::compiler::compiledCode.clear();
        newasm::mem::instructions.clear();
        newasm::mem::labels.clear();
    
        std::string line;
        int lineidx = newasm::mem::regs::lcx.get_value();
        newasm::system::terminated = false;

        std::ifstream _file(file);
        if(_file.is_open())
        {
            lineidx = 1;
            while(std::getline(_file, line))
            {
                //std::cout << lineidx << " |  " << line << std::endl;
                line = newasm::header::functions::trim(line);

                if(line.empty())
                {
                    lineidx++;
                    newasm::mem::COD.push_back("; empty");
                    continue;
                }

                if(line.at(0) == ';')
                {
                    lineidx++;
                    newasm::mem::COD.push_back("; comment");
                    continue;
                }
                line = newasm::header::functions::remc(line);
          
                newasm::mem::COD.push_back(line);
                lineidx++;
            }
            lineidx = 1;
            _file.close();

            newasm::header::functions::compilerinfo("Compiling the project...");
            newasm::compiler::data::lnidx = 1;
            __newasmDBG_COMPLEX({
                std::cout << "LINE DATA SIZE -> " << newasm::forLinker::lineData.size() << std::endl;
                std::cout << "MEM COD SIZE -> " << newasm::mem::COD.size() << std::endl;
            });
            try
            {
                for(int i = 0; i < newasm::mem::COD.size();)//for(auto i = newasm::mem::COD.begin(); i != newasm::mem::COD.end(); ++i)
                {
                    auto g = newasm::compiler::DO(newasm::mem::COD.at(i));
                    if(g.type == newasm::compiler::empty) //VERY IMPORTANT OPTIMIZATIONZ!!
                    {
                        //empty lines are no more included in the binary!
                        newasm::forLinker::lineData.erase(newasm::forLinker::lineData.begin() + i);
                        newasm::mem::COD.erase(newasm::mem::COD.begin() + i);
                        continue;
                    }

                    newasm::compiler::compiledCode.push_back(g);
                    ++newasm::compiler::data::lnidx;
                    ++i;
                }
            }
            catch(std::exception& e)
            {
                std::cout << "E JEBGA SAD KUME! -> " << e.what() << std::endl;
                throw;
            }
            for(int i = 0; i < newasm::compiler::compiledCode.size(); ++i)
            {
                if(newasm::compiler::compiledCode.at(i).type == newasm::compiler::labelJumpPoint)
                {
                    newasm::process_l(newasm::compiler::compiledCode.at(i).other, i);
                }
            }

            if(!newasm::compiler::data::aborted)
            {
                std::cout << "  " << newasm::header::col::gray << "\tProject successfully compiled.\n\n";
            }
            if(newasm::compiler::data::aborted)
            {
                std::cout << "  " << newasm::header::col::red << "\tCompilation aborted.\n\n";
            }
            std::cout << newasm::header::col::reset;
            if(newasm::compiler::data::aborted)
            {
                std::cout << std::endl;
                return 1;
            }

            newasm::compiler::bin::ASSEMBLE( // create the binary format
                newasm::bin_out_name,
                newasm::compiler::compiledCode,
                newasm::mem::labels,
                newasm::forLinker::lineData,
                newasm::mem::instructions
            );
            return 1;
        }
        else
        {
            newasm::header::functions::err(
                static_cast<std::string>("Unable to open the file: ") + static_cast<std::string>("`") + 
                /*newasm::header::constants::scripts_folder +*/ file + static_cast<std::string>("`"));
        }
        return 0;
    }
    #endif
    inline int compile_and_exec(std::string file, int lineidx_____)
    {
        NewASM::DynLibNames.clear();
        if(lineidx_____ == -1)
        {
            newasm::mem::regs::resetRegisters();
            newasm::mem::regs::hea = 0;

            newasm::mem::COD.clear();
            newasm::compiler::compiledCode.clear();
            //newasm::compiler::lineData first;
            //first.type = newasm::compiler::empty;
            //newasm::compiler::compiledCode.push_back(first);
            //newasm::mem::COD.push_back("; comment");
            newasm::mem::regs::lcx.set_value(0);
        }
        

        std::string line;
        int lineidx = newasm::mem::regs::lcx.get_value();
        newasm::system::terminated = false;

        std::ifstream _file(file);
        if(_file.is_open())
        {
            lineidx = 1;
            //std::cout << "file = `" << file << "`" << std::endl;
            if(lineidx_____ == -1) while(std::getline(_file, line)) //internal compile
            {
                //std::cout << lineidx << " |  " << line << std::endl;
                line = newasm::header::functions::trim(line);

                if(line.empty())
                {
                    lineidx++;
                    newasm::mem::COD.push_back("; empty");
                    continue;
                }

                if(line.at(0) == ';')
                {
                    lineidx++;
                    newasm::mem::COD.push_back("; comment");
                    continue;
                }
                line = newasm::header::functions::remc(line);
                #if 0
                if(line.at(0) == ':')
                {
                    lineidx++;
                    newasm::process_l(newasm::header::functions::trim(line.substr(1)), lineidx);
                    newasm::mem::COD.push_back(NEWASM_JUMP_POINT);
                    continue;
                }
                #endif
                newasm::mem::COD.push_back(line);
                lineidx++;
            }
            lineidx = 1;
            _file.close();

            newasm::header::functions::compilerinfo("Compiling the project...");
            newasm::compiler::data::lnidx = 0;
            __newasmDBG_COMPLEX({
                std::cout << "LINE DATA SIZE -> " << newasm::forLinker::lineData.size() << std::endl;
                std::cout << "MEM COD SIZE -> " << newasm::mem::COD.size() << std::endl;
            });
            if(NewASM::header::data::LogCompilerOptimizations)
            {
                NewASM::header::functions::compilerinfo("Performing code optimizations...");
            }
            try
            {
                for(int i = 0; i < newasm::mem::COD.size(); ++i)//for(auto i = newasm::mem::COD.begin(); i != newasm::mem::COD.end(); ++i)
                {
                    newasm::compiler::data::lnidx = i;

                    auto g = newasm::compiler::DO(newasm::mem::COD.at(i));
                    NewASM::compiler::Optimize<newasm::compiler::OPT_PEEPHOLE>(g, i);

                    newasm::compiler::compiledCode.push_back(g);
                }
            }
            catch(std::exception& e)
            {
                std::cout << "E JEBGA SAD KUME! -> " << e.what() << std::endl;
                throw;
            }
            // 2nd pass for macros, we need to tinker with the linker again //newasm::Linker::replaceVectorElement__NEW
            auto& v1 = newasm::compiler::compiledCode;
            auto& v2 = newasm::forLinker::lineData;
            auto& f = newasm::compiler::data::MacroTable;
            for(int i = 0; i < v1.size(); ++i)
            {
                if(v1[i].type == newasm::compiler::macroCall) //macro inlining
                {
                    auto& l = v1[i];
                    if(f.find(l.other) == f.end())
                    {
                        newasm::compiler::abort(newasm::compiler::fail::unmatched_syntax);
                        break;
                    }
                    std::vector<newasm::compiler::lineData>& v3 = f.at(l.other);
                    newasm::Linker::replaceVectorElement__NEW(v1, v3, i);
                    std::vector<NewASM::lineSource> v4;
                    for(int j = 0; j < v3.size(); ++j)
                    {
                        v4.push_back({v2.at(i).first, v2.at(i).second});
                    }
                    newasm::Linker::replaceVectorElement__NEW(v2, v4, i);
                }
            }
            //intermediary pass for eliminating dead code
            if(v1.size() != v2.size()) [[unlikely]]
            {
                newasm::header::functions::err("CC and LD sizes don't match!");
            }

            NewASM::compiler::Optimize<newasm::compiler::OPT_CODESEC>();

            //removing unreachable code
            NewASM::compiler::Optimize<newasm::compiler::OPT_UNREACHABLE>();

            for(int i = 0; i < v1.size(); true) // VERY IMPORTANT PART!
            {
                //this loop ensures that all empty lines are not in the final binary
                if(
                    v1[i].type == newasm::compiler::empty or
                    v1[i].type == newasm::compiler::macroTerminator or
                    v1[i].type == newasm::compiler::macroDecl
                )
                {
                    v1.erase(v1.begin() + i);
                    v2.erase(v2.begin() + i);
                }
                else
                {
                    // if it isnt empty go on
                    ++i;
                }
            }
            //3rd compilation pass for labels
            for(int i = 0; i < newasm::compiler::compiledCode.size(); ++i)
            {
                newasm::compiler::data::lnidx = i;
                auto& bytecode = newasm::compiler::compiledCode.at(i);
                if(bytecode.type == newasm::compiler::labelJumpPoint)
                {
                    newasm::compiler::data::line = bytecode.raw;
                    newasm::process_l(bytecode.other, i);
                }
            }
            //newasm::compiler::data::lnidx = 1;
            //thing above us was for this down here
            auto AOTCompileBytecode = <:&:>(auto& vec) -> void {
                static bool already_processed = false;
                for(int i = 0; i < vec.size(); ++i)
                {
                    auto& bytecode = vec.at(i);
                    if(!already_processed) newasm::compiler::data::lnidx = i;
                    newasm::compiler::data::line = bytecode.raw;
                    if(
                        bytecode.whatAmIDoing == newasm::core::lang_inf::loop
                    )
                    {
                        if(bytecode.tokens.size() != 3)
                        {
                            continue;
                        }
                        auto label_name = newasm::header::functions::trim(bytecode.tokens[2]);
                        auto p = NewASM::header::functions::DetectNamespace(label_name);
                        if(p.first)
                        {
                            label_name = NewASM::compiler::utils::MangleName(p.second.first, p.second.second);
                        }
                        auto& sl = newasm::compiler::data::sealed_labels;
                        if(
                            newasm::mem::labels.find(label_name) == newasm::mem::labels.end() and
                            ![&](const std::string& name) -> bool {
                                for(int q = 0; q < sl.size(); ++q)
                                {
                                    if(sl[q] == name)
                                    {
                                        return true;
                                    }
                                }
                                return false;
                            }(label_name)
                        )
                        {
                            newasm::compiler::abort(newasm::compiler::fail::unknown_label);
                            //std::cout << "Tried compiling -> `" << bytecode.raw << "` " << (std::find(sl.begin(), sl.end(), label_name) != sl.end()) << "\n";
                            if(0) for(int j = 0; j < sl.size(); ++j)
                            {
                                std::cout << "sl[" << j << "] = `" << sl[j] << "`\n";
                            }
                            break;
                        }
                        try
                        {
                            bytecode.jumpinTo = newasm::mem::labels[label_name];
                        }
                        catch(std::exception& e)
                        {
                            if(0) for(auto p = newasm::mem::labels.begin(); p != newasm::mem::labels.end(); ++p)
                            {
                                std::cout << "labels[" << p->first << "] = `" << p->second << "`\n";
                            }
                            newasm::compiler::abort(newasm::compiler::fail::unknown_label);
                        }
                    }
                    if(newasm::compiler::utils::IsJumpIns(bytecode))
                    {
                        if(bytecode.whatAmIDoing == newasm::core::lang_inf::catch__)
                        {
                            if(bytecode.priArgType == newasm::datatypes::NIL)
                            {
                                continue;
                            }
                        }
                        if(bytecode.tokens.size() != 2)
                        {
                            continue;
                        }
                        auto label_name = newasm::header::functions::trim(bytecode.tokens[1]);
                        auto p = NewASM::header::functions::DetectNamespace(label_name);
                        if(p.first)
                        {
                            label_name = NewASM::compiler::utils::MangleName(p.second.first, p.second.second);
                        }
                        auto& sl = newasm::compiler::data::sealed_labels;
                        if(bytecode.whatAmIDoing == newasm::core::lang_inf::callc)
                        {
                            if(already_processed)
                            {
                                newasm::compiler::abort(newasm::compiler::fail::retc_fail);
                                break;
                            }
                        }
                        if(
                            newasm::mem::labels.find(label_name) == newasm::mem::labels.end() and
                            ![&](const std::string& name) -> bool {
                                for(int q = 0; q < sl.size(); ++q)
                                {
                                    if(sl[q] == name)
                                    {
                                        return true;
                                    }
                                }
                                return false;
                            }(label_name)
                        )
                        {
                            newasm::compiler::abort(newasm::compiler::fail::unknown_label);
                            //std::cout << "Tried compiling -> `" << bytecode.raw << "` " << (std::find(sl.begin(), sl.end(), label_name) != sl.end()) << "\n";
                            if(0) for(int j = 0; j < sl.size(); ++j)
                            {
                                std::cout << "sl[" << j << "] = `" << sl[j] << "`\n";
                            }
                            break;
                        }
                        try
                        {
                            if(bytecode.whatAmIDoing == newasm::core::lang_inf::callc)
                            {
                                bytecode.returninTo = i;
                            }
                            bytecode.jumpinTo = newasm::mem::labels[label_name];
                        }
                        catch(std::exception& e)
                        {
                            for(auto p = newasm::mem::labels.begin(); p != newasm::mem::labels.end(); ++p)
                            {
                                std::cout << "labels[" << p->first << "] = `" << p->second << "`\n";
                            }
                            newasm::compiler::abort(newasm::compiler::fail::unknown_label);
                        }
                    }
                }
                already_processed = true;
                return;
            };

            AOTCompileBytecode(newasm::compiler::compiledCode);
            AOTCompileBytecode(newasm::compiler::caseJumpTable);

            //4th compiler pass for try-catch blocks
            int TryFound = -1;
            //int CatchFound = -1;
            for(int i = 0; i < newasm::compiler::compiledCode.size(); ++i)
            {
                newasm::compiler::data::lnidx = i;
                auto& bytecode = newasm::compiler::compiledCode.at(i);
                newasm::compiler::data::line = bytecode.raw;
                
                //we check if try and catch blocks align
                if(
                    bytecode.type == newasm::compiler::labelJumpPoint or
                    bytecode.type == newasm::compiler::sectionModifier or
                    bytecode.whatAmIDoing == newasm::core::lang_inf::proc or
                    bytecode.whatAmIDoing == newasm::core::lang_inf::thread__
                )
                {
                    if(TryFound != -1)
                    {
                        newasm::compiler::abort(newasm::compiler::fail::unmatched_syntax);
                        break;
                    }
                }
                if(bytecode.whatAmIDoing == newasm::core::lang_inf::try__)
                {
                    if(TryFound != -1)
                    {
                        newasm::compiler::abort(newasm::compiler::fail::redundant_try);
                        break;
                    }
                    TryFound = i;
                    continue;
                }
                if(bytecode.whatAmIDoing == newasm::core::lang_inf::catch__)
                {
                    if(TryFound == -1)
                    {
                        newasm::compiler::abort(newasm::compiler::fail::redundant_catch);
                        break;
                    }
                    newasm::compiler::compiledCode.at(TryFound).jumpinTo = i;
                    TryFound = -1;
                    continue;
                }
            }

            __newasmDBG_COMPLEX({
                std::cout << "2: LINE DATA SIZE -> " << newasm::forLinker::lineData.size() << std::endl;
                std::cout << "2: COMPILED CODE SIZE -> " << newasm::compiler::compiledCode.size() << std::endl;
                std::cout << "Line data front -> " << newasm::forLinker::lineData.front().first << ", " << newasm::forLinker::lineData.front().second << std::endl;
                std::cout << "Compiled code front -> " << newasm::compiler::compiledCode.front().raw << std::endl;
                std::cout << "Line data back -> " << newasm::forLinker::lineData.back().first << ", " << newasm::forLinker::lineData.back().second << std::endl;
                std::cout << "Compiled code back -> " << newasm::compiler::compiledCode.back().raw << std::endl;
            });

            static const std::string newasm_LOCAL_ALIGNMENT__ = "  "_str;
            std::cout << newasm_LOCAL_ALIGNMENT__ << newasm::header::col::magenta;
            std::cout << "\t" << NewASM::compiler::data::OptimizationCount << " optimization(s).\n";
            std::cout << newasm::header::col::reset;

            if(!newasm::compiler::data::aborted)
            {
                std::cout << newasm_LOCAL_ALIGNMENT__ << newasm::header::col::gray << "\tProject successfully compiled. Running...\n\n";
            }
            if(newasm::compiler::data::aborted)
            {
                std::cout << newasm_LOCAL_ALIGNMENT__ << newasm::header::col::red << "\t" << NewASM::compiler::data::ErrorCount << " error(s).\n";
                std::cout << newasm_LOCAL_ALIGNMENT__ << newasm::header::col::red << "\tCompilation aborted.\n\n";
            }
            std::cout << newasm::header::col::reset;
            if(newasm::compiler::data::aborted)
            {
                std::cout << std::endl;
                return 1;
            }

            newasm::compiler::bin::ASSEMBLE( // create the binary format
                newasm::bin_out_name,
                newasm::compiler::compiledCode,
                newasm::mem::labels,
                newasm::forLinker::lineData,
                newasm::mem::instructions,
                newasm::compiler::caseJumpTable,
                NewASM::DynLibNames
            );

            newasm::header::functions::wait(1000);
            newasm::execute<false>();
        }
        else
        {
            newasm::header::functions::err(
                "Unable to open the file: `"_str +
                file + "`"_str
            );
        }
        return 0;
    }
    inline void handle_threads()
    {
        if(!NewASM::header::data::EnableThreads)
        {
            return;
        }
        if(newasm::header::data::proc_now)
        {
            return;
        }
        NewASM::header::data::ActiveThreads = 0;
        for(int p = 0; p < newasm::threads::valid_threads.size(); ++p)
        {
            auto& i = newasm::threads::valid_threads.at(p);
            auto& mmap = newasm::threads::memory.at(i);
            //if(0) newasm::threads::memory.at(*i)->prepare_sys();
            if(mmap->returned)
            {
                continue;
            }
            if(mmap->contents.empty())
            {
                continue;
            }
            ++NewASM::header::data::ActiveThreads;
            auto& IDX = mmap->lcx;
            newasm::thread_line = true;
            newasm::threads::now = (i); // thread name
            newasm::threads::id_now = mmap->id;
            
            if(IDX < 0 or IDX >= mmap->contents.size())
            {
                mmap->returned = true;
                mmap->returned_val = "0";
                auto ptr = newasm::_this.getThreadValue(mmap->id);
                if(ptr != nullptr)
                {
                    ptr->fetched = false;
                    newasm::_this.setThreadValue(mmap->id, nullptr);
                }
            }
            else newasm::procline(mmap->contents.at(IDX));
            
            ++IDX;
            newasm::thread_line = false;
            if(mmap->paused)
            {
                --IDX;
                mmap->paused = false;
                continue;
            }
        }
        return;
    }
}