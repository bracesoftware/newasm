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

namespace newasm
{
    namespace shell
    {
        namespace data
        {
            bool terminated = false;
        }
        int main(std::string cmd)
        {
            auto tokenize = [](std::string str) -> std::vector<std::string> {
                std::vector<std::string> tokens;
                std::string token;
                std::istringstream stream(str);

                while(stream >> token)
                {
                    tokens.push_back(token);
                }

                return tokens;
            };

            auto exec = []() -> void {
                newasm::header::functions::trim(newasm::header::settings::script_file);
                newasm::execute(newasm::header::settings::script_file);

                if(newasm::mem::functions::datavalid(newasm::handlers::exit_handler, newasm::mem::funcs))
                {
                    newasm::global::event_now = true;
                    newasm::copyproc(newasm::handlers::exit_handler);
                    for(auto i = newasm::global::event_codeblock.begin(); i != newasm::global::event_codeblock.end(); ++i)
                    {
                        newasm::procline(*i);
                    }
                    newasm::global::event_now = false;
                }
            };

            auto repl = []() -> void {
                newasm::header::data::repl_end = false;
                std::cout << std::endl;
                newasm::header::functions::nullprint_wnm(
                    static_cast<std::string>("Read-Eval-Print Loop mode loaded; to leave, use the `")+
                    newasm::header::style::underline+static_cast<std::string>("exit")+newasm::header::col::reset+
                    static_cast<std::string>("` instruction."));
                newasm::header::data::lastlndx = 0;
                newasm::header::data::repl = true;
                
                newasm::repl();
            };
            std::vector<std::string> command = tokenize(cmd);
            if(command.size() >= 1)
            {
                if(command[0] == "exit")
                {
                    newasm::shell::data::terminated = true;
                    return 1;
                }
                if(command[0] == "exec")
                {
                    exec();
                    return 1;
                }
                if(command[0] == "repl")
                {
                    repl();
                    return 1;
                }
            }
            newasm::header::functions::err("Invalid command.");
            return 1;
        }
    }
}