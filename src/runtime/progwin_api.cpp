// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

module(progwin_api, {
    NewASM::Modules::PrintLine("Successfully established a connection.");
});

inline static void send_req(const std::string& msg)
{
    std::string path = newasm::core::constants::data_folder+
        newasm::core::constants::separator+
        newasm::core::constants::cache_folder+
        newasm::core::constants::separator+
        newasm::core::constants::ipc_file;
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    out.write(msg.c_str(), msg.size());
    out.close();

    while(std::ifstream(path)) //wait for response
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

//usable api
namespace newasm
{
    namespace progwin
    {
        std::string buffer;
        namespace api
        {
            ATTR_FLAT inline void cout(std::string text)
            {
                if(!newasm::dwin)
                {
                    return;
                }
                //send_req("cout:"+text+"\n");
                newasm::progwin::buffer = newasm::progwin::buffer + text + "\n";
            }
            ATTR_FLAT inline void flush()
            {
                if(!newasm::dwin)
                {
                    return;
                }
                send_req("cout:" + newasm::progwin::buffer);
                newasm::progwin::buffer = "";
            }
            ATTR_FLAT inline void exit()
            {
                if(!newasm::dwin)
                {
                    return;
                }
                send_req("exit:0");
            }
        }
    }
}