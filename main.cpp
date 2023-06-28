
#include "ReadFile.h"
#include "FindSimpleRepeat.h"

using namespace llvm;

int main() {
    FindSimpleRepeat findSimpleRepeat("/home/david/Documents/Demo/cfa.txt");
    for (int i = 0; i < 100; ++i) {
        findSimpleRepeat.analysisNextFunction();
    }

    findSimpleRepeat.writeToFile("/home/david/Documents/Demo/cfa_predict.txt");
    return 0;
}