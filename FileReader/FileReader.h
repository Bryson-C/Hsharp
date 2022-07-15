//
// Created by Owner on 6/8/2022.
//

#ifndef COMMONLANGUAGELIBRARIES_FILEREADER_H
#define COMMONLANGUAGELIBRARIES_FILEREADER_H

#include <string>
#include <fstream>
#include <vector>
#include <concepts>
#include "../FancyLogger/FancyLogger.h"

std::vector<std::string> getKeywords(std::string string);
std::string GetFileContents(std::ifstream& file);

//std::string FromTo(std::string string, int& indexOffset, bool (*from)(char), bool (*to)(char));


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

class FileReader {
public:
    struct WordBufferWithPos {
        std::string str; size_t line;
        operator std::string() { return str; }
    };
private:
    size_t m_Line;
    std::ifstream m_File;
    std::vector<WordBufferWithPos> m_WordBuffer;
    std::string m_Chars;
public:
    FileReader(std::string path);
    ~FileReader();
    void free();

    std::vector<WordBufferWithPos> getWordBuffer();
    std::string getChars();

    std::vector<WordBufferWithPos> getFromTo(bool (*from)(std::string), bool (*to)(std::string));
    std::string getFromToChars(bool (*from)(char), bool (*to)(char));

};


#endif //COMMONLANGUAGELIBRARIES_FILEREADER_H
