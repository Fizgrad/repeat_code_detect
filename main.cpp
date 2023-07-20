
#include "include/File.h"
#include "include/FindSimpleRepeat.h"
#include <boost/program_options.hpp>
#include <capstone/capstone.h>

namespace po = boost::program_options;


using namespace llvm;


int main(int argc, char *argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
            ("code", po::value<std::string>(), "file produced by oat-dump-tool")
            ("out", po::value<std::string>(), "file to store repeated res")
            ("mode", po::value<std::string>(), "hash or origin");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("code") && vm.count("out") && vm.count("mode")) {
        FindSimpleRepeat findSimpleRepeat(vm["code"].as<std::string>());
        if (vm["mode"].as<std::string>() == "origin")
            findSimpleRepeat.analysisAll();
        else if (vm["mode"].as<std::string>() == "hash")
            findSimpleRepeat.analysisHash();
        else {
            std::cout << desc << "\n";
            return 1;
        }
        findSimpleRepeat.writeToFile(vm["out"].as<std::string>());
    } else {
        std::cout << desc << "\n";
        return 1;
    }
    return 0;
}