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

std::vector<Duople<std::string, std::string>> getCmdLineArguments(int argc, const char** argv) {
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
    for (auto& value : optionAndValue) {
        std::string fixed;
        for (auto& c : value.two) {
            if (c == '\"' || c == '\'')
                continue;
            else
                fixed += c;
        }
        value.two = fixed;
    }
    return optionAndValue;
}
std::vector<Duople<std::string, std::string>> getFileArguments(std::filesystem::path path) {
    path = std::filesystem::absolute(path);
    if (!std::filesystem::exists(path) || path.empty())
        CLL_StdErr("Error Loading Path:", {"Path"}, {path.string()});

    std::vector<Duople<std::string, std::string>> optionAndValue;
    auto wordBuffer = Parser(path.string(), Parser::RecordNewLine).getWordBuffer();
    Duople<std::string, std::string> option;
    for (int i = 0; i < wordBuffer.size(); i++) {
        std::cout << wordBuffer[i].str << "\n";
        continue;
        if (wordBuffer[i].str == "-") {
            option.one = wordBuffer[i+1];
            i++;
            continue;
        }

    }


    return {
            {}
    };
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
        else if (arg.one == "dir") {
            baseDirectory = std::filesystem::absolute(arg.two);
        }
    }
    printf("Settings:\n"
           "\tDefault Integer Width: %i\n"
           "\tDefault String Size: %i\n"
           "\tRecord New Lines: %s\n"
           "\tBase File Directory: '%s'\n",
           defaultIntegerWidth,
           defaultStringSize,
           ((newLines)?"True":"False"),
           baseDirectory.string().c_str());
}


CompilerDirectory::CompilerDirectory(std::filesystem::path path, std::string extension) {
    path = std::filesystem::absolute(path);
    if (path.empty())
        path = std::filesystem::current_path();
    if (!std::filesystem::exists(path) || path.empty())
        CLL_StdErr("Error Loading Path:", {"Path"}, {path.string()});


    Duople<bool, std::filesystem::path> confFile = { false, {} };
    for (const auto& file : std::filesystem::directory_iterator(path)) {
        std::cout << file.path().string() << "\n";
        if (file.path().filename() == "config.txt") {
            confFile.one = true;
            confFile.two = file.path();
        } else {
            std::cout << file.path().filename() << " " << file.path().extension() << "\n";
        }
    }

    baseDirectory = path;
    configFile = confFile.two;
}
