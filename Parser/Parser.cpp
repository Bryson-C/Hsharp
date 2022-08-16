//
// Created by Owner on 6/8/2022.
//

#include "Parser.h"






[[deprecated("Old System. I Dont Know If I Can Remove Tho")]] std::vector<std::string> getKeywords(std::string string) {
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

std::vector<Parser::ParsedString> Parser::parse(std::string path) {
    std::ifstream file(path);
    if (!file.is_open()) { std::cerr << "File '" << path << "' Couldn't Open!\n"; return {}; }
    std::vector<ParsedString> content;
    std::string buffer;
    size_t line = 1;
    while (std::getline(file, buffer)) {
        if (m_Settings & Settings::RecordNewLine && !buffer.empty())
            content.push_back({"\\n", {line, buffer.size(), path}});

        for (int i = 0; i < buffer.size(); i++) {

            // Skip Comments
            if (buffer[i] == '/' && buffer[i+1] == '/') {
                while (i < buffer.size()) {
                    i++;
                }
                continue;
            }
            // Reader Entire String
            else if (buffer[i] == '\"') {
                size_t startIndex = i;
                std::string strBuffer = "\"";
                i++;
                while (buffer[i] != '\"') {
                    strBuffer += buffer[i++];
                }
                strBuffer += "\"";
                content.push_back({strBuffer, {line, startIndex, path}});
                continue;
            }
            // While The Character Is A Digit Continue Until It Is Not
            else if (isdigit(buffer[i])) {
                size_t startIndex = i;
                std::string numBuffer;
                while (isdigit(buffer[i])) {
                    numBuffer += buffer[i++];
                }
                content.push_back({numBuffer, {line, startIndex, path}});
                i--;
                continue;
            }
            // If Character Is A NewLine Or Tab And The RecordNewLine Setting Is On Record The NewLine Or Tab
            else if (isspace(buffer[i]) && (m_Settings & Settings::RecordNewLine)) {
                size_t startIndex = i;
                if (buffer[i] == '\n' || buffer[i] == '\t') {
                    content.push_back({std::to_string(buffer[i]), {line, startIndex, path}});
                    continue;
                }
            }
            // If It Is A Symbol
            // If It Is A Symbol Followed By The Same Symbol Combine Them
            // Otherwise Just Record The Symbol
            else if (!isalnum(buffer[i]) && !isspace(buffer[i])) {
                size_t startIndex = i;
                std::string symbolBuffer;
                if (buffer[i+1] == buffer[i]) {
                    symbolBuffer += buffer[i];
                    symbolBuffer += buffer[i];
                    i++;
                } else {
                    // Special Cases For Big And Little Arrows
                    if ((buffer[i] == '=' || buffer[i] == '-') && buffer[i+1] == '>') {
                        symbolBuffer += buffer[i];
                        symbolBuffer += buffer[i+1];
                        i++;
                    }
                    else {
                        symbolBuffer = buffer[i];
                    }
                }
                content.push_back({symbolBuffer, {line, startIndex, path}});
                continue;
            }
            // Record Normal Characters
            else if (isalpha(buffer[i])) {
                size_t startIndex = i;
                std::string strBuffer;
                while (!isspace(buffer[i]) && isalnum(buffer[i]) && i < buffer.size()) {
                    strBuffer += buffer[i++];
                }
                content.push_back({strBuffer, {line, startIndex, path}});
                i--;
                continue;
            }
            // If It Is A Space Ignore
            else if (isspace(buffer[i])) {
                continue;
            }
            // Report Any Unhandled Rules Of The Parser
            else {
                std::cerr << "[" << path << ":" << line << ":" << i << "]" << "Unknown Parse Rule\n";
            }
        }
        line++;
    }
    // Post Parsing
    for (int i = 0; i < content.size(); i++) {
        // Include Different Files
        if (content[i].str == "request") {

            auto includePath = content[i+1].str;
            includePath.erase(includePath.begin());
            includePath.erase(includePath.end()-1);

            bool includedAlready = false;
            for (auto& included : m_IncludedFiles) {
                if (included == includePath) includedAlready = true;
            }

            content.erase(content.begin() + i, content.begin() + i + 3);

            if (!includedAlready) {
                auto wordBuffer = parse(includePath);

                for (int j = 0; j < wordBuffer.size(); j++) {
                    content.insert(content.begin() + i+j, wordBuffer[j]);
                }
                m_IncludedFiles.push_back(includePath);
            } else {
                std::cerr << content[i+1].errorString() << "Trying To Re-Include: '" << includePath << "' " << "\n";
            }



            continue;
        }
    }
    return content;
}

Parser::Parser(std::string path, Settings settings) {
    m_Settings = settings;
    auto wordBuffer = parse(path);
    for (int i = 0; i < wordBuffer.size(); i++)
        m_WordBuffer.push_back(wordBuffer[i]);
}


std::vector<Parser::ParsedString> Parser::getWordBuffer() { return m_WordBuffer; }


bool Parser::operator==(Parser& parser) {
    return parser.m_ParsedId == this->m_ParsedId;
}
