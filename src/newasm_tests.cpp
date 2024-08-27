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

namespace newasm
{
    namespace tests
    {
        void main()
        {
            newasm::header::functions::info("Starting tests...");
            auto* binaryTree = new newasm::containers::binary_tree<10>();
            binaryTree->set_at_left_child_of(1, 64);
            binaryTree->set_at_right_child_of(1, 44);
            binaryTree->set_at_parent_of(2, 66);

            using std::cout, std::endl;
            for(int i = 0; i < 10; i++)
            {
                cout << "bintree<" << i << "> = " << binaryTree->get_at(i) << endl;
            }

            delete binaryTree;
            newasm::header::functions::info("Tests finished.");
            return;
        }
    }
}