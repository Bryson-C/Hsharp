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
#include <any>

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
std::string getVariableTypeAsString(VariableType type);
std::string getVariableTypeAsCType(VariableType type, bool useStdInt);
bool isIntegerType(VariableType type);




namespace {
    class Any {
    private:
        VariableType type;
        union multiTypeStorage {
            int64_t integer;
            uint64_t uinteger;
            char string[128];
        } _data;

        [[maybe_unused]] bool result;
    public:
        explicit Any(int8_t value, VariableType typeOverride = VariableType::INT32_TYPE) : result(true),
                                                                                           type(typeOverride) { _data.integer = value; }

        explicit Any(int16_t value, VariableType typeOverride = VariableType::INT32_TYPE) : result(true),
                                                                                            type(typeOverride) { _data.integer = value; }

        explicit Any(int32_t value, VariableType typeOverride = VariableType::INT32_TYPE) : result(true),
                                                                                            type(typeOverride) { _data.integer = value; }

        explicit Any(int64_t value, VariableType typeOverride = VariableType::INT32_TYPE) : result(true),
                                                                                            type(typeOverride) { _data.integer = value; }

        explicit Any(uint8_t value, VariableType typeOverride = VariableType::UINT32_TYPE) : result(true),
                                                                                             type(typeOverride) { _data.uinteger = value; }

        explicit Any(uint16_t value, VariableType typeOverride = VariableType::UINT32_TYPE) : result(true),
                                                                                              type(typeOverride) { _data.uinteger = value; }

        explicit Any(uint32_t value, VariableType typeOverride = VariableType::UINT32_TYPE) : result(true),
                                                                                              type(typeOverride) { _data.uinteger = value; }

        explicit Any(uint64_t value, VariableType typeOverride = VariableType::UINT32_TYPE) : result(true),
                                                                                              type(typeOverride) { _data.uinteger = value; }

        explicit Any(std::string value, VariableType typeOverride = VariableType::STRING_TYPE) : type(
                typeOverride) { (strcpy_s(_data.string, value.c_str()) == 0) ? result = true : result = false; }

        template<VariableType> friend
        class Value;
    };

    // TODO: Finish Any Value Re-work
    template<VariableType DefaultDataType = VariableType::AUTO>
    class Value {
    private:
        std::vector<Any> _data{};
    public:
        Value() = default;

        explicit Value(int32_t number, VariableType overrideType = DefaultDataType) {
            _data.emplace_back(number,
                               (DefaultDataType == VariableType::AUTO) ? VariableType::INT32_TYPE : overrideType);
        }

        explicit Value(int64_t number, VariableType overrideType = DefaultDataType) {
            _data.emplace_back(number,
                               (DefaultDataType == VariableType::AUTO) ? VariableType::INT64_TYPE : overrideType);
        }

        explicit Value(std::string string, VariableType overrideType = DefaultDataType) {
            if (DefaultDataType != VariableType::AUTO && DefaultDataType != VariableType::STRING_TYPE)
                std::cerr << "Cannot Push Data Type Of: " << (int) overrideType << " To String Variable\n";
            _data.emplace_back(string, VariableType::STRING_TYPE);
        }

        Duople<VariableType, Any::multiTypeStorage> operator[](size_t index) {
            return {_data[index].type, _data[index]._data};
        }

        VariableType getType(size_t index = 0) { return _data[index].type; }

        std::string getAsString(size_t index = 0) { return _data[index]._data.string; }

        int64_t getAsInt(size_t index = 0) { return _data[index]._data.integer; }

        uint64_t getAsUInt(size_t index = 0) { return _data[index]._data.uinteger; }

        template<VariableType DataType = DefaultDataType, typename Data>
        void push(Data data) {
            Any value(data);
            if (DataType != VariableType::AUTO) value.type = DataType;
            _data.push_back(value);
        }

        bool empty() { return _data.empty(); }

        size_t size() { return _data.size(); }

        friend class Variable;

        friend class Function;
    };
}

// TODO: For Code-Gen Ignore Commas Separating Values, This Is Just A Byproduct Of Bad Planning


class BaseDataHandler {
private:
    enum class DataHandlerType {
        UNKNOWN,
        VARIABLE,
        FUNCTION,
        OPERATION,
    };
    static std::string dataHandlerTypeToString(DataHandlerType type) {
        switch (type) {
            case DataHandlerType::UNKNOWN: return "Unknown";
            case DataHandlerType::VARIABLE: return "Variable";
            case DataHandlerType::FUNCTION: return "Function";
            case DataHandlerType::OPERATION: return "Operation";
        }
    }
    DataHandlerType _dataType;


    std::string _name;
    VariableType _type;
    bool typeInitialized = false;
    Value<VariableType::AUTO> _values;
    std::vector<Triple<VariableType, std::string, Value<VariableType::AUTO>>> _arguments;
public:
    BaseDataHandler() = default;
    explicit BaseDataHandler(const char* name, VariableType type = VariableType::AUTO) : _name(name), _type(type), _dataType(DataHandlerType::UNKNOWN) {}
    explicit BaseDataHandler(TokenGroup& group) {
        for (const auto& i : group.tokens) {
            switch (i.token) {
                case Tokenizer::MainToken::AUTO_TYPE:       _type = VariableType::AUTO; typeInitialized = true; break;
                case Tokenizer::MainToken::INT_TYPE:        _type = VariableType::INT32_TYPE; typeInitialized = true; break;
                case Tokenizer::MainToken::STRING_TYPE:     _type = VariableType::STRING_TYPE; typeInitialized = true; break;
                case Tokenizer::MainToken::NAMED:           _name = i.tokenData; break;
                default: break;
            }
        }
        if (_type == VariableType::AUTO)
            printf("Initialized '%s' As Type: Auto\n", _name.c_str());
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

        for (const auto& i : group.initializer) {
            switch (i.token) {
                case Tokenizer::MainToken::INTEGER:
                    if (isIntegerType(_type))
                        _values.push(std::stoi(i.tokenData));
                    else if (_type == VariableType::AUTO || _type == VariableType::NONE) {
                        _values.push(std::stoi(i.tokenData));
                        _type = VariableType::INT32_TYPE;
                    }
                    break;
                case Tokenizer::MainToken::STRING:
                    if (_type == VariableType::STRING_TYPE)
                        _values.push(i.tokenData);
                    else if (_type == VariableType::AUTO || _type == VariableType::NONE) {
                        _values.push(i.tokenData);
                        _type = VariableType::STRING_TYPE;
                    }
                    break;
                default: break;
            }
        }

        if (!typeInitialized) _dataType = DataHandlerType::OPERATION;
        else if (group.isFunction) _dataType = DataHandlerType::FUNCTION;
        else _dataType = DataHandlerType::VARIABLE;
    }
    void print() {
        std::cout << "-- NEW DATA HANDLER " << " [." << dataHandlerTypeToString(_dataType) << ".] " << "--\n";
        std::cout << getVariableTypeAsString(_type) << " " << _name << "\n";
        for (int i = 0; i < _values.size(); i++) {
            std::cout << "\t -- " << getVariableTypeAsString(_values[i].one) << " ";
            if (isIntegerType(_values.getType(i)))
                std::cout << _values[i].two.integer << "\n";
            else if (_values.getType(i) == VariableType::STRING_TYPE)
                std::cout << _values[i].two.string << "\n";
        }
        std::cout << "-- END DATA HANDLER --\n\n";
    }
};


/*
class Variable {
private:
    std::string name;
    VariableType type;
    Value<VariableType::AUTO> values;
public:
    Variable() {}
    Variable(const char* _name) : name(_name), type(VariableType::NONE) {}
    Variable(const char* _name, VariableType _type) : name(_name), type(_type) {}


    void setName(std::string str) { name = str; }
    std::string getName() { return name; }
    void setType(VariableType _type) { type = _type; }
    VariableType getType() { return type; }

    // TODO: Check Bounds Of Arguments Compared To min-max Range Type Deduction
    // TODO: Deal With Type Collisions
    template<typename DataType> void push(DataType data) {
        // TODO: Problems Arise Here
        Value v(data);
        if (type == VariableType::NONE || type == VariableType::AUTO) type = v.getType();
        if (v.getType() != type) {
            std::cerr << "Type Collision Pushing Value: {" << v.getAsInt() << " or " << v.getAsString() << "} Of Type: " << getVariableTypeAsString(v.getType())
                        << " Into Variable Of Type: " << getVariableTypeAsString(type) << "\n";
        }
        values.push(v._data[0]);
    }
    std::string generateOutput() {
        std::string str;

        if (type == VariableType::NONE || type == VariableType::AUTO) {
            if (values.empty())
                std::cerr << "No Type Specified For Variable: " << name << "\n";
            else if (!values.empty())
                type = values[0].one;
        }

        str += getVariableTypeAsCType(type, false) + " ";

        str += name;

        if (values.size() == 1) {
            str += " = " + ((values[0].one == VariableType::STRING_TYPE) ? values[0].two.string : std::to_string(values[0].two.integer) );
        }
        else if (values.size() > 1) {
            str += "[] = {";
            for (int i = 0; i < values.size(); i++) {
                // Check For String Emptiness
                if (type == VariableType::STRING_TYPE && values[i].one == VariableType::STRING_TYPE ) {
                    continue;
                }
                str += ((type == VariableType::STRING_TYPE) ? values[i].two.string : std::to_string(values[i].two.integer));
                if (i < values.size() - 1) str += ", ";
            }
            str += "}";
        }

        if (values.empty()) str += "; // Warning: Uninitialized Variable\n";
        else str += ";\n";

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

    std::string getName() {
        return name;
    }
    std::vector<Tokenizer::Token> getInitializer() {
        return initializer;
    }
    std::string generateOutput() {
        std::string str;
        for (auto& i : initializer) {
            using TokenType = Tokenizer::MainToken;
            switch (i.token) {
                case TokenType::NAMED: str += i.tokenData; break;
                default: str += i.tokenData; break;
            }
        }
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
*/



#endif //CLL_VARIABLEHANDLER_HPP
