// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

module(test_bootloader, {
    NewASM::Modules::SetError("Test error message.");
});

module(bootloader, {
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
    #if NEWASM_BROKEN_ACTIVE_THREAD_COUNTER
    unsigned int OldValue = 100;
    bool StopActiveThreadCounter = false;
    void CheckForActiveThreads()
    {        
        while(true)
        {
            if(StopActiveThreadCounter)
            {
                return;
            }
            unsigned int CurrentValue = NewASM::header::data::ActiveThreads();
            if(OldValue != CurrentValue)
            {
                std::cout << NewASM::header::col::red;
                std::cout << "=====================================================\n\n";
                std::cout << "ACTIVE THREADZ: " << CurrentValue << '\n' << std::endl;
                std::cout << "=====================================================\n";
                std::cout << NewASM::header::col::reset;
                OldValue = CurrentValue;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    #endif
    inline int __main__(int argc, char* argv[])
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
            return 0;
        }

        newasm::runtime::gui::box("Test", "Mdfkin test", {"xd","lmao","lel","haha", "lesgooo", "i am gay"});

        #if 0
        module(test, {
            NewASM::Modules::PrintLine("Test.");
        });
        #endif
        
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

        #if 0
        auto v = newasm::common::tokenize("mov qword ptr lmao xd");
        for(int i = 0; i < v.size(); ++i)
        {
            std::cout << v.at(i) << std::endl;
        }
        #endif
        return 0;
    }
}

int main(int argc, char** argv)
{
    if constexpr(0)
    {
        std::thread t1(newasm::bootloader::__main__, argc, argv);

        if(t1.joinable())
        {
            t1.join();
        }
    }

    #if NEWASM_BROKEN_ACTIVE_THREAD_COUNTER
    std::thread p(NewASM::bootloader::CheckForActiveThreads);
    #endif

    NewASM::bootloader::__main__(argc, argv);

    NewASM::BetterCPlusPlus::init();

    #if NEWASM_BROKEN_ACTIVE_THREAD_COUNTER
    NewASM::bootloader::StopActiveThreadCounter = true;

    if(p.joinable())
    {
        p.join();
    }
    #endif
    return 0;
}