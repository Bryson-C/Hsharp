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
    *this = Tokenizer(parser.getWordBuffer());
}

std::vector<Tokenizer::Token> Tokenizer::getTokens() {
    return m_Tokens;
};
