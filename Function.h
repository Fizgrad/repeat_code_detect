//
// Created by david on 23-7-9.
//

#ifndef SUFFIX_TREE_FUNCTION_H
#define SUFFIX_TREE_FUNCTION_H

#include <vector>
#include <iostream>
#include "Block.h"

using std::string;
using std::vector;


class Function {
public:

    string methodID;
    unsigned long long int address;
    vector<Block> basicBlocks;

    explicit Function(string id, unsigned long long int address);

    Function(const Function &function);


};


#endif //SUFFIX_TREE_FUNCTION_H
