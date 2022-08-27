//
// Created by Owner on 8/14/2022.
//

#include "Tokenizer.h"

Tokenizer::Tokenizer(std::vector<Parser::ParsedString> parsedArray) {
    std::string volatileString;
    Parser::FilePosition volatilePos;
    for (bool recordVolatile = false; auto& i : parsedArray) {
        auto validTokenObject = parseStringToToken(i);

        if (validTokenObject.one && validTokenObject.two.token == MainToken::VOLATILE_OPEN) {
            recordVolatile = true;
            volatilePos = validTokenObject.two.filePosition;
            continue;
        }
        else if (validTokenObject.one && validTokenObject.two.token == MainToken::VOLATILE_CLOSE) {
            m_Tokens.push_back({MainToken::VOLATILE_STATEMENT, volatileString, volatilePos});
            recordVolatile = false;
            volatileString.clear();
            continue;
        }
        if (recordVolatile) {
            volatileString += validTokenObject.two.tokenData + " ";
            continue;
        }

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

