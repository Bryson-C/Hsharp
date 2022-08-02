//
// Created by Owner on 7/31/2022.
//

#ifndef CLL_VARIABLEHANDLER_HPP
#define CLL_VARIABLEHANDLER_HPP

#include <vector>
#include <string>
#include <map>
#include <algorithm>

#include "CLL.hpp"
enum class CLL_EVariableTypes {
    String,
    Integer64,
    Void,
};
inline CLL_EVariableTypes CLL_StringToVarType(std::string type) {
    std::string lower;
    for (auto& chr : type) lower += std::tolower(chr);
    if (lower == "string") return CLL_EVariableTypes::String;
    else if (lower == "int") return CLL_EVariableTypes::Integer64;
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

    inline CLL_VariableResult<std::string> newScopedVariable(std::string name, CLL_EVariableTypes type, std::vector<std::string> data) {
        size_t index = varCount++;
        varIndex.emplace(name, index);
        vars.push_back({name, type, data});
        return {
                .result = CLL_EVariableHandlerResult::VariableCreationSuccess,
                .variable = vars[index],
                .cope = "",
        };
    };
    inline CLL_VariableResult<std::string> getScopedVarByName(std::string name) {
        bool variableExists = varIndex.contains(name);
        return {
                .result = (variableExists) ? CLL_EVariableHandlerResult::VariablePresent : CLL_EVariableHandlerResult::VariableNonExistent,
                .variable = vars[varIndex[name]],
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
    Unknown,
};
template<typename DataType>
struct CLL_OperationHandlerResult {
    CLL_EOperationResult result;
    DataType value;
};

template<typename DataType>
struct CLL_ContainedData {
private:
    size_t childCount;
public:
    CLL_ContainedData() : childCount(0), data({}), children(nullptr) {
        children = (DataType*)malloc(sizeof(DataType) * (childCount+=1));
    }
    std::vector<DataType> data;
    DataType* children;
};

inline CLL_ContainedData<std::string> CLL_GetContainedData(std::string string) {
    std::string containers = "{}()[]";
    std::string opens, closes;
    size_t iter = 0;
    while (string[iter]) {
        for (size_t i = 0; i < containers.size(); i++) {
            if (string[iter] == containers[i]) {
                printf("Found Container: %c\n", string[iter]);
                if (i == 0 || i % 2 == 0) opens += containers[i];
                else closes += containers[i];
            }
        }
        iter++;
    }
    CLL_StdOut("OPENS:", {CLL_StdLabels::Data}, {opens});
    CLL_StdOut("CLOSES:", {CLL_StdLabels::Data}, {closes});
    return {};
}


// If The Value Passed Into `name` Parameter Is Present As A Variable In `variables` It Will Return The Variable, Otherwise It Will Return A Cope Value
template<typename DataType>
CLL_VariableResult<DataType> CLL_GetVariableOptionally(CLL_ScopedVariables& variables, std::string name, DataType (*cast)(std::string));
// Implicitly Find What Types The Variables Are And Pass The Values Into Another Function
CLL_OperationHandlerResult<std::vector<int64_t>> CLL_PreformAutoOperation(CLL_ScopedVariables& variables, std::vector<std::string> left, std::vector<std::string> right, std::string op);
// Operations Preformed On Single Integers
CLL_OperationHandlerResult<std::vector<int64_t>> CLL_PreformOperation(CLL_ScopedVariables& variables, std::vector<std::string> left, std::vector<std::string> right, std::string op);
// Operations Preformed On Integer Arrays
CLL_OperationHandlerResult<std::vector<int64_t>> CLL_PreformArrayOperation(CLL_ScopedVariables& variables, std::vector<std::string> left, std::vector<std::string> right, std::string op);
// Operations Preformed On Strings
CLL_OperationHandlerResult<std::vector<int64_t>> CLL_PreformStringOperation(CLL_ScopedVariables& variables, std::vector<std::string> left, std::vector<std::string> right, std::string op);







#endif //CLL_VARIABLEHANDLER_HPP
