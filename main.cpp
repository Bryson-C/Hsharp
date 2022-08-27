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


enum class Token {
    Int,
    String,
    NewLine,
    Named,
};
Duople<bool, std::string> tokenToString(Token token) {
    switch (token) {
        case Token::Int: return {true, "Int"};
        case Token::String: return {true, "String"};
        case Token::NewLine: return {true, "\\n"};
        case Token::Named: return {true, "Custom_Named_String"};
    }
    return {false,""};
}
Duople<bool, Token> stringToToken(std::string str) {
    if (str == "Int") return {true, Token::Int};
    else if (str == "String") return {true, Token::String};
    else if (str == "\\n") return {true, Token::NewLine};
    // Cannot Be Converted To Therefore it must be left out
    // Token::Named
    return {false, {}};
}

int main() {

    std::ofstream output(R"(D:\Languages\CLL\output.c)", std::ios::trunc);

    Parser parser(R"(D:\Languages\CLL\Dynamic.lang)", Parser::Settings::RecordNewLine);

    DynamicTokenizer<Token> tokenizer;
    tokenizer.stringToToken = stringToToken;
    tokenizer.tokenToString = tokenToString;
    tokenizer.isVarType = {{Token::Int, Token::String}, Token::Named};
    tokenizer.run(parser);
    tokenizer.printTokens(true);



/*
    Tokenizer tokenizer(parser);
    DataStage dataStage(tokenizer);
    dataStage.write(output);
*/




    return 0;
}
