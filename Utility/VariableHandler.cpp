//
// Created by Owner on 7/31/2022.
//

#include "VariableHandler.hpp"

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