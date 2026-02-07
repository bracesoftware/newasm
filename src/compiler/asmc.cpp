// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

// JIT Compiler for NewASM

namespace newasm
{
    namespace compiler
    {
        namespace data
        {
            bool aborted = false;
            std::string line;
            int lnidx = 0;
        }
        namespace fail
        {
            const int unmatched_syntax = 1;
            const int constant_redef = 2;
            const int invalid_krnlmod = 3;
            const int expected_token = 4;

            const std::unordered_map<int, std::string> id = {
                {unmatched_syntax, "UnmatchedSyntax"},
                {constant_redef, "ConstValRedefinition"},
                {invalid_krnlmod, "UnknownKernelModule"},
                {expected_token, "ExpectedToken"}
            };
        }

        void abort_(int exc)
        {
            std::cout << newasm::header::col::red; // << "\t";
            //std::cout << "Compilation aborted.\n";
            std::cout << "\tEncountered \"" << newasm::header::col::gray << newasm::header::style::underline;
            std::cout << newasm::compiler::fail::id.at(exc) << newasm::header::col::reset << newasm::header::col::red;
            std::cout << "\" on line:\n";
            std::cout << newasm::header::col::gray << "\t\t";

            if(!newasm::header::data::repl) std::cout << newasm::compiler::data::lnidx << " |  ";
            if(newasm::header::data::repl) std::cout << ">>> ";
            
            std::cout << newasm::header::col::reset
            << newasm::compiler::data::line << newasm::header::col::red << std::endl;
            //print_pointer(col);
            std::cout << newasm::header::col::reset << std::endl;

            newasm::compiler::data::aborted = true;
        }

        void abort(int exc)
        {
            try
            {
                abort_(exc);
            }
            catch(const std::exception& e)
            {
                std::cerr << "CompilerAbortZajebucnuoSiSeException ---> " << e.what() << '\n';
            }
            
            return;
        }

        std::vector<newasm::compiler::lineData> compiledCode;

        [[nodiscard]]
        inline newasm::compiler::lineData DO(std::string& line)
        {
            newasm::compiler::data::line = line;
            newasm::compiler::lineData lineCompiled;

            lineCompiled.raw = line;

            line = newasm::header::functions::remc(line);
            line = newasm::header::functions::trim(line);

            // compiling here:
            //empty
            if(line.empty())
            {
                lineCompiled.type = newasm::compiler::empty;
                return lineCompiled;
            }
            if(line.at(0) == ';')
            {
                lineCompiled.type = newasm::compiler::empty;
                return lineCompiled;
            }
            if(line.back() == ',')
            {
                newasm::compiler::abort(newasm::compiler::fail::expected_token);
                return lineCompiled;
            }
            //directive
            if(newasm::header::functions::parseDirective(line).first)
            {
                auto it = newasm::header::functions::parseDirective(line);
                //newasm::pp::impl::processDirectives(it.second.first, it.second.second);
                lineCompiled.type = newasm::compiler::directive;
                lineCompiled.tokens.push_back(it.second.first);
                lineCompiled.tokens.push_back(it.second.second);

                if(!newasm::header::functions::isalphanum(it.second.first))
                {
                    newasm::compiler::abort(newasm::compiler::fail::unmatched_syntax);
                }
                return lineCompiled;
            }
            //sealed label
            auto sealedLabelData = newasm::header::functions::parseSealedLabel(line);
            if(sealedLabelData.first)
            {
                lineCompiled.type = newasm::compiler::sealedLabel;
                lineCompiled.other = sealedLabelData.second;
                return lineCompiled;
            }
            //decorator
            if(newasm::header::functions::isdeco(line).first)
            {
                static const std::unordered_map<std::string, short> decorators = {
                    {"lock", newasm::decorators::id::LOCK},
                    {"volatile", newasm::decorators::id::VOLATILE},
                    {"transient", newasm::decorators::id::TRANSIENT}
                };
                lineCompiled.type = newasm::compiler::decorator;
                lineCompiled.tokens.push_back(newasm::header::functions::isdeco(line).second);

                auto decorator = lineCompiled.tokens.at(0);
                short constructionStatus = decorator.at(0) == '!' ? newasm::decorators::DESTRUCTIVE : newasm::decorators::CONSTRUCTIVE;
                std::string real_decorator;

                if(constructionStatus == newasm::decorators::DESTRUCTIVE)
                {
                    real_decorator = newasm::header::functions::trim(decorator.substr(1));
                }
                if(constructionStatus == newasm::decorators::CONSTRUCTIVE)
                {
                    real_decorator = newasm::header::functions::trim(decorator);
                }

                auto it = decorators.find(real_decorator);
                //std::cout << "constructionStatus: " << constructionStatus << std::endl;
                //std::cout << "newasm::header::functions::trim(decorator.substr(1)): `" << newasm::header::functions::trim(decorator.substr(1)) << "`" << std::endl;
                if(it != decorators.end())
                {
                    lineCompiled.letsDecorateVariables = newasm::kernel::makeHash(constructionStatus, it->second);
                }
                return lineCompiled;
            }
            //namespace
            if(newasm::header::functions::parseNamespace(line).first)
            {
                lineCompiled.type = newasm::compiler::namespace__;
                lineCompiled.tokens.push_back(newasm::header::functions::parseNamespace(line).second);
                return lineCompiled;
            }
            //closingbrace
            if(line == static_cast<std::string>("}"))
            {
                lineCompiled.type = newasm::compiler::closingBrace;
                return lineCompiled;
            }
            //macroterminator
            if(line == static_cast<std::string>("#"))
            {
                lineCompiled.type = newasm::compiler::macroTerminator;
                return lineCompiled;
            }
            //LAMBDA TERMINATOR
            auto lambda = newasm::header::functions::is_lambda(line);
            if(lambda.first) if(lambda.second == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::end))
            {
                lineCompiled.type = newasm::compiler::lambdaTerminator;
                return lineCompiled;
            }
            // SECTION MODIFIER
            if(line.at(0) == '.')
            {
                lineCompiled.type = newasm::compiler::sectionModifier;
                lineCompiled.tokens.push_back(newasm::header::functions::trim(line.substr(1)));
                auto& section = lineCompiled.tokens.at(0);
                static std::unordered_map<std::string, int> sections = {
                    {"start", newasm::code_stream::sections::start},
                    {"data", newasm::code_stream::sections::data},
                    {"text", newasm::code_stream::sections::text}
                };

                auto it = sections.find(section);
                if(it != sections.end())
                {
                    lineCompiled.whatCodeSection = it->second;
                }
                return lineCompiled;
            }
            // HANDLE MODIFIER
            #if 0
            if(newasm::header::functions::parseHandleModifier(line))
            {
                auto tmp = newasm::header::functions::split_fixed(line, '=');
                auto ev = tmp[0];
                auto pr = tmp[1];
                
                ev = newasm::header::functions::trim(ev);
                pr = newasm::header::functions::trim(pr);

                lineCompiled.type = newasm::compiler::handleModifier;
                lineCompiled.tokens.push_back(ev);
                lineCompiled.tokens.push_back(pr);
                
                return lineCompiled;
            }
            #endif
            // MACRO DECL
            auto data_macroDecl = newasm::header::functions::parseDataMacroDecl(line);
            if(data_macroDecl.first == 1)
            {
                auto ev = data_macroDecl.second.at(0);
                auto pr = data_macroDecl.second.at(1);

                lineCompiled.type = newasm::compiler::macroDecl;
                lineCompiled.tokens.push_back(ev);
                lineCompiled.tokens.push_back(pr);
                
                return lineCompiled; 
            }
            // DATA DECL
            if(data_macroDecl.first == 2)
            {
                auto typ = data_macroDecl.second.at(0);
                auto name = data_macroDecl.second.at(1);
                auto value = data_macroDecl.second.at(2);

                lineCompiled.type = newasm::compiler::dataDecl;
                lineCompiled.tokens.push_back(typ);
                lineCompiled.tokens.push_back(name);
                value = newasm::header::functions::parseBackslash(value);
                lineCompiled.tokens.push_back(value);

                auto it = newasm::inverted_types.find(typ);
                if(it != newasm::inverted_types.end())
                {
                    lineCompiled.parsedType = it->second;
                }

                if(typ == "obj")
                {
                    auto t = newasm::header::functions::tokenize__2(value);
                    if(t.size() == 2)
                    {
                        if(newasm::core::lang_inf::utils::iskeyword(t.at(0), newasm::core::lang_inf::utils::instance))
                        {
                            lineCompiled.type = newasm::compiler::classInstance;
                            lineCompiled.tokens.pop_back();
                            lineCompiled.tokens.push_back(t.at(1));
                            
                            if(!newasm::header::functions::isref(t.at(1)))
                            {
                                newasm::compiler::abort(newasm::compiler::fail::unmatched_syntax);
                            }
                            
                            return lineCompiled;
                        }
                    }
                }
                
                return lineCompiled;
            }
            // class instance
            // MACRO CALL
            if(line.at(0) == '$')
            {
                lineCompiled.type = newasm::compiler::macroCall;
                lineCompiled.tokens.push_back(newasm::header::functions::trim(line.substr(1)));
                return lineCompiled;
            }
            // CONDITIONALS
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
                    if(line.size() != idx__ + 2)
                    {
                        std::vector<std::string> linetokens_inline = newasm::common::tokenize(line.substr(0, idx__));
                        if(linetokens_inline.size() == 2)
                        {
                            lineCompiled.type = newasm::compiler::conditional;
                            lineCompiled.other = newasm::header::functions::trim(line.substr(idx__ + 2));
                            lineCompiled.tokens.push_back(linetokens_inline.at(0));
                            lineCompiled.tokens.push_back(linetokens_inline.at(1));

                            std::string& instruction = linetokens_inline.at(0);
                            std::string& otherShit = linetokens_inline.at(1);

                            auto it = newasm::inverted_ins.find(instruction);
                            if(it != newasm::inverted_ins.end())
                            {
                                lineCompiled.whatAmIDoing = it->second;
                            }

                            if(it == newasm::inverted_ins.end())
                            if(newasm::mem::functions::datavalid(instruction, newasm::mem::instructions))
                            {
                                lineCompiled.whatAmIDoing = INS_EXTERNAL;
                            }
                            //compiling eventNames cuz SPEED
                            if(lineCompiled.whatAmIDoing == newasm::core::lang_inf::evt)
                            {
                                auto it2 = newasm::events::eventNames.find(otherShit);
                                if(it2 != newasm::events::eventNames.end())
                                {
                                    lineCompiled.whatTheFuckAreEvents = it2->second;
                                }
                            }
                            return lineCompiled;
                        }
                        if(linetokens_inline.size() == 1) // process_is(line, linetokens_inline.at(0), linetokens_inline.at(1))
                        {
                            lineCompiled.type = newasm::compiler::conditional;
                            lineCompiled.other = newasm::header::functions::trim(line.substr(idx__ + 2));
                            lineCompiled.tokens.push_back(linetokens_inline.at(0));

                            std::string& instruction = linetokens_inline.at(0);

                            auto it = newasm::inverted_ins.find(instruction);
                            if(it != newasm::inverted_ins.end())
                            {
                                lineCompiled.whatAmIDoing = it->second;
                            }

                            if(it == newasm::inverted_ins.end())
                            if(newasm::mem::functions::datavalid(instruction, newasm::mem::instructions))
                            {
                                lineCompiled.whatAmIDoing = INS_EXTERNAL;
                            }
                            return lineCompiled;
                        }
                    }
                }
            }
            //INSTRUCTION
            std::vector<std::string> linetokens = newasm::common::tokenize(line);
            std::string instruction;
            if(!linetokens.empty())
            {
                instruction = linetokens.at(0);
            }
            if(newasm::compiler::iscomptins(instruction))
            {
                if(linetokens.size() == 3)
                {
                    lineCompiled.type = newasm::compiler::empty;
                    newasm::compiler::process_comptiso(instruction, linetokens.at(1), linetokens.at(2));
                    return lineCompiled;
                }
                if(linetokens.size() == 2)
                {
                    lineCompiled.type = newasm::compiler::empty;
                    newasm::compiler::process_comptis(instruction, linetokens.at(1));
                    return lineCompiled;
                }
            }
            if(newasm::header::functions::ishex(instruction))
            {
                for(std::unordered_map<int, std::string>::iterator i = newasm::opcodes::mem.begin(); i != newasm::opcodes::mem.end(); ++i)
                {
                    if(i->first == newasm::header::functions::hextoi(instruction))
                    {
                        instruction = i->second;
                    }
                }
            }
            if(!linetokens.empty())
            {
                lineCompiled.type = newasm::compiler::instruction;
                lineCompiled.tokens = linetokens;
                lineCompiled.tokens.at(0) = instruction;

                auto it = newasm::inverted_ins.find(instruction);
                if(it != newasm::inverted_ins.end())
                {
                    lineCompiled.whatAmIDoing = it->second;
                }

                if(it == newasm::inverted_ins.end())
                if(newasm::mem::functions::datavalid(instruction, newasm::mem::instructions))
                {
                    lineCompiled.whatAmIDoing = INS_EXTERNAL;
                }

                lineCompiled.priArgType = newasm::datatypes::symbol_name;
                lineCompiled.altArgType = newasm::datatypes::symbol_name;

                for(int i = 0; i < lineCompiled.tokens.size(); i++)
                {
                    lineCompiled.tokens.at(i) = newasm::header::functions::trim(lineCompiled.tokens.at(i));
                    lineCompiled.tokens.at(i) = newasm::compiler::parse_def(lineCompiled.tokens.at(i));

                    lineCompiled.tokens.at(i) = newasm::header::functions::parseBackslash(lineCompiled.tokens.at(i));
                    // optimisation
                    if(i == 1)
                    {
                        if(newasm::header::functions::isnumeric(lineCompiled.tokens.at(i)))
                        {
                            lineCompiled.priArgType = newasm::datatypes::number;
                        }
                        if(newasm::header::functions::isfloat(lineCompiled.tokens.at(i)))
                        {
                            lineCompiled.priArgType = newasm::datatypes::decimal;
                        }
                        if(newasm::header::functions::istext(lineCompiled.tokens.at(i)))
                        {
                            lineCompiled.priArgType = newasm::datatypes::text;
                        }
                        if(newasm::header::functions::ischar(lineCompiled.tokens.at(i)))
                        {
                            lineCompiled.priArgType = newasm::datatypes::character;
                        }
                    }
                    if(i == 2)
                    {
                        if(newasm::header::functions::isnumeric(lineCompiled.tokens.at(i)))
                        {
                            lineCompiled.altArgType = newasm::datatypes::number;
                        }
                        if(newasm::header::functions::isfloat(lineCompiled.tokens.at(i)))
                        {
                            lineCompiled.altArgType = newasm::datatypes::decimal;
                        }
                        if(newasm::header::functions::istext(lineCompiled.tokens.at(i)))
                        {
                            lineCompiled.altArgType = newasm::datatypes::text;
                        }
                        if(newasm::header::functions::ischar(lineCompiled.tokens.at(i)))
                        {
                            lineCompiled.altArgType = newasm::datatypes::character;
                        }
                    }
                }

                return lineCompiled;
            }

            // failed to compile
            newasm::compiler::abort(newasm::compiler::fail::unmatched_syntax);
            return lineCompiled;
        }
    }
}
