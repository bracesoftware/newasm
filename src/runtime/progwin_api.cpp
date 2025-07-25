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


#ifdef _WIN32
#define _WIN32_WINNT 0x0601
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#endif

#include <iostream>
#include <string>

#ifdef _WIN32
void init_winsock()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
}
void cleanup_winsock() 
{
    WSACleanup();
}
#else
void init_winsock() {}
void cleanup_winsock() {}
#endif

int sock = -1;

bool ConnectConsole()
{
    init_winsock();
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return false;
    int flag = 1;
setsockopt(sock, IPPROTO_TCP, TCP_NODELAY,
#ifdef _WIN32
    (const char*)&flag,
#else
    &flag,
#endif
    sizeof(int));

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(12345);
#ifdef _WIN32
    serv_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
#else
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
#endif

    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        return false;
    }
    return true;
}

void DisconnectConsole()
{
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    cleanup_winsock();
}

void SendCommand(const std::string& cmd)
{
#ifdef _WIN32
    send(sock, cmd.c_str(), (int)cmd.size(), 0);
#else
    write(sock, cmd.c_str(), cmd.size());
#endif
}

std::string ReceiveResponse()
{
    char buffer[1024];
#ifdef _WIN32
    int len = recv(sock, buffer, sizeof(buffer) - 1, 0);
#else
    int len = read(sock, buffer, sizeof(buffer) -1);
#endif
    if (len <= 0) return "";

    buffer[len] = '\0';
    return std::string(buffer);
}

static void OpenProgramConsole()
{
    SendCommand("OpenProgramConsole");
    std::string resp = ReceiveResponse();
    std::cout << resp;
}

static void PrintText(const std::string& text)
{
    SendCommand("PrintText:" + text);
    std::string resp = ReceiveResponse();
    std::cout << resp;
}

static std::string RequestInput()
{
    SendCommand("RequestInput");
    return ReceiveResponse();
}

static void CloseProgramConsole()
{
    SendCommand("CloseProgram");
    DisconnectConsole();
}

static bool IsConsoleClosed()
{
    int result = send(sock, "", 0,
#ifdef _WIN32
    0
#else
    MSG_NOSIGNAL
#endif
    );
    return result == -1;
}
/*
int main()
{
    if(!ConnectConsole())
    {
        std::cerr << "Cannot connect to console program\n";
        return 1;
    }

    OpenProgramConsole();
    PrintText("Pozdrav iz runtime!");
    PrintText("Unesi tekst:");

    std::string input = RequestInput();
    PrintText("Unio si: " + input);

    CloseProgramConsole();
    return 0;
}
*/
//usable api
namespace newasm
{
    namespace progwin
    {
        namespace api
        {
            void start
            {
                if(!ConnectConsole())
                {
                    return;
                }
                OpenProgramConsole();
            }
            void cout(std::string text)
            {
                PrintText(text);
            }
            std::string cin()
            {
                return RequestInput();
            }
            void exit
            {
                CloseProgramConsole();
            }
            bool isConsoleRunning()
            {
                return IsConsoleClosed() == false;
            }
        }
    }
}