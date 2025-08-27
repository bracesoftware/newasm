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


#define __newasm_MEMORY_SIZE 10 // mb
#define __newasm_DISK_SIZE 20 // mb

// funcs
#define __newasm_B_TO_KB(val)   ((val) / 1024.0)
#define __newasm_B_TO_MB(val)   ((val) / (1024.0 * 1024.0))
#define __newasm_B_TO_GB(val)   ((val) / (1024.0 * 1024.0 * 1024.0))

#define __newasm_KB_TO_B(val)   ((val) * 1024)
#define __newasm_MB_TO_B(val)   ((val) * 1024 * 1024)
#define __newasm_GB_TO_B(val)   ((val) * 1024 * 1024 * 1024)

#define __newasm_KB_TO_MB(val)  ((val) / 1024.0)
#define __newasm_MB_TO_KB(val)  ((val) * 1024)

#define __newasm_MB_TO_GB(val)  ((val) / 1024.0)
#define __newasm_GB_TO_MB(val)  ((val) * 1024)

// other bs
#define STR(x) #x
#define CONCAT(a, b) a##b

static_assert(sizeof(int) == 4);
static_assert(sizeof(float) == 4);
static_assert(sizeof(char) == 1);