
#include "ReadFile.h"
#include "FindSimpleRepeat.h"

using namespace llvm;

int main() {
    FindSimpleRepeat findSimpleRepeat("/home/david/Documents/Demo/data/cfa");
    findSimpleRepeat.analysisAll();

    findSimpleRepeat.writeToFile("/home/david/Documents/Demo/res/origin_out.txt");

//    ReadFile file("/home/david/Documents/Demo/data/cfa");
//    file.testReadCode();
    return 0;
}