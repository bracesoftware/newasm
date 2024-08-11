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
    int process_s(bool &valid, std::string wholeline, std::string stat, std::string arg)
    {
        valid = false;
        if(stat == static_cast<std::string>("_"))
        {
            if(arg == static_cast<std::string>("config"))
            {
                newasm::system::section = newasm::code_stream::sections::config;
                valid = true;
                return 1;
            }
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
            newasm::terminate(newasm::exit_codes::invalid_section);
            return 1;
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
        if(opr == newasm::header::constants::inv_reg_val)
        {
            newasm::terminate(newasm::exit_codes::mem_overflow);//,wholeline);
            return 1;
        }

        if(newasm::system::stop == 1)
        {
            std::string newline = ins + static_cast<std::string>(".") + suf + static_cast<std::string>(",") + opr;
            newasm::mem::funcs[newasm::system::cproc].push_back(newline);
            //std::cout << newasm::system::cproc << " : " << newline << std::endl;
            return 1;
        }
        //LOAD.ref
        if(ins == static_cast<std::string>("load"))
        {
            if(suf == static_cast<std::string>("ref"))
            {
                if(!newasm::header::functions::isalphanum(opr))
                {
                    return 1;
                }
                if(!newasm::mem::functions::datavalid(opr,newasm::mem::data))
                {
                    return 1;
                }

                if(newasm::header::functions::isnumeric
                (
                    newasm::mem::program_memory
                    [
                        newasm::mem::regs::heaptr
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
                        newasm::mem::regs::heaptr
                    ];
                    return 1;
                }
                if(newasm::header::functions::isfloat
                (
                    newasm::mem::program_memory
                    [
                        newasm::mem::regs::heaptr
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
                        newasm::mem::regs::heaptr
                    ];
                    return 1;
                }
                if(newasm::header::functions::istext
                (
                    newasm::mem::program_memory
                    [
                        newasm::mem::regs::heaptr
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
                        newasm::mem::regs::heaptr
                    ];
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                return 1;
            }
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
                newasm::mem::data[opr] = std::to_string(newasm::mem::regs::heaptr);
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
                using std::cout, std::endl;
                //cout << "\n\t >> tlr set to: " << opr << endl;
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
                newasm::mem::regs::heaptr = std::stoi(opr);
                if(newasm::mem::regs::heaptr > newasm::mem::regs::hea)
                {
                    newasm::terminate(newasm::exit_codes::invalid_memacc);
                }
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
                            //cout << "tlr is: " << newasm::mem::regs::tlr << endl;
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
                    newasm::mem::regs::heaptr = newasm::mem::regs::hea;
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
        //load.adr
        if(ins == static_cast<std::string>("load"))
        {
            if(suf == static_cast<std::string>("adr"))
            {
                if(!newasm::header::functions::isnumeric(opr) && !newasm::header::functions::isfloat(opr) && !newasm::header::functions::istext(opr))
                {
                    //std::cout << "opr is " << opr << std::endl;
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }
                newasm::mem::program_memory[newasm::mem::regs::heaptr] = opr;
                return 1;
            }
        }
        
        //std::cout << ins << "." << suf << "," << opr;
        newasm::terminate(newasm::exit_codes::invalid_ins);//,wholeline);
        return 1;
    }
    int process_is(std::string line, std::string ins, std::string suf)
    {
        if(newasm::system::stop == 1)
        {
            std::string newline = ins + static_cast<std::string>(".") + suf;
            newasm::mem::funcs[newasm::system::cproc].push_back(newline);
            //std::cout << newasm::system::cproc << " : " << newline << std::endl;
            return 1;
        }
        //ret
        if(ins == static_cast<std::string>("ret"))
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
                newasm::terminate(newasm::mem::regs::heaptr);//,wholeline);
                return 1;
            }
            newasm::terminate(newasm::exit_codes::invalid_retn);//,wholeline);
            return 1;
        }
        
        //zero
        if(ins == static_cast<std::string>("zero"))
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
                newasm::mem::regs::heaptr = 0;
                return 1;
            }
            if(suf == static_cast<std::string>("psx"))
            {
                newasm::mem::regs::psx = newasm::header::constants::inv_reg_val;
                return 1;
            }
        }
        newasm::terminate(newasm::exit_codes::invalid_ins);//,wholeline);
        return 1;
    }
    int process_i(std::string line, std::string ins)
    {
        //end
        if(ins == static_cast<std::string>("end"))
        {
            newasm::system::stop = 0;
            //std::cout << "Finished proc: " << newasm::system::cproc << std::endl;
            return 1;
        }
        if(newasm::system::stop == 1)
        {
            std::string newline = ins;
            newasm::mem::funcs[newasm::system::cproc].push_back(newline);
            //std::cout << newasm::system::cproc << " : " << newline << std::endl;
            return 1;
        }
        if(ins == static_cast<std::string>("nop"))
        {
            // do nothing
            return 1;
        }
        newasm::terminate(newasm::exit_codes::invalid_ins);//,wholeline);
        return 1;
    }
    int process_c(std::string wholeline, std::string stat, std::string arg)
    {
        if(stat == static_cast<std::string>("memsize"))
        {
            if(!newasm::header::functions::isnumeric(arg))
            {
                newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                return 1;
            }
            if(!newasm::mem::functions::setup_memsize(std::stoi(arg)))
            {
                newasm::terminate(newasm::exit_codes::invalid_config);
                return 1;
            }
            return 1;
        }
        newasm::terminate(newasm::exit_codes::invalid_config);
        return 1;
    }
    int procline(std::string &line)
    {
        line = newasm::header::functions::trim(line);
        if(line[0] == ';')
        {
            return 0;
        }
        if(line.empty())
        {
            return 0;
        }
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
            bool valid = false;
            tmp2 = newasm::header::functions::split_fixed(line, ':');
            stat = tmp2[0];
            arg = tmp2[1];
            stat = newasm::header::functions::trim(stat);
            arg = newasm::header::functions::trim(arg);
            newasm::process_s(valid,line,stat,arg);
            if(valid)
            {
                return 1;
            }
        }
        if(newasm::system::section == newasm::code_stream::sections::config)
        {
            if(line.find('@') == std::string::npos)
            {
                newasm::terminate(newasm::exit_codes::invalid_syntax);
                return 1;
            }
            tmp = newasm::header::functions::split_fixed(line, '@');
            stat = tmp[0];
            arg = tmp[1];
            stat = newasm::header::functions::trim(stat);
            arg = newasm::header::functions::trim(arg);
            return newasm::process_c(line,stat,arg);
        }
        if(newasm::system::section == newasm::code_stream::sections::data)
        {
            if(line.find('$') == std::string::npos || line.find('=') == std::string::npos)
            {
                newasm::terminate(newasm::exit_codes::invalid_syntax);
                return 1;
            }
            if(line.find('$') > line.find('='))
            {
                newasm::terminate(newasm::exit_codes::invalid_syntax);
                return 1;
            }
            tmp2 = newasm::header::functions::split_fixed(line, '$');
            tmp3 = newasm::header::functions::split_fixed(tmp2[1], '=');
            dtyp = tmp2[0];
            stat = tmp3[0];
            arg = tmp3[1];
            dtyp = newasm::header::functions::trim(dtyp);
            stat = newasm::header::functions::trim(stat);
            arg = newasm::header::functions::trim(arg);
            return newasm::process_d(line,dtyp,stat,arg);
        }
        if(newasm::system::section == newasm::code_stream::sections::start)
        {
            if(newasm::header::functions::strfind(line,'.')) if(newasm::header::functions::strfind(line,','))
            {
                if(line.find('.') > line.find(','))
                {
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }
                tmp = newasm::header::functions::split_fixed(line, '.');
                tmp1 = newasm::header::functions::split_fixed(tmp[1], ',');
                ins = tmp[0];
                suf = tmp1[0];
                opr = tmp1[1];
                ins = newasm::header::functions::trim(ins);
                suf = newasm::header::functions::trim(suf);
                opr = newasm::header::functions::trim(opr);
                return newasm::process_iso(line,ins,suf,opr);
            }
            if(newasm::header::functions::strfind(line,'.')) if(!newasm::header::functions::strfind(line,','))
            {
                tmp = newasm::header::functions::split_fixed(line, '.');
                ins = tmp[0];
                suf = tmp[1];
                ins = newasm::header::functions::trim(ins);
                suf = newasm::header::functions::trim(suf);
                return newasm::process_is(line,ins,suf);
            }
            if(!newasm::header::functions::strfind(line,'.')) if(!newasm::header::functions::strfind(line,','))
            {
                ins = line;
                ins = newasm::header::functions::trim(ins);
                return newasm::process_i(line,ins);
            }
        }
        
        /*if(!newasm::header::functions::strfind(line,'.'))
            if(!newasm::header::functions::strfind(line,','))
                if(newasm::header::functions::strfind(line,':'))
                    return newasm::process_s(line,stat,arg);*/

        /*if(!newasm::header::functions::strfind(line,'.'))
            if(!newasm::header::functions::strfind(line,','))
                if(newasm::header::functions::strfind(line,'$'))
                    if(newasm::header::functions::strfind(line,'='))
                        return newasm::process_d(line,dtyp,stat,arg);*/
        
        /*if(newasm::header::functions::strfind(line,'.'))
            if(newasm::header::functions::strfind(line,','))
                return newasm::process_iso(line,ins,suf,opr);
        if(newasm::header::functions::strfind(line,'.'))
            if(!newasm::header::functions::strfind(line,','))
                return newasm::process_is(line,ins,suf);
        if(!newasm::header::functions::strfind(line,'.'))
            if(!newasm::header::functions::strfind(line,','))
                return newasm::process_i(line,ins);*/
        newasm::terminate(newasm::exit_codes::invalid_syntax);
        return 0;
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
                if(newasm::mem::regs::hea != 0)
                {
                    newasm::terminate(newasm::exit_codes::memory_leak); // Learn to clean after yourself.
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::noterm_point); // You got to end your programs.
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