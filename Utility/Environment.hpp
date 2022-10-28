//
// Created by Owner on 10/17/2022.
//

#ifndef CLL_ENVIRONMENT_HPP
#define CLL_ENVIRONMENT_HPP

#include <stdint.h>
#include <filesystem>
#include <fstream>

#include "../CLL.hpp"
#include "../Parser/Parser.hpp"

std::vector<Duople<std::string, std::string>> getCmdLineArguments(size_t argc, const char** argv, size_t indexOffset = 1, char prefix = '-');
std::vector<Duople<std::string, std::string>> getFileArguments(std::filesystem::path path);

// TODO: Work On File/Directory System
// TODO: Work On Code Modules

struct CompilerCodeModule {
    std::string moduleName;
    std::filesystem::path path;
    std::vector<std::string> wordBuffer;
};

struct CompilerDirectory {
    CompilerDirectory() = default;
    CompilerDirectory(std::filesystem::path path);

    std::filesystem::path baseDirectory;
    std::filesystem::path configFile;
    std::vector<CompilerCodeModule> modules;
};

struct CompilerOptions {
    CompilerOptions() = default;
    CompilerOptions(std::vector<Duople<std::string, std::string>>& arguments);
    CompilerOptions(CompilerDirectory& directory);

    std::filesystem::path baseDirectory = std::filesystem::current_path();
    std::filesystem::path configFile;

    int32_t defaultIntegerWidth = 32;
    int32_t defaultStringSize = 128;
    bool newLines = false;

    void linkDirectory(CompilerDirectory& directory) {
        baseDirectory = directory.baseDirectory;
        configFile = directory.configFile;
        printf("Found New Configuration File, Overriding Current Settings: '%s'\n", configFile.string().c_str());
        auto arguments = getFileArguments(configFile);
        *this = CompilerOptions(arguments);
    }
};




#endif //CLL_ENVIRONMENT_HPP
