//
// Created by david on 23-7-20.
//
#include "../include/Instruction.h"

#include <capstone/capstone.h>
#include <capstone/arm64.h>


void Instruction::parse(csh handle, cs_insn *insn) {
    cs_detail *detail = insn->detail;
    this->address = insn->address;
    this->id = insn->id;

    auto bytesLen = insn->size;
    for (int i = 0; i < bytesLen; ++i) {
        this->bytecode |= (insn->bytes[i] << (8 * i));
    }
    this->mnemonic = string(insn->mnemonic);

    if (detail->arm64.op_count)
        this->operandsCount = detail->arm64.op_count;

    for (int n = 0; n < detail->arm64.op_count; n++) {
        cs_arm64_op *op = &(detail->arm64.operands[n]);
        switch (op->type) {
            case ARM64_OP_REG:
//                printf("\t\toperands[%u].type: REG = %s\n", n, cs_reg_name(handle, op->reg));
                this->regs.insert(cs_reg_name(handle, op->reg));

                break;
            case ARM64_OP_MEM:
//                printf("\t\toperands[%u].type: MEM\n", n);
                if (op->mem.base != ARM64_REG_INVALID)
                    this->regs.insert(cs_reg_name(handle, op->mem.base));
//                    printf("\t\t\toperands[%u].mem.base: REG = %s\n", n, cs_reg_name(handle, op->mem.base));
                if (op->mem.index != ARM64_REG_INVALID)
                    this->regs.insert(cs_reg_name(handle, op->mem.index));
//                    printf("\t\t\toperands[%u].mem.index: REG = %s\n", n, cs_reg_name(handle, op->mem.index));

            default:
                break;
        }
    }
}

Instruction::Instruction(csh handle, cs_insn *insn) {
    this->parse(handle, insn);
    calculateHashcode();
}

Instruction::Instruction(const Instruction &instruction) : bytecode(instruction.bytecode),
                                                           address(instruction.address),
                                                           hashcode(instruction.hashcode),
                                                           id(instruction.id),
                                                           operandsCount(instruction.operandsCount),
                                                           mnemonic(instruction.mnemonic),
                                                           regs(instruction.regs) {

}


void Instruction::calculateHashcode() {
    hashcode = bytecode;
    for (auto &i: regs) {
        auto reg = parseReg(i);
        if (reg.size() == 2 && isalnum(reg[0]) && isalnum(reg[1])) {
            hashcode = changeRegName(hashcode, std::stoi(reg), 0);
        }
    }
}
