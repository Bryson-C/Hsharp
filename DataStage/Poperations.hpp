//
// Created by Owner on 8/19/2022.
//

#ifndef CLL_POPERATIONS_HPP
#define CLL_POPERATIONS_HPP


#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <assert.h>

class OperationStack {
public:
    enum class OPs {
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        EXP,
        RNG,
        EQL,
        NEQL,
        GEQL,
        LEQL,
    };
private:
    static constexpr bool logOperation = false;
    std::map<std::string, OPs> stringToOp = {
            {"+", OPs::ADD},
            {"-", OPs::SUB},
            {"*", OPs::MUL},
            {"/", OPs::DIV},
            {"%", OPs::MOD},
            {"**", OPs::EXP},
            {"..", OPs::RNG},
            {"==", OPs::EQL},
            {"!=", OPs::NEQL},
            {">=", OPs::GEQL},
            {"<=", OPs::LEQL},

    };


    using IntegerType = int64_t;
    using IntegerArray = std::vector<IntegerType>;
    std::vector<IntegerArray> stack;

    static void printOperation(IntegerType left, IntegerType right, std::string op, IntegerType result) {
        if (!logOperation) return;
        std::cout << "Operation: " << left << " " << op << " " << right << " = " << result << "\n";
    }
    static void printOperation(IntegerType left, IntegerType right, std::string op, IntegerArray result) {
        if (!logOperation) return;
        std::cout << "Operation: " << left << " " << op << " " << right << " = ";
        for (auto& i : result) {
            std::cout << i << ((i != result[result.size()-1]) ? ", ":"\n");
        }
    }
public:
    struct SuccessType {
        bool success;
        IntegerArray result;
    };

    static inline OPs opFromString(std::string op) {
        if (op == "+") { return OPs::ADD; }
        else if (op == "-") { return OPs::SUB; }
        else if (op == "*") { return OPs::MUL; }
        else if (op == "/") { return OPs::DIV; }
        else if (op == "%") { return OPs::MOD; }
        else if (op == "**") { return  OPs::EXP; }
        else if (op == "..") { return  OPs::RNG; }
        else if (op == "==") { return  OPs::EQL; }
        else if (op == "!=") { return  OPs::NEQL; }
        else if (op == ">=") { return  OPs::GEQL; }
        else if (op == "<=") { return  OPs::LEQL; }
    }
    inline void clear() { stack.clear(); }
    inline void pushInt(IntegerType operand) { stack.push_back({operand}); }
    inline void pushInt(IntegerArray operand) { stack.push_back(operand); }
    inline SuccessType pushOp(OPs op) {
        SuccessType success = {.success = true, .result = {}};


        IntegerArray left = stack[0], right = stack[1];
        IntegerArray result;
        if (left.size() != right.size()) {
            std::cerr << "Cannot Preform Operation On Different Sized Arrays\n";
            success.success = false;
        }

        switch (op) {
            case OPs::ADD: {
                for (int i = 0; i < left.size(); i++) {
                    result.push_back(left[i] + right[i]);
                    printOperation(left[i], right[i], "+", result[i]);
                }
                stack.erase(stack.begin(), stack.begin()+2);
                stack.push_back(result);
                break;
            };
            case OPs::SUB: {
                for (int i = 0; i < left.size(); i++) {
                    result.push_back(left[i] - right[i]);
                    printOperation(left[i], right[i], "-", result[i]);
                }
                stack.erase(stack.begin(), stack.begin()+2);
                stack.push_back(result);
                break;
            };
            case OPs::MUL: {
                for (int i = 0; i < left.size(); i++) {
                    result.push_back(left[i] * right[i]);
                    printOperation(left[i], right[i], "x", result[i]);
                }
                stack.erase(stack.begin(), stack.begin()+2);
                stack.push_back(result);
                break;
            };
            case OPs::DIV: {
                for (int i = 0; i < left.size(); i++) {
                    result.push_back(left[i] / right[i]);
                    printOperation(left[i], right[i], "/", result[i]);
                }
                stack.erase(stack.begin(), stack.begin()+2);
                stack.push_back(result);
                break;
            };
            case OPs::MOD: {
                result.push_back(left[0] % right[0]);
                stack.erase(stack.begin(), stack.begin()+2);
                stack.push_back(result);
                break;
            };
            case OPs::EXP: {
                // TODO: Add Array Support
                IntegerType power = left[0];
                for ( IntegerType i = right[0]; i > 1; i--) {
                    power *= left[0];
                }
                printOperation(left[0], right[0], "^", power);
                result.push_back(power);
                stack.erase(stack.begin(), stack.begin()+2);
                stack.push_back(result);
                break;
            };
            case OPs::RNG: {
                // TODO: Add Array Support
                for (IntegerType i = left[0]; (left > right) ? i >= right[0] : i <= right[0]; (left > right) ? i-- : i++) {
                    result.push_back(i);
                }
                printOperation(left[0], right[0], "->", result);
                stack.erase(stack.begin(), stack.begin()+2);
                stack.push_back(result);
                break;
            };
            case OPs::EQL: {
                result.push_back(left[0] == right[0]);
                printOperation(left[0], right[0], "==", result);
                stack.erase(stack.begin(), stack.begin()+2);
                stack.push_back(result);
                break;
            };
            case OPs::NEQL: {
                result.push_back(left[0] != right[0]);
                printOperation(left[0], right[0], "!=", result);
                stack.erase(stack.begin(), stack.begin()+2);
                stack.push_back(result);
                break;
            };
            case OPs::GEQL: {
                result.push_back(left[0] >= right[0]);
                printOperation(left[0], right[0], ">=", result);
                stack.erase(stack.begin(), stack.begin()+2);
                stack.push_back(result);
                break;
            };
            case OPs::LEQL: {
                result.push_back(left[0] <= right[0]);
                printOperation(left[0], right[0], "<=", result);
                stack.erase(stack.begin(), stack.begin()+2);
                stack.push_back(result);
                break;
            };
            default: {
                std::cerr << "Unsupported Operation\n";
                success.success = false;
                break;
            };
        }
        success.result = result;
        return success;
    }

    inline void print() {
        for (auto& item : stack) {
            for (auto& child : item) {
                std::cout << child << ((child != item[item.size()-1]) ? ", ":"\n");
            }
        }
    }

    inline void parseString(std::string string) {
        IntegerArray ints;
        std::vector<OperationStack::OPs> ops;

        std::string buffer;
        for (int i = 0; i < string.size(); i++) {
            if (isdigit(string[i])) {
                while (isdigit(string[i])) {
                    buffer += string[i++];
                }
                ints.push_back(std::stoll(buffer));
                i--;
                buffer.clear();
                continue;
            }
            if (!isalnum(string[i]) && !isspace(string[i])) {
                while (!isalnum(string[i]) && !isspace(string[i])) {
                    buffer += string[i++];
                }
                if (stringToOp.contains(buffer))
                    ops.push_back(stringToOp[buffer]);
                i--;
                buffer.clear();
                continue;
            }
        }

        for (auto& i : ints)
            stack.push_back({i});

        for (auto& op : ops) {
            print();
            pushOp(op);
        }
    }

};



#endif //CLL_POPERATIONS_HPP
