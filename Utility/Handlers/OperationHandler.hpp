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
        None,
        Set,
    } _action;
    std::string operationActionTypeToString(OperationActionType action) const {
        switch (action) {
            case OperationActionType::None: return "None";
            case OperationActionType::Set: return "Set";
            default: return "None";
        }
    }

    [[nodiscard("")]] static std::string operationActionToString(OperationActionType action);

    std::string _operand;
    std::vector<Tokenizer::Token> _operations;
public:
    OperationHandler() = default;
    explicit OperationHandler(const BaseDataHandler& baseDataHandler);
    std::string generateOutput();
    void print() const override;
};



#endif //CLL_OPERATIONHANDLER_HPP
