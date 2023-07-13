//
// Created by david on 23-7-13.
//

#include "Block.h"
#include <regex>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>

const std::regex IDPattern = std::regex("ID:\t(\\d+)");
const std::regex AddressPattern = std::regex("Address:\t(0x[a-fA-F0-9]{8})");
const std::regex PredecessorPattern = std::regex("Predecessor:\t(\\d+)");
const std::regex SuccessorPattern = std::regex("Successor:\t(\\d+)");
const std::regex CodePattern = std::regex("\t(0x[a-fA-F0-9]{8}):\t([a-fA-F0-9]{8})\t(.*)");


Block::Block(std::string methodID, int blockID, unsigned long long address) : methodID(std::move(methodID)),
                                                                              blockID(blockID),
                                                                              address(address) {

}

void Block::addSuccessor(int id) {
    this->successor.insert(id);
}

void Block::addPredecessor(int id) {
    this->predecessor.insert(id);
}

void Block::addInstruction(unsigned int bytecode, const string &instruction) {
    this->instructionBytecode.emplace_back(bytecode);
    this->instructions.emplace_back(instruction);
}

Block::Block(const Block &block) : methodID(block.methodID), blockID(block.blockID), address(block.address) {
    for (auto i: block.predecessor) this->predecessor.insert(i);
    for (auto i: block.successor) this->successor.insert(i);
    for (auto i: block.instructionBytecode) this->instructionBytecode.push_back(i);
    for (auto &i: block.instructions) this->instructions.push_back(i);

}

void Block::print() {

    std::cout << "---------------------------" << std::endl;
    std::cout <<std::dec<< "ID:\t" << blockID << std::endl;

    std::cout << "Address:\t0x" << std::hex << std::setw(8) << std::setfill('0') << address << std::endl;
    for (const auto& predecessor : predecessor) {
        std::cout << "Predecessor:\t" << predecessor << std::endl;
    }

    for (const auto& successor : successor) {
        std::cout << "Successor:\t" << successor << std::endl;
    }

    std::cout << "Code:" << std::endl;
    for (int i = 0; i < instructionBytecode.size(); ++i) {
        std::cout <<"\t" << instructionBytecode[i] << "\t" << instructions[i] << std::endl;
    }

    std::cout << "---------------------------" << std::endl;

};


Block parseBlock(const string &methodID, vector<string> &inputs) {
    Block res(methodID, 0, 0);

    for (auto &input: inputs) {
        std::smatch match;
        // 匹配和处理 ID
        if (std::regex_search(input, match, IDPattern)) {
            std::string id = match[1];
            res.blockID = std::stoi(id);
        }

        // 匹配和处理 Address
        else if (std::regex_search(input, match, AddressPattern)) {
            std::string address = match[1];
            res.address = std::stoull(address.substr(2), nullptr, 16);
        }
        // 匹配和处理 Predecessor
        else if (std::regex_search(input, match, PredecessorPattern)) {
            std::string predecessor = match[1];
            res.addPredecessor(std::stoi(predecessor));

        }

        // 匹配和处理 Successor
        else if (std::regex_search(input, match, SuccessorPattern)) {
            std::string successor = match[1];
            res.addSuccessor(std::stoi(successor));
        }

        // 匹配和处理 Code
        else if (std::regex_search(input, match, CodePattern)) {
            std::string codeAddress = match[1];
            std::string codeValue = match[2];
            std::string codeString = match[3];
            res.addInstruction(std::stoul(codeValue, nullptr, 16), codeString);
        }

    }

    return res;
}
