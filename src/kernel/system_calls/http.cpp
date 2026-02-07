// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain


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