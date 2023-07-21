//
// Created by david on 23-7-20.
//

#ifndef SUFFIX_TREE_INSTRUCTION_H
#define SUFFIX_TREE_INSTRUCTION_H

#include <capstone/capstone.h>
#include <capstone/arm64.h>
#include <vector>
#include <string>
#include <map>
#include <ostream>
#include <set>

using std::vector;
using std::string;
using std::set;

class Instruction {
public:
    unsigned bytecode = 0;
    unsigned long long int address = 0;
    unsigned hashcode = 0;
    unsigned int id = 0;
    int operandsCount = 0;
    string mnemonic;
    set<string> regs;
    string op_str;
    string csInsnName;

    void parse(csh handle, cs_insn *insn);

    Instruction() = default;

    Instruction(const Instruction &instruction);

    Instruction(csh handle, cs_insn *insn);

    void calculateHashcode();

    void outputInstructionDetail(std::ostream &out) const;

private:

    static std::pair<bool, int> parseReg(const std::string &reg) {
        if (reg.length() == 3 && std::isalpha(reg[0]) && std::isdigit(reg[1]) && std::isdigit(reg[2])) {
            return {true, std::stoi(reg.substr(1))};
        } else if (reg.length() == 2 && std::isalpha(reg[0]) && std::isdigit(reg[1])) {
            return {true, std::stoi(reg.substr(1))};
        } else {
            return {false, -1};
        }
    }

    static unsigned int setRaRt2(unsigned int byteCode, int ra) {
        byteCode = byteCode & 0xffff83ff;
        byteCode = byteCode | (ra << 10);
        return byteCode;
    }

    static unsigned int getRaRt2(unsigned int byteCode) {
        byteCode = (byteCode >> 10);
        return byteCode & 0b11111;
    }

    static unsigned int getRmRs(unsigned int byteCode) {
        byteCode = (byteCode >> 16);
        return byteCode & 0b11111;
    }

    static unsigned int setRmRs(unsigned int byteCode, int ra) {
        byteCode = byteCode & 0xffe0ffff;
        byteCode = byteCode | (ra << 16);
        return byteCode;
    }

    static unsigned int getRn(unsigned int byteCode) {
        byteCode = (byteCode >> 5);
        return byteCode & 0b11111;
    }

    static unsigned int setRn(unsigned int byteCode, int ra) {
        byteCode = byteCode & 0xfffffc1f;
        byteCode = byteCode | (ra << 5);
        return byteCode;
    }

    static unsigned int getRdRt(unsigned int byteCode) {
        return byteCode & 0b11111;
    }

    static unsigned int setRdRt(unsigned int byteCode, int ra) {
        byteCode = byteCode & 0xffffffe0;
        byteCode = byteCode | ra;
        return byteCode;
    }

    static unsigned int changeRegName(unsigned int byteCode, int origin, int to) {
        if (getRn(byteCode) == origin) {
            byteCode = setRn(byteCode, to);
        }
        if (getRdRt(byteCode) == origin) {
            byteCode = setRdRt(byteCode, to);
        }
        if (getRaRt2(byteCode) == origin) {
            byteCode = setRaRt2(byteCode, to);
        }
        if (getRmRs(byteCode) == origin) {
            byteCode = setRmRs(byteCode, to);
        }
        return byteCode;
    }

};

#endif //SUFFIX_TREE_INSTRUCTION_H
