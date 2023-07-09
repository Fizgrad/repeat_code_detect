//
// Created by David Chen on 6/22/23.
//

#include "ReadFile.h"

using namespace llvm;

Function ReadFile::getNextFunction() {
    Function function;
    function.methodID = methodId;
    std::vector<unsigned> &res = function.codeHash;
    while (!checkIsEOF()) {
        readNextLine();
        LineType type = parseCurLine();
        switch (type) {
            case LineType::Instruction:
                res.push_back(getInstructionMachineCode());
                break;
            case LineType::FunctionInfo:
                this->methodId =  curMatch[1].str();;
                return function;
            default:
                break;
        }
    }
    return function;
}

LineType ReadFile::parseCurLine() {
    if (std::regex_search(curLine, curMatch, methodPattern)) {
        return LineType::FunctionInfo;
    } else {
        if (std::regex_search(curLine, curMatch, instPattern)) {
            return LineType::Instruction;
        }
    }
    return LineType::Unknown;
}

bool ReadFile::readNextLine() {
    if (!checkIsEOF()) {
        std::getline(this->file, this->curLine);
        return true;
    }
    return false;
}


unsigned ReadFile::getInstructionMachineCode() {
    std::string matchedStr = curMatch[0].str().substr(12);
    return std::stoul(matchedStr, nullptr, 16);
}

std::string ReadFile::getInstructionString() {
    auto pos = curLine.find('\t');
    return curLine.substr(pos + 1);
}




