// NewASM Virtual Machine

int main(int argc, char* argv[])
{
    if constexpr(0)
    {
        std::ios::sync_with_stdio(false);
        openWindow("NewASM Application Console");
        printToWindow("Pozz Lp");
        printToWindow("Pozz again nigz");
        printToWindow("Ukucaj ime:");
        std::string lmao = inputTextToWindow();
    }
    for(int i = 0; i < 100; ++i)
    {
        printToWindow("pozz za " + lmao + "!");
    }

    try
    {
        newasm::entry(argc, argv);
    }
    catch(const std::exception& e)
    {
        std::cerr << "[NewASM] You got VERY ZAJEBAN -> " << e.what() << std::endl;
    }

    if constexpr(0)
    {
        closeWindow();
    }
    
    return 0;
}