//
// Created by Owner on 6/8/2022.
//

#include "Parser.h"


struct [[deprecated("Unused")]] SafeString {
private:
    enum class SafetyValue { Safe = 1, SafeIfNext = 2, SafeIfPrevious = 4 };
public:
    SafeString(const char* str) : base(str), index(0), isSafe(true) {}
    SafeString(std::string str) : base(str), index(0), isSafe(true) {}
    SafeString(std::ifstream& stream) : index(0), isSafe(true) {
        std::string buffer;
        while (std::getline(stream, buffer))
            base += buffer;
    }

    std::string base;
    int32_t index;
    bool isSafe;

    char Current() { return base[index]; }
    char Next() { if (index < base.size()) isSafe = true; return base[++index]; }
    char Previous() { if (index > 0) isSafe = true; return base[--index]; }

    bool IsAlpha() { return isalpha(Current()); }
    bool IsSpace() { return isspace(Current()); }
    bool IsAlnum() { return isalnum(Current()); }
    bool IsSymbol() { return (!IsSpace() && !IsAlnum()); }
    bool IsDigit() { return isdigit(Current()); }
};

std::string GetFileContents(std::ifstream& file) {
    std::string buffer, content;
    while (std::getline(file, buffer)) content += {buffer + '\n'};
    file.clear();
    file.seekg(0, std::ios::beg);
    return content;
}
/*
std::vector<std::string> parseString(std::string& string) {
    std::vector<std::string> wordBuffer;
    std::string buffer;
    // We Dont Want To Add An Iteration For Every Loop, This Is To Improve Control Of How The Chars Are Read
    // This Is The First Pass
    for (int i = 0; i < string.size();) {
        if (isspace(string[i]) || !(isalnum(string[i]) && isspace(string[i]))) {
            if (!buffer.empty()) {
                wordBuffer.push_back(buffer);
                buffer.clear();
            }
            i++;
            continue;
        }
        buffer += string[i];
        i++;
    }
    if (!buffer.empty()) wordBuffer.push_back(buffer);
    for (auto& i : wordBuffer) {
        printf("%s\n",i.c_str());
    }
    return wordBuffer;
}
*/
std::vector<Parser::WordBufferWithPos> parse(std::ifstream& file) {
    std::vector<Parser::WordBufferWithPos> wordBuffer;
    std::string buffer;
    size_t line = 0;
    while (std::getline(file, buffer)) {
        wordBuffer.push_back({buffer, line});
        line++;
    }
    return wordBuffer;
}


std::vector<std::string> getKeywords(std::string string) {
    std::vector<std::string> content;
    std::string buffer, strBuffer;

    int strType = -1, lastType = -1;
    // TODO: String Support
    for (int i = 0; i < string.size(); i++) {
        if (string[i] == '/' && string[i+1] == '/') {
            while (string[i] != '\n') i++;
        }
        if (string[i] == '\"') {
            i++;
            strBuffer = FromTo<std::string, char>(string, i, [](char c){return true;},[](char c){return c=='\"';});
            content.push_back("\""+strBuffer+"\"");
            i++;
            strBuffer.clear();
        }
        if (string[i] == '/' && string[i+1] == '/') {
            strBuffer = FromTo<std::string, char>(string, i, [](char c){return true;}, [](char c){return c=='\n';});
            content.push_back(strBuffer);
            strBuffer.clear();
        }

        if (string[i] == '\\') {
            content.push_back("\\"+std::to_string(string[i+1]));
            i+=2;
        }



        if (isspace(string[i])) { strType = 0; }
        if (isalpha(string[i])) { strType = 1; }
        if (isdigit(string[i])) { strType = 2; }
        if (!isalnum(string[i]) && !isspace(string[i])) { strType = 3; }


        if (lastType != strType) {
            if (lastType == 1 && strType == 2) {}
            else {
                if (!buffer.empty()) { content.push_back(buffer); buffer.clear(); }
                if (strType == 0) continue;
            }
        }

        if (strType == 3) {
            std::string symbolBuffer;
            symbolBuffer+=string[i];
            if (string[++i] == symbolBuffer[0]) {
                symbolBuffer+=string[i];
                if (!symbolBuffer.empty()) content.push_back(symbolBuffer);
            } else {
                content.push_back({string[--i]});
            }
            continue;
        }

        buffer += string[i];
        lastType = strType;
    }
    return content;
}


void Parser::parse(std::string path) {
    std::ifstream file(path);
    std::vector<WordBufferWithPos> content;
    std::string buffer;
    size_t line = 1;
    while (std::getline(file, buffer)) {
        if (buffer.empty()) continue;
        for (int i = 0; i < buffer.size(); i++) {
            m_FileId += (int)buffer[i];
            if (buffer[i] == '/' && buffer[i+1] == '/') {
                while (i < buffer.size()) {
                    i++;
                }
                continue;
            } else if (buffer[i] == '\"') {
                size_t startIndex = i;
                std::string strBuffer = "\"";
                i++;
                while (buffer[i] != '\"') {
                    strBuffer += buffer[i++];
                }
                strBuffer += "\"";
                content.emplace_back(strBuffer, line, startIndex);
                continue;
            } else if (isdigit(buffer[i])) {
                size_t startIndex = i;
                std::string numBuffer;
                while (isdigit(buffer[i])) {
                    numBuffer += buffer[i++];
                }
                content.emplace_back(numBuffer, line, startIndex);
                i--;
                continue;
            } else if (isspace(buffer[i]) && (m_Settings & Settings::RecordNewLine)) {
                size_t startIndex = i;
                if (buffer[i] == '\n' || buffer[i] == '\t') {
                    content.emplace_back(std::to_string(buffer[i]), line, startIndex);
                    continue;
                }
            } else if (!isalnum(buffer[i]) && !isspace(buffer[i])) {
                size_t startIndex = i;
                std::string symbolBuffer;
                if (buffer[i+1] == buffer[i]) {
                    symbolBuffer += buffer[i];
                    symbolBuffer += buffer[i];
                    i++;
                } else {
                    symbolBuffer = buffer[i];
                }
                content.emplace_back(symbolBuffer, line, startIndex);
                continue;
            } else if (isalpha(buffer[i])) {
                size_t startIndex = i;
                std::string strBuffer;
                while (!isspace(buffer[i]) && isalnum(buffer[i]) && i < buffer.size()) {
                    strBuffer += buffer[i++];
                }
                content.emplace_back(strBuffer, line, startIndex);
                continue;
            } else if (isspace(buffer[i])) {
                continue;
            } else {
                std::cerr << "Unknown Parse Rule At Line: " << line << ":" << i << "  Offender: '" << buffer[i] << "'\n";
            }
        }
        content.emplace_back("\\n", line, buffer.size());
        line++;
    }
    m_WordBuffer = content;
}

Parser::Parser(std::string path, Settings settings) : m_FileId(0) {
    bool print = false;
    parse(path);
/*
    m_File.open(path);
    m_Chars = GetFileContents(m_File);
    for (auto& chr : m_Chars)
        m_CharId += (int)chr;

    int index = 0;
    std::string buffer;
    auto wordBuffer = getKeywords(m_Chars);
    for (auto& i : wordBuffer) m_WordBuffer.push_back({.str = i, .line = m_Line});
    m_File.close();*/
}


std::vector<Parser::WordBufferWithPos> Parser::getWordBuffer() { return m_WordBuffer; }


bool Parser::operator==(Parser& reader) {
    return reader.m_FileId == this->m_FileId;
}
