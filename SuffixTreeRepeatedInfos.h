//===- llvm/ADT/SuffixTreeRepeatedInfos.h - Tree for substrings --------------*-
// C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines the Repeated Infos we can find from Suffix Tree class and
// Suffix Tree Node struct.
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_SUPPORT_SUFFIXTREE_REPEATED_INFOS_H
#define LLVM_SUPPORT_SUFFIXTREE_REPEATED_INFOS_H

#include "SuffixTree.h"
#include <algorithm>
#include <map>
#include <cmath>
#include <vector>

namespace llvm {

#pragma region swh_modify

    class RepeatedInfos {
        std::vector<unsigned> Str;
        unsigned int RepeatedStrLenLimit = 2;

    public:
        struct RepeatedSubstringByS {
            /// The length of the string.
            unsigned Length;
            /// The start indices of each occurrence.
            std::vector<unsigned> StartIndices;
            RepeatedSubstringByS *OriginalRS = nullptr;

            std::vector<RepeatedSubstringByS *> SubLinkRSs;
            std::vector<RepeatedSubstringByS *> SuperLinkRSs;
            SuffixTreeNode *ReleatedNode;
            // a open_flag to judge whether the string is from root node
            bool FromRoot = false;
            unsigned MaxSRSSize = 0;

            bool operator<(RepeatedSubstringByS RS) const;

            bool operator==(RepeatedSubstringByS RS) const;

            RepeatedSubstringByS(unsigned Len, std::vector<unsigned> SI,
                                 SuffixTreeNode *Node)
                    : Length(Len), StartIndices(SI), ReleatedNode(Node) {
                MaxSRSSize = SI.size();
            };

            // use the instr number to predict the benefit. Only if the RS has no
            // overlapping, can we get the suitable result
            unsigned getPredictBenefit(unsigned CreateFuncOverHead) const;

            void print(const std::vector<unsigned> &Str);
        };

        // std::set<RepeatedSubstringByS> RSSet;
        std::map<SuffixTreeNode *, RepeatedSubstringByS *> RSMap;

        // if a repeat substring's all occurances with the same prefix, then the
        // repeat substring make no sense, we should delete it after consract the
        // RepeatedInfos.
        std::map<SuffixTreeNode *, RepeatedSubstringByS *> NoSenseRSList;

        std::vector<RepeatedSubstringByS *> RSList;

        RepeatedInfos();

        // get Repeated Info from ST
        RepeatedInfos(SuffixTree &ST, unsigned RepeatedStrLenLowerLimit);

        RepeatedInfos(SuffixTree &ST, bool Clear, unsigned RepeatedStrLenLowerLimit);

        void printRepeatedInfoOfNode(SuffixTreeNode &Node,
                                     std::vector<unsigned> &SuffixIndicesOfCurrent);

        void setRepeatedStrLenLimit(unsigned int RepeatedStrLenLowerLimit) {
            RepeatedStrLenLimit = RepeatedStrLenLowerLimit;
        }

        unsigned int getRepeatedStrLenLimit() { return RepeatedStrLenLimit; }

        static void elimateInterOverlap(
                std::vector<RepeatedInfos::RepeatedSubstringByS *> &RSList,
                std::vector<unsigned> &StrMap, unsigned CreateFunctionOverhead) {
            // sort by benefit
            std::sort(
                    RSList.begin(), RSList.end(),
                    [&CreateFunctionOverhead](RepeatedInfos::RepeatedSubstringByS *LHS,
                                              RepeatedInfos::RepeatedSubstringByS *RHS) {
                        return LHS->getPredictBenefit(CreateFunctionOverhead) >
                               RHS->getPredictBenefit(CreateFunctionOverhead);
                    });

            // iterate the list and remove all inter-overlap
            for (std::vector<RepeatedInfos::RepeatedSubstringByS *>::iterator RSIt =
                    RSList.begin();
                 RSIt != RSList.end();) {
                // If we outlined something that overlapped with a candidate in a previous
                // step, then we can't outline from it.
                llvm::RepeatedInfos::RepeatedSubstringByS *RSP = *RSIt;
                // llvm::RepeatedInfos::RepeatedSubstringByS RS = *RSP;
                unsigned StrLength = RSP->Length;

                // std::erase_if(RSP->StartIndices, [&StrMap, &StrLength](unsigned
                // &StartIdx) {
                //   return std::any_of(
                //       StrMap.begin() + StartIdx, StrMap.begin() + StartIdx + StrLength,
                //       [](unsigned I) { return (I == static_cast<unsigned>(-1)); });
                // });

                for (auto It = RSP->StartIndices.begin();
                     It != RSP->StartIndices.end();) {
                    unsigned StartIdx = *It;
                    if (std::any_of(StrMap.begin() + StartIdx,
                                    StrMap.begin() + StartIdx + StrLength, [](unsigned I) {
                                return (I == static_cast<unsigned>(-1));
                            })) {
                        It = RSP->StartIndices.erase(It);
                    } else {
                        It++;
                    }
                }

                // If we made it unbeneficial to outline this function, skip it.
                if (RSP->getPredictBenefit(CreateFunctionOverhead) < 1) {
                    RSList.erase(RSIt);
                    continue;
                }
                ++RSIt;

                // If beneficial, record it in StrMap;
                std::for_each(RSP->StartIndices.begin(), RSP->StartIndices.end(),
                              [&StrMap, &StrLength](unsigned &StartIdx) {
                                  std::for_each(
                                          StrMap.begin() + StartIdx,
                                          StrMap.begin() + StartIdx + StrLength,
                                          [](unsigned &I) { I = static_cast<unsigned>(-1); });
                              });
            }
        }

        static unsigned analysisOld(SuffixTree &ST,
                                    unsigned int RepeatedStrLenLowerLimit,
                                    unsigned CreateFuncOverhead) {

            std::vector<RepeatedInfos::RepeatedSubstringByS *> RSList;
            for (auto It = ST.begin(), Et = ST.end(); It != Et; ++It) {
                SuffixTree::RepeatedSubstring RS = *It;
                unsigned StringLen = RS.Length;
                std::vector<unsigned> NewStartIndices;

                if (RS.Length < RepeatedStrLenLowerLimit) {
                    continue;
                }

                for (const unsigned &StartIdx: RS.StartIndices) {
                    unsigned EndIdx = StartIdx + StringLen - 1;
                    if (std::all_of(NewStartIndices.begin(), NewStartIndices.end(),
                                    [&StartIdx, &StringLen](const unsigned &Idx) {
                                        return (StartIdx > Idx + StringLen - 1 ||
                                                StartIdx + StringLen - 1 < Idx);
                                    })) {
                        NewStartIndices.push_back(StartIdx);
                    }
                }
                // if (NewStartIndices.size() < RepeatedStrLenLowerLimit) {

                RepeatedInfos::RepeatedSubstringByS *NewRS =
                        new RepeatedInfos::RepeatedSubstringByS(StringLen, NewStartIndices,
                                                                nullptr);
                if (NewRS->getPredictBenefit(CreateFuncOverhead) < 1) {
                    continue;
                }
                RSList.push_back(NewRS);
            }

            // Previous process make sure that RSList do not have overlap in one RS;
            // Next we should eliminate the overlap between RSs of RSList;
            std::vector<unsigned> StrMap = ST.Str;
            RepeatedInfos::elimateInterOverlap(RSList, StrMap, CreateFuncOverhead);
            unsigned TotalBenefit = 0;
            std::for_each(RSList.begin(), RSList.end(),
                          [&TotalBenefit, CreateFuncOverhead](
                                  RepeatedInfos::RepeatedSubstringByS *RS) {
                              TotalBenefit += RS->getPredictBenefit(CreateFuncOverhead);
                          });
            return TotalBenefit;
        }

    private:
        // insert into RSMap if the node's string repeat, and return start indices
        // of each occurrence'suffix of this node
        std::vector<unsigned> traverseSTNode(SuffixTreeNode &Node);

        bool isOverlap(std::vector<unsigned> &SuffixIndices, unsigned Length);

        bool isAllOverlap(std::vector<unsigned> &SuffixIndices, unsigned Length);

        RepeatedSubstringByS *processSelfing(RepeatedSubstringByS *RS);

        void setSubRSRelation(RepeatedSubstringByS &CurrentNode, size_t Size);

        void findMoreNoSenseRSs(SuffixTreeNode &CurrentNode);

        void findNoSenseRSs();

        void freeMemory() {
            for (std::pair<SuffixTreeNode *const, RepeatedSubstringByS *> Pair:
                    RSMap) {
                RepeatedSubstringByS *RSToFree = Pair.second;
                delete RSToFree;
                Pair.second = nullptr;
            }
        };
    };

#pragma endregion

} // namespace llvm

#endif // LLVM_SUPPORT_SUFFIXTREE_REPEATED_INFOS_H