//
// Created by david on 23-7-13.
//

#include "../include/Block.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>

Block::Block(std::string methodID, unsigned int blockID, unsigned long long address) : methodID(std::move(methodID)),
                                                                                       blockID(blockID),
                                                                                       address(address) {

}

void Block::addSuccessor(unsigned int id) {
    this->successors.insert(id);
}

void Block::addPredecessor(unsigned int id) {
    this->predecessors.insert(id);
}

void Block::print() {

    std::cout << "---------------------------" << std::endl;
    std::cout << std::dec << "ID:\t" << blockID << std::endl;

    std::cout << "Address:\t0x" << std::hex << std::setw(8) << std::setfill('0') << address << std::endl;
    for (const auto &predecessor: predecessors) {
        std::cout << "Predecessor:\t" << predecessor << std::endl;
    }

    for (const auto &successor: successors) {
        std::cout << "Successor:\t" << successor << std::endl;
    }

    std::cout << "Code:" << std::endl;
    for (auto &instruction: instructions) {
        std::cout << "\t" << std::hex
                  << instruction.bytecode << "\t"
                  << instruction.mnemonic << "\t"
                  << instruction.hashcode << std::dec << std::endl;
    }

    std::cout << "---------------------------" << std::endl;

}

Block::Block(const Block &block) : methodID(block.methodID), blockID(block.blockID), address(block.address),
                                   predecessors(block.predecessors),
                                   successors(block.successors), instructions(block.instructions) {

}
