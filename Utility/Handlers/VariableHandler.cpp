//
// Created by Owner on 11/8/2022.
//

#include "VariableHandler.hpp"

VariableHandler::VariableHandler(const BaseDataHandler &handler) {
    _name = handler._name;
    _type = handler._type;
    _values = handler._values;
    _initializerToken = handler._initializerToken;
}

void VariableHandler::print() const {
    std::cout << "[.Variable." << getVariableTypeAsString(_type) << ".] '" << _name << "' ";
    std::cout << " {(" << Tokenizer::tokenToString(_initializerToken.token) << ") " << _initializerToken.tokenData << "}\n";
    std::cout << "\t[\n";
    for (int i = 0; i < _values.size(); i++) {
        std::cout << "\t-- (" << getVariableTypeAsString(_values[i].one) << ")\n";
        std::cout << "\t\t" << ((isIntegerType(_values[i].one)) ? std::to_string(_values[i].two.integer) : _values[i].two.string) << "\n";
    }
    std::cout << "\t]\n";
}