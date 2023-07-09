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

enum LineType {
    Instruction,
    FunctionInfo,
    Unknown,
};


namespace llvm {

    const std::regex methodPattern = std::regex("dex_method_idx=(\\d+)");

    const std::regex instPattern = std::regex("0x[a-fA-F0-9]{8}: [a-fA-F0-9]{8}");

    class ReadFile {
        std::ifstream file;
        bool open_flag = false;
        std::string methodId;
        std::smatch curMatch;
        std::string curLine;

    public:
        explicit ReadFile(const std::string &inputFile) : file(inputFile) {
            open_flag = file.is_open();
            if (!open_flag) {
                std::cerr << "unable to open file " << std::endl;
            }
        }

        ~ReadFile() {
            if (open_flag)
                file.close();
        }


        bool checkIsEOF() {
            if (open_flag) {
                if (!file.eof())
                    return false;
            }
            return true;
        }

        Function getNextFunction();

        LineType parseCurLine();

        bool readNextLine();

        unsigned getInstructionMachineCode();

        unsigned getInstructionHashCode();

        std::string getInstructionString();

        void testReadCode();
    };
}

#endif //SUFFIX_TREE_READFILE_H
