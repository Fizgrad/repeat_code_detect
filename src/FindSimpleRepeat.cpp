//
// Created by david on 23-6-23.
//

#include "../include/FindSimpleRepeat.h"
#include <fstream>

using namespace llvm;


void FindSimpleRepeat::analysisAll() {


    std::vector<unsigned> AllCode = file.program.getAllBytes();

    codeCount = file.program.getCodeCount();

    vector<Instruction> instructions = file.program.getAllInstructions();


    if (AllCode.empty()) {
        return;
    }

    LOG(INFO) << "Construct suffix tree";
    // 构造后缀树
    SuffixTree ST(AllCode);

    LOG(INFO) << "Find repeat info from suffix tree";
    // 从后缀树提取冗余信息
    RepeatedInfos repeatedInfos(ST, 3);
    std::vector<RepeatedInfos::RepeatedSubstringByS *> NewRSList =
            repeatedInfos.RSList;


    LOG(INFO) << "Eliminate internal overlap";
    // 消除内部的冗余
    std::vector<unsigned> StrMap = ST.Str;
    RepeatedInfos::elimateInterOverlap(NewRSList, StrMap, 0);

    LOG(INFO) << "Print repeat info";

    // 打印提取到的冗余信息
    for (RepeatedInfos::RepeatedSubstringByS *RSS: NewRSList) {
        RSS->print(instructions);
    }


    LOG(INFO) << "Calculate benefits";
    // 统计收益
    std::for_each(NewRSList.begin(), NewRSList.end(),
                  [&](RepeatedInfos::RepeatedSubstringByS *RS) {
                      totalBenefit += RS->getPredictBenefit(0);
                      std::cout << "Test:" << RS->getPredictBenefit(0) << std::endl;
                  });

    std::cout << "Predict Benefit:" << totalBenefit << std::endl;
}


void FindSimpleRepeat::analysisHash() {


    std::vector<unsigned> AllCode = file.program.getAllHashCode();

    codeCount = file.program.getCodeCount();

    vector<Instruction> instructions = file.program.getAllInstructions();


    if (AllCode.empty()) {
        return;
    }

    LOG(INFO) << "Construct suffix tree";
    // 构造后缀树
    SuffixTree ST(AllCode);

    LOG(INFO) << "Find repeat info from suffix tree";
    // 从后缀树提取冗余信息
    RepeatedInfos repeatedInfos(ST, 3);
    std::vector<RepeatedInfos::RepeatedSubstringByS *> NewRSList =
            repeatedInfos.RSList;


    LOG(INFO) << "Eliminate internal overlap";
    // 消除内部的冗余
    std::vector<unsigned> StrMap = ST.Str;
    RepeatedInfos::elimateInterOverlapHash(NewRSList, StrMap, 0,instructions);

    LOG(INFO) << "Print repeat info";

    // 打印提取到的冗余信息
    for (RepeatedInfos::RepeatedSubstringByS *RSS: NewRSList) {
        RSS->print(instructions);
    }


    LOG(INFO) << "Calculate benefits";
    // 统计收益
    std::for_each(NewRSList.begin(), NewRSList.end(),
                  [&](RepeatedInfos::RepeatedSubstringByS *RS) {
                      totalBenefit += RS->getHashPredictBenefit(instructions);
                      std::cout << "Test:" << RS->getHashPredictBenefit(instructions) << std::endl;
                  });

    std::cout << "Predict Benefit:" << totalBenefit << std::endl;
}

void FindSimpleRepeat::writeToFile(const std::string &fileName) {
    try {
        std::ofstream out(fileName);
        if (!out.is_open()) {
            throw std::runtime_error("Unable to open file: " + fileName);
        }
        out << "Basic Block Count : " << this->file.program.getNumOfBlocks() << std::endl << "Function Count : "
            << this->file.program.getNumOfFunctions() << std::endl;
        out << "Code Count : " << codeCount << std::endl << "Predict Benefits : " << totalBenefit << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

}

void FindSimpleRepeat::printDisassembleCode(const string &fileName) {
    try {
        std::ofstream out(fileName);
        if (!out.is_open()) {
            throw std::runtime_error("Unable to open file: " + fileName);
        }
        file.program.printBlocks(out);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
