//
// Created by Owner on 10/17/2022.
//

#ifndef CLL_ENVIORNMENT_HPP
#define CLL_ENVIORNMENT_HPP

#include <stdint.h>
#include <filesystem>

#include "../CLL.hpp"

std::vector<Duople<std::string, std::string>> gatherCmdLineArguments(int argc, const char** argv);

struct CompilerOptions {
    CompilerOptions() = default;
    CompilerOptions(std::vector<Duople<std::string, std::string>>& arguments);
    int32_t defaultIntegerWidth = 32;
    int32_t defaultStringSize   = 128;
    bool    newLines            = false;
};

struct CompilerDirectory {
    CompilerDirectory() = default;
    std::filesystem::path baseDirectory;
};



#endif //CLL_ENVIORNMENT_HPP
