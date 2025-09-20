// NewASM Virtual Machine

int main(int argc, char* argv[])
{
    if constexpr(0) std::ios::sync_with_stdio(false);

    if(__newasm_REG == 0)
    {
        return 0;
    }

    try
    {
        newasm::entry(argc, argv);
    }
    catch(const std::exception& e)
    {
        std::cerr << "[NewASM] You got ZAJEBAN -> " << e.what() << std::endl;
    }
    
    return 0;
}