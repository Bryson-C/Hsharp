#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "ctype.h"

#include "FileReader/FileReader.h"
#include "CLL.hpp"
#include "VariableHandler.hpp"

struct CLL_StringBuffer {
    std::vector<std::string> content;
    std::string buffer;

    void operator+=(char c) { buffer += c; }
    void operator+=(std::string str) { buffer += str; }
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
bool isOperator(std::string string) {
    if (string == "+") return true;
    if (string == "-") return true;
    if (string == "*") return true;
    if (string == "/") return true;
    if (string == "%") return true;
    if (string == "**") return true;
    if (string == "..") return true;
    return false;
}


std::vector<int32_t> parseExpression(int32_t left, int32_t right, std::string op) {
    std::vector<int32_t> integer;
    bool arrayOperation = false;
    // addition
    if (op == "+") {
        integer = {left+right};
    }
    // subtraction
    else if (op == "-") {
        integer = {left-right};
    }
    // multiplication
    else if (op == "*") {
        integer = {left * right};
    }
    // division
    else if (op == "/") {
        integer = {left / right};
    }
    // modulus
    else if (op == "%") {
        integer = {left % right};
    }
    // exponent
    else if (op == "**") {
        integer = {left};
        for (uint32_t i = 0; i < right; i++) integer[0] *= left;
    }
    // range
    else if(op == "..") {
        std::vector<int32_t> range;
        if (left < right) {
            for (int32_t i = left; i <= right; i++) range.push_back(i);
        } else if (right > left) {
            for (int32_t i = right; i >= left; i--) range.push_back(i);
        } else if (left == right){
            integer = {left};
        }
        integer = range;
    }
    for (int32_t index = 0; auto& val : integer) {
        if (val >= INT32_MAX)
            std::cerr << "Math Operation Hit The 32 Bit Upper Limit There For Cannot Give A Completely Accurate Result\n  Operation: " << op << "\n  Equation: " << left << op << right << "\n  @Index: " << index << "\n @: Unknown Location";
        if (val <= INT32_MIN)
            std::cerr << "Math Operation Hit The 32 Bit Lower Limit There For Cannot Give A Completely Accurate Result\n  Operation: "<< op << "\n  Equation: " << left << op << right << "\n  @Index: " << index << "\n  @: Unknown Location";
        index++;
    }
    return integer;
}

std::string vectorToString(std::vector<std::string>& vec, std::string splitStr) {
    std::string str;
    for (auto& i : vec) { str += i + ((i==vec[vec.size()-1])?"":(splitStr)); }
    return str;
}



#include <filesystem>

class Parser {
public:
    enum class type {
        IDENTIFIER,
        STRING,
        INTEGER,
        INITIALIZER,
        CONTAINER,
        SYMBOL,
        UNKNOWN,
    };
private:
    std::string content;
    std::vector<std::string> wordBuffer;
    std::vector<std::pair<uint32_t, type>> typeBuffer;
public:
    // Basic Initialization Of The Private Variables
    Parser() : wordBuffer({}), content("") {}
    // Reads The Given Path, Corrects The Path To Be Absolute
    Parser(const char* path) {
        // Correct Path To Be Absolute
        const char* correctedPath = std::filesystem::absolute(path).string().c_str();
        // Read File And Split The Words
        FileReader reader(correctedPath);
        content = reader.getChars();
        // Get The Words From The Word Buffer (Which Also Has The Line; Which Is Excluded)
        for (auto& wrd : reader.getWordBuffer()) wordBuffer.push_back(wrd.str);

        // TODO: Loop Through Word Buffer To Estimate The Type Of Data

    }
    // Checks The Previous And Next Words To Check Certain Parameters
    bool expects() {}
    //


    std::vector<std::string> getWordBuffer() { return wordBuffer; }
};



int main() {

    // EXP Start
    //Parser parser("data.lang");

    auto contained = CLL_GetContainedData("[{({})}]");
    return 0;


    // EXP End





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
            if (!varNames.contains(name)) {
                std::cerr << "Tried Obtaining A Variable Which Does Not Appear To Exist\n  Name: '" << name << "'\n  @: Unknown Location\n";
                return {false, "", "", {}};
            }
            return { true, name, varTypes[varNames[name]], varValues[varNames[name]] };
        }
        bool hasVariable(std::string name) {
            return varNames.contains(name);
        }
    };

    // Values To Be Used During Compilation
    std::vector<Macro> macros;

    CLL_ScopedVariables compileTime;
    /*
    compileTime compileTime;
    compileTime.newVariable("runtimeVersion", "Int", {"1"});
    compileTime.newVariable("runtimeName", "String", {"LongBow"});
     */


    for (int i = 0; i < wordBuffer.size(); i++) {
        // We Skip `;` Because It Does Not Add To The Program
        if (wordBuffer[i].str == ";") continue;
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
            std::string name;
            CLL_EVariableTypes type = CLL_StringToVarType(wordBuffer[i].str);
            i++;
            name = wordBuffer[i].str;
            i++;
            if (wordBuffer[i].str == ";") {
                compileTime.newScopedVariable(name, type, {});
                // We Are Done With This Variable
                continue;
            }
            if (wordBuffer[i].str == "=") {
                CLL_StringBuffer buffer;
                i++;
                while (wordBuffer[i].str != ";") {
                    if (type == CLL_EVariableTypes::String && isOperator(wordBuffer[i].str)) {
                        CLL_StdErr("String Operations Have Yet To Be Implemented", {CLL_StdLabels::Location}, {std::to_string(wordBuffer[i].line)});
                        //std::cerr << "String Operations Have Yet To Be Implemented\n  @: " << wordBuffer[i].line << "\n";
                        break;
                    }



                    if (wordBuffer[i].str == ",") {
                        buffer.push();
                        i++;
                        continue;
                    }
                    if (isOperator(wordBuffer[i+1].str)) {

                        std::string leftString = wordBuffer[i].str, rightSting = wordBuffer[i+2].str, OpString = wordBuffer[i+1].str;

                        auto result = CLL_PreformAutoOperation(compileTime, {leftString}, {rightSting}, OpString);
                        CLL_StdOut("Result Of Operation", {"Operation"}, {{leftString + " " + OpString + " " + rightSting}});
                        if (result.result != CLL_EOperationResult::Success) CLL_StdErr("Operation On Data Was Not Successful", {CLL_StdLabels::Cope}, {"Cannot Cope"});
                        for (auto& res : result.value) {
                            buffer.push(std::to_string(res));
                            std::cout << "  |: " << res << "\n";
                        }
                        i+=3;
                        continue;
                    }

                    if (compileTime.findVariable(wordBuffer[i].str)) {
                        auto var = compileTime.getScopedVarByName(wordBuffer[i].str);
                        if (var.variable.type != type)
                            std::cerr << "Variable Types Miss Match\n  Type Collision Between: " << name << "Of Type " << CLL_VarTypeToString(type) <<  "And" << var.variable.name << "Of Type " << CLL_VarTypeToString(var.variable.type) << "\n";
                        buffer += "{";
                        buffer += vectorToString(var.variable.data, ", ");
                        buffer += "}";
                        i++;
                        continue;
                    }


                    buffer += wordBuffer[i].str;
                    i++;
                }
                buffer.push();
                compileTime.newScopedVariable(name, type, buffer.content);
                // We Are Done With This Variable
                continue;
            }

        } else {
            auto var = compileTime.getScopedVarByName(wordBuffer[i].str);
            if (var.result == CLL_EVariableHandlerResult::VariablePresent) {
                i++;
                if (wordBuffer[i].str == "?") {
                    CLL_StdOut("Variable Requested To Be Printed", {CLL_StdLabels::Variable},{var.variable.name});
                    if (!var.variable.data.empty()) {
                        for (auto &varval: var.variable.data)
                            std::cout << "  |: " << varval << "\n";
                    }
                    else {
                        CLL_StdErr("Variable Has No Values Therefore Will Not Be Printed",{CLL_StdLabels::Data}, {var.variable.name});
                    }
                }
            } else {
                CLL_StdErr("Unknown Error", {CLL_StdLabels::Offender, CLL_StdLabels::Location}, {wordBuffer[i].str, std::to_string(wordBuffer[i].line)});
            }
        }


    }


    return 0;
}
