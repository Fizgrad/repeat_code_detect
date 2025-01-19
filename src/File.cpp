//
// Created by David Chen on 6/22/23.
//

#include "../include/File.h"
#include <random>
#include <cassert>

const std::regex MethodPattern = std::regex("^([0-9a-fA-F]+)\\s+<([^>]+)>:$");
const std::regex InstructionPattern = std::regex("(^\\s*[0-9a-fA-F]+):\\t([a-fA-F0-9]{8})\\s+(.*)$");
// const std::regex CodeOffsetPattern = std::regex("(code_offset=)(0x[a-fA-F0-9]{8})");

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
    controlFlowGraph.methodID = methodCodeOffset;
    string line;
    while (readNextLine(line)) {
        std::smatch match;

        if (std::regex_search(line, match, InstructionPattern)) {
            LOG(INFO) << "Instruction\t" << line << std::endl;
            controlFlowGraph.bytecode.append(convertHexString(match[2]));
        } else if (std::regex_search(line, match, MethodPattern)) {
            LOG(INFO) << "MethodID\t" << line << std::endl;
            if (controlFlowGraph.methodID.empty()) {
                controlFlowGraph.methodID = match[2];
                controlFlowGraph.methodCodeOffset = match[1];
            } else {
                methodID = match[2];
                methodCodeOffset = match[1];
                if (!controlFlowGraph.bytecode.empty())
                    return controlFlowGraph.buildCFG();
                else return {"-1", 0};
            }
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

