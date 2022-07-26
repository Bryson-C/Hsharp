#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "ctype.h"

// Does Require File Reader
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


bool expects(std::vector<std::string>& vector, int index, std::vector<std::string> substr) {
    for (auto& i : vector)
        if (i != substr[0]) return false;
    return true;
}
bool isVarType(std::string string) {
    if (string == "Int") return true;
    return false;
}
bool isKeyword(std::string string) {
    if (isVarType(string)) return true;
    else if (string == "=") return true;
    return false;
}
bool isDigit(std::string string) {
    for (auto& i : string) if (!isdigit(i)) return false;
    return true;
}
bool isOperator(std::string string) {
    if (string == "+") return true;
    if (string == "-") return true;
    if (string == "*") return true;
    if (string == "/") return true;
    if (string == "%") return true;
    if (string == "**") return true;
    return false;
}


int32_t parseExpression(int32_t left, int32_t right, std::string op) {
    int32_t integer;
    // addition
    if (op == "+") {
        return left+right;
    }
    // subtraction
    else if (op == "-") {
        return left-right;
    }
    // multiplication
    else if (op == "*") {
        return left*right;
    }
    // division
    else if (op == "/") {
        return left/right;
    }
    // modulus
    else if (op == "%") {
        return left%right;
    }
    // exponent
    else if (op == "**") {
        integer = left;
        for (uint32_t i = 0; i < right; i++) integer *= left;
    }
}



int main() {

    FileReader reader("D:\\Languages\\CLL\\data.lang");
    auto wordBuffer = reader.getWordBuffer();

    struct Macro {
        using strvec = std::vector<std::string>;
        std::string activator;
        strvec format;
        strvec args;
    };
    struct compileTime {
        uint32_t varCount = 0;
        std::map<std::string, uint32_t> varNames;
        std::vector<std::string> varTypes;
        std::vector<std::vector<std::string>> varValues;
        void newVariable(std::string name, std::string type, std::vector<std::string> value) {
            std::cout << "Variable Added To Compile Time Data:\n  |: " << type << " " << name << "\n";
            std::cout << "  |: Data\n";
            for (auto& i : value) std::cout << "    |: " << i << "\n";
            varNames.emplace(name, varCount++);
            varValues.push_back(value);
            varTypes.push_back(type);
        }
        struct compileTimeResult {
            bool hasVariable;
            std::string name;
            std::string type;
            std::vector<std::string> varValues;
        };
        compileTimeResult getVariable(std::string name) {
            if (varNames.contains(name)) {
                std::cerr << "Tried Obtaining A Variable Which Does Not Appear To Exist\n";
                return {false, "", "", {}};
            }
            return { true, name, varTypes[varNames[name]], varValues[varNames[name]] };
        }
    };

    // Values To Be Used During Compilation
    std::vector<Macro> macros;
    compileTime compileTime;


    for (int i = 0; i < wordBuffer.size(); i++) {
        if (wordBuffer[i].str == "#") {
            Macro mc;
            i++;
            mc.activator = wordBuffer[i];
            std::cout << "New Macro: " << mc.activator << "\n";
            i++;
            if (wordBuffer[i].str == "[") {
                i++;
                while (wordBuffer[i].str != "]") mc.args.push_back(wordBuffer[i++].str);
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
                macros.push_back(mc);
            } else {
                std::cerr << "Macro Expects That Following A Argument List The `=>` Operator Is Present\n";
            }
        }
        // TODO: Activate Macros

        if (isVarType(wordBuffer[i].str)) {
            std::string name, type;
            type = wordBuffer[i].str;
            i++;
            name = wordBuffer[i].str;
            i++;
            if (wordBuffer[i].str == ";") {
                compileTime.newVariable(name, type, {});
                // We Are Done With This Variable
                continue;
            }
            if (wordBuffer[i].str == "=") {
                std::vector<std::string> values;
                std::string strBuffer;
                std::string buffer;
                while (wordBuffer[i].str != ";") {
                    if (wordBuffer[i].str == "\"" && !strBuffer.empty()) {
                        i++;
                        values.push_back(strBuffer);
                        strBuffer.clear();
                        continue;
                    }
                    if (type == "Int" && isOperator(wordBuffer[i].str)) {
                        // TODO: Figure Out If I Want 32 Bit Or 64 Bit Integers
                        // std::stoll Should Count For 64 Bit For Future Use
                        int64_t left = std::stoll(wordBuffer[i-1].str);
                        int64_t right = std::stoll(wordBuffer[i+1].str);
                        std::string op = wordBuffer[i].str;
                        parseExpression(left, right, op);
                        i++;
                    } else if (type == "string" && isOperator(wordBuffer[i].str)) {
                        std::cerr << "String Operations Have Yet To Be Implemented\n  @: " << wordBuffer[i].line << "\n";
                    }
                    if (wordBuffer[i].str == ",") {
                        values.push_back(wordBuffer[i].str);
                        i++;
                        continue;
                    }
                    buffer += wordBuffer[i].str;
                    i++;
                }
                if (!buffer.empty()) values.push_back(buffer);
                compileTime.newVariable(name, type, values);
                // We Are Done With This Variable
                continue;
            }

        } else {
            auto variable = compileTime.getVariable(wordBuffer[i]);
            if (variable.hasVariable) {
                std::cout << "Found Variable!\n";
                i++;
                if (wordBuffer[i].str == "?") {
                    std::cout << "Variable Requested To Be Printed\n  |: ";
                    if (!variable.varValues.empty()) {
                        for (auto &varval: variable.varValues)
                            std::cout << varval << "\n  |  ";
                    }
                    else {
                        std::cerr << "Variable Has No Values Therefore Will Not Be Printed\n";
                    }
                }

            } else {
                std::cerr << "Unknown Error\n  @: " << wordBuffer[i].line << "\n";
            }
        }





    }


    return 0;
}
