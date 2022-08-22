//
// Created by Owner on 8/12/2022.
//

#ifndef CLL_DATASTAGE_H
#define CLL_DATASTAGE_H

#include <iostream>
#include <vector>
#include <map>
#include <filesystem>

#include "../Parser/Parser.h"
#include "../Tokenizer/Tokenizer.h"
#include "Poperations.hpp"

struct Variable {
private:
public:
    std::string name;
    Tokenizer::MainToken type;

    std::vector<int64_t> initializer;


    inline void print() {
        printf("New Var: '%s' as '%s': (", name.c_str(), Tokenizer::tokenToString(type).c_str());
        for (int i = 0; i < initializer.size(); i++) {
            bool isBack = (i >= initializer.size()-1);
            std::cout << initializer[i] << ((!isBack) ? ", " : "");
        }
        printf(")\n");
    }
};
struct ScopedVariables {
private:
    uint32_t varCount = 0;
public:
    std::map<std::string, uint32_t> varIndex;
    std::vector<Variable> variables;

    void newVariable(std::string name, Tokenizer::MainToken type, std::vector<int64_t> initializer) {
        uint32_t index = varCount++;
        varIndex.emplace(name, index);
        variables.push_back({name, type, initializer});
    }
    void newVariable(Variable variable) {
        newVariable(variable.name, variable.type, variable.initializer);
    }
    Duople<bool,Variable> getVariable(std::string name) {
        if (!varIndex.contains(name)) return {false, {}};
        return {true, variables[varIndex[name]]};
    }
};

struct Function {
private:
public:
    std::string name;
    Tokenizer::MainToken type;
    std::vector<std::vector<Tokenizer::Token>> argTypes;
    std::vector<Tokenizer::Token> argNames;
    std::vector<Tokenizer::Token> body;
    inline void print() {
        printf("Found Function: '%s' as '%s'\n", name.c_str(), Tokenizer::tokenToString(type).c_str());
        for (int i = 0; i < argTypes.size(); i++) {
            printf("\t[With '%s' as ", argNames[i].tokenData.c_str());
            for (int j = 0; j < argTypes[i].size(); j++) {
                printf("'%s'%s", argTypes[i][j].tokenData.c_str(), (j != argTypes[i].size()-1) ? " or " : "");
            }
            printf("]\n");
        }
        printf("{\n");
        for (auto& i : body)
            printf("  %s  ", i.tokenData.c_str());
        printf("\n}\n");
    }
};
struct ScopedFunctions {
private:
    uint32_t funcCount = 0;
public:
    std::map<std::string, uint32_t> funcIndex;
    std::vector<Function> functions;

    void newFunction(std::string name, Tokenizer::MainToken type, std::vector<std::vector<Tokenizer::Token>> argTypes, std::vector<Tokenizer::Token> argNames, std::vector<Tokenizer::Token> body) {
        uint32_t index = funcCount++;
        funcIndex.emplace(name, index);
        functions.push_back({name, type, argTypes, argNames, body});
    }
    void newFunction(Function function) {
        newFunction(function.name, function.type, function.argTypes, function.argNames, function.body);
    }
    Duople<bool,Function> getFunction(std::string name) {
        if (!funcIndex.contains(name)) return {false, {}};
        return {true, functions[funcIndex[name]]};
    }
};

struct Statement {
    std::vector<Tokenizer::Token> statement;
    Statement(std::vector<Tokenizer::Token> statement) { for (auto& i : statement) statement.push_back(i); }
};
struct ScopedStatements {
    std::vector<Statement> statements;
    void newStatement(std::vector<Tokenizer::Token> statement) {
        statements.emplace_back(statement);
    }
    void newStatement(Statement statement) {
        statements.push_back(statement);
    }
};


struct Scope {
    ScopedFunctions functions;
    ScopedVariables variables;
    ScopedStatements statements;
    // Use If I Ever Get To This Point
    //Scope* childScope = nullptr;
};


inline std::vector<int64_t> GetValues(std::vector<Tokenizer::Token> tokens) {
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].token == Tokenizer::MainToken::OPEN_BRACKET) {
            while (tokens[i].token != Tokenizer::MainToken::CLOSE_BRACKET) {

            }
        }
    }
}

inline std::vector<int64_t> PreformOperation(std::vector<int64_t> left, std::vector<int64_t> right, Tokenizer::Token op) {

    using TokenType = Tokenizer::MainToken;
    std::vector<int64_t> result;
    if (left.size() != right.size()) std::cerr << "Miss-Matched Array Size For Operation\n";

    if (op.token == TokenType::OP_CONCAT) {
        for (auto& arrL : left)
            result.push_back(arrL);
        for (auto& arrR : right)
            result.push_back(arrR);
    } else {
        for (uint32_t iter = 0; auto &l: left) {
            auto &r = right[iter++];

            if (op.token == TokenType::OP_ADDITION) {
                int64_t res = l + r;
                printf("Operation: %lli %s %lli = %lli\n", l, "+", r, res);
                result.push_back(res);
            } else if (op.token == TokenType::OP_SUBTRACTION) {
                int64_t res = l - r;
                printf("Operation: %lli %s %lli = %lli\n", l, "-", r, res);
                result.push_back(res);
            } else if (op.token == TokenType::OP_DIVISION) {
                int64_t res = l / r;
                printf("Operation: %lli %s %lli = %lli\n", l, "/", r, res);
                result.push_back(res);
            } else if (op.token == TokenType::OP_MULTIPLICATION) {
                int64_t res = l * r;
                printf("Operation: %lli %s %lli = %lli\n", l, "x", r, res);
                result.push_back(res);
            } else if (op.token == TokenType::OP_MODULUS) {
                int64_t res = l % r;
                printf("Operation: %lli %s %lli = %lli\n", l, "%", r, res);
                result.push_back(res);
            } else if (op.token == TokenType::OP_EXPONENTIAL) {
                int64_t res = l;
                for (int64_t pow = r; r > 1; r--) res *= l;
                printf("Operation: %lli %s %lli = %lli\n", l, "^", r, res);
                result.push_back(res);
            } else if (op.token == TokenType::OP_RANGE) {
                int64_t res[] = {l, r};
                for (int64_t i = res[0]; (res[0] > res[1]) ? i >= res[1] : i <= res[1]; (res[0] > res[1]) ? i-- : i++) {
                    result.push_back(i);
                }
                printf("Operation: %lli %s %lli = ", l, "to", r);
                for (int i = 0; i < result.size(); i++)
                    printf("%lli%s", result[i], (i < result.size() - 1) ? ", " : "\n");
            }
        }
    }
    return result;
}
/*
inline Duople<bool, Function> expectFunction();
inline Duople<bool, Variable> expectVariable();
*/

class DataStage {
private:
    void run(std::vector<Tokenizer::Token> tokens);
    void run(Tokenizer tokenizer);

    bool expects(std::vector<Tokenizer::Token> tokens, int index, std::vector<std::vector<Tokenizer::MainToken>> expected);
public:
    Scope scope;

    DataStage(std::vector<Tokenizer::Token> tokens);
    DataStage(Tokenizer tokenizer);
    ~DataStage();

/*
    [[deprecated("Using Tokenization Now, Therefore Using Strings Is Not Helpful")]]inline void run(std::vector<std::string>& wordBuffer) {
        for (int i = 0; i < wordBuffer.size(); i++) {
            std::cout << wordBuffer[i] << "\n";
            if (wordBuffer[i] == "request") {
                i++;
                std::string pathStr;
                for (auto& chr : wordBuffer[i])
                    if (chr != '\"') pathStr += chr;

                // TODO: Following This Function, The First Character In The File Is Not Read

                Parser reader(pathStr);
                for (int iter = 0; auto& val : reader.getWordBuffer()) {
                    std::cout << "Inserting: " << val.str << "\n";
                    wordBuffer.insert(wordBuffer.begin()+i+(iter++), val.str);
                }
                continue;
            }
            else if (bVariableTypeCheck(wordBuffer[i])) {
                std::string type = wordBuffer[i++];
                std::string name = wordBuffer[i++];
                std::vector<std::string> initializer;
                std::vector<std::string> argTypes, argNames;
                std::vector<std::string> functionBody;
                if (wordBuffer[i] == "=") {
                    i++;
                    std::string buffer;
                    while (wordBuffer[i] != ";") {
                        if (wordBuffer[i] == ",") {
                            initializer.push_back(buffer);
                            buffer.clear();
                            i++;
                            continue;
                        } else if (bOperatorCheck(wordBuffer[i+1])) {
                            i++;
                            continue;
                        } else if (bOperatorCheck(wordBuffer[i])) {
                            auto result = CLL_PreformAutoOperation(variables, {wordBuffer[i-1]}, {wordBuffer[i+1]}, wordBuffer[i]);
                            for (auto& val : result.value)
                                initializer.push_back(std::to_string(val));
                            i+=2;
                            continue;
                        }
                        buffer += wordBuffer[i++];
                    }
                    if (!buffer.empty()) {
                        initializer.push_back(buffer);
                    }
                } else if (wordBuffer[i] == "[") {
                    i++;
                    while (wordBuffer[i] != "]") {
                        if (wordBuffer[i] == ",") { i++; continue; }
                        if (bVariableTypeCheck(wordBuffer[i])) {
                            argTypes.push_back(wordBuffer[i++]);
                            argNames.push_back(wordBuffer[i]);
                        }
                        i++;
                    }
                    i++;
                    if (wordBuffer[i] == "{") {
                        i++;
                        while (wordBuffer[i] != "}") {
                            functionBody.push_back(wordBuffer[i++]);
                        }
                    } else if (wordBuffer[i] == "=" && wordBuffer[i+1] == ">") {
                        i+=2;
                        functionBody.emplace_back("return");
                        while (wordBuffer[i] != ";") {
                            functionBody.push_back(wordBuffer[i++]);
                        }
                        functionBody.emplace_back(";");
                    }
                    functions.newFunction(name, CLL_StringToVarType(type), argTypes, argNames, functionBody);
                    fnWhenFunctionFound(name, type, argTypes, argNames, functionBody);
                    continue;
                }
                variables.newScopedVariable(name, CLL_StringToVarType(type), initializer);
                fnWhenVariableFound(name, type, initializer);
            }
        }
    }
*/


};


#endif //CLL_DATASTAGE_H
