/*

Version: MPL 1.1

The contents of this file are subject to the Mozilla Public License Version 
1.1 the "License"; you may not use this file except in compliance with 
the License. You may obtain a copy of the License at 
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the
License.

Portions created by the Initial Developer are Copyright (c) The COPYRIGHT YEAR
the Initial Developer. All Rights Reserved.

*/

#if 0
namespace newasm
{
    namespace compiler
    {
        namespace core
        {
            void saveInsFile(const std::vector<newasm::compiler::lineData>& data, const std::string& filename)
            {
                std::ofstream out(filename + ".ins", std::ios::binary);
                if(!out)
                {
                    newasm::header::functions::err("Cannot write binaries.");
                    return;
                }

                // vec size
                size_t vecSize = data.size();
                out.write(reinterpret_cast<const char*>(&vecSize), sizeof(vecSize));

                for(const auto& ld : data)
                {
                    // 1) type
                    out.write(reinterpret_cast<const char*>(&ld.type), sizeof(ld.type));

                    // 2) tokens
                    size_t tokenCount = ld.tokens.size();
                    out.write(reinterpret_cast<const char*>(&tokenCount), sizeof(tokenCount));
                    for(const auto& token : ld.tokens)
                    {
                        size_t len = token.size();
                        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
                        out.write(token.data(), len);
                    }

                    // 3) other
                    size_t otherLen = ld.other.size();
                    out.write(reinterpret_cast<const char*>(&otherLen), sizeof(otherLen));
                    out.write(ld.other.data(), otherLen);

                    // 4) raw
                    size_t rawLen = ld.raw.size();
                    out.write(reinterpret_cast<const char*>(&rawLen), sizeof(rawLen));
                    out.write(ld.raw.data(), rawLen);
                }
                return;
            }

            std::vector<newasm::compiler::lineData> loadInsFile(const std::string& filename)
            {
                std::ifstream in(filename + ".ins", std::ios::binary);
                if(!in)
                {
                    newasm::header::functions::err("Cannot read binaries.");
                    return;
                }

                std::vector<newasm::compiler::lineData> result;

                size_t vecSize;
                in.read(reinterpret_cast<char*>(&vecSize), sizeof(vecSize));
                result.resize(vecSize);

                for(auto& ld : result)
                {
                    // 1) type
                    in.read(reinterpret_cast<char*>(&ld.type), sizeof(ld.type));

                    // 2) tokens
                    size_t tokenCount;
                    in.read(reinterpret_cast<char*>(&tokenCount), sizeof(tokenCount));
                    ld.tokens.resize(tokenCount);
                    for (auto& token : ld.tokens) {
                        size_t len;
                        in.read(reinterpret_cast<char*>(&len), sizeof(len));
                        token.resize(len);
                        in.read(&token[0], len);
                    }

                    // 3) other
                    size_t otherLen;
                    in.read(reinterpret_cast<char*>(&otherLen), sizeof(otherLen));
                    ld.other.resize(otherLen);
                    in.read(&ld.other[0], otherLen);

                    // 4) raw
                    size_t rawLen;
                    in.read(reinterpret_cast<char*>(&rawLen), sizeof(rawLen));
                    ld.raw.resize(rawLen);
                    in.read(&ld.raw[0], rawLen);
                }

                return result;
            }
        }
    }
}
#endif