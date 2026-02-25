namespace newasm
{
    class __SCREEN_MGR
    {
        public static inline void open()
        {
            std::cout << "\033[?1049h\033[2J\033[H" << std::flush;
        }

        static inline void write(const std::string& text)
        {
            std::cout << text;
        }

        static inline std::string read()
        {
            std::string input;
            std::getline(std::cin, input);
            return input;
        }

        static inline void close()
        {
            std::cout << "\033[?1049l" << std::flush;
            return;
        }
    };
}