// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

module(shell_tools__USER, {
    //setup goes here
});

#define _newasm_CHECKLOGIN if(!newasm::user::loggedin){newasm::header::functions::err("You have to be logged in.");return(1);}std::cout<<""

namespace newasm
{
    namespace user
    {
        bool loggedin = false;
        const std::string username_file = std::string(newasm::core::constants::data_folder+
            newasm::core::constants::separator+
            newasm::core::constants::user_folder+newasm::core::constants::separator+"u._sys");
        const std::string passwd_file = std::string(newasm::core::constants::data_folder+
            newasm::core::constants::separator+
            newasm::core::constants::user_folder+newasm::core::constants::separator+"p._sys");
        const std::string cache_file = std::string(newasm::core::constants::data_folder+
            newasm::core::constants::separator+
            newasm::core::constants::cache_folder+newasm::core::constants::separator+"user._cache");
        namespace global
        {
            const std::string default_user = "root";
            std::string username;
        }
        ///////////////
        void main()
        {
            newasm::user::loggedin = false;
            newasm::user::global::username = newasm::user::global::default_user;
        }
        ///////////////
        int udb_hash(const std::string& input)
        {
            std::size_t hash = 0;
            for (char c : input)
            {
                hash ^= static_cast<std::size_t>(c) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            return static_cast<int>(hash & 0x7FFFFFFF); // samo pozitivne 32-bitne vrijednosti
        }

        static void overwriteFile(const std::string& file, const std::string& text)
        {
            std::ofstream out(file, std::ios::out | std::ios::trunc); // automatski kreira fajl ako ne postoji
            if (out.is_open()) {
                out << text;
                out.close();
            }
        }
		
        static void getFileContent(const std::string& file, std::string& dest)
        {
            std::ifstream in(file);
            if (in.is_open())
            {
                std::ostringstream ss;
                ss << in.rdbuf();
                dest = ss.str();
                in.close();
            } else
            {
                dest = ""; // fajl ne postoji ili ne može da se otvori
            }
        }

        namespace impl
        {
            inline static int getPasswdHash()
            {
                std::string content;

                newasm::user::getFileContent(newasm::user::passwd_file, content);

                content = newasm::header::functions::trim(content);
                
                return std::stoi(content);
            }
            inline static std::string getUsername()
            {
                std::string content;

                newasm::user::getFileContent(newasm::user::username_file, content);

                content = newasm::header::functions::trim(content);
                
                return (content);
            }
            inline static void setPasswdHash(std::string text)
            {
                std::string hash = std::to_string(int(newasm::user::udb_hash(text)));
                newasm::user::overwriteFile(newasm::user::passwd_file, hash);
                return;
            }
            void changePasswd()
            {
                std::cout << newasm::header::col::gray << "\tCurrent password: " << newasm::header::col::reset;
                std::string passwd;
                std::getline(std::cin, passwd);
                passwd = newasm::header::functions::trim(passwd);

                if(newasm::user::udb_hash(passwd) != newasm::user::impl::getPasswdHash())
                {
                    newasm::header::functions::err("Incorrect password!");
                    return;
                }

                std::cout << newasm::header::col::gray << "\tNew password: " << newasm::header::col::reset;
                std::string passwd2;
                std::getline(std::cin, passwd2);
                passwd2 = newasm::header::functions::trim(passwd2);

                newasm::user::impl::setPasswdHash(passwd2);
                newasm::user::main();
                newasm::header::functions::info("Log in again!");
                return;
            }
            void changeUsername()
            {
                std::cout << newasm::header::col::gray << "\tNew username: " << newasm::header::col::reset;
                std::string username;
                std::getline(std::cin, username);
                username = newasm::header::functions::trim(username);
                newasm::user::overwriteFile(newasm::user::username_file, username);

                newasm::user::global::username = username;
                return;
            }
            /*usable*/
            void login()
            {
                std::string content;
                newasm::user::getFileContent(newasm::user::cache_file, content);

                if(!newasm::header::functions::isnumeric(content))
                {
                    content = "0";
                }

                if(std::stoi(content) == 0) //create new user
                {
                    std::cout << newasm::header::col::gray << "\tInput password: " << newasm::header::col::reset;
                    std::string passwd;
                    std::getline(std::cin, passwd);
                    if(!newasm::header::functions::isalphanum(passwd))
                    {
                        newasm::header::functions::err("Password can consist of only alphanumeric characters.");
                        return;
                    }
                    passwd = newasm::header::functions::trim(passwd);
                    if(passwd.size() < 8)
                    {
                        newasm::header::functions::err("Password at least contain 8 characters.");
                        return;
                    }
                    newasm::user::impl::setPasswdHash(passwd);
                    std::cout << newasm::header::col::gray << "\tInput username: " << newasm::header::col::reset;
                    std::string username;
                    std::getline(std::cin, username);
                    username = newasm::header::functions::trim(username);
                    newasm::user::overwriteFile(newasm::user::username_file, username);

                    newasm::user::loggedin = true;
                    newasm::user::global::username = username;
                    
                    newasm::user::overwriteFile(newasm::user::cache_file, "1");
                    return;
                }
                if(std::stoi(content) == 1) //login
                {
                    if(newasm::user::loggedin)
                    {
                        newasm::header::functions::err("Already logged in.");
                        return;
                    }
                    
                    std::cout << newasm::header::col::gray << "\tInput password: " << newasm::header::col::reset;
                    std::string passwd;
                    std::getline(std::cin, passwd);
                    passwd = newasm::header::functions::trim(passwd);

                    if(newasm::user::udb_hash(passwd) != newasm::user::impl::getPasswdHash())
                    {
                        newasm::header::functions::err("Incorrect password!");
                        return;
                    } 

                    newasm::user::loggedin = true;
                    newasm::user::global::username = newasm::user::impl::getUsername();
                    
                    return;
                }
                return;
            }
        }
    }
}