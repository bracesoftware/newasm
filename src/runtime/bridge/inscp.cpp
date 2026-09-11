// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

module(ins_class_processor, {
    NewASM::Modules::PrintLine("Successfully loaded the instruction class processor/INSCP module.");
});

namespace newasm
{
    namespace compiler
    {
        ATTR_HOT void lineDataRT::Process(lineData& l)
        {
            //std::cout << "FIRST->Yo bro this crashed did it ?? -> " << l.Class - 1 << std::endl;
            if(this->Processor)
            {
                this->Processor(l);
                return;
            }
            if(Const::InstructionClass::ClassProcessors[l.Class])
            {
                Const::InstructionClass::ClassProcessors[l.Class](l);
                return;
            }
            newasm::SetExceptionComment("critical issue with the instruction dispatch; instruction class -> " + newasm::_std::to_string(l.Class));
            newasm::terminate(newasm::exit_codes::unknown_inscp);
            return;
        }
    }

    inline void TinyCondProc(compiler::lineData& line)
    {
        newasm::header::data::case_line = line.other;
        newasm::process_i(line.raw, line.tokens.at(0), line);
        return;
    }
    
    inline void MediumCondProc(compiler::lineData& line)
    {
        newasm::header::data::case_line = line.other;
        newasm::process_is(line.raw, line.tokens.at(0), line.tokens.at(1), line);
        return;
    }

    namespace ApiBridge
    {
        ATTR_HOT inline void procCallStackArgs(compiler::lineData& line, std::string& operand)
        {
            if(!line.AltStackArg) return;
            if(newasm::header::data::proc_now)
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
                        return;
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
                        return;
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
            }
            return;
        }
    }

    inline void LargeInsProc(compiler::lineData& line)
    {
        auto operand = line.tokens.at(2);
        newasm::ApiBridge::procCallStackArgs(line, operand);
        newasm::process_iso(line.raw, line.tokens.at(0), line.tokens.at(1), operand, line);
        return;
    }

    inline void MediumInsProc(compiler::lineData& line)
    {
        newasm::process_is(line.raw, line.tokens.at(0), line.tokens.at(1), line);
        return;
    }

    inline void TinyInsProc(compiler::lineData& line)
    {
        static std::string libname;
        if(line.whatAmIDoing == INS_EXTERNAL)
        {
            libname = line.tokens.at(0);
            for(int i = 0; i < newasm::dynlib::mem::invalid_dynlibs.size(); ++i)
            {
                if(libname == newasm::dynlib::mem::invalid_dynlibs.at(i))
                {
                    newasm::terminate(newasm::exit_codes::improper_dynlib);
                    return;
                }
            }
            auto& l = newasm::mem::instructions[libname];
            for(int i = 0; i < l.size(); ++i)
            {
                if(newasm::system::terminated)
                {
                    return;
                }
                auto JIT_COMPILE = newasm::compiler::DO(l.at(i));
                newasm::procline(JIT_COMPILE);
            }
            return;
        }
        newasm::process_i(line.raw, line.tokens.at(0), line);
        return;
    }
}