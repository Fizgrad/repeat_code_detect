//
// Created by david on 23-7-9.
//

#ifndef SUFFIX_TREE_FUNCTION_H
#define SUFFIX_TREE_FUNCTION_H

#include "Constants.h"
#include <capstone/capstone.h>
#include <capstone/arm64.h>
#include <vector>
#include <set>
#include <iostream>
#include "Block.h"

using std::set;
using std::string;
using std::vector;


class Function {
public:

    string methodID;
    unsigned long long int address;
    vector<Block> basicBlocks;
    bool hasBR = false;

    Function(string id, unsigned long long int address);

    Function(string id, unsigned long long int address, const vector<Block> &blocks, bool br);

    Function(const Function &function);

    vector<unsigned> getAllBytes();

    vector<unsigned> getAllHashCode();

    vector<Instruction> getAllInstructions();

};


#endif //SUFFIX_TREE_FUNCTION_H
