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
        constexpr uint32_t makeHash(int16_t a, int16_t b)
        {
            return (uint32_t(uint16_t(a)) << 16) | uint32_t(uint16_t(b));
        }

        inline int handleSysCall()
        {
            //ext
            if(newasm::kernel::cfg::Extensions)
            if(newasm::threads::functions::get_sysenter() == newasm::core::lang_inf::refs::ext)
            {
                newasm::kernel::dynamic::CALL(newasm::mem::regs::dlx, std::to_string(newasm::mem::regs::fdx));
                return 1;
            }
            if(newasm::kernel::cfg::ContainerManipulation) if(newasm::threads::functions::get_sysenter() == newasm::core::lang_inf::refs::cmanip)
            if(newasm::mem::regs::cpt == newasm::header::constants::inv_reg_val)
            {
                newasm::terminate(newasm::exit_codes::invalid_memacc);
                return 1;
            }
            int code = newasm::kernel::makeHash(newasm::threads::functions::get_sysenter(), newasm::mem::regs::fdx.get_value());
            if(newasm::kernel::cfg::Math) switch(code)
            {
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::math, 1): //sinus
                {
                    if(!newasm::header::functions::isnumeric(newasm::mem::regs::tlr.get_value()) &&
                    !newasm::header::functions::isfloat(newasm::mem::regs::tlr.get_value()))
                    {
                        newasm::header::functions::krnl("`tlr` (" + newasm::mem::regs::tlr.get_value() + ") has to be a numeric value.");
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    float value = std::stof(newasm::mem::regs::tlr.get_value());

                    newasm::mem::regs::tlr.set_value(std::to_string(std::sin(value)));
                    return 1;
                }
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::math, 2): //cosinus
                {
                    if(!newasm::header::functions::isnumeric(newasm::mem::regs::tlr.get_value()) &&
                    !newasm::header::functions::isfloat(newasm::mem::regs::tlr.get_value()))
                    {
                        newasm::header::functions::krnl("`tlr` (" + newasm::mem::regs::tlr.get_value() + ") has to be a numeric value.");
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    float value = std::stof(newasm::mem::regs::tlr.get_value());

                    newasm::mem::regs::tlr.set_value(std::to_string(std::cos(value)));
                    return 1;
                }
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::math, 3): //tangens
                {
                    if(!newasm::header::functions::isnumeric(newasm::mem::regs::tlr.get_value()) &&
                    !newasm::header::functions::isfloat(newasm::mem::regs::tlr.get_value()))
                    {
                        newasm::header::functions::krnl("`tlr` (" + newasm::mem::regs::tlr.get_value() + ") has to be a numeric value.");
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    float value = std::stof(newasm::mem::regs::tlr.get_value());

                    newasm::mem::regs::tlr.set_value(std::to_string(std::tan(value)));
                    return 1;
                }
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::math, 4): //sqrt
                {
                    if(!newasm::header::functions::isnumeric(newasm::mem::regs::tlr.get_value()) &&
                    !newasm::header::functions::isfloat(newasm::mem::regs::tlr.get_value()))
                    {
                        newasm::header::functions::krnl("`tlr` (" + newasm::mem::regs::tlr.get_value() + ") has to be a numeric value.");
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    float value = std::stof(newasm::mem::regs::tlr.get_value());

                    if(value < 0)
                    {
                        newasm::header::functions::krnl("`tlr` (" + newasm::mem::regs::tlr.get_value() + ") was negative, so it was converted with `::abs`.");
                        value = -value;
                    }

                    newasm::mem::regs::tlr.set_value(std::to_string(std::sqrt(value)));
                    return 1;
                }
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::math, 5): //absolute value
                {
                    if(!newasm::header::functions::isnumeric(newasm::mem::regs::tlr.get_value()) &&
                    !newasm::header::functions::isfloat(newasm::mem::regs::tlr.get_value()))
                    {
                        newasm::header::functions::krnl("`tlr` (" + newasm::mem::regs::tlr.get_value() + ") has to be a numeric value.");
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    float value = std::stof(newasm::mem::regs::tlr.get_value());

                    newasm::mem::regs::tlr.set_value(std::to_string(std::abs(value)));
                    return 1;
                }
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::math, 6): //cbrt
                {
                    if(!newasm::header::functions::isnumeric(newasm::mem::regs::tlr.get_value()) &&
                    !newasm::header::functions::isfloat(newasm::mem::regs::tlr.get_value()))
                    {
                        newasm::header::functions::krnl("`tlr` (" + newasm::mem::regs::tlr.get_value() + ") has to be a numeric value.");
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    float value = std::stof(newasm::mem::regs::tlr.get_value());

                    newasm::mem::regs::tlr.set_value(std::to_string(std::cbrt(value)));
                    return 1;
                }
                default:
                {
                    newasm::terminate(newasm::exit_codes::unknown_fdx);
                    return 1;
                }
            }
            if(newasm::kernel::cfg::Misc) switch(code)
            {
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::misc, 1): // misc, random
                {
                    if(!newasm::header::functions::isnumeric(newasm::mem::regs::tlr.get_value()))
                    {
                        newasm::header::functions::krnl("`tlr` (" + newasm::mem::regs::tlr.get_value() + ") has to be a numeric value.");
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    if(!newasm::header::functions::isnumeric(newasm::mem::regs::stl.get_value()))
                    {
                        newasm::header::functions::krnl("`stl` (" + newasm::mem::regs::stl.get_value() + ") has to be a numeric value.");
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }

                    int result = newasm::syscalls::misc::rand(std::stoi(newasm::mem::regs::tlr.get_value()), std::stoi(newasm::mem::regs::stl.get_value()));
                    newasm::mem::regs::tlr.set_value(std::to_string(result));
                    return 1;
                }
                default:
                {
                    newasm::terminate(newasm::exit_codes::unknown_fdx);
                    return 1;
                }
            }
            if(newasm::kernel::cfg::Crypto) switch(code)
            {
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::crypto, 1): // crypto, sha256
                {
                    if(!newasm::header::functions::istext(newasm::mem::regs::tlr.get_value()))
                    {
                        newasm::header::functions::krnl("`tlr` (" + newasm::mem::regs::tlr.get_value() + ") has to be a string value.");
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }

                    auto do_ = newasm::header::functions::remq(newasm::mem::regs::tlr.get_value());
                    auto result = newasm::syscalls::crypto::sha256(do_);
                    newasm::mem::regs::tlr.set_value(result);
                    newasm::mem::regs::tlr.add_end_("\"");
                    return 1;
                }
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::crypto, 2): // crypto, xor encrypt
                {
                    if(!newasm::header::functions::istext(newasm::mem::regs::tlr.get_value()))
                    {
                        newasm::header::functions::krnl("`tlr` (" + newasm::mem::regs::tlr.get_value() + ") has to be a string value.");
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    if(!newasm::header::functions::istext(newasm::mem::regs::stl.get_value()))
                    {
                        newasm::header::functions::krnl("`stl` (" + newasm::mem::regs::stl.get_value() + ") has to be a string value.");
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }

                    auto key = newasm::header::functions::remq(newasm::mem::regs::stl.get_value());

                    auto do_ = newasm::header::functions::remq(newasm::mem::regs::tlr.get_value());
                    auto result = newasm::syscalls::crypto::xor_encrypt(do_, key);
                    newasm::mem::regs::tlr.set_value(result);
                    newasm::mem::regs::tlr.add_end_("\"");
                    return 1;
                }
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::crypto, 3): // crypto, xor decrypt
                {
                    if(!newasm::header::functions::istext(newasm::mem::regs::tlr.get_value()))
                    {
                        newasm::header::functions::krnl("`tlr` (" + newasm::mem::regs::tlr.get_value() + ") has to be a string value.");
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    if(!newasm::header::functions::istext(newasm::mem::regs::stl.get_value()))
                    {
                        newasm::header::functions::krnl("`stl` (" + newasm::mem::regs::stl.get_value() + ") has to be a string value.");
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }

                    auto key = newasm::header::functions::remq(newasm::mem::regs::stl.get_value());

                    auto do_ = newasm::header::functions::remq(newasm::mem::regs::tlr.get_value());
                    auto result = newasm::syscalls::crypto::xor_decrypt(do_, key);
                    newasm::mem::regs::tlr.set_value(result);
                    newasm::mem::regs::tlr.add_end_("\"");
                    return 1;
                }
                default:
                {
                    newasm::terminate(newasm::exit_codes::unknown_fdx);
                    return 1;
                }
            }
            //http
            if(newasm::kernel::cfg::HTTP) switch(code)
            {
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::http, 1): // get http
                {
                    if(!newasm::header::functions::istext(newasm::mem::regs::tlr))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
          
                    newasm::start = std::chrono::high_resolution_clock::now();
                    std::string result = newasm::syscalls::http::get(
                        newasm::header::functions::remq(newasm::mem::regs::tlr.get_value())
                    );
                    newasm::end = std::chrono::high_resolution_clock::now();
                    newasm::network_deduction.push_back(newasm::end - newasm::start);
                    
                    auto removeexc = [](std::string& input) -> std::string {
                            for(int i = 0; i < input.size(); ++i)
                            {
                                if(input[i] == '"')
                                {
                                    input[i] = ' ';
                                }
                            }

                            input = '"' + input + '"';
                            return input;
                    };

                    newasm::mem::regs::tlr.set_value(removeexc(result));
                    return 1;
                }
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::http, 2): // post http
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

                    newasm::start = std::chrono::high_resolution_clock::now();
                    std::string result = newasm::syscalls::http::post(
                        newasm::header::functions::remq(newasm::mem::regs::tlr.get_value()),
                        newasm::header::functions::remq(newasm::mem::regs::stl.get_value())
                    );
                    newasm::end = std::chrono::high_resolution_clock::now();
                    newasm::network_deduction.push_back(newasm::end - newasm::start);

                    auto removeexc = [](std::string& input) -> std::string {
                            for(int i = 0; i < input.size(); ++i)
                            {
                                if(input[i] == '"')
                                {
                                    input[i] = ' ';
                                }
                            }

                            input = '"' + input + '"';
                            return input;
                    };

                    newasm::mem::regs::tlr.set_value(removeexc(result));
                    return 1;
                }
                default:
                {
                    newasm::terminate(newasm::exit_codes::unknown_fdx);
                    return 1;
                }
            }
            //tcp
            if(newasm::kernel::cfg::TCProtocol) switch(code)
            {
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::tcp, 1): // send tcp
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

                    newasm::start = std::chrono::high_resolution_clock::now();
                    int result = newasm::syscalls::tcp::send(
                        newasm::header::functions::remq(newasm::mem::regs::tlr.get_value()), 
                        newasm::header::functions::remq(newasm::mem::regs::stl.get_value())
                    );
                    newasm::end = std::chrono::high_resolution_clock::now();
                    newasm::network_deduction.push_back(newasm::end - newasm::start);

                    newasm::mem::regs::tlr.set_value(std::to_string(result));
                    return 1;
                }
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::tcp, 2): // recv tcp
                {
                    if(!newasm::header::functions::istext(newasm::mem::regs::tlr))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }

                    newasm::start = std::chrono::high_resolution_clock::now();
                    std::string result = newasm::syscalls::tcp::recv(
                        newasm::header::functions::remq(newasm::mem::regs::tlr.get_value())
                    );
                    newasm::end = std::chrono::high_resolution_clock::now();
                    newasm::network_deduction.push_back(newasm::end - newasm::start);

                    newasm::mem::regs::tlr.set_value("\"" + (result) + "\"");
                    return 1;
                }
                default:
                {
                    newasm::terminate(newasm::exit_codes::unknown_fdx);
                    return 1;
                }
            }
            //tuple
            if(newasm::kernel::cfg::Tuple) switch(code)
            {
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::tuple, 1): // tuple size
                {
                    if(!newasm::header::functions::isref(newasm::mem::regs::tlr.get_value()))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }

                    std::string tuple_name = newasm::mem::regs::tlr.get_value().substr(1);
                  
                    auto it = newasm::variables::ids.find(tuple_name);
                    if(it != newasm::variables::ids.end())
                    {
                        if(it->second.type == newasm::datatypes::tuple)
                        {
                            newasm::mem::regs::tlr.set_value(std::to_string(it->second.tuple->addr.size()));
                            return 1;
                        }
                    }
                    newasm::terminate(newasm::exit_codes::invalid_memacc);
                    return 1;
                }
                default:
                {
                    newasm::terminate(newasm::exit_codes::unknown_fdx);
                    return 1;
                }
            }
            //ctx
            if(newasm::kernel::cfg::Context) switch(code)
            {
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::ctx, 1): // ctx size
                {
                    if(!newasm::header::functions::isref(newasm::mem::regs::tlr.get_value()))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }

                    std::string context_name = newasm::mem::regs::tlr.get_value().substr(1);
                  
                    auto it = newasm::variables::ids.find(context_name);
                    if(it != newasm::variables::ids.end())
                    {
                        if(it->second.type == newasm::datatypes::mycontext)
                        {
                            newasm::mem::regs::tlr.set_value(std::to_string(it->second.context->addr.size()));
                            return 1;
                        }
                    }
                    newasm::terminate(newasm::exit_codes::invalid_memacc);
                    return 1;
                }
                default:
                {
                    newasm::terminate(newasm::exit_codes::unknown_fdx);
                    return 1;
                }
            }
            //thread
            if(newasm::kernel::cfg::Thread) switch(code)
            {
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::thread, 1): //print output of the thread
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
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::thread, 2): //get returned val from thread
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
                default:
                {
                    newasm::terminate(newasm::exit_codes::invalid_syntax);
                    return 1;
                }
            }
            //chrono
            if(newasm::kernel::cfg::Chrono) switch(code)
            {
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::chrono, 1): //getyear
                {
                    newasm::mem::regs::tlr = std::to_string(newasm::chrono::year());
                    return 1;
                }
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::chrono, 2): //getmonth
                {
                    newasm::mem::regs::tlr = std::to_string(newasm::chrono::month());
                    return 1;
                }
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::chrono, 3): //day
                {
                    newasm::mem::regs::tlr = std::to_string(newasm::chrono::day());
                    return 1;
                }
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::chrono, 4): //hour
                {
                    newasm::mem::regs::tlr = std::to_string(newasm::chrono::hour());
                    return 1;
                }
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::chrono, 5): //min
                {
                    newasm::mem::regs::tlr = std::to_string(newasm::chrono::minute());
                    return 1;
                }
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::chrono, 6): //sec
                {
                    newasm::mem::regs::tlr = std::to_string(newasm::chrono::second());
                    return 1;
                }
                default:
                {
                    newasm::terminate(newasm::exit_codes::unknown_fdx);
                    return 1;
                }
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
            if(newasm::kernel::cfg::Network) switch(code)
            {
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::net, 1): //download files
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
            if(newasm::kernel::cfg::Memory) switch(code)
            {
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::mem, 1):
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
                default:
                {
                    newasm::terminate(newasm::exit_codes::unknown_fdx);
                    return 1;
                }
            }
            //text manipulation
            //if(opr == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::txtop))
            if(newasm::kernel::cfg::TextOperations) switch(code)
            {
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::txtop, 1): //concat
                {
                    if(!newasm::header::functions::istext(newasm::mem::regs::tlr.get_value()))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }
                    if(!newasm::header::functions::istext(newasm::mem::regs::stl.get_value()))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }

                    newasm::syscalls::txtop::impl::concat();
                    return 1;
                }
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::txtop, 2)://trim
                {
                    if(!newasm::header::functions::istext(newasm::mem::regs::tlr.get_value()))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }

                    newasm::syscalls::txtop::impl::trim();
                    return 1;
                }
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::txtop, 3)://tokenize
                {
                    if(!newasm::header::functions::istext(newasm::mem::regs::tlr.get_value()))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }

                    newasm::syscalls::txtop::impl::tokenize();
                    return 1;
                }
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::txtop, 4)://format
                {
                    if(!newasm::header::functions::istext(newasm::mem::regs::tlr.get_value()))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return 1;
                    }

                    std::string string__ = newasm::header::functions::remq(newasm::mem::regs::tlr.get_value());
                    newasm::syscalls::txtop::impl::fmat(string__);
                    newasm::mem::regs::tlr.set_value(string__);
                    newasm::mem::regs::tlr.add_end_("\"");
                    return 1;
                }
                default:
                {
                    newasm::terminate(newasm::exit_codes::unknown_fdx);
                    return 1;
                }
            }
            //////////// container manipulation
            //if(opr == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::cmanip))
            if(newasm::kernel::cfg::ContainerManipulation) switch(code)
            {
                //clear
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::cmanip, 1):
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
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::cmanip, 2):
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
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::cmanip, 3):
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
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::cmanip, 4):
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
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::cmanip, 5):
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
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::cmanip, 6):
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
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::cmanip, 7):
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
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::cmanip, 8):
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
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::cmanip, 9):
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
                default:
                {
                    newasm::terminate(newasm::exit_codes::unknown_fdx);
                    return 1;
                }
            }
            ///// input-output stream
            //if(opr == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::ios))
            if(newasm::kernel::cfg::IOStream) switch(code)
            {
                //print text
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::ios, 1):
                {
                    if(!newasm::header::functions::istext(newasm::mem::regs::tlr))
                    {
                        newasm::header::functions::krnl("`tlr` ("+newasm::mem::regs::tlr.get_value()+") is not a valid value.");
                        //std::cout << "DEBUG -> " << newasm::thread_line << " -> " << newasm::threads::now << std::endl;
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
                            //newasm::header::functions::wrn("Incorrect `bos` size.");
                            newasm::mem::regs::bos.set_value(_chars_.size());
                            newasm::terminate(newasm::exit_codes::seg_fault);
                        }
                        if(false) newasm::_std::write(_chars_.substr(0, newasm::mem::regs::bos));
                        newasm::native_jit::print(_chars_.substr(0, newasm::mem::regs::bos.get_value()));
                    }
                    else
                    {
                        newasm::native_jit::print(_chars_);
                    }
                    
                    newasm::syscalls::iostream::out_bopr(newasm::mem::regs::stl);
                    return 1;
                }
                //print numbers and floats
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::ios, 2):
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
                    if(0) std::cout << newasm::mem::regs::tlr;// << std::endl;
                    newasm::native_jit::print(newasm::mem::regs::tlr.get_value()); //-> code generation
                    //newasm::mem::functions::out_bopr(newasm::mem::regs::stl);
                    newasm::syscalls::iostream::out_bopr(newasm::mem::regs::stl);
                    return 1;
                }
                //input text
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::ios, 3):
                {
                    newasm::perf::inputWasteTimer.start();
                    std::getline(std::cin, newasm::mem::regs::tlr.ref_value());
                    newasm::perf::inputWasteTimer.stop();
                    newasm::mem::regs::tlr.add_end_("\"");
                    return 1;
                }
                //input numbers and floats
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::ios, 4):
                {
                    newasm::perf::inputWasteTimer.start();
                    std::cin >> newasm::mem::regs::tlr;
                    newasm::perf::inputWasteTimer.stop();
                    if(!newasm::header::functions::isnumeric(newasm::mem::regs::tlr) && !newasm::header::functions::isfloat(newasm::mem::regs::tlr))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);//,wholeline);
                        return 1;
                    }
                    return 1;
                }
                //print values of builtin operands
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::ios, 5):
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
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::ios, 6):
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
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::ios, 7):
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
                //input characters
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::ios, 8):
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
                default:
                {
                    newasm::terminate(newasm::exit_codes::unknown_fdx);
                    return 1;
                }
            }
            //////file stream
            //if(opr == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::fs))
            if(newasm::kernel::cfg::FileStream) switch(code)
            {
                //create folder
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::fs, 1):
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
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::fs, 2):
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
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::fs, 3):
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
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::fs, 4):
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
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::fs, 5):
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
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::fs, 6):
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
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::fs, 7):
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
                case newasm::kernel::makeHash(newasm::core::lang_inf::refs::fs, 8):
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
                default:
                {
                    newasm::terminate(newasm::exit_codes::unknown_fdx);
                    return 1;
                }
            }
            newasm::terminate(newasm::exit_codes::sysenter_fail);
            return 1;
        }
    }
}