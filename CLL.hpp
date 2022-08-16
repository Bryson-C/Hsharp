//
// Created by Owner on 7/31/2022.
//

#ifndef CLL_CLL_H
#define CLL_CLL_H


#include <iostream>
#include <string>
#include <vector>
#include <assert.h>

namespace CLL_StdLabels {
    constexpr const char* Data = "Data";
    constexpr const char* Cope = "Cope";
    constexpr const char* Location = "Location";
    constexpr const char* TODO = "TODO";
    constexpr const char* Index = "Index";
    constexpr const char* Variable = "Variable";
    constexpr const char* Offender = "Offender";
    constexpr const char* OffendingVariable = "Offending Variable";

};
#define PANIC_ON_ERROR false
inline void CLL_StdErr(std::string message, std::initializer_list<std::string> labels = {}, std::initializer_list<std::string> labelText = {}) {
    std::cerr << message << "\n";
    for (uint32_t i = 0; i < labels.size(); i++) {
        std::cerr << "  " << labels.begin()[i] << ": " << labelText.begin()[i] << "\n";
    }
    std::cout << "\n";
    if (PANIC_ON_ERROR)
        exit(-1);
}
inline void CLL_StdOut(std::string message, std::initializer_list<std::string> labels = {}, std::initializer_list<std::string> labelText = {}) {
    std::cout << message << "\n";
    for (uint32_t i = 0; i < labels.size(); i++) {
        std::cout << "  " << labels.begin()[i] << ": " << labelText.begin()[i] << "\n";
    }
}

// Sounded Funny And Also Has Duo (2) In The Name
template<typename DataType1, typename DataType2>
struct Duople {
    DataType1 one;
    DataType2 two;
};


inline Duople<bool, int64_t> isDigit(std::string string) {
    bool isnum = true;
    for (auto& i : string) { if (!isdigit(i)) isnum = false; break; }
    return {isnum, (isnum) ? std::stoll(string) : -1};
}
inline Duople<bool, std::string> isString(std::string string) {
    return { (string[string.size()-1] == '\"' && string[0] == '\"'), string };
}



template<typename T>
bool isAny(T data, std::vector<T> comp) {
    for (auto& i : comp) if (data == i) return true;
    return false;
}
template<typename T>
bool isAll(T data, std::vector<T> comp) {
    for (auto& i : comp) if (data != comp) return false;
    return true;
}



#endif //CLL_CLL_H
