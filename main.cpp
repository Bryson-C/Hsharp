#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "ctype.h"

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

/*
std::string generateVariable(std::string name, std::string type, std::vector<std::string> initializer) {
    std::string output;
    output += type + " ";
    output += name;
    output += "[" + std::to_string(initializer.size()) + "] = ";
    output += "{";
    for (auto& i : initializer) {
        output += i + ((i != initializer[initializer.size()-1])?", ":"");
    }
    output += "};\n";
    return output;
}
std::string generateVariable(CLL_Variable variable) {
    return generateVariable(variable.name, CLL_VarTypeToCType(variable.type), variable.data);
}
std::string generateFunction(std::string name, std::string type, std::vector<std::string> argTypes, std::vector<std::string> argNames, std::vector<std::string> body) {
    std::string output;
    output += type + " ";
    output += name + "(";
    for (int iter = 0; auto& i : argTypes) {
        output += CLL_VarTypeToCType(CLL_StringToVarType(i)) + " " + argNames[iter];
        if (iter < argTypes.size()-1) output += ", ";
        iter++;
    }
    output += ")";
    if (!body.empty()) {
        // TODO: Implement Function Body
        output += ";\n";
    } else {
        output += ";\n";
    }
    return output;
}
std::string generateFunction(CLL_Function function) {
    return generateFunction(function.name, CLL_VarTypeToCType(function.type), function.argTypes, function.argNames, function.body);
}


void FoundFunction(std::string name, std::string type, std::vector<std::string> argTypes, std::vector<std::string> argNames, std::vector<std::string> body) {
    printf("Found Function: '%s' as '%s'\n", name.c_str(), type.c_str());
    for (int i = 0; i < argTypes.size(); i++)
        printf("\tWith '%s' as '%s'\n", argNames[i].c_str(), argTypes[i].c_str());
    printf("{\n");
    for (auto& i : body)
        printf("  %s  ", i.c_str());
    printf("\n}\n");
}
void FoundVariable(std::string name, std::string type, std::vector<std::string> initializer) {
    printf("New Var: '%s' as '%s': (", name.c_str(), type.c_str());
    for (auto& i : initializer)
        std::cout << i << ((i != initializer.back()) ? ", " : ")\n");
}
*/
int main() {

    Parser parser(R"(D:\Languages\CLL\data.lang)", Parser::Settings::RecordNewLine);
    Tokenizer tokenizer(parser);
    DataStage dataStage(tokenizer);



    /*std::vector<std::string> strBuffer;
    for (auto& wrd : wordBuffer) strBuffer.push_back(wrd.str);

    DataStage dataStage;
    dataStage.bVariableTypeCheck = isVarType;
    dataStage.fnWhenFunctionFound = FoundFunction;
    dataStage.fnWhenVariableFound = FoundVariable;
    dataStage.bOperatorCheck = isOperator;
    dataStage.run(strBuffer);

    std::ofstream file("D:\\Languages\\CLL\\output.c", std::ios::trunc);
    for (auto& var : dataStage.variables.vars) {
        file << generateVariable(var);
    }
    for (auto& func : dataStage.functions.functions) {
        file << generateFunction(func);
    }
    file.close();
*/
    /*




    // Values To Be Used During Compilation
    CLL_ScopedVariables compileTime;


    for (int i = 0; i < wordBuffer.size(); i++) {
        // We Skip `;` Because It Does Not Add To The Program
        if (wordBuffer[i].str == ";") continue;

        if (wordBuffer[i].str == "!!") {
            i++;
            int64_t shouldLog = false;

            auto var = CLL_GetVariableOptionally<int64_t>(compileTime, wordBuffer[i].str, [](std::string str){ return std::stoll(str); });

            if (var.result == CLL_EVariableHandlerResult::VariablePresent) {
                shouldLog = std::stoll(var.variable.data[0]);
                i++;
            } else if (isDigit(wordBuffer[i].str)) {
                shouldLog = std::stoll(wordBuffer[i].str);
                i++;

            }

            if (shouldLog) {
                std::cerr << "User Defined Error:\n\t";
                // This Function Requires The Parser To Correctly Parse Strings As A String
                std::cerr << wordBuffer[i].str << "\n";
            }

            i++;
            continue;
        }
        else if (wordBuffer[i].str == "??") {
            i++;
            int64_t shouldLog = false;

            auto var = CLL_GetVariableOptionally<int64_t>(compileTime, wordBuffer[i].str, [](std::string str){ return std::stoll(str); });

            if (var.result == CLL_EVariableHandlerResult::VariablePresent) {
                shouldLog = std::stoll(var.variable.data[0]);
                i++;
            } else if (isDigit(wordBuffer[i].str)) {
                shouldLog = std::stoll(wordBuffer[i].str);
                i++;

            }

            if (shouldLog) {
                std::cout << "User Defined Warning:\n\t";
                // This Function Requires The Parser To Correctly Parse Strings As A String
                std::cout << wordBuffer[i].str << "\n";
            }

            i++;
            continue;
        }

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
                        break;
                    }

                    if (wordBuffer[i].str == ",") {
                        buffer.push();
                        i++;
                        continue;
                    }

                    if (isOperator(wordBuffer[i].str)) {

                        std::string leftString = wordBuffer[i-1].str,
                                    rightSting = wordBuffer[i+1].str,
                                    opString = wordBuffer[i].str;

                        i+=2;

                        auto result = CLL_PreformAutoOperation(compileTime, {leftString}, {rightSting}, opString);
                        CLL_StdOut("Result Of Operation", {"Operation"}, {{leftString + " " + opString + " " + rightSting}});
                        if (result.result != CLL_EOperationResult::Success) CLL_StdErr("Operation On Data Was Not Successful", {CLL_StdLabels::Cope}, {"Cannot Cope"});
                        for (auto& res : result.value) {
                            buffer.push(std::to_string(res));
                            std::cout << "  |: " << res << "\n";
                        }
                        continue;
                    }


                    CLL_VariableResult<std::string> variable = CLL_GetVariableOptionally(compileTime, wordBuffer[i].str);
                    if (variable.result == CLL_EVariableHandlerResult::VariablePresent) {
                        if (variable.variable.type != type)
                            CLL_StdOut("Variable Types Miss Match", {"Type Collision"},{{name + "Of Type " + CLL_VarTypeToString(type) + " And " + variable.variable.name + "Of Type " + CLL_VarTypeToString(variable.variable.type)}});

                        // TODO: This IF Statement Is For Getting Indices, I Still Need To Apply Single To Single Operations On Them
                        i++;
                        if (wordBuffer[i].str == "[") {
                            std::vector<size_t> indices;
                            // This Is To Skip The First `[`
                            i++;
                            while (wordBuffer[i].str != "]") {
                                if (isOperator(wordBuffer[i+1])) {
                                    auto result = CLL_PreformAutoOperation(compileTime, {wordBuffer[i]}, {wordBuffer[i+2]}, wordBuffer[i+1]);
                                    for (auto& resultIndex : result.value) {
                                        indices.push_back(resultIndex);
                                    }
                                    // We Only Need To Add 2 To The Iteration Because Once This If Statement Exits, The Iteration Will Go Up By 1
                                    i+=2;
                                } else if (wordBuffer[i].str == ",") {
                                    i++;
                                    continue;
                                } else if (isDigit(wordBuffer[i].str)) {
                                    indices.push_back(std::stoll(wordBuffer[i]));
                                } else
                                    CLL_StdErr("Unhandled Index Error", {CLL_StdLabels::Offender}, {wordBuffer[i].str});
                                i++;
                            }
                            // This Is To Skip The Second `]`
                            i++;
                            for (auto& index : indices) {
                                std::string pushedData;
                                if (index >= variable.variable.data.size() || index < 0) {
                                    CLL_StdErr("Index Is Out Of Bounds", {CLL_StdLabels::Offender, CLL_StdLabels::Index, CLL_StdLabels::Cope}, {variable.variable.name, std::to_string(index), "Using Int-64 Minimum"});
                                    pushedData = std::to_string(INT64_MIN);
                                } else {
                                    pushedData = variable.variable.data[index];
                                }
                                buffer.push(pushedData);
                            }
                            continue;
                        } else {
                            for (auto& vData : variable.variable.data)
                                buffer.push(vData);
                            i++;
                            continue;
                        }
                    }

                    buffer += wordBuffer[i].str;
                    i++;
                }
                buffer.push();
                compileTime.newScopedVariable(name, type, buffer.content);
                // We Are Done With This Variable
                continue;
            }

        } else if (auto var = CLL_GetVariableOptionally(compileTime, wordBuffer[i].str); var.result == CLL_EVariableHandlerResult::VariablePresent) {
            i++;
            if (wordBuffer[i].str == "?") {
                CLL_StdOut("Variable Requested To Be Printed", {CLL_StdLabels::Variable},{var.variable.name});
                if (!var.variable.data.empty()) {
                    for (auto &varval: var.variable.data) {
                        std::cout << "  |: " << varval << "\n";
                    }
                }
                else {
                    CLL_StdErr("Variable Has No Values Therefore Will Not Be Printed",{CLL_StdLabels::OffendingVariable}, {var.variable.name});
                }
            }
        }
        else {
            CLL_StringBuffer scope;
            while (wordBuffer[i].str != ";") {
                if (isAny(wordBuffer[i].str, {",","(",")"})) {
                    scope.push();
                    i++;
                    continue;
                }
                if (isOperator(wordBuffer[i].str)) {
                    scope.push();
                    scope.push(wordBuffer[i].str);
                    i++;
                    continue;
                }
                scope += wordBuffer[i].str;
                i++;
            }
            scope.push();

            for (auto& val : scope.content) {
                std::cout << val << ", \n";
            }
            break;
            //The Code Is Not A Variable
            CLL_StdErr("Unknown Error", {CLL_StdLabels::Offender, CLL_StdLabels::Location}, {wordBuffer[i].str, std::to_string(wordBuffer[i].line)});
        }


    }
    */

    return 0;
}
