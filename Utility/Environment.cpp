//
// Created by Owner on 10/17/2022.
//

#include "Environment.hpp"

namespace {
    // Actual Stolen Code: https://stackoverflow.com/questions/108318/how-can-i-test-whether-a-number-is-a-power-of-2
    bool powerOfTwo(int32_t x) {
        return (!(x == 0) && !(x & (x - 1)));
    }
}

std::vector<Duople<std::string, std::string>> getCmdLineArguments(size_t argc, const char** argv, size_t indexOffset, char prefix) {
    std::vector<std::string> cmdArgument(argc);
    // start at index of 1 to avoid program name
    for (size_t i = 1; i < argc; i++) {
        cmdArgument.emplace_back(argv[i]);
    }

    std::vector<Duople<std::string, std::string>> optionAndValue;
    for (int loop = 0; const auto& arg : cmdArgument) {
        if (prefix == ' ' || arg[0] == prefix) {
            size_t index = indexOffset;
            std::string option, value;
            while (index < arg.size()-1 && arg[index] != ':') {
                option += arg[index++];
            }
            if (arg[index] == ':') {
                index++;
                do {
                    value += arg[index++];
                } while (index < arg.size());
            }
            optionAndValue.emplace_back(option, value);
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

    auto wordBuffer = Parser(path.string(), Parser::RecordNewLine).getWordBuffer();
    std::vector<Duople<std::string, std::string>> options;
    std::string option, value;
    bool foundOptionName = false;
    for (const auto& i : wordBuffer) {
        if (i.str == "\\n") {
            if (!option.empty())
                options.emplace_back(option, value);
            option.clear();
            value.clear();
            foundOptionName = false;
            continue;
        }
        else if (i.str == ":") {
            foundOptionName = true;
            continue;
        }

        if (!foundOptionName) {
            option += i.str;
        } else {
            value += stripQuotes(i.str);
        }

    }
    if (!option.empty() && !value.empty()) {
        options.emplace_back(option, value);
    }

    return options;
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
            std::cout << "Path: " << arg.two << "\n";
            baseDirectory = (std::filesystem::path(arg.two).is_absolute()) ? arg.two : std::filesystem::absolute(arg.two);
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
CompilerOptions::CompilerOptions(CompilerDirectory& directory) {
    linkDirectory(directory);
}

/*

CompilerDirectory::CompilerDirectory(std::filesystem::path path) {
    path = std::filesystem::absolute(path);
    if (path.empty())
        path = std::filesystem::current_path();
    if (!std::filesystem::exists(path) || path.empty())
        CLL_StdErr("Error Loading Path:", {"Path"}, {path.string()});


    Duople<bool, std::filesystem::path> confFile = { false, {} };
    for (const auto& file : std::filesystem::directory_iterator(path)) {
        if (file.path().filename() == "config.txt") {
            confFile.one = true;
            confFile.two = file.path();
        }
    }

    baseDirectory = confFile.two.parent_path();
    configFile = confFile.two;
}

*/

CompilerDirectory::CompilerDirectory(std::filesystem::path path) {
    if (!path.is_absolute())
        path = absolute(path);
    if (path.empty() || !exists(path))
        std::cerr << "Could Not Open Directory: '" << path.string() << "'\n";
    for (auto& file : std::filesystem::directory_iterator(path)) {
        std::cout << "[" << path << "]  " << file.path().string() << "\n";
        if (file.path().filename() == "config.txt") {
            configFile = file.path();
        }
    }

    if (configFile.empty())
        configFile = std::filesystem::current_path();
}