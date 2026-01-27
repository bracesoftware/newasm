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


#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

// get shi from go
extern "C"
{
    char* crypto_sha256_hex(const char* text);
    char* crypto_xor_encrypt(const char* text, const char* key);
    char* crypto_xor_decrypt(const char* text, const char* key);
}


namespace newasm
{
    namespace syscalls
    {
        namespace crypto
        {
            inline std::string sha256(const std::string& text)
            {
                char* result = crypto_sha256_hex(text.c_str());
                std::string res(result);
                free_string(result);
                return res;
            }

            inline std::string xor_encrypt(const std::string& text, const std::string& key)
            {
                char* result = crypto_xor_encrypt(text.data(), key.data());
                std::string res(result);
                free_string(result);
                return res;
            }

            inline std::string xor_decrypt(const std::string& text, const std::string& key)
            {
                char* result = crypto_xor_decrypt(text.data(), key.data());
                std::string res(result);
                free_string(result);
                return res;
            }
        }
    }
}