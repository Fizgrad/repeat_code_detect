//
// Created by David Chen on 6/22/23.
//
#include <regex>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>
#include <vector>
#include "ControlFlowGraph.h"
#include "Function.h"

#ifndef SUFFIX_TREE_READFILE_H
#define SUFFIX_TREE_READFILE_H


class File {
    std::ifstream file;

public:
    explicit File(const std::string &inputFile) : file(inputFile) {
        if (!file.is_open()) {
            std::cerr << "unable to open file " << std::endl;
        }
    }

    ~File() {
        if (file.is_open())
            file.close();
    }


    bool checkIsEOF() {
        if (file.is_open()) {
            if (!file.eof())
                return false;
        }
        return true;
    }

    std::pair<ControlFlowGraph,string> nextFunctionCode(string methodID);

    bool readNextLine(string &line);

};


#endif //SUFFIX_TREE_READFILE_H
