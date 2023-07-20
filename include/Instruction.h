//
// Created by david on 23-7-20.
//

#include <capstone/capstone.h>
#include <capstone/arm64.h>


#ifndef SUFFIX_TREE_INSTRUCTION_H
#define SUFFIX_TREE_INSTRUCTION_H

#include <vector>
#include <string>

using std::vector;
using std::string;


class Instruction {
public:
    unsigned bytecode;
    unsigned long long int address;
    unsigned hashcode;
    int id;
    int operandsCount;
    vector<string> regs;

    void parse(csh handle, cs_insn *insn);

    Instruction() = default;

    explicit Instruction(csh handle, cs_insn *insn);
};

#endif //SUFFIX_TREE_INSTRUCTION_H
