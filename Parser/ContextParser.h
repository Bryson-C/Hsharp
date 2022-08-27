//
// Created by Owner on 8/26/2022.
//

#ifndef CLL_CONTEXTPARSER_H
#define CLL_CONTEXTPARSER_H

#include <vector>

#include "../Tokenizer/Tokenizer.h"

struct ParsedArguments {
    ParsedArguments() {}
    ParsedArguments(std::string name, std::string type) : name(name), type(type) {}
    ParsedArguments(std::string name, Tokenizer::MainToken type) : name(name), type(type) {}
    ParsedArguments(std::string name, Tokenizer::Token type) : name(name), type(type.token) {}
    std::string name;
    ParsedType type;
};

enum class ExpressionType {
    Variable,
    Function,
    VariableCall,
    FunctionCall,
    VariableModify,
};

struct Expression {
    ExpressionType exprType;
    std::string name;
    ParsedType type;
    std::vector<ParsedArguments> args;
    std::vector<Tokenizer::Token> functionBody;
    std::vector<int64_t> variableInitializer;

    void setBody(std::vector<Tokenizer::Token>& body) {
        functionBody = body;
    }
    void setBody(std::vector<int64_t>& initializer) {
        variableInitializer = initializer;
    }
};


class ContextParser {
    ContextParser(Tokenizer tokenizer);
    ContextParser(std::vector<Tokenizer::Token> tokenList);
};


#endif //CLL_CONTEXTPARSER_H
