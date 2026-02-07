// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#include "sysext/shared_mem.cpp"
#include "sysext/linear_map.cpp"
#include "sysext/range.cpp"

namespace newasm
{
    namespace tests
    {
        void main()
        {
            return;
        }
        void main(int argc, char** argv)
        {
            return;
        }
    }
}

#if 0
namespace newasm
{
    namespace tests
    {
        void main()
        {
            return;

            ///ignore
            newasm::runtime::start_program(newasm::core::constants::progwin);
            for(int i = 0; i < 100; i++)
            {
                newasm::progwin::api::cout(newasm::header::col::magenta + std::string("HELLLLLLLLO ") + std::to_string(i) + "\n");
            }
            return;
        }
        void main_()
        {
            newasm::header::functions::info("Starting tests...");
            auto* binaryTree = new newasm::containers::binary_tree<10>();
            binaryTree->set_at_left_child_of(1, 64);
            binaryTree->set_at_right_child_of(1, 44);
            binaryTree->set_at_parent_of(2, 66);
            if(true == false) binaryTree->set_at_parent_of(9, 3);

            using std::cout, std::endl;
            for(int i = 0; i < 10; ++i)
            {
                cout << "bintree<" << i << "> = " << binaryTree->get_at(i) << endl;
            }

            delete binaryTree;
            binaryTree = nullptr;

            int testint = 23874;
            printf("number is %i",testint);

           /* std::string teststr = "dhnkjs ?? \"Hello World /h\"";
            newasm::tokenize(teststr);
            for(int i = 0; i < newasm::tokens.size(); ++i)
            {
                std::cout << i << '-'
                << newasm::tokens.at(i)->type<<'-'
                << newasm::tokens.at(i)->symbol<<'-'
                << newasm::tokens.at(i)->operator__
                << std::endl; 
            }
*/
            std::cout << std::endl; newasm::header::functions::info("Tests finished.");
            return;
			
        }
    }
}
#endif