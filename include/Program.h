//
// Created by David Chen on 7/21/23.
//

#ifndef SUFFIX_TREE_PROGRAM_H
#define SUFFIX_TREE_PROGRAM_H

#include "Function.h"
#include <ostream>
class Program {
public:

    vector<Function> funtions;

    vector<unsigned> getAllBytes();

    vector<unsigned> getAllHashCode();

    vector<Instruction> getAllInstructions();

    unsigned int getCodeCount();

    unsigned int getNumOfBlocks();

    unsigned int getNumOfFunctions();

    void printBlocks(std::ostream& out);
};


#endif //SUFFIX_TREE_PROGRAM_H
