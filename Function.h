//
// Created by david on 23-7-9.
//

#ifndef SUFFIX_TREE_FUNCTION_H
#define SUFFIX_TREE_FUNCTION_H

#include <vector>
#include <iostream>

using std::string;
using std::vector;


class Function {
public:

    string methodID;
    int address;
    vector<unsigned> codeHash;

};


#endif //SUFFIX_TREE_FUNCTION_H
