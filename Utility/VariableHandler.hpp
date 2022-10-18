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

#include "../CLL.hpp"
#include "../Tokenizer/Tokenizer.hpp"


enum class VariableType {
    NONE,
    AUTO,

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
inline std::string getVariableTypeAsString(VariableType type) {
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
inline std::string getVariableTypeAsCType(VariableType type, bool useStdInt) {
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

template<typename DataType> struct Range {
private:
public:
    static VariableType GetAsType(DataType from, DataType max) {
        bool isSigned = from < 0;
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
    std::string strval;
    VariableType type;
public:
    Value() {}
    Value(int32_t number) : value(number), strval(""), type(VariableType::INT32_TYPE) {}
    explicit Value(int64_t number) : value(number), strval(""), type(VariableType::INT64_TYPE) {}
    Value(std::string string) : value(0), strval(string), type(VariableType::STRING_TYPE) {}
    friend class Variable;
    friend class Function;
};

// TODO: For Code-Gen Ignore Commas Separating Values, This Is Just A Byproduct Of Bad Planning

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


    void setName(std::string str) { name = str; }
    std::string getName() { return name; }
    void setType(VariableType _type) { type = _type; }
    VariableType getType() { return type; }

    // TODO: Check Bounds Of Arguments Compared To min-max Range Type Deduction
    // TODO: Deal With Type Collisions
    template<typename DataType> void push(DataType data) {
        Value v(data);
        if (type == VariableType::NONE || type == VariableType::AUTO) type = v.type;
        if (v.type != type) {
            std::cerr << "Type Collision Pushing Value: {" << v.value << " or " << v.strval << "} Of Type: " << getVariableTypeAsString(v.type)
                        << " Into Variable Of Type: " << getVariableTypeAsString(type) << "\n";
        }
        values.push_back(v);
    }
    std::string generateOutput() {
        std::string str;

        if (type == VariableType::NONE || type == VariableType::AUTO) {
            if (values.empty())
                std::cerr << "No Type Specified For Variable: " << name << "\n";
            else if (!values.empty())
                type = values[0].type;
        }

        str += getVariableTypeAsCType(type, false) + " ";

        str += name;

        if (values.size() == 1) {
            str += " = " + ((values[0].type == VariableType::STRING_TYPE) ? values[0].strval : std::to_string(values[0].value) );
        }
        else if (values.size() > 1) {
            str += "[] = {";
            for (int loop = 0; auto &val: values) {
                if (type == VariableType::STRING_TYPE && val.strval.empty()) { loop++; continue; }

                str += ((type == VariableType::STRING_TYPE) ? val.strval : std::to_string(val.value));

                if (loop < values.size() - 1) str += ", ";
                loop++;
            }
            if (values.size() > 1) str += "}";
        }

        if (values.empty()) str += "; /* Warning: Uninitialized Variable */\n";
        else str += ";\n";

        return str;
    }
};

class Function {
private:
    VariableType                    type;
    std::string                     name;
    std::vector<Variable>           arguments;
    std::vector<Tokenizer::Token>   body;
    std::string                     returnValue;
public:
    void setType(VariableType _type) { type = _type; }
    VariableType getType() { return type; }
    void setName(std::string _name) { name = _name; }
    std::string getName() { return name; }

    void pushArgument(Variable arg) { arguments.push_back(arg); }

    void pushBody(Tokenizer::Token token) { body.push_back(token); }
    void pushBody(std::vector<Tokenizer::Token> tokens) { for (const auto& i : tokens) body.push_back(i); }

    std::string generateOutput() {
        std::string str = getVariableTypeAsCType(type, false) + " ";
        str += name;
        str += "(";
        for (int loop = 0; auto& args : arguments) {
            str += getVariableTypeAsCType(args.getType(), false) + " " + args.getName();
            if (loop < arguments.size()-1)
                str += ", ";
            loop++;
        }
        str += ")";
        str += " {";
        if (!returnValue.empty()) {
            str += "\n" + returnValue + ";\n";
        }
        str += "}\n";
        return str;
    }
};

class Operation {
private:
    std::string name;
    std::vector<Tokenizer::Token> initializer;
public:
    Operation() = default;

    void setName(std::string str) { name = str; }
    void push(Tokenizer::Token token) { initializer.push_back(token); }
};


#endif //CLL_VARIABLEHANDLER_HPP
