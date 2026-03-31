// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

__newasm_LOAD_PACKAGE_MODULE(test_bootloader, {
    NewASM::Modules::SetError("Test error message.");
});

__newasm_LOAD_PACKAGE_MODULE(bootloader, {
    NewASM::Modules::PrintLine("Bootloader setup finished.");

    NewASM::Modules::SetDestructor(NEWASM_BASIC_FUNCTION_SIG {
        newasm::header::functions::wait(5000);
        return;
    });
    return;
});

#ifndef __NEWASM_BCXX_INTERNET_WORKS
    #error TEST
#endif

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
class E__ extends public D__ {};

namespace newasm::bootloader
{
    int __main__(int argc, char* argv[])
    {
        NewASM::Console::cls_BARE_METAL__();
        if(false)
        {
            auto& dsk = NewASM::hardware::Disk;
            NewASM::Drivers::FileSystem_V::DEFRAG(dsk);
            if(!NewASM::Drivers::FileSystem_V::EXISTS(dsk, "hi.txt"))
            {
                NewASM::Drivers::FileSystem_V::MKFILE(dsk, "hi.txt", "Lmao");
            }
            NewASM::Drivers::FileSystem_V::APPTOFILE(dsk, "hi.txt", "Hello!\n");
            auto content = NewASM::Drivers::FileSystem_V::READFILE(dsk, "hi.txt");
            std::cout << content;
        }
        
        newasm::sealedLabels = &newasm::compiler::data::sealed_labels;
        $defer //BC++ feature, $ blocks
            newasm::GLOBAL::cleanup();
        $// instead of {}, we use $..$

        if constexpr(false)
        {
            auto prompt = newasm::utils::inlineMenu({"opcija 1", "opcija 2", "opcija 3", "opcija 4"});
            std::cout << "odabrali ste " << prompt << std::endl;
        }
        if constexpr(0) std::ios::sync_with_stdio(false);

        NewASM::Experimental::entry();

        try
        {
            NewASM::entry(argc, argv);
        }
        catch(const std::exception& e)
        {
            std::cerr << "[NewASM] You got VERY ZAJEBAN -> " << e.what() << std::endl;
            throw;
        }
        
        if constexpr(false)
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
}

int main(int argc, char** argv)
{
    std::thread t1(newasm::bootloader::__main__, argc, argv);

    if(t1.joinable())
    {
        t1.join();
    }

    NewASM::BetterCPlusPlus::init();
    return 0;
}