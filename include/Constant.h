//
// Created by david on 23-7-20.
//

#ifndef SUFFIX_TREE_CONSTANT_H
#define SUFFIX_TREE_CONSTANT_H

#include <capstone/capstone.h>
#include <capstone/arm64.h>

#include <set>

using std::set;

const set<unsigned int> BranchSet = {ARM64_INS_CBNZ, ARM64_INS_CBZ, ARM64_INS_TBNZ, ARM64_INS_TBZ};
const set<unsigned int> CallSet = {ARM64_INS_BL};
const set<unsigned int> CallRegSet = {ARM64_INS_BLR};
const set<unsigned int> JumpSet = {ARM64_INS_B};
const set<unsigned int> JumpRegSet = {ARM64_INS_BR};
const set<unsigned int> RetSet = {ARM64_INS_RET};
const set<unsigned int> SoftBreak = {ARM64_INS_BRK};
const set<unsigned int> CCAlwaysSet = {ARM64_CC_INVALID, ARM64_CC_AL, ARM64_CC_NV};

#endif //SUFFIX_TREE_CONSTANT_H
