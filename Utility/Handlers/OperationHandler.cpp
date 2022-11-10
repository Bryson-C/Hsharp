//
// Created by Owner on 10/26/2022.
//

#include "OperationHandler.hpp"

[[nodiscard("")]] std::string OperationHandler::operationActionToString(OperationActionType action) {
    switch (action) {
        case OperationActionType::Set: return "Set";
        default: return "";
    }
}

OperationHandler::OperationHandler(const BaseDataHandler& baseDataHandler) {
    if (baseDataHandler._initializerToken.token == Tokenizer::MainToken::EQUALS) {
        _action = OperationActionType::Set;
    }
    _operations = baseDataHandler._operations;
    _operand = baseDataHandler._name;
}

// TODO: Add Variable Array To Get Variable Data
std::string OperationHandler::generateOutput() {
    if (_action == OperationActionType::None) return "";
    std::string str = _operand;
    switch (_action) {
        case OperationActionType::Set: str += " = "; break;
        default: break;
    }

    for (const auto& i : _operations) {
        switch (i.token)
        {
            // TODO: Add Variable Data Getting Instead Of Putting The Variable Name In The Initializer
            case Tokenizer::MainToken::NAMED: str += i.tokenData + " "; break;

            case Tokenizer::MainToken::INTEGER:
            case Tokenizer::MainToken::STRING: str += i.tokenData + " "; break;

            default: str += i.tokenData + " "; break;
        }
    }
    str += ";";
    return str;
};

void OperationHandler::print() const {
    std::cout << "[.Operation." << operationActionToString(_action) << ".] '" << _operand << "'\n";
    for (const auto& i : _operations) {
        std::cout << "\t-- (" << Tokenizer::tokenToString(i.token) << ")\n\t\t =>  " << i.tokenData << "\n";
    }
    std::cout << "\n";
}