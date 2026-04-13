// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#define _newasm_addnamespaces(name) if(newasm::nms::count != 0) for(int i = 0; i < newasm::nms::count; ++i)newasm::mem::data_attrib[name].namespaces.push_back(newasm::nms::stack.at(i));

namespace newasm
{
	namespace nms
	{
		std::vector<std::string> stack;
		int count = 0;
		
		FORCE_INLINE inline void ProcessNamespace(newasm::compiler::lineData& line)
		{
            /*
            NAMESPACE DESTRUCTOR
            */
            if(line.priInt == NewASM::Namespaces::Destruction)
            {
                if(stack.empty())
				{
					newasm::terminate(newasm::exit_codes::namespace_err);
					return;
				}
				if(stack.back() == line.priString)
				{
					stack.pop_back();
					newasm::nms::count --;
					return;
				}
				
				newasm::terminate(newasm::exit_codes::namespace_err);
				return;
            }
            /*
            NAMESPACE CONSTRUCTOR
            */
			
            stack.push_back(line.priString);
			newasm::nms::count ++;
            return;
        }
	}
}