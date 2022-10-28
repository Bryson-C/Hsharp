//
// Created by Owner on 7/31/2022.
//

#include "BaseDataHandler.hpp"

std::string getVariableTypeAsString(VariableType type) {
    switch (type) {
        case VariableType::NONE: return "Void";
        case VariableType::INT8_TYPE: return "Int8";
        case VariableType::INT16_TYPE: return "Int16";
        case VariableType::INT32_TYPE: return "Int32";
        case VariableType::INT64_TYPE: return "Int64";
        case VariableType::UINT8_TYPE: return "Uint8";
        case VariableType::UINT16_TYPE: return "Uint16";
        case VariableType::UINT32_TYPE: return "Uint32";
        case VariableType::UINT64_TYPE: return "Uint64";
        case VariableType::STRING_TYPE: return "String";
        default: return "None";
    }
}
std::string getVariableTypeAsCType(VariableType type, bool useStdInt) {
    switch (type) {
        case VariableType::NONE:        return "void";
        case VariableType::INT8_TYPE:   return useStdInt ? "int8_t" : "signed char";
        case VariableType::INT16_TYPE:  return useStdInt ? "int16_t" : "short";
        case VariableType::INT32_TYPE:  return useStdInt ? "int32_t" : "int";
        case VariableType::INT64_TYPE:  return useStdInt ? "int64_t" : "long long";
        case VariableType::UINT8_TYPE:  return useStdInt ? "uint8_t" : "unsigned char";
        case VariableType::UINT16_TYPE: return useStdInt ? "uint16_t" : "unsigned short";
        case VariableType::UINT32_TYPE: return useStdInt ? "uint32_t" : "unsigned int";
        case VariableType::UINT64_TYPE: return useStdInt ? "uint64_t" : "unsigned long long";
        case VariableType::STRING_TYPE: return "const char*";
        default: return "None";
    }
}
bool isIntegerType(VariableType type) {
    return isAny(type, {VariableType::INT8_TYPE, VariableType::INT16_TYPE, VariableType::INT32_TYPE, VariableType::INT64_TYPE});
}
bool isIntegerType(Tokenizer::MainToken token) {
    return isAny(token, {Tokenizer::MainToken::INTEGER});
}


std::string BaseDataHandler::dataHandlerTypeToString(BaseDataHandler::DataHandlerType type) {
    switch (type) {
        case DataHandlerType::UNKNOWN: return "Unknown";
        case DataHandlerType::VARIABLE: return "Variable";
        case DataHandlerType::FUNCTION: return "Function";
        case DataHandlerType::OPERATION: return "Operation";
    }
}

BaseDataHandler::BaseDataHandler(TokenGroup &group) {
    _initializerToken = group.initializingToken;
    for (const auto& i : group.tokens) {
        switch (i.token) {
            case Tokenizer::MainToken::AUTO_TYPE:       _type = VariableType::AUTO; typeInitialized = true; break;
            case Tokenizer::MainToken::INT_TYPE:        _type = VariableType::INT32_TYPE; typeInitialized = true; break;
            case Tokenizer::MainToken::STRING_TYPE:     _type = VariableType::STRING_TYPE; typeInitialized = true; break;
            case Tokenizer::MainToken::NAMED:           _name = i.tokenData; break;
            default: break;
        }
    }
    for (Triple<VariableType, std::string, Value<VariableType::AUTO>> argument; const auto& i : group.arguments) {
        switch (i.token) {
            case Tokenizer::MainToken::AUTO_TYPE:       argument.one = VariableType::AUTO; break;
            case Tokenizer::MainToken::INT_TYPE:        argument.one = VariableType::INT32_TYPE; break;
            case Tokenizer::MainToken::STRING_TYPE:     argument.one = VariableType::STRING_TYPE; break;
            case Tokenizer::MainToken::NAMED:           argument.two = i.tokenData; break;
            case Tokenizer::MainToken::COMMA:           _arguments.push_back(argument); argument = Triple<VariableType, std::string, Value<VariableType::AUTO>>(); break;
            default: break;
        }
    }

    for (const auto& initializerToken : group.initializer) {
        switch (initializerToken.token) {
            case Tokenizer::MainToken::INTEGER:
                _operations.push_back(Tokenizer::MainToken::Unknown);
                _values.push(std::stoi(initializerToken.tokenData));
                break;
            case Tokenizer::MainToken::STRING:
                _operations.push_back(Tokenizer::MainToken::Unknown);
                _values.push(initializerToken.tokenData);
                break;

            case Tokenizer::MainToken::OP_ADDITION:
                _operations.push_back(Tokenizer::MainToken::OP_ADDITION);
                break;

            default: break;
        }

    }

    if (!typeInitialized) _dataType = DataHandlerType::OPERATION;
    else if (group.isFunction) _dataType = DataHandlerType::FUNCTION;
    else _dataType = DataHandlerType::VARIABLE;
}

BaseDataHandler::DataHandlerType BaseDataHandler::getHandledDataType() const { return _dataType; }

void BaseDataHandler::print() const {
    std::cout << " [." << dataHandlerTypeToString(_dataType) << ".] "
              << ((_dataType != DataHandlerType::OPERATION) ? getVariableTypeAsString(_type) : "") << " " << _name << "\n";
    for (int i = 0; i < _values.size(); i++) {
        std::cout << "\t -- " << getVariableTypeAsString(_values[i].one) << " ";
        _values.println(false, i);
    }
    std::cout << "\n";
}

Duople<bool, Any> evaluateExpression(TokenGroup& group) {
    std::vector<Any> mathStack;
    for (int i = 0; i < group.getInitializer().size(); i++) {
        auto current = group.getInitializer()[i];
        bool isInt = false;

        int64_t currentInt;
        int64_t nextInt;
        if (i+1 < group.getInitializer().size() && isIntegerType(group.getInitializer()[i+1].token))
            nextInt = std::stoll(group.getInitializer()[i+1].tokenData);
        if (isIntegerType(current.token)) {
            currentInt = std::stoll(current.tokenData);
            isInt = true;
        } else { std::cerr << "Unhandled Case Or String Found In Expression Evaluator\n"; }


        if (isAny(current.token, {
                Tokenizer::MainToken::OP_ADDITION,
                Tokenizer::MainToken::OP_SUBTRACTION,
                Tokenizer::MainToken::OP_MULTIPLICATION,
                Tokenizer::MainToken::OP_DIVISION,
                Tokenizer::MainToken::OP_MODULUS,
                Tokenizer::MainToken::OP_EXPONENTIAL
        })) {
            // Do Maths;
            if (mathStack.size() < 2) {
                std::cerr << "Failure In Math Operation Stack!\n";

                return { false, Any(0, VariableType::INT32_TYPE) };
            }
        }





    }
}
