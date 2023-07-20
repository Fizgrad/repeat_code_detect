//
// Created by david on 23-7-20.
//
#include "../include/Instruction.h"

#include <capstone/capstone.h>
#include <capstone/arm64.h>


void Instruction::parse(csh handle, cs_insn *insn) {
    cs_detail *detail = insn->detail;
    if (detail->arm64.op_count)
        this->operandsCount = detail->arm64.op_count;

    for (int n = 0; n < detail->arm64.op_count; n++) {
        cs_arm64_op *op = &(detail->arm64.operands[n]);
        switch (op->type) {
            case ARM64_OP_REG:
//                printf("\t\toperands[%u].type: REG = %s\n", n, cs_reg_name(handle, op->reg));
                this->regs.emplace_back(cs_reg_name(handle, op->reg));

                break;
            case ARM64_OP_MEM:
//                printf("\t\toperands[%u].type: MEM\n", n);
                if (op->mem.base != ARM64_REG_INVALID)
                    this->regs.emplace_back(cs_reg_name(handle, op->mem.base));
//                    printf("\t\t\toperands[%u].mem.base: REG = %s\n", n, cs_reg_name(handle, op->mem.base));
                if (op->mem.index != ARM64_REG_INVALID)
                    this->regs.emplace_back(cs_reg_name(handle, op->mem.index));
//                    printf("\t\t\toperands[%u].mem.index: REG = %s\n", n, cs_reg_name(handle, op->mem.index));

            default:
                break;
        }
    }
}

Instruction::Instruction(csh handle, cs_insn *insn) {
    this->parse(handle, insn);
}
