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
    void callproc(std::string name);
    int process_l(std::string stat, std::string arg)
    {
        if(stat == static_cast<std::string>("_"))
        {
            if(arg == static_cast<std::string>("data"))
            {
                newasm::system::label = newasm::constv::__data;
                return 1;
            }
            if(arg == static_cast<std::string>("start"))
            {
                newasm::system::label = newasm::constv::__start;
                return 1;
            }
        }
        return 1;
    }
    int process_d(std::string name, std::string value)
    {
        if(newasm::system::label != newasm::constv::__data)
        {
            newasm::mem::regs::pri = 1;
            newasm::system::terminated = true;
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
        if(newasm::system::label != newasm::constv::__start)
        {
            newasm::mem::regs::pri = 1;
            newasm::system::terminated = true;
            newasm::header::functions::info("Program finished with exit code : " + std::to_string(newasm::mem::regs::pri));
            return 1;
        }

        //end
        if(ins == static_cast<std::string>("end"))
        {
            if(suf == static_cast<std::string>("0"))
            {
                if(opr == static_cast<std::string>("0"))
                {
                    newasm::system::stop = 0;
                    return 1;
                }
            }
        }
        if(newasm::system::stop == 1)
        {
            std::string newline = ins + static_cast<std::string>(".") + suf + static_cast<std::string>(",") + opr;
            newasm::mem::funcs[newasm::system::cproc].push_back(newline);
            return 1;
        }
        // STOR
        if(ins == static_cast<std::string>("stor"))
        {
            if(!newasm::header::functions::isalphanum(opr))
            {
                return 1;
            }
            if(!newasm::mem::functions::datavalid(opr,newasm::mem::data))
            {
                return 1;
            }

            if(suf == static_cast<std::string>("fdx"))
            {
                newasm::mem::data[opr] = std::to_string(newasm::mem::regs::fdx);
                return 1;
            }
            if(suf == static_cast<std::string>("onm"))
            {
                newasm::mem::data[opr] = std::to_string(newasm::mem::regs::onm);
                return 1;
            }
            if(suf == static_cast<std::string>("otx"))
            {
                newasm::mem::data[opr] = (newasm::mem::regs::otx);
                return 1;
            }
        }
        newasm::header::functions::parseopr(opr, newasm::mem::data);
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
        // MOV
        if(ins == static_cast<std::string>("mov"))
        {
            if(suf == static_cast<std::string>("fdx"))
            {
                if(!newasm::header::functions::isnumeric(opr))
                {
                    newasm::mem::regs::fdx = 0;
                    return 1;
                }
                newasm::mem::regs::fdx = std::stoi(opr);
                return 1;
            }
            if(suf == static_cast<std::string>("onm"))
            {
                if(!newasm::header::functions::isnumeric(opr))
                {
                    newasm::mem::regs::onm = 0;
                    return 1;
                }
                newasm::mem::regs::onm = std::stoi(opr);
                return 1;
            }
            if(suf == static_cast<std::string>("otx"))
            {
                newasm::mem::regs::otx = opr;
                return 1;
            }
        }
        // syscall
        if(ins == static_cast<std::string>("syscall"))
        {
            if(suf == static_cast<std::string>("0"))
            {
                if(opr == static_cast<std::string>("0"))
                {
                    if(newasm::mem::regs::fdx == 1)
                    {
                        std::cout << newasm::mem::regs::otx << std::endl;
                    }
                    if(newasm::mem::regs::fdx == 2)
                    {
                        std::cout << newasm::mem::regs::onm << std::endl;
                    }
                    return 1;
                }
            }
        }
        //proc
        if(ins == static_cast<std::string>("proc"))
        {
            if(suf == static_cast<std::string>("0"))
            {
                if(newasm::header::functions::isalphanum(opr))
                {
                    newasm::system::stop = 1;
                    newasm::system::cproc = opr;
                    return 1;
                }
            }
        }
        //call
        if(ins == static_cast<std::string>("call"))
        {
            if(suf == static_cast<std::string>("0"))
            {
                if(newasm::header::functions::isalphanum(opr))
                {
                    newasm::callproc(opr);
                    return 1;
                }
            }
        }
        //rem
        if(ins == static_cast<std::string>("rem"))
        {
            if(suf == static_cast<std::string>("0"))
            {
                /*if(opr.empty())
                {
                    newasm::header::functions::wrn("Encountered an empty comment.");
                }*/
                //do nothing,this is a comment
                return 1;
            }
        }
        //nop
        if(ins == static_cast<std::string>("nop"))
        {
            if(suf == static_cast<std::string>("0"))
            {
                if(opr == static_cast<std::string>("0"))
                {
                    return 1;//newasm::header::functions::wrn("Encountered an empty comment.");
                }
            }
        }
        return 1;
    }
    
    void procline(std::string &line)
    {
        std::string ins, suf, opr, stat, arg;
        std::vector<std::string> tmp, tmp1, tmp2;
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
        
        if(newasm::header::functions::strfind(line,':'))
            newasm::process_l(stat,arg);
        if(newasm::header::functions::strfind(line,';'))
            newasm::process_d(stat,arg);
        if(newasm::header::functions::strfind(line,'.'))
            if(newasm::header::functions::strfind(line,','))
                newasm::process_iso(ins,suf,opr);
    }
    void callproc(std::string name)
    {
        auto it = newasm::mem::funcs.find(name);
        if(it != newasm::mem::funcs.end())
        {
            for(std::string &line : it->second)
            {
                newasm::procline(line);
                //std::cout << "Executing : " << line << std::endl;
            }
        }
        else
        {
            newasm::mem::regs::pri = 2;
            newasm::system::terminated = true;
            newasm::header::functions::info("Program finished with exit code : " + std::to_string(newasm::mem::regs::pri));
        }
    }
    void execute(std::string file)
    {
        std::ifstream internal_fileobject(newasm::header::constants::scripts_folder + file);
        if(internal_fileobject.is_open())
        {
            std::string line;
            int lineidx = 1;

            while(std::getline(internal_fileobject, line))
            {
                if(newasm::system::terminated)
                {
                    break;
                }
                newasm::procline(line);
                lineidx++;
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