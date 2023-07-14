//
// Created by david on 23-7-13.
//

#ifndef SUFFIX_TREE_BLOCK_H
#define SUFFIX_TREE_BLOCK_H

#include <vector>
#include<string>
#include <unordered_set>

using std::unordered_set;
using std::vector;
using std::string;

class Block {
public:
    string methodID;
    int blockID;
    unsigned long long int address;
    unordered_set<int> predecessor;
    unordered_set<int> successor;
    vector<unsigned> instructionBytecode;
    vector<string> instructions;
    vector<string> instructionAddress;


    Block(string methodID, int blockID, unsigned long long int address);

    void addSuccessor(int id);

    void addInstruction(const string &address, unsigned bytecode, const string &instruction);

    void addPredecessor(int id);

    Block(const Block &block);

    void print();

};

Block parseBlock(const string &methodID, vector<string> &input);

#endif //SUFFIX_TREE_BLOCK_H
