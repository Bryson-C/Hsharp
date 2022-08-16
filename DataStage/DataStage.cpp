//
// Created by Owner on 8/12/2022.
//

#include "DataStage.h"

DataStage::DataStage(std::vector<Tokenizer::Token> tokens) { run(tokens); }
DataStage::DataStage(Tokenizer tokenizer) { run(tokenizer); }
DataStage::~DataStage() {
    for (auto& var : scope.variables.variables) {
        var.print();
    }
    for (auto& func : scope.functions.functions) {
        func.print();
    }
}

bool DataStage::expects(std::vector<Tokenizer::Token> tokens, int index, std::vector<std::vector<Tokenizer::MainToken>> expected) {
    for (uint32_t iter = 0; auto& token : tokens) {
        if (!isAny(token.token, expected[iter])) return false;
    }
    return true;
}


void DataStage::run(std::vector<Tokenizer::Token> tokens) {
    using TokenType = Tokenizer::MainToken;
    for (int i = 0; i < tokens.size(); i++) {
        Variable var;
        Function func;
        if (isAny(tokens[i].token, {TokenType::INT_TYPE})) {
            // Checks If Vector Has
            // 1. A Type (Which It Should)
            // 2. A Name
            // 3. Ending Statement (Either `;` Or A Value)
            expects(tokens, i, {{TokenType::INT_TYPE}, {TokenType::NAMED}, {TokenType::SEMICOLON, TokenType::EQUALS, TokenType::OPEN_BRACKET}});

            var.type = tokens[i].token;
            func.type = tokens[i].token;
            i++;
            if (tokens[i].token != TokenType::NAMED)
                std::cerr << "Named String '" << tokens[i].tokenData << "' Does Not Comply With DataStage\n";
            var.name = tokens[i].tokenData;
            func.name = tokens[i].tokenData;
            i++;
            // A Semi-Colon Indicates That The Data Must Be A Uninitialized Variable
            if (tokens[i].token == TokenType::SEMICOLON) {
                var.initializer = {};
                scope.variables.newVariable(var);
                continue;
            }
            // An Equals Means That The Data Is An Initialized Variable
            else if (tokens[i].token == TokenType::EQUALS) {
                i++;
                std::vector<Tokenizer::Token> initializer;
                while (tokens[i].token != TokenType::SEMICOLON) {
                    if (tokens[i].token == TokenType::COMMA) {
                        i++;
                        continue;
                    } else if (Tokenizer::isOperation(tokens[i])) {
                        Tokenizer::Token left = (initializer.size() > 0) ? initializer[initializer.size()-1] : tokens[i-1], right = tokens[i+1], op = tokens[i];
                        auto result = PreformOperation({left}, {right}, op);
                        if (initializer.size() > 0) 
                        for (auto& res : result)
                            initializer.push_back(res);
                        i++;
                        continue;
                    }
                    if (!Tokenizer::isOperation(tokens[i+1]))
                        initializer.push_back(tokens[i]);
                    i++;
                }
                for (auto& val : initializer)
                    var.initializer.push_back(val.tokenData);
                scope.variables.newVariable(var);
                continue;
            }
            // A Bracket Indicates That The Data Must Be A Argument Array, Meaning A Function
            else if (tokens[i].token == TokenType::OPEN_BRACKET) {
                i++;
                // TODO: Implement
                scope.functions.newFunction(func);
            }
        }
    }
}
void DataStage::run(Tokenizer tokenizer) {
    run(tokenizer.getTokens());
}
