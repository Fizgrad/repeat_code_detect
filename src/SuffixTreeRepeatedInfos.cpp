//===- llvm/Support/SuffixTreeRepeatedInfos.cpp - Implement Suffix Tree Repeated
// Infos------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the Suffix Tree Repeated Infos class.
//
//===----------------------------------------------------------------------===//
#include "../include/SuffixTreeRepeatedInfos.h"
#include <iostream>
#include <vector>

using namespace llvm;

#pragma region implementations of SuffixTreeRepeatedInfos


unsigned RepeatedInfos::RepeatedSubstringByS::getPredictBenefit(
        unsigned CreateFuncOverHead) const {
    if (StartIndices.empty() || StartIndices.size() < 2 || Length < 2) {
        return 0;
    }
    // Original:  Length*StartIndices.size()
    unsigned Original = Length * StartIndices.size();
    // Abstract:  Length+1+StartIndices.size()+CreateFuncOverHead;
    unsigned Abstract = Length + 1 + StartIndices.size() + CreateFuncOverHead;

    if (Original <= Abstract)
        return 0;
    // Benefit: Original - Abstract
    return Original - Abstract;
}

void RepeatedInfos::RepeatedSubstringByS::print(
        const std::vector<unsigned> &Str) {
    unsigned StringLen = Length;
    std::vector<unsigned> RepeatStr;
    RepeatStr.insert(RepeatStr.end(), std::next(Str.begin(), StartIndices[0]),
                     std::next(Str.begin(), StartIndices[0] + StringLen));
    std::cout << "Len:\t" << Length << "\tRS:\t\n\n";
    for (auto Via: RepeatStr) {
        std::cout << std::hex << Via << " " << std::dec;
    }
    std::cout << "\nIndices:\t";
    for (unsigned SI: StartIndices) {
        std::cout << SI << " ";
    }
    std::cout << std::endl;
}

bool RepeatedInfos::RepeatedSubstringByS::operator<(
        RepeatedSubstringByS RS) const {
    if (Length < RS.Length) {
        return true;
    } else if (Length > RS.Length) {
        return false;
    }
    for (unsigned int SI: RS.StartIndices) {
        if (std::find(StartIndices.begin(), StartIndices.end(), SI) !=
            StartIndices.end()) {
            return false;
        }
    }
    if (StartIndices[0] > RS.StartIndices[0]) {
        return false;
    }
    return true;
}

bool RepeatedInfos::RepeatedSubstringByS::operator==(
        RepeatedSubstringByS RS) const {
    if (Length != RS.Length) {
        return false;
    }
    for (unsigned int SI: RS.StartIndices) {
        if (std::find(StartIndices.begin(), StartIndices.end(), SI) !=
            StartIndices.end()) {
            return true;
        }
    }
    return false;
}

void RepeatedInfos::RepeatedSubstringByS::print(const std::vector<Instruction> &instructions) {
    unsigned StringLen = Length;

    std::cout << "Len:\t" << Length << "\tRS:\t\n\n";
    for (int i = StartIndices[0]; i < StartIndices[0] + StringLen; ++i) {
        instructions[i].outputInstructionDetail(std::cout);
    }
    std::cout << std::dec << "\nRepeat times:\t" << StartIndices.size() << "\n";
    std::cout << "Indices:\t";
    for (unsigned SI: StartIndices) {
        std::cout << "0x" << std::hex << instructions[SI].address << std::dec << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

bool RepeatedInfos::isOverlap(std::vector<unsigned> &SuffixIndices,
                              unsigned Length) {
    std::sort(SuffixIndices.begin(), SuffixIndices.end());
    for (size_t I = 0; I < SuffixIndices.size() - 1; I++) {
        if ((SuffixIndices[I + 1] - SuffixIndices[I]) < Length) {
            return true;
        }
    }
    return false;
}

RepeatedInfos::RepeatedSubstringByS *
RepeatedInfos::processSelfing(RepeatedSubstringByS *RS) {
    if (isOverlap(RS->StartIndices, RS->Length)) {
        // isOverlap function has sort the Start Indices;
        std::vector<unsigned> NewStartIndices;
        NewStartIndices.push_back(RS->StartIndices[0]);
        size_t J = 0;
        for (size_t I = 1; I < RS->StartIndices.size(); I++) {
            if ((RS->StartIndices[I] - RS->StartIndices[J]) >= RS->Length) {
                NewStartIndices.push_back(RS->StartIndices[I]);
                J = I;
            }
        }
        RepeatedSubstringByS *NewRS =
                new RepeatedSubstringByS(RS->Length, NewStartIndices, RS->ReleatedNode);
        NewRS->FromRoot = true;
        NewRS->OriginalRS = RS;
        return NewRS;
    }
    return RS;
}

void RepeatedInfos::printRepeatedInfoOfNode(
        SuffixTreeNode &Node, std::vector<unsigned> &SuffixIndicesOfCurrent) {
    // std::cout<<"\n"<<Node.
    std::cout << "\n###### Start Node ######\n"
              << Node.StartIdx << "\t" << *Node.EndIdx
              << "\tisLeaf:" << Node.isLeaf() << "\n";
    for (auto Suffix: SuffixIndicesOfCurrent) {
        std::cout << Suffix << "\t";
    }
    std::cout << "\n###### End Node ######\n";
}

bool RepeatedInfos::isAllOverlap(std::vector<unsigned> &SuffixIndices,
                                 unsigned Length) {
    std::sort(SuffixIndices.begin(), SuffixIndices.end());
    // for (size_t I = 0; I < SuffixIndices.size(); I++) {

    // }
    if ((SuffixIndices[SuffixIndices.size() - 1] - SuffixIndices[0]) >= Length) {
        return false;
    }
    return true;
}

void RepeatedInfos::setSubRSRelation(RepeatedSubstringByS &CurrentNode,
                                     size_t Size) {
    unsigned Offset = CurrentNode.Length - Size;
    std::vector<unsigned> NewStartIndices;
    for (unsigned StartIdx: CurrentNode.StartIndices) {
        NewStartIndices.push_back(StartIdx + Offset);
    }
    RepeatedSubstringByS SubRS(Size, NewStartIndices, nullptr);
    SubRS.FromRoot = false;
    SubRS.SuperLinkRSs.push_back(&CurrentNode);
    // auto InsertResult = RSSet.insert(SubRS);
    // RepeatedSubstringByS *It = (RepeatedSubstringByS *)&*InsertResult.first;
    // // (RepeatedSubstringByS *)&*std::find(RSSet.begin(), RSSet.end(), SubRS);
    // if (!InsertResult.second) {
    //   // if this node fully include by another
    //   //TODO
    //   // if (It->FromRoot) {
    //   //   if (It->MaxSRSSize <= SubRS.MaxSRSSize)
    //   //     NoSenseRSList.push_back(&*It);
    //   // } else if (It->MaxSRSSize < SubRS.MaxSRSSize) {
    //   //   It->MaxSRSSize = SubRS.MaxSRSSize;
    //   // }

    //   // cause this node will finally replaced by from root RS, no need to
    //   update
    //   // Start Indices; Only need update super RSs;
    //   It->SuperLinkRSs.push_back(&CurrentNode);
    //   // return;
    // }
    // RepeatedSubstringByS *Inserted =
    //   (RepeatedSubstringByS *)&*std::find(RSSet.begin(), RSSet.end(), SubRS);
    // CurrentNode.SubLinkRSs.push_back(It);
}

void RepeatedInfos::findMoreNoSenseRSs(SuffixTreeNode &CurrentNode) {
    //如果要重新使用这一方法，需要认真检查正确性
    if (CurrentNode.isLeaf()) {
        return;
    }

    for (auto ChildPair: CurrentNode.Children) {
        findMoreNoSenseRSs(*ChildPair.second);
    }

    if (CurrentNode.isRoot() || CurrentNode.ConcatLen < 2 ||
        CurrentNode.Link == nullptr)
        return;

    RepeatedSubstringByS *RS0 = RSMap[&CurrentNode];
    if (!RS0) {
        return;
    }
    RepeatedSubstringByS *Via1 = RS0;
    bool NeedRelease1 = false;
    if (isOverlap(Via1->StartIndices, Via1->Length)) {
        // return;
        Via1 = processSelfing(Via1);
        NeedRelease1 = true;
    }

    RepeatedSubstringByS *TmpRS2 = RSMap[CurrentNode.Link];
    if (!TmpRS2)
        return;
    RepeatedSubstringByS *NewVia2;
    bool NeedRelease2 = false;
    if (isOverlap(TmpRS2->StartIndices, TmpRS2->Length)) {
        // return;
        NewVia2 = processSelfing(TmpRS2);
        NeedRelease2 = true;
    }
    if (NeedRelease2) {
        if (NewVia2->StartIndices.size() <= Via1->StartIndices.size()) {
            NoSenseRSList[CurrentNode.Link] = TmpRS2;
        }
        delete NewVia2;
    } else if (TmpRS2->StartIndices.size() <= Via1->StartIndices.size()) {
        NoSenseRSList[CurrentNode.Link] = TmpRS2;
    }
    if (NeedRelease1) {
        delete Via1;
    }
}

void RepeatedInfos::findNoSenseRSs() {
    int Index = 0;
    for (auto Pair: RSMap) {
        Index++;
        RepeatedSubstringByS *RS = Pair.second;
        if (!RS) {
            std::cout << "error:RSMap has null pointer.\n";
            continue;
        }

        if (isOverlap(RS->StartIndices, RS->Length)) {
            continue;
        }

        SuffixTreeNode *NextNode = RS->ReleatedNode->Link;
        if (RSMap.find(NextNode) == RSMap.end()) {
            continue;
        }
        RepeatedSubstringByS *TmpRS2 = RSMap[NextNode];
        if (!TmpRS2)
            continue;
        if (TmpRS2->StartIndices.size() == RS->StartIndices.size()) {
            NoSenseRSList[NextNode] = TmpRS2;
        }
    }
}

RepeatedInfos::RepeatedInfos(SuffixTree &ST, unsigned RepeatedStrLenLowerLimit)
        : RepeatedStrLenLimit(RepeatedStrLenLowerLimit) {
    // constract RepeatedInfos from a suffix tree
    Str = ST.Str;
    traverseSTNode(*ST.getRoot());
    // findMoreNoSenseRSs(*ST.getRoot());
    findNoSenseRSs();
    // delete node which not need;

    for (auto Pair: RSMap) {
        if (NoSenseRSList.find(Pair.first) == NoSenseRSList.end()) {
            // RS which make sense will be push back into RSList after we process
            // selfing;
            RSList.push_back(processSelfing(Pair.second));
        }
    }
}

RepeatedInfos::RepeatedInfos(SuffixTree &ST, bool Clear,
                             unsigned RepeatedStrLenLowerLimit)
        : RepeatedStrLenLimit(RepeatedStrLenLowerLimit) {
    // constract RepeatedInfos from a suffix tree
    Str = ST.Str;
    traverseSTNode(*ST.getRoot());
    // findMoreNoSenseRSs(*ST.getRoot());
    if (Clear)
        findNoSenseRSs();
    // delete node which not need;

    for (auto Pair: RSMap) {
        if (NoSenseRSList.find(Pair.first) == NoSenseRSList.end()) {
            // RS which make sense will be push back into RSList after we process
            // selfing;
            RSList.push_back(processSelfing(Pair.second));
        }
    }
}

std::vector<unsigned>
RepeatedInfos::traverseSTNode(SuffixTreeNode &CurrentNode) {
    std::vector<unsigned> SuffixIndicesOfCurrent;
    if (CurrentNode.isLeaf()) {
        // each leaf node only occur one time;
        // if (CurrentNode.SuffixIdx == 60) {
        //   std::cout << "debug";
        // }
        SuffixIndicesOfCurrent.push_back(CurrentNode.SuffixIdx);
        return SuffixIndicesOfCurrent;
    }
    bool IsRoot = CurrentNode.isRoot();
    for (auto ChildPair: CurrentNode.Children) {
        std::vector<unsigned int> SuffixIndicesOfChild =
                traverseSTNode(*ChildPair.second);
        if (!IsRoot)
            SuffixIndicesOfCurrent.insert(SuffixIndicesOfCurrent.end(),
                                          SuffixIndicesOfChild.begin(),
                                          SuffixIndicesOfChild.end());
    }
    if (!IsRoot && CurrentNode.ConcatLen >= RepeatedStrLenLimit &&
        !isAllOverlap(SuffixIndicesOfCurrent, CurrentNode.ConcatLen)) {
        // create RepeatedSubstringByS and try to insert it into RSSet
        // RepeatedSubstringByS *RS = new
        // RepeatedSubstringByS(CurrentNode.ConcatLen, SuffixIndicesOfCurrent);
        RepeatedSubstringByS *RS = new RepeatedSubstringByS(
                CurrentNode.ConcatLen, SuffixIndicesOfCurrent, &CurrentNode);
        RS->FromRoot = true;
        RSMap[&CurrentNode] = RS;
        // TODO
        // if (!RSInsertResult.second) {
        //   // if insert failed, it means that the node has already insert into the
        //   // set when we deal with the SubRS relation, these node inserted has no
        //   // subRS and has SuperRS.
        //   // We need to replace the node with the new one.
        //   // So we need to update all the pointer to old, and keep the super RS
        //   info
        //   // in the new one.
        //   auto It = std::find(RSSet.begin(), RSSet.end(), RS);
        //   RepeatedSubstringByS *OldRSPointer = (RepeatedSubstringByS *)&*It;

        //   RS.SuperLinkRSs = It->SuperLinkRSs;
        //   for (RepeatedSubstringByS *SuperRS : RS.SuperLinkRSs) {
        //     std::vector<RepeatedSubstringByS *>::iterator ToDel =
        //         std::find(SuperRS->SubLinkRSs.begin(), SuperRS->SubLinkRSs.end(),
        //                   OldRSPointer);
        //     if (ToDel != SuperRS->SubLinkRSs.end())
        //       SuperRS->SubLinkRSs.erase(ToDel);
        //   }
        //   RSSet.erase(It);
        //   if (!RSSet.insert(RS).second) {
        //     std::cout << "Error!!!!!!!";
        //   }
        //   RepeatedSubstringByS *NewRS =
        //       (RepeatedSubstringByS *)&*std::find(RSSet.begin(), RSSet.end(),
        //       RS);
        //   for (RepeatedSubstringByS *SuperRS : NewRS->SuperLinkRSs) {
        //     SuperRS->SubLinkRSs.push_back(NewRS);
        //   }
        //   // if this node fully include by another
        //   if (RS.StartIndices.size() <= OldRSPointer->MaxSRSSize) {
        //     // NoSenseRSList.push_back(NewRS);
        //   }
        // }
        // if (CurrentNode.size() >= 2 && CurrentNode.size() <
        // CurrentNode.ConcatLen) {
        //   RepeatedSubstringByS *NewRS =
        //       (RepeatedSubstringByS *)&*std::find(RSSet.begin(), RSSet.end(),
        //       RS);
        //   setSubRSRelation(*NewRS, CurrentNode.size());
        // }
        // end TODO
    }
#ifdef ENABLE_DEBUG_CODE
    // printRepeatedInfoOfNode(CurrentNode, SuffixIndicesOfCurrent);
#endif

    return SuffixIndicesOfCurrent;
}

#pragma endregion implementations of SuffixTreeRepeatedInfos