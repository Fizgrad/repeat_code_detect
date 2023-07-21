//
// Created by David Chen on 6/22/23.
//

#include "../include/File.h"
#include <random>
#include <cassert>

const std::regex MethodPattern = std::regex("(dex_method_idx=)(\\d+)");
const std::regex InstructionPattern = std::regex("(0x[a-fA-F0-9]{8})(: )([a-fA-F0-9]{8})");
const std::regex CodeOffsetPattern = std::regex("(code_offset=)(0x[a-fA-F0-9]{8})");

std::string convertHexString(const std::string &input) {
    assert(input.size() == 8);
    return {static_cast<char>(std::stoul(input.substr(6, 2), nullptr, 16)),
            static_cast<char>(std::stoul(input.substr(4, 2), nullptr, 16)),
            static_cast<char>(std::stoul(input.substr(2, 2), nullptr, 16)),
            static_cast<char>(std::stoul(input.substr(0, 2), nullptr, 16))};
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
//            std::cout << "Instruction\t" << line << std::endl;
            controlFlowGraph.bytecode.append(convertHexString(match[3]));
        } else if (std::regex_search(line, match, MethodPattern)) {
//            std::cout << "MethodID\t" << line << std::endl;
            if (controlFlowGraph.methodID.empty()) {
                controlFlowGraph.methodID = match[2];
            } else {
                methodID = match[2];
                if (!controlFlowGraph.bytecode.empty())
                    return controlFlowGraph.buildCFG();
                else return {"-1", 0};
            }
        } else if (std::regex_search(line, match, CodeOffsetPattern)) {
            controlFlowGraph.methodCodeOffset = match[2];
        } else {
            continue;
        }
    }
    if (!controlFlowGraph.bytecode.empty())
        return controlFlowGraph.buildCFG();
    else {
        return {"-1", 0};
    }
}

void File::parseFile() {
    while (!checkIsEOF()) {
        Function function = nextFunction();
        if (function.methodID != "-1")
            program.funtions.emplace_back(function);
    }
}

