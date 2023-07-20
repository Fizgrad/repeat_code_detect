//
// Created by david on 23-7-9.
//

#include "../include/Function.h"

#include <random>
#include <utility>

Function::Function(string id, unsigned long long int address) : methodID(std::move(id)), address(address) {

}

Function::Function(const Function &function) : methodID(function.methodID), address(function.address),
                                               basicBlocks(function.basicBlocks) {

}

Function::Function(string id, unsigned long long int address, const vector<Block> &blocks, bool br) : methodID(
        std::move(
                id)), address(address), basicBlocks(blocks), hasBR(br) {

}

vector<unsigned> Function::getAllBytes() {
    // 使用随机设备作为种子
    std::random_device rd;
    // 使用 Mersenne Twister 引擎
    std::mt19937 gen(rd());
    // 定义生成的范围
    std::uniform_int_distribution<unsigned int> dis(0, std::numeric_limits<unsigned int>::max());


    vector<unsigned int> res;
    for (auto &b: basicBlocks) {
        for (auto &i: b.instructions) {
            res.push_back(i.bytecode);
        }
        res.push_back(dis(gen));
        res.push_back(dis(gen));
    }
    return res;
}

vector<unsigned> Function::getAllHashCode() {
    // 使用随机设备作为种子
    std::random_device rd;
    // 使用 Mersenne Twister 引擎
    std::mt19937 gen(rd());
    // 定义生成的范围
    std::uniform_int_distribution<unsigned int> dis(0, std::numeric_limits<unsigned int>::max());


    vector<unsigned int> res;
    for (auto &b: basicBlocks) {
        for (auto &i: b.instructions) {
            res.push_back(i.hashcode);
        }
        res.push_back(dis(gen));
        res.push_back(dis(gen));
    }
    return res;
}

vector<Instruction> Function::getAllInstructions() {
    vector<Instruction> res;
    for (auto &b: basicBlocks) {
        for (auto &i: b.instructions) {
            res.push_back(i);
        }
        res.emplace_back();
        res.emplace_back();
    }
    return res;
}



