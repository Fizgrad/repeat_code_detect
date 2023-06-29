//
// Created by David Chen on 6/22/23.
//

#include "ReadFile.h"

using namespace llvm;

std::vector<unsigned> ReadFile::getNextFunctionCode() {
    std::vector<unsigned> res;
    while (!checkIsEOF()) {
        readNextLine();
        LineType type = parseCurLine();
        switch (type) {
            case LineType::Instruction:
                res.push_back(getInstructionMachineCode());
                break;
            case LineType::FunctionInfo:
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
    std::string matchedStr = curMatch[0].str().substr(12);
    return std::stoul(matchedStr, nullptr, 16);
}

std::string ReadFile::getInstructionString() {
    auto pos = curLine.find('\t');
    return curLine.substr(pos + 1);
}

unsigned ReadFile::getInstructionHashCode() {
    return 0;
}

void ReadFile::testReadCode() {
    int count = 0;
    getNextFunctionCode();
    while (!checkIsEOF()) {
        std::cout << "dex_method_" << getMethodID() << " count:\t";
        std::vector<unsigned> codes = getNextFunctionCode();
        std::cout << codes.size() << std::endl;
        count += codes.size();
    }
    std::cout << "All Code:\t" << count << std::endl;

}


