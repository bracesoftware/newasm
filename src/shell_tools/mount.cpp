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

static const std::string __stdlib_code = R"(
;
; NewASM Standard Library
; 2025 (c) Brace Software Co.
;
.data
    ./std
        intg version: 13

        ./math
            float pi: 3.141
        ./!math
    ./!std
.start
.$__newasm_sys_interrupt __std_now__
;
; Start of the STD
;
;============================================================================================
.start
    ; using "ios" ; using iostream for printing
    ./std
        ./ios
            proc write
                .$__newasm_cfg_manip __autobos=__TRUE
                .$__newasm_krnl_config IOS/YES__
                mov stl, 0c0 ; null terminator
                
                mov fdx, 0

                switch *tlr
                case intg -> mov fdx, 2
                case float -> mov fdx, 2
                case string -> mov fdx, 1
                case char -> mov fdx, 7
                case ref -> mov fdx, 6
                default -> nop ;mov fdx, 1

                sysenter "ios"
                syscall
                .$__newasm_krnl_config IOS/NO__
                .$__newasm_cfg_manip __autobos=__FALSE
                halt 0
            end

            proc writeln
                .$__newasm_cfg_manip __autobos=__TRUE
                .$__newasm_krnl_config IOS/YES__
                mov stl, 0c1 ; newline character

                mov fdx, 0
                
                switch *tlr
                case 1 -> mov fdx, 2
                case intg -> mov fdx, 2
                case float -> mov fdx, 2
                case string -> mov fdx, 1
                case char -> mov fdx, 7
                case ref -> mov fdx, 6
                default -> nop ;mov fdx, 1
                
                sysenter "ios"
                syscall
                .$__newasm_cfg_manip __autobos=__FALSE
                .$__newasm_krnl_config IOS/NO__
                halt 0
            end
        ./!ios
    ./!std
;============================================================================================
;
; EOF
;
.$__newasm_sys_interrupt __std_now__
)";

namespace newasm
{
    namespace shell_tools
    {
        namespace mount
        {
            const std::string setup_help = "?"; 
            const std::string stdlib = "stdlib";

            const std::vector<std::pair<std::string, std::string>> help_table_data__ = {
                {"?",               "Displays the help panel."},
                {"stdlib",          "Installs the standard library."}
            };

            inline void mount_help()
            {
                std::cout << std::endl;
                std::cout << newasm::header::col::light_blue << newasm::header::style::underline;
                std::cout << "\tSetup" << std::string(15, 32) << "Description" << std::string(10, 32) << "\n";

                std::cout << newasm::header::col::reset;
                std::cout << newasm::header::col::gray;


                for(auto i = help_table_data__.begin(); i != help_table_data__.end(); ++i)
                {
                    std::cout << "\t" << i->first << "\t\t" << i->second << "\n";
                }

                std::cout << std::endl;
                std::cout << newasm::header::col::reset;
                return;
            }

            inline void mount_stdlib()
            {
                bool result = newasm::shell_tools::prompt("Proceed with the standard library installation?");
                if(result)
                {
                    std::ofstream file(newasm::header::constants::std_library);
                    newasm::utils::loading("Please wait...", newasm::utils::load_speed);
                    file << __stdlib_code;
                    file.close();
                    return;
                }
                return;
            }
        }
    }
}