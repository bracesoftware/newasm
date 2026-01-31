// NewASM Virtual Machine

namespace newasm
{
    namespace bootloader
    {
        inline void entry() noexcept
        {
            return;
        }
    }
}

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
    
    if(false)
    {
        for(int i = 0; i < newasm::forLinker::lineData.size(); ++i)
        {
            std::cout << newasm::forLinker::lineData.at(i).first << " = " << newasm::forLinker::lineData.at(i).second << std::endl;
        }

        std::cout << "newasm::forLinker::lineData.size(): " << newasm::forLinker::lineData.size() << std::endl;
        std::cout << "newasm::forLinker__OLD::linked_size: " << newasm::forLinker__OLD::linked_size << std::endl;
    }

    return 0;
}