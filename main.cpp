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

    Parser parser(R"(D:\Languages\CLL\Dynamic.lang)");
    Tokenizer tokenizer(parser);

    auto tokenGroups = GetTokenGroups(tokenizer);
    for (auto& i : tokenGroups) i.printGroup();

    std::vector<CLL_EXP::Variable> variables;
    for (auto& i : tokenGroups) {

        CLL_EXP::Variable var;
        using TokenType = Tokenizer::MainToken;

        std::string name;
        bool variableDefinition = true;

        for (auto& tok : i.tokens) {
            if (tok.token == TokenType::INT_TYPE) {  var.setType(CLL_EXP::VariableType::INT32_TYPE); }
            if (tok.token == TokenType::STRING_TYPE) { var.setType(CLL_EXP::VariableType::STRING_TYPE); }
            if (tok.token == TokenType::NAMED) {
                for (auto& alreadyDefined : variables) {
                    if (alreadyDefined.getName() == tok.tokenData) {
                        tok.token = Tokenizer::MainToken::VARIABLE_CALL;
                        variableDefinition = false;
                    }
                }
                if (variableDefinition)
                    var.setName(tok.tokenData);
            }
            if (tok.token == TokenType::EQUALS) {}
            if (tok.token == TokenType::AUTO_TYPE) { var.setType(CLL_EXP::VariableType::AUTO); }
        }

        for (int iter = 0; auto &init: i.initializer) {
            CLL_EXP::Value val;

            if (init.token == TokenType::INTEGER) val = CLL_EXP::Value(std::stoi(init.tokenData));
            else if (init.token == TokenType::STRING) val = CLL_EXP::Value(init.tokenData);

            var.push(val);
            iter++;
        }

        if (variableDefinition) {
            output << var.generateOutput();
            variables.push_back(var);
        }
    }


    return 0;
}
