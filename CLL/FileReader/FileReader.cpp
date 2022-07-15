//
// Created by Owner on 6/8/2022.
//

#include "FileReader.h"


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


std::vector<std::string> getKeywords(std::string string) {
    std::vector<std::string> content;
    std::string buffer, strBuffer;

    int strType = -1, lastType = -1;
    // TODO: String Support
    for (int i = 0; i < string.size(); i++) {
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


FileReader::FileReader(std::string path) : m_Line(0) {
    bool print = false;

    m_File.open(path);
    m_Chars = GetFileContents(m_File);

    int index = 0;
    std::string buffer;
    auto wordBuffer = getKeywords(m_Chars);
    for (auto& i : wordBuffer) m_WordBuffer.push_back({.str = i, .line = m_Line});
    m_File.close();
}


std::vector<FileReader::WordBufferWithPos> FileReader::getWordBuffer() { return m_WordBuffer; }

std::string FileReader::getChars() { return m_Chars; }

std::vector<FileReader::WordBufferWithPos> FileReader::getFromTo(bool (*from)(std::string), bool (*to)(std::string)) {
    std::vector<FileReader::WordBufferWithPos> content;

    size_t index = 0;
    while (!from(m_WordBuffer[index])) index++;
    while (!to(m_WordBuffer[index])) content.push_back(m_WordBuffer[index++]); content.push_back(m_WordBuffer[index]);
    return content;
}
std::string FileReader::getFromToChars(bool (*from)(char), bool (*to)(char)) {
    std::string content;

    size_t index = 0;
    while (!from(m_Chars[index])) index++;
    while (!to(m_Chars[index])) content.push_back(m_Chars[index++]); content.push_back(m_Chars[index]);
    return content;
}

void FileReader::free() {
    m_WordBuffer.clear();
    m_File.close();
}

FileReader::~FileReader() { free(); }