// NewASM Virtual Machine

int main(int argc, char* argv[])
{
    if constexpr(0) std::ios::sync_with_stdio(false);

    openWindow("NewASM Application Console");
    printToWindow("Pozz Lp");
    printToWindow("Pozz again nigz");
    
    try
    {
        newasm::entry(argc, argv);
    }
    catch(const std::exception& e)
    {
        std::cerr << "[NewASM] You got VERY ZAJEBAN -> " << e.what() << std::endl;
    }

    closeWindow();
    
    return 0;
}