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

#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

namespace newasm
{
    const int operator_T = 1;
    const int alphanum_T = 2;
    namespace operators
    {
        const int alternative = 1;
        const int struct_access = 2;
        const std::unordered_map<std::string, int> _mem = {
            {"??", alternative},
            {"@", struct_access}
        };
    }
    class token
    {
        public:
        int type = 0;
        std::string symbol;
        int operator__ = 0;

        token(){}
        token(int T, std::string S, int O)
            : type(T), symbol(S), operator__(O) {}
    };

    std::vector<newasm::token*> tokens;

    void tokenize(std::string str)
    {
        std::string token = "";
        newasm::token* token_obj = nullptr;
        int type = 0;
        auto is_str_ = [](std::string text, int idx) -> bool {
            std::vector<std::pair<int,int>> positions;
            int first_p = -1, second_p = -1;
       
            for(int i = 0; i < text.size(); ++i)
            {
                if(text.at(i) == '"')
                {
                    if(first_p == -1)
                    {
                        first_p = i;
                        continue;
                    }
                    if(second_p == -1)
                    {
                        second_p = i;
                        continue;
                    }
                    if((first_p != -1) && (second_p != -1))
                    {
                        positions.push_back({first_p, second_p});
                        first_p = -1;
                        second_p = -1;
                        continue;
                    }
                }
            }
            if(first_p == -1)
            {
                newasm::terminate(newasm::exit_codes::invalid_exp);
                return false;
            }
            for(int i = 0; i < positions.size(); ++i)
            {
                if(positions.at(i).first <= idx && idx <= positions.at(i).second)
                {
                    return true;
                }
            }
            return false;
        };
        auto remove_whitespace = [&](std::string &input) -> void {
            std::string result;
            for(int i = 0; i < input.size(); ++i)
            {
                if(is_str_(input, i))
                {
                    continue;
                }
                if(std::isspace(input.at(i)))
                {
                    continue;
                }
                result.push_back(input.at(i));
            }
            input = result;
        };
        remove_whitespace(str);
        std::string query = str;
        for(int i = 0; i < query.size(); ++i)
        {
            /*if(std::isspace(query.at(i)))
            {

                continue;
            }*/
            if(std::isalnum(query.at(i)) || query.at(i) == '_' || is_str_(query, i))
            {
                if(type == 0) type = newasm::alphanum_T;
                if(type == newasm::operator_T)
                {
                    token_obj = new newasm::token();
                    token_obj->type = newasm::operator_T;
                    if(newasm::operators::_mem.find(token) == newasm::operators::_mem.end())//Invalid exp
                    {
                        //std::cout << "Token >> " << token << std::endl;
                        newasm::terminate(newasm::exit_codes::invalid_exp);
                        return;
                    }
                    token_obj->operator__ = newasm::operators::_mem.at(token);
                    token_obj->symbol.assign(token);
                    newasm::tokens.push_back(token_obj);
                    //delete token_obj;
                    token_obj = nullptr;
                    token.clear();
                    type = newasm::alphanum_T;
                }
                token.push_back(query.at(i));
            }// if alfanum
            else
            {
                if(type == 0) type = newasm::operator_T;
                if(type == newasm::alphanum_T)
                {
                    token_obj = new newasm::token();
                    token_obj->type = newasm::alphanum_T;
                    token_obj->symbol.assign(token);
                    token_obj->operator__ = 0;
                    newasm::tokens.push_back(token_obj);
                    //delete token_obj;
                    token_obj = nullptr;
                    token.clear();
                    type = newasm::operator_T;
                }
                token.push_back(query.at(i));
            }//else
        }//for loop
        if(!token.empty())
        {
            token_obj = new newasm::token();
            if(type == newasm::alphanum_T)
            {
                token_obj->type = newasm::alphanum_T;
                token_obj->symbol.assign(token);
                token_obj->operator__ = 0;
                newasm::tokens.push_back(token_obj);
                
                token_obj = nullptr;
                token.clear();
            }
            if(type == newasm::operator_T)
            {
                token_obj->type = newasm::operator_T;
                if(newasm::operators::_mem.find(token) == newasm::operators::_mem.end())//Invalid exp
                {
                    //std::cout << "Token >> " << token << std::endl;
                    newasm::terminate(newasm::exit_codes::invalid_exp);
                    return;
                }
                token_obj->operator__ = newasm::operators::_mem.at(token);
                token_obj->symbol.assign(token);
                newasm::tokens.push_back(token_obj);

                token_obj = nullptr;
                token.clear();
            }
        }
        return;
    }//func end
    namespace impl
    {
        std::string eval(std::string query)
        {
            if(newasm::header::flags::compexpr)
            {
                newasm::header::flags::compexpr = false;
            }
            std::string result, lhs, rhs, temp;
            newasm::tokens.clear();
            newasm::tokenize(query);
            
            /*for(int i = 0; i < newasm::tokens.size(); ++i)
            {
                std::cout << "TOKENS DATA :: " << i << std::endl;
                std::cout << "operator__ >> `" << newasm::tokens.at(i)->operator__ << '`' <<std::endl;
                std::cout << "symbol >> `" << newasm::tokens.at(i)->symbol << '`' <<std::endl;
                std::cout << "type >> `" << newasm::tokens.at(i)->type << '`' <<std::endl;
            }*/

            for(int i = 0; i < newasm::tokens.size(); ++i) // ALTERNATIVE OPERATOR ??
            {
                if(newasm::tokens.at(i)->type == newasm::operator_T)
                {
                    if(newasm::tokens.at(i)->operator__ == newasm::operators::alternative)
                    {
                        if(
                            (i == 0 || i + 1 >= newasm::tokens.size())
                        )/*check if this operator has a lhs and rhs*/{
                            //std::cout << "EVAL db 1 >> query :: " << 
                            //query << " size :: " << newasm::tokens.size() << std::endl;
                            newasm::terminate(newasm::exit_codes::invalid_exp);
                            return temp;
                        }
                        if(!(
                            newasm::tokens.at(i - 1)->type == newasm::alphanum_T &&
                            newasm::tokens.at(i + 1)->type == newasm::alphanum_T
                        ))/*check if lhs and rhs are not operators*/{
                            //std::cout << "EVAL db 2 >> query :: " << 
                            //query << " size :: " << newasm::tokens.size() << std::endl;
                            newasm::terminate(newasm::exit_codes::invalid_exp);
                            return temp;
                        }
                        //alles gut
                        lhs = newasm::tokens.at(i - 1)->symbol;
                        rhs = newasm::tokens.at(i + 1)->symbol;

                        auto memacc = newasm::mem::functions::datavalid(lhs, newasm::mem::data);

                        if(memacc)
                        {
                            auto& vec = newasm::tokens;
                            temp = newasm::mem::data.at(lhs);
                            for (int j = i - 1; j <= i + 1; ++j) delete vec[j];
                            vec.erase(vec.begin() + (i - 1), vec.begin() + (i + 2)); // [i-1, i+2)
                            vec.insert(vec.begin() + (i - 1), new newasm::token(newasm::alphanum_T, temp, 0));
                        }
                        if(!memacc)
                        {
                            auto& vec = newasm::tokens;
                            for (int j = i - 1; j <= i + 1; ++j) delete vec[j];
                            vec.erase(vec.begin() + (i - 1), vec.begin() + (i + 2)); // [i-1, i+2)
                            vec.insert(vec.begin() + (i - 1), new newasm::token(newasm::alphanum_T, rhs, 0));
                        }
                        continue;
                    }
                    //std::cout << "EVAL db 3 >> query :: " << 
                       //     query << " size :: " << newasm::tokens.size() << std::endl;
                    newasm::terminate(newasm::exit_codes::invalid_exp);
                    return temp;
                }
            }
            result = newasm::tokens.at(0)->symbol;
            for(auto* t : newasm::tokens) delete t;
            return result;
        }
    }
} // namespace