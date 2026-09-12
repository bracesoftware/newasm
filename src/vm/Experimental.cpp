// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

// TESTING EXPERIMENTAL STUFF

module(experimental__TESTS, {
    //setup goes here
});

namespace newasm
{
    class Experimental final
    {
        public inline static void entry()
        {
            // code goes here
            return;
        }
    };
    
    class procline_insert final
    {
        public:
        int processline(std::string &ln)
        {
            return newasm::procline(ln);
        }
    };

    int operator<<(newasm::procline_insert &obj, std::string &ln)
    {
        obj.processline(ln);
        return 1;
    }
    namespace setup
    {
        namespace args
        {
            const int mode = 0;
            const int ver = 1;
            const int input = 2;
            const int help = 3;

            const int extra = 5;
            const int cnpf = 6;
            const int tests = 7;
            const int log = 8;
            const int nover = 9;

            const std::unordered_map<int, std::string> arg_map = {
                {mode, "-mode"},
                {ver, "-ver"},
                {input, "-input"},
                {help, "-help"},
                {extra, "-extra"},
                {cnpf, "-newproj"},
                {tests, "-tests"},
                {log, "-log"},
                {nover, "-nover"}
            };
        }
    }
    newasm::procline_insert process;
}