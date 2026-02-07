// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain


#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

// get shi from go
extern "C"
{
    int send_tcp(const char* addr, const char* msg, const char* err_buf, int size);
    char* recv_tcp(const char* addr);
    void tcp_init();
}


namespace newasm
{
    namespace syscalls
    {
        namespace tcp
        {
            inline std::pair<int, std::string> send(std::string addr, std::string content)
            {
                char err_buf[128] = {0};
                auto res = send_tcp(addr.c_str(), content.c_str(), err_buf, sizeof(err_buf));
                std::string err(err_buf);
                return {res, err};
            }
            std::string recv(std::string addr)
            {
                char* res = recv_tcp(addr.c_str());
                std::string result(res);
                free_string(res);
                return result;
            }
            void init()
            {
                tcp_init();
                return;
            }
        }
    }
}