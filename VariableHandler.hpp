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

inline bool isDigit(std::string string) {
    for (auto& i : string) if (!isdigit(i)) return false;
    return true;
}
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
    if (isDigit(value)) return CLL_EVariableTypes::Integer64;
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






#endif //CLL_VARIABLEHANDLER_HPP
