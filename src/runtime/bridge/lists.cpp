// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

namespace newasm
{
    namespace variables
    {
        ATTR_HOT inline void listData::DeleteFromMemory()
        {
            auto& v = this->addr;
            auto& t = this->type;

            for(size_t i = 0; i < v.size(); ++i)
            {
                if(t == newasm::datatypes::number)
                {
                    newasm::RAM->delete__HEAP(v.at(i), v.at(i) + sizeof(int));
                    continue;
                }
                if(t == newasm::datatypes::decimal)
                {
                    newasm::RAM->delete__HEAP(v.at(i), v.at(i) + sizeof(float));
                    continue;
                }
                if(t == newasm::datatypes::character)
                {
                    newasm::RAM->delete__HEAP(v.at(i), v.at(i) + sizeof(char));
                    continue;
                }
                if(t == newasm::datatypes::text)
                {
                    int buffer_len;
                    std::memcpy(&buffer_len, &newasm::hardware::randAccessMem.__memory__[v.at(i)], sizeof(int));
                    int bytes = sizeof(int) + buffer_len;
                    newasm::RAM->delete__HEAP(v.at(i), v.at(i) + bytes);
                    continue;
                }
            }
            return;
        }

        ATTR_HOT inline void listData::IResizeList(int s)
        {
            auto& v = this->addr;
            auto& t = this->type;
            this->IDX = NEWASM_INVALID_LIST_IDX;
            if(s == this->size or s < 0)
            {
                return;
            }
            if(s == 0)
            {
                this->DeleteFromMemory();
                this->size = 0;
                return;
            }
            if(s < this->size)
            {
                this->size = s;
                for(size_t i = 0; i < v.size(); ++i)
                {
                    if(i < s)
                    {
                        continue;
                    }
                    if(t == newasm::datatypes::number)
                    {
                        newasm::RAM->delete__HEAP(v.at(i), v.at(i) + sizeof(int));
                        continue;
                    }
                    if(t == newasm::datatypes::decimal)
                    {
                        newasm::RAM->delete__HEAP(v.at(i), v.at(i) + sizeof(float));
                        continue;
                    }
                    if(t == newasm::datatypes::character)
                    {
                        newasm::RAM->delete__HEAP(v.at(i), v.at(i) + sizeof(char));
                        continue;
                    }
                    if(t == newasm::datatypes::text)
                    {
                        int buffer_len;
                        std::memcpy(&buffer_len, &newasm::hardware::randAccessMem.__memory__[v.at(i)], sizeof(int));
                        int bytes = sizeof(int) + buffer_len;
                        newasm::RAM->delete__HEAP(v.at(i), v.at(i) + bytes);
                        continue;
                    }
                }
                v.resize(this->size);
                return;
            }
            if(s > this->size)
            {
                this->size = s;
                v.resize(this->size, -1);
                for(size_t i = 0; i < v.size(); ++i)
                {
                    if(v.at(i) == -1)
                    {
                        if(t == newasm::datatypes::number)
                        {
                            v[i] = newasm::RAM->write<int>(0);
                            continue;
                        }
                        if(t == newasm::datatypes::decimal)
                        {
                            v[i] = newasm::RAM->write<float>(0.0);
                            continue;
                        }
                        if(t == newasm::datatypes::character)
                        {
                            v[i] = newasm::RAM->write<char>('0');
                            continue;
                        }
                        if(t == newasm::datatypes::text)
                        {
                            v[i] = newasm::RAM->write<std::string>("text");
                            continue;
                        }
                    }
                }
            }
            return;
        }
    }
}