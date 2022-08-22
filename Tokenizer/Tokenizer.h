//
// Created by Owner on 8/14/2022.
//

#ifndef CLL_TOKENIZER_H
#define CLL_TOKENIZER_H

#include <map>

#include "../Parser/Parser.h"
#include "../CLL.hpp"

class Tokenizer {
public:
    enum class MainToken {
        Unknown,

        INT_TYPE,
        STRING_TYPE,

        NAMED,
        INTEGER,
        STRING,

        FUNCTION_CALL,
        VARIABLE_CALL,

        NEW_LINE,
        TAB,

        OPEN_BRACE,
        CLOSE_BRACE,
        OPEN_BRACKET,
        CLOSE_BRACKET,
        SEMICOLON,
        EQUALS,
        COMMA,
        BIG_ARROW,
        SMALL_ARROW,
        RETURN,

        OP_ADDITION,
        OP_SUBTRACTION,
        OP_DIVISION,
        OP_MULTIPLICATION,
        OP_MODULUS,
        OP_EXPONENTIAL,
        OP_RANGE,
        OP_CONCAT,
        OP_OTHER,
    };

    static inline std::string tokenToString(MainToken token) {
        switch (token) {
            case MainToken::Unknown: return "Unknown";
            case MainToken::INT_TYPE: return "Integer_Type";
            case MainToken::STRING_TYPE: return "String_Type";
            case MainToken::NAMED: return "Custom_Named_Token";
            case MainToken::INTEGER: return "Integer";
            case MainToken::STRING: return "String";
            case MainToken::FUNCTION_CALL: return "Function_Call";
            case MainToken::VARIABLE_CALL: return "Variable_Call";
            case MainToken::NEW_LINE: return "New_Line";
            case MainToken::TAB: return "Tab";
            case MainToken::OPEN_BRACE: return "[";
            case MainToken::CLOSE_BRACE: return "]";
            case MainToken::OPEN_BRACKET: return "{";
            case MainToken::CLOSE_BRACKET: return "}";
            case MainToken::SEMICOLON: return ";";
            case MainToken::EQUALS: return "=";
            case MainToken::COMMA: return ",";
            case MainToken::BIG_ARROW: return "=>";
            case MainToken::SMALL_ARROW: return "->";
            case MainToken::RETURN: return "return";
            case MainToken::OP_ADDITION: return "+";
            case MainToken::OP_SUBTRACTION: return "-";
            case MainToken::OP_DIVISION: return "/";
            case MainToken::OP_MULTIPLICATION: return "*";
            case MainToken::OP_MODULUS: return "%";
            case MainToken::OP_EXPONENTIAL: return "**";
            case MainToken::OP_RANGE: return "..";
            case MainToken::OP_CONCAT: return "&";
            case MainToken::OP_OTHER: return "|";
        }
    }

    struct Token {
        MainToken token;
        std::string tokenData;
        Parser::FilePosition filePosition;
        std::string asString() { return tokenToString(token); }
    };
private:


    std::map<std::string, MainToken> m_TokenDictionary {
            {"Int", MainToken::INT_TYPE},
            {"String", MainToken::STRING_TYPE},
            {"return", MainToken::RETURN},
            {"\\n", MainToken::NEW_LINE},
            {"\\t", MainToken::TAB},
            {"{", MainToken::OPEN_BRACE},
            {"}", MainToken::CLOSE_BRACE},
            {"[", MainToken::OPEN_BRACKET},
            {"]", MainToken::CLOSE_BRACKET},
            {";", MainToken::SEMICOLON},
            {"=", MainToken::EQUALS},
            {",", MainToken::COMMA},
            {"=>", MainToken::BIG_ARROW},
            {"->", MainToken::SMALL_ARROW},

            {"+", MainToken::OP_ADDITION},
            {"-", MainToken::OP_SUBTRACTION},
            {"/", MainToken::OP_DIVISION},
            {"*", MainToken::OP_MULTIPLICATION},
            {"%", MainToken::OP_MODULUS},
            {"**", MainToken::OP_EXPONENTIAL},
            {"..", MainToken::OP_RANGE},
            {"&", MainToken::OP_CONCAT},
            {"|", MainToken::OP_OTHER},
    };

    Duople<bool, Token> parseStringToToken(Parser::ParsedString string) {
        MainToken main = MainToken::Unknown;
        if (!m_TokenDictionary.contains(string.str))
            ;
        else
            main = m_TokenDictionary[string];

        return { (main != MainToken::Unknown), {.token = main, .tokenData = string.str} };
    }

    std::vector<Token> m_Tokens;
public:

    Tokenizer(std::vector<Parser::ParsedString> parsedArray);
    Tokenizer(Parser parser);

    std::vector<Token> getTokens();

    static inline bool isOperation(Token token) {
        return isAny(token.token, {MainToken::OP_ADDITION, MainToken::OP_SUBTRACTION,
                                       MainToken::OP_DIVISION, MainToken::OP_MULTIPLICATION,
                                       MainToken::OP_MODULUS, MainToken::OP_EXPONENTIAL, MainToken::OP_RANGE});
    }
    static inline std::string typeToString(MainToken token) {
        switch (token) {
            case Tokenizer::MainToken::INT_TYPE: return "long long int";
            case Tokenizer::MainToken::STRING_TYPE: return "const char*";
        }
        return "";
    }
    static inline std::string typeToString(Token token) {
        return typeToString(token.token);
    }
};


#endif //CLL_TOKENIZER_H
