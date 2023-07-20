//
// Created by David Chen on 7/21/23.
//

#include "../include/Program.h"

vector<unsigned> Program::getAllBytes() {
    vector<unsigned> res;
    for (auto &f: funtions) {
        for (auto &b: f.basicBlocks) {
            for (auto &i: b.instructions) {
                res.push_back(i.bytecode);
            }
            res.emplace_back();
            res.emplace_back();
        }
    }
    return res;
}

vector<unsigned> Program::getAllHashCode() {
    vector<unsigned> res;
    for (auto &f: funtions) {
        for (auto &b: f.basicBlocks) {
            for (auto &i: b.instructions) {
                res.push_back(i.hashcode);
            }
            res.emplace_back();
            res.emplace_back();
        }
    }
    return res;
}

vector<Instruction> Program::getAllInstructions() {
    vector<Instruction> res;
    for (auto &f: funtions) {
        for (auto &b: f.basicBlocks) {
            for (auto &i: b.instructions) {
                res.push_back(i);
            }
            res.emplace_back();
            res.emplace_back();
        }
    }
    return res;
}

unsigned int Program::getCodeCount() {
    unsigned int res;
    for (auto &f: funtions) {
        for (auto &b: f.basicBlocks) {
            res += b.instructions.size();
        }
    }
    return res;
}

unsigned int Program::getNumOfBlocks() {
    unsigned int res;
    for (auto &f: funtions) {
        res += f.basicBlocks.size();
    }
    return res;
}

unsigned int Program::getNumOfFunctions() {
    return funtions.size();
}
