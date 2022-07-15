//
// Created by Owner on 7/14/2022.
//

#ifndef COMMONLANGUAGELIBRARIES_GENERATORSTATEMENTS_H
#define COMMONLANGUAGELIBRARIES_GENERATORSTATEMENTS_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

static std::map<std::string, std::string> AliasTable {
        {"int", "int64_t"},
        {"Int", "int64_t"},
        {"void", "void"},
        {"Void", "void"},
        {"String", "const char*"},
        {"string", "const char*"},
};

struct parameter {
    std::string name,type,defaultVal;
};

inline void WriteStatement(std::ofstream& file, std::string str) {
    file << str << " ";
}

inline void WriteStatementCompliantToAlias(std::ofstream& file, std::string str) {
    if (AliasTable.contains(str)) file << AliasTable[str] << " ";
    else WriteStatement(file, str);
}


#endif //COMMONLANGUAGELIBRARIES_GENERATORSTATEMENTS_H
