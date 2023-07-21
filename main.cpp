
#include "include/File.h"
#include "include/FindSimpleRepeat.h"
#include <boost/program_options.hpp>
#include <glog/logging.h>

namespace po = boost::program_options;


static string out;
static string code;
static string mode;


using namespace llvm;

static int parseArgs(int argc, char *argv[]) {

    po::options_description desc("Allowed options");
    desc.add_options()
            ("code", po::value<std::string>(), "file produced by oat-dump-tool")
            ("out", po::value<std::string>(), "file to store repeated res")
            ("mode", po::value<std::string>(), "hash or origin");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("code") && vm.count("out") && vm.count("mode")) {
        mode = vm["mode"].as<std::string>();
        code = vm["code"].as<std::string>();
        out = vm["out"].as<std::string>();
    } else {
        std::cout << desc << "\n";
        return 1;
    }
    return 0;
}


int main(int argc, char *argv[]) {

    google::InitGoogleLogging(argv[0]);

    if (parseArgs(argc, argv)) {
        return -1;
    }

    FindSimpleRepeat findSimpleRepeat(code);
    if (mode == "origin") {
        LOG(INFO) << "Start Origin Mode ";
        findSimpleRepeat.analysisAll();
    } else if (mode == "hash") {
        LOG(INFO) << "Start Hash Mode ";
        findSimpleRepeat.analysisHash();
    } else {
        LOG(ERROR) << "Invalid Mode";
        return 1;
    }
    findSimpleRepeat.writeToFile(out);

}