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

template<typename T>
T& last(std::vector<T>& vec) { return vec[vec.size()-1]; }
template<typename T>
T& first(std::vector<T>& vec) { return vec[0]; }
template<typename T>
void push(std::vector<T>& vec, std::vector<T> data) { for (auto& i : data) vec.push_back(i); }
template<typename T>
void push(std::vector<T>& vec, T data) {  vec.push_back(data); }


/*
 // TODO: Finish Struct
template<typename T>
class Vector {
private:
    T* data;
    size_t elementCount;
    size_t elementSize;
    size_t getElementCount(T* array) { return (sizeof(array)/sizeof(array[0])); }
public:
    Vector(std::initializer_list<T> init) : elementCount(init.size()), elementSize(sizeof(data(init)[0])) {
        data = (T*)malloc(sizeof(init.size()) * elementSize);
        data = (T*)data(init);
    }
    inline T& last() { return data[elementCount-1]; }
    inline T& first() { return data[0]; }
    //inline void push(std::initializer_list<T> init) { size_t count = getElementCount(init); data[elementCount+=count] = init; }
};
*/


#endif //CLL_CLL_H
