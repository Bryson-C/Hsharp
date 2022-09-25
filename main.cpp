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


    for (auto& i : tokenGroups) {
        using TokenType = Tokenizer::MainToken;

        TokenType type;
        std::string str;

        for (auto& tok : i.tokens) {
            if (tok.token == TokenType::INT_TYPE) { str += "int "; type = TokenType::INT_TYPE; }
            if (tok.token == TokenType::STRING_TYPE) { str += "const char* "; type = TokenType::STRING_TYPE;}
            if (tok.token == TokenType::NAMED) str += tok.tokenData + " ";
            if (tok.token == TokenType::EQUALS) str += "= ";
            if (tok.token == TokenType::AUTO_TYPE) type = TokenType::AUTO_TYPE;
        }
        std::vector<std::string> initializer;
        for (auto& init : i.initializer) {
            if (auto digit = isDigit(init.tokenData); digit.one && type == TokenType::AUTO_TYPE) { str = "int " + str; type = TokenType::INT_TYPE; }

            if (type == TokenType::INT_TYPE && init.token == TokenType::INTEGER) { initializer.push_back(init.tokenData); }
            else { std::cerr << "Type Conflict From: '" << init.tokenData << "' :\n\tInitializer Type: " << Tokenizer::tokenToString(type) << "  |  Value Type: " << Tokenizer::tokenToString(init.token) << "\n"; }

            if (type == TokenType::STRING_TYPE && init.token == TokenType::STRING) { initializer.push_back(init.tokenData); }
            else { std::cerr << "Type Conflict From: '" << init.tokenData << "' :\n\tInitializer Type: " << Tokenizer::tokenToString(type) << "  |  Value Type: " << Tokenizer::tokenToString(init.token) << "\n"; }
        }
        for (auto& )
        str += ";\n";
        output << str;
    }


    return 0;
}
