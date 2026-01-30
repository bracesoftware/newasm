// NewASM Virtual Machine

int main(int argc, char* argv[])
{
    if constexpr(0) std::ios::sync_with_stdio(false);

    newasm::Experimental::entry();

    try
    {
        newasm::entry(argc, argv);
    }
    catch(const std::exception& e)
    {
        std::cerr << "[NewASM] You got VERY ZAJEBAN -> " << e.what() << std::endl;
    }
    
    if(0) for(int i = 0; i < newasm::forLinker::files.size(); ++i)
    {
        std::cout << newasm::forLinker::files(i).first << " = " << newasm::forLinker::files(i).second << std::endl;
    }

    return 0;
}