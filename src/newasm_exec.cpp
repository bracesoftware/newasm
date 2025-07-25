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


#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

namespace newasm
{
    int execute(std::string file, int startline, int proceed);
    void unsins(std::string ins)
    {
        newasm::header::functions::wrn(
            static_cast<std::string>("Instruction `") +
            newasm::header::style::underline+ins+newasm::header::col::reset
            +static_cast<std::string>("` is not supported in the REPL mode.")
        );
    }
    void unsins_repl(std::string ins)
    {
        newasm::header::functions::wrn(
            static_cast<std::string>("Instruction `") +
            newasm::header::style::underline+ins+newasm::header::col::reset
            +static_cast<std::string>("` is not supported outside the REPL mode.")
        );
    }
    //int redirect_exec(std::string filename);
    int terminate(int exit_code)//, std::string line)
    {
        //std::cout << "TERMINATEEE" << std::endl;
        if(newasm::header::data::repl)
        {
            newasm::header::data::exception = true;
            if(newasm::header::data::exception)
            {
                std::cout <<
                "\t\t\t" <<
                newasm::header::col::magenta <<
                "Exception \"" <<
                newasm::header::col::gray<<
                newasm::header::style::underline<<
                newasm::exit_codes::identifier.at(exit_code)<<
                newasm::header::col::reset<<
                newasm::header::col::magenta << "\" occured >> "<<
                newasm::header::col::gray<<
                newasm::header::data::lastln << std::endl;

                std::cout << newasm::header::col::reset << std::endl;
            }
            return 1;
        }
        //newasm::header::data::lastln = line;
        if(!newasm::system::terminated)
        {
            std::cout << std::endl;
            newasm::mem::regs::exc = exit_code;
            newasm::header::functions::info(newasm::constv::pxstr + std::to_string(newasm::mem::regs::exc));
        }
        
        newasm::system::terminated = true;

        if(newasm::header::data::exception)
        {
            std::cout <<
            "\t\t\t" <<
            newasm::header::col::magenta <<
            "Exception \"" <<
            newasm::header::col::gray<<
            newasm::header::style::underline<<
            newasm::exit_codes::identifier.at(exit_code)<<
            newasm::header::col::reset <<
            newasm::header::col::magenta <<
            "\" in ";

            if(newasm::header::execution_flow::exec_redirected == false) if(newasm::header::data::proc_now == false)
            {
                std::cout <<
                newasm::header::col::gray <<
                newasm::header::style::bold <<
                newasm::header::style::underline <<
                newasm::header::settings::script_file <<
                ":" <<
                newasm::header::data::lastlndx;
            }
            if(newasm::header::data::proc_now == true)
            {
                std::cout << "the procedure " <<
                newasm::header::col::gray <<
                newasm::header::style::bold <<
                newasm::header::style::underline <<
                newasm::mem::regs::prp;
            }
            if(newasm::header::execution_flow::exec_redirected) if(newasm::header::data::proc_now == false)
            {
                std::cout << "child process "<<
                newasm::header::col::gray <<
                newasm::header::style::bold <<
                newasm::header::style::underline <<
                newasm::header::execution_flow::file <<
                ":" <<
                newasm::header::data::lastlndx;
            }

            std::cout <<
            newasm::header::col::reset <<
            newasm::header::col::magenta <<
            " >> " <<
            newasm::header::col::gray <<
            newasm::header::data::lastln << 
            std::endl;

            std::cout << newasm::header::col::reset;
        }
        return 1;
    }
    int parseopr_struct(std::string& opr)
    {
        if(opr.find("@") != std::string::npos)
        {
            std::vector<std::string> tokens;
            std::string member_name;
            std::string struct_name;

            tokens = newasm::header::functions::split_fixed(opr, '@');
            member_name = tokens[0];
            struct_name = tokens[1];
            member_name = newasm::header::functions::trim(member_name);
            struct_name = newasm::header::functions::trim(struct_name);

            if(!newasm::mem::functions::datavalid(struct_name, newasm::mem::structs))
            {
                newasm::terminate(newasm::exit_codes::undefined_struct);
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
                newasm::terminate(newasm::exit_codes::undefined_structmem);
                return 1;
            }
            opr = i->value;
            return 1;
        }
        return 1;
    }
    void callproc(std::string name);
    int process_s(bool &valid, std::string wholeline, std::string stat, std::string arg)
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
        if(newasm::mem::functions::datavalid(name, newasm::mem::data))
        {
            newasm::terminate(newasm::exit_codes::var_redef);
            return 1;
        }
        if(newasm::mem::functions::datavalid(name, newasm::mem::structs))
        {
            newasm::terminate(newasm::exit_codes::struct_redef);
            return 1;
        }
        if(newasm::mem::functions::datavalid(name, newasm::threads::memory))
        {
            newasm::terminate(newasm::exit_codes::struct_redef);
            return 1;
        }
        if(newasm::header::functions::isalphanum(name))
        {
            if(dtyp == static_cast<std::string>("thread"))
            {
                if(value != static_cast<std::string>("{"))
                {
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }
                newasm::threads::thread_now = true;
                newasm::threads::thread_decl = name;
                newasm::threads::memory[name] = new newasm::threads::object__();
                newasm::threads::valid_threads.push_back(name);
                //newasm::threads::thread_count++;
                return 1;
            }
            if(dtyp == static_cast<std::string>("struct"))
            {
                if(newasm::header::data::struct_now)
                {
                    newasm::terminate(newasm::exit_codes::nested_struct);
                    return 1;
                }
                if(value != static_cast<std::string>("{"))
                {
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }
                newasm::header::data::struct_now = true;
                newasm::header::data::struct_decl = name;
                //newasm::mem::structs[newasm::header::data::struct_decl].push_back({0,"?","?"});
                //std::cout << "Struct made: " << newasm::header::data::struct_decl << "\n";
                //if(!newasm::mem::functions::datavalid())
                return 1;
            }
            if(dtyp == static_cast<std::string>("num"))
            {
                if(!newasm::header::functions::isnumeric(value))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                if(newasm::header::data::struct_now)
                {
                    newasm::mem::structs[newasm::header::data::struct_decl].push_back({newasm::datatypes::number, name, value});
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
                if(newasm::header::data::struct_now)
                {
                    newasm::mem::structs[newasm::header::data::struct_decl].push_back({newasm::datatypes::decimal, name, value});
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
                //newasm::header::functions::remq(value);
                if(newasm::header::data::struct_now)
                {
                    newasm::mem::structs[newasm::header::data::struct_decl].push_back({newasm::datatypes::text, name, value});
                    return 1;
                }
                newasm::mem::datatypes[name] = newasm::datatypes::text;
                newasm::mem::data[name] = value;
                return 1;
            }
            if(dtyp == static_cast<std::string>("ref"))
            {
                if(!newasm::header::functions::isref(value) && value != static_cast<std::string>("&\%null"))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                if(!newasm::mem::functions::datavalid(
                newasm::header::functions::remamp(value), newasm::mem::data) 
                && !newasm::mem::functions::datavalid(
                newasm::header::functions::remamp(value), newasm::mem::funcs)
                && value != static_cast<std::string>("&\%null"))
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
                return 1;
            }
            if(dtyp == static_cast<std::string>("char"))
            {
                if(!newasm::header::functions::ischar(value))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                //newasm::header::functions::remsq(value);
                if(newasm::header::data::struct_now)
                {
                    newasm::mem::structs[newasm::header::data::struct_decl].push_back({newasm::datatypes::character, name, value});
                    return 1;
                }
                newasm::mem::datatypes[name] = newasm::datatypes::character;
                newasm::mem::data[name] = value;
                return 1;
            }

            if(dtyp == static_cast<std::string>("bit_arr"))
            {
                if(!newasm::header::functions::isnumeric(value))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                if(newasm::mem::functions::datavalid(name, newasm::containers::bit_arrays))
                {
                    newasm::terminate(newasm::exit_codes::datastruct_redef);
                    return 1;
                }
          
                newasm::containers::bit_arrays[name] = new newasm::containers::bit_array<512>();
                return 1;
            }

            if(dtyp == static_cast<std::string>("bin_tree"))
            {
                if(!newasm::header::functions::isnumeric(value))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                if(newasm::mem::functions::datavalid(name, newasm::containers::binary_trees))
                {
                    newasm::terminate(newasm::exit_codes::datastruct_redef);
                    return 1;
                }

                newasm::containers::binary_trees[name] = new newasm::containers::binary_tree<512>();
                newasm::containers::binary_trees.at(name)->set_at__(0,0);
                return 1;
            }
            
            newasm::terminate(newasm::exit_codes::invalid_syntax);
            return 1;
        }
        return 1;
    }
    int stor_structmem(const std::string &suf, const std::string &into)
    {
        std::vector<std::string> tokens;
        std::string member_name;
        std::string struct_name;

        tokens = newasm::header::functions::split_fixed(into, '@');
        member_name = tokens[0];
        struct_name = tokens[1];
        member_name = newasm::header::functions::trim(member_name);
        struct_name = newasm::header::functions::trim(struct_name);

        if(!newasm::mem::functions::datavalid(struct_name, newasm::mem::structs))
        {
            newasm::terminate(newasm::exit_codes::undefined_struct);
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
            newasm::terminate(newasm::exit_codes::undefined_structmem);
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
        if(suf == newasm::mem::regs::stk.identifier())
        {
            intreg = newasm::mem::regs::stk;
        }
        if(suf == newasm::mem::regs::heaptr.identifier())
        {
            intreg = newasm::mem::regs::heaptr;
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
        if(suf == newasm::mem::regs::stl.identifier())
        {
            strreg = newasm::mem::regs::stl;
        }
        if(suf == newasm::mem::regs::psx.identifier())
        {
            strreg = newasm::mem::regs::psx;
        }
        if(suf == newasm::mem::regs::prp.identifier())
        {
            strreg = newasm::mem::regs::prp;
        }
        if(suf == newasm::mem::regs::cpt.identifier())
        {
            strreg = newasm::mem::regs::cpt;
        }

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
            struct_member_id->value = std::to_string(intreg);
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
            struct_member_id->value = std::to_string(floatreg);
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
            newasm::terminate(newasm::exit_codes::nested_redirect);
            return 1;
        }

        return 1;
    }
    int process_iso(std::string wholeline, std::string ins, std::string suf, std::string opr)
    {
        if(newasm::header::flags::compexpr)
        {
            newasm::impl::eval(opr);
        }
        auto lambda = newasm::header::functions::is_lambda(opr);
        if(lambda.first)
        {
            if(lambda.second == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::proc))
            {
                //std::cout << "Called (LAMBDA.INS) << proc" << std::endl;
                newasm::lambda::lambda_now = true;
                newasm::lambda::GLOBAL.thread = false;
                if(newasm::thread_line)
                {
                    newasm::lambda::GLOBAL.thread = true;
                }
                if(!newasm::lambda::GLOBAL.contents.empty())
                {
                    newasm::lambda::GLOBAL.contents.clear();
                }
                newasm::lambda::GLOBAL.line = newasm::header::functions::form_iso(ins,suf,"");
                return 1;
            }
            newasm::terminate(newasm::exit_codes::invalid_exp);
            return 1;
        }
       

        if(newasm::system::stop == 1)
        {
            newasm::system::proclines ++;
            std::string newline = ins + static_cast<std::string>(" ") + suf + static_cast<std::string>(",") + opr;
            newasm::mem::funcs[newasm::system::cproc].push_back(newline);
            //std::cout << newasm::system::cproc << " : " << newline << std::endl;
            return 1;
        }
        //parse the operand
        newasm::runtime::functions::parse(opr);

        //__say
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::__say))
        {
            if(suf == static_cast<std::string>("0"))
            {
                if(!newasm::header::functions::istext(opr))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }
                std::cout << newasm::header::col::gray;newasm::header::functions::nullprint(newasm::tab + newasm::header::functions::remq(opr));
                std::cout << newasm::header::col::reset;
                return 1;
            }
        }
        //LOAD.ref
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::load))
        {
            if(suf == static_cast<std::string>("ref"))
            {
                if(!newasm::header::functions::isref(opr))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }
                opr = newasm::header::functions::remamp(opr);
                if(!newasm::mem::functions::datavalid(opr,newasm::mem::data))
                {
                    newasm::terminate(newasm::exit_codes::invalid_memacc);
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
                if(newasm::header::functions::isref
                (
                    newasm::mem::program_memory
                    [
                        newasm::mem::regs::heaptr
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
                        newasm::mem::regs::heaptr
                    ];
                    //auto i = std::find(newasm::mem::uninitialized_pointer.begin(), newasm::mem::uninitialized_pointer.end(), opr);
                    //if(i != newasm::mem::uninitialized_pointer.end()) newasm::mem::uninitialized_pointer.erase(i);
                    return 1;
                }
                if(newasm::header::functions::ischar
                (
                    newasm::mem::program_memory
                    [
                        newasm::mem::regs::heaptr
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
                        newasm::mem::regs::heaptr
                    ];
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                return 1;
            }
        }
        // STOR
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::stor))
        {
            if(!newasm::header::functions::isref(opr))
            {
                newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                return 1;
            }
            opr = newasm::header::functions::remamp(opr);
            if(newasm::header::functions::isrefat(opr) && !newasm::header::functions::istext(opr))
            {
                newasm::stor_structmem(suf, opr);
                return 1;
            }
            /*if(!newasm::header::functions::isalphanum(opr))
            {
                newasm::terminate(newasm::exit_codes::invalid_syntax);
                return 1;
            }*/
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

            if(suf == newasm::mem::regs::fdx.identifier())
            {
                if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::data[opr] = std::to_string(newasm::mem::regs::fdx);
                return 1;
            }
            if(suf == newasm::mem::regs::tlr.identifier())
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
            if(suf == newasm::mem::regs::tr0.identifier())
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
            if(suf == newasm::mem::regs::tr1.identifier())
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
            if(suf == newasm::mem::regs::stl.identifier())
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
            if(suf == newasm::mem::regs::psx.identifier())
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
            if(suf == newasm::mem::regs::stk.identifier())
            {
                if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::data[opr] = std::to_string(newasm::mem::regs::stk);
                return 1;
            }
            if(suf == newasm::mem::regs::heaptr.identifier())
            {
                if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::data[opr] = std::to_string(newasm::mem::regs::heaptr);
                return 1;
            }
            if(suf == newasm::mem::regs::prp.identifier())
            {
                if(newasm::mem::datatypes[opr] != newasm::datatypes::reference)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::data[opr] = newasm::mem::regs::prp;
                //auto i = std::find(newasm::mem::uninitialized_pointer.begin(), newasm::mem::uninitialized_pointer.end(), opr);
                //if(i != newasm::mem::uninitialized_pointer.end()) newasm::mem::uninitialized_pointer.erase(i);
                return 1;
            }
            if(suf == newasm::mem::regs::cpt.identifier())
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
            
            if(suf == newasm::mem::regs::cpr.identifier())
            {
                if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::data[opr] = std::to_string(newasm::mem::regs::cpr);
                return 1;
            }
            if(suf == newasm::mem::regs::cr0.identifier())
            {
                if(newasm::mem::datatypes[opr] == newasm::datatypes::number)
                {
                    newasm::mem::data[opr] = std::to_string(static_cast<int>(newasm::mem::regs::cr0));
                    return 1;
                }
                if(newasm::mem::datatypes[opr] == newasm::datatypes::decimal)
                {
                    newasm::mem::data[opr] = std::to_string(newasm::mem::regs::cr0);
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                return 1;
            }
            if(suf == newasm::mem::regs::cr1.identifier())
            {
                if(newasm::mem::datatypes[opr] == newasm::datatypes::number)
                {
                    newasm::mem::data[opr] = std::to_string(static_cast<int>(newasm::mem::regs::cr1));
                    return 1;
                }
                if(newasm::mem::datatypes[opr] == newasm::datatypes::decimal)
                {
                    newasm::mem::data[opr] = std::to_string(newasm::mem::regs::cr1);
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                return 1;
            }
            if(suf == newasm::mem::regs::br0.identifier())
            {
                if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::data[opr] = std::to_string(newasm::mem::regs::br0);
                return 1;
            }
            if(suf == newasm::mem::regs::br1.identifier())
            {
                if(newasm::mem::datatypes[opr] != newasm::datatypes::number)
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::data[opr] = std::to_string(newasm::mem::regs::br1);
                return 1;
            }
            newasm::terminate(newasm::exit_codes::bus_err);
            return 1;
        }
        //sysreq
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::sysreq))
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
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::pop))
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
                return 1;
            }
            newasm::terminate(newasm::exit_codes::invalid_syntax);
            return 1;
        }
        /*for(std::vector<std::string>::iterator i = newasm::mem::uninitialized_pointer.begin(); i != newasm::mem::uninitialized_pointer.end(); ++i)
        {
            if(*i == opr)
            {
                newasm::terminate(newasm::exit_codes::uninptr_usage);
                return 1;
            }
        }*/
        newasm::header::functions::parseopr(opr, newasm::mem::data);
        newasm::parseopr_struct(opr);
        if(opr == static_cast<std::string>("&\%null"))
        {
            newasm::terminate(newasm::exit_codes::uninptr_usage);
            return 1;
        }
        
        // MOV
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::mov))
        {
            if(suf == newasm::mem::regs::fdx.identifier())
            {
                if(!newasm::header::functions::isnumeric(opr))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::regs::fdx = std::stoi(opr);
                return 1;
            }
            if(suf == newasm::mem::regs::tlr.identifier())
            {
                //using std::cout, std::endl;
                //cout << "\n\t >> tlr set to: " << opr << endl;
                newasm::mem::regs::tlr = (opr);
                return 1;
            }
            if(suf == newasm::mem::regs::tr0.identifier())
            {
                //using std::cout, std::endl;
                //cout << "\n\t >> tlr set to: " << opr << endl;
                newasm::mem::regs::tr0 = (opr);
                return 1;
            }
            if(suf == newasm::mem::regs::tr1.identifier())
            {
                //using std::cout, std::endl;
                //cout << "\n\t >> tlr set to: " << opr << endl;
                newasm::mem::regs::tr1 = (opr);
                return 1;
            }
            if(suf == newasm::mem::regs::stl.identifier())
            {
                newasm::mem::regs::stl = (opr);
                return 1;
            }
            if(suf == newasm::mem::regs::psx.identifier())
            {
                newasm::mem::regs::psx = (opr);
                return 1;
            }
            if(suf == newasm::mem::regs::stk.identifier())
            {
                if(!newasm::header::functions::isnumeric(opr))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::regs::stk = std::stoi(opr);
                return 1;
            }
            if(suf == newasm::mem::regs::heaptr.identifier())
            {
                //malloc
                if(newasm::header::functions::isallocref(opr).first)
                {
                    if(newasm::allocation_data == nullptr) //NoAlloc
                    {
                        newasm::terminate(newasm::exit_codes::malloc_err);
                        return 1;
                    }
                    if(newasm::header::functions::isallocref(opr).second >= newasm::allocation_data->size) //AllocSizeExceeded
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    newasm::mem::regs::heaptr = 1+newasm::allocation_data->heapsize_new - newasm::allocation_data->size + newasm::header::functions::isallocref(opr).second;
                    return 1;
                }

                //standard heap adr alloc
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
            if(suf == newasm::mem::regs::prp.identifier()) //procedure pointer
            {
                if(!newasm::header::functions::isref(opr))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                if(!newasm::mem::functions::datavalid(
                newasm::header::functions::remamp(opr), newasm::mem::data) 
                && !newasm::mem::functions::datavalid(
                newasm::header::functions::remamp(opr), newasm::mem::funcs))
                {
                    newasm::terminate(newasm::exit_codes::invalid_memacc);
                    return 1;
                }
                newasm::mem::regs::prp = (opr);
                return 1;
            }
            if(suf == newasm::mem::regs::cpt.identifier()) //container pointer
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
            if(suf == newasm::mem::regs::cpr.identifier())
            {
                if(!newasm::header::functions::isnumeric(opr))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::regs::cpr = std::stoi(opr);
                return 1;
            }
            if(suf == newasm::mem::regs::cr0.identifier())
            {
                if(!newasm::header::functions::isnumeric(opr) && !newasm::header::functions::isfloat(opr))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::regs::cr0 = std::stof(opr);
                return 1;
            }
            if(suf == newasm::mem::regs::cr1.identifier())
            {
                if(!newasm::header::functions::isnumeric(opr) && !newasm::header::functions::isfloat(opr))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::regs::cr1 = std::stof(opr);
                return 1;
            }
            if(suf == newasm::mem::regs::br0.identifier())
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
            if(suf == newasm::mem::regs::br1.identifier())
            {
                if(!newasm::header::functions::isnumeric(opr))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                    return 1;
                }
                newasm::mem::regs::br1 = std::stoi(opr);
                return 1;
            }
            /*if(suf == static_cast<std::string>("mcd"))
            {
                if(opr != static_cast<std::string>("\%own") && opr != static_cast<std::string>("\%orn"))
                {
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }
                if(opr == static_cast<std::string>("\%own"))
                {
                    newasm::mem::regs::mcd = newasm::datatypes::number;
                }
                if(opr == static_cast<std::string>("\%orn"))
                {
                    newasm::mem::regs::mcd = newasm::datatypes::decimal;
                }
                return 1;
            }*/
            newasm::terminate(newasm::exit_codes::bus_err);
            return 1;
        }
        
        
        
        //rem
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::rem))
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
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::halt))
        {
            if(suf == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::proc))
            {
                if(newasm::lambda::process)
                {
                    newasm::lambda::GLOBAL.result = opr;
                    newasm::lambda::GLOBAL.ret = true;
                    return 1;
                }
                newasm::system::stoproc = 1;
                newasm::mem::regs::psx = (opr);
                return 1;
            }
            newasm::terminate(newasm::exit_codes::invalid_syntax);
            return 1;
        }
        //push
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::push))
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
                if(newasm::header::functions::ishex(opr))
                {
                    newasm::header::data::callstkidx = newasm::mem::regs::stk;
                    if(newasm::stack::events.find(opr) != newasm::stack::events.end())
                    {
                        newasm::callproc(newasm::stack::events.at(opr));
                    }
                }
                return 1;
            }
        }
        //jmp
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::jmp))
        {
            if(newasm::header::execution_flow::exec_redirected)
            {
                newasm::terminate(newasm::exit_codes::nested_csm);
                return 1;
            }
            if(newasm::header::data::repl)
            {
                newasm::unsins(ins);
                return 1;
            }
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
        
        //load.adr
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::load))
        {
            if(suf == static_cast<std::string>("adr"))
            {
                if(!newasm::header::functions::isnumeric(opr) && !newasm::header::functions::isfloat(opr) &&
                !newasm::header::functions::istext(opr) && !newasm::header::functions::isref(opr) &&
                !newasm::header::functions::ischar(opr))
                {
                    //std::cout << "opr is " << opr << std::endl;
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }
                newasm::mem::program_memory[newasm::mem::regs::heaptr] = opr;
                return 1;
            }
        }

        //cmp
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::cmp))
        {
            if(newasm::header::data::repl)
            {
                newasm::unsins(ins);
                return 1;
            }
            //if(suf == newasm::mem::regs::fdx.identifier())
            int intreg = newasm::header::constants::inv_ireg_val;
            int floatreg = newasm::header::constants::inv_freg_val;
            std::string strreg = newasm::header::constants::inv_reg_val;

            if(suf == newasm::mem::regs::fdx.identifier())
            {
                intreg = newasm::mem::regs::fdx;
            }
            if(suf == newasm::mem::regs::stk.identifier())
            {
                intreg = newasm::mem::regs::stk;
            }
            if(suf == newasm::mem::regs::heaptr.identifier())
            {
                intreg = newasm::mem::regs::heaptr;
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
                strreg = newasm::mem::regs::tlr;
            }
            if(suf == newasm::mem::regs::tr0.identifier())
            {
                strreg = newasm::mem::regs::tr0;
            }
            if(suf == newasm::mem::regs::tr1.identifier())
            {
                strreg = newasm::mem::regs::tr1;
            }
            if(suf == newasm::mem::regs::stl.identifier())
            {
                strreg = newasm::mem::regs::stl;
            }
            if(suf == newasm::mem::regs::psx.identifier())
            {
                strreg = newasm::mem::regs::psx;
            }
            if(suf == newasm::mem::regs::prp.identifier())
            {
                strreg = newasm::mem::regs::prp;
            }
            if(suf == newasm::mem::regs::cpt.identifier())
            {
                strreg = newasm::mem::regs::cpt;
            }

            if(intreg != newasm::header::constants::inv_ireg_val)
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
                return 1;
            }
            if(floatreg != newasm::header::constants::inv_ireg_val)
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
        //je
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::je))
        {
            if(newasm::header::execution_flow::exec_redirected)
            {
                newasm::terminate(newasm::exit_codes::nested_csm);
                return 1;
            }
            if(newasm::header::data::repl)
            {
                newasm::unsins(ins);
                return 1;
            }
            if(suf == static_cast<std::string>("0"))
            {
                if(!newasm::mem::functions::datavalid(opr, newasm::mem::labels))
                {
                    newasm::terminate(newasm::exit_codes::bus_err);//,wholeline);
                    return 1;
                }
                if(newasm::mem::regs::cpr != newasm::cmp_results::equal)
                {
                    return 1;
                }
                newasm::code_stream::jump = 1;
                newasm::code_stream::jumpto = 1 + newasm::mem::labels[opr];
                return 1;
            }
        }
        //jne
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::jne))
        {
            if(newasm::header::execution_flow::exec_redirected)
            {
                newasm::terminate(newasm::exit_codes::nested_csm);
                return 1;
            }
            if(newasm::header::data::repl)
            {
                newasm::unsins(ins);
                return 1;
            }
            if(suf == static_cast<std::string>("0"))
            {
                if(!newasm::mem::functions::datavalid(opr, newasm::mem::labels))
                {
                    newasm::terminate(newasm::exit_codes::bus_err);//,wholeline);
                    return 1;
                }
                if(newasm::mem::regs::cpr == newasm::cmp_results::equal)
                {
                    return 1;
                }
                newasm::code_stream::jump = 1;
                newasm::code_stream::jumpto = 1 + newasm::mem::labels[opr];
                return 1;
            }
        }
        //jl
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::jl))
        {
            if(newasm::header::execution_flow::exec_redirected)
            {
                newasm::terminate(newasm::exit_codes::nested_csm);
                return 1;
            }
            if(newasm::header::data::repl)
            {
                newasm::unsins(ins);
                return 1;
            }
            if(suf == static_cast<std::string>("0"))
            {
                if(!newasm::mem::functions::datavalid(opr, newasm::mem::labels))
                {
                    newasm::terminate(newasm::exit_codes::bus_err);//,wholeline);
                    return 1;
                }
                if(newasm::mem::regs::cpr != newasm::cmp_results::less)
                {
                    return 1;
                }
                newasm::code_stream::jump = 1;
                newasm::code_stream::jumpto = 1 + newasm::mem::labels[opr];
                return 1;
            }
        }
        //jg
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::jg))
        {
            if(newasm::header::execution_flow::exec_redirected)
            {
                newasm::terminate(newasm::exit_codes::nested_csm);
                return 1;
            }
            if(newasm::header::data::repl)
            {
                newasm::unsins(ins);
                return 1;
            }
            if(suf == static_cast<std::string>("0"))
            {
                if(!newasm::mem::functions::datavalid(opr, newasm::mem::labels))
                {
                    newasm::terminate(newasm::exit_codes::bus_err);//,wholeline);
                    return 1;
                }
                if(newasm::mem::regs::cpr != newasm::cmp_results::greater)
                {
                    return 1;
                }
                newasm::code_stream::jump = 1;
                newasm::code_stream::jumpto = 1 + newasm::mem::labels[opr];
                return 1;
            }
        }
        //jle
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::jle))
        {
            if(newasm::header::execution_flow::exec_redirected)
            {
                newasm::terminate(newasm::exit_codes::nested_csm);
                return 1;
            }
            if(newasm::header::data::repl)
            {
                newasm::unsins(ins);
                return 1;
            }
            if(suf == static_cast<std::string>("0"))
            {
                if(!newasm::mem::functions::datavalid(opr, newasm::mem::labels))
                {
                    newasm::terminate(newasm::exit_codes::bus_err);//,wholeline);
                    return 1;
                }
                if(newasm::mem::regs::cpr != newasm::cmp_results::less && newasm::mem::regs::cpr != newasm::cmp_results::equal)
                {
                    return 1;
                }
                newasm::code_stream::jump = 1;
                newasm::code_stream::jumpto = 1 + newasm::mem::labels[opr];
                return 1;
            }
        }
        //jge
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::jge))
        {
            if(newasm::header::execution_flow::exec_redirected)
            {
                newasm::terminate(newasm::exit_codes::nested_csm);
                return 1;
            }
            if(newasm::header::data::repl)
            {
                newasm::unsins(ins);
                return 1;
            }
            if(suf == static_cast<std::string>("0"))
            {
                if(!newasm::mem::functions::datavalid(opr, newasm::mem::labels))
                {
                    newasm::terminate(newasm::exit_codes::bus_err);//,wholeline);
                    return 1;
                }
                if(newasm::mem::regs::cpr != newasm::cmp_results::greater && newasm::mem::regs::cpr != newasm::cmp_results::equal)
                {
                    return 1;
                }
                newasm::code_stream::jump = 1;
                newasm::code_stream::jumpto = 1 + newasm::mem::labels[opr];
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
            newasm::system::proclines ++;
            std::string newline = ins + static_cast<std::string>(" ") + suf;
            newasm::mem::funcs[newasm::system::cproc].push_back(newline);
            //std::cout << newasm::system::cproc << " : " << newline << std::endl;
            return 1;
        }
        //retf
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::retf))
        {
            try
            {
                if(!newasm::thread_line)
                {
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }
                std::string retf__v = [suf]()->std::string
                {
                    std::string result = suf;
                    newasm::runtime::functions::parse(result);
                    return result;
                }();
                //newasm::threads::memory.at(newasm::threads::now)->contents.clear();
                newasm::threads::memory.at(newasm::threads::now)->returned = true;
                newasm::threads::memory.at(newasm::threads::now)->returned_val = retf__v;
            }
            catch(const std::exception& e)
            {
                std::cerr << "Pravo si se zajebucnuo :: " << e.what() << '\n';
            }
            
            return 1;
        }
        //await
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::await__))
        {
            if(!newasm::header::functions::isref(suf))
            {
                //std::cout << "suf - `" << suf << '`' << std::endl;
                newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                return 1;
            }
            if(!newasm::mem::functions::datavalid(
                newasm::header::functions::remamp(suf), newasm::threads::memory))
            {
                newasm::terminate(newasm::exit_codes::invalid_thread);
                return 1;
            }
            std::string thread__ = newasm::header::functions::remamp(suf);
            try
            {
                while(!newasm::threads::memory.at(thread__)->contents.empty())
                {
                    newasm::thread_line = true;
                    newasm::threads::now = (thread__);
                    newasm::procline(*newasm::threads::memory.at(thread__)->contents.begin());
                    newasm::thread_line = false;
                    newasm::threads::memory.at(thread__)->contents.pop_front();
                    /*if(newasm::threads::memory.at(thread__)->returned)
                    {
                        break;
                    }*/
                }
            }
            catch(const std::exception& e)
            {
                std::cerr << "Zajebucnuo si se :: " << e.what() << '\n';
            }
            
            return 1;
        }
        //int
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::int__))
        {
            if(suf == static_cast<std::string>("0x1")) // sys_memsize
            {
                if(!newasm::header::functions::isnumeric(newasm::mem::regs::tlr))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }
                if(!newasm::mem::functions::setup_memsize(std::stoi(newasm::mem::regs::tlr)))
                {
                    newasm::terminate(newasm::exit_codes::invalid_config);
                    return 1;
                }
                return 1;
            }
            if(suf == static_cast<std::string>("0x2")) // sys_lazy_evhndlr
            {
                if(!newasm::header::functions::isnumeric(newasm::mem::regs::tlr))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }
                if(std::stoi(newasm::mem::regs::tlr) == 0)
                {
                    newasm::header::settings::lazy_evhndlr = false;
                    return 1;
                }
                if(std::stoi(newasm::mem::regs::tlr) == 1)
                {
                    newasm::header::settings::lazy_evhndlr = true;
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::invalid_syntax);
                return 1;
            }
            if(suf == static_cast<std::string>("0x3")) //sys_expect_compexpr
            {
                newasm::header::flags::compexpr = true;
                return 1;
            }
            newasm::terminate(newasm::exit_codes::invalid_sysint);
            return 1;
        }
        //sysenter
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::sysenter))
        {
            if(suf == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::ios))
            {
                if(newasm::thread_line)
                {
                    newasm::threads::sys_module = newasm::core::lang_inf::refs::ios;
                    return 1;
                }
                newasm::header::data::module = newasm::core::lang_inf::refs::ios;
                return 1;
            }
            if(suf == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::fs))
            {
                if(newasm::thread_line)
                {
                    newasm::threads::sys_module = newasm::core::lang_inf::refs::fs;
                    return 1;
                }
                newasm::header::data::module = newasm::core::lang_inf::refs::fs;
                return 1;
            }
            if(suf == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::exf))
            {
                if(newasm::thread_line)
                {
                    newasm::threads::sys_module = newasm::core::lang_inf::refs::exf;
                    return 1;
                }
                newasm::header::data::module = newasm::core::lang_inf::refs::exf;
                return 1;
            }
            if(suf == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::cmanip))
            {
                if(newasm::thread_line)
                {
                    newasm::threads::sys_module = newasm::core::lang_inf::refs::cmanip;
                    return 1;
                }
                newasm::header::data::module = newasm::core::lang_inf::refs::cmanip;
                return 1;
            }
            if(suf == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::txtop))
            {
                if(newasm::thread_line)
                {
                    newasm::threads::sys_module = newasm::core::lang_inf::refs::txtop;
                    return 1;
                }
                newasm::header::data::module = newasm::core::lang_inf::refs::txtop;
                return 1;
            }
            if(suf == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::net))
            {
                if(newasm::thread_line)
                {
                    newasm::threads::sys_module = newasm::core::lang_inf::refs::net;
                    return 1;
                }
                newasm::header::data::module = newasm::core::lang_inf::refs::net;
                return 1;
            }
            if(suf == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::mem))
            {
                if(newasm::thread_line)
                {
                    newasm::threads::sys_module = newasm::core::lang_inf::refs::mem;
                    return 1;
                }
                newasm::header::data::module = newasm::core::lang_inf::refs::mem;
                return 1;
            }
            if(suf == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::chrono))
            {
                if(newasm::thread_line)
                {
                    newasm::threads::sys_module = newasm::core::lang_inf::refs::chrono;
                    return 1;
                }
                newasm::header::data::module = newasm::core::lang_inf::refs::chrono;
                return 1;
            }
            if(suf == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::thread))
            {
                if(newasm::thread_line)
                {
                    newasm::threads::sys_module = newasm::core::lang_inf::refs::thread;
                    return 1;
                }
                newasm::header::data::module = newasm::core::lang_inf::refs::thread;
                return 1;
            }
            newasm::terminate(newasm::exit_codes::sysenter_fail);
            return 1;
        }
        //
        //wait
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::wait))
        {
            if(!newasm::header::functions::isnumeric(suf))
            {
                newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                return 1;
            }
            if(newasm::header::functions::wait(std::stoi(suf)) == -1)
            {
                newasm::terminate(newasm::exit_codes::dtyp_mismatch);
            }

            return 1;
        }
        //malloc
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::malloc__))
        {
            if(newasm::header::functions::isvmemsize(suf).first)
            {
                newasm::_virtual::realloc(newasm::header::functions::isvmemsize(suf).second);
                return 1;
            }
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

            return 1;
        }
        //call
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::call))
        {
            if(newasm::header::data::proc_now)
            {
                newasm::terminate(newasm::exit_codes::inline_proc);
                return 1;
            }
            if(newasm::header::functions::isalphanum(suf))
            {
                if(!newasm::mem::functions::datavalid(suf,newasm::mem::funcs))
                {
                    newasm::terminate(newasm::exit_codes::invalid_proc);
                    return 1;
                }
                newasm::callproc(suf);
                return 1;
            }
        }
        //proc
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::proc))
        {
            if(newasm::header::data::repl)
            {
                newasm::unsins(ins); // incompatible instruction for repl
                return 1;
            }
            if(newasm::thread_line)
            {
                newasm::terminate(newasm::exit_codes::invalid_syntax);
                return 1;
            }
            if(newasm::header::functions::isalphanum(suf))
            {
                if(newasm::mem::functions::datavalid(suf,newasm::mem::funcs))
                {
                    newasm::terminate(newasm::exit_codes::proc_redef);
                    return 1;
                }
                newasm::system::stop = 1;
                newasm::system::cproc = suf;
                newasm::system::proclines = 0;
                //std::cout << "Creating proc: " << opr << std::endl;
                return 1;
            }
        }
        //heap
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::heap))
        {
            if(newasm::header::functions::isnumeric(suf))
            {
                newasm::mem::regs::hea = newasm::mem::regs::hea + std::stoi(suf);
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
        //db - debug
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::db))
        {
            auto debugRegister = [](std::string str1, std::string str2)
            {
                newasm::header::functions::dbg(str1 + static_cast<std::string>(" = `") + 
                    str2 + 
                    static_cast<std::string>("`")
                );
            };
            if(suf == newasm::mem::regs::fdx.identifier())
            {
                debugRegister(suf, std::to_string(newasm::mem::regs::fdx));
                return 1;
            }
            if(suf == newasm::mem::regs::tlr.identifier())
            {
                debugRegister(suf, (newasm::mem::regs::tlr));
                return 1;
            }
            if(suf == newasm::mem::regs::tr0.identifier())
            {
                debugRegister(suf, (newasm::mem::regs::tr0));
                return 1;
            }
            if(suf == newasm::mem::regs::tr1.identifier())
            {
                debugRegister(suf, (newasm::mem::regs::tr1));
                return 1;
            }
            if(suf == newasm::mem::regs::stl.identifier())
            {
                debugRegister(suf, (newasm::mem::regs::stl));
                return 1;
            }
            if(suf == newasm::mem::regs::psx.identifier())
            {
                debugRegister(suf, (newasm::mem::regs::psx));
                return 1;
            }
            if(suf == newasm::mem::regs::stk.identifier())
            {
                debugRegister(suf, std::to_string(newasm::mem::regs::stk));
                return 1;
            }
            if(suf == newasm::mem::regs::heaptr.identifier())
            {
                debugRegister(suf, std::to_string(newasm::mem::regs::heaptr));
                return 1;
            }
            if(suf == newasm::mem::regs::prp.identifier())
            {
                debugRegister(suf, (newasm::mem::regs::prp));
                return 1;
            }
            if(suf == newasm::mem::regs::cpt.identifier())
            {
                debugRegister(suf, (newasm::mem::regs::cpt));
                return 1;
            }
            if(suf == newasm::mem::regs::cpr.identifier())
            {
                debugRegister(suf, std::to_string(newasm::mem::regs::cpr));
                return 1;
            }
            if(suf == newasm::mem::regs::cr0.identifier())
            {
                debugRegister(suf, std::to_string(newasm::mem::regs::cr0));
                return 1;
            }
            if(suf == newasm::mem::regs::cr1.identifier())
            {
                debugRegister(suf, std::to_string(newasm::mem::regs::cr1));
                return 1;
            }
            if(suf == newasm::mem::regs::br0.identifier())
            {
                debugRegister(suf, std::to_string(newasm::mem::regs::br0));
                return 1;
            }
            if(suf == newasm::mem::regs::br1.identifier())
            {
                debugRegister(suf, std::to_string(newasm::mem::regs::br1));
                return 1;
            }
            newasm::terminate(newasm::exit_codes::invalid_syntax);
            return 1;
        }
        // ret - classic return
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::ret))
        {
            if(newasm::header::data::repl)
            {
                newasm::unsins(ins);
                return 1;
            }
            std::vector<std::string> tokens;
            for(std::vector<std::pair<std::string,std::string>>::iterator i = newasm::env_vars->begin(); i < newasm::env_vars->end(); ++i)
            {
                tokens = newasm::header::functions::split_fixed(suf,'/');
                if(newasm::header::functions::trim(tokens[0]) == "*") if(newasm::header::functions::trim(tokens[1]) == i->first)
                {
                    suf = i->second;
                }
            }

            if(suf == newasm::header::constants::inv_reg_val)
            {
                newasm::terminate(newasm::exit_codes::mem_overflow);//,wholeline);
                return 1;
            }
            newasm::header::functions::parseopr(suf, newasm::mem::data);
            newasm::parseopr_struct(suf);
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
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::retn))
        {
            if(newasm::header::data::repl)
            {
                newasm::unsins(ins);
                return 1;
            }
            if(suf == newasm::mem::regs::fdx.identifier())
            {
                newasm::header::data::exception = false;
                newasm::terminate(newasm::mem::regs::fdx);//,wholeline);
                return 1;
            }
            if(suf == newasm::mem::regs::tlr.identifier())
            {
                if(newasm::header::functions::isnumeric(newasm::mem::regs::tlr))
                {
                    newasm::header::data::exception = false;
                    newasm::terminate(std::stoi(newasm::mem::regs::tlr));//,wholeline);
                    return 1;
                }
            }
            if(suf == newasm::mem::regs::tr0.identifier())
            {
                if(newasm::header::functions::isnumeric(newasm::mem::regs::tr0))
                {
                    newasm::header::data::exception = false;
                    newasm::terminate(std::stoi(newasm::mem::regs::tr0));//,wholeline);
                    return 1;
                }
            }
            if(suf == newasm::mem::regs::tr1.identifier())
            {
                if(newasm::header::functions::isnumeric(newasm::mem::regs::tr1))
                {
                    newasm::header::data::exception = false;
                    newasm::terminate(std::stoi(newasm::mem::regs::tr1));//,wholeline);
                    return 1;
                }
            }
            if(suf == newasm::mem::regs::stl.identifier())
            {
                if(newasm::header::functions::isnumeric(newasm::mem::regs::stl))
                {
                    newasm::header::data::exception = false;
                    newasm::terminate(std::stoi(newasm::mem::regs::stl));//,wholeline);
                    return 1;
                }
            }
            if(suf == newasm::mem::regs::psx.identifier())
            {
                if(newasm::header::functions::isnumeric(newasm::mem::regs::psx))
                {
                    newasm::header::data::exception = false;
                    newasm::terminate(std::stoi(newasm::mem::regs::psx));//,wholeline);
                    return 1;
                }
            }
            if(suf == newasm::mem::regs::stk.identifier())
            {
                newasm::header::data::exception = false;
                newasm::terminate(newasm::mem::regs::stk);//,wholeline);
                return 1;
            }
            if(suf == newasm::mem::regs::heaptr.identifier())
            {
                newasm::header::data::exception = false;
                newasm::terminate(newasm::mem::regs::heaptr);//,wholeline);
                return 1;
            }
            if(suf == newasm::mem::regs::prp.identifier())
            {
                if(newasm::header::functions::isnumeric(newasm::mem::regs::prp))
                {
                    newasm::header::data::exception = false;
                    newasm::terminate(std::stoi(newasm::mem::regs::prp));//,wholeline);
                    return 1;
                }
            }
            if(suf == newasm::mem::regs::cpt.identifier())
            {
                if(newasm::header::functions::isnumeric(newasm::mem::regs::cpt))
                {
                    newasm::header::data::exception = false;
                    newasm::terminate(std::stoi(newasm::mem::regs::cpt));//,wholeline);
                    return 1;
                }
            }
            if(suf == newasm::mem::regs::cpr.identifier())
            {
                newasm::header::data::exception = false;
                newasm::terminate(newasm::mem::regs::cpr);//,wholeline);
                return 1;
            }
            if(suf == newasm::mem::regs::cr0.identifier())
            {
                newasm::header::data::exception = false;
                newasm::terminate(static_cast<int>(newasm::mem::regs::cr0));//,wholeline);
                return 1;
            }
            if(suf == newasm::mem::regs::cr1.identifier())
            {
                newasm::header::data::exception = false;
                newasm::terminate(static_cast<int>(newasm::mem::regs::cr1));//,wholeline);
                return 1;
            }
            if(suf == newasm::mem::regs::br0.identifier())
            {
                newasm::header::data::exception = false;
                newasm::terminate(newasm::mem::regs::br0);//,wholeline);
                return 1;
            }
            if(suf == newasm::mem::regs::br1.identifier())
            {
                newasm::header::data::exception = false;
                newasm::terminate(newasm::mem::regs::br1);//,wholeline);
                return 1;
            }
            newasm::terminate(newasm::exit_codes::invalid_retn);//,wholeline);
            return 1;
        }
        
        //zero
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::zero))
        {
            if(suf == newasm::mem::regs::fdx.identifier())
            {
                newasm::mem::regs::fdx = 0;
                return 1;
            }
            if(suf == newasm::mem::regs::tlr.identifier())
            {
                newasm::mem::regs::tlr = newasm::header::constants::inv_reg_val;
                return 1;
            }
            if(suf == newasm::mem::regs::tr0.identifier())
            {
                newasm::mem::regs::tr0 = newasm::header::constants::inv_reg_val;
                return 1;
            }
            if(suf == newasm::mem::regs::tr1.identifier())
            {
                newasm::mem::regs::tr1 = newasm::header::constants::inv_reg_val;
                return 1;
            }
            if(suf == newasm::mem::regs::stl.identifier())
            {
                newasm::mem::regs::stl = newasm::header::constants::inv_reg_val;
                return 1;
            }
            if(suf == newasm::mem::regs::stk.identifier())
            {
                newasm::mem::regs::stk = 0;
                newasm::terminate(newasm::exit_codes::stkhea_col);//,wholeline); // Why'd you touch STK in the first place?
                return 1;
            }
            if(suf == newasm::mem::regs::heaptr.identifier())
            {
                newasm::mem::regs::heaptr = 0;
                return 1;
            }
            if(suf == newasm::mem::regs::psx.identifier())
            {
                newasm::mem::regs::psx = newasm::header::constants::inv_reg_val;
                return 1;
            }
            if(suf == newasm::mem::regs::prp.identifier())
            {
                newasm::mem::regs::prp = newasm::header::constants::inv_reg_val;
                return 1;
            }
            if(suf == newasm::mem::regs::cpt.identifier())
            {
                newasm::mem::regs::cpt = newasm::header::constants::inv_reg_val;
                return 1;
            }
            if(suf == newasm::mem::regs::cpr.identifier())
            {
                newasm::mem::regs::cpr = 0;
                return 1;
            }
            if(suf == newasm::mem::regs::cr0.identifier())
            {
                newasm::mem::regs::cr0 = 0.0;
                return 1;
            }
            if(suf == newasm::mem::regs::cr1.identifier())
            {
                newasm::mem::regs::cr1 = 0.0;
                return 1;
            }
            if(suf == newasm::mem::regs::br0.identifier())
            {
                newasm::mem::regs::br0 = 0;
                return 1;
            }
            if(suf == newasm::mem::regs::br1.identifier())
            {
                newasm::mem::regs::br1 = 0;
                return 1;
            }
            newasm::terminate(newasm::exit_codes::bus_err); // non existing register; 
            //however this error is highly misleading since registers arent in RAM
            return 1;
        }
        //inc
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::inc))
        {
            if(suf == newasm::mem::regs::fdx.identifier())
            {
                newasm::mem::regs::fdx ++;
                return 1;
            }
            if(suf == newasm::mem::regs::stk.identifier())
            {
                newasm::mem::regs::stk ++;
                return 1;
            }
            if(suf == newasm::mem::regs::heaptr.identifier())
            {
                newasm::mem::regs::heaptr ++;
                return 1;
            }
            if(suf == newasm::mem::regs::cpr.identifier())
            {
                newasm::mem::regs::cpr ++;
                return 1;
            }
            if(suf == newasm::mem::regs::cr0.identifier())
            {
                newasm::mem::regs::cr0 ++;
                return 1;
            }
            if(suf == newasm::mem::regs::cr1.identifier())
            {
                newasm::mem::regs::cr1 ++;
                return 1;
            }
            if(suf == newasm::mem::regs::br0.identifier())
            {
                newasm::mem::regs::br0 ++;
                return 1;
            }
            if(suf == newasm::mem::regs::br1.identifier())
            {
                newasm::mem::regs::br1 ++;
                return 1;
            }

            //typeless registers require a different approach
            if(suf == newasm::mem::regs::tlr.identifier())
            {
                if(!newasm::header::functions::isnumeric(newasm::mem::regs::tlr))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }
                int tmp = std::stoi(newasm::mem::regs::tlr)+1;
                newasm::mem::regs::tlr = std::to_string(tmp);
                return 1;
            }
            if(suf == newasm::mem::regs::stl.identifier())
            {
                if(!newasm::header::functions::isnumeric(newasm::mem::regs::stl))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }
                int tmp = std::stoi(newasm::mem::regs::stl)+1;
                newasm::mem::regs::stl = std::to_string(tmp);
                return 1;
            }
            if(suf == newasm::mem::regs::psx.identifier())
            {
                if(!newasm::header::functions::isnumeric(newasm::mem::regs::psx))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }
                int tmp = std::stoi(newasm::mem::regs::psx)+1;
                newasm::mem::regs::psx = std::to_string(tmp);
                return 1;
            }

            if(suf == newasm::mem::regs::tr0.identifier())
            {
                if(!newasm::header::functions::isnumeric(newasm::mem::regs::tr0))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }
                int tmp = std::stoi(newasm::mem::regs::tr0)+1;
                newasm::mem::regs::tr0 = std::to_string(tmp);
                return 1;
            }
            if(suf == newasm::mem::regs::tr1.identifier())
            {
                if(!newasm::header::functions::isnumeric(newasm::mem::regs::tr1))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }
                int tmp = std::stoi(newasm::mem::regs::tr1)+1;
                newasm::mem::regs::tr1 = std::to_string(tmp);
                return 1;
            }
            //and then, we have this beautiful procedure pointer...
            //we'll just pickup the next procedure from the map memory
            if(suf == newasm::mem::regs::prp.identifier())
            {
                if(!newasm::mem::functions::datavalid(newasm::header::functions::remamp(newasm::mem::regs::prp), newasm::mem::funcs))
                {
                    newasm::terminate(newasm::exit_codes::invalid_proc);
                    return 1;
                }
                bool found = false;
                for(std::map<std::string, std::vector<std::string>>::iterator i = newasm::mem::funcs.begin(); i != newasm::mem::funcs.end(); ++i)
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
            if(suf == newasm::mem::regs::cpt.identifier())
            {
                //make later
                return 1;
            }
            
            newasm::terminate(newasm::exit_codes::invalid_syntax); // non existing register; 
            //however this error is highly misleading since registers arent in RAM
            return 1;
        }
        //dec
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::dec))
        {
            if(suf == newasm::mem::regs::fdx.identifier())
            {
                newasm::mem::regs::fdx --;
                return 1;
            }
            if(suf == newasm::mem::regs::stk.identifier())
            {
                newasm::mem::regs::stk --;
                return 1;
            }
            if(suf == newasm::mem::regs::heaptr.identifier())
            {
                newasm::mem::regs::heaptr --;
                return 1;
            }
            if(suf == newasm::mem::regs::cpr.identifier())
            {
                newasm::mem::regs::cpr --;
                return 1;
            }
            if(suf == newasm::mem::regs::cr0.identifier())
            {
                newasm::mem::regs::cr0 --;
                return 1;
            }
            if(suf == newasm::mem::regs::cr1.identifier())
            {
                newasm::mem::regs::cr1 --;
                return 1;
            }
            if(suf == newasm::mem::regs::br0.identifier())
            {
                newasm::mem::regs::br0 --;
                return 1;
            }
            if(suf == newasm::mem::regs::br1.identifier())
            {
                newasm::mem::regs::br1 --;
                return 1;
            }

            //typeless registers require a different approach
            if(suf == newasm::mem::regs::tlr.identifier())
            {
                if(!newasm::header::functions::isnumeric(newasm::mem::regs::tlr))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }
                int tmp = std::stoi(newasm::mem::regs::tlr)-1;
                newasm::mem::regs::tlr = std::to_string(tmp);
                return 1;
            }
            if(suf == newasm::mem::regs::stl.identifier())
            {
                if(!newasm::header::functions::isnumeric(newasm::mem::regs::stl))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }
                int tmp = std::stoi(newasm::mem::regs::stl)-1;
                newasm::mem::regs::stl = std::to_string(tmp);
                return 1;
            }
            if(suf == newasm::mem::regs::psx.identifier())
            {
                if(!newasm::header::functions::isnumeric(newasm::mem::regs::psx))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }
                int tmp = std::stoi(newasm::mem::regs::psx)-1;
                newasm::mem::regs::psx = std::to_string(tmp);
                return 1;
            }

            if(suf == newasm::mem::regs::tr1.identifier())
            {
                if(!newasm::header::functions::isnumeric(newasm::mem::regs::tr1))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }
                int tmp = std::stoi(newasm::mem::regs::tr1)-1;
                newasm::mem::regs::tr1 = std::to_string(tmp);
                return 1;
            }
            if(suf == newasm::mem::regs::tr0.identifier())
            {
                if(!newasm::header::functions::isnumeric(newasm::mem::regs::tr0))
                {
                    newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                    return 1;
                }
                int tmp = std::stoi(newasm::mem::regs::tr0)-1;
                newasm::mem::regs::tr0 = std::to_string(tmp);
                return 1;
            }
            //and then, we have this beautiful procedure pointer...
            //we'll just pickup the last procedure from the map memory
            if(suf == newasm::mem::regs::prp.identifier())
            {
                if(!newasm::mem::functions::datavalid(newasm::header::functions::remamp(newasm::mem::regs::prp), newasm::mem::funcs))
                {
                    newasm::terminate(newasm::exit_codes::invalid_proc);
                    return 1;
                }
                for(std::map<std::string, std::vector<std::string>>::iterator i = newasm::mem::funcs.begin(); i != newasm::mem::funcs.end(); ++i)
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
            if(suf == newasm::mem::regs::cpt.identifier())
            {
                //make later
                return 1;
            }
            newasm::terminate(newasm::exit_codes::invalid_syntax); // non existing register; 
            //however this error is highly misleading since registers arent in RAM
            return 1;
        }
        
        
        newasm::terminate(newasm::exit_codes::invalid_ins);//,wholeline);
        return 1;
    }
    int process_i(std::string line, std::string ins)
    {
        
        //exit
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::exit))
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
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::end))
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
            newasm::system::stop = 0;
            //std::cout << "Finished proc: " << newasm::system::cproc << std::endl;
            return 1;
        }
        if(newasm::system::stop == 1)
        {
            newasm::system::proclines ++;
            std::string newline = ins;
            newasm::mem::funcs[newasm::system::cproc].push_back(newline);
            //std::cout << newasm::system::cproc << " : " << newline << std::endl;
            return 1;
        }
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::nop))
        {
            // do nothing
            return 1;
        }

        //cls
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::cls))
        {
            #ifdef _WIN32
                std::system("cls");
            #else
                std::system("clear");
            #endif

            return 1;
        }
        //xchg
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::xchg))
        {
            newasm::header::data::temp = newasm::mem::regs::tlr;
            //newasm::mem::regs::tlr = newasm::mem::regs::stl;
            newasm::mem::regs::tlr.set_value(newasm::mem::regs::stl.get_value());
            //newasm::mem::regs::stl = newasm::header::data::temp;
            newasm::mem::regs::stl.set_value(newasm::header::data::temp);
            return 1;
        }
        //syscall
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::syscall))
        {
            //thread
            if(newasm::threads::functions::get_sysenter() == newasm::core::lang_inf::refs::thread)
            {
                if(newasm::mem::regs::fdx == 1) //print output of the thread
                {
                    if(!newasm::header::functions::isref(newasm::mem::regs::tlr))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    std::string thread__ = newasm::header::functions::remamp(newasm::mem::regs::tlr);
                    if(!newasm::mem::functions::datavalid(thread__, newasm::threads::memory))
                    {
                        newasm::terminate(newasm::exit_codes::invalid_thread);
                        return 1;
                    }
                    if(!newasm::threads::memory.at(thread__)->contents.empty())
                    {
                        newasm::terminate(newasm::exit_codes::expected_await);
                        return 1;
                    }
                    std::cout << newasm::threads::memory.at(thread__)->output.str();
                    newasm::syscalls::iostream::out_bopr(newasm::mem::regs::stl);
                    return 1;
                }
                if(newasm::mem::regs::fdx == 2) //get returned val from thread
                {
                    if(!newasm::header::functions::isref(newasm::mem::regs::tlr))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    std::string thread__ = newasm::header::functions::remamp(newasm::mem::regs::tlr);
                    if(!newasm::mem::functions::datavalid(thread__, newasm::threads::memory))
                    {
                        newasm::terminate(newasm::exit_codes::invalid_thread);
                        return 1;
                    }
                    if(!newasm::threads::memory.at(thread__)->contents.empty())
                    {
                        newasm::terminate(newasm::exit_codes::expected_await);
                        return 1;
                    }
                    //std::cout << newasm::threads::memory.at(thread__)->output.str();
                    //newasm::syscalls::iostream::out_bopr(newasm::mem::regs::stl);
                    newasm::mem::regs::tlr = newasm::threads::memory.at(thread__)->returned_val;
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::invalid_syntax);
                return 1;
            }
            //chrono
            if(newasm::threads::functions::get_sysenter() == newasm::core::lang_inf::refs::chrono)
            {
                if(newasm::mem::regs::fdx == 1) //getyear
                {
                    newasm::mem::regs::tlr = std::to_string(newasm::chrono::year());
                    return 1;
                }
                if(newasm::mem::regs::fdx == 2) //getmonth
                {
                    newasm::mem::regs::tlr = std::to_string(newasm::chrono::month());
                    return 1;
                }
                if(newasm::mem::regs::fdx == 3) //day
                {
                    newasm::mem::regs::tlr = std::to_string(newasm::chrono::day());
                    return 1;
                }
                if(newasm::mem::regs::fdx == 4) //hour
                {
                    newasm::mem::regs::tlr = std::to_string(newasm::chrono::hour());
                    return 1;
                }
                if(newasm::mem::regs::fdx == 5) //min
                {
                    newasm::mem::regs::tlr = std::to_string(newasm::chrono::minute());
                    return 1;
                }
                if(newasm::mem::regs::fdx == 6) //sec
                {
                    newasm::mem::regs::tlr = std::to_string(newasm::chrono::second());
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::unknown_fdx);
                return 1;
            }
            //////process management (execution flow)
            //if(opr == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::exf))
            if(newasm::threads::functions::get_sysenter() == newasm::core::lang_inf::refs::exf)
            {
                if(newasm::header::execution_flow::exec_redirected)
                {
                    newasm::terminate(newasm::exit_codes::nested_redirect);
                    return 1;
                }
                if(newasm::mem::regs::fdx == 1)
                {
                    if(!newasm::header::functions::istext(newasm::mem::regs::tlr))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    
                    newasm::header::execution_flow::entry_start_line = newasm::header::data::lastlndx+1;
                    newasm::header::execution_flow::file = newasm::header::functions::remq(newasm::mem::regs::tlr);
                    newasm::header::execution_flow::exec_redirected = true;
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::unknown_fdx);
                return 1;
            }
            //net
            //if(opr == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::net))
            if(newasm::threads::functions::get_sysenter() == newasm::core::lang_inf::refs::net)
            {
                if(newasm::mem::regs::fdx == 1) //download files
                {
                    if(!newasm::header::functions::istext(newasm::mem::regs::tlr))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    if(!newasm::header::functions::istext(newasm::mem::regs::stl))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }

                    int result = newasm::net::download(
                        newasm::header::functions::trim(
                            newasm::header::functions::remq(
                                newasm::mem::regs::tlr
                            )
                        ),
                        newasm::header::functions::trim(
                            newasm::header::functions::remq(
                                newasm::mem::regs::stl
                            )
                        )
                    );

                    newasm::header::functions::log(
                        static_cast<std::string>("Network info\nnewasm::net::download - tlr : '") + newasm::header::functions::trim(
                            newasm::header::functions::remq(
                                newasm::mem::regs::tlr
                            )
                        ) + "'" + static_cast<std::string>("\nnewasm::net::download - stl : '") + newasm::header::functions::trim(
                            newasm::header::functions::remq(
                                newasm::mem::regs::stl
                            )
                        ) + "'" + static_cast<std::string>("\nnewasm::net::download - result : '") + std::to_string(result) + "'"
                    );
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::unknown_fdx);
                return 1;
            }
            //memory/data manipulation
            //if(opr == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::mem))
            if(newasm::threads::functions::get_sysenter() == newasm::core::lang_inf::refs::mem)
            {
                if(newasm::mem::regs::fdx == 1)
                {
                    if(!newasm::header::functions::isref(newasm::mem::regs::tlr))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    if(!newasm::mem::functions::datavalid(newasm::header::functions::remamp(newasm::mem::regs::tlr),newasm::mem::data))
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    newasm::syscalls::mem::constvals.push_back(newasm::header::functions::remamp(newasm::mem::regs::tlr));
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::unknown_fdx);
                return 1;
            }
            //text manipulation
            //if(opr == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::txtop))
            if(newasm::threads::functions::get_sysenter() == newasm::core::lang_inf::refs::txtop)
            {
                if(newasm::mem::regs::fdx == 1)
                {
                    newasm::syscalls::txtop::impl::concat();
                    return 1;
                }
                if(newasm::mem::regs::fdx == 2)
                {
                    newasm::syscalls::txtop::impl::trim();
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::unknown_fdx);
                return 1;
            }
            //////////// container manipulation
            //if(opr == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::cmanip))
            if(newasm::threads::functions::get_sysenter() == newasm::core::lang_inf::refs::cmanip)
            {
                if(newasm::mem::regs::cpt == newasm::header::constants::inv_reg_val)
                {
                    newasm::terminate(newasm::exit_codes::invalid_memacc);
                    return 1;
                }
                //clear
                if(newasm::mem::regs::fdx == 1)
                {
                    if(!newasm::mem::functions::datavalid(newasm::header::functions::remamp(newasm::mem::regs::cpt),newasm::containers::bit_arrays))
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    newasm::containers::bit_arrays.at(newasm::header::functions::remamp(newasm::mem::regs::cpt))->clear();
                    return 1;
                }
                //flip
                if(newasm::mem::regs::fdx == 2)
                {
                    if(!newasm::mem::functions::datavalid(newasm::header::functions::remamp(newasm::mem::regs::cpt),newasm::containers::bit_arrays))
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    newasm::containers::bit_arrays.at(newasm::header::functions::remamp(newasm::mem::regs::cpt))->flip();
                    return 1;
                }
                //reverse
                if(newasm::mem::regs::fdx == 3)
                {
                    if(!newasm::mem::functions::datavalid(newasm::header::functions::remamp(newasm::mem::regs::cpt),newasm::containers::bit_arrays))
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    newasm::containers::bit_arrays.at(newasm::header::functions::remamp(newasm::mem::regs::cpt))->reverse();
                    return 1;
                }
                //set at
                if(newasm::mem::regs::fdx == 4)
                {
                    if(!newasm::mem::functions::datavalid(newasm::header::functions::remamp(newasm::mem::regs::cpt),newasm::containers::bit_arrays))
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    if(!newasm::header::functions::isnumeric(newasm::mem::regs::tlr))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    if(!newasm::header::functions::isnumeric(newasm::mem::regs::stl))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    newasm::containers::bit_arrays.at(newasm::header::functions::remamp(newasm::mem::regs::cpt))->set_at(std::stoi(newasm::mem::regs::tlr),std::stoi(newasm::mem::regs::stl));
                    return 1;
                }
                // get at
                if(newasm::mem::regs::fdx == 5)
                {
                    if(!newasm::mem::functions::datavalid(newasm::header::functions::remamp(newasm::mem::regs::cpt),newasm::containers::bit_arrays))
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    int result = newasm::containers::bit_arrays.at(newasm::header::functions::remamp(newasm::mem::regs::cpt))->get_at(std::stoi(newasm::mem::regs::tlr));
                    newasm::mem::regs::tlr = std::to_string(result);
                    return 1;
                }
                //binary trees
                //set at parent of
                if(newasm::mem::regs::fdx == 6)
                {
                    if(!newasm::mem::functions::datavalid(newasm::header::functions::remamp(newasm::mem::regs::cpt),newasm::containers::binary_trees))
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    newasm::containers::binary_trees.at(newasm::header::functions::remamp(newasm::mem::regs::cpt))->set_at_parent_of(std::stoi(newasm::mem::regs::tlr),std::stoi(newasm::mem::regs::stl));
                    return 1;
                }
                //right child
                if(newasm::mem::regs::fdx == 7)
                {
                    if(!newasm::mem::functions::datavalid(newasm::header::functions::remamp(newasm::mem::regs::cpt),newasm::containers::binary_trees))
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    newasm::containers::binary_trees.at(newasm::header::functions::remamp(newasm::mem::regs::cpt))->set_at_right_child_of(std::stoi(newasm::mem::regs::tlr),std::stoi(newasm::mem::regs::stl));
                    return 1;
                }
                //left child
                if(newasm::mem::regs::fdx == 8)
                {
                    if(!newasm::mem::functions::datavalid(newasm::header::functions::remamp(newasm::mem::regs::cpt),newasm::containers::binary_trees))
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    newasm::containers::binary_trees.at(newasm::header::functions::remamp(newasm::mem::regs::cpt))->set_at_left_child_of(std::stoi(newasm::mem::regs::tlr),std::stoi(newasm::mem::regs::stl));
                    return 1;
                }
                //get
                if(newasm::mem::regs::fdx == 9)
                {
                    if(!newasm::mem::functions::datavalid(newasm::header::functions::remamp(newasm::mem::regs::cpt),newasm::containers::binary_trees))
                    {
                        newasm::terminate(newasm::exit_codes::invalid_memacc);
                        return 1;
                    }
                    int result = newasm::containers::binary_trees.at(newasm::header::functions::remamp(newasm::mem::regs::cpt))->get_at(std::stoi(newasm::mem::regs::tlr));
                    newasm::mem::regs::tlr = std::to_string(result);
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::unknown_fdx);
                return 1;
            }
            ///// input-output stream
            //if(opr == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::ios))
            if(newasm::threads::functions::get_sysenter() == newasm::core::lang_inf::refs::ios)
            {
                //print text
                if(newasm::mem::regs::fdx == 1)
                {
                    if(!newasm::header::functions::istext(newasm::mem::regs::tlr))
                    {
                        using std::endl, std::cout;
                        //cout << " ERROR CATCH - tlr is: " << newasm::mem::regs::tlr << endl;
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    newasm::mem::regs::tlr = newasm::header::functions::remq(newasm::mem::regs::tlr);
                    if(newasm::thread_line)
                    {
                        newasm::threads::memory.at(newasm::threads::now)->output << newasm::mem::regs::tlr.get_value();
                        newasm::threads::memory.at(newasm::threads::now)->output << newasm::syscalls::iostream::get_ref_val__2(newasm::mem::regs::stl);
                        return 1;
                    }
                    std::cout << newasm::mem::regs::tlr;// << std::endl;
                    //newasm::mem::functions::out_bopr(newasm::mem::regs::stl);
                    newasm::syscalls::iostream::out_bopr(newasm::mem::regs::stl);
                    return 1;
                }
                //print numbers and floats
                if(newasm::mem::regs::fdx == 2)
                {
                    if(!newasm::header::functions::isnumeric(newasm::mem::regs::tlr) && !newasm::header::functions::isfloat(newasm::mem::regs::tlr))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    if(newasm::thread_line)
                    {
                        newasm::threads::memory.at(newasm::threads::now)->output << newasm::mem::regs::tlr.get_value();
                        newasm::threads::memory.at(newasm::threads::now)->output << newasm::syscalls::iostream::get_ref_val__2(newasm::mem::regs::stl);
                        return 1;
                    }
                    std::cout << newasm::mem::regs::tlr;// << std::endl;
                    //newasm::mem::functions::out_bopr(newasm::mem::regs::stl);
                    newasm::syscalls::iostream::out_bopr(newasm::mem::regs::stl);
                    return 1;
                }
                //input text
                if(newasm::mem::regs::fdx == 3)
                {
                    std::getline(std::cin, newasm::mem::regs::tlr.ref_value());
                    newasm::mem::regs::tlr.add_end_("\"");
                    return 1;
                }
                //input numbers and floats
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
                //print values of builtin operands
                if(newasm::mem::regs::fdx == 5)
                {
                    if(newasm::thread_line)
                    {
                        newasm::threads::memory.at(newasm::threads::now)->output << newasm::syscalls::iostream::get_ref_val__2(newasm::mem::regs::tlr);
                        return 1;
                    }
                    //newasm::mem::functions::out_bopr(newasm::mem::regs::tlr);
                    newasm::syscalls::iostream::out_bopr(newasm::mem::regs::tlr);
                    return 1;
                }
                //print references
                if(newasm::mem::regs::fdx == 6)
                {
                    if(!newasm::header::functions::isref(newasm::mem::regs::tlr))
                    {
                        //std::cout << "tlr is " << newasm::mem::regs::tlr << std::endl;
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    if(newasm::thread_line)
                    {
                        newasm::threads::memory.at(newasm::threads::now)->output << newasm::header::functions::remamp(newasm::mem::regs::tlr);
                        newasm::threads::memory.at(newasm::threads::now)->output << newasm::syscalls::iostream::get_ref_val__2(newasm::mem::regs::stl);
                        return 1;
                    }
                    std::cout << newasm::header::functions::remamp(newasm::mem::regs::tlr);
                    //newasm::mem::functions::out_bopr(newasm::mem::regs::stl);
                    newasm::syscalls::iostream::out_bopr(newasm::mem::regs::stl);
                    return 1;
                }
                //print characters
                if(newasm::mem::regs::fdx == 7)
                {
                    //std::cout << "tlr is: \"" << newasm::mem::regs::tlr << "\"\n";
                    if(!newasm::header::functions::ischar(newasm::mem::regs::tlr))
                    {
                        //std::cout << "tlr is " << newasm::mem::regs::tlr << std::endl;
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    if(newasm::thread_line)
                    {
                        newasm::threads::memory.at(newasm::threads::now)->output << newasm::header::functions::remsq(newasm::mem::regs::tlr);
                        newasm::threads::memory.at(newasm::threads::now)->output << newasm::syscalls::iostream::get_ref_val__2(newasm::mem::regs::stl);
                        return 1;
                    }
                    std::cout << newasm::header::functions::remsq(newasm::mem::regs::tlr);
                    //newasm::mem::functions::out_bopr(newasm::mem::regs::stl);
                    newasm::syscalls::iostream::out_bopr(newasm::mem::regs::stl);
                    return 1;
                }
                if(newasm::mem::regs::fdx == 8)
                {
                    std::cin >> newasm::mem::regs::tlr;
                    newasm::mem::regs::tlr.add_end_("'");
                    if(!newasm::header::functions::ischar(newasm::mem::regs::tlr))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::unknown_fdx);
                return 1;
            }
            //////file stream
            //if(opr == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::fs))
            if(newasm::threads::functions::get_sysenter() == newasm::core::lang_inf::refs::fs)
            {
                //create folder
                if(newasm::mem::regs::fdx == 1)
                {
                    if(!newasm::header::functions::istext(newasm::mem::regs::tlr))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    newasm::handles::dir_handle->set_name(newasm::header::functions::remq(newasm::mem::regs::tlr));
                    newasm::handles::dir_handle->create();
                    return 1;
                }
                //remove folder
                if(newasm::mem::regs::fdx == 2)
                {
                    if(!newasm::header::functions::istext(newasm::mem::regs::tlr))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    newasm::handles::dir_handle->set_name(newasm::header::functions::remq(newasm::mem::regs::tlr));
                    newasm::handles::dir_handle->remove();
                    return 1;
                }
                //create file
                if(newasm::mem::regs::fdx == 3)
                {
                    if(!newasm::header::functions::istext(newasm::mem::regs::tlr))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    newasm::handles::file_handle->set_name(newasm::header::functions::remq(newasm::mem::regs::tlr));
                    newasm::handles::file_handle->overwrite_text("");
                    return 1;
                }
                //remove file
                if(newasm::mem::regs::fdx == 4)
                {
                    if(!newasm::header::functions::istext(newasm::mem::regs::tlr))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    newasm::handles::file_handle->set_name(newasm::header::functions::remq(newasm::mem::regs::tlr));
                    newasm::handles::file_handle->remove();
                    return 1;
                }
                //overwrite text
                if(newasm::mem::regs::fdx == 5)
                {
                    if(!newasm::header::functions::istext(newasm::mem::regs::tlr))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    if(!newasm::header::functions::istext(newasm::mem::regs::stl))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    newasm::handles::file_handle->set_name(newasm::header::functions::remq(newasm::mem::regs::tlr));
                    newasm::handles::file_handle->overwrite_text(newasm::header::functions::remq(newasm::mem::regs::stl));
                    return 1;
                }
                //append text
                if(newasm::mem::regs::fdx == 6)
                {
                    if(!newasm::header::functions::istext(newasm::mem::regs::tlr))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    if(!newasm::header::functions::istext(newasm::mem::regs::stl))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    newasm::handles::file_handle->set_name(newasm::header::functions::remq(newasm::mem::regs::tlr));
                    newasm::handles::file_handle->append_text(newasm::header::functions::remq(newasm::mem::regs::stl));
                    return 1;
                }
                //remove content
                if(newasm::mem::regs::fdx == 7)
                {
                    if(!newasm::header::functions::istext(newasm::mem::regs::tlr))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    /*if(!newasm::header::functions::istext(newasm::mem::regs::stl))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }*/
                    newasm::handles::file_handle->set_name(newasm::header::functions::remq(newasm::mem::regs::tlr));
                    newasm::handles::file_handle->remove_content();
                    return 1;
                }
                //read line
                if(newasm::mem::regs::fdx == 8)
                {
                    if(!newasm::header::functions::istext(newasm::mem::regs::tlr))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    if(!newasm::header::functions::isnumeric(newasm::mem::regs::stl))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    newasm::handles::file_handle->set_name(newasm::header::functions::remq(newasm::mem::regs::tlr));
                    newasm::handles::file_handle->read_line(std::stoi(newasm::mem::regs::stl),newasm::mem::regs::tlr);
                    newasm::mem::regs::tlr = static_cast<std::string>("\"")+newasm::mem::regs::tlr+static_cast<std::string>("\"");
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::unknown_fdx);
                return 1;
            }
            newasm::terminate(newasm::exit_codes::sysenter_fail);
            return 1;
        }
        //stack
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::stack))
        {
            if(newasm::header::data::callstkidx == 0)
            {
                newasm::terminate(newasm::exit_codes::invalid_memacc);
                return 1;
            }
            newasm::mem::regs::stk = newasm::mem::regs::stk + 1 + newasm::header::data::argc;
            newasm::header::data::argc = 0;
            newasm::header::data::callstkidx = 0;
            return 1;
        }
        //free
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::free__))
        {
            if(newasm::allocation_data == nullptr) // malloc nije upotrebljen // NoAlloc
            {
                newasm::terminate(newasm::exit_codes::malloc_err);
                return 1;
            }
            newasm::mem::regs::hea -= newasm::allocation_data->size;
            if(newasm::mem::regs::hea != newasm::allocation_data->heapsize_new - newasm::allocation_data->size) //neko je manualno dirao heap prije free
            {
                newasm::terminate(newasm::exit_codes::manual_heap);
                return 1;
            }
            newasm::mem::regs::heaptr = newasm::mem::regs::hea;
            delete newasm::allocation_data;
            newasm::allocation_data = nullptr;
            return 1;
        }
        //MATH OPERATIONS
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::add))
        {
            newasm::mem::regs::cr0 = newasm::mem::regs::cr0 + newasm::mem::regs::cr1;
            return 1;
        }
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::sub))
        {
            newasm::mem::regs::cr0 = newasm::mem::regs::cr0 - newasm::mem::regs::cr1;
            return 1;
        }
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::div))
        {
            newasm::mem::regs::cr0 = newasm::mem::regs::cr0 / newasm::mem::regs::cr1;
            return 1;
        }
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::mul))
        {
            newasm::mem::regs::cr0 = newasm::mem::regs::cr0 * newasm::mem::regs::cr1;
            return 1;
        }
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::exp))
        {
            newasm::mem::regs::cr0 = std::pow(newasm::mem::regs::cr0,newasm::mem::regs::cr1);
            return 1;
        }
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::log))
        {
            newasm::mem::regs::cr0 = std::log10(newasm::mem::regs::cr0) / std::log10(newasm::mem::regs::cr1);
            return 1;
        }
        //BITWISE OPERATIONS
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::and__))
        {
            newasm::mem::regs::br0 = newasm::mem::regs::br0 & newasm::mem::regs::br1;
            return 1;
        }
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::or__))
        {
            newasm::mem::regs::br0 = newasm::mem::regs::br0 | newasm::mem::regs::br1;
            return 1;
        }
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::not__))
        {
            newasm::mem::regs::br0 = ~newasm::mem::regs::br1;
            return 1;
        }
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::xor__))
        {
            newasm::mem::regs::br0 = newasm::mem::regs::br0 ^ newasm::mem::regs::br1;
            return 1;
        }
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::shl))
        {
            newasm::mem::regs::br0 = newasm::mem::regs::br0 << newasm::mem::regs::br1;
            return 1;
        }
        if(ins == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::shr))
        {
            newasm::mem::regs::br0 = newasm::mem::regs::br0 >> newasm::mem::regs::br1;
            return 1;
        }
        newasm::terminate(newasm::exit_codes::invalid_ins);//,wholeline);
        return 1;
    }
    
    int process_hndl(std::string tohandle, std::string procedure)
    {
        if(newasm::header::settings::lazy_evhndlr == false) if(!newasm::mem::functions::datavalid(procedure, newasm::mem::funcs))
        {
            newasm::terminate(newasm::exit_codes::invalid_evhndlr);
            return 1;
        }
        if(tohandle == newasm::core::lang_inf::events::identifiers__.at(newasm::core::lang_inf::events::exit))
        {
            newasm::handlers::exit_handler = procedure;
            return 1;
        }
        if(newasm::header::functions::ishex(tohandle))
        {
            if(newasm::stack::events.find(tohandle) != newasm::stack::events.end())
            {
                newasm::terminate(newasm::exit_codes::hndl_reassign);
                return 1;
            }
            newasm::stack::events[tohandle] = procedure;
            return 1;
        }
        newasm::terminate(newasm::exit_codes::unknown_event);
        return 1;
    }
    int process_text(std::string macroname, std::string symbol)
    {
        if(symbol != static_cast<std::string>("#"))
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
    void handle_threads(int method); int procline(std::string &line)
    {
        
        if(!newasm::header::execution_flow::exec_redirected) if(newasm::mem::functions::islbln(newasm::header::data::lastlndx))
        {
            return 1;
        }
        line = newasm::header::functions::trim(line);
        if(line[0] == ';')
        {
            return 0;
        }
        if(line.empty())
        {
            return 0;
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
        
        line = newasm::header::functions::remc(line);
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
            newasm::process_s(valid,line,"_",sec);
            if(valid)
            {
                return 1;
            }
        }

        /*if(newasm::header::functions::strfind(line,':'))
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
        }*/

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
                newasm::handle_threads(0);
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
                        //std::cout << "token is arg | token :: `" << linetokens.at(2) << "` | argc :: " << newasm::header::data::argc << std::endl;
                        newasm::header::data::argc++;
                        operand = newasm::mem::program_memory[newasm::header::data::callstkidx + 2 + newasm::header::functions::isargref(linetokens.at(2)).second];
                        //std::cout << "newasm::header::data::callstkidx is " << newasm::header::data::callstkidx << std::endl;
                        //std::cout << "operand is " << operand << std::endl;
                    }
                }
                return newasm::process_iso(line, instruction,linetokens.at(1),operand);
            }
            /*if(newasm::header::functions::strfind(line,'.')) if(newasm::header::functions::strfind(line,','))
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
            }*/
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
        //threads
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
                newasm::process_l(line,"_",arg,lineidx);
            }
        }
        catch(std::exception& err)
        {
            std::cout << "LABEL SEC :: " << err.what() << std::endl;
        }
    }
    void callproc(std::string name)
    {
        newasm::system::stoproc = 0;
        newasm::header::data::proc_now = true;
        newasm::mem::regs::prp = static_cast<std::string>("&") + name;
        auto it = newasm::mem::funcs.find(name);
        if(it != newasm::mem::funcs.end())
        {
            for(std::string &line : it->second)
            {
                if(newasm::system::stoproc == 1)
                {
                    newasm::system::stoproc = 0;
                    newasm::header::data::proc_now = false;
                    break;
                }
                newasm::header::data::lastln = line;
                newasm::procline(line);
                //std::cout << "Executed : " << line << std::endl;
            }
            newasm::header::data::proc_now = false;
        }
    }
    void copyproc(std::string name)
    {
        newasm::global::event_codeblock.clear();
        auto it = newasm::mem::funcs.find(name);
        if(it != newasm::mem::funcs.end())
        {
            for(std::string &line : it->second)
            {
                newasm::global::event_codeblock.push_back(line);
            }
        }
        return;
    }
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
                static_cast<std::string>("Unable to open the file: ") + static_cast<std::string>("'") + 
                /*newasm::header::constants::scripts_folder + */file + static_cast<std::string>("'"));
        }
        return 0;
    }
    int executechild()
    {
        std::ifstream internal_fileobject(/*newasm::header::constants::scripts_folder + */newasm::header::execution_flow::file);
        if(internal_fileobject.is_open())
        {
            std::string line;
            int lineidx = 1;

            while(std::getline(internal_fileobject, line))
            {
                if(newasm::system::terminated)
                {
                    newasm::header::execution_flow::exec_redirected = false;
                    newasm::execute(newasm::header::settings::script_file, newasm::header::execution_flow::entry_start_line, 1);
                    break;
                }

                newasm::header::data::lastln = line;
                newasm::header::data::lastlndx = lineidx;
                newasm::procline(line);
                lineidx++;
            }
            internal_fileobject.close();
            newasm::header::execution_flow::exec_redirected = false;
            newasm::execute(newasm::header::settings::script_file, newasm::header::execution_flow::entry_start_line, 1);
            return 1;
        }
        //else
        return 0;
    }
    int execute(std::string file, int startline, int proceed)
    {
        if(proceed == 0)
        {
            return proceed;
        }
        std::ifstream internal_fileobject(/*newasm::header::constants::scripts_folder + */file);
        if(internal_fileobject.is_open())
        {
            std::string line;
            int lineidx = 1;

            while(std::getline(internal_fileobject, line))
            {
                if(startline != newasm::code_stream::invalid_lnidx) if(!(lineidx >= startline))
                {
                    lineidx++;
                    //newasm::header::data::lastlndx = lineidx;
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
                if(newasm::header::execution_flow::exec_redirected)
                {
                    newasm::executechild();
                    break;
                }

                newasm::header::data::lastln = line;
                newasm::header::data::lastlndx = lineidx;
                newasm::procline(line);
                lineidx++;

                //////////
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
                /*newasm::header::constants::scripts_folder +*/ file + static_cast<std::string>("'"));
        }
        return 0;
    }
    void handle_threads(int method)
    {
        if(method == 0)
        for(auto i = newasm::threads::valid_threads.begin(); i != newasm::threads::valid_threads.end(); ++i)
        {
            if(newasm::threads::memory.at(*i)->contents.empty())
            {
                continue;
            }
            newasm::thread_line = true;
            newasm::threads::now = (*i);
            newasm::procline(*newasm::threads::memory.at(*i)->contents.begin());
            newasm::thread_line = false;
            newasm::threads::memory.at(*i)->contents.pop_front();
            //continue;
        }
    }
}