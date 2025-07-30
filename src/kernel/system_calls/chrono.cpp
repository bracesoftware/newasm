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
namespace newasm
{
    namespace chrono
    {
        void main()
        {
            #ifdef ___________d
            newasm::header::functions::log("Fetching chrono data...");
            #endif
            return;
        }
        using namespace std::chrono;
        int year()
        {
            auto now = system_clock::now();
            auto today = floor<days>(now);
            year_month_day ymd{today};
            return int(ymd.year());
        }
        unsigned month()
        {
            auto now = system_clock::now();
            auto today = floor<days>(now);
            year_month_day ymd{today};
            return unsigned(ymd.month());
        }
        unsigned day()
        {
            auto now = system_clock::now();
            auto today = floor<days>(now);
            year_month_day ymd{today};
            return unsigned(ymd.day());
        }
        int hour()
        {
            auto now = system_clock::now();
            auto time = floor<std::chrono::seconds>(now);
            auto day_point = floor<days>(now);
            auto time_since_midnight = time - day_point;
            auto h = duration_cast<hours>(time_since_midnight);
            return int(h.count());
        }
        int minute()
        {
            auto now = system_clock::now();
            auto time = floor<std::chrono::seconds>(now);
            auto day_point = floor<days>(now);
            auto time_since_midnight = time - day_point;
            auto m = duration_cast<minutes>(time_since_midnight - duration_cast<hours>(time_since_midnight));
            return int(m.count());
        }
        int second()
        {
            auto now = system_clock::now();
            auto time = floor<std::chrono::seconds>(now);
            auto day_point = floor<days>(now);
            auto time_since_midnight = time - day_point;
            auto s = duration_cast<seconds>(time_since_midnight - duration_cast<minutes>(time_since_midnight));
            return int(s.count());
        }
    }
}