// NewASM Virtual Machine

int main(int argc, char* argv[])
{
    std::ios::sync_with_stdio(false);

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