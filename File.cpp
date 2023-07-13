//
// Created by David Chen on 6/22/23.
//

#include "File.h"
#include <random>

using namespace llvm;

const std::regex methodPattern = std::regex("dex_method_idx=(\\d+)");

const std::regex delimiterPattern = std::regex("---------------------------");


bool File::readNextLine(string &line) {
    if (!checkIsEOF()) {
        std::getline(this->file, line);
        return true;
    }
    return false;
}

vector<unsigned> File::getAllBasicBlockCode() {
    std::random_device rd;  // 用于生成种子
    std::mt19937 engine(rd());  // 随机数引擎
    std::uniform_int_distribution<unsigned> dist(0, std::numeric_limits<unsigned>::max());  // 范围限定为 unsigned 的最大值


    if (this->functions.empty()) {
        this->parseFile(1);
    }
    vector<unsigned int> res;
    for (auto &f: this->functions) {
        for (auto &b: f.basicBlocks) {
            for (auto &i: b.instructionBytecode) {
                res.push_back(i);
            }

            // -1 代表后面的不是一条命令，只是用来分割的随机数
            res.push_back(-1);
            res.push_back(dist(engine));  // 生成随机数
        }
    }
    return res;
}

void File::parseFile(bool outputLog) {
    vector<string> blockString;
    Function function("-1", 0);
    while (!checkIsEOF()) {
        string line;
        readNextLine(line);
        std::smatch match;
        if (std::regex_search(line, match, methodPattern)) {
            if (function.methodID != "-1") {
                this->functions.push_back(function);
            }
            std::string id = match[1];
            function = Function(id, 0);
            std::cout << "Function " << id << " start" << std::endl;
        }
        if (std::regex_search(line, match, delimiterPattern)) {
            while (!checkIsEOF()) {
                readNextLine(line);
                blockString.emplace_back(line);
                if (std::regex_search(line, match, delimiterPattern)) {
                    function.basicBlocks.push_back(parseBlock(function.methodID, blockString));
                    blockString.clear();
                    if (outputLog) {
                        std::cout << "Basic Block\t" << std::hex << function.basicBlocks.back().address
                                  << "\tCode Count\t"
                                  << std::dec << function.basicBlocks.back().instructionBytecode.size() << std::endl;
                    }
                    break;
                }
            }
        }
    }
    if (!function.basicBlocks.empty()) {
        this->functions.push_back(function);
    }
}

unsigned long long File::getNumOfBlocks() {
    if (this->functions.empty()) {
        this->parseFile(1);
    }
    unsigned long long res = 0;
    for (auto &f: this->functions) {
        res += f.basicBlocks.size();
    }
    return res;
}

unsigned long long File::getNumOfFunctions() {
    return this->functions.size();
}

unsigned long long File::getAllBasicBlockCodeCount() {
    if (this->functions.empty()) {
        this->parseFile(1);
    }
    unsigned long long res = 0;
    for (auto &f: this->functions) {
        for (auto &b: f.basicBlocks) {
            res += b.instructionBytecode.size();
        }
    }
    return res;
}

void File::print() {
    for (auto &i: functions) {
        std::cout << "dex_method_idx=" << i.methodID << std::endl;
        for (auto &b: i.basicBlocks) {
            b.print();
        }
    }
}


