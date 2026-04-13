// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

__newasm_LOAD_PACKAGE_MODULE(compiler, {
    //setup goes here
});

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

            std::vector<std::string> symbol_map;
            std::vector<std::string> namespace_stack;
            std::vector<std::string> sealed_labels;

            constinit bool objectDecl = false;
            constinit bool JIT_mode = false;

            unsigned int ErrorCount = 0;
        }
        namespace fail
        {
            const int unmatched_syntax = 1;
            const int constant_redef = 2;
            const int invalid_krnlmod = 3;
            const int expected_token = 4;
            const int unknown_attrib = 5;
            const int unknown_register = 6;
            const int symbol_redecl = 7;
            const int unknown_label = 8;
            const int invalid_symbol = 9;
            const int retc_fail = 10;

            const std::unordered_map<int, std::string> id = {
                {unmatched_syntax, "UnmatchedSyntax"},
                {constant_redef, "ConstValRedefinition"},
                {invalid_krnlmod, "UnknownKernelModule"},
                {expected_token, "ExpectedToken"},
                {unknown_attrib, "UnknownAttribute"},
                {unknown_register, "UnknownRegister"},
                {symbol_redecl, "SymbolRedeclaration"},
                {unknown_label, "InvalidLabelAddress"},
                {invalid_symbol, "InvalidSymbol"},
                {retc_fail, "CannotReturnToCaseJumpTable"}
            };
        }

        void abort_(int exc)
        {
            NewASM::compiler::data::ErrorCount++;
            std::cout << newasm::header::col::red; // << "\t";
            //std::cout << "Compilation aborted.\n";
            std::cout << "\tEncountered \"" << newasm::header::col::gray << newasm::header::style::underline;
            std::cout << newasm::compiler::fail::id.at(exc) << newasm::header::col::reset << newasm::header::col::red;
            std::cout << "\" on line:\n";
            std::cout << newasm::header::col::gray << "\t\t";

            auto& idx = newasm::compiler::data::lnidx;
            try
            {
                if(!newasm::header::data::repl) std::cout << newasm::forLinker::getFile(idx - 1) << ":" << newasm::forLinker::getLine(idx - 1) << " |  ";
            }
            catch(const std::exception& e)
            {
                if(!newasm::header::data::repl) std::cout << "JIT cache |  ";
            }
            
            if(newasm::header::data::repl) std::cout << "JIT buffer >>>  ";
            
            std::cout << newasm::header::col::reset
            << newasm::compiler::data::line << newasm::header::col::red << std::endl;
            //print_pointer(col);
            std::cout << newasm::header::col::reset << std::endl;

            newasm::compiler::data::aborted = true;
        }

        inline void abort(int exc)
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
        std::vector<newasm::compiler::lineData> caseJumpTable;
        namespace utils
        {
            template<char delim>
			FORCE_INLINE inline bool fix_spaces(std::string& s)
			{
				if(s.empty() || s[0] != delim)
				{
					return false;
				}

				size_t i = 1;
				while(i < s.size() && std::isspace(static_cast<unsigned char>(s[i])))
				{
					++i;
				}

				if(i > 1)
				{
					s.erase(1, i - 1);
				}

				return true;
			}

            template<char a, char b>
			FORCE_INLINE inline std::string lenofop(const std::string& s)
			{
				if(s.empty() or s[0] != a)
				{
                    return s;
                }

				size_t i = 1;
				while(i < s.size() && std::isspace((unsigned char)s[i])) i++;
				if(i >= s.size() || s[i] != b)
                {
                    return s;
                }
				i++;
				while(i < s.size() && std::isspace((unsigned char)s[i])) i++;

				std::string text = s.substr(i);
				return std::string(1, a) + std::string(1, b) + text;
			}

            FORCE_INLINE inline void fixString(std::string& s)
            {
                fix_spaces<'&'>(s);
				fix_spaces<'#'>(s);
				fix_spaces<'~'>(s);
				s = lenofop<'$', '-'>(s);
                s = lenofop<'*', '/'>(s); //for env
            }

            inline int getEvalMode(const std::string& s, newasm::compiler::EvalMode& mode)
            {
                if(s.at(0) == '*' and s.at(1) != '/')
                {
                    auto it = newasm::mem::regs::identifiers.find(newasm::header::functions::trim(s.substr(1)));
                    if(it == newasm::mem::regs::identifiers.end())
                    {
                        newasm::compiler::abort(newasm::compiler::fail::unknown_register);
                        return newasm::runtime::evalModes::regDeref;
                    }
                    mode.argType = newasm::datatypes::number;
                    mode.argInt = it->second;
                    return newasm::runtime::evalModes::regDeref;
                }
                if(s.front() == '#')
                {
                    return newasm::runtime::evalModes::addressOf;
                }
                if(s.at(0) == '*' and s.at(1) == '/')
                {
                    //std::cout << "Ovo je bajo env -> " << s << std::endl;
                    return newasm::runtime::evalModes::environmentVariable;
                }
                if(s.at(0) == '$' and s.at(1) == '-')//if(newasm::header::functions::issizeof(s).first)
                {
                    return newasm::runtime::evalModes::sizeOf;
                }
                if(newasm::header::functions::isalphanum(s))
                {
                    //std::cout << "Ovo je bajo alfanumerično -> " << s << std::endl;
                    return newasm::runtime::evalModes::valueOf;
                }
                if(newasm::header::functions::parseNamespaceSegments(s).first)
                {
                    return newasm::runtime::evalModes::valueOfNamespacedVar;
                }
                if(newasm::header::functions::isref(s))
                {
                    return newasm::runtime::evalModes::referenceOfNamespacedVar;
                }
                auto n = newasm::header::functions::checkTupleFormat(s);
                if(n.first)
                {
                    mode.argString2 = n.second.first;
                    mode.argString = n.second.second;
                    if(NewASM::header::functions::isnumeric(mode.argString))
                    {
                        mode.argType = NewASM::datatypes::number;
                        mode.argInt = std::stoi(mode.argString);
                    }
                    else if(NewASM::header::functions::istext(mode.argString))
                    {
                        mode.argType = NewASM::datatypes::text;
                        mode.argString = NewASM::header::functions::remq(mode.argString);
                    }
                    else
                    {
                        mode.argType = NewASM::datatypes::symbol_name;
                    }
                    return newasm::runtime::evalModes::valueOfNamespacedTupleOrContext;
                }
                return 0;
            }
        }

        @nodiscard
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
            //if(line == NEWASM_JUMP_POINT)
            if(line.front() == ':')
            {
                lineCompiled.type = newasm::compiler::labelJumpPoint;
                lineCompiled.other = newasm::header::functions::trim(line.substr(1));
                if(!newasm::header::functions::isalphanum(lineCompiled.other))
                {
                    newasm::compiler::abort(newasm::compiler::fail::unmatched_syntax);
                }
                return lineCompiled;
            }
            //attribute
            auto h = newasm::header::functions::parseAttribute(line);
            if(h.first)
            {
                auto& v = h.second;
                lineCompiled.attribute = 0;
                lineCompiled.type = newasm::compiler::attribute;
                for(int i = 0; i < v.size(); ++i)
                {
                    auto& attrib = v.at(i);
                    auto it = newasm::core::lang_inf::attributes::ids.find(attrib);
                    if(it == newasm::core::lang_inf::attributes::ids.end())
                    {
                        lineCompiled.type = newasm::compiler::empty;
                        newasm::compiler::abort(newasm::compiler::fail::unknown_attrib);
                        return lineCompiled;
                    }
                    lineCompiled.attribute |= it->second;
                }
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
                auto p = newasm::pp::impl::runtimeDirectives.find(it.second.first);
                if(p == newasm::pp::impl::runtimeDirectives.end())
                {
                    newasm::compiler::abort(newasm::compiler::fail::unmatched_syntax);
                    return lineCompiled;
                }
                lineCompiled.priInt = p->second;
                return lineCompiled;
            }
            //sealed label
            auto sealedLabelData = newasm::header::functions::parseSealedLabel(line);
            if(sealedLabelData.first)
            {
                lineCompiled.type = newasm::compiler::sealedLabel;
                lineCompiled.other = sealedLabelData.second;
                newasm::compiler::data::sealed_labels.push_back(sealedLabelData.second);
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
                auto namespace_name = newasm::header::functions::parseNamespace(line).second;
                lineCompiled.tokens.push_back(namespace_name);
                if(namespace_name[0] == '!' && newasm::header::functions::trim(namespace_name.substr(1)) == newasm::compiler::data::namespace_stack.back())
                {
                    //else u gonna get a runtime exception,cuz namespace mangling is a runtime operation
                    //tell me about an insane vm design
                    newasm::compiler::data::namespace_stack.pop_back();
                }
                else
                {
                    newasm::compiler::data::namespace_stack.push_back(namespace_name);
                }
                return lineCompiled;
            }
            //closingbrace
            if(line == static_cast<std::string>("}"))
            {
                lineCompiled.type = newasm::compiler::closingBrace;
                newasm::compiler::data::objectDecl = false; //no namespace checking within classes or objects
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
            auto checkCollisions = [](std::string name) -> void {//error checking at compile time
                if(!newasm::compiler::data::JIT_mode) if(newasm::compiler::data::objectDecl)
                {
                    return;
                }
                if(newasm::compiler::data::JIT_mode) if(newasm::header::data::struct_now)
                {
                    return;
                }
                std::string temp_name;
                temp_name.append(name);
                for(int i = 0; i < newasm::compiler::data::namespace_stack.size(); ++i)
                {
                    temp_name.append("--");
                    temp_name.append(newasm::compiler::data::namespace_stack[i]);
                }
                auto& vec = newasm::compiler::data::symbol_map;
                bool exists = std::find(vec.begin(), vec.end(), temp_name) != vec.end();
                if(exists)
                {
                    newasm::compiler::abort(newasm::compiler::fail::symbol_redecl);
                }
                newasm::compiler::data::symbol_map.push_back(temp_name);
            };
            // DATA DECL
            if(data_macroDecl.first == 2)
            {
                auto typ = data_macroDecl.second.at(0);
                auto name = data_macroDecl.second.at(1);
                auto value = data_macroDecl.second.at(2);
                newasm::compiler::utils::fixString(value);
                lineCompiled.priEvalMode.type = newasm::compiler::utils::getEvalMode(value, lineCompiled.priEvalMode);

                if(!newasm::header::functions::isalphanum(name))
                {
                    newasm::compiler::abort(newasm::compiler::fail::unmatched_syntax);
                }

                checkCollisions(name);

                lineCompiled.type = newasm::compiler::dataDecl;
                lineCompiled.tokens.push_back(typ);
                lineCompiled.tokens.push_back(name);
                value = newasm::header::functions::parseBackslash(value);
                lineCompiled.tokens.push_back(value);
                if(newasm::header::functions::isnumeric(value))
                {
                    lineCompiled.priArgType = newasm::datatypes::number;
                    lineCompiled.priInt = std::stoi(value);
                }
                if(newasm::header::functions::isfloat(value))
                {
                    lineCompiled.priArgType = newasm::datatypes::decimal;
                    lineCompiled.priFloat = std::stof(value);
                }
                if(newasm::header::functions::ischar(value))
                {
                    lineCompiled.priArgType = newasm::datatypes::character;
                    lineCompiled.priChar = newasm::header::functions::remsq(value)[0];
                }
                if(newasm::header::functions::istext(value))
                {
                    lineCompiled.priArgType = newasm::datatypes::text;
                    lineCompiled.priString = newasm::header::functions::remq(value);
                }

                auto it = newasm::inverted_types.find(typ);
                if(it != newasm::inverted_types.end())
                {
                    lineCompiled.parsedType = it->second;
                }

                if(typ == "class")
                {
                    newasm::compiler::data::objectDecl = true;
                }
                if(typ == "obj")
                {
                    newasm::compiler::data::objectDecl = true;
                    auto t = newasm::header::functions::tokenize__2(value);
                    if(t.size() == 2)
                    {
                        if(newasm::core::lang_inf::utils::iskeyword(t.at(0), newasm::core::lang_inf::utils::instance))
                        {
                            newasm::compiler::data::objectDecl = false;
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
                        auto CompileCaseLine = [&](decltype(lineCompiled)& l) -> void {
                            if(l.other == OPEN_BRACE_STR)
                            {
                                l.caseLineArgType = NewASM::datatypes::tokenOpenBrace;
                                return;
                            }
                            if(l.other == NIL_STR)
                            {
                                l.caseLineArgType = NewASM::datatypes::NIL;
                                return;
                            }
                        };
                        std::vector<std::string> linetokens_inline = newasm::common::tokenize(line.substr(0, idx__));
                        if(linetokens_inline.size() == 2)
                        {
                            lineCompiled.type = newasm::compiler::conditional;
                            lineCompiled.other = newasm::header::functions::trim(line.substr(idx__ + 2));//part after ->
                            lineCompiled.tokens.push_back(linetokens_inline.at(0));
                            lineCompiled.tokens.push_back(linetokens_inline.at(1));
                            CompileCaseLine(lineCompiled);

                            std::string& instruction = linetokens_inline.at(0);
                            std::string& otherShit = linetokens_inline.at(1);
                            newasm::compiler::utils::fixString(otherShit);
                            lineCompiled.priEvalMode.type = newasm::compiler::utils::getEvalMode(otherShit, lineCompiled.priEvalMode);

                            auto it = newasm::inverted_ins.find(instruction);
                            if(it != newasm::inverted_ins.end())
                            {
                                lineCompiled.whatAmIDoing = it->second;
                            }

                            auto it_ = newasm::mem::regs::identifiers.find(otherShit);
                            if(it_ != newasm::mem::regs::identifiers.end())
                            {
                                lineCompiled.whatAreRegistersLol = it_->second;
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
                                //parse numbers
                                if(newasm::header::functions::ishex(otherShit))
                                {
                                    otherShit = std::to_string(newasm::header::functions::hextoi(otherShit));
                                }
                                if(newasm::header::functions::isbin(otherShit))
                                {
                                    otherShit = std::to_string(newasm::header::functions::bintoi(otherShit));
                                }
                                if(newasm::header::functions::isnumeric(otherShit))
                                {
                                    lineCompiled.priArgType = newasm::datatypes::number;
                                    lineCompiled.priInt = std::stoi(otherShit);
                                }
                            }
                            //checking for thread names cuz SPEED
                            if(lineCompiled.whatAmIDoing == newasm::core::lang_inf::thread__)
                            {
                                checkCollisions(otherShit);
                            }
                            //creating caseJumpTable
                            if(lineCompiled.whatAmIDoing == newasm::core::lang_inf::case__)
                            {
                                auto cc = DO(lineCompiled.other);
                                newasm::compiler::caseJumpTable.push_back(cc);
                                lineCompiled.caseTableAddress = newasm::compiler::caseJumpTable.size() - 1;
                            }
                            return lineCompiled;
                        }
                        if(linetokens_inline.size() == 1) // process_is(line, linetokens_inline.at(0), linetokens_inline.at(1))
                        {
                            lineCompiled.type = newasm::compiler::conditional;
                            lineCompiled.other = newasm::header::functions::trim(line.substr(idx__ + 2));
                            lineCompiled.tokens.push_back(linetokens_inline.at(0));

                            CompileCaseLine(lineCompiled);

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
            for(int i = 0; i < linetokens.size(); ++i)
            {
                newasm::compiler::utils::fixString(linetokens[i]);
                if(i == 1)
                {
                    lineCompiled.priEvalMode.type = newasm::compiler::utils::getEvalMode(linetokens[i], lineCompiled.priEvalMode);
                }
                if(i == 2)
                {
                    lineCompiled.altEvalMode.type = newasm::compiler::utils::getEvalMode(linetokens[i], lineCompiled.altEvalMode);
                }
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
                    if(newasm::header::functions::ishex(lineCompiled.tokens.at(i)))
                    {
                        lineCompiled.tokens.at(i) = std::to_string(newasm::header::functions::hextoi(lineCompiled.tokens.at(i)));
                    }
                    if(newasm::header::functions::isbin(lineCompiled.tokens.at(i)))
                    {
                        lineCompiled.tokens.at(i) = std::to_string(newasm::header::functions::bintoi(lineCompiled.tokens.at(i)));
                    }
                    if(i == 1)
                    {
                        lineCompiled.priEvalMode.type = newasm::compiler::utils::getEvalMode(lineCompiled.tokens.at(i), lineCompiled.priEvalMode);
                        //compiling krnl modules cuz SPEED
                        if(lineCompiled.whatAmIDoing == newasm::core::lang_inf::sysenter)
                        {
                            auto suf = lineCompiled.tokens.at(i);
                            if(!newasm::header::functions::istext(suf))
                            {
                                lineCompiled.krnlMod = INVALID_INS;
                                continue;
                            }
                            suf = newasm::header::functions::remq(suf);

                            auto kernel_module = newasm::inverted_kernel.find(suf);
                            if(kernel_module == newasm::inverted_kernel.end())
                            {
                                newasm::compiler::abort(newasm::compiler::fail::invalid_krnlmod);
                                return lineCompiled;
                            }
                            lineCompiled.krnlMod = kernel_module->second;
                        }
                        //compiling proc names CUZ SPEED
                        if(lineCompiled.whatAmIDoing == newasm::core::lang_inf::proc)
                        {
                            checkCollisions(lineCompiled.tokens.at(i));
                        }
                        auto it_ = newasm::mem::regs::identifiers.find(lineCompiled.tokens.at(i));
                        if(it_ != newasm::mem::regs::identifiers.end())
                        {
                            lineCompiled.whatAreRegistersLol = it_->second;
                        }
                        if(lineCompiled.tokens.at(i).size() >= 3)
                        {
                            auto regName = newasm::header::functions::trim(lineCompiled.tokens.at(i).substr(1));
                            if(lineCompiled.tokens.at(i).front() == '*' and newasm::header::functions::isalphanum(regName))
                            {
                                auto it__ = newasm::mem::regs::identifiers.find(regName);
                                if(it__ == newasm::mem::regs::identifiers.end())
                                {
                                    newasm::compiler::abort(newasm::compiler::fail::unknown_register);
                                    return lineCompiled;
                                }
                                if(it__ != newasm::mem::regs::identifiers.end())
                                {
                                    lineCompiled.priArgType = newasm::datatypes::symbol_name;//newasm::datatypes::_regDeref;
                                    lineCompiled.priInt = it__->second;
                                }
                            }
                        }
                        if(newasm::header::functions::isnumeric(lineCompiled.tokens.at(i)))
                        {
                            lineCompiled.priArgType = newasm::datatypes::number;
                            lineCompiled.priInt = std::stoi(lineCompiled.tokens.at(i));
                        }
                        if(newasm::header::functions::isfloat(lineCompiled.tokens.at(i)))
                        {
                            lineCompiled.priArgType = newasm::datatypes::decimal;
                            lineCompiled.priFloat = std::stof(lineCompiled.tokens.at(i));
                        }
                        if(newasm::header::functions::istext(lineCompiled.tokens.at(i)))
                        {
                            lineCompiled.priArgType = newasm::datatypes::text;
                            lineCompiled.priString = newasm::header::functions::remq(lineCompiled.tokens.at(i));
                        }
                        if(newasm::header::functions::ischar(lineCompiled.tokens.at(i)))
                        {
                            lineCompiled.priArgType = newasm::datatypes::character;
                            lineCompiled.priChar = newasm::header::functions::remsq(lineCompiled.tokens.at(i))[0];
                        }

                        if(
                            (lineCompiled.whatAmIDoing == NewASM::core::lang_inf::free__) or
                            (lineCompiled.whatAmIDoing == NewASM::core::lang_inf::pop)
                        )
                        {
                            if(lineCompiled.tokens.at(i) == NIL_STR)
                            {
                                lineCompiled.priArgType = NewASM::datatypes::NIL;
                            }
                        }
                    }
                    if(i == 2)
                    {
                        lineCompiled.altEvalMode.type = newasm::compiler::utils::getEvalMode(lineCompiled.tokens.at(i), lineCompiled.altEvalMode);
                        if(lineCompiled.tokens.at(i).size() >= 3)
                        {
                            auto regName = newasm::header::functions::trim(lineCompiled.tokens.at(i).substr(1));
                            if(lineCompiled.tokens.at(i).front() == '*' and newasm::header::functions::isalphanum(regName))
                            {
                                auto it__ = newasm::mem::regs::identifiers.find(regName);
                                if(it__ == newasm::mem::regs::identifiers.end())
                                {
                                    newasm::compiler::abort(newasm::compiler::fail::unknown_register);
                                    return lineCompiled;
                                }
                                if(it__ != newasm::mem::regs::identifiers.end())
                                {
                                    lineCompiled.altArgType = newasm::datatypes::symbol_name;//newasm::datatypes::_regDeref;
                                    lineCompiled.altInt = it__->second;
                                }
                            }
                        }
                        if(newasm::header::functions::isnumeric(lineCompiled.tokens.at(i)))
                        {
                            lineCompiled.altArgType = newasm::datatypes::number;
                            lineCompiled.altInt = std::stoi(lineCompiled.tokens.at(i));
                        }
                        if(newasm::header::functions::isfloat(lineCompiled.tokens.at(i)))
                        {
                            lineCompiled.altArgType = newasm::datatypes::decimal;
                            lineCompiled.altFloat = std::stof(lineCompiled.tokens.at(i));
                        }
                        if(newasm::header::functions::istext(lineCompiled.tokens.at(i)))
                        {
                            lineCompiled.altArgType = newasm::datatypes::text;
                            lineCompiled.altString = newasm::header::functions::remq(lineCompiled.tokens.at(i));
                        }
                        if(newasm::header::functions::ischar(lineCompiled.tokens.at(i)))
                        {
                            lineCompiled.altArgType = newasm::datatypes::character;
                            lineCompiled.altChar = newasm::header::functions::remsq(lineCompiled.tokens.at(i))[0];
                        }

                        if(lineCompiled.whatAmIDoing == NewASM::core::lang_inf::mov)
                        {
                            auto& k = lineCompiled.tokens.at(i);
                            if(k.front() == '{' && k.back() == '}' && k.size() >= 3)
                            {
                                std::string number_part = newasm::header::functions::trim(k.substr(1, k.size() - 2));
                                k = "{"_str + newasm::compiler::parse_def(number_part) + "}"_str;
                                lineCompiled.VirtualMemoryAccess = true;
                            }
                            if(k.front() == '[' && k.back() == ']' && k.size() >= 3)
                            {
                                std::string number_part = newasm::header::functions::trim(k.substr(1, k.size() - 2));
                                k = "["_str + newasm::compiler::parse_def(number_part) + "]"_str;
                            }
                        }
                    }
                }

                return lineCompiled;
            }

            // failed to compile
            newasm::compiler::abort(newasm::compiler::fail::unmatched_syntax);
            return lineCompiled;
        }

        inline void Optimize(newasm::compiler::lineData& line, int idx)
        {
            if(newasm::compiler::compiledCode.empty())
            {
                return;
            }
            auto& lastLine = newasm::compiler::compiledCode.back();
            //helper func
            auto OptDescription = [&](const std::string& text) -> void {
                if(NewASM::header::data::LogCompilerOptimizations)
                {
                    std::cout << "\t  " << newasm::header::col::magenta;
                    try
                    {
                        std::cout << newasm::forLinker::getFile(idx - 1) << ":" << newasm::forLinker::getLine(idx - 1);
                    }
                    catch(const std::exception& e)
                    {
                        std::cout << "cached code";
                    }
                    std::cout << newasm::header::col::gray;
                    std::cout << ": " << text << ": " << newasm::header::col::magenta << line.raw;
                    std::cout << '\n' << newasm::header::col::reset;
                }
                return;
            };
            //actual optimizations
            if(
                (line == lastLine) or
                (line.whatAmIDoing == newasm::core::lang_inf::jmp and lastLine.whatAmIDoing == newasm::core::lang_inf::jmp)
            ) //double code
            {
                bool IsRedundant = (//remove only specific instructions
                    line.whatAmIDoing == newasm::core::lang_inf::mov or
                    line.whatAmIDoing == newasm::core::lang_inf::nop or
                    line.whatAmIDoing == newasm::core::lang_inf::zero or
                    line.whatAmIDoing == newasm::core::lang_inf::jmp or
                    line.whatAmIDoing == newasm::core::lang_inf::retc or
                    line.whatAmIDoing == newasm::core::lang_inf::ret or
                    line.whatAmIDoing == newasm::core::lang_inf::retn or
                    line.whatAmIDoing == newasm::core::lang_inf::retf or
                    line.whatAmIDoing == newasm::core::lang_inf::align or
                    line.whatAmIDoing == newasm::core::lang_inf::sel or
                    line.whatAmIDoing == newasm::core::lang_inf::lea or
                    line.whatAmIDoing == newasm::core::lang_inf::switch__
                );
                if(IsRedundant)
                {
                    OptDescription("peephole optimization, removed redundant double code");
                    line.type = newasm::compiler::empty;
                }
                return;
            }
            if(line.whatAmIDoing == newasm::core::lang_inf::mov)//mov rax, *rax
            {
                if(
                    line.whatAreRegistersLol == line.altEvalMode.argInt and
                    line.altEvalMode.type == newasm::runtime::evalModes::regDeref and
                    line.altEvalMode.argType == newasm::datatypes::number
                )
                {
                    //logging the optimization
                    OptDescription("peephole optimization, removed redundant code");
                    //actual optimization xd
                    line.type = newasm::compiler::empty;
                    return;
                }
            }
            if(line.whatAmIDoing == newasm::core::lang_inf::rem)//useless instructions
            {
                //logging the optimization
                OptDescription("peephole optimization, removed dead code");
                //actual optimization xd
                line.type = newasm::compiler::empty;
                return;
            }
            return;
        }

        template<typename _Type>
        concept _Compileable = (
            std::is_same_v<_Type, std::string> or
            std::is_same_v<_Type, char> or
            std::is_same_v<_Type, const char*> or
            std::is_same_v<_Type, char*> or
            std::is_same_v<_Type, const std::string>
        );

        template<newasm::compiler::_Compileable T>
        inline auto DO(T line)
        {
            std::string l = line;
            return DO(l);
        }
    }
}
