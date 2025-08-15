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

//literally kernel core
namespace newasm
{
    namespace kernel
    {
        int handleSysCall()
        {
            //tuple
            if(newasm::kernel::cfg::Tuple)
            if(newasm::threads::functions::get_sysenter() == newasm::core::lang_inf::refs::tuple)
            {
                if(newasm::mem::regs::fdx == 1) // tuple size
                {
                    if(!newasm::header::functions::isref(newasm::mem::regs::tlr.get_value()))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }

                    std::string tuple_name = newasm::mem::regs::tlr.get_value().substr(1);
                    if(!newasm::mem::functions::datavalid(tuple_name, newasm::mem::tuple))
                    {
                        newasm::terminate(newasm::exit_codes::invalid_tuple);
                        return 1;
                    }

                    newasm::mem::regs::tlr.set_value(std::to_string(newasm::mem::tuple.at(tuple_name).contents.size()));
                    return 1;
                }
                newasm::terminate(newasm::exit_codes::unknown_fdx);
                return 1;
            }
            //ext
            if(newasm::kernel::cfg::Extensions)
            if(newasm::threads::functions::get_sysenter() == newasm::core::lang_inf::refs::ext)
            {
                newasm::kernel::dynamic::CALL(newasm::mem::regs::dlx, std::to_string(newasm::mem::regs::fdx));
                return 1;
            }
            //thread
            if(newasm::kernel::cfg::Thread)
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
            if(newasm::kernel::cfg::Chrono)
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
            /*if(newasm::threads::functions::get_sysenter() == newasm::core::lang_inf::refs::exf)
            {
                if(true)
                {
                    return 1;
                }
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
            }*/
            //net
            //if(opr == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::net))
            if(newasm::kernel::cfg::Network)
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
            if(newasm::kernel::cfg::Memory)
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
            if(newasm::kernel::cfg::TextOperations)
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
            if(newasm::kernel::cfg::ContainerManipulation)
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
            if(newasm::kernel::cfg::IOStream)
            if(newasm::threads::functions::get_sysenter() == newasm::core::lang_inf::refs::ios)
            {
                //print text
                if(newasm::mem::regs::fdx == 1)
                {
                    if(!newasm::header::functions::istext(newasm::mem::regs::tlr))
                    {
                        newasm::header::functions::krnl("`tlr` ("+newasm::mem::regs::tlr.get_value()+") is not a valid value.");
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    std::string _chars_ = newasm::header::functions::remq(newasm::mem::regs::tlr);
                    if(newasm::thread_line)
                    {
                        newasm::threads::memory.at(newasm::threads::now)->output << _chars_;
                        newasm::threads::memory.at(newasm::threads::now)->output << newasm::syscalls::iostream::get_ref_val__2(newasm::mem::regs::stl);
                        return 1;
                    }

                    if(!newasm::header::flags::autobos)
                    {
                        if(newasm::mem::regs::bos.get_value() > _chars_.size())
                        {
                            newasm::header::functions::wrn("Incorrect `bos` size.");
                            newasm::mem::regs::bos.set_value(_chars_.size());
                        }
                        std::cout << _chars_.substr(0, newasm::mem::regs::bos);
                    }
                    else
                    {
                        std::cout << _chars_;
                    }
                    
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
            if(newasm::kernel::cfg::FileStream)
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
    }
}