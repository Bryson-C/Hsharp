//
// Created by Owner on 8/14/2022.
//

#include "Tokenizer.h"

Tokenizer::Tokenizer(std::vector<Parser::ParsedString> parsedArray) {
    for (auto& i : parsedArray) {
        auto validTokenObject = parseStringToToken(i);
        if (validTokenObject.one) {
            m_Tokens.push_back({validTokenObject.two.token, validTokenObject.two.tokenData, i.filePosition});

        } else if (auto validDigit = isDigit(i.str); validDigit.one) {
            m_Tokens.push_back({MainToken::INTEGER, std::to_string(validDigit.two), i.filePosition});

        } else if (auto validString = isString(i.str); validString.one) {
            m_Tokens.push_back({MainToken::STRING, validString.two, i.filePosition});

        } else {
            m_Tokens.push_back({MainToken::NAMED, i.str, i.filePosition});
        }
    }
}
Tokenizer::Tokenizer(Parser parser) {
    auto buffer = parser.getWordBuffer();
    *this = Tokenizer(buffer);
}

std::vector<Tokenizer::Token> Tokenizer::getTokens() { return m_Tokens; };


std::vector<TokenGroup> GetTokenGroups(Tokenizer tokenizer) {
    auto tokenList = tokenizer.getTokens();

    std::vector<TokenGroup> statements;
    TokenGroup group;
    bool initialized = false, insideArgList = false;

    for (int i = 0; i < tokenList.size(); i++) {
        using TokenType = Tokenizer::MainToken;

        if (tokenList[i].token == TokenType::SEMICOLON) {
            statements.push_back(group);
            group = TokenGroup();
            initialized = false;
            continue;
        }

        if (tokenList[i].token == TokenType::EQUALS) { initialized = true; continue; }
        else if (tokenList[i].token == TokenType::COMMA) { continue; }

        if (tokenList[i].token == TokenType::OPEN_BRACKET) { if (!initialized) { insideArgList = true; group.isFunction = true; } continue; }
        if (tokenList[i].token == TokenType::CLOSE_BRACKET) { if (insideArgList) insideArgList = false; continue; }

        if (initialized)
            group.initializer.push_back(tokenList[i]);
        else if (insideArgList)
            group.arguments.push_back(tokenList[i]);
        else
            group.tokens.push_back(tokenList[i]);
    }
    if (!group.tokens.empty())
        statements.push_back(group);
    return statements;
}

