
#include "File.h"
#include "FindSimpleRepeat.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;


using namespace llvm;


int main(int argc, char *argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
            ("dump-code", po::value<std::string>(), "file produced by oatdump")
            ("out-file", po::value<std::string>(), "file to store repeated info");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("dump-code") && vm.count("out-file")) {
        FindSimpleRepeat findSimpleRepeat(vm["dump-code"].as<std::string>());
        findSimpleRepeat.analysisAll();
        findSimpleRepeat.writeToFile(vm["out-file"].as<std::string>());
    }else {
        std::cout << desc << "\n";
        return 1;
    }

    return 0;
}