//
// Created by Owner on 7/31/2022.
//

#ifndef CLL_CLL_H
#define CLL_CLL_H


#include <iostream>
#include <string>
#include <vector>
#include <varargs.h>
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

inline void panic(std::string error = "", int code = -1) {
    std::cerr << error << "\n";
    exit(code);
}


constexpr bool PANIC_ON_ERROR = true;

namespace {
    struct Any {
    private:
        static const int32_t STRING_SIZE = 128;
        union Data {
            int_fast64_t    _int;
            char            _str[STRING_SIZE];
            double          _flt;
        };
    public:
        enum class Type {
            None,
            Int,
            String,
            Float
        } dataType;
        Data value;

        Any() : dataType(Type::None) { value._int = 0; }
        Any(const char* string) : dataType(Type::String) {
            if (strlen(string) > STRING_SIZE)
                std::cerr << "String Too Large For Storage!\n";
            memcpy(value._str, string, STRING_SIZE);
        }
        explicit Any(std::string string) : dataType(Type::String) {
            *this = Any(string.c_str());
        }
        explicit Any(int64_t integer) : dataType(Type::Int) { value._int = integer; }
        explicit Any(float flt) : dataType(Type::Int) { value._flt = 0; }

        template<typename ...T> friend void CLL_StdErr(std::string message, T... t);
    };
}

inline void CLL_StdErr(std::string message, std::initializer_list<std::string> labels = {}, std::initializer_list<std::string> labelText = {}) {
    std::cerr << message << "\n";
    for (uint32_t i = 0; i < labels.size(); i++) {
        std::cerr << "  " << labels.begin()[i] << ": " << labelText.begin()[i] << "\n";
    }
    std::cout << "\n";
    if (PANIC_ON_ERROR)
        panic();
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
    Duople() = default;
    Duople(DataType1 data1, DataType2 data2) { one = data1; two = data2; }

    DataType1 one;
    DataType2 two;
};


template<typename DataType1, typename DataType2, typename DataType3>
struct Triple {
    DataType1 one;
    DataType2 two;
    DataType3 three;
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

template<typename T> T& last(std::vector<T>& vec) { return vec[vec.size()-1]; }
template<typename T> T& first(std::vector<T>& vec) { return vec[0]; }
template<typename T> void push(std::vector<T>& vec, std::vector<T> data) { for (auto& i : data) vec.push_back(i); }
template<typename T> void push(std::vector<T>& vec, T data) {  vec.push_back(data); }


inline std::string toLowerCase(std::string str) {
    for (auto& c : str) c = (char)tolower(c);
    return str;
}
inline std::string toUpperCase(std::string str) {
    for (auto& c : str) c = (char)toupper(c);
    return str;
}
inline std::string stripQuotes(std::string str) {
    if (str.empty() || !isAny(str[0], {'\'','\"'}))
        return str;
    char quoteType = str[0];
    std::string newString;
    for (auto& chr : str) {
        if (chr == quoteType) {
            continue;
        }
        newString += chr;
    }
    return newString;
}



#endif //CLL_CLL_H
