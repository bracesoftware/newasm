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

Portions created by the Initial Developer are Copyright (c) 2022
the Initial Developer. All Rights Reserved.

*/


namespace newasm
{
    void terminate(int exit_code)//, std::string line)
    {
        //newasm::header::data::lastln = line;
        newasm::mem::regs::exc = exit_code;
        newasm::system::terminated = true;
        std::cout << std::endl;
        newasm::header::functions::info(newasm::constv::pxstr + std::to_string(newasm::mem::regs::exc));
        if(newasm::header::data::exception) 
            std::cout <<
            "\t\t\t\t\t" <<
            newasm::header::col::magenta <<
            "Exception in " <<
            newasm::header::col::gray <<
            newasm::header::style::bold <<
            newasm::header::style::underline <<
            newasm::header::settings::script_file <<
            ":" <<
            newasm::header::data::lastlndx <<
            newasm::header::col::reset <<
            newasm::header::col::magenta <<
            " >> " <<
            newasm::header::col::gray <<
            newasm::header::data::lastln << 
            std::endl;
        if(newasm::header::data::exception) std::cout << newasm::header::col::reset;
    }
    void callproc(std::string name);
    int process_s(std::string wholeline, std::string stat, std::string arg)
    {
        if(stat == static_cast<std::string>("_"))
        {
            if(arg == static_cast<std::string>("data"))
            {
                newasm::system::section = newasm::constv::__data;
                return 1;
            }
            if(arg == static_cast<std::string>("start"))
            {
                newasm::system::section = newasm::constv::__start;
                return 1;
            }
        }
        return 1;
    }
    int process_l(std::string wholeline, std::string stat, std::string arg, int lineidx)
    {
        if(stat == static_cast<std::string>("_"))
        {
            if(newasm::mem::functions::datavalid(arg,newasm::mem::labels))
            {
                newasm::terminate(newasm::exit_codes::label_redef);//,wholeline);
                return 1;
            }
            newasm::mem::labels[arg] = lineidx;
            return 1;
        }
        return 1;
    }
    int process_d(std::string wholeline, std::string dtyp, std::string name, std::string value)
    {
        if(newasm::system::section != newasm::constv::__data)
        {
            newasm::terminate(newasm::exit_codes::invalid_section);//,wholeline);
            return 1;
        }
        //std::cout << dtyp << ":" << name << ":" << value << std::endl;
        if(newasm::header::functions::isalphanum(name))
        {
            if(dtyp == static_cast<std::string>("num"))
            {
                if(!newasm::header::functions::isnumeric(value))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::datatypes[name] = newasm::datatypes::number;
                newasm::mem::data[name] = value;
                return 1;
            }
            if(dtyp == static_cast<std::string>("decm"))
            {
                if(!newasm::header::functions::isfloat(value))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::datatypes[name] = newasm::datatypes::decimal;
                newasm::mem::data[name] = value;
                return 1;
            }
            if(dtyp == static_cast<std::string>("txt"))
            {
                if(!newasm::header::functions::istext(value))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::header::functions::remq(value);
                newasm::mem::datatypes[name] = newasm::datatypes::text;
                newasm::mem::data[name] = value;
                return 1;
            }
        }
        return 1;
    }
    int process_iso(std::string wholeline, std::string ins, std::string suf, std::string opr)
    {
        if(newasm::system::section != newasm::constv::__start)
        {
            newasm::terminate(newasm::exit_codes::invalid_section);//,wholeline);
            return 1;
        }
        if(opr == newasm::header::constants::inv_reg_val)
        {
            newasm::terminate(newasm::exit_codes::mem_overflow);//,wholeline);
            return 1;
        }

        //end
        if(ins == static_cast<std::string>("end"))
        {
            if(suf == static_cast<std::string>("0"))
            {
                if(opr == static_cast<std::string>("0"))
                {
                    newasm::system::stop = 0;
                    //std::cout << "Finished proc: " << newasm::system::cproc << std::endl;
                    return 1;
                }
            }
        }
        if(newasm::system::stop == 1)
        {
            std::string newline = ins + static_cast<std::string>(".") + suf + static_cast<std::string>(",") + opr;
            newasm::mem::funcs[newasm::system::cproc].push_back(newline);
            //std::cout << newasm::system::cproc << " : " << newline << std::endl;
            return 1;
        }
        // STOR
        if(ins == static_cast<std::string>("stor"))
        {
            if(!newasm::header::functions::isalphanum(opr))
            {
                return 1;
            }
            if(!newasm::mem::functions::datavalid(opr,newasm::mem::data))
            {
                return 1;
            }

            if(suf == static_cast<std::string>("fdx"))
            {
                if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::data[opr] = std::to_string(newasm::mem::regs::fdx);
                return 1;
            }
            if(suf == static_cast<std::string>("tlr"))
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
                if(newasm::mem::datatypes[opr] != newasm::datatypes::text)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::data[opr] = newasm::mem::regs::tlr;
                return 1;
            }
            if(suf == static_cast<std::string>("stl"))
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
                if(newasm::mem::datatypes[opr] != newasm::datatypes::text)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::data[opr] = newasm::mem::regs::stl;
                return 1;
            }
            if(suf == static_cast<std::string>("psx"))
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
                if(newasm::mem::datatypes[opr] != newasm::datatypes::text)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::data[opr] = newasm::mem::regs::psx;
                return 1;
            }
            if(suf == static_cast<std::string>("stk"))
            {
                if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::data[opr] = std::to_string(newasm::mem::regs::stk);
                return 1;
            }
            if(suf == static_cast<std::string>("hea"))
            {
                if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::data[opr] = std::to_string(newasm::mem::regs::hea);
                return 1;
            }
        }
        //sysreq
        if(ins == static_cast<std::string>("sysreq"))
        {
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
                if(!newasm::mem::functions::datavalid(opr,newasm::mem::data))
                {
                    //std::cout << opr << " not found <data>" << std::endl;
                    newasm::terminate(newasm::exit_codes::sysreq_fail);//,wholeline);
                    return 1;
                }
                return 1;
            }
        }
        //pop
        if(ins == static_cast<std::string>("pop"))
        {
            if(suf == static_cast<std::string>("0"))
            {
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
                if(!newasm::mem::functions::datavalid(opr,newasm::mem::data))
                {
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
                newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                return 1;
            }
        }
        newasm::header::functions::parseopr(opr, newasm::mem::data);
        // RETURN
        if(ins == static_cast<std::string>("retn"))
        {
            if(suf == static_cast<std::string>("0"))
            {
                if(newasm::header::functions::isnumeric(opr))
                {
                    newasm::mem::regs::exc = std::stoi(opr);
                    newasm::header::data::exception = false;
                    newasm::terminate(newasm::mem::regs::exc);//,wholeline);
                }
                else
                {
                    newasm::mem::regs::exc = newasm::exit_codes::invalid_retn;
                    newasm::terminate(newasm::mem::regs::exc);//,wholeline);
                }
                return 1;
            }
        }
        if(ins == static_cast<std::string>("ret"))
        {
            if(opr == static_cast<std::string>("0"))
            {
                if(suf == static_cast<std::string>("fdx"))
                {
                    newasm::header::data::exception = false;
                    newasm::terminate(newasm::mem::regs::fdx);//,wholeline);
                    return 1;
                }
                if(suf == static_cast<std::string>("tlr"))
                {
                    if(newasm::header::functions::isnumeric(newasm::mem::regs::tlr))
                    {
                        newasm::header::data::exception = false;
                        newasm::terminate(std::stoi(newasm::mem::regs::tlr));//,wholeline);
                        return 1;
                    }
                }
                if(suf == static_cast<std::string>("stl"))
                {
                    if(newasm::header::functions::isnumeric(newasm::mem::regs::stl))
                    {
                        newasm::header::data::exception = false;
                        newasm::terminate(std::stoi(newasm::mem::regs::stl));//,wholeline);
                        return 1;
                    }
                }
                if(suf == static_cast<std::string>("psx"))
                {
                    if(newasm::header::functions::isnumeric(newasm::mem::regs::psx))
                    {
                        newasm::header::data::exception = false;
                        newasm::terminate(std::stoi(newasm::mem::regs::psx));//,wholeline);
                        return 1;
                    }
                }
                if(suf == static_cast<std::string>("stk"))
                {
                    newasm::header::data::exception = false;
                    newasm::terminate(newasm::mem::regs::stk);//,wholeline);
                    return 1;
                }
                if(suf == static_cast<std::string>("hea"))
                {
                    newasm::header::data::exception = false;
                    newasm::terminate(newasm::mem::regs::hea);//,wholeline);
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::invalid_retn);//,wholeline);
                return 1;
            }
        }
        // MOV
        if(ins == static_cast<std::string>("mov"))
        {
            if(suf == static_cast<std::string>("fdx"))
            {
                if(!newasm::header::functions::isnumeric(opr))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::regs::fdx = std::stoi(opr);
                return 1;
            }
            if(suf == static_cast<std::string>("tlr"))
            {
                newasm::mem::regs::tlr = (opr);
                return 1;
            }
            if(suf == static_cast<std::string>("stl"))
            {
                newasm::mem::regs::stl = (opr);
                return 1;
            }
            if(suf == static_cast<std::string>("psx"))
            {
                newasm::mem::regs::psx = (opr);
                return 1;
            }
            if(suf == static_cast<std::string>("stk"))
            {
                if(!newasm::header::functions::isnumeric(opr))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::regs::stk = std::stoi(opr);
                return 1;
            }
            if(suf == static_cast<std::string>("hea"))
            {
                if(!newasm::header::functions::isnumeric(opr))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::regs::hea = std::stoi(opr);
                return 1;
            }
        }
        // syscall
        if(ins == static_cast<std::string>("syscall"))
        {
            if(suf == static_cast<std::string>("0"))
            {
                if(opr == static_cast<std::string>("\%ios"))
                {
                    if(newasm::mem::regs::fdx == 1)
                    {
                        if(!newasm::header::functions::istext(newasm::mem::regs::tlr))
                        {
                            using std::endl, std::cout;
                            cout << "tlr is: " << newasm::mem::regs::tlr << endl;
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        newasm::mem::regs::tlr = newasm::header::functions::remq(newasm::mem::regs::tlr);
                        std::cout << newasm::mem::regs::tlr;// << std::endl;
                        newasm::mem::functions::out_bopr(newasm::mem::regs::stl);
                        return 1;
                    }
                    if(newasm::mem::regs::fdx == 2)
                    {
                        if(!newasm::header::functions::isnumeric(newasm::mem::regs::tlr) && !newasm::header::functions::isfloat(newasm::mem::regs::tlr))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
                        std::cout << newasm::mem::regs::tlr;// << std::endl;
                        newasm::mem::functions::out_bopr(newasm::mem::regs::stl);
                        return 1;
                    }
                    if(newasm::mem::regs::fdx == 3)
                    {
        	            std::getline(std::cin, newasm::mem::regs::tlr);
                        newasm::mem::regs::tlr = static_cast<std::string>("\"") + newasm::mem::regs::tlr + static_cast<std::string>("\"");
                        return 1;
                    }
                    if(newasm::mem::regs::fdx == 4)
                    {
                        std::cin >> newasm::mem::regs::tlr;
                        if(!newasm::header::functions::isnumeric(newasm::mem::regs::tlr) && !newasm::header::functions::isfloat(newasm::mem::regs::tlr))
                        {
                            newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                            return 1;
                        }
        	            return 1;
                    }
                    if(newasm::mem::regs::fdx == 5)
                    {
                        newasm::mem::functions::out_bopr(newasm::mem::regs::tlr);
                        return 1;
                    }
                }
            }
        }
        //proc
        if(ins == static_cast<std::string>("proc"))
        {
            if(suf == static_cast<std::string>("0"))
            {
                if(newasm::header::functions::isalphanum(opr))
                {
                    if(newasm::mem::functions::datavalid(opr,newasm::mem::funcs))
                    {
                        newasm::terminate(newasm::exit_codes::proc_redef);
                        return 1;
                    }
                    newasm::system::stop = 1;
                    newasm::system::cproc = opr;
                    //std::cout << "Creating proc: " << opr << std::endl;
                    return 1;
                }
            }
        }
        //call
        if(ins == static_cast<std::string>("call"))
        {
            if(suf == static_cast<std::string>("0"))
            {
                if(newasm::header::functions::isalphanum(opr))
                {
                    if(!newasm::mem::functions::datavalid(opr,newasm::mem::funcs))
                    {
                        newasm::terminate(newasm::exit_codes::invalid_proc);
                        return 1;
                    }
                    newasm::callproc(opr);
                    return 1;
                }
            }
        }
        //rem
        if(ins == static_cast<std::string>("rem"))
        {
            if(suf == static_cast<std::string>("0"))
            {
                if(newasm::header::functions::istext(opr))
                {
                    //do nothing,it's a comment
                    return 1;
                }
            }
        }
        //nop
        if(ins == static_cast<std::string>("nop"))
        {
            if(suf == static_cast<std::string>("0"))
            {
                if(opr == static_cast<std::string>("0"))
                {
                    return 1;//newasm::header::functions::wrn("Encountered an empty comment.");
                }
            }
        }
        //halt
        if(ins == static_cast<std::string>("halt"))
        {
            if(suf == static_cast<std::string>("proc"))
            {
                newasm::system::stoproc = 1;
                newasm::mem::regs::psx = (opr);
                return 1;
            }
        }
        //push
        if(ins == static_cast<std::string>("push"))
        {
            if(suf == static_cast<std::string>("0"))
            {
                if(newasm::mem::functions::check_stkhea_col())
                {
                    newasm::terminate(newasm::exit_codes::stkhea_col);//,wholeline);
                    return 1;
                }
                newasm::mem::program_memory[newasm::mem::regs::stk] = opr;
                newasm::mem::regs::stk = newasm::mem::regs::stk - 1;
                return 1;
            }
        }
        //jmp
        if(ins == static_cast<std::string>("jmp"))
        {
            if(suf == static_cast<std::string>("0"))
            {
                if(!newasm::mem::functions::datavalid(opr, newasm::mem::labels))
                {
                    newasm::terminate(newasm::exit_codes::bus_err);//,wholeline);
                    return 1;
                }
                newasm::code_stream::jump = 1;
                newasm::code_stream::jumpto = 1 + newasm::mem::labels[opr];
                return 1;
            }
        }
        //heap
        if(ins == static_cast<std::string>("heap"))
        {
            if(suf == static_cast<std::string>("0"))
            {
                if(newasm::header::functions::isnumeric(opr))
                {
                    newasm::mem::regs::hea = newasm::mem::regs::hea + std::stoi(opr);
                    if(newasm::mem::regs::hea > newasm::mem::inf::mem_size - 1)
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
            }
        }
        //zero
        if(ins == static_cast<std::string>("zero"))
        {
            if(opr == static_cast<std::string>("0"))
            {
                if(suf == static_cast<std::string>("fdx"))
                {
                    newasm::mem::regs::fdx = 0;
                    return 1;
                }
                if(suf == static_cast<std::string>("tlr"))
                {
                    newasm::mem::regs::tlr = newasm::header::constants::inv_reg_val;
                    return 1;
                }
                if(suf == static_cast<std::string>("stl"))
                {
                    newasm::mem::regs::stl = newasm::header::constants::inv_reg_val;
                    return 1;
                }
                if(suf == static_cast<std::string>("stk"))
                {
                    newasm::mem::regs::stk = 0;
                    newasm::terminate(newasm::exit_codes::stkhea_col);//,wholeline); // Why'd you touch STK in the first place?
                    return 1;
                }
                if(suf == static_cast<std::string>("hea"))
                {
                    newasm::mem::regs::hea = 0; // This is fine, easy way to get rid of all memory.
                    return 1;
                }
                if(suf == static_cast<std::string>("psx"))
                {
                    newasm::mem::regs::psx = newasm::header::constants::inv_reg_val;
                    return 1;
                }
            }
        }
        //std::cout << ins << "." << suf << "," << opr;
        newasm::terminate(newasm::exit_codes::invalid_ins);//,wholeline);
        return 1;
    }
    
    void procline(std::string &line)
    {
        line = newasm::header::functions::remc(line);
        std::string ins, suf, opr, stat, arg, dtyp;
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

        if(newasm::header::functions::strfind(line,':'))
        {
            tmp2 = newasm::header::functions::split(line, ':');
            stat = tmp2[0];
            arg = tmp2[1];
            stat = newasm::header::functions::trim(stat);
            arg = newasm::header::functions::trim(arg);
        }
        if(newasm::header::functions::strfind(line,'$')) if(newasm::header::functions::strfind(line,'='))
        {
            tmp2 = newasm::header::functions::split(line, '$');
            tmp3 = newasm::header::functions::split(tmp2[1], '=');
            dtyp = tmp2[0];
            stat = tmp3[0];
            arg = tmp3[1];
            dtyp = newasm::header::functions::trim(dtyp);
            stat = newasm::header::functions::trim(stat);
            arg = newasm::header::functions::trim(arg);
        }
        if(newasm::header::functions::strfind(line,'.')) if(newasm::header::functions::strfind(line,','))
        {
            tmp = newasm::header::functions::split_fixed(line, '.', newasm::header::constants::max_tokens);
            tmp1 = newasm::header::functions::split_fixed(tmp[1], ',', newasm::header::constants::max_tokens);
            ins = tmp[0];
            suf = tmp1[0];
            opr = tmp1[1];
            ins = newasm::header::functions::trim(ins);
            suf = newasm::header::functions::trim(suf);
            opr = newasm::header::functions::trim(opr);
        }
        
        if(newasm::header::functions::strfind(line,':'))
            newasm::process_s(line,stat,arg);
        if(newasm::header::functions::strfind(line,'$'))
            if(newasm::header::functions::strfind(line,'='))
                newasm::process_d(line,dtyp,stat,arg);
        if(newasm::header::functions::strfind(line,'.'))
            if(newasm::header::functions::strfind(line,','))
                newasm::process_iso(line,ins,suf,opr);
    }
    void analyzeline(std::string &line, int lineidx)
    {
        std::string ins, suf, opr, stat, arg, dtyp;
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

        if(lineidx != newasm::code_stream::invalid_lnidx) if(newasm::header::functions::strfind(line,'!'))
        {
            tmp2 = newasm::header::functions::split(line, '!');
            stat = tmp2[0];
            arg = tmp2[1];
            stat = newasm::header::functions::trim(stat);
            arg = newasm::header::functions::trim(arg);
        }
        
        if(lineidx != newasm::code_stream::invalid_lnidx) 
            if(newasm::header::functions::strfind(line,'!'))
                newasm::process_l(line,stat,arg,lineidx);
    }
    void callproc(std::string name)
    {
        newasm::system::stoproc = 0;
        auto it = newasm::mem::funcs.find(name);
        if(it != newasm::mem::funcs.end())
        {
            for(std::string &line : it->second)
            {
                if(newasm::system::stoproc == 1)
                {
                    newasm::system::stoproc = 0;
                    break;
                }
                newasm::procline(line);
                //std::cout << "Executing : " << line << std::endl;
            }
        }
    }
    int analyze(std::string file)
    {
        std::ifstream internal_fileobject(newasm::header::constants::scripts_folder + file);
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
                static_cast<std::string>("Unable to open the file : ") + static_cast<std::string>("'") + 
                newasm::header::constants::scripts_folder + file + static_cast<std::string>("'"));
        }
        return 0;
    }
    int execute(std::string file, int startline, int proc)
    {
        if(proc == 0)
        {
            return proc;
        }
        std::ifstream internal_fileobject(newasm::header::constants::scripts_folder + file);
        if(internal_fileobject.is_open())
        {
            std::string line;
            int lineidx = 1;

            while(std::getline(internal_fileobject, line))
            {
                if(startline != newasm::code_stream::invalid_lnidx) if(!(lineidx >= startline))
                {
                    lineidx++;
                    continue;
                }
                if(newasm::system::terminated)
                {
                    break;
                }
                if(newasm::code_stream::jump == 1)
                {
                    newasm::code_stream::jump = 0;
                    newasm::execute(file, newasm::code_stream::jumpto, 1);
                    break;
                }

                newasm::header::data::lastln = line;
                newasm::header::data::lastlndx = lineidx;
                newasm::procline(line);
                lineidx++;
            }
            internal_fileobject.close();
            if(!newasm::system::terminated)
            {
                newasm::terminate(newasm::exit_codes::noterm_point);//,static_cast<std::string>("null"));
            }
            return 1;
        }
        else
        {
            newasm::header::functions::err(
                static_cast<std::string>("Unable to open the file : ") + static_cast<std::string>("'") + 
                newasm::header::constants::scripts_folder + file + static_cast<std::string>("'"));
        }
        return 0;
    }
}