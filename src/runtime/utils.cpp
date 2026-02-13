// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

__newasm_LOAD_PACKAGE_MODULE(runtime_util, {
    //setup goes here
});

namespace newasm
{
    namespace utils
    {
        inline void displaybar(const std::string &text, float progress)
        {
            int bar_width = 40;
            std::cout << newasm::header::col::reset /*<< newasm::header::col::green*/ << text << newasm::header::col::gray << "[";
            int pos = static_cast<int>(bar_width * progress);
            for (int i = 0; i < bar_width; ++i)
            {
                if (i < pos) std::cout << "=";
                else if (i == pos) std::cout << "*";
                else std::cout << " ";
            }
            std::cout << "] " << newasm::header::col::reset << int(progress * 100.0) << " %\r";
            std::cout.flush();
            return;
        }

        int loadingbar(const std::string &text)
        {
            for (int i = 0; i <= 100; i+=5)
            {
                newasm::utils::displaybar(text, i / 100.0f);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            std::cout << std::endl;
            return 0;
        }

        constinit const float load_speed = 0.009f;
        inline void loading(std::string text, float speed)
        {
            const char animation[] = {'\\', '|', '/', '-'};
            const int anim_len = 4;

            int percent = 0;
            int anim_index = 0;

            auto start_time = std::chrono::steady_clock::now();
            auto next_percent_time = start_time + std::chrono::milliseconds(static_cast<int>(speed * 1000));

            while(percent <= 100)
            {
                std::cout << newasm::header::col::yellow
                << "\r\t" << animation[anim_index] << " "<< newasm::header::col::gray << newasm::header::style::underline << text 
                << newasm::header::col::reset << newasm::header::col::yellow
                << " [" << percent << "%]" << std::flush;
                std::cout << newasm::header::col::reset;

                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                anim_index = (anim_index + 1) % anim_len;

                if (std::chrono::steady_clock::now() >= next_percent_time)
                {
                    percent++;
                    next_percent_time += std::chrono::milliseconds(static_cast<int>(speed * 1000));
                }
            }
            std::cout << newasm::header::col::gray << "\r\t" << "* "<< newasm::header::style::underline << text 
            << newasm::header::col::reset << newasm::header::col::green << " [100%]" << std::flush;

            std::cout << newasm::header::col::reset << std::endl;
        }

        inline int inlineMenu(const std::vector<std::string>& options)
        {
            int selected = 0;
            __NEWASM_inlineMenu___1(startY)
            while(true)
            {
                __NEWASM_inlineMenu___2(startY, options)

                char c = newasm::_compat::getch();
                if(c == 'w' || c == 'W')
                {
                    selected = (selected > 0 ? selected - 1 : options.size() - 1);
                }
                if(c == 's' || c == 'S')
                {
                    selected = (selected + 1) % options.size();
                }
                if(c == '\n' or c == '\r')
                {
                    __NEWASM_inlineMenu___3(startY, options)
                    return selected;
                }
            }
        }

        void type(const std::string& text, int delay_ms = 50)
        {
            for(char c : text)
            {
                std::cout << c << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
            }
            return;
        }

        void type_and_erase(const std::string& text)
        {
            for(char c : text)
            {
                std::cout << c << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            for(size_t i = 0; i < text.size(); ++i)
            {
                std::cout << "\b \b" << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            return;
        }

        void glitch_text(const std::string& text)
        {
            std::string corrupted = text;
            for(int i = 0; i < 150; ++i)
            {
                for(char& c : corrupted)
                {
                    if(rand() % 5 == 0) c = 'A' + rand() % 26;
                }
                std::cout << "\r" << corrupted << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
            std::cout << "\r" << text;
            return;
        }
    }
}