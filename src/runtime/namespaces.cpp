// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#define _newasm_addnamespaces(name) if(newasm::nms::count != 0) for(int i = 0; i < newasm::nms::count; ++i)newasm::mem::data_attrib[name].namespaces.push_back(newasm::nms::stack.at(i));

namespace newasm
{
	namespace nms
	{
		std::vector<std::string> stack;
		int count = 0;
		
		void process_nms(std::string text)
		{
			newasm::progwin::api::cout("Namespace -> " + text);
			std::string namespace_id;
            /*
            NAMESPACE DESTRUCTOR
            */
            if(text.at(0) == '!')
            {
				namespace_id = newasm::header::functions::trim(text.substr(1));
                if(stack.empty())
				{
					newasm::progwin::api::cout("Namespace -> " + text + " :: empty stack");
					newasm::terminate(newasm::exit_codes::namespace_err);
					return;
				}
				if(stack.back() == namespace_id)
				{
					stack.pop_back();
					newasm::nms::count --;
					return;
				}
				newasm::progwin::api::cout("Namespace -> " + text + " :: unknown namespace");
				newasm::terminate(newasm::exit_codes::namespace_err);
				return;
            }
            /*
            NAMESPACE CONSTRUCTOR
            */
			if(!newasm::header::functions::isalphanum(text))
			{
				newasm::progwin::api::cout("Namespace -> " + text + " :: not alphanumeric");
				newasm::terminate(newasm::exit_codes::namespace_err);
				return;
			}
			namespace_id = text;
            stack.push_back(namespace_id);
			newasm::nms::count ++;
            return;
        }
	}
}