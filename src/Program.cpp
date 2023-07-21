//
// Created by David Chen on 7/21/23.
//

#include "../include/Program.h"
#include <random>
#include <glog/logging.h>

vector<unsigned> Program::getAllBytes() {

    // 使用随机设备作为种子
    std::random_device rd;
    // 使用 Mersenne Twister 引擎
    std::mt19937 gen(rd());
    // 定义生成的范围
    std::uniform_int_distribution<unsigned int> dis(0, std::numeric_limits<unsigned int>::max());

    LOG(INFO) << "Program Get All bytes";

    vector<unsigned> res;
    for (auto &f: funtions) {
        for (auto &b: f.basicBlocks) {
            for (auto &i: b.instructions) {
                res.push_back(i.bytecode);
            }
            res.emplace_back(dis(gen));
            res.emplace_back(dis(gen));
        }
    }
    return res;
}

vector<unsigned> Program::getAllHashCode() {
    std::random_device rd;
    // 使用 Mersenne Twister 引擎
    std::mt19937 gen(rd());
    // 定义生成的范围
    std::uniform_int_distribution<unsigned int> dis(0, std::numeric_limits<unsigned int>::max());

    LOG(INFO) << "Program Get All Hashcode";

    vector<unsigned> res;
    for (auto &f: funtions) {
        for (auto &b: f.basicBlocks) {
            for (auto &i: b.instructions) {
                res.push_back(i.hashcode);
            }
            res.emplace_back(dis(gen));
            res.emplace_back(dis(gen));
        }
    }
    return res;
}

vector<Instruction> Program::getAllInstructions() {

    LOG(INFO) << "Program Get All Instructions";

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

    LOG(INFO) << "Program Get Code Count";

    unsigned int res;
    for (auto &f: funtions) {
        for (auto &b: f.basicBlocks) {
            res += b.instructions.size();
        }
    }
    return res;
}

unsigned int Program::getNumOfBlocks() {

    LOG(INFO) << "Program Get Block Count";

    unsigned int res;
    for (auto &f: funtions) {
        res += f.basicBlocks.size();
    }
    return res;
}

unsigned int Program::getNumOfFunctions() {

    LOG(INFO) << "Program Get Function Count";

    return funtions.size();
}
