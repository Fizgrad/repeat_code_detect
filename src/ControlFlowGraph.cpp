//
// Created by david on 23-7-20.
//

#include "../include/ControlFlowGraph.h"
#include "../include/Constant.h"
#include <cassert>
#include <glog/logging.h>

Function ControlFlowGraph::buildCFG() {
    if (this->methodCodeOffset.size() == 10) {
        address = std::stoull(this->methodCodeOffset.substr(2), nullptr, 16);
    } else if (this->methodCodeOffset.size() == 8) {
        address = std::stoull(this->methodCodeOffset, nullptr, 16);
    }

    parseInstruction();

    buildBlocks();

    buildEdges();

    return {this->methodID, this->address, this->blocks, this->hashBR};
}

void ControlFlowGraph::parseInstruction() {
    csh handle;
    cs_insn *insn;
    size_t count;
    if (this->methodCodeOffset.size() == 10) {
        this->methodCodeOffset = this->methodCodeOffset.substr(2);
    }

    LOG(INFO) << "Parse function:\t" << this->methodID << "\t" << this->methodCodeOffset;
    assert(this->methodCodeOffset.size() == 8);

    this->address = std::stoull(this->methodCodeOffset, nullptr, 16);

    if (cs_open(CS_ARCH_ARM64, CS_MODE_ARM, &handle) != CS_ERR_OK) {
        LOG(ERROR) << "cs_open(CS_ARCH_ARM64, CS_MODE_ARM, &handle) != CS_ERR_OK";
        return;
    }

    cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);

    if (this->code == nullptr) {
        this->parseCode();
    }

    count = cs_disasm(handle, code, this->bytecode.size(), address, 0, &insn);
    if (count > 0) {
        bool flag = false;
        size_t j;
        addLabel(address);
        for (j = 0; j < count; j++) {
            Instruction instruction(handle, &insn[j]);
            instructions.push_back(instruction);

//            instruction.outputInstructionDetail(std::cout);

            if (flag) {
                addLabel(insn[j].address);
                flag = false;
            }

            if (BranchSet.count(insn[j].id)) {
                cs_arm64_op opr = insn[j].detail->arm64.operands[insn[j].detail->arm.op_count - 1];
                int64_t to = opr.imm;
                address2Target[insn[j].address] = to;
                isConditionalJumpOrBranch[insn[j].address] = true;
                addLabel(to);
                flag = true;
            } else if (CallSet.count(insn[j].id)) {
                cs_arm64_op opr = insn[j].detail->arm64.operands[0];
                int64_t to = opr.imm;
                addLabel(to);
                address2Target[insn[j].address] = to;
            } else if (CallRegSet.find(insn[j].id) != CallRegSet.end()) {

            } else if (JumpSet.count(insn[j].id)) {
                cs_arm64_op opr = insn[j].detail->arm64.operands[0];
                int64_t to = opr.imm;
                addLabel(to);
                isConditionalJumpOrBranch[insn[j].address] = false;
                flag = true;
            } else if (JumpRegSet.count(insn[j].id)) {
                isConditionalJumpOrBranch[insn[j].address] = false;
                flag = true;
                hashBR = true;
            } else if (RetSet.count(insn[j].id)) {
                flag = true;
            }
        }

        LOG(INFO) << "Code count:\t" << j;

        cs_free(insn, count);
    } else {
        LOG(ERROR) << "Failed to disassemble given code!";
    }

    cs_close(&handle);
}

void ControlFlowGraph::parseCode() {
    auto len = this->bytecode.size();
    this->code = new uint8_t[len + 1];
    for (auto i = 0; i <= len; ++i) {
        code[i] = static_cast<uint8_t>(this->bytecode[i]);
    }
    code[len] = '\0';
}

void ControlFlowGraph::addLabel(unsigned long long int addr) {
    this->labels.insert(addr);
}

void ControlFlowGraph::buildBlocks() {
    unsigned int blockIndex = 0;
    Block block(methodID, blockIndex, address);
    address2BlockID[address] = 0;
    for (auto &inst: instructions) {
        unsigned long long addr = inst.address;
        if (labels.count(addr)) {
            if (!block.instructions.empty()) this->blocks.emplace_back(block);
            address2BlockID[addr] = blockIndex;
            block = Block(methodID, blockIndex++, addr);
        }
        block.instructions.emplace_back(inst);
    }
    if (!block.instructions.empty()) {
        this->blocks.emplace_back(block);
    }
}

void ControlFlowGraph::buildEdges() {
    Block *prev = &this->blocks[0];
    for (int i = 1; i < blocks.size(); ++i) {
        Block *now = &blocks[i];
        auto lastBlockLastInstructionAddress = prev->instructions.back().address;
        if (!isConditionalJumpOrBranch.count(lastBlockLastInstructionAddress) ||
            isConditionalJumpOrBranch[lastBlockLastInstructionAddress]) {
            prev->addSuccessor(now->blockID);
            now->addPredecessor(prev->blockID);
        }
        auto nowBlockLastInstructionAddress = now->instructions.back().address;
        if (address2Target.count(nowBlockLastInstructionAddress)) {
            auto toAddress = address2Target[nowBlockLastInstructionAddress];
            now->addSuccessor(address2BlockID[toAddress]);
            this->blocks[address2BlockID[toAddress]].addPredecessor(now->blockID);
        }
        prev = now;
    }
}
