#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <ctype.h>

#include "Parser/Parser.h"
#include "Tokenizer/Tokenizer.h"
#include "CLL.hpp"
#include "VariableHandler.hpp"
#include "DataStage/DataStage.h"


int main() {

    std::ofstream output(R"(D:\Languages\CLL\output.c)", std::ios::trunc);

    Parser parser(R"(D:\Languages\CLL\default.lang)");
    Tokenizer tokenizer(parser);

    auto tokenGroups = GetTokenGroups(tokenizer);
    for (auto& i : tokenGroups) i.printGroup();

    std::vector<CLL_EXP::Variable> variables;
    std::vector<CLL_EXP::Function> functions;
    for (auto& i : tokenGroups) {

        CLL_EXP::Variable var;
        CLL_EXP::Function func;
        using TokenType = Tokenizer::MainToken;

        std::string name;
        bool namedDefinition = true;

        for (int iteration = 0; auto& tok : i.getTokens()) {
            if (tok.token == TokenType::INT_TYPE)
                {
                    var.setType(CLL_EXP::VariableType::INT32_TYPE);
                    func.setType(CLL_EXP::VariableType::INT32_TYPE);
                }
            if (tok.token == TokenType::STRING_TYPE)
                {
                    var.setType(CLL_EXP::VariableType::STRING_TYPE);
                    func.setType(CLL_EXP::VariableType::STRING_TYPE);
                }
            if (tok.token == TokenType::NAMED) {
                for (auto& alreadyDefined : variables) {
                    if (alreadyDefined.getName() == tok.tokenData) {
                        tok.token = Tokenizer::MainToken::VARIABLE_CALL;
                        namedDefinition = false;
                    }
                }
                for (auto& alreadyDefined : functions) {
                    if (alreadyDefined.getName() == tok.tokenData) {
                        tok.token = Tokenizer::MainToken::FUNCTION_CALL;
                        namedDefinition = false;
                    }
                }
                if (namedDefinition)
                    {
                        var.setName(tok.tokenData);
                        func.setName(tok.tokenData);
                    }
            }
            if (tok.token == TokenType::AUTO_TYPE)
                {
                    var.setType(CLL_EXP::VariableType::AUTO);
                    func.setType(CLL_EXP::VariableType::AUTO);
                }

            iteration++;
        }
        CLL_EXP::Variable argument;
        for (auto& arg : i.getArguments()) {
            if (arg.token == TokenType::INT_TYPE)
                {
                    argument.setType(CLL_EXP::VariableType::INT32_TYPE);
                }
            else if (arg.token == TokenType::STRING_TYPE)
                {
                    argument.setType(CLL_EXP::VariableType::STRING_TYPE);
                }
            else if (arg.token == TokenType::NAMED)
                argument.setName(arg.tokenData);
            else if (arg.token == TokenType::COMMA) {
                func.pushArgument(argument);
                argument = CLL_EXP::Variable();
            }
        }
        if (argument.getType() != CLL_EXP::VariableType::NONE) {
            func.pushArgument(argument);
            argument = CLL_EXP::Variable();
        }

        for (int iter = 0; auto &init: i.getInitializer()) {
            CLL_EXP::Value val;

            if (init.token == TokenType::INTEGER) val = CLL_EXP::Value(std::stoi(init.tokenData));
            else if (init.token == TokenType::STRING) val = CLL_EXP::Value(init.tokenData);

            var.push(val);
            iter++;
        }

        if (namedDefinition && !i.isFunctionType()) {
            output << var.generateOutput();
            variables.push_back(var);
        } else if (namedDefinition && i.isFunctionType()) {
            output << func.generateOutput();
            functions.push_back(func);
        }
    }


    return 0;
}
