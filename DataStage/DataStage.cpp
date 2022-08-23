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
    bool LOG_UNHANDLED_RESULT = false;
    using TokenType = Tokenizer::MainToken;
    for (int i = 0; i < tokens.size(); i++) {
        Variable var;
        Function func;
        if (isAny(tokens[i].token, {TokenType::INT_TYPE})) {
            // Checks If Vector Has
            // 1. A Type (Which It Should)
            // 2. A Name
            // 3. Ending Statement (Either `;` Or A Value)
            //expects(tokens, i, {{TokenType::INT_TYPE}, {TokenType::NAMED}, {TokenType::SEMICOLON, TokenType::EQUALS, TokenType::OPEN_BRACKET}});

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
                bool usesBrackets = false;
                if (tokens[i+1].token == TokenType::OPEN_BRACKET) {
                    usesBrackets = true;
                    i++;
                } else if (tokens[i+1].token == TokenType::NAMED) {
                    i++;
                    usesBrackets = false;

                    auto functionResult = scope.functions.getFunction(tokens[i].tokenData);
                    auto variableResult = scope.variables.getVariable(tokens[i].tokenData);

                    // If It Is A Function
                    if (functionResult.one) {
                        //auto function = functionResult.two;
                        // I Dont Really Know What To Do Here (What To Do When Var Is Initialized With Function?)
                    } else {
                        std::cerr << tokens[i].filePosition.errorString() << "Absolutely No Function Of The Name '" << tokens[i].tokenData << "' Exists\n";
                    }

                }
                i++;

                std::vector<Tokenizer::Token> initializer;
                while ((usesBrackets)?(tokens[i].token != TokenType::CLOSE_BRACKET) : (tokens[i].token != TokenType::SEMICOLON)) {
                    if (tokens[i].token == TokenType::COMMA) {
                        if (!usesBrackets) std::cerr << tokens[i].filePosition.errorString() << "Arrays Need '[' And ']' For Open And Closing\n";
                        i++;
                        continue;
                    }

                    else if (Tokenizer::isOperation(tokens[i+1])) {
                        i++;
                        std::vector<int64_t> left, right;
                        Tokenizer::Token op = tokens[i];

                        if (tokens[i-1].token == TokenType::NAMED)
                            left = scope.variables.getVariable(tokens[i-1].tokenData).two.initializer;
                        else left = {std::stoll(tokens[i-1].tokenData)};

                        if (tokens[i+1].token == TokenType::NAMED)
                            right = scope.variables.getVariable(tokens[i+1].tokenData).two.initializer;
                        else
                            right = {std::stoll(tokens[i+1].tokenData)};


                        OperationStack stack;
                        stack.pushInt(left);
                        stack.pushInt(right);
                        auto opString = OperationStack::opFromString(op.tokenData);
                        auto opResult = stack.pushOp(opString);
                        if (opResult.success) {
                            for (auto& res : opResult.result)
                                push(var.initializer, res);
                        } else {
                            std::cerr << tokens[i].filePosition.errorString() << "Operation Error\n";
                            panic();
                        }
                        i+=2;
                        continue;
                    }
                    if (tokens[i].token == TokenType::INTEGER)
                        push(var.initializer, std::stoll(tokens[i].tokenData));
                    i++;
                }
                scope.variables.newVariable(var);
                continue;
            }

            // A Bracket Indicates That The Data Must Be A Argument Array, Meaning A Function
            else if (tokens[i].token == TokenType::OPEN_BRACKET) {
                i++;
                // Arguments
                std::vector<std::vector<Tokenizer::Token>> argTypes;
                std::vector<Tokenizer::Token> argNames;

                std::vector<Tokenizer::Token> currentArgTypes;

                // Get The Arguments
                while (tokens[i].token != TokenType::CLOSE_BRACKET) {
                    if (isAny(tokens[i].token, {TokenType::INT_TYPE, TokenType::STRING_TYPE})) {
                        currentArgTypes.push_back(tokens[i]);
                    }
                    else if (tokens[i].token == TokenType::OP_OTHER) {
                    }
                    else if (tokens[i].token == TokenType::NAMED) {
                        argTypes.push_back(currentArgTypes);
                        argNames.push_back(tokens[i]);
                        currentArgTypes.clear();
                    }
                    i++;
                }
                // Push The Arguments
                func.argTypes = argTypes;
                func.argNames = argNames;
                i++;
                // Get The Body Of The Function
                if (tokens[i].token == TokenType::OPEN_BRACE) {
                    i++;
                    while (tokens[i].token != TokenType::CLOSE_BRACE) {
                        func.body.push_back(tokens[i]);
                        i++;
                    }
                } else if (tokens[i].token == TokenType::BIG_ARROW) {
                    i++;
                    func.body.push_back({TokenType::RETURN, "return", tokens[i].filePosition});
                    while (tokens[i].token != TokenType::SEMICOLON) {
                        func.body.push_back(tokens[i]);
                        i++;
                    }
                    func.body.push_back(tokens[i]);
                }
                scope.functions.newFunction(func);
            }
        }
        else if (tokens[i].token == TokenType::NAMED) {
            // TODO: Found A Variable To Modify, Or Some Function Is Called
            auto function = scope.functions.getFunction(tokens[i].tokenData);
            auto variable = scope.variables.getVariable(tokens[i].tokenData);

            if (variable.one && function.one)
                std::cerr << tokens[i].filePosition.errorString()
                          << "Name Collision Of Function '" << function.two.name
                          << "' And Variable '" << variable.two.name << "'\n";
            else if (function.one) tokens[i].token = TokenType::FUNCTION_CALL;
            else if (variable.one) tokens[i].token = TokenType::VARIABLE_CALL;

            std::vector<Tokenizer::Token> stmt;
            do {
                stmt.push_back(tokens[i]);
                i++;
            } while (tokens[i].token != TokenType::SEMICOLON);
            stmt.push_back(tokens[i]);
            scope.statements.newStatement(stmt);
            i++;
        }
        else {
            if (LOG_UNHANDLED_RESULT)
                fprintf(stderr, "%s Unknown DataStage Case: '%s'\n", tokens[i].filePosition.errorString().c_str(), tokens[i].tokenData.c_str());
        }
    }
}
void DataStage::run(Tokenizer tokenizer) {
    run(tokenizer.getTokens());
}
