//
// Created by Owner on 7/31/2022.
//

#ifndef CLL_BASEDATAHANDLER_HPP
#define CLL_BASEDATAHANDLER_HPP

// TODO: Migrate Code To A Cleaner Code Base


#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <any>

#include "../../CLL.hpp"
#include "../../Tokenizer/Tokenizer.hpp"


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
        Any() = default;
        explicit Any(int8_t value, VariableType typeOverride = VariableType::INT32_TYPE) : result(true), type(typeOverride) { _data.integer = value; }
        explicit Any(int16_t value, VariableType typeOverride = VariableType::INT32_TYPE) : result(true), type(typeOverride) { _data.integer = value; }
        explicit Any(int32_t value, VariableType typeOverride = VariableType::INT32_TYPE) : result(true), type(typeOverride) { _data.integer = value; }
        explicit Any(int64_t value, VariableType typeOverride = VariableType::INT32_TYPE) : result(true), type(typeOverride) { _data.integer = value; }
        explicit Any(uint8_t value, VariableType typeOverride = VariableType::UINT32_TYPE) : result(true), type(typeOverride) { _data.uinteger = value; }
        explicit Any(uint16_t value, VariableType typeOverride = VariableType::UINT32_TYPE) : result(true), type(typeOverride) { _data.uinteger = value; }
        explicit Any(uint32_t value, VariableType typeOverride = VariableType::UINT32_TYPE) : result(true), type(typeOverride) { _data.uinteger = value; }
        explicit Any(uint64_t value, VariableType typeOverride = VariableType::UINT32_TYPE) : result(true), type(typeOverride) { _data.uinteger = value; }

        explicit Any(std::string value, VariableType typeOverride = VariableType::STRING_TYPE) : type(typeOverride)
        { (strcpy_s(_data.string, value.c_str()) == 0) ? result = true : result = false; }

        [[deprecated("Unused")]] Any& operator+=(Any& value) {
            if (type == VariableType::STRING_TYPE) {
                std::cerr << "Cannot Apply Operation '+=' On A Non Integer Type\n";
                return *this;
            }
            if (type == VariableType::INT32_TYPE)
                _data.integer += (value.type == VariableType::INT32_TYPE) ? value._data.integer : value._data.uinteger;
            else
                _data.uinteger += (value.type == VariableType::INT32_TYPE) ? value._data.integer : value._data.uinteger;
            return *this;
        }


        template<VariableType> friend class ValueArray;

    };


    // TODO: Finish Any Value Re-work
    template<VariableType DefaultDataType = VariableType::AUTO>
    class ValueArray {
    private:
        std::vector<Any> _data{};
    public:
        ValueArray() = default;

        explicit ValueArray(int32_t number, VariableType overrideType = DefaultDataType) {
            _data.emplace_back(number,
                               (DefaultDataType == VariableType::AUTO) ? VariableType::INT32_TYPE : overrideType);
        }

        explicit ValueArray(int64_t number, VariableType overrideType = DefaultDataType) {
            _data.emplace_back(number,
                               (DefaultDataType == VariableType::AUTO) ? VariableType::INT64_TYPE : overrideType);
        }

        explicit ValueArray(std::string string, VariableType overrideType = DefaultDataType) {
            if (DefaultDataType != VariableType::AUTO && DefaultDataType != VariableType::STRING_TYPE)
                std::cerr << "Cannot Push Data Type Of: " << (int) overrideType << " To String Variable\n";
            _data.emplace_back(string, VariableType::STRING_TYPE);
        }

        Duople<VariableType, Any::multiTypeStorage> operator[](size_t index) const {
            return {_data[index].type, _data[index]._data};
        }

        [[nodiscard("")]] Any getRawData(size_t index) const { return _data[index]; }

        [[nodiscard("")]] VariableType getType(size_t index = 0) const { return _data[index].type; }

        [[nodiscard("")]] std::string getAsString(size_t index = 0) const { return _data[index]._data.string; }

        [[nodiscard("")]] int64_t getAsInt(size_t index = 0) const { return _data[index]._data.integer; }

        [[nodiscard("")]] uint64_t getAsUInt(size_t index = 0) const { return _data[index]._data.uinteger; }

        template<VariableType DataType = DefaultDataType, typename Data>
        void push(Data data) {
            Any value(data);
            if (DataType != VariableType::AUTO) value.type = DataType;
            _data.push_back(value);
        }

        void print(bool printAll = true, size_t index = 0) const {
            int valuesPrinted = 0;
            for (size_t i = index; i < _data.size(); i++) {
                if (!printAll && valuesPrinted > 0) return;
                switch (_data[i].type) {
                    case VariableType::INT32_TYPE:
                        std::cout << std::to_string(_data[index]._data.integer);
                        valuesPrinted++;
                        break;
                    case VariableType::UINT32_TYPE:
                        std::cout << std::to_string(_data[index]._data.uinteger);
                        valuesPrinted++;
                        break;
                    case VariableType::STRING_TYPE:
                        std::cout << _data[index]._data.string;
                        valuesPrinted++;
                        break;
                    default: break;
                }
            }
        }
        void println(bool printAll = true, size_t index = 0) const {
            print(printAll, index);
            printf("\n");
        }


        bool empty() const { return _data.empty(); }

        size_t size() const { return _data.size(); }

    };


}


// TODO: For Code-Gen Ignore Commas Separating Values, This Is Just A Byproduct Of Bad Planning


class BaseDataHandler {
public:
    enum class DataHandlerType {
        UNKNOWN,
        VARIABLE,
        FUNCTION,
        OPERATION,
    };

private:
    friend class OperationHandler;
    friend class FunctionHandler;
    friend class VariableHandler;

    static std::string dataHandlerTypeToString(DataHandlerType type);
    DataHandlerType _dataType;


    std::string _name;
    VariableType _type;
    Tokenizer::Token _initializerToken;
    bool typeInitialized = false;
    ValueArray<VariableType::AUTO> _values;
    std::vector<Tokenizer::Token> _operations;
    std::vector<Triple<VariableType, std::string, ValueArray<VariableType::AUTO>>> _arguments;
    std::vector<BaseDataHandler> _functionBody;


public:
    BaseDataHandler() = default;
    explicit BaseDataHandler(TokenGroup& group);

    [[nodiscard("")]] DataHandlerType getHandledDataType() const;
    virtual void print() const;
};

// This Will Evaluate Expressions In A Left To Right Order
Duople<bool, Any> evaluateExpression(TokenGroup& group);



#endif //CLL_BASEDATAHANDLER_HPP
