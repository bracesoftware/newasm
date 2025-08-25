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

// little preprocessor

namespace newasm
{
    namespace compiler
    {
        static const int def = 1;
        static const int using__ = 2;

        static const std::unordered_map<std::string, int> instructions = {
            {"def", def},
            {"using", using__}
        };

        namespace meta
        {
            std::unordered_map<std::string, std::string> defines;
        }

        std::string parse_def(std::string suf)
        {
            std::string tobeparsed = newasm::header::functions::trim(suf);
            auto it = newasm::compiler::meta::defines.find(tobeparsed);
            if(it != newasm::compiler::meta::defines.end())
            {
                tobeparsed = newasm::compiler::meta::defines.at(tobeparsed);
            }
            return tobeparsed;
        }
        void process_comptis(std::string ins, std::string arg1)
        {
            ins = newasm::header::functions::trim(ins);
            arg1 = newasm::header::functions::trim(arg1);

            auto it = newasm::compiler::instructions.find(ins);
            if(it == newasm::compiler::instructions.end())
            {
                return;
            }
            switch(it->second)
            {
                case newasm::compiler::using__:
                {
                    auto arg = arg1;
                    if(!newasm::header::functions::istext(arg))
                    {
                        newasm::terminate(newasm::exit_codes::dtyp_mismatch);
                        return;
                    }
                    arg = newasm::header::functions::remq(arg);
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::ios))
                    {
                        newasm::kernel::cfg::IOStream = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::fs))
                    {
                        newasm::kernel::cfg::FileStream = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::ext))
                    {
                        newasm::kernel::cfg::Extensions = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::cmanip))
                    {
                        newasm::kernel::cfg::ContainerManipulation = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::txtop))
                    {
                        newasm::kernel::cfg::TextOperations = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::net))
                    {
                        newasm::kernel::cfg::Network = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::mem))
                    {
                        newasm::kernel::cfg::Memory = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::chrono))
                    {
                        newasm::kernel::cfg::Chrono = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::thread))
                    {
                        newasm::kernel::cfg::Thread = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::tuple))
                    {
                        newasm::kernel::cfg::Tuple = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::tcp))
                    {
                        newasm::kernel::cfg::TCProtocol = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::http))
                    {
                        newasm::kernel::cfg::HTTP = true;
                        return;
                    }
                    if(arg == newasm::core::lang_inf::refs::identifiers__.at(newasm::core::lang_inf::refs::math))
                    {
                        newasm::kernel::cfg::Math = true;
                        return;
                    }
                    //newasm::terminate(newasm::exit_codes::invalid_config);
                    return;
                }
            }
        }
        void process_comptiso(std::string ins, std::string arg1, std::string arg2)
        {
            ins = newasm::header::functions::trim(ins);
            arg1 = newasm::header::functions::trim(arg1);
            arg2 = newasm::header::functions::trim(arg2);

            auto it = newasm::compiler::instructions.find(ins);
            if(it == newasm::compiler::instructions.end())
            {
                return;
            }
            switch(it->second)
            {
                case newasm::compiler::def:
                {
                    newasm::compiler::meta::defines[arg1] = arg2;
                    return;
                }
            }
            return;
        }
    }
}