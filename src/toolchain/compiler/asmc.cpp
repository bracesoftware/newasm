// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

module(compiler, {
    //setup goes here
});

// JIT Compiler for NewASM

namespace newasm
{
    namespace OptimizerData
    {
        constinit signed int Section = -1;
        constinit bool JmpUsed = false;
        //constinit unsigned int LastLine = 0;
        constinit unsigned int LastLineIdx = 0;
    }
    namespace compiler
    {
        namespace data
        {
            bool aborted = false;
            std::string line;
            std::string DeclaringMacroName;
            constinit bool DeclaringMacro = false;
            int lnidx = 0;
            constinit bool RecursionProtection = false;

            typedef std::vector<std::string> StringVec;

            StringVec symbol_map;
            StringVec namespace_stack;
            StringVec sealed_labels;
            StringVec NamespaceStackA;
            std::unordered_map<std::string, std::vector<newasm::compiler::lineData>> MacroTable;

            constinit bool objectDecl = false;
            constinit bool JIT_mode = false;
            constinit bool CompileTimeMangle = false;

            unsigned int ErrorCount = 0;
            unsigned int OptimizationCount = 0;

            constinit bool IfResult = true;
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
            constinit const int label_redef = 11;
            constinit const int unexpected_term = 12;
            constinit const int linker_err = 13;
            constinit const int already_linked = 14;
            constinit const int redundant_try = 15;
            constinit const int redundant_catch = 16;
            constinit const int unexpected_nmst = 17;
            constinit const int invalid_nilu = 18;

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
                {retc_fail, "CannotReturnToCaseJumpTable"},
                {label_redef, "LabelAlreadyExists"},
                {unexpected_term, "UnexpectedMacroTerminator"},
                {linker_err, "LinkerError"},
                {already_linked, "LibraryAlreadyLinked"},
                {redundant_try, "RedundantTry"},
                {redundant_catch, "RedundantCatch"},
                {unexpected_nmst, "UnexpectedNamespaceTerminator"},
                {invalid_nilu, "InvalidNilUsage"}
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
                if(!newasm::header::data::repl) std::cout << newasm::forLinker::getFile(idx) << ":" << newasm::forLinker::getLine(idx) << " |  ";
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
            inline std::string MangleName(const std::vector<std::string>& data, const std::string& final_str)
            {
                auto vec = data;
                vec.push_back(__DATE__);
                vec.push_back(__TIME__);
                int seed = 0;
                std::hash<std::string> hasher;

                for(const std::string& s : vec)
                {
                    seed ^= hasher(s) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                }

                std::string result = newasm::_std::to_string(seed);
                result = result + "NEWASM"_str + newasm::_std::to_string(vec.size()) + final_str;
                return result;
            }

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
                auto o = newasm::header::functions::parseObject(s);
                mode.UsingStruct = o.first;
                mode.structString1 = o.second.first;
                mode.structString2 = o.second.second;

                if(s.at(0) == '*' and s.at(1) != '/')
                {
                    auto DEREF_TEXT = newasm::header::functions::trim(s.substr(1));
                    if(DEREF_TEXT == THIS_STR)
                    {
                        return newasm::runtime::evalModes::thisDeref;
                    }
                    auto it = newasm::mem::regs::identifiers.find(DEREF_TEXT);
                    if(it == newasm::mem::regs::identifiers.end())
                    {
                        newasm::compiler::abort(newasm::compiler::fail::unknown_register);
                        return newasm::runtime::evalModes::regDeref;
                    }
                    mode.argType = newasm::datatypes::number;
                    mode.argInt = it->second;
                    return newasm::runtime::evalModes::regDeref;
                }
                if(newasm::header::functions::trim(s) == "#")
                {
                    newasm::compiler::abort(newasm::compiler::fail::expected_token);
                    return newasm::runtime::evalModes::addressOf;
                }
                if(s.front() == '#' and s.size() >= 2)
                {
                    std::string addrOfWhat = newasm::header::functions::trim(s.substr(1));
                    mode.argString2 = addrOfWhat;
                    if(mode.argString2 == THIS_STR)
                    {
                        mode.UsingThisPtr = true;
                    }
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
                    if(mode.argString2 == THIS_STR)
                    {
                        mode.UsingThisPtr = true;
                    }
                    mode.argString = n.second.second;
                    mode.argString = NewASM::header::functions::trim(mode.argString);
                    if(NewASM::header::functions::isnumeric(mode.argString))
                    {
                        mode.argType = NewASM::datatypes::number;
                        mode.argInt = std::stoi(mode.argString);
                    }
                    else if(NewASM::header::functions::istext(mode.argString))
                    {
                        mode.argString = newasm::header::functions::parseBackslash(mode.argString);
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
            newasm::perf::Jitc.start();
            $defer
                newasm::perf::Jitc.stop();
            $
            newasm::compiler::data::line = line;
            newasm::compiler::lineData lineCompiled;

            lineCompiled.raw = line;
            auto& lc = lineCompiled;

            line = newasm::header::functions::remc(line);
            line = newasm::header::functions::trim(line);

            // compiling here:
            std::vector<std::string> linetokens = newasm::common::tokenize(line);
            if(!linetokens.empty())
            {
                unsigned int id = newasm::compiler::iscomptins(linetokens.at(0));
                if(id == newasm::compiler::link__)
                {
                    newasm::compiler::abort(newasm::compiler::fail::linker_err);
                    lc.type = newasm::compiler::empty;
                    return lineCompiled;
                }
             
                if(linetokens.size() == 1)
                if(
                    id == newasm::compiler::fi__
                )
                {
                    newasm::compiler::process_compti(linetokens.at(0));
                    lineCompiled.type = newasm::compiler::empty;
                    return lineCompiled;
                }
            }

            if(!newasm::compiler::data::IfResult)
            {
                lineCompiled.type = newasm::compiler::empty;
                return lineCompiled;
            }

            if(!linetokens.empty())
            {
                unsigned int id = newasm::compiler::iscomptins(linetokens.at(0));
                if(linetokens.size() == 2)
                if(
                    id == newasm::compiler::ifdef__ or
                    id == newasm::compiler::ifndef__
                )
                {
                    newasm::compiler::process_comptis(linetokens.at(0), linetokens.at(1));
                    lineCompiled.type = newasm::compiler::empty;
                    return lineCompiled;
                }
            }

            //desc
            auto desc = newasm::header::functions::parseDescriptor(line);
            if(desc.first)
            {
                NewASM::header::functions::removeDescriptor(line);
                //nil desc
                if(desc.second == NIL_STR or desc.second.empty())
                {
                    lc.Descriptor.type = NewASM::Const::InsDescriptor::Types::NIL;
                }
                else if(newasm::header::functions::isnumeric(desc.second))
                {
                    lc.Descriptor.type = NewASM::Const::InsDescriptor::Types::DATA;
                    lc.Descriptor.descInt = std::stoi(desc.second);
                    lc.Descriptor.descType = newasm::datatypes::number;
                }
                else if(newasm::header::functions::isfloat(desc.second))
                {
                    lc.Descriptor.type = NewASM::Const::InsDescriptor::Types::DATA;
                    lc.Descriptor.descFloat = std::stof(desc.second);
                    lc.Descriptor.descType = newasm::datatypes::decimal;
                }
                else if(newasm::header::functions::ischar(desc.second))
                {
                    lc.Descriptor.type = NewASM::Const::InsDescriptor::Types::DATA;
                    lc.Descriptor.descChar = newasm::header::functions::remsq(desc.second)[0];
                    lc.Descriptor.descType = newasm::datatypes::character;
                }
                else if(newasm::header::functions::istext(desc.second))
                {
                    lc.Descriptor.type = NewASM::Const::InsDescriptor::Types::DATA;
                    lc.Descriptor.descString = newasm::header::functions::remq(desc.second);
                    lc.Descriptor.descType = newasm::datatypes::text;
                }
                //invalid
                else
                {
                    newasm::compiler::abort(newasm::compiler::fail::invalid_symbol);
                    lc.type = newasm::compiler::empty;
                    return lc;
                }
            }
            
            //macroterminator
            if(line == static_cast<std::string>("#"))
            {
                lineCompiled.type = newasm::compiler::macroTerminator;
                if(newasm::header::data::repl)
                {
                    newasm::compiler::abort(newasm::compiler::fail::unexpected_term);
                    return lineCompiled;
                }
                if(!newasm::compiler::data::DeclaringMacro)
                {
                    newasm::compiler::abort(newasm::compiler::fail::unexpected_term);
                    return lineCompiled;
                }
                newasm::compiler::data::DeclaringMacro = false;
                return lineCompiled;
            }
            if(!newasm::compiler::data::RecursionProtection) if(newasm::compiler::data::DeclaringMacro)
            {
                lineCompiled.type = newasm::compiler::empty;
                newasm::compiler::data::RecursionProtection = true;
                auto _BYTECODE = DO(line);
                newasm::compiler::data::RecursionProtection = false;
                auto& n = newasm::compiler::data::DeclaringMacroName;
                _BYTECODE.MacroComponent = true;
                _BYTECODE.SourceMacroName = n;
                newasm::compiler::data::MacroTable[n].push_back(_BYTECODE);
                return lineCompiled;
            }
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
                if(newasm::compiler::data::DeclaringMacro)
                {
                    newasm::compiler::abort(newasm::compiler::fail::unmatched_syntax);
                    lineCompiled.type = newasm::compiler::empty;
                    return lineCompiled;
                }
                lineCompiled.type = newasm::compiler::labelJumpPoint;
                lineCompiled.other = newasm::header::functions::trim(line.substr(1));
                if(!newasm::header::functions::isalphanum(lineCompiled.other))
                {
                    newasm::compiler::abort(newasm::compiler::fail::unmatched_syntax);
                    return lineCompiled;
                }
                if(NewASM::compiler::data::CompileTimeMangle)
                {
                    NewASM::compiler::data::CompileTimeMangle = false;
                    if(!NewASM::compiler::data::NamespaceStackA.empty())
                    {
                        lineCompiled.other = NewASM::compiler::utils::MangleName(newasm::compiler::data::NamespaceStackA, lineCompiled.other);
                    }
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
                    //compile time attributes
                    if(it->second == NewASM::core::lang_inf::attributes::MANGLE__)
                    {
                        NewASM::compiler::data::CompileTimeMangle = true;
                        NewASM::compiler::data::NamespaceStackA.clear();
                        continue;
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
                    {"private", newasm::decorators::id::LOCK},
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
            auto c = newasm::header::functions::parseNamespace(line);
            if(c.first)
            {
                lineCompiled.type = newasm::compiler::namespace__;
                auto namespace_name = c.second;
                if(NewASM::compiler::data::CompileTimeMangle)
                {
                    lineCompiled.type = newasm::compiler::empty;
                    newasm::compiler::data::NamespaceStackA.push_back(namespace_name);
                    if(!NewASM::header::functions::isalphanum(namespace_name))
                    {
                        newasm::compiler::abort(newasm::compiler::fail::unmatched_syntax);
                    }
                    return lineCompiled;
                }
                lineCompiled.tokens.push_back(namespace_name);
                if(namespace_name[0] == '!' && newasm::header::functions::trim(namespace_name.substr(1)) == newasm::compiler::data::namespace_stack.back())
                {
                    //else u gonna get a runtime exception,cuz namespace mangling is a runtime operation
                    //tell me about an insane vm design
                    newasm::compiler::data::namespace_stack.pop_back();
                    lineCompiled.priString = newasm::header::functions::trim(namespace_name.substr(1));
                    lineCompiled.priInt = NewASM::Namespaces::Destruction;
                }
                else
                {
                    newasm::compiler::data::namespace_stack.push_back(namespace_name);
                    lineCompiled.priString = newasm::header::functions::trim(namespace_name);
                    lineCompiled.priInt = NewASM::Namespaces::Construction;
                }
                if(!NewASM::header::functions::isalphanum(lineCompiled.priString))
                {
                    newasm::compiler::abort(newasm::compiler::fail::unmatched_syntax);
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
            //LAMBDA TERMINATOR
            auto lambda = newasm::header::functions::is_lambda(line);
            if(lambda.first) if(lambda.second == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::end))
            {
                lineCompiled.type = newasm::compiler::lambdaTerminator;
                return lineCompiled;
            }
            // SECTION MODIFIER
            if(line.at(0) == '.' and line.size() > 1)
            {
                lineCompiled.type = newasm::compiler::sectionModifier;
                lineCompiled.tokens.push_back(newasm::header::functions::trim(line.substr(1)));
                auto& section = lineCompiled.tokens.at(0);
                static std::unordered_map<std::string, int> sections = {
                    {"start", newasm::code_stream::sections::start},
                    {"data", newasm::code_stream::sections::data},
                    {"text", newasm::code_stream::sections::start}
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
                #if 0
                lineCompiled.type = newasm::compiler::macroDecl;
                lineCompiled.tokens.push_back(ev);
                lineCompiled.tokens.push_back(pr);
                #endif
                lineCompiled.type = newasm::compiler::macroDecl;
                if(newasm::header::data::repl)
                {
                    newasm::compiler::abort(newasm::compiler::fail::unexpected_term);
                    return lineCompiled;
                }
                if(newasm::compiler::data::DeclaringMacro)
                {
                    lineCompiled.type = newasm::compiler::empty;
                    newasm::compiler::abort(newasm::compiler::fail::unmatched_syntax);
                    return lineCompiled;
                }
                if(pr != "#"_str)
                {
                    newasm::compiler::abort(newasm::compiler::fail::unmatched_syntax);
                    return lineCompiled;
                }

                newasm::compiler::data::DeclaringMacro = true;
                newasm::compiler::data::DeclaringMacroName = ev;
                auto& t = newasm::compiler::data::MacroTable;
                if(t.find(newasm::compiler::data::DeclaringMacroName) != t.end())
                {
                    newasm::compiler::abort(newasm::compiler::fail::symbol_redecl);
                }
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
                lc.CompileTime.dataDecl = true;
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

                if(typ == "class"_str)
                {
                    newasm::compiler::data::objectDecl = true;
                }
                if(typ == "obj"_str)
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
                if(lc.parsedType == newasm::core::lang_inf::typenames::list__)
                {
                    auto h = newasm::inverted_types.find(value);
                    if(h != newasm::inverted_types.end())
                    {
                        lc.priInt = h->second;
                    }
                }
                return lineCompiled;
            }
            // class instance
            // MACRO CALL
            if(line.at(0) == '$')
            {
                lineCompiled.type = newasm::compiler::macroCall;
                lineCompiled.other = newasm::header::functions::trim(line.substr(1));
                if(newasm::header::data::repl)
                {
                    newasm::compiler::abort(newasm::compiler::fail::unexpected_term);
                    return lineCompiled;
                }
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
                            lc.Class = NewASM::Const::InstructionClass::DedicatedClass::MediumConditional;
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
                                    otherShit = newasm::_std::to_string(newasm::header::functions::hextoi(otherShit));
                                }
                                if(newasm::header::functions::isbin(otherShit))
                                {
                                    otherShit = newasm::_std::to_string(newasm::header::functions::bintoi(otherShit));
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
                            lc.Class = NewASM::Const::InstructionClass::DedicatedClass::TinyConditional;
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
            static std::string instruction;
            linetokens.clear();
            linetokens = newasm::common::tokenize(line);
            if(!linetokens.empty())
            {
                instruction = linetokens.at(0);
            }
            if(!linetokens.empty()) for(int i = 0; i < linetokens.size(); ++i)
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
                if(linetokens.size() == 1)
                {
                    lineCompiled.type = newasm::compiler::empty;
                    newasm::compiler::process_compti(instruction);
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
            if(!linetokens.empty()) //ins
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

                auto SIZEE = lineCompiled.tokens.size();
                lineCompiled.Class = SIZEE;
                if(
                    lineCompiled.Class < NewASM::Const::InstructionClass::Tiny or
                    lineCompiled.Class > NewASM::Const::InstructionClass::Large
                )
                {
                    newasm::compiler::abort(newasm::compiler::fail::unmatched_syntax);
                }

                for(size_t i = 0; i < SIZEE; i++)
                {
                    lineCompiled.tokens.at(i) = newasm::header::functions::trim(lineCompiled.tokens.at(i));
                    lineCompiled.tokens.at(i) = newasm::compiler::parse_def(lineCompiled.tokens.at(i));

                    lineCompiled.tokens.at(i) = newasm::header::functions::parseBackslash(lineCompiled.tokens.at(i));
                    // optimisation
                    if(newasm::header::functions::ishex(lineCompiled.tokens.at(i)))
                    {
                        lineCompiled.tokens.at(i) = newasm::_std::to_string(newasm::header::functions::hextoi(lineCompiled.tokens.at(i)));
                    }
                    if(newasm::header::functions::isbin(lineCompiled.tokens.at(i)))
                    {
                        lineCompiled.tokens.at(i) = newasm::_std::to_string(newasm::header::functions::bintoi(lineCompiled.tokens.at(i)));
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
                            if(lc.whatAmIDoing == newasm::core::lang_inf::mov)
                            {
                                lc.Class = NewASM::Const::InstructionClass::DedicatedClass::MovReg;
                            }
                        }
                        if(lineCompiled.tokens.at(i).size() >= 3)
                        {
                            auto regName = newasm::header::functions::trim(lineCompiled.tokens.at(i).substr(1));
                            if(lineCompiled.tokens.at(i).front() == '*' and newasm::header::functions::isalphanum(regName))
                            {
                                auto it__ = newasm::mem::regs::identifiers.find(regName);
                                if(it__ == newasm::mem::regs::identifiers.end() and regName != THIS_STR)
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

                        //inlining this pointer
                        if(lineCompiled.tokens.at(i) == THIS_STR)
                        {
                            lineCompiled.priArgType = newasm::datatypes::ThisPtr;
                        }
                        else if(newasm::header::functions::isnumeric(lineCompiled.tokens.at(i)))
                        {
                            lineCompiled.priArgType = newasm::datatypes::number;
                            lineCompiled.priInt = std::stoi(lineCompiled.tokens.at(i));
                        }
                        else if(newasm::header::functions::isfloat(lineCompiled.tokens.at(i)))
                        {
                            lineCompiled.priArgType = newasm::datatypes::decimal;
                            lineCompiled.priFloat = std::stof(lineCompiled.tokens.at(i));
                        }
                        else if(newasm::header::functions::istext(lineCompiled.tokens.at(i)))
                        {
                            lineCompiled.priArgType = newasm::datatypes::text;
                            lineCompiled.priString = newasm::header::functions::remq(lineCompiled.tokens.at(i));
                        }
                        else if(newasm::header::functions::ischar(lineCompiled.tokens.at(i)))
                        {
                            lineCompiled.priArgType = newasm::datatypes::character;
                            lineCompiled.priChar = newasm::header::functions::remsq(lineCompiled.tokens.at(i))[0];
                        }
                        else if(lineCompiled.tokens.at(i) == NIL_STR)
                        {
                            if(
                                (lineCompiled.whatAmIDoing == NewASM::core::lang_inf::free__) or
                                (lineCompiled.whatAmIDoing == NewASM::core::lang_inf::fetch__) or
                                (lineCompiled.whatAmIDoing == NewASM::core::lang_inf::catch__) or
                                (lineCompiled.whatAmIDoing == NewASM::core::lang_inf::pop) or
                                (lineCompiled.whatAmIDoing == newasm::core::lang_inf::resb__)
                            )
                            {
                                lineCompiled.priArgType = NewASM::datatypes::NIL;
                            }
                            else
                            {
                                newasm::compiler::abort(newasm::compiler::fail::invalid_nilu);
                            }
                        }

                        if(lineCompiled.whatAmIDoing == newasm::core::lang_inf::malloc__)
                        {
                            auto e = newasm::header::functions::isvmemsize(lc.tokens.at(i));
                            if(e.first)
                            {
                                lc.VirtualMemoryAccess = true;
                                lc.priInt = e.second;
                            }
                        }
                    }
                    if(i == 2)
                    {
                        auto p = newasm::header::functions::isargref(lc.tokens.at(i));
                        if(p.first)
                        {
                            lc.AltStackArg = true;
                            lc.altInt = p.second;
                            return lc;
                        }
                        auto lambda = newasm::header::functions::is_lambda(lineCompiled.tokens.at(i));
                        if(lambda.first) if(lambda.second == newasm::core::lang_inf::instruction_set.at(newasm::core::lang_inf::proc))
                        {
                            lineCompiled.AltArgLambda = true;
                            return lineCompiled;
                        }
                        lineCompiled.altEvalMode.type = newasm::compiler::utils::getEvalMode(lineCompiled.tokens.at(i), lineCompiled.altEvalMode);
                        if(lineCompiled.tokens.at(i).size() >= 3)
                        {
                            auto regName = newasm::header::functions::trim(lineCompiled.tokens.at(i).substr(1));
                            if(lineCompiled.tokens.at(i).front() == '*' and newasm::header::functions::isalphanum(regName))
                            {
                                auto it__ = newasm::mem::regs::identifiers.find(regName);
                                if(it__ == newasm::mem::regs::identifiers.end() and regName != THIS_STR)
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
                        else if(newasm::header::functions::isfloat(lineCompiled.tokens.at(i)))
                        {
                            lineCompiled.altArgType = newasm::datatypes::decimal;
                            lineCompiled.altFloat = std::stof(lineCompiled.tokens.at(i));
                        }
                        else if(newasm::header::functions::istext(lineCompiled.tokens.at(i)))
                        {
                            lineCompiled.altArgType = newasm::datatypes::text;
                            lineCompiled.altString = newasm::header::functions::remq(lineCompiled.tokens.at(i));
                        }
                        else if(newasm::header::functions::ischar(lineCompiled.tokens.at(i)))
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

                if(
                    lc.whatAmIDoing == NewASM::core::lang_inf::unlock__ or
                    lc.whatAmIDoing == NewASM::core::lang_inf::lock__
                )
                {
                    lc.Class = NewASM::Const::InstructionClass::DedicatedClass::Mutex;
                }

                return lineCompiled;
            }

            // failed to compile
            newasm::compiler::abort(newasm::compiler::fail::unmatched_syntax);
            return lineCompiled;
        }

        // ---------------------------------- OPTIMIZER ----------------------------------
        using DescFunc = std::function<void(const std::string&, newasm::compiler::lineData*, int)>;
        using DescFuncRange = std::function<void(const std::string&, int, int)>;
        using DescFuncLmao = std::function<void(const std::string&, const std::vector<int>&)>;

        inline void OptimizeCodeA(
            DescFunc _OptDescription,
            newasm::compiler::lineData& line, int idx
        )
        {
            auto& cc = newasm::compiler::compiledCode;
            if(cc.empty())
            {
                return;
            }

            unsigned int invalidIndex = cc.size() + 1;
            auto GetLastRelevantLine = <:&:>() noexcept -> unsigned int {
                for(
                    int i = cc.size();
                    i > 0; --i
                )
                {
                    int real_i = i - 1;
                    auto& z = cc.at(real_i);
                    if(
                        z.type != newasm::compiler::empty
                    )
                    {
                        return real_i;
                    }
                    continue;
                }
                return invalidIndex;
            };
            auto _idx = GetLastRelevantLine();
            if(_idx == invalidIndex)
            {
                return;
            }
            newasm::OptimizerData::LastLineIdx = _idx;
            auto& lastLine = newasm::compiler::compiledCode.at(newasm::OptimizerData::LastLineIdx);
            auto OptDescription = <:&:>(
                const std::string& text, newasm::compiler::lineData* lptr = nullptr, int lidx = -1
            ) -> void {
                //we use the generic function we got
                //and apply peephole optimizer's logic
                _OptDescription(text, (lptr == nullptr) ? &line : lptr, (lidx == -1) ? idx : lidx);
                return;
            };
            if(!newasm::OptimizerData::JmpUsed) [[unlikely]]
            {
                if(NewASM::compiler::utils::IsJumpIns(line))
                {
                    newasm::OptimizerData::JmpUsed = true;
                }
            }
            //actual optimizations
            //------------------------------------------- double instructions -------------------------------------------
            if(
                (line == lastLine) or
                (line.whatAmIDoing == newasm::core::lang_inf::jmp and lastLine.whatAmIDoing == newasm::core::lang_inf::jmp)
            )
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
                    line.whatAmIDoing == newasm::core::lang_inf::switch__ or
                    line.whatAmIDoing == newasm::core::lang_inf::fetch__
                );
                if(IsRedundant)
                {
                    OptDescription("peephole optimization, removed redundant double code");
                    line.type = newasm::compiler::empty;
                    return;
                }
            }
            //------------------------------------------- empty try-catch block -------------------------------------------
            if(
                line.whatAmIDoing == newasm::core::lang_inf::catch__ and
                lastLine.whatAmIDoing == newasm::core::lang_inf::try__
            )
            {
                OptDescription("removed redundant try-catch block", &lastLine, newasm::OptimizerData::LastLineIdx);
                OptDescription("^");

                lastLine.type = newasm::compiler::empty;
                line.type = newasm::compiler::empty;
                return;
            }
            //------------------------------------------- redundant operations -------------------------------------------
            if(line.whatAmIDoing == lastLine.whatAmIDoing)
            {
                auto ins = line.whatAmIDoing;
                if(ins == newasm::core::lang_inf::fetch__) // redundant refetch
                {
                    OptDescription(
                        "peephole optimization, removed redundant refetch",
                        &lastLine, newasm::OptimizerData::LastLineIdx
                    );
                    lastLine.type = newasm::compiler::empty;
                    return;
                }
                if(ins == newasm::core::lang_inf::resb__) // redundant stack memory dedication
                {
                    OptDescription(
                        "peephole optimization, removed redundant stack memory dedication",
                        &lastLine, newasm::OptimizerData::LastLineIdx
                    );
                    lastLine.type = newasm::compiler::empty;
                    return;
                }
                if(ins == newasm::core::lang_inf::sysenter)
                {
                    OptDescription(
                        "peephole optimization, removed redundant kernel module entrance",
                        &lastLine, newasm::OptimizerData::LastLineIdx
                    );
                    lastLine.type = newasm::compiler::empty;
                    return;
                }
                if(ins == newasm::core::lang_inf::align)
                {
                    OptDescription(
                        "peephole optimization, removed redundant memory alignment",
                        &lastLine, newasm::OptimizerData::LastLineIdx
                    );
                    lastLine.type = newasm::compiler::empty;
                    return;
                }
            }
            //------------------------------------------- assigning register value to itself -------------------------------------------
            if(line.whatAmIDoing == newasm::core::lang_inf::mov)
            {
                if(
                    ( // mov rax, *rax
                        line.whatAreRegistersLol == line.altEvalMode.argInt and
                        line.altEvalMode.type == newasm::runtime::evalModes::regDeref and
                        line.altEvalMode.argType == newasm::datatypes::number
                    ) or ( // mov this, *this
                        line.altEvalMode.type == newasm::runtime::evalModes::thisDeref and
                        line.priArgType == newasm::datatypes::ThisPtr
                    )
                )
                {
                    //logging the optimization
                    OptDescription("peephole optimization, value of a register changed to itself");
                    //actual optimization xd
                    line.type = newasm::compiler::empty;
                    return;
                }
            }
            //------------------------------------------- useless ins -------------------------------------------
            if(line.whatAmIDoing == newasm::core::lang_inf::rem)
            {
                //logging the optimization
                OptDescription("peephole optimization, removed dead code");
                //actual optimization xd
                line.type = newasm::compiler::empty;
                return;
            }
            //------------------------------------------- remove redundant reassignments -------------------------------------------
            if(
                (
                    line.whatAmIDoing == newasm::core::lang_inf::mov and
                    lastLine.whatAmIDoing == newasm::core::lang_inf::mov
                ) or (
                    line.whatAmIDoing == newasm::core::lang_inf::mov and
                    lastLine.whatAmIDoing == newasm::core::lang_inf::zero
                )
            )
            {
                bool SameRegisters = line.whatAreRegistersLol == lastLine.whatAreRegistersLol;
                bool ValidRegisters = line.whatAreRegistersLol != INVALID_INS;

                if(SameRegisters and ValidRegisters)
                {
                    OptDescription("peephole optimization, removed redundant assignment before reassignment", &lastLine, newasm::OptimizerData::LastLineIdx);
                    lastLine.type = newasm::compiler::empty;
                    return;
                }
            }
            //------------------------------------------- code section reassignments -------------------------------------------
            if(line.type == newasm::compiler::sectionModifier and !newasm::OptimizerData::JmpUsed)
            {
                if(line.whatCodeSection == NewASM::OptimizerData::Section)
                {
                    OptDescription("removed redundant code section reset");
                    line.type = newasm::compiler::empty;
                    return;
                }
                NewASM::OptimizerData::Section = line.whatCodeSection;
                return;
            }
            if(
                line.whatCodeSection == lastLine.whatCodeSection and (
                    (line.whatCodeSection == newasm::code_stream::sections::data) or
                    (line.whatCodeSection == newasm::code_stream::sections::start)
                )
            )
            {
                OptDescription("removed redundant code section reset");
                line.type = newasm::compiler::empty;
                return;
            }
            return;
        }

        inline void OptimizeCodeB(DescFunc _OptDescription)
        {
            auto& k = newasm::compiler::compiledCode;
            if(k.empty()) [[unlikely]]
            {
                return;
            }
            static constexpr signed int INVALID_LINE = -1;
            int CodesecFound = INVALID_LINE;
            for(int i = 0; i < k.size(); ++i)
            {
                auto& line = k.at(i);
                //find section modifier and set the flag
                if(CodesecFound == INVALID_LINE)
                {
                    if(line.type == newasm::compiler::sectionModifier)
                    {
                        CodesecFound = i;
                        continue;
                    }
                }
                //reset the flag if it isn't linear code
                if(CodesecFound != INVALID_LINE)
                {
                    if(
                        newasm::compiler::utils::IsJumpIns(line) or
                        line.type == newasm::compiler::labelJumpPoint or
                        line.whatAmIDoing == newasm::core::lang_inf::loop
                    )
                    {
                        CodesecFound = INVALID_LINE;
                        continue;
                    }
                }
                //if we reach another section modifier
                //we check if it is the same one, and delete it
                if(CodesecFound != INVALID_LINE) if(line.type == newasm::compiler::sectionModifier)
                {
                    if(line.whatCodeSection != k.at(CodesecFound).whatCodeSection)
                    {
                        CodesecFound = i;
                        continue;
                    }
                    else if(line.whatCodeSection == k.at(CodesecFound).whatCodeSection)
                    {
                        _OptDescription("dead code elimination, removed redundant code section reset", &line, i);
                        line.type = newasm::compiler::empty;
                        continue;
                    }
                }
            }
            return;
        }

        inline void OptimizeCodeC(DescFuncRange OptDescription)
        {
            auto& k = newasm::compiler::compiledCode;
            if(k.empty()) [[unlikely]]
            {
                return;
            }
            static constexpr signed int INVALID_LINE = -1;
            int JumpFound = INVALID_LINE;
            std::string label_name;

            for(unsigned int i = 0; i < k.size(); ++i)
            {
                auto& lc = k.at(i);
                if(lc.whatAmIDoing == newasm::core::lang_inf::jmp)
                {
                    if(lc.tokens.size() != 2)
                    {
                        continue;
                    }
                    JumpFound = i;
                    label_name = newasm::header::functions::trim(lc.tokens[1]);
                    auto p = NewASM::header::functions::DetectNamespace(label_name);
                    if(p.first)
                    {
                        label_name = NewASM::compiler::utils::MangleName(p.second.first, p.second.second);
                    }
                    continue;
                }
                if(lc.type == newasm::compiler::labelJumpPoint)
                {
                    if(
                        JumpFound != INVALID_LINE and
                        lc.other == label_name
                    )
                    {
                        OptDescription("removed unreachable code block", JumpFound, i);
                        for(unsigned int j = JumpFound; j < i; ++j)
                        {
                            k.at(j).type = newasm::compiler::empty;
                        }
                        continue;
                    }
                    JumpFound = INVALID_LINE;
                    continue;
                }
            }

            return;
        }

        inline void OptimizeCodeD(DescFuncLmao OptDescription)
        {
            auto& k = newasm::compiler::compiledCode;
            if(k.empty()) [[unlikely]]
            {
                return;
            }
            static constexpr signed int INVALID_LINE = -1;
            bool NamespaceEmpty = true;
            std::vector<std::pair<int, int>> NamespaceTree;
            auto GetNamespaceLine = <::>(const decltype(NamespaceTree)& v) -> int {
                if(v.empty())
                {
                    return INVALID_LINE;
                }
                for(unsigned int i = v.size(); i > 0; true)
                {
                    --i;
                    if(v.at(i).second == INVALID_LINE)
                    {
                        //std::cout << "found idx " << i << std::endl;
                        return i;
                    }
                }
                return INVALID_LINE;
            };
            //we make a namespace tree and 
            //check if the namespaces are empty from top to bottom
            for(unsigned int i = 0; i < k.size(); ++i)
            {
                newasm::compiler::data::lnidx = i;
                auto& lc = k.at(i);
                newasm::compiler::data::line = lc.raw;
                //check if it is a namespace
                if(lc.type == newasm::compiler::namespace__)
                {
                    if(lc.priInt == NewASM::Namespaces::Construction)
                    {
                        NamespaceTree.push_back({i, INVALID_LINE});
                        continue;
                    }
                    else
                    {
                        auto IDX = GetNamespaceLine(NamespaceTree);
                        if(
                            IDX == INVALID_LINE or
                            (
                                IDX != INVALID_LINE and
                                k.at(i).priString != k.at(NamespaceTree[IDX].first).priString
                            )
                        )
                        {
                            #if 0
                            try
                            {
                                std::cout << "Lines: " << k.at(i).raw << " | " << k.at(NamespaceTree[IDX].first).raw << std::endl;
                            }
                            catch(const std::exception& e)
                            {
                                std::cerr << e.what() << '\n';
                            }
                            #endif
                            if constexpr(false) for(int s = 0; s < NamespaceTree.size(); ++s)
                            {
                                std::cout << "NamespaceTree[" << s << "]: ";
                                if(NamespaceTree[s].first != INVALID_LINE) std::cout << newasm::forLinker::getFile(NamespaceTree[s].first) << ":" << newasm::forLinker::getLine(NamespaceTree[s].first) << " -> ";
                                if(NamespaceTree[s].first == INVALID_LINE) std::cout << "-1" << " -> ";
                                if(NamespaceTree[s].second == INVALID_LINE) std::cout << "-1" << std::endl;
                                if(NamespaceTree[s].second != INVALID_LINE) std::cout << newasm::forLinker::getFile(NamespaceTree[s].second) << ":" << newasm::forLinker::getLine(NamespaceTree[s].second) << std::endl;
                            }
                            newasm::compiler::abort(newasm::compiler::fail::unexpected_nmst);
                            break;
                        }
                        NamespaceTree[IDX].second = i;
                        continue;
                    }
                }
            }
            //now after we have a good namespace tree
            //we check if any of these are empty
            auto& g = NamespaceTree;
            if(g.empty())
            {
                return;
            }
            for(unsigned int j = g.size() - 1; j != 0; --j)
            {
                for(unsigned int i = g.at(j).first; i < g.at(j).second; ++i)
                {
                    auto& lc = k.at(i);
                    if(
                        lc.whatAmIDoing == newasm::core::lang_inf::proc or
                        lc.whatAmIDoing == newasm::core::lang_inf::thread__
                    )
                    {
                        NamespaceEmpty = false;
                    }
                    else if(lc.CompileTime.dataDecl)
                    {
                        NamespaceEmpty = false;
                    }
                }
                if(NamespaceEmpty)
                {
                    OptDescription("removed useless namespace labels", {g.at(j).first, g.at(j).second});
                    k.at(g.at(j).first).type = newasm::compiler::empty;
                    k.at(g.at(j).second).type = newasm::compiler::empty;
                }
                NamespaceEmpty = true;
            }
            return;
        }

        inline namespace OPTIMIZATION_LEVELS
        {
            constinit const signed int OPT_PEEPHOLE = 0;
            constinit const signed int OPT_CODESEC = 1;
            constinit const signed int OPT_UNREACHABLE = 2;
            constinit const signed int OPT_EMPTY_NAMESPACES = 3;
        }

        template<int _What, typename... Args>
        inline void Optimize(Args&&... a)
        {
            if(newasm::compiler::data::aborted)
            {
                return;
            }
            //helper func
            auto __O_DESC_GENERIC__ = <::>(const std::string& text, newasm::compiler::lineData* L, int idx) -> void {
                if(NewASM::header::data::LogCompilerOptimizations)
                {
                    static const std::string Insomnia = "\t  "_str;
                    std::cout << Insomnia << newasm::header::col::magenta;
                    try
                    {
                        std::cout << newasm::forLinker::getFile(idx) << ":";
                        std::cout << newasm::forLinker::getLine(idx);
                    }
                    catch(const std::exception& e)
                    {
                        std::cout << "cached code";
                    }
                    std::cout << newasm::header::col::gray;
                    auto LINE_CODE__ = newasm::header::functions::trim(L->raw);
                    std::cout << ": " << text << ": " << newasm::header::col::magenta;
                    if(LINE_CODE__.size() > 15)
                    {
                        std::cout << '\n' << Insomnia << Insomnia;
                    }
                    std::cout << LINE_CODE__;
                    std::cout << '\n' << newasm::header::col::reset;
                    ++NewASM::compiler::data::OptimizationCount;
                }
                return;
            };
            auto __O_DESC_GENERIC2__ = <::>(const std::string& text, int range1, int range2) -> void {
                if(NewASM::header::data::LogCompilerOptimizations)
                {
                    std::cout << "\t  " << newasm::header::col::magenta;
                    try
                    {
                        std::cout << newasm::forLinker::getFile(range1) << ":";
                        std::cout << newasm::forLinker::getLine(range1) << " -> ";
                        std::cout << newasm::forLinker::getFile(range2) << ":";
                        std::cout << newasm::forLinker::getLine(range2);
                    }
                    catch(const std::exception& e)
                    {
                        std::cout << "cached code block";
                    }
                    std::cout << newasm::header::col::gray;
                    std::cout << ": " << text << ": " << newasm::header::col::magenta << '\n';
                    std::cout << newasm::header::style::dim;
                    for(unsigned int i = range1; i <= range2; ++i)
                    {
                        std::cout << "\t\t\t" << newasm::compiler::compiledCode.at(i).raw << '\n';
                    }
                    std::cout << '\n' << newasm::header::col::reset;
                    ++NewASM::compiler::data::OptimizationCount;
                }
                return;
            };
            auto __O_DESC_GENERIC3__ = <::>(const std::string& text, const std::vector<int>& v) -> void {
                if(NewASM::header::data::LogCompilerOptimizations)
                {
                    std::cout << "\t  " << newasm::header::col::magenta;
                    std::cout << NewASM::project_data::name;
                    if(!NewASM::project_data::version.empty()) std::cout << ":" << NewASM::project_data::version;
                    std::cout << newasm::header::col::gray;
                    std::cout << ": " << text << ": " << newasm::header::col::magenta << '\n';
                    std::cout << newasm::header::style::dim;
                    for(unsigned int i = 0; i < v.size(); ++i)
                    {
                        auto IDX = v.at(i);
                        std::cout << "\t\t\t";
                        try
                        {
                            std::cout << newasm::forLinker::getFile(IDX) << ":";
                            std::cout << newasm::forLinker::getLine(IDX);
                        }
                        catch(const std::exception& e)
                        {
                            std::cout << "cached code";
                        }
                        std::cout << "  | ";
                        std::cout << newasm::compiler::compiledCode.at(IDX).raw << '\n';
                    }
                    std::cout << '\n' << newasm::header::col::reset;
                    ++NewASM::compiler::data::OptimizationCount;
                }
                return;
            };
            if constexpr(_What == OPT_PEEPHOLE)
            {
                OptimizeCodeA(__O_DESC_GENERIC__, std::forward<Args>(a) ...);
                return;
            }
            if constexpr(_What == OPT_CODESEC)
            {
                OptimizeCodeB(__O_DESC_GENERIC__, std::forward<Args>(a) ...);
                return;
            }
            if constexpr(_What == OPT_UNREACHABLE)
            {
                OptimizeCodeC(__O_DESC_GENERIC2__);
                return;
            }
            if constexpr(_What == OPT_EMPTY_NAMESPACES)
            {
                OptimizeCodeD(__O_DESC_GENERIC3__);
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
