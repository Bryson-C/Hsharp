#include <iostream>
#include <vector>
#include <string>
#include "ctype.h"

#include "FileReader/FileReader.h"

struct CLL_StringBuffer {
    std::vector<std::string> content;
    std::string buffer;

    void operator+=(char c) { buffer += c; }
    void push() { if (!buffer.empty()) { content.push_back(buffer); buffer.clear(); } }
    void push(std::string str) { if (!str.empty()) { content.push_back(str); } }
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


struct Symbol {};


int main() {

    FileReader reader("D:\\Languages\\CLL\\data.lang");
    auto wordBuffer = reader.getWordBuffer();

    struct Macro {
        std::string activator;
        std::vector<std::string> format;
        std::string args;
    };

    for (auto& i : wordBuffer) std::cout << i.str << "\n";


    std::vector<Macro> macros;
    for (int i = 0; i < wordBuffer.size(); i++) {
        if (wordBuffer[i].str == "#") {
            Macro mc;
            i++;
            mc.activator = wordBuffer[i];
            i++;
            if (wordBuffer[i].str == "[") {
                i++;
                while (wordBuffer[i].str != "]") mc.args += wordBuffer[i++].str;
                i++;
            } else {
                std::cerr << "Macro Requires Argument List After Activator\n";
            }

            if (wordBuffer[i].str == "=" && wordBuffer[i+1].str == ">") {
                i+=2;
                while (wordBuffer[i].str != ";") {
                    mc.format.push_back(wordBuffer[i++].str);
                }
                mc.format.push_back(";");
                i++;
                macros.push_back(mc);
            } else {
                std::cerr << "Macro Expects That Following A Argument List The `=>` Operator Is Present\n";
            }
        }

        for (auto& mc : macros) {
            if (wordBuffer[i].str == mc.activator) {
                printf("Activating Macro: %s\n", mc.activator.c_str());
                std::string buffer;
                // Skip Activator
                i++;
                for (int index = 0; index < mc.format.size(); index++) {
                    // TODO: If A Macro Argument Is A Symbol, Make It Required To Be That Symbol
                    // TODO: If A Macro Argument Is A _, Make It Be Ignored To The Compiler
                    if (mc.format[index] == "$") {
                        buffer += wordBuffer[i].str + " ";
                        i++;
                    } else {
                        buffer += mc.format[index] + " ";
                    }
                }
                printf("Buffer: %s\n", buffer.c_str());
            }
        }
    }


    return 0;
}
