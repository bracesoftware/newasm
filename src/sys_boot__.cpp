// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

__newasm_LOAD_PACKAGE_MODULE(bootloader, {
    newasm::header::functions::wait(5000);
    newasm::Console::cls_BARE_METAL__();
    return;
});

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

struct A {};
struct B {};
struct C extends A , B {};
struct D extends private A, protected B {};

class A__ {};
class B__ {};
class C__ extends A__ {};
class D__ extends private C__, public B__ {};

int main(int argc, char* argv[])
{
    if constexpr(false)
    {
        auto prompt = newasm::utils::inlineMenu({"opcija 1", "opcija 2", "opcija 3", "opcija 4"});
        std::cout << "odabrali ste " << prompt << std::endl;
    }
    if constexpr(0) std::ios::sync_with_stdio(false);

    newasm::Experimental::entry();

    try
    {
        newasm::entry(argc, argv);
    }
    catch(const std::exception& e)
    {
        std::cerr << "[NewASM] You got VERY ZAJEBAN -> " << e.what() << std::endl;
        throw;
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