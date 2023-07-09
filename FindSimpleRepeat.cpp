//
// Created by david on 23-6-23.
//

#include "FindSimpleRepeat.h"
#include <fstream>

using namespace llvm;

//void FindSimpleRepeat::analysisNextFunction() {
//
//
//}

void FindSimpleRepeat::analysisAll() {
    std::vector<unsigned> AllCode;

    while (!file.checkIsEOF()) {
        Function function = file.getNextFunction();
        std::vector<unsigned>&  IntegerMapping = function.codeHash;
        if (IntegerMapping.empty())
            continue;
        for (auto i: IntegerMapping)
            AllCode.push_back(i);
        AllCode.push_back(static_cast<unsigned >(-1));
    }

    codeCount = AllCode.size();

    if (AllCode.empty()) {
        return;
    }
    // 构造后缀树
    SuffixTree ST(AllCode);

    // 从后缀树提取冗余信息
    RepeatedInfos ReptInfo(ST, 2);
    std::vector<RepeatedInfos::RepeatedSubstringByS *> NewRSList =
            ReptInfo.RSList;

    // 打印提取到的冗余信息
    for (RepeatedInfos::RepeatedSubstringByS *RSS: NewRSList) {
        RSS->print(AllCode);
    }

    // 消除内部的冗余
    std::vector<unsigned> StrMap = ST.Str;
    RepeatedInfos::elimateInterOverlap(NewRSList, StrMap, 0);

    // 统计收益
    std::for_each(NewRSList.begin(), NewRSList.end(),
                  [&](RepeatedInfos::RepeatedSubstringByS *RS) {
                      totalBenefit += RS->getPredictBenefit(0);
                      std::cout << "Test:" << RS->getPredictBenefit(0) << std::endl;
                  });

    std::cout << "Predict Benefit:" << totalBenefit << std::endl;
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
