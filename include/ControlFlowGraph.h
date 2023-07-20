//
// Created by david on 23-7-20.
//

#ifndef SUFFIX_TREE_CONTROLFLOWGRAPH_H
#define SUFFIX_TREE_CONTROLFLOWGRAPH_H


#include <capstone/capstone.h>
#include <capstone/arm64.h>
#include "Function.h"
#include <iostream>
#include <vector>
#include <set>
#include "Constant.h"

using std::string;
using std::vector;

class ControlFlowGraph {
public:
    string code;
    string methodID;
    string methodCodeOffset;
    vector<Instruction> instructions;
    vector<Block> blocks;
    vector<int> labels;

    bool hashBR = false;

    Function buildCFG();

};


#endif //SUFFIX_TREE_CONTROLFLOWGRAPH_H
