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
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <curl/curl.h>
#include <fcntl.h>
#include <cstdio>

namespace newasm
{
    namespace net
    {
        static size_t write_file(void* ptr, size_t size, size_t nmemb, void* stream)
        {
            FILE* fp = (FILE*)stream;
            return fwrite(ptr, size, nmemb, fp);
        }

        bool download(const std::string& url, const std::string& output_path)
        {
            CURL* curl = curl_easy_init();
            if (!curl) return false;

            FILE* fp = fopen(output_path.c_str(), "wb");
            if (!fp)
            {
                curl_easy_cleanup(curl);
                return false;
            }

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_file);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

            CURLcode res = curl_easy_perform(curl);

            fclose(fp);
            curl_easy_cleanup(curl);

            return (res == CURLE_OK);
        }
    }

    namespace runtime
    {
        bool start_program(const std::string& path)
        {
            pid_t pid = fork();
            if (pid == 0)
            {
                execl(path.c_str(), path.c_str(), (char*)NULL);
                _exit(1); // ako execl ne uspije
            }
            else if (pid > 0)
            {
                int status;
                waitpid(pid, &status, 0);
                return (WIFEXITED(status) && WEXITSTATUS(status) == 0);
            }
            return false;
        }
    }
}
