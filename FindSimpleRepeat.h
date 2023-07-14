//
// Created by david on 23-6-23.
//

#include <iostream>
#include "SuffixTree.h"
#include "File.h"
#include "Function.h"
#include "SuffixTreeRepeatedInfos.h"
#include <iomanip>

#ifndef SUFFIX_TREE_FINDSIMPLEREPEAT_H
#define SUFFIX_TREE_FINDSIMPLEREPEAT_H

using namespace llvm;

class FindSimpleRepeat {
    File file;
    unsigned long long int codeCount = 0;
    unsigned long long int totalBenefit = 0;

public:
    explicit FindSimpleRepeat(const std::string &file) : file(file) {
    }


    void analysisAll();

    void writeToFile(const std::string & fileName);
};


#endif //SUFFIX_TREE_FINDSIMPLEREPEAT_H