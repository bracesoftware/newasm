// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

module(primitive_garbage_collector, {
    //setup goes here
});

#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

namespace newasm
{
    namespace garbage
    {
        int FLAG = 0;
        std::vector<std::string> addr__;
    }
    class garbageCollector
    {
        public inline static void DO()
        {
            if(newasm::garbage::FLAG == 0)
            {
                return;
            }

            newasm::garbage::FLAG = 0;
            int addr;

            for(int i = 0; i < newasm::garbage::addr__.size(); ++i)
            {
                auto& it = newasm::variables::ids.at(newasm::garbage::addr__.at(i));
                if(it.type = newasm::datatypes::number)
                {
                    addr = it.addr;
                    newasm::hardware::randAccessMem.delete__HEAP(addr, addr + sizeof(int));
                    continue;
                }
                if(it.type = newasm::datatypes::decimal)
                {
                    addr = it.addr;
                    newasm::hardware::randAccessMem.delete__HEAP(addr, addr + sizeof(float));
                    continue;
                }
                if(it.type = newasm::datatypes::character)
                {
                    addr = it.addr;
                    newasm::hardware::randAccessMem.delete__HEAP(addr, addr + sizeof(char));
                    continue;
                }
                if(it.type = newasm::datatypes::text)
                {
                    addr = it.addr;
                    int buffer_len;
                    std::memcpy(&buffer_len, &newasm::hardware::randAccessMem.__memory__[addr], sizeof(int));
                    int bytes = buffer_len + sizeof(int);
                    newasm::hardware::randAccessMem.delete__HEAP(addr, addr + bytes);
                    continue;
                }
            }

            newasm::garbage::addr__.clear();
            return;
        }
    };
}