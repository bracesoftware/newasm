// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#ifndef __newasm_included
    #error [New-ASM] Cannot compile.
#endif

namespace newasm
{
    namespace runtime
    {
        void main()
        {
            version = newasm::RUNTIME_VERSION;
            newasm::header::functions::log("Runtime loaded.");
        }
        namespace functions
        {
			inline void parse(std::string& suf);
			FORCE_INLINE inline void eval(std::string& s, int mode)
			{
				switch(mode)
				{
					case newasm::runtime::evalModes::sizeOf:
					{
						int size = newasm::header::functions::issizeof(s).second;
						s = std::to_string(size);
						return;
					}
					case newasm::runtime::evalModes::regDeref:
					{
						newasm::header::functions::parseRegDeref(s);
						return;
					}
					case newasm::runtime::evalModes::addressOf:
					{
						newasm::header::functions::parseAddressOf(s);
						return;
					}
					case newasm::runtime::evalModes::valueOf: //primitive types and unions,and whole tuples/contexts
					{
						auto parseFromRAM = [](std::string& suf) -> void {
							if(newasm::variables::ids.find(suf) != newasm::variables::ids.end())
							{
								auto i = newasm::variables::ids.at(suf);
								if(i.type == newasm::datatypes::number)
								{
									if(i.locked)
									{
										suf = std::to_string(0);
										return;
									}
									suf = std::to_string(newasm::hardware::randAccessMem.peek<int>(i.addr));
									return;
								}
								if(i.type == newasm::datatypes::decimal)
								{
									if(i.locked)
									{
										suf = std::to_string(0.0);
										return;
									}
									suf = std::to_string(newasm::hardware::randAccessMem.peek<float>(i.addr));
									return;
								}
								if(i.type == newasm::datatypes::character)
								{
									if(i.locked)
									{
										suf = ("'?'");
										return;
									}
									std::string buf(1, newasm::hardware::randAccessMem.peek<char>(i.addr));
									suf = "'" + buf + "'";
									return;
								}
								if(i.type == newasm::datatypes::text)
								{
									if(i.locked)
									{
										suf = "\"unknown??\"";
										return;
									}
									suf = '"' + newasm::hardware::randAccessMem.peek<std::string>(i.addr) + '"';
									return;
								}
								if(i.type == newasm::datatypes::yunion)
								{
									if(i.locked)
									{
										if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::num)
										{
											suf = std::to_string(0);
											return;
										}
										if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::decm)
										{
											suf = std::to_string(0.0);
											return;
										}
										if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::char__)
										{
											suf = "'?'";
											return;
										}
										if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::txt)
										{
											suf = "\"unknown??\"";
											return;
										}
										return;
									}

									if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::num)
									{
										suf = std::to_string(newasm::hardware::randAccessMem.peek<int>(i.yunion->addr));
										return;
									}
									if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::decm)
									{
										suf = std::to_string(newasm::hardware::randAccessMem.peek<float>(i.yunion->addr));
										return;
									}
									if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::char__)
									{
										std::string buf(1, newasm::hardware::randAccessMem.peek<char>(i.yunion->addr));
										suf = "'" + buf + "'";
										return;
									}
									if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::txt)
									{
										suf = '"' + newasm::hardware::randAccessMem.peek<std::string>(i.yunion->addr) + '"';
										return;
									}
									return;
								}
								//tuples and contexts
								//TUPLES
								if(i.type == newasm::datatypes::tuple)
								{
									std::vector<std::string> contents;
									std::string temp, parsed_contents;
									auto tuple_size = int(i.tuple->addr.size());
									for(int k = 0; k < tuple_size; ++k)
									{
										if(i.tuple->type[k] == newasm::datatypes::number)
										{
											temp = std::to_string(newasm::hardware::randAccessMem.peek<int>(i.tuple->addr[k]));
											contents.push_back(temp);
											continue;
										}
										if(i.tuple->type[k] == newasm::datatypes::decimal)
										{
											temp = std::to_string(newasm::hardware::randAccessMem.peek<float>(i.tuple->addr[k]));
											contents.push_back(temp);
											continue;
										}
										if(i.tuple->type[k] == newasm::datatypes::character)
										{
											std::string buf(1, newasm::hardware::randAccessMem.peek<char>(i.tuple->addr[k]));
											temp = "'" + buf + "'";
											contents.push_back(temp);
											continue;
										}
										if(i.tuple->type[k] == newasm::datatypes::text)
										{
											std::string buf = (newasm::hardware::randAccessMem.peek<std::string>(i.tuple->addr[k]));
											temp = "\"" + buf + "\"";
											contents.push_back(temp);
											continue;
										}
									}
									temp.clear();
									parsed_contents = "(";
									for(int k = 0; k < tuple_size; ++k)
									{
										temp = contents.at(k);
										parsed_contents.append(temp);
										if(k + 1 != tuple_size)
										{
											parsed_contents.append(",");
										}
										if(k + 1 == tuple_size)
										{
											parsed_contents.append(")");
										}
									}
									suf = parsed_contents;
								}
								/////////////CONTEXTDS
								if(i.type == newasm::datatypes::mycontext)
								{
									std::vector<std::string> contents;
									std::string temp, parsed_contents;
									
									int context_size = i.context->addr.size();

									for(int idx = 0; idx < context_size; ++idx)
									{
										temp.clear();
										temp.append("\"" + i.context->keys[idx] + "\":");
										if(i.context->type[idx] == newasm::datatypes::number)
										{
											temp.append(std::to_string(newasm::hardware::randAccessMem.peek<int>(i.context->addr[idx])));
											contents.push_back(temp);
											continue;
										}
										if(i.context->type[idx] == newasm::datatypes::decimal)
										{
											temp.append(std::to_string(newasm::hardware::randAccessMem.peek<float>(i.context->addr[idx])));
											contents.push_back(temp);
											continue;
										}
										if(i.context->type[idx] == newasm::datatypes::character)
										{
											std::string buf(1, (newasm::hardware::randAccessMem.peek<char>(i.context->addr[idx])));
											temp.append("'" + buf + "'");
											contents.push_back(temp);
											continue;
										}
										if(i.context->type[idx] == newasm::datatypes::text)
										{
											std::string buf = newasm::hardware::randAccessMem.peek<std::string>(i.context->addr[idx]);
											temp.append("\"" + buf + "\"");
											contents.push_back(temp);
											continue;
										}
									}

									temp.clear();
									parsed_contents.clear();
									parsed_contents = "(";
									for(int u = 0; u < context_size; ++u)
									{
										temp = contents.at(u);
										parsed_contents.append(temp);
										if(u + 1 != context_size)
										{
											parsed_contents.append(",");
										}
										if(u + 1 == context_size)
										{
											parsed_contents.append(")");
										}
									}
									suf = parsed_contents;
								}
							}
							return;
						};
						parseFromRAM(s);
						newasm::header::functions::parseopr(s, newasm::mem::data);
						newasm::parseopr_struct(s);
						return;
					}
					case newasm::runtime::evalModes::environmentVariable:
					{
						//std::cout << "Is this called? -> " << s << std::endl;
						s = s.substr(2);
						for(std::vector<std::pair<std::string,std::string>>::iterator i = newasm::env_vars->begin(); i < newasm::env_vars->end(); ++i)
						{
							if(s == i->first)
							{
								s = i->second;
								break;
							}
						}
						return;
					}
					case newasm::runtime::evalModes::referenceOfNamespacedVar:
					{
						std::string newsuf = newasm::header::functions::trim(s.substr(1));
						if(newasm::header::functions::parseNamespaceSegments(newsuf).first)
						{
							newasm::progwin::api::cout("Yes NMS -> " + s);
							auto i = newasm::header::functions::parseNamespaceSegments(newsuf);
							std::string symbol_name = i.second.back();
							auto vec = i.second;
							vec.pop_back(); // namespace list
							
							s = static_cast<std::string>("&") + newasm::header::functions::mangleName(vec, symbol_name);
							return;
						}
						if(newasm::header::functions::parseObject(newsuf).first)
						{
							newasm::progwin::api::cout("Object<yes::ref> NMS -> " + s);

							auto objectData = newasm::header::functions::parseObject(newsuf);
							auto objectName = objectData.second.first;
							auto objectMember = objectData.second.second;
							if(newasm::header::functions::parseNamespaceSegments(objectName).first)
							{
								auto i = newasm::header::functions::parseNamespaceSegments(objectName);
								//std::cout << "objectName -> " << objectName << "| i.size() -> " << i.second.size() << std::endl;
								std::string symbol_name = i.second.back();
								auto vec = i.second;
								vec.pop_back(); // namespace list
								
								s = static_cast<std::string>("&") + newasm::header::functions::mangleName(vec, symbol_name) + 
								static_cast<std::string>("{")+objectMember+static_cast<std::string>("}");
								return;
							}
						}
						return;
					}
					case newasm::runtime::evalModes::valueOfNamespacedVar:
					{
						auto parseFromRAM = [](std::string& suf) -> void {
							if(newasm::variables::ids.find(suf) != newasm::variables::ids.end())
							{
								auto i = newasm::variables::ids.at(suf);
								if(i.type == newasm::datatypes::number)
								{
									if(i.locked)
									{
										suf = std::to_string(0);
										return;
									}
									suf = std::to_string(newasm::hardware::randAccessMem.peek<int>(i.addr));
									return;
								}
								if(i.type == newasm::datatypes::decimal)
								{
									if(i.locked)
									{
										suf = std::to_string(0.0);
										return;
									}
									suf = std::to_string(newasm::hardware::randAccessMem.peek<float>(i.addr));
									return;
								}
								if(i.type == newasm::datatypes::character)
								{
									if(i.locked)
									{
										suf = ("'?'");
										return;
									}
									std::string buf(1, newasm::hardware::randAccessMem.peek<char>(i.addr));
									suf = "'" + buf + "'";
									return;
								}
								if(i.type == newasm::datatypes::text)
								{
									if(i.locked)
									{
										suf = "\"unknown??\"";
										return;
									}
									suf = '"' + newasm::hardware::randAccessMem.peek<std::string>(i.addr) + '"';
									return;
								}
								if(i.type == newasm::datatypes::yunion)
								{
									if(i.locked)
									{
										if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::num)
										{
											suf = std::to_string(0);
											return;
										}
										if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::decm)
										{
											suf = std::to_string(0.0);
											return;
										}
										if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::char__)
										{
											suf = "'?'";
											return;
										}
										if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::txt)
										{
											suf = "\"unknown??\"";
											return;
										}
										return;
									}

									if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::num)
									{
										suf = std::to_string(newasm::hardware::randAccessMem.peek<int>(i.yunion->addr));
										return;
									}
									if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::decm)
									{
										suf = std::to_string(newasm::hardware::randAccessMem.peek<float>(i.yunion->addr));
										return;
									}
									if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::char__)
									{
										std::string buf(1, newasm::hardware::randAccessMem.peek<char>(i.yunion->addr));
										suf = "'" + buf + "'";
										return;
									}
									if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::txt)
									{
										suf = '"' + newasm::hardware::randAccessMem.peek<std::string>(i.yunion->addr) + '"';
										return;
									}
									return;
								}
								//tuples and contexts
								//TUPLES
								if(i.type == newasm::datatypes::tuple)
								{
									std::vector<std::string> contents;
									std::string temp, parsed_contents;
									auto tuple_size = int(i.tuple->addr.size());
									for(int k = 0; k < tuple_size; ++k)
									{
										if(i.tuple->type[k] == newasm::datatypes::number)
										{
											temp = std::to_string(newasm::hardware::randAccessMem.peek<int>(i.tuple->addr[k]));
											contents.push_back(temp);
											continue;
										}
										if(i.tuple->type[k] == newasm::datatypes::decimal)
										{
											temp = std::to_string(newasm::hardware::randAccessMem.peek<float>(i.tuple->addr[k]));
											contents.push_back(temp);
											continue;
										}
										if(i.tuple->type[k] == newasm::datatypes::character)
										{
											std::string buf(1, newasm::hardware::randAccessMem.peek<char>(i.tuple->addr[k]));
											temp = "'" + buf + "'";
											contents.push_back(temp);
											continue;
										}
										if(i.tuple->type[k] == newasm::datatypes::text)
										{
											std::string buf = (newasm::hardware::randAccessMem.peek<std::string>(i.tuple->addr[k]));
											temp = "\"" + buf + "\"";
											contents.push_back(temp);
											continue;
										}
									}
									temp.clear();
									parsed_contents = "(";
									for(int k = 0; k < tuple_size; ++k)
									{
										temp = contents.at(k);
										parsed_contents.append(temp);
										if(k + 1 != tuple_size)
										{
											parsed_contents.append(",");
										}
										if(k + 1 == tuple_size)
										{
											parsed_contents.append(")");
										}
									}
									suf = parsed_contents;
								}
								/////////////CONTEXTDS
								if(i.type == newasm::datatypes::mycontext)
								{
									std::vector<std::string> contents;
									std::string temp, parsed_contents;
									
									int context_size = i.context->addr.size();

									for(int idx = 0; idx < context_size; ++idx)
									{
										temp.clear();
										temp.append("\"" + i.context->keys[idx] + "\":");
										if(i.context->type[idx] == newasm::datatypes::number)
										{
											temp.append(std::to_string(newasm::hardware::randAccessMem.peek<int>(i.context->addr[idx])));
											contents.push_back(temp);
											continue;
										}
										if(i.context->type[idx] == newasm::datatypes::decimal)
										{
											temp.append(std::to_string(newasm::hardware::randAccessMem.peek<float>(i.context->addr[idx])));
											contents.push_back(temp);
											continue;
										}
										if(i.context->type[idx] == newasm::datatypes::character)
										{
											std::string buf(1, (newasm::hardware::randAccessMem.peek<char>(i.context->addr[idx])));
											temp.append("'" + buf + "'");
											contents.push_back(temp);
											continue;
										}
										if(i.context->type[idx] == newasm::datatypes::text)
										{
											std::string buf = newasm::hardware::randAccessMem.peek<std::string>(i.context->addr[idx]);
											temp.append("\"" + buf + "\"");
											contents.push_back(temp);
											continue;
										}
									}

									temp.clear();
									parsed_contents.clear();
									parsed_contents = "(";
									for(int u = 0; u < context_size; ++u)
									{
										temp = contents.at(u);
										parsed_contents.append(temp);
										if(u + 1 != context_size)
										{
											parsed_contents.append(",");
										}
										if(u + 1 == context_size)
										{
											parsed_contents.append(")");
										}
									}
									suf = parsed_contents;
								}
							}
							return;
						};
						newasm::progwin::api::cout("Yes NMS -> " + s);
						auto i = newasm::header::functions::parseNamespaceSegments(s);
						if(i.first)
						{
							std::string symbol_name = i.second.back();
							auto vec = i.second;
							vec.pop_back(); // namespace list
							
							s = newasm::header::functions::mangleName(vec, symbol_name);
						}
						parseFromRAM(s);
						newasm::header::functions::parseopr(s, newasm::mem::data);
						newasm::parseopr_struct(s);
						return;
					}
					case newasm::runtime::evalModes::valueOfNamespacedTupleOrContext:
					{
						//std::cout << "IF TUPLE x2 << " << suf << "\n";
						std::string& suf = s;
						
						auto tupleName = newasm::header::functions::trim(newasm::header::functions::checkTupleFormat(suf).second.first);
						auto tupleIndex = newasm::header::functions::trim(newasm::header::functions::checkTupleFormat(suf).second.second);
						auto& tupleOrContextName = tupleName;
						auto& tupleOrContextIndex = tupleIndex;

						auto i = newasm::header::functions::parseNamespaceSegments(tupleName);
						if(i.first)
						{
							std::string symbol_name = i.second.back();
							auto vec = i.second;
							vec.pop_back(); // namespace list
							
							tupleName = newasm::header::functions::mangleName(vec, symbol_name);
						}
						parse(tupleIndex);
						bool indexNumeric = newasm::header::functions::isnumeric(tupleIndex);
						if(!indexNumeric)
						{
							suf = newasm::header::constants::inv_reg_val;
							bool indexText = newasm::header::functions::istext(tupleOrContextIndex);
							if(indexText)
							{
								//newasm::header::functions::err("tupleOrContextName: `" + tupleOrContextName + "`");
								//newasm::header::functions::err("tupleOrContextIndex: `" + tupleOrContextIndex + "`");
								bool validContext = true;
								auto it = newasm::variables::ids.find(tupleOrContextName);
								if(it == newasm::variables::ids.end())
								{
									validContext = false;
								}
								if(it->second.type != newasm::datatypes::mycontext)
								{
									validContext = false;
								}
								if(!validContext)
								{
									newasm::terminate(newasm::exit_codes::invalid_memacc);
								}

								tupleOrContextIndex = newasm::header::functions::remq(tupleOrContextIndex);
								int idx = newasm::header::functions::getIndex<std::string>(it->second.context->keys, tupleOrContextIndex);
								#if 0
								if(idx != (-1))
								{
									newasm::header::functions::wrn("Found key: `" + tupleOrContextIndex + "` at idx " + std::to_string(idx) + ", it->second.context->keys.at(idx): `" + it->second.context->keys.at(idx) + "`");
								}
								#endif
								if(idx == (-1))
								{
									newasm::terminate(newasm::exit_codes::invalid_memacc);
									return;
								}

								if(it->second.context->type[idx] == newasm::datatypes::number)
								{
									suf = std::to_string(newasm::hardware::randAccessMem.peek<int>(it->second.context->addr[idx]));
								}
								if(it->second.context->type[idx] == newasm::datatypes::decimal)
								{
									suf = std::to_string(newasm::hardware::randAccessMem.peek<float>(it->second.context->addr[idx]));
								}
								if(it->second.context->type[idx] == newasm::datatypes::character)
								{
									std::string buf(1, newasm::hardware::randAccessMem.peek<char>(it->second.context->addr[idx]));
									suf = "'" + buf + "'";
								}
								if(it->second.context->type[idx] == newasm::datatypes::text)
								{
									std::string buf = newasm::hardware::randAccessMem.peek<std::string>(it->second.context->addr[idx]);
									suf = "\"" + buf + "\"";
								}
								return;
							}
						}

						bool validTuple = true;
						auto it = newasm::variables::ids.find(tupleName);
						if(it == newasm::variables::ids.end())
						{
							validTuple = false;
						}
						if(it->second.type != newasm::datatypes::tuple)
						{
							validTuple = false;
						}
						if(!validTuple)
						{
							newasm::terminate(newasm::exit_codes::invalid_memacc);
						}
						if(validTuple && indexNumeric)
						{
							int index = std::stoi(tupleIndex);
							if(index >= it->second.tuple->addr.size() || index < 0)
							{
								newasm::terminate(newasm::exit_codes::seg_fault);
								return;
							}
							if(it->second.tuple->type[index] == newasm::datatypes::number)
							{
								if(it->second.locked)
								{
									suf = "0";
									return;
								}
								suf = std::to_string(newasm::hardware::randAccessMem.peek<int>(it->second.tuple->addr[index]));
							}
							if(it->second.tuple->type[index] == newasm::datatypes::decimal)
							{
								if(it->second.locked)
								{
									suf = "0.0";
									return;
								}
								suf = std::to_string(newasm::hardware::randAccessMem.peek<float>(it->second.tuple->addr[index]));
							}
							if(it->second.tuple->type[index] == newasm::datatypes::character)
							{
								if(it->second.locked)
								{
									suf = "'?'";
									return;
								}
								std::string buf(1, newasm::hardware::randAccessMem.peek<char>(it->second.tuple->addr[index]));
								suf = "'" + buf + "'";
							}
							if(it->second.tuple->type[index] == newasm::datatypes::text)
							{
								if(it->second.locked)
								{
									suf = "\"unknown??\"";
									return;
								}
								std::string buf = newasm::hardware::randAccessMem.peek<std::string>(it->second.tuple->addr[index]);
								suf = "\"" + buf + "\"";
							}
						}
						return;
					}
				}
				parse(s);
				return;
			}

            inline void parse(std::string& suf)
            {
				#if 0
				fix_spaces<'&'>(suf);
				fix_spaces<'#'>(suf);
				fix_spaces<'~'>(suf);
				suf = lenofop(suf);
				#endif
				newasm::header::functions::parseRegDeref(suf);
				newasm::header::functions::parseAddressOf(suf);
				bool mangled = false;
				
				//valueOfNamespacedVar
				if(newasm::header::functions::parseNamespaceSegments(suf).first)
				{
					newasm::progwin::api::cout("Yes NMS -> " + suf);
					auto i = newasm::header::functions::parseNamespaceSegments(suf);
					std::string symbol_name = i.second.back();
					auto vec = i.second;
					vec.pop_back(); // namespace list
					
					suf = newasm::header::functions::mangleName(vec, symbol_name);
					mangled = true;
					
					/*
						this is abnormal
					*/
					#if 0
					if(vec.size() != newasm::mem::data_attrib[symbol_name].namespaces.size())
					{
						newasm::terminate(newasm::exit_codes::invalid_memacc);
						return;
					}
					for(int i = 0; i < expected_size; ++i)
					{
						if(vec.at(i) != newasm::mem::data_attrib[symbol_name].namespaces.at(i))
						{
							newasm::terminate(newasm::exit_codes::invalid_memacc);
							return;
						}
					}
					suf = newasm::header::functions::parseNamespaceSegments(suf).second.back();
					#endif
				}
				//referenceOfNamespacedVar
                if(newasm::header::functions::isref(suf))
                {
					std::string newsuf = newasm::header::functions::trim(suf.substr(1));
					if(newasm::header::functions::parseNamespaceSegments(newsuf).first)
					{
						newasm::progwin::api::cout("Yes NMS -> " + suf);
						auto i = newasm::header::functions::parseNamespaceSegments(newsuf);
						std::string symbol_name = i.second.back();
						auto vec = i.second;
						vec.pop_back(); // namespace list
						
						suf = static_cast<std::string>("&") + newasm::header::functions::mangleName(vec, symbol_name);
						return;
					}
					if(newasm::header::functions::parseObject(newsuf).first)
					{
						newasm::progwin::api::cout("Object<yes::ref> NMS -> " + suf);

						auto objectData = newasm::header::functions::parseObject(newsuf);
						auto objectName = objectData.second.first;
						auto objectMember = objectData.second.second;
						if(newasm::header::functions::parseNamespaceSegments(objectName).first)
						{
							auto i = newasm::header::functions::parseNamespaceSegments(objectName);
							//std::cout << "objectName -> " << objectName << "| i.size() -> " << i.second.size() << std::endl;
							std::string symbol_name = i.second.back();
							auto vec = i.second;
							vec.pop_back(); // namespace list
							
							suf = static_cast<std::string>("&") + newasm::header::functions::mangleName(vec, symbol_name) + 
							static_cast<std::string>("{")+objectMember+static_cast<std::string>("}");
							return;
						}
					}
                    return;
                }
				//if tuple
				//valueOfNamespacedTuple
				if(newasm::header::functions::checkTupleFormat(suf).first)
				{
					//std::cout << "IF TUPLE x1 << " << suf << "\n";
					auto tupleName = newasm::header::functions::trim(newasm::header::functions::checkTupleFormat(suf).second.first);
					auto tupleIndex = newasm::header::functions::trim(newasm::header::functions::checkTupleFormat(suf).second.second);
					
					if(newasm::header::functions::parseNamespaceSegments(tupleName).first)
					{
						newasm::progwin::api::cout("Yes NMS -> " + suf);
						auto i = newasm::header::functions::parseNamespaceSegments(tupleName);
						std::string symbol_name = i.second.back();
						auto vec = i.second;
						vec.pop_back(); // namespace list
						parse(tupleIndex); // i love recursion :D
						
						suf = newasm::header::functions::mangleName(vec, symbol_name) + "(" + tupleIndex + ")";
					}
				}
                if(newasm::mem::data_attrib[suf].locked)
                {
                    suf = "\"unknown??\"";
                }
				//environmentVariable
                std::vector<std::string> tokens;
                for(std::vector<std::pair<std::string,std::string>>::iterator i = newasm::env_vars->begin(); i < newasm::env_vars->end(); ++i)
                {
                    tokens = newasm::header::functions::split_fixed(suf,'/');
                    if(newasm::header::functions::trim(tokens[0]) == "*") if(newasm::header::functions::trim(tokens[1]) == i->first)
                    {
                        suf = i->second;
                    }
                }
				
				if((newasm::mem::data.find(suf) == newasm::mem::data.end()) && (newasm::variables::ids.find(suf) == newasm::variables::ids.end()))
				{
					if(mangled)
					{
						//std::cout << "PravoSiSeZajebucnuoException::0x873 -> "<< suf << "\n";
						newasm::terminate(newasm::exit_codes::invalid_memacc);
					}
				}

                newasm::header::functions::parseopr(suf, newasm::mem::data);
				
                auto parseFromRAM = [](std::string& suf) -> void {
					if(newasm::variables::ids.find(suf) != newasm::variables::ids.end())
					{
						auto i = newasm::variables::ids.at(suf);
						if(i.type == newasm::datatypes::number)
						{
							if(i.locked)
							{
								suf = std::to_string(0);
								return;
							}
							suf = std::to_string(newasm::hardware::randAccessMem.peek<int>(i.addr));
							return;
						}
						if(i.type == newasm::datatypes::decimal)
						{
							if(i.locked)
							{
								suf = std::to_string(0.0);
								return;
							}
							suf = std::to_string(newasm::hardware::randAccessMem.peek<float>(i.addr));
							return;
						}
						if(i.type == newasm::datatypes::character)
						{
							if(i.locked)
							{
								suf = ("'?'");
								return;
							}
							std::string buf(1, newasm::hardware::randAccessMem.peek<char>(i.addr));
							suf = "'" + buf + "'";
							return;
						}
						if(i.type == newasm::datatypes::text)
						{
							if(i.locked)
							{
								suf = "\"unknown??\"";
								return;
							}
							suf = '"' + newasm::hardware::randAccessMem.peek<std::string>(i.addr) + '"';
							return;
						}
						if(i.type == newasm::datatypes::yunion)
						{
							if(i.locked)
							{
								if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::num)
								{
									suf = std::to_string(0);
									return;
								}
								if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::decm)
								{
									suf = std::to_string(0.0);
									return;
								}
								if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::char__)
								{
									suf = "'?'";
									return;
								}
								if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::txt)
								{
									suf = "\"unknown??\"";
									return;
								}
								return;
							}

							if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::num)
							{
								suf = std::to_string(newasm::hardware::randAccessMem.peek<int>(i.yunion->addr));
								return;
							}
							if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::decm)
							{
								suf = std::to_string(newasm::hardware::randAccessMem.peek<float>(i.yunion->addr));
								return;
							}
							if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::char__)
							{
								std::string buf(1, newasm::hardware::randAccessMem.peek<char>(i.yunion->addr));
								suf = "'" + buf + "'";
								return;
							}
							if(newasm::header::data::movas_type == newasm::core::lang_inf::typenames::txt)
							{
								suf = '"' + newasm::hardware::randAccessMem.peek<std::string>(i.yunion->addr) + '"';
								return;
							}
							return;
						}
					}
					return;
				};
				parseFromRAM(suf);
				newasm::parseopr_struct(suf);
				if constexpr(0)
				{
					try
					{
						std::cout;
					}
					catch(std::exception& e)
					{
						std::cout << "ParseObject :: ZAJEBUCNUO TE -> " << e.what() << std::endl;
					}
				}

                /*if(newasm::chars::map.find(suf) != newasm::chars::map.end())
                {
                    suf = newasm::chars::map.at(suf);
                }*/
				
				// Just a specific tuple OR context index
				if(newasm::header::functions::checkTupleFormat(suf).first)
				{
					//std::cout << "IF TUPLE x2 << " << suf << "\n";
					auto tupleName = newasm::header::functions::trim(newasm::header::functions::checkTupleFormat(suf).second.first);
					auto tupleIndex = newasm::header::functions::trim(newasm::header::functions::checkTupleFormat(suf).second.second);
					auto& tupleOrContextName = tupleName;
					auto& tupleOrContextIndex = tupleIndex;

					parse(tupleIndex);
					bool indexNumeric = newasm::header::functions::isnumeric(tupleIndex);
					if(!indexNumeric)
					{
						suf = newasm::header::constants::inv_reg_val;
						bool indexText = newasm::header::functions::istext(tupleOrContextIndex);
						if(indexText)
						{
							//newasm::header::functions::err("tupleOrContextName: `" + tupleOrContextName + "`");
							//newasm::header::functions::err("tupleOrContextIndex: `" + tupleOrContextIndex + "`");
							bool validContext = true;
							auto it = newasm::variables::ids.find(tupleOrContextName);
							if(it == newasm::variables::ids.end())
							{
								validContext = false;
							}
							if(it->second.type != newasm::datatypes::mycontext)
							{
								validContext = false;
							}
							if(!validContext)
							{
								newasm::terminate(newasm::exit_codes::invalid_memacc);
							}

							tupleOrContextIndex = newasm::header::functions::remq(tupleOrContextIndex);
							int idx = newasm::header::functions::getIndex<std::string>(it->second.context->keys, tupleOrContextIndex);
							#if 0
							if(idx != (-1))
							{
								newasm::header::functions::wrn("Found key: `" + tupleOrContextIndex + "` at idx " + std::to_string(idx) + ", it->second.context->keys.at(idx): `" + it->second.context->keys.at(idx) + "`");
							}
							#endif
							if(idx == (-1))
							{
								newasm::terminate(newasm::exit_codes::invalid_memacc);
								return;
							}

							if(it->second.context->type[idx] == newasm::datatypes::number)
							{
								suf = std::to_string(newasm::hardware::randAccessMem.peek<int>(it->second.context->addr[idx]));
							}
							if(it->second.context->type[idx] == newasm::datatypes::decimal)
							{
								suf = std::to_string(newasm::hardware::randAccessMem.peek<float>(it->second.context->addr[idx]));
							}
							if(it->second.context->type[idx] == newasm::datatypes::character)
							{
								std::string buf(1, newasm::hardware::randAccessMem.peek<char>(it->second.context->addr[idx]));
								suf = "'" + buf + "'";
							}
							if(it->second.context->type[idx] == newasm::datatypes::text)
							{
								std::string buf = newasm::hardware::randAccessMem.peek<std::string>(it->second.context->addr[idx]);
								suf = "\"" + buf + "\"";
							}
							return;
						}
					}

					bool validTuple = true;
					auto it = newasm::variables::ids.find(tupleName);
					if(it == newasm::variables::ids.end())
					{
						validTuple = false;
					}
					if(it->second.type != newasm::datatypes::tuple)
					{
						validTuple = false;
					}
					if(!validTuple)
					{
						newasm::terminate(newasm::exit_codes::invalid_memacc);
					}
					if(validTuple && indexNumeric)
					{
						int index = std::stoi(tupleIndex);
						if(index >= it->second.tuple->addr.size() || index < 0)
						{
							newasm::terminate(newasm::exit_codes::seg_fault);
							return;
						}
						if(it->second.tuple->type[index] == newasm::datatypes::number)
						{
							if(it->second.locked)
							{
								suf = "0";
								return;
							}
							suf = std::to_string(newasm::hardware::randAccessMem.peek<int>(it->second.tuple->addr[index]));
						}
						if(it->second.tuple->type[index] == newasm::datatypes::decimal)
						{
							if(it->second.locked)
							{
								suf = "0.0";
								return;
							}
							suf = std::to_string(newasm::hardware::randAccessMem.peek<float>(it->second.tuple->addr[index]));
						}
						if(it->second.tuple->type[index] == newasm::datatypes::character)
						{
							if(it->second.locked)
							{
								suf = "'?'";
								return;
							}
							std::string buf(1, newasm::hardware::randAccessMem.peek<char>(it->second.tuple->addr[index]));
							suf = "'" + buf + "'";
						}
						if(it->second.tuple->type[index] == newasm::datatypes::text)
						{
							if(it->second.locked)
							{
								suf = "\"unknown??\"";
								return;
							}
							std::string buf = newasm::hardware::randAccessMem.peek<std::string>(it->second.tuple->addr[index]);
							suf = "\"" + buf + "\"";
						}
					}
				}
				// Whole tuple OR context
				auto it = newasm::variables::ids.find(suf);
				if(it != newasm::variables::ids.end())
				{
					//TUPLES
					if(it->second.type == newasm::datatypes::tuple)
					{
						std::vector<std::string> contents;
						std::string temp, parsed_contents;
						auto tuple_size = int(it->second.tuple->addr.size());
						for(int i = 0; i < tuple_size; ++i)
						{
							if(it->second.tuple->type[i] == newasm::datatypes::number)
							{
								temp = std::to_string(newasm::hardware::randAccessMem.peek<int>(it->second.tuple->addr[i]));
								contents.push_back(temp);
								continue;
							}
							if(it->second.tuple->type[i] == newasm::datatypes::decimal)
							{
								temp = std::to_string(newasm::hardware::randAccessMem.peek<float>(it->second.tuple->addr[i]));
								contents.push_back(temp);
								continue;
							}
							if(it->second.tuple->type[i] == newasm::datatypes::character)
							{
								std::string buf(1, newasm::hardware::randAccessMem.peek<char>(it->second.tuple->addr[i]));
								temp = "'" + buf + "'";
								contents.push_back(temp);
								continue;
							}
							if(it->second.tuple->type[i] == newasm::datatypes::text)
							{
								std::string buf = (newasm::hardware::randAccessMem.peek<std::string>(it->second.tuple->addr[i]));
								temp = "\"" + buf + "\"";
								contents.push_back(temp);
								continue;
							}
						}
						temp.clear();
						parsed_contents = "(";
						for(int i = 0; i < tuple_size; ++i)
						{
							temp = contents.at(i);
							parsed_contents.append(temp);
							if(i + 1 != tuple_size)
							{
								parsed_contents.append(",");
							}
							if(i + 1 == tuple_size)
							{
								parsed_contents.append(")");
							}
						}
						suf = parsed_contents;
					}
					/////////////CONTEXTDS
					if(it->second.type == newasm::datatypes::mycontext)
					{
						std::vector<std::string> contents;
						std::string temp, parsed_contents;
						
						int context_size = it->second.context->addr.size();

						for(int idx = 0; idx < context_size; ++idx)
						{
							temp.clear();
							temp.append("\"" + it->second.context->keys[idx] + "\":");
							if(it->second.context->type[idx] == newasm::datatypes::number)
							{
								temp.append(std::to_string(newasm::hardware::randAccessMem.peek<int>(it->second.context->addr[idx])));
								contents.push_back(temp);
								continue;
							}
							if(it->second.context->type[idx] == newasm::datatypes::decimal)
							{
								temp.append(std::to_string(newasm::hardware::randAccessMem.peek<float>(it->second.context->addr[idx])));
								contents.push_back(temp);
								continue;
							}
							if(it->second.context->type[idx] == newasm::datatypes::character)
							{
								std::string buf(1, (newasm::hardware::randAccessMem.peek<char>(it->second.context->addr[idx])));
								temp.append("'" + buf + "'");
								contents.push_back(temp);
								continue;
							}
							if(it->second.context->type[idx] == newasm::datatypes::text)
							{
								std::string buf = newasm::hardware::randAccessMem.peek<std::string>(it->second.context->addr[idx]);
								temp.append("\"" + buf + "\"");
								contents.push_back(temp);
								continue;
							}
						}

						temp.clear();
						parsed_contents.clear();
						parsed_contents = "(";
						for(int i = 0; i < context_size; ++i)
						{
							temp = contents.at(i);
							parsed_contents.append(temp);
							if(i + 1 != context_size)
							{
								parsed_contents.append(",");
							}
							if(i + 1 == context_size)
							{
								parsed_contents.append(")");
							}
						}
						suf = parsed_contents;
					}
				}
				
                ///////////////////
                if(suf == newasm::header::constants::inv_reg_val)
                {
                    newasm::terminate(newasm::exit_codes::mem_overflow);//,wholeline);
                    return;
                }
				try{
					std::cout << "";
				}catch(std::exception& e)
				{
					std::cout << "Parser te zajebucnuo -> hihi :: " << e.what() << std::endl;
				}
                return;
            }

			template<bool _procNameParse>
			inline void parse(std::string& suf)
			{
				if constexpr(_procNameParse == true)
				{
					if(newasm::header::functions::parseNamespaceSegments(suf).first)
					{
						newasm::progwin::api::cout("Yes proc NMS -> " + suf);
						auto i = newasm::header::functions::parseNamespaceSegments(suf);
						std::string symbol_name = i.second.back();
						auto vec = i.second;
						vec.pop_back(); // namespace list
						
						suf = newasm::header::functions::mangleName(vec, symbol_name);
					}
					return;
				}
				return;
			}
        }
    }
}