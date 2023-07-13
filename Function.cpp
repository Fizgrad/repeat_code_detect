//
// Created by david on 23-7-9.
//

#include "Function.h"

#include <utility>

Function::Function(string id, unsigned long long int address) : methodID(std::move(id)), address(address) {

}

Function::Function(const Function &function) : methodID(function.methodID), address(function.address) {
    for (auto &i: function.basicBlocks)
        this->basicBlocks.push_back(i);
}
