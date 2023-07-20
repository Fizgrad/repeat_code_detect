//
// Created by david on 23-7-13.
//

#ifndef SUFFIX_TREE_BLOCK_H
#define SUFFIX_TREE_BLOCK_H

#include <vector>
#include<string>
#include <unordered_set>
#include "Constant.h"
#include "Instruction.h"

using std::unordered_set;
using std::vector;
using std::string;


class Block {
public:
    string methodID;
    unsigned int blockID;
    unsigned long long int address;
    unordered_set<unsigned int> predecessors;
    unordered_set<unsigned int> successors;
    vector<Instruction> instructions;


    Block(string methodID, unsigned int blockID, unsigned long long int address);

    Block(const Block& block);

    void addSuccessor(unsigned int id);

    void addPredecessor(unsigned int id);

    void print();

};

#endif //SUFFIX_TREE_BLOCK_H
