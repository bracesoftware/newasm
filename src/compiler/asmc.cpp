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

            const std::unordered_map<int, std::string> id = {
                {unmatched_syntax, "UnmatchedSyntax"}
            };
        }

        void abort_(int exc)
        {
            std::cout << newasm::header::col::red << "\t";
            std::cout << "Compilation aborted.\n";
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

        const int empty = 0;
        const int directive = 1;
        const int decorator = 2;
        const int namespace__ = 3;
        const int closingBrace = 4;
        const int macroTerminator = 5;
        const int lambdaTerminator = 6;
        const int sectionModifier = 7;
        const int handleModifier = 8;
        const int macroDecl = 9;
        const int dataDecl = 10;
        const int macroCall = 11;
        const int conditional = 12;
        const int instruction = 13;

        std::vector<newasm::compiler::lineData> compiledCode;

        newasm::compiler::lineData DO(std::string& line)
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
            //decorator
            if(newasm::header::functions::isdeco(line).first)
            {
                lineCompiled.type = newasm::compiler::decorator;
                lineCompiled.tokens.push_back(newasm::header::functions::isdeco(line).second);
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
                return lineCompiled;
            }
            // HANDLE MODIFIER
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
                lineCompiled.tokens.push_back(value);
                
                return lineCompiled; 
            }
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
                    if(line.size() != idx__+2)
                    {
                        std::vector<std::string> linetokens_inline = newasm::common::tokenize(line.substr(0, idx__));
                        if(linetokens_inline.size() == 2)
                        {
                            lineCompiled.type = newasm::compiler::conditional;
                            lineCompiled.other = newasm::header::functions::trim(line.substr(idx__ + 2));
                            lineCompiled.tokens.push_back(linetokens_inline.at(0));
                            lineCompiled.tokens.push_back(linetokens_inline.at(1));
                            return lineCompiled;
                        }
                        if(linetokens_inline.size() == 1) // process_is(line, linetokens_inline.at(0), linetokens_inline.at(1))
                        {
                            lineCompiled.type = newasm::compiler::conditional;
                            lineCompiled.other = newasm::header::functions::trim(line.substr(idx__ + 2));
                            lineCompiled.tokens.push_back(linetokens_inline.at(0));
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
            if(newasm::compiler::instructions.find(instruction) != newasm::compiler::instructions.end())
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
                for(int i = 0; i < lineCompiled.tokens.size(); i++)
                {
                    lineCompiled.tokens.at(i) = newasm::header::functions::trim(lineCompiled.tokens.at(i));
                    lineCompiled.tokens.at(i) = newasm::compiler::parse_def(lineCompiled.tokens.at(i));
                }
                return lineCompiled;
            }

            // failed to compile
            newasm::compiler::abort(newasm::compiler::fail::unmatched_syntax);
            return lineCompiled;
        }
    }
}
