// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#ifndef __newasm_included
    #error haha 
#endif

namespace newasm
{
    namespace syscalls
    {
        namespace txtop
        {
            std::string formatted_string;
            void main()
            {
                return;
            }
            namespace impl
            {
                std::string quotes = static_cast<std::string>("\"");
                inline int concat()
                {
                    newasm::mem::regs::tlr = quotes+newasm::header::functions::remq(newasm::mem::regs::tlr) + 
                    newasm::header::functions::remq(newasm::mem::regs::stl)+quotes;
                    return 1;
                }
                inline int trim()
                {
                    newasm::mem::regs::tlr = quotes + newasm::header::functions::trim(newasm::header::functions::remq(newasm::mem::regs::tlr)) + quotes;
                    return 1;
                }

                inline void stoi()
                {
                    std::string str;
                    if(newasm::header::functions::isnumeric(newasm::mem::regs::tlr.get_value()))
                    {
                        newasm::mem::regs::rax.set_value(std::stoi(newasm::mem::regs::tlr.get_value()));
                        return;
                    }
                    if(newasm::header::functions::istext(newasm::mem::regs::tlr.get_value()))
                    {
                        str = newasm::header::functions::remq(newasm::mem::regs::tlr.get_value());
                        if(!newasm::header::functions::isnumeric(str))
                        {
                            //newasm::mem::regs::tlr.set_value("0");
                            newasm::mem::regs::rax.set_value(0);
                            return;
                        }
                        newasm::mem::regs::rax.set_value(std::stoi(str));
                        return;
                    }
                    return;
                }
                inline void stof()
                {
                    std::string str;
                    if(newasm::header::functions::isfloat(newasm::mem::regs::tlr.get_value()))
                    {
                        newasm::mem::regs::rbx.set_value(std::stof(newasm::mem::regs::tlr.get_value()));
                        return;
                    }
                    if(newasm::header::functions::istext(newasm::mem::regs::tlr.get_value()))
                    {
                        str = newasm::header::functions::remq(newasm::mem::regs::tlr.get_value());
                        if(!newasm::header::functions::isfloat(str))
                        {
                            //newasm::mem::regs::tlr.set_value("0");
                            newasm::mem::regs::rbx.set_value(0);
                            return;
                        }
                        newasm::mem::regs::rbx.set_value(std::stof(str));
                        return;
                    }
                    return;
                }
                inline void at()
                {
                    std::string str = newasm::header::functions::remq(newasm::mem::regs::tlr.get_value());
                    int idx = newasm::mem::regs::rax.get_value();
                    if(idx < 0 or idx >= str.size())
                    {
                        newasm::terminate(newasm::exit_codes::seg_fault);
                        return;
                    }
                    char c = str[idx];
                    std::stringstream ss;
                    ss << "'" << c << "'";
                    newasm::mem::regs::tlr.set_value(ss.str());
                    return;
                }

                inline void tokenize()
                {
                    auto addq = [](std::string& str) -> std::string {
                        str = quotes + str + quotes;
                        return str;
                    };

                    std::string string__ = newasm::header::functions::trim(newasm::header::functions::remq(newasm::mem::regs::tlr));
                    std::string tuple__, token_temp__;

                    std::vector<std::string> tokens;
                    std::istringstream iss(string__);
                    std::string token;

                    while(iss >> token)
                    {
                        tokens.push_back(token);
                    }

                    tuple__ = "(";

                    for(int i = 0; i < tokens.size(); ++i)
                    {
                        token_temp__ = addq(tokens[i]);
                        tuple__.append(token_temp__);
                        if(i + 1 == tokens.size())
                        {
                            tuple__.append(")");
                            continue;
                        }
                        if(i + 1 != tokens.size())
                        {
                            tuple__.append(",");
                            continue;
                        }
                    }

                    newasm::mem::regs::tlr.set_value(tuple__);

                    return;
                }

                // formats stuff "My name is {namespace::name}, her name is {context("key")}"
                inline void fmat(std::string& newasm_string)
                {
                    std::string to_fmat, before_brace, after_brace;
                    int brace_open, brace_close;
                    newasm::syscalls::txtop::formatted_string.clear();

                    brace_open = newasm_string.find('{');
                    brace_close = newasm_string.find('}');

                    if((brace_open == std::string::npos) or (brace_close == std::string::npos))
                    {
                        return;
                    }

                    if(!(brace_open < brace_close))
                    {
                        return;
                    }

                    to_fmat = newasm_string.substr(brace_open + 1, brace_close - brace_open - 1);
                    before_brace = newasm_string.substr(0, brace_open);
                    after_brace = newasm_string.substr(brace_close + 1);

                    //std::cout << "before_brace: " << before_brace << std::endl;
                    //std::cout << "to_fmat: " << to_fmat << std::endl;
                    //std::cout << "after_brace: " << after_brace << std::endl;

                    to_fmat = newasm::header::functions::trim(to_fmat);
                    newasm::runtime::functions::parse(to_fmat);

                    if(newasm::header::functions::istext(to_fmat))
                    {
                        to_fmat = newasm::header::functions::remq(to_fmat);
                    }

                    newasm_string = before_brace + to_fmat + after_brace;
                    if(newasm_string.find('{') && newasm_string.find('}'))
                    {
                        fmat(newasm_string);
                    }
                    return;
                }
            }
        }
    }
}