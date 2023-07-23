//
// Created by david on 23-7-13.
//

#include "../include/Block.h"
#include <iomanip>
#include <ostream>
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

void Block::print(std::ostream &out) {

    out << "---------------------------" << std::endl;
    out << std::dec << "ID:\t" << blockID << std::endl;

    out << "Address:\t0x" << std::hex << std::setw(8) << std::setfill('0') << address << std::dec << std::endl;
    for (const auto &predecessor: predecessors) {
        out << "Predecessor:\t" << predecessor << std::endl;
    }

    for (const auto &successor: successors) {
        out << "Successor:\t" << successor << std::endl;
    }

    out << "Code:" << std::endl;
    for (auto &instruction: instructions) {
        out << "\t0x" << std::hex << std::setw(8) << std::setfill('0')
            << instruction.address << "\t"
            << instruction.bytecode << "\t"
            << instruction.hashcode << "\t"
            << instruction.mnemonic << "\t"
            << instruction.op_str << "\t"
            << std::dec << std::endl;
    }

    out << "---------------------------" << std::endl;

}

Block::Block(const Block &block) : methodID(block.methodID), blockID(block.blockID), address(block.address),
                                   predecessors(block.predecessors),
                                   successors(block.successors), instructions(block.instructions) {

}
