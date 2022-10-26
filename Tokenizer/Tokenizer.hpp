//
// Created by Owner on 8/14/2022.
//

#ifndef CLL_TOKENIZER_HPP
#define CLL_TOKENIZER_HPP

#include <map>

#include "../Parser/Parser.hpp"
#include "../CLL.hpp"





class Tokenizer {
public:
    enum class MainToken {
        Unknown,

        AUTO_TYPE,
        INT_TYPE,
        STRING_TYPE,

        NAMED,
        INTEGER,
        STRING,

        VARIABLE_CALL,
        FUNCTION_CALL,

        NEW_LINE,
        TAB,

        // {
        OPEN_BRACE,
        // }
        CLOSE_BRACE,
        // [
        OPEN_BRACKET,
        // ]
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
            case MainToken::AUTO_TYPE: return "Auto_Type";
            case MainToken::NAMED: return "Custom_Named_Token";
            case MainToken::INTEGER: return "Integer";
            case MainToken::STRING: return "String";
            case MainToken::VARIABLE_CALL: return "";
            case MainToken::FUNCTION_CALL: return "";
            case MainToken::NEW_LINE: return "New_Line";
            case MainToken::TAB: return "Tab";
            case MainToken::OPEN_BRACE: return "{";
            case MainToken::CLOSE_BRACE: return "}";
            case MainToken::OPEN_BRACKET: return "[";
            case MainToken::CLOSE_BRACKET: return "]";
            case MainToken::SEMICOLON: return ";";
            case MainToken::EQUALS: return "=";
            case MainToken::COMMA: return ",";
            case MainToken::BIG_ARROW: return "=>";
            case MainToken::SMALL_ARROW: return "->";
            case MainToken::RETURN: return "Return";
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
        std::string tokenAsString() { return tokenToString(token); }
        inline void print() { std::cout << tokenData << " as '" << tokenToString(token) << "'\n"; }
    };
private:


    std::map<std::string, MainToken> m_TokenDictionary {
            {"Let", MainToken::AUTO_TYPE},
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

    static inline bool isOperation(Token& token) { return isAny(token.token, {MainToken::OP_ADDITION, MainToken::OP_SUBTRACTION, MainToken::OP_DIVISION, MainToken::OP_MULTIPLICATION, MainToken::OP_MODULUS, MainToken::OP_EXPONENTIAL, MainToken::OP_RANGE}); }
    static inline bool isVariableType(Token& token) { return isAny(token.token, {MainToken::INT_TYPE, MainToken::STRING_TYPE}); }

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

    static inline MainToken stringToType(std::string name) {
        std::string type;
        for (auto& chr : name) type += tolower(chr);
        if (type == "int") return MainToken::INT_TYPE;
        else if (type == "string") return MainToken::STRING_TYPE;
        return MainToken::Unknown;
    }
};

class TokenGroup {
private:
    bool isFunction = false;
    std::vector<Tokenizer::Token> tokens;
    Tokenizer::Token initializingToken;
    std::vector<Tokenizer::Token> initializer;
    std::vector<Tokenizer::Token> arguments;

    Parser::FilePosition startPos, endPos;
public:
    void printGroup() {
        if (empty()) {
            std::cout << "-- EMPTY GROUP --\n" << "\tFrom " << startPos.errorString() << "\n\tTo " << endPos.errorString() << "\n-- END GROUP --\n";
        }

        std::cout << " -- NEW GROUP ";
        if (isFunction) std::cout << "[.FUNCTION.] --\n"; else std::cout << "--\n";

        for (auto& def : tokens) { printf("\t"); def.print(); }

        std::cout << " -- () --\n";
        for (auto& arg : arguments) { printf("\t"); arg.print(); }

        std::cout << " -- :: --\n";
        for (auto& init : initializer) { printf("\t"); init.print(); }
        std::cout << " -- END GROUP --\n\n";
    }
    bool empty() {
        return tokens.empty() && initializer.empty() && arguments.empty();
    }
    bool isFunctionType() {
        return isFunction;
    }
    std::vector<Tokenizer::Token> getTokens() { return tokens; }
    std::vector<Tokenizer::Token> getInitializer() { return initializer; }
    std::vector<Tokenizer::Token> getArguments() { return arguments; }
    friend class Tokenizer;
    friend std::vector<TokenGroup> GetTokenGroups(Tokenizer tokenizer);
    friend class BaseDataHandler;
};

std::vector<TokenGroup> GetTokenGroups(Tokenizer tokenizer);



template<typename TokenType>
class [[deprecated("Not Very Useful")]] DynamicTokenizer {
private:
    std::vector<Duople<TokenType, Parser::ParsedString>> tokens;
public:
    Duople<bool, std::string> (*tokenToString)(TokenType);
    Duople<bool, TokenType> (*stringToToken)(std::string);
    Duople<std::vector<TokenType>, TokenType> isVarType;
    TokenType digitType, customNamedString;

    inline void run(Parser& parser) {
        for (int i = 0; i < parser.getWordBuffer().size(); i++) {
            auto current = parser.getWordBuffer()[i];
            auto tokenResult = stringToToken(current.str);
            if (auto isdig = isDigit(current.str); isdig.one) {
                tokens.emplace_back(digitType, current);
                continue;
            }
            else if (!tokenResult.one) {
                tokens.emplace_back(customNamedString, current);
            }
            else {
                tokens.emplace_back(tokenResult.two, current);
                if (isAny(tokenResult.two, isVarType.one) && i < parser.getWordBuffer().size()-1) {
                    tokens.emplace_back(isVarType.two, parser.getWordBuffer()[i+1]);
                    i++;
                }
            }
        }
    }

    inline std::vector<Duople<TokenType, Parser::ParsedString>> getTokens() { return tokens; }
    inline void printTokens(bool printData = false) {
        for (auto& tok : tokens)
            std::cout <<
                ((printData) ? "'" + tok.two.str + "'  as  " : " ")
                << "'" << tokenToString(tok.one).two << "'\n";
    }

};












#endif //CLL_TOKENIZER_HPP
