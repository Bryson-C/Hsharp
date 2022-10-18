//
// Created by Owner on 10/17/2022.
//

#include "Enviornment.hpp"

namespace {
    // Actual Stolen Code: https://stackoverflow.com/questions/108318/how-can-i-test-whether-a-number-is-a-power-of-2
    bool powerOfTwo(int32_t x) {
        return (!(x == 0) && !(x & (x - 1)));
    }
}

std::vector<Duople<std::string, std::string>> gatherCmdLineArguments(int argc, const char** argv) {
    std::vector<std::string> cmdArgument(argc);
    // start at index of 1 to avoid program name
    for (size_t i = 1; i < argc; i++) {
        cmdArgument.emplace_back(argv[i]);
    }

    std::vector<Duople<std::string, std::string>> optionAndValue;
    for (int loop = 0; const auto& arg : cmdArgument) {
        if (arg[0] == '-') {
            int index = 1;
            std::string option, value;
            while (arg[index] != ':' && index < arg.size()-1) {
                option += arg[index++];
            }
            if (arg[index] == ':') {
                index++;
                do {
                    value += arg[index++];
                } while (index < arg.size());
            }
            optionAndValue.push_back({option, value});
        }
        loop++;
    }
    return optionAndValue;
}

CompilerOptions::CompilerOptions(std::vector<Duople<std::string, std::string>>& arguments) {
    for (const auto& arg : arguments) {
        if (arg.one == "intWidth") {
            defaultIntegerWidth = powerOfTwo(std::stoi(arg.two)) ? std::stoi(arg.two): 32;
        }
        else if (arg.one == "stringSize") {
            defaultStringSize = std::stoi(arg.two);
        }
        else if (arg.one == "newLines") {
            if (toLowerCase(arg.two) == "true")
                newLines = true;
            else if (toLowerCase(arg.two) == "false")
                newLines = false;
        }
    }
    printf("Settings:\n\tDefault Integer Width: %i\n\tDefault String Size: %i\n\tRecord New Lines: %s\n",
           defaultIntegerWidth,
           defaultStringSize,
           ((newLines)?"True":"False"));
}

