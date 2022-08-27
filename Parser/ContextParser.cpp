//
// Created by Owner on 8/26/2022.
//

#include "ContextParser.h"


ContextParser::ContextParser(std::vector<Tokenizer::Token> tokenList) {
    for (uint32_t i = 0; i < tokenList.size(); i++) {
        if (Tokenizer::isVariableType(tokenList[i])) {
            Expression expr;
            expr.type = ParsedType(tokenList[i++].token);
            expr.name = tokenList[i++].tokenData;
            if (tokenList[i].token == Tokenizer::MainToken::SEMICOLON) { expr.exprType = ExpressionType::Variable; }
            else if (tokenList[i].token == Tokenizer::MainToken::EQUALS) {
                i++;
            }// else if (tokenList[i].)
        }
    }
}
ContextParser::ContextParser(Tokenizer tokenizer) {

}
