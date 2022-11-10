//
// Created by Owner on 11/8/2022.
//

#ifndef CLL_FUNCTIONHANDLER_HPP
#define CLL_FUNCTIONHANDLER_HPP

#include "BaseDataHandler.hpp"
#include "OperationHandler.hpp"
#include "VariableHandler.hpp"

class FunctionHandler : public BaseDataHandler {
private:
    std::string _name;
    VariableType _type;
    using FunctionArgument = Triple<VariableType, std::string, ValueArray<VariableType::AUTO>>;
    std::vector<FunctionArgument> _args;
    std::vector<BaseDataHandler> _body;
public:
    explicit FunctionHandler(const BaseDataHandler& handler);
    void print() const override;
};


#endif //CLL_FUNCTIONHANDLER_HPP
