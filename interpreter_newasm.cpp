/*

The ORIGINAL CODE is the `New-Assembly eXecutor` Source Code.
The INITIAL DEVELOPER is Brace Software Co., DEntisT.

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

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdio>
#include <sstream>
#include <filesystem>
#include <unordered_map>

#include <algorithm>
#include <cctype>

#define __newasm_included
#include "newasm_header.h"
#include "newasm_mem.h"
#include "newasm_parser.h"

int main(int argc, char *argv[])
{
    if(newasm::header::functions::check_args("-ver",argc,argv))
    {
        newasm::header::functions::vers_info();
        return 0;
    }
    newasm::header::functions::vers_info();
    newasm::execute("index.nax");
    return 0;
}
