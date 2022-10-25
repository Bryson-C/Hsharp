//
// Created by Owner on 10/15/2022.
//

#include "Scope.hpp"

namespace {
    VariableType GetCompilerOptionIntegerSize(CompilerOptions& compilerOptions) {
        switch (compilerOptions.defaultIntegerWidth) {
            case 8:  return VariableType::INT8_TYPE;
            case 16: return VariableType::INT16_TYPE;
            case 32:
            default: return VariableType::INT32_TYPE;
            case 64: return VariableType::INT64_TYPE;
        }
    }
}

/*
Scope DetermineScope(CompilerOptions& compilerOptions, std::vector<TokenGroup>& tokenGroups) {
    Scope scope;

    for (auto& group : tokenGroups) {
        if (group.empty()) break;
        using TokenType = Tokenizer::MainToken;
        Variable var;
        Function func;
        Operation operation;
        bool isOperation = true;
        // This Is For Determining The Type Of Data To Process
        for (auto& defToken : group.getTokens()) {
            switch (defToken.token) {
                case TokenType::NAMED:
                    {
                        bool sharesName = false;
                        for (auto& varNames : scope.variables) {
                            if (defToken.tokenData == varNames.getName()) { sharesName = true; break; }
                        }
                        if (!sharesName) {
                            var.setName(defToken.tokenData);
                            func.setName(defToken.tokenData);
                        } else {
                            // TODO: This Is Likely A Function/Variable Call
                        }
                        operation.setName(defToken.tokenData);
                        break;
                    }
                case TokenType::AUTO_TYPE:
                    {
                        isOperation = false;
                        var.setType(VariableType::AUTO);
                        func.setType(VariableType::AUTO);
                        break;
                    }
                case TokenType::INT_TYPE:
                    {
                        isOperation = false;
                        VariableType integerWidth = GetCompilerOptionIntegerSize(compilerOptions);
                        var.setType(integerWidth);
                        func.setType(integerWidth);
                        break;
                    }
                case TokenType::STRING_TYPE:
                    {
                        isOperation = false;
                        var.setType(VariableType::STRING_TYPE);
                        func.setType(VariableType::STRING_TYPE);
                        break;
                    }
                default: {
                    var.setType(VariableType::NONE);
                    func.setType(VariableType::NONE);
                    break;
                }
            }
        }
        // This Is For Setting Arguments For The Data
        Variable argument;
        for (auto& argToken : group.getArguments()) {
            switch (argToken.token) {
                case TokenType::INT_TYPE:
                    {
                        argument.setType(GetCompilerOptionIntegerSize(compilerOptions));
                        break;
                    }
                case TokenType::STRING_TYPE:
                    {
                        argument.setType(VariableType::STRING_TYPE);
                        break;
                    }
                case TokenType::NAMED:
                    {
                        argument.setName(argToken.tokenData);
                        break;
                    }
                case TokenType::COMMA:
                    {
                        func.pushArgument(argument);
                        argument = Variable();
                        break;
                    }
                default:
                    {
                        break;
                    }
            }
        }
        if (argument.getType() != VariableType::NONE) {
            func.pushArgument(argument);
            argument = Variable();
        }
        // This Is For Determining The Operations That Happen To The Data
        for (auto& initToken : group.getInitializer()) {
            if (!group.isFunctionType()) {
                operation.push(initToken);
                Value val;

                if (initToken.token == TokenType::INTEGER) val = Value(std::stoi(initToken.tokenData));
                else if (initToken.token == TokenType::STRING) val = Value(initToken.tokenData);
                else {
                    continue;
                }

                var.push(val);
            } else {

            }
        }
        if (isOperation) {
            scope.operations.push_back(operation);
        }
        else if (group.isFunctionType()) {
            scope.functions.push_back(func);
        }
        else {
            scope.variables.push_back(var);
        }
    }

    return scope;
}
*/