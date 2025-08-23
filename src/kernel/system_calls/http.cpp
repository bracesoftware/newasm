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


// get shi from go again
extern "C"
{
    char* http_get(char* url);
    char* http_post(char* url, char* data);
}


namespace newasm
{
    namespace syscalls
    {
        namespace http
        {
            std::string get(std::string url)
            {
                char* res = http_get(const_cast<char*>(url.c_str()));
                std::string result(res);
                free_string(res);
                //result = newasm::header::functions::remq(result);
                return result;
            }

            std::string post(std::string url, std::string data)
            {
                char* res = http_post(const_cast<char*>(url.c_str()), const_cast<char*>(data.c_str()));
                std::string result(res);
                free_string(res);
                //result = newasm::header::functions::remq(result);
                return result;
            }
        }
    }
}