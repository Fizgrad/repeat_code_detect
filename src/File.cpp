//
// Created by David Chen on 6/22/23.
//

#include "../include/File.h"
#include "../include/Instruction.h"
#include <random>


const std::regex MethodPattern = std::regex("(dex_method_idx=)(\\d+)");
const std::regex InstructionPattern = std::regex("(0x[a-fA-F0-9]{8})(: )([a-fA-F0-9]{8})");
const std::regex CodeOffsetPattern = std::regex("(code_offset=)(0x[a-fA-F0-9]{8})");

std::string convertHexString(const std::string &input) {
    std::string result;
    for (int i = static_cast<int>(input.length() - 2); i >= 0; i -= 2) {
        std::string byteString = input.substr(i, 2);
        char byteChar = static_cast<char>(std::stoi(byteString, nullptr, 16));
        result.push_back(byteChar);
    }

    return result;
}

bool File::readNextLine(string &line) {
    if (!checkIsEOF()) {
        std::getline(this->file, line);
        return true;
    }
    return false;
}

Function File::nextFunction() {
    ControlFlowGraph controlFlowGraph;
    controlFlowGraph.methodID = methodID;
    string line;
    while (readNextLine(line)) {
        std::smatch match;

        if (std::regex_search(line, match, InstructionPattern)) {
            controlFlowGraph.bytecode.append(convertHexString(match[3]));
        } else if (std::regex_search(line, match, MethodPattern)) {
            if (controlFlowGraph.methodID.empty()) {
                controlFlowGraph.methodID = match[2];
            } else {
                methodID = match[2];
                return controlFlowGraph.buildCFG();
            }
        } else if (std::regex_search(line, match, CodeOffsetPattern)) {
            controlFlowGraph.methodCodeOffset = match[2];
        } else {
            continue;
        }
    }
    return controlFlowGraph.buildCFG();
}

void File::parseFile() {
    while (!checkIsEOF()) {
        program.funtions.emplace_back(nextFunction());
    }
}

