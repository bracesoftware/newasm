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
    namespace handles
    {
        newasm::syscalls::filestream::dir *dir_handle = new newasm::syscalls::filestream::dir;
        newasm::syscalls::filestream::file<
            newasm::syscalls::filestream::filetype::txt
        > *file_handle = new newasm::syscalls::filestream::file<newasm::syscalls::filestream::filetype::txt>();
        void delete_handles()
        {
            delete dir_handle;
            delete file_handle;
        }
    }
}