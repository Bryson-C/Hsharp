//
// Created by Owner on 7/31/2022.
//

#include "VariableHandler.hpp"


template<typename DataType>
CLL_VariableResult<DataType> CLL_GetVariableOptionally(CLL_ScopedVariables& variables, std::string name, DataType (*copeCast)(std::string)) {
    CLL_VariableResult<DataType> variable;

    if (variables.varIndex.contains(name)) {
        variable.result = CLL_EVariableHandlerResult::VariablePresent;
        variable.variable = variables.vars[variables.varIndex[name]];
        variable.indexedSlice = {};
        variable.cope = {};
    } else {
        variable.result = CLL_EVariableHandlerResult::VariableNonExistent;
        variable.variable = {};
        variable.indexedSlice = {};
        variable.cope = copeCast(name);
    }
    return variable;
}
CLL_VariableResult<std::string> CLL_GetVariableOptionally(CLL_ScopedVariables& variables, std::string name) {
    return CLL_GetVariableOptionally<std::string>(variables, name, [](std::string str){return str;});
}

CLL_OperationHandlerResult<std::vector<int64_t>> CLL_PreformAutoOperation(CLL_ScopedVariables& variables, std::vector<std::string> left, std::vector<std::string> right, std::string op) {
    CLL_EOperationResult result = CLL_EOperationResult::Success;
    // Check For Valid Data
    if (left.empty() || right.empty()) result = CLL_EOperationResult::FaultyData;
    else if ((left.size() > 1 && right.size() == 1) || (right.size() > 1 && left.size() == 1)) result = CLL_EOperationResult::ArrayAgainstSingleValue;
    else if (left.size() != right.size()) result = CLL_EOperationResult::InvalidArgumentRatio;


    return CLL_PreformOperation(variables, left, right, op);
}
CLL_OperationHandlerResult<std::vector<int64_t>> CLL_PreformOperation(CLL_ScopedVariables& variables, std::vector<std::string> left, std::vector<std::string> right, std::string op) {
    CLL_EOperationResult result = CLL_EOperationResult::Success;
    std::vector<int64_t> leftInt, rightInt;

    if (left.empty() || right.empty()) result = CLL_EOperationResult::FaultyData;
    else if ((left.size() > 1 && right.size() == 1) || (right.size() > 1 && left.size() == 1))
        result = CLL_EOperationResult::ArrayAgainstSingleValue;
    else if (left.size() > 1 || right.size() > 1)
        CLL_StdErr("Attempting To Use An Array In Operation", {CLL_StdLabels::Cope, CLL_StdLabels::Offender},
                   {"Using First Value In Array", {left[0] + ", ... Or " + right[0] + ", ..."}});

    auto leftVariable = CLL_GetVariableOptionally<int64_t>(variables, left[0],
                                                           [](std::string name) { return std::stoll(name); });
    if (leftVariable.result == CLL_EVariableHandlerResult::VariablePresent)
        for (auto& i : leftVariable.variable.data)
            leftInt.push_back(std::stoll(i));
    else
        leftInt.push_back(leftVariable.cope);

    auto rightVariable = CLL_GetVariableOptionally<int64_t>(variables, right[0],
                                                            [](std::string name) { return std::stoll(name); });
    if (rightVariable.result == CLL_EVariableHandlerResult::VariablePresent)
        for (auto& i : rightVariable.variable.data)
            rightInt.push_back(std::stoll(i));
    else
        rightInt.push_back(rightVariable.cope);

    bool twoSidedOperation = false;

    // Since We Dont Support String Operations We Need To Give An Error
    // Since We Cannot Support Miss Matched Types We Need To Give An Error
    if (leftVariable.result == CLL_EVariableHandlerResult::VariablePresent && rightVariable.result == CLL_EVariableHandlerResult::VariablePresent) {
        if (leftVariable.variable.type != rightVariable.variable.type) {
            result = CLL_EOperationResult::MissMatchedTypes;
            CLL_StdErr("Trying To Preform An Operation On Miss Matched Types", {}, {});
        }
        if (leftVariable.variable.type == CLL_EVariableTypes::String ||
            rightVariable.variable.type == CLL_EVariableTypes::String) {
            CLL_StdErr("String Operations Are Not Supported By This Function", {CLL_StdLabels::TODO},
                       {"Make String Operations Work"});
        }
    }
    twoSidedOperation = true;


    std::vector<int64_t> integer;
    if (twoSidedOperation) {
        bool arrayOperation = false;
        bool exceededLimits = false;
        // addition
        if (op == "+") {
            for (size_t iter = 0; auto& i : leftInt)
                integer.push_back(i + rightInt[iter++]);
        }
            // subtraction
        else if (op == "-") {
            for (size_t iter = 0; auto& i : leftInt)
                integer.push_back(i - rightInt[iter++]);
        }
            // multiplication
        else if (op == "*") {
            for (size_t iter = 0; auto& i : leftInt)
                integer.push_back(i * rightInt[iter++]);
        }
            // division
        else if (op == "/") {
            for (size_t iter = 0; auto& i : leftInt)
                integer.push_back(i / rightInt[iter++]);
        }
            // modulus
        else if (op == "%") {
            for (size_t iter = 0; auto& i : leftInt)
                integer.push_back(i % rightInt[iter++]);
        }
            // exponent
        else if (op == "**") {
            for (size_t iter = 0; auto& i : leftInt) {
                int64_t val = i;
                int64_t lastStep = 0;
                for (uint32_t i = 0; i < rightInt[iter]; i++) {
                    lastStep = val *= rightInt[iter];
                    if (abs(lastStep) > integer[0]) exceededLimits = true;
                }
                integer.push_back(val);
                iter++;
            }
        }
            // range
        else if (op == "..") {
            if (leftInt.size() > 1 || rightInt.size() > 1) {
                CLL_StdErr("Cannot Create 2D Arrays", {CLL_StdLabels::TODO}, {"Make The Return Value Of Operation 2 Dimensional"});
            } else {
                int64_t left = leftInt[0], right = rightInt[0];
                std::vector<int64_t> range;
                if (left < right)
                {
                    for (int64_t i = left; i <= right; i++) range.push_back(i);
                }
                else if (right > left)
                {
                    for (int64_t i = right; i >= left; i--) range.push_back(i);
                }
                else if (left == right)
                {
                    integer = {left};
                }
                integer = range;
            }
        }
        for (int32_t index = 0; auto &val: integer) {
            if (val >= INT64_MAX || val <= INT64_MIN || exceededLimits) {

                std::string equation = std::to_string(leftInt[0]);
                equation += (leftInt.size() > 1) ? "...": "";
                equation += {" " + op + " " + std::to_string(rightInt[0])};
                equation += (rightInt.size() > 1) ? "...": "";

                CLL_StdErr("Math Operation Hit The 64 Bit Limit There For Cannot Give A Completely Accurate Result",{"Operation", "Equation", CLL_StdLabels::Offender, CLL_StdLabels::Location}, {op, equation, equation, "Unknown Location"});
            }
            index++;
        }
    }
    return {
            .result = result,
            .value = integer,
    };
}



