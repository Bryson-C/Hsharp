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

        TokenType type;
        std::string name;
        std::string str;

        for (auto& tok : i.tokens) {
            if (tok.token == TokenType::INT_TYPE) { str += "int "; type = TokenType::INT_TYPE; }
            if (tok.token == TokenType::STRING_TYPE) { str += "const char* "; type = TokenType::STRING_TYPE;}
            if (tok.token == TokenType::NAMED) { str += (name = tok.tokenData); }
            if (tok.token == TokenType::EQUALS) i.initializer.size() > 1 ? str += "[] = " : str += " = ";
            if (tok.token == TokenType::AUTO_TYPE) {
                if (!i.initializer.empty()) {
                    auto digit = isDigit(i.initializer[0].tokenData);
                    auto string = isString(i.initializer[0].tokenData);

                    if (digit.one)
                        type = TokenType::INT_TYPE;
                    else if (string.one)
                        type = TokenType::STRING_TYPE;
                    else
                        type = TokenType :: AUTO_TYPE;

                    switch (type) {
                        case TokenType::INT_TYPE: str = "int " + str; break;
                        case TokenType::STRING_TYPE: str = "const char* " + str; break;
                        default: str += "auto "; break;
                    }
                }
            }
        }

        if (i.initializer.empty())
            str += "/* Unused Variable*/";

        else if (i.initializer.size() == 1)
            str += i.initializer[0].tokenData;

        else if (i.initializer.size() > 1) {
            str += "{";
            for (int iter = 0; auto &init: i.initializer) {

                if (type == TokenType::INT_TYPE && init.token == TokenType::INTEGER || type == TokenType::STRING_TYPE && init.token == TokenType::STRING)
                        str += init.tokenData;
                else
                    std::cerr << "Type Conflict From: '" << init.tokenData << "' :\n\tInitializer Type: " << Tokenizer::tokenToString(type) << "  |  Value Type: " << Tokenizer::tokenToString(init.token) << "\n";


                if (iter < i.initializer.size()-1) str += ", ";
                iter++;
            }
            str += "}";
        }

        str += ";\n";
        output << str;
    }


    return 0;
}
