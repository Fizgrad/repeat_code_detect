//
// Created by David Chen on 6/22/23.
//

#include "ReadFile.h"

using namespace llvm;

std::vector<unsigned> ReadFile::getNextFuctionCode() {
    std::vector<unsigned> res;
    while(!checkIsEOF()){
        readNextLine();
        LineType type = parseCurLine();
        switch (type) {
            case LineType::Instruction:
                res.push_back(getInstructionMachineCode());
                break;
            case LineType::FunctionInfo:
                std::cout<<getMethodID()<<std::endl;
                return res;
            default:
                break;
        }
    }
    return res;
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

std::string ReadFile::getMethodID() {
    std::string matchedStr = curMatch[1].str();
    return std::move(matchedStr);
}

unsigned ReadFile::getInstructionMachineCode() {
    std::string matchedStr = curMatch[0].str().substr(2);
    return std::stoul(matchedStr, nullptr, 16);
}

std::string ReadFile::getInstructionString() {
    auto pos = curLine.find('\t');
    return curLine.substr(pos+1);
}


unsigned ReadFile::getInstructionHashCode() {



    return 0;
}
