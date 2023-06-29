//
// Created by david on 23-6-23.
//

#include "FindSimpleRepeat.h"
#include <fstream>

using namespace llvm;

void FindSimpleRepeat::analysisNextFunction() {
    std::vector<unsigned> IntegerMapping = file.getNextFunctionCode();

    codeCount += IntegerMapping.size();
    if (IntegerMapping.empty()) {
        return;
    }
    // 构造后缀树
    SuffixTree ST(IntegerMapping);

    // 从后缀树提取冗余信息
    RepeatedInfos ReptInfo(ST, 2);
    std::vector<RepeatedInfos::RepeatedSubstringByS *> NewRSList =
            ReptInfo.RSList;

    // 打印提取到的冗余信息
    for (RepeatedInfos::RepeatedSubstringByS *RSS: NewRSList) {
        RSS->print(IntegerMapping);
    }

    // 消除内部的冗余
    std::vector<unsigned> StrMap = ST.Str;
    RepeatedInfos::elimateInterOverlap(NewRSList, StrMap, 0);

    // 统计收益
    unsigned NewTotalBenefit = 0;
    std::for_each(NewRSList.begin(), NewRSList.end(),
                  [&NewTotalBenefit](RepeatedInfos::RepeatedSubstringByS *RS) {
                      NewTotalBenefit += RS->getPredictBenefit(0);
                      std::cout << "Test:" << RS->getPredictBenefit(0) << std::endl;
                  });

    std::cout << "Predict Benefit:" << NewTotalBenefit << std::endl;
    totalBenefit += NewTotalBenefit;

}

void FindSimpleRepeat::analysisAll() {
    while (!file.checkIsEOF()) {
        analysisNextFunction();
    }
}

void FindSimpleRepeat::writeToFile(const std::string &fileName) {
    try {
        std::ofstream out(fileName);
        if (!out.is_open()) {
            throw std::runtime_error("Unable to open file: " + fileName);
        }
        out << "Code Count : " << codeCount << std::endl << "Predict Benefits : " << totalBenefit << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

}
