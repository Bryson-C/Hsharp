//
// Created by Owner on 8/14/2022.
//

#include "Tokenizer.hpp"

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
        auto current = tokenList[i];
        auto currentToken = tokenList[i].token;
        using TokenType = Tokenizer::MainToken;


        switch (currentToken) {
            case TokenType::NEW_LINE:
            case TokenType::CLOSE_BRACE:
            case TokenType::SEMICOLON: {
                group.endPos = current.filePosition;
                if (!group.empty())
                    statements.push_back(group);
                group = TokenGroup();
                group.startPos = current.filePosition;
                initialized = insideArgList = false;
                break;
            }
            
            case TokenType::OPEN_BRACE:
            case TokenType::EQUALS: { initialized = true; group.initializingToken = current; break; }
            case TokenType::OPEN_BRACKET: {
                if (!initialized) {
                    insideArgList = true;
                    group.isFunction = true;
                }
                break;
            }

            case TokenType::CLOSE_BRACKET: {
                if (group.isFunction) { insideArgList = false; }
                break;
            }

            default: {
                if (initialized)
                    group.initializer.push_back(current);
                else if (insideArgList)
                    group.arguments.push_back(current);
                else
                    group.tokens.push_back(current);
                break;
            }

        }
    }
    if (!group.empty())
        statements.push_back(group);

    return statements;
}

