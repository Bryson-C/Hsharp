//
// Created by Owner on 7/31/2022.
//

#ifndef CLL_VARIABLEHANDLER_HPP
#define CLL_VARIABLEHANDLER_HPP

// TODO: Migrate Code To A Cleaner Code Base


#include <vector>
#include <string>
#include <map>
#include <algorithm>

#include "CLL.hpp"
enum class CLL_EVariableTypes {
    None,
    String,
    Integer64,
    Void,
};
inline CLL_EVariableTypes CLL_StringToVarType(std::string type) {
    std::string lower;
    for (auto& chr : type) lower += std::tolower(chr);
    if (lower == "string") return CLL_EVariableTypes::String;
    else if (lower == "int") return CLL_EVariableTypes::Integer64;
    return CLL_EVariableTypes::None;
}
inline std::string CLL_VarTypeToString(CLL_EVariableTypes type) {
    switch (type) {
        case CLL_EVariableTypes::String: return "string";
        case CLL_EVariableTypes::Integer64: return "int64";
        default: return "";
    }
}
inline std::string CLL_VarTypeToCType(CLL_EVariableTypes type) {
    switch (type) {
        case CLL_EVariableTypes::String: return "const char*";
        case CLL_EVariableTypes::Integer64: return "long long int";
        default: return "";
    }
}

enum class CLL_EVariableHandlerResult {
    VariablePresent,
    VariableNonExistent,
    VariableCreationSuccess,
    VariableCreationFailure,
};
enum class CLL_EVariableValueResult {
    IndexableData,
    NonIndexableData,
};
struct CLL_Variable {
    std::string name;
    CLL_EVariableTypes type;
    std::vector<std::string> data;
};
template<typename DataType>
struct CLL_VariableResult {
    CLL_EVariableHandlerResult result;
    CLL_Variable variable;
    std::vector<DataType> indexedSlice;
    DataType cope;
};
template<typename DataType>
struct CLL_ValueResult {
    CLL_EVariableValueResult result;
    DataType data;
};

inline bool isOperator(std::string string) {
    if (string == "+") return true;
    if (string == "-") return true;
    if (string == "*") return true;
    if (string == "/") return true;
    if (string == "%") return true;
    if (string == "**") return true;
    if (string == "..") return true;
    // TODO: Implement ++ And --
    //if (string == "++") return true;
    //if (string == "--") return true;
    return false;
}
inline CLL_EVariableTypes CLL_InferType(std::string value) {
    if (isDigit(value).one) return CLL_EVariableTypes::Integer64;
    else if (value.empty()) return CLL_EVariableTypes::Void;
    return CLL_EVariableTypes::String;
}

struct CLL_ScopedVariables {
private:
    size_t varCount = 0;
public:
    std::map<std::string, uint32_t> varIndex;
    std::vector<CLL_Variable> vars;


    inline CLL_VariableResult<std::string> newScopedVariable(CLL_Variable variable) {
        size_t index = varCount++;
        varIndex.emplace(variable.name, index);
        vars.push_back(variable);
        return {
                .result = CLL_EVariableHandlerResult::VariableCreationSuccess,
                .variable = variable,
                .indexedSlice = {},
                .cope = "",
        };
    }
    inline CLL_VariableResult<std::string> newScopedVariable(std::string name, CLL_EVariableTypes type, std::vector<std::string> data) {
        size_t index = varCount++;
        varIndex.emplace(name, index);
        vars.push_back({name, type, data});
        return {
                .result = CLL_EVariableHandlerResult::VariableCreationSuccess,
                .variable = vars[index],
                .indexedSlice = {},
                .cope = "",
        };
    };
    inline CLL_VariableResult<std::string> getScopedVarByName(std::string name) {
        bool variableExists = varIndex.contains(name);
        return {
                .result = (variableExists) ? CLL_EVariableHandlerResult::VariablePresent : CLL_EVariableHandlerResult::VariableNonExistent,
                .variable = vars[varIndex[name]],
                .indexedSlice = {},
                .cope = "",
        };
    }
    inline bool findVariable(std::string name) {
        return varIndex.contains(name);
    }
    inline CLL_ValueResult<std::vector<std::string>> getVariableValue(std::string name) {
        bool variableExists = varIndex.contains(name);
        return {
                .result = (variableExists && vars[varIndex[name]].data.size() > 1) ? CLL_EVariableValueResult::IndexableData : CLL_EVariableValueResult::NonIndexableData,
                .data = vars[varIndex[name]].data,
        };
    }
    inline CLL_ValueResult<std::string> getVariableValueIndexed(std::string name, size_t index) {
        bool variableExists = varIndex.contains(name);
        return {
                .result = (variableExists && vars[varIndex[name]].data.size() > 1) ? CLL_EVariableValueResult::IndexableData : CLL_EVariableValueResult::NonIndexableData,
                .data = vars[varIndex[name]].data[index],
        };
    }

};

enum class CLL_EOperationResult {
    Success,
    MissMatchedTypes,
    ArrayAgainstSingleValue,
    FaultyData,
    // This Means That The Ratio Of A Array Argument Is Just Not Possible To Operate On
    // So If `Arg1` Has 12 Variables And `Arg2` Has 5, There Is No Foreseeable Way To Cope
    InvalidArgumentRatio,
    Unknown,
};
template<typename DataType>
struct CLL_OperationHandlerResult {
    CLL_EOperationResult result;
    DataType value;
};


// Turns A String Into A Array
// For Example `[0,1,2,3,4]` Would Be Turned Into A Vector With 0, 1, 2, 3, And 4 As Elements
inline std::vector<std::string> CLL_AssembleArray(std::string string) {
    std::vector<std::string> content;
    std::string containers = "{[()]}", buffer;

    size_t iter = 0, opens = 0;
    while (string[iter]) {
        bool found = false;
        for (size_t i = 0; i < containers.size(); i++) {
            if (string[iter] == containers[i]) {
                found = true;
                if (i <= 2) opens++;
                else opens--;
                iter++;
            }
            if (iter >= string.size()) goto EXIT_LABEL;
            if (found) break;
        }
        if (string[iter] == ',') {
            content.push_back(buffer);
            buffer.clear();
            iter++;
            continue;
        }
        buffer += string[iter];
        iter++;
    }
    EXIT_LABEL:
    if (opens > 0) CLL_StdErr("Function `CLL_AssembleArray` Only Supports Single Dimensional Arrays", {CLL_StdLabels::Offender, CLL_StdLabels::Cope, CLL_StdLabels::Data}, {"Offender: Argument 1", "Ignore", {"Scope = " + std::to_string(opens)}});
    if (!buffer.empty()) content.push_back(buffer);

    return content;
}

// If The Value Passed Into `name` Parameter Is Present In `variables` The Function Will Try To Get The Value At The Index/Index Range
template<typename DataType>
CLL_VariableResult<DataType> CLL_GetVariableIndexOptionally(CLL_ScopedVariables& variables, std::string name, DataType (*cast)(std::string));


// If The Value Passed Into `name` Parameter Is Present As A Variable In `variables` It Will Return The Variable, Otherwise It Will Return A Cope Value
template<typename DataType>
CLL_VariableResult<DataType> CLL_GetVariableOptionally(CLL_ScopedVariables& variables, std::string name, DataType (*cast)(std::string));
CLL_VariableResult<std::string> CLL_GetVariableOptionally(CLL_ScopedVariables& variables, std::string name);



// Implicitly Find What Types The Variables Are And Pass The Values Into Another Function
CLL_OperationHandlerResult<std::vector<int64_t>> CLL_PreformAutoOperation(CLL_ScopedVariables& variables, std::vector<std::string> left, std::vector<std::string> right, std::string op);
// Operations Preformed On Single Integers
CLL_OperationHandlerResult<std::vector<int64_t>> CLL_PreformOperation(CLL_ScopedVariables& variables, std::vector<std::string> left, std::vector<std::string> right, std::string op);

template<typename DataType>
struct CLL_ArrayType {
    std::vector<DataType> array;
};


inline void ParseExpression(CLL_ScopedVariables& variables, std::vector<std::string> string) {
    std::vector<std::string> array;
    std::string buffer;
    for (int i = 0; i < string.size(); i ++) {

        auto stringRead = [&string, &i, &buffer, &array, &variables](char end){
            while (string[i] != std::to_string(end)) {
                if (string[i] == ",") {
                    array.push_back(buffer);
                    buffer.clear();
                    i++;
                    continue;
                } else if (isOperator(string[i+1])) {
                    auto result = CLL_PreformAutoOperation(variables, {string[i]}, {string[i+2]}, string[i+1]);
                    i+=3;
                    continue;
                }
                buffer += string[i];
                i++;
            }
        };


        if (string[i] == "(") {
            i++;
            stringRead(')');
        } else if (string[i] == "\"") {
            i++;
            stringRead('\"');
        }
        for (auto& wrd : array)
            std::cout << wrd << "\n";
    }
}

// When The Function Will Be Called:
// 1. Get The Name And Find Out If It Is A Variable Or A Value; If It Is A Value Return The Number
// 2. If It Is A Variable Check If Indices Is Valid, Or If Indices Are Empty Return The First Element Of `name`
// 3. If Name Can Not Return Then (Do Something I Cant Think Of)
inline void CLL_GetVariableValue(CLL_ScopedVariables& variables, std::string name, std::vector<int64_t> indices) {}




namespace CLL_EXP {
    enum class VariableType {
        NONE,

        INT8_TYPE,
        INT16_TYPE,
        INT32_TYPE,
        INT64_TYPE,

        UINT8_TYPE,
        UINT16_TYPE,
        UINT32_TYPE,
        UINT64_TYPE,

        STRING_TYPE
    };

    template<typename DataType> struct Range {
    private:
    public:
        static VariableType GetAsType(DataType from, DataType max) {
            bool isSigned = !(from >= 0);
            if (isSigned) {
                if (max > INT32_MAX) return VariableType::INT64_TYPE;
                else if (max > INT16_MAX) return VariableType::INT32_TYPE;
                else if (max > INT8_MAX) return VariableType::INT16_TYPE;
                else return VariableType::INT8_TYPE;
            } else {
                if (max > UINT32_MAX) return VariableType::UINT64_TYPE;
                else if (max > UINT16_MAX) return VariableType::UINT32_TYPE;
                else if (max > UINT8_MAX) return VariableType::UINT16_TYPE;
                else return VariableType::UINT8_TYPE;
            }
        }
        static std::vector<DataType> GetAsRange(DataType from, DataType to, int_fast32_t step) {
            std::vector<DataType> range;
            for (int i = static_cast<int>(from); i < static_cast<int>(to); i += step)
                range.push_back(static_cast<DataType>(i));
            return range;
        }
    };


    class Value {
    private:
        int64_t value;
        VariableType type;
    public:
        Value(int32_t number) : value(number) {
            Range<int_least64_t>::GetAsType(INT32_MIN, INT32_MAX);
        }
        explicit Value(int64_t number) : value(number) {
            Range<int_least64_t>::GetAsType(INT64_MIN, INT64_MAX);
        }
        friend class Variable;
    };

    class Variable {
    private:
        std::string name;
        VariableType type;
        std::vector<Value> values;
    public:
        Variable() {}
        Variable(const char* _name) : name(_name), type(VariableType::NONE) {}
        Variable(const char* _name, VariableType _type) : name(_name), type(_type) {}
        Variable(const char* _name, int_fast64_t min, uint_fast64_t max) : name(_name) { type = Range<int_least64_t>::GetAsType(min, max); }
        void push(int32_t number) {
            Value v = Value(number);
            if (type == VariableType::NONE)
                type = v.type;
            values.push_back(v);
        }
        void push(int64_t number) {
            Value v = Value((int64_t)number);
            if (type == VariableType::NONE)
                type = v.type;
            values.push_back(v);
        }
    };



}




#endif //CLL_VARIABLEHANDLER_HPP
