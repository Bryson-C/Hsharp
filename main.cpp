#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <ctype.h>

#include "Parser/Parser.h"
#include "Tokenizer/Tokenizer.h"
#include "CLL.hpp"
#include "VariableHandler.hpp"

struct CLL_StringBuffer {
    std::vector<std::string> content;
    std::string buffer;

    std::string operator[](size_t index) { return content[index]; }

    void operator+=(char c) { buffer += c; }
    void operator+=(std::string str) { buffer += str; }

    void push() { if (!buffer.empty()) { content.push_back(buffer); buffer.clear(); } }
    void push(std::string str) { if (!str.empty()) { content.push_back(str); } }
    template<typename T>
    void push(std::vector<T> vec) { for (auto& str : vec) content.push_back(std::to_string(str)); }
};

// String Collectors:
std::vector<std::string> splitString(std::string& string, std::string pattern) {
    CLL_StringBuffer buffer;
    for (uint32_t i = 0; i < string.size(); i++) {
        for (int chr = 0, matches = 0, j = i; chr < pattern.size(); chr++, j++) {
            if (string[j] == pattern[chr]) matches++;
            if (matches == pattern.size()) buffer.push();
        }
        buffer += string[i];
    }
    buffer.push();
    return buffer.content;
}
std::vector<std::string> splitString(std::string& string, bool (*func)(char)) {
    CLL_StringBuffer buffer;

    int chr = 0;
    do {
        if (func(string[chr])) buffer.push();
        buffer.buffer += string[chr++];
    } while(chr < string.size());

    return buffer.content;
}
std::vector<std::string> removeWhiteSpace(std::vector<std::string>& strings) {
    for (int wrdIndex = 0; auto& wrd : strings) {
        if (wrd.empty()) { strings.erase(strings.begin()+wrdIndex); wrdIndex++; continue; }
        for (int index = 0; auto& chr : wrd) { if (isspace(chr)) wrd.erase(wrd.begin()+index); index++; }
        wrdIndex++;
    }
    return strings;
}


bool isVarType(std::string str) {
    return isAny(str, {"Int"});
}

#include "DataStage/DataStage.h"


int main() {


    Parser parser(R"(D:\Languages\CLL\data.lang)"/*, Parser::Settings::RecordNewLine*/);
    std::ofstream output(R"(D:\Languages\CLL\output.c)", std::ios::trunc);

    Tokenizer tokenizer(parser);
    DataStage dataStage(tokenizer);
    dataStage.write(output);

    //Generator generator(output, dataStage);

    return 0;
}
