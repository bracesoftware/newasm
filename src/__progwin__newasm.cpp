/*

The ORIGINAL CODE is the `NewASM Program Window` Source Code.
The INITIAL DEVELOPER is Brace Software Co., DEntisT.
The COPYRIGHT YEAR is 2024.

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
#include <netinet/tcp.h> 
#include <netinet/in.h>
#include <cstring>
#endif

#include <iostream>
#include <string>

#ifdef _WIN32
void init_winsock() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
}
void cleanup_winsock() {
    WSACleanup();
}
#else
void init_winsock() {}
void cleanup_winsock() {}
#endif

int main() {
    init_winsock();

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { std::cerr << "Socket failed\n"; return 1; }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(12345);

    int opt = 1;
#ifdef _WIN32
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
#else
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed\n";
        return 1;
    }

    if (listen(server_fd, 1) < 0) {
        std::cerr << "Listen failed\n";
        return 1;
    }

    std::cout << "Console waiting for connection...\n";

    int addrlen = sizeof(address);
    int new_socket = accept(server_fd, (sockaddr*)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        std::cerr << "Accept failed\n";
        return 1;
    }

    std::cout << "Connected to runtime.\n";

    char buffer[1024];
    bool running = true;
    while (running) {
        memset(buffer, 0, sizeof(buffer));
#ifdef _WIN32
        int valread = recv(new_socket, buffer, sizeof(buffer)-1, 0);
#else
        int valread = read(new_socket, buffer, sizeof(buffer)-1);
#endif
        if (valread <= 0) {
            std::cout << "Connection closed.\n";
            break;
        }

        std::string cmd(buffer);
        if (cmd.find("PrintText:") == 0) {
            std::string text = cmd.substr(10);
            std::cout << text << std::endl;

            // Ack back
            std::string ack = "Printed\n";
#ifdef _WIN32
            send(new_socket, ack.c_str(), (int)ack.size(), 0);
#else
            write(new_socket, ack.c_str(), ack.size());
#endif
        }
        else if (cmd.find("RequestInput") == 0) {
            std::cout << "> ";
            std::string input;
            std::getline(std::cin, input);
            input += "\n";

#ifdef _WIN32
            send(new_socket, input.c_str(), (int)input.size(), 0);
#else
            write(new_socket, input.c_str(), input.size());
#endif
        }
        else if (cmd.find("CloseProgram") == 0) {
            std::cout << "Closing console program.\n";
            running = false;
        }
        else if (cmd.find("OpenProgramConsole") == 0) {
            std::cout << "Console ready.\n";
#ifdef _WIN32
            std::string ack = "ConsoleOpened\n";
            send(new_socket, ack.c_str(), (int)ack.size(), 0);
#else
            std::string ack = "ConsoleOpened\n";
            write(new_socket, ack.c_str(), ack.size());
#endif
        }
    }

#ifdef _WIN32
    closesocket(new_socket);
    closesocket(server_fd);
#else
    close(new_socket);
    close(server_fd);
#endif

    cleanup_winsock();
    return 0;
}

