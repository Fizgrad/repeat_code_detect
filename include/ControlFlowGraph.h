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
#include <map>
#include <set>
#include "Constants.h"

using std::string;
using std::vector;
using std::set;
using std::map;

class ControlFlowGraph {
public:
    string bytecode;

    uint8_t *code = nullptr;

    string methodID;
    string methodCodeOffset;
    unsigned long long int address = 0;
    vector<Instruction> instructions;
    vector<Block> blocks;
    set<unsigned long long int> labels;
    map<unsigned long long int, unsigned long long int> address2Target;
    map<unsigned long long int, bool> isConditionalJumpOrBranch;
    map<unsigned long long int, unsigned int> address2BlockID;

    bool hashBR = false;

    ~ControlFlowGraph() {
        delete[] code;
    }

    ControlFlowGraph() = default;

    ControlFlowGraph(const ControlFlowGraph &controlFlowGraph) = delete;


    Function buildCFG();

private:

    void parseCode();

    void parseInstruction();

    void buildBlocks();

    void buildEdges();

    void addLabel(unsigned long long int address);
};


#endif //SUFFIX_TREE_CONTROLFLOWGRAPH_H
