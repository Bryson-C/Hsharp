//
// Created by Owner on 7/31/2022.
//

#include "VariableHandler.hpp"



template<typename DataType>
CLL_VariableResult<DataType> CLL_GetVariableOptionally(CLL_ScopedVariables& variables, std::string name, DataType (*cast)(std::string)) {
    CLL_VariableResult<DataType> variable;
    if (variables.varIndex.contains(name)) {
        variable.result = CLL_EVariableHandlerResult::VariablePresent;
        variable.variable = variables.vars[variables.varIndex[name]];
        variable.cope = {};
    } else {
        variable.result = CLL_EVariableHandlerResult::VariableNonExistent;
        variable.variable = {};
        variable.cope = cast(name);
    }
    return variable;
}
CLL_OperationHandlerResult<std::vector<int64_t>> CLL_PreformAutoOperation(CLL_ScopedVariables& variables, std::vector<std::string> left, std::vector<std::string> right, std::string op) { return {}; }
CLL_OperationHandlerResult<std::vector<int64_t>> CLL_PreformOperation(CLL_ScopedVariables& variables, std::vector<std::string> left, std::vector<std::string> right, std::string op) {
    CLL_EOperationResult opResult = CLL_EOperationResult::Success;
    if (left.empty() || right.empty()) opResult = CLL_EOperationResult::FaultyData;
    else if ((left.size() > 1 && right.size() == 1) || (right.size() > 1 && left.size() == 1)) opResult = CLL_EOperationResult::ArrayAgainstSingleValue;
    else if (left.size() > 1 || right.size() > 1) CLL_StdErr("Array Operations Are Not Supported By This Function", {CLL_StdLabels::TODO}, {"Make Array Operations Work"});

    int64_t leftInt, rightInt;

    if (left.size() > 1 || right.size() > 1)  CLL_StdErr("Attempting To Use An Array In Operation", {CLL_StdLabels::Cope, CLL_StdLabels::Offender}, {"Using First Value In Array", {left[0] + ", ... Or " + right[0] + ", ..."}});

    //CLL_VariableResult<std::string> leftVariable, rightVariable;

    auto leftVariable = CLL_GetVariableOptionally<int64_t>(variables, left[0], [](std::string name){ return std::stoll(name); });
    if (leftVariable.result == CLL_EVariableHandlerResult::VariablePresent)
        leftInt = std::stoll(leftVariable.variable.data[0]);
    else
        leftInt = leftVariable.cope;

    auto rightVariable = CLL_GetVariableOptionally<int64_t>(variables, right[0], [](std::string name){ return std::stoll(name); });
    if (rightVariable.result == CLL_EVariableHandlerResult::VariablePresent)
        rightInt = std::stoll(rightVariable.variable.data[0]);
    else
        rightInt = rightVariable.cope;

    // Since We Dont Support String Operations We Need To Give An Error
    // Since We Cannot Support Miss Matched Types We Need To Give An Error
    if (leftVariable.result == CLL_EVariableHandlerResult::VariablePresent && rightVariable.result == CLL_EVariableHandlerResult::VariablePresent) {
        if (leftVariable.variable.type != rightVariable.variable.type) {
            opResult = CLL_EOperationResult::MissMatchedTypes;
            CLL_StdErr("Trying To Preform An Operation On Miss Matched Types",{},{});
        }
        if (leftVariable.variable.type == CLL_EVariableTypes::String || rightVariable.variable.type == CLL_EVariableTypes::String) {
            CLL_StdErr("String Operations Are Not Supported By This Function", {CLL_StdLabels::TODO}, {"Make String Operations Work"});
        }
    }



    std::vector<int64_t> integer;
    bool arrayOperation = false;
    bool exceededLimits = false;
    // addition
    if (op == "+") {
        integer = {leftInt+rightInt};
    }
        // subtraction
    else if (op == "-") {
        integer = {leftInt-rightInt};
    }
        // multiplication
    else if (op == "*") {
        integer = {leftInt*rightInt};
    }
        // division
    else if (op == "/") {
        integer = {leftInt/rightInt};
    }
        // modulus
    else if (op == "%") {
        integer = {leftInt%rightInt};
    }
        // exponent
    else if (op == "**") {
        integer = {leftInt};
        int64_t lastStep = 0;
        for (uint32_t i = 0; i < rightInt; i++) {
            lastStep = integer[0] *= leftInt;
            if (abs(lastStep) > integer[0]) exceededLimits = true;
        }
    }
        // range
    else if(op == "..") {
        std::vector<int64_t> range;
        if (leftInt < rightInt) {
            for (int32_t i = leftInt; i <= rightInt; i++) range.push_back(i);
        } else if (rightInt > leftInt) {
            for (int32_t i = rightInt; i >= leftInt; i--) range.push_back(i);
        } else if (leftInt == rightInt){
            integer = {leftInt};
        }
        integer = range;
    }
    for (int32_t index = 0; auto& val : integer) {
        if (val >= INT64_MAX || val <= INT64_MIN || exceededLimits) {
            std::string equation = {std::to_string(leftInt) + " " + op + " " + std::to_string(rightInt)};
            CLL_StdErr("Math Operation Hit The 64 Bit Limit There For Cannot Give A Completely Accurate Result",
                       {"Operation", "Equation", CLL_StdLabels::Offender, CLL_StdLabels::Location},
                       {op, equation, equation, "Unknown Location"});
        }
        index++;
    }

    return {
            .result = opResult,
            .value = integer,
    };
}
CLL_OperationHandlerResult<std::vector<int64_t>> CLL_PreformArrayOperation(CLL_ScopedVariables& variables, std::vector<std::string> left, std::vector<std::string> right, std::string op) { return {}; }
CLL_OperationHandlerResult<std::vector<int64_t>> CLL_PreformStringOperation(CLL_ScopedVariables& variables, std::vector<std::string> left, std::vector<std::string> right, std::string op) { return {}; }
