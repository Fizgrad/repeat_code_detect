#include "include/File.h"
#include "include/FindSimpleRepeat.h"
#include <glog/logging.h>
#include <iostream>
#include <string>
#include <cstdlib> // 用于 std::exit

using namespace llvm;

// 封装命令行参数的结构体
struct Options {
    std::string code;
    std::string out;
    std::string mode;
};

int main(int argc, char *argv[]) {

    google::InitGoogleLogging(argv[0]);

    // 创建 Options 对象
    Options options{"","",""};

    // 输出所有传入的命令行参数
    LOG(INFO) << "Command Line Arguments:";
    for (int i = 0; i < argc; ++i) {
        LOG(INFO) << "argv[" << i << "]: " << argv[i];
    }

    // 最少需要 3 个参数: --mode, --code, --out
    if (argc < 7) {
        std::cerr << "Error: Missing required arguments\n";
        std::cerr << "Usage: " << argv[0] << " --mode <mode> --code <file_path> --out <file_path>\n";
        return 1;
    }

    // 遍历参数
    for (int i = 1; i < argc; ++i) {
        std::string arg =    std::string(argv[i]);
        
        // 解析 --mode 参数
        if (arg == "--mode" && i + 1 < argc) {
            options.mode =std::string(argv[++i]);
        }
        // 解析 --code 参数
        else if (arg == "--code" && i + 1 < argc) {
            options.code = std::string(argv[++i]);
        }
        // 解析 --out 参数
        else if (arg == "--out" && i + 1 < argc) {
            options.out = std::string(argv[++i]);
        }
        // 未识别的参数
        else {
            std::cerr << "Error: Unknown option " << arg << "\n";
            std::cerr << "Usage: " << argv[0] << " --mode <mode> --code <file_path> --out <file_path>\n";
            return 1;
        }
    }

    // 检查必需的参数是否都已解析
    if (options.code.empty() || options.out.empty() || options.mode.empty()) {
        std::cerr << "Error: Missing required parameters. Please provide --mode, --code, and --out.\n";
        return 1;
    }

    // 使用 glog 打印解析出的参数
    LOG(INFO) << "Parsed Parameters: ";
    LOG(INFO) << "Code File: " << options.code;
    LOG(INFO) << "Output File: " << options.out;
    LOG(INFO) << "Mode: " << options.mode;

    // 创建 FindSimpleRepeat 对象
    FindSimpleRepeat findSimpleRepeat(options.code);

    // 根据模式执行不同的分析操作
    if (options.mode == "origin") {
        LOG(INFO) << "Start Origin Mode ";
        findSimpleRepeat.analysisAll();
    } else if (options.mode == "hash") {
        LOG(INFO) << "Start Hash Mode ";
        findSimpleRepeat.analysisHash();
    } else {
        LOG(ERROR) << "Invalid Mode: " << options.mode;
        return 1;
    }

    // 将结果写入文件
    findSimpleRepeat.writeToFile(options.out);

    return 0;
}