//
// Created by David Chen on 6/22/23.
//
#include <regex>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Function.h"

#ifndef SUFFIX_TREE_READFILE_H
#define SUFFIX_TREE_READFILE_H


namespace llvm {

    class File {
        std::ifstream file;
        vector<Function> functions;

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

        void parseFile(bool outputLog = false);

        bool readNextLine(string &line);

        vector<unsigned> getAllBasicBlockCode();

        unsigned long long getNumOfBlocks();

        unsigned long long getNumOfFunctions();

        unsigned long long getAllBasicBlockCodeCount();

        void print();
    };
}

#endif //SUFFIX_TREE_READFILE_H
