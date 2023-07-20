//
// Created by david on 23-7-20.
//

#ifndef SUFFIX_TREE_CONSTANT_H
#define SUFFIX_TREE_CONSTANT_H

#include <capstone/capstone.h>
#include <capstone/arm64.h>

#include <set>

using std::set;

const set<arm64_insn> BranchSet = {ARM64_INS_CBNZ, ARM64_INS_CBZ, ARM64_INS_TBNZ, ARM64_INS_TBZ};
const set<arm64_insn> CallSet = {ARM64_INS_BL};
const set<arm64_insn> CallRegSet = {ARM64_INS_BLR};
const set<arm64_insn> JumpSet = {ARM64_INS_B};
const set<arm64_insn> JumpRegSet = {ARM64_INS_BR};
const set<arm64_insn> RetSet = {ARM64_INS_RET};
const set<arm64_insn> SoftBreak = {ARM64_INS_BRK};
const set<arm64_cc> CCAlwaysSet = {ARM64_CC_INVALID, ARM64_CC_AL, ARM64_CC_NV};

#endif //SUFFIX_TREE_CONSTANT_H
