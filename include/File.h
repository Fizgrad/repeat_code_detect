//
// Created by David Chen on 6/22/23.
//

#ifndef SUFFIX_TREE_READFILE_H
#define SUFFIX_TREE_READFILE_H

#include <regex>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>
#include <vector>
#include "ControlFlowGraph.h"
#include "Function.h"
#include "Program.h"

class File {

public:
    std::ifstream file;
    Program program;

    explicit File(const std::string &inputFile) : file(inputFile) {
        if (!file.is_open()) {
            std::cerr << "unable to open file " << std::endl;
        }
    }

    ~File() {
        if (file.is_open())
            file.close();
    }

    void parseFile();

private:

    string methodID;

    bool checkIsEOF() {
        if (file.is_open()) {
            if (!file.eof())
                return false;
        }
        return true;
    }

    Function nextFunction();

    bool readNextLine(string &line);

};


#endif //SUFFIX_TREE_READFILE_H
