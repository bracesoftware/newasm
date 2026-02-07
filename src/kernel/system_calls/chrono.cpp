// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain


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