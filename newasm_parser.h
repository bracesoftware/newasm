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
    int process_l(std::string stat, std::string arg)
    {
        if(stat == static_cast<std::string>("_"))
        {
            if(arg == static_cast<std::string>("data"))
            {
                try
                {
                    newasm::system::labels |= newasm::constv::__data;
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n' << "RANDOM ERROR 1" << std::endl;
                }
                
                return 1;
            }
            if(arg == static_cast<std::string>("start"))
            {
                try
                {
                    newasm::system::labels |= newasm::constv::__start;
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n' << "RANDOM ERROR 2" << std::endl;
                }
                return 1;
            }
        }
        return 1;
    }
    int process_d(std::string name, std::string value)
    {
        if(!(newasm::system::labels & newasm::constv::__data) && (newasm::system::labels & newasm::constv::__start))
        {
            newasm::header::functions::err("Program cannot start.");
            newasm::mem::regs::pri = 1;
            newasm::header::functions::info("Program finished with exit code : " + std::to_string(newasm::mem::regs::pri));
            return 1;
        }
        if(newasm::header::functions::isalphanum(name))
        {
            newasm::mem::data[name] = value;
            return 1;
        }
        return 1;
    }
    int process_iso(std::string ins, std::string suf, std::string opr)
    {
        newasm::header::functions::parseopr(opr, newasm::mem::data);
        if(!(newasm::system::labels & newasm::constv::__data))
        {
            newasm::header::functions::err("Program cannot start.");
            newasm::mem::regs::pri = 1;
            newasm::header::functions::info("Program finished with exit code : " + std::to_string(newasm::mem::regs::pri));
            return 1;
        }
        if(!(newasm::system::labels & newasm::constv::__start))
        {
            newasm::header::functions::err("Program cannot start.");
            newasm::mem::regs::pri = 1;
            newasm::header::functions::info("Program finished with exit code : " + std::to_string(newasm::mem::regs::pri));
            return 1;
        }
        // RETURN
        if(ins == static_cast<std::string>("retn"))
        {
            if(suf == static_cast<std::string>("0"))
            {
                if(newasm::header::functions::isnumeric(opr))
                {
                    newasm::mem::regs::pri = std::stoi(opr);
                }
                else
                {
                    newasm::mem::regs::pri = 0;
                }
                newasm::header::functions::info("Program finished with exit code : " + std::to_string(newasm::mem::regs::pri));
                newasm::system::terminated = true;
                return 1;
            }
        }
        // ZERO
        if(ins == static_cast<std::string>("zero"))
        {
            if(suf == static_cast<std::string>("alt"))
            {
                if(opr == static_cast<std::string>("0"))
                {
                    newasm::mem::regs::alt = 0;
                    return 1;
                }
            }
            if(suf == static_cast<std::string>("pri"))
            {
                if(opr == static_cast<std::string>("0"))
                {
                    newasm::mem::regs::pri = 0;
                    return 1;
                }
            }
        }
        return 1;
    }
    void execute(std::string file)
    {
        std::ifstream internal_fileobject(newasm::header::constants::scripts_folder + file);
        if(internal_fileobject.is_open())
        {
            std::string line, ins, suf, opr, stat, arg;
            std::vector<std::string> tmp;
            std::vector<std::string> tmp1;
            std::vector<std::string> tmp2;

            while(std::getline(internal_fileobject, line))
            {
                tmp.clear();
                tmp1.clear();
                tmp2.clear();

                ins.clear();
                suf.clear();
                opr.clear();

                stat.clear();
                arg.clear();

                if(newasm::header::functions::strfind(line,':'))
                {
                    tmp2 = newasm::header::functions::split(line, ':');
                    stat = tmp2[0];
                    arg = tmp2[1];
                    stat = newasm::header::functions::trim(stat);
                    arg = newasm::header::functions::trim(arg);
                }
                if(newasm::header::functions::strfind(line,';'))
                {
                    tmp2 = newasm::header::functions::split(line, ';');
                    stat = tmp2[0];
                    arg = tmp2[1];
                    stat = newasm::header::functions::trim(stat);
                    arg = newasm::header::functions::trim(arg);
                }
                if(newasm::header::functions::strfind(line,'.')) if(newasm::header::functions::strfind(line,','))
                {
                    tmp = newasm::header::functions::split(line, '.');
                    tmp1 = newasm::header::functions::split(tmp[1], ',');
                    ins = tmp[0];
                    suf = tmp1[0];
                    opr = tmp1[1];
                    ins = newasm::header::functions::trim(ins);
                    suf = newasm::header::functions::trim(suf);
                    opr = newasm::header::functions::trim(opr);
                }

                if(newasm::system::terminated)
                {
                    break;
                }
                if(newasm::header::functions::strfind(line,':'))
                    newasm::process_l(stat,arg);
                if(newasm::header::functions::strfind(line,';'))
                    newasm::process_d(stat,arg);
                if(newasm::header::functions::strfind(line,'.')) if(newasm::header::functions::strfind(line,','))
                    newasm::process_iso(ins,suf,opr);
            }
            internal_fileobject.close();
        }
        else
        {
            newasm::header::functions::err(
                static_cast<std::string>("Unable to open the file : ") + static_cast<std::string>("'") + 
                newasm::header::constants::scripts_folder + file + static_cast<std::string>("'"));
        }
    }
}