//
// Created by david on 23-7-20.
//

#include "../include/ControlFlowGraph.h"


Function ControlFlowGraph::buildCFG() {
    unsigned long long int address = 0;
    if (this->methodCodeOffset.size() == 10) {
        address = std::stoull(this->methodCodeOffset.substr(2), nullptr, 16);
    } else if (this->methodCodeOffset.size() == 8) {
        address = std::stoull(this->methodCodeOffset, nullptr, 16);
    }
    Function res(this->methodID, address);








    return res;
}
