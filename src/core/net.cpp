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


#ifdef _WIN32
    #include <windows.h>
    #include <urlmon.h>
    #pragma comment(lib, "urlmon.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <cstring>
#endif


//

namespace newasm
{
    namespace net
    {
        bool download(const std::string& url, const std::string& output_path) {
        #ifdef _WIN32
            std::wstring wurl(url.begin(), url.end());
            std::wstring woutput(output_path.begin(), output_path.end());
            HRESULT res = URLDownloadToFileW(NULL, wurl.c_str(), woutput.c_str(), 0, NULL);
            return res == S_OK;

        #else
            std::string host, path;
            if (url.find("http://") == 0)
                host = url.substr(7);
            else if (url.find("https://") == 0)
                host = url.substr(8);
            else
                return false;

            size_t slash_pos = host.find('/');
            if (slash_pos == std::string::npos)
                return false;

            path = host.substr(slash_pos);
            host = host.substr(0, slash_pos);

            struct hostent* server = gethostbyname(host.c_str());
            if (!server)
                return false;

            int sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0)
                return false;

            struct sockaddr_in serv_addr{};
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(80);
            std::memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

            if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
                return false;

            std::string request = "GET " + path + " HTTP/1.1\r\n"
                                "Host: " + host + "\r\n"
                                "Connection: close\r\n\r\n";
            send(sockfd, request.c_str(), request.length(), 0);

            std::ofstream file(output_path, std::ios::binary);
            if (!file)
                return false;

            char buffer[1024];
            bool header_ended = false;
            ssize_t bytes_received;
            while ((bytes_received = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
                if (!header_ended) {
                    std::string response(buffer, bytes_received);
                    size_t header_end = response.find("\r\n\r\n");
                    if (header_end != std::string::npos) {
                        file.write(response.c_str() + header_end + 4, bytes_received - header_end - 4);
                        header_ended = true;
                    }
                } else {
                    file.write(buffer, bytes_received);
                }
            }

            close(sockfd);
            file.close();
            return true;
        #endif
        }
    }
}
#ifdef CRAZY
int main() {
    std::string url = "http://example.com/file.txt";  // Zamijeni s pravim URL-om
    std::string output_path = "file.txt";

    if (DownloadFile(url, output_path)) {
        std::cout << "Preuzimanje uspješno!\n";
    } else {
        std::cout << "Greška pri preuzimanju.\n";
    }

    return 0;
}
#endif