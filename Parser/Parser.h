//
// Created by Owner on 6/8/2022.
//

#ifndef COMMONLANGUAGELIBRARIES_FILEREADER_H
#define COMMONLANGUAGELIBRARIES_FILEREADER_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <concepts>


std::vector<std::string> getKeywords(std::string string);
std::string GetFileContents(std::ifstream& file);

//std::string FromTo(std::string string, int& indexOffset, bool (*from)(char), bool (*to)(char));
//std::vector<std::string> parseString(std::string& string);

template<typename Indexable, typename IndexType>
Indexable FromTo(Indexable& vec, int& indexOffset, bool (*from)(IndexType), bool (*to)(IndexType)) {
    Indexable content;
    int& index = indexOffset;
    while (!from(vec[index])) index++;
    do { content.push_back(vec[index++]); } while (!to(vec[index]));
    return content;
}
template<typename Indexable, typename IndexType>
Indexable HereTo(Indexable& vec, int& indexOffset, bool (*to)(IndexType)) {
    return FromTo<Indexable, IndexType>(vec, indexOffset, [](IndexType){return true;}, to);
}

class Parser {
public:
    struct WordBufferWithPos {
        std::string str;
        size_t line, column;

        inline void print() { std::cout << "[" << line << ":" << column << "]  " << str << "\n"; }
        operator std::string() { return str; }
    };
    enum Settings {
        None = 1,
        RecordNewLine = 2,
    };
private:
    std::vector<WordBufferWithPos> m_WordBuffer;

    size_t m_FileId;
    Settings m_Settings;

    void parse(std::string path);
public:
    Parser(std::string path, Settings settings = Settings::None);

    std::vector<WordBufferWithPos> getWordBuffer();

    bool operator==(Parser& reader);
};


#endif //COMMONLANGUAGELIBRARIES_FILEREADER_H
