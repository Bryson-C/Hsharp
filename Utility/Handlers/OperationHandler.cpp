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

OperationHandler::OperationHandler(const BaseDataHandler &baseDataHandler) {
    if (baseDataHandler._initializerToken.token == Tokenizer::MainToken::EQUALS) {
        _action = OperationActionType::Set;
    }
    _operation = baseDataHandler._operations;
}

