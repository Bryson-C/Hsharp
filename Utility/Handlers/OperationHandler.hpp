//
// Created by Owner on 10/26/2022.
//

#ifndef CLL_OPERATIONHANDLER_HPP
#define CLL_OPERATIONHANDLER_HPP

#include "BaseDataHandler.hpp"


// TODO: Figure Out What To Do With The Data
class OperationHandler : public BaseDataHandler {
private:
    enum class OperationActionType {
        Set,
    } _action;

    [[nodiscard("")]] static std::string operationActionToString(OperationActionType action);

    std::vector<Tokenizer::Token> _operation;
public:
    OperationHandler() = default;
    explicit OperationHandler(const BaseDataHandler& baseDataHandler);
};



#endif //CLL_OPERATIONHANDLER_HPP
