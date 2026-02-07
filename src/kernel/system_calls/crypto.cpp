// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain


#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

// get shi from go
extern "C"
{
    char* crypto_sha256_hex(const char* text);
    //char* crypto_xor_encrypt(const char* text, const char* key);
    //char* crypto_xor_decrypt(const char* text, const char* key);
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
                std::string result = text;
                for(size_t i = 0; i < result.size(); ++i)
                {
                    result[i] ^= key[i % key.size()];
                }
                return result;
            }

            inline std::string xor_decrypt(const std::string& text, const std::string& key)
            {
                return xor_encrypt(text, key);
            }
        }
    }
}