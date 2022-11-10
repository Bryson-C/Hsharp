//
// Created by Owner on 11/8/2022.
//

#ifndef CLL_VARIABLEHANDLER_HPP
#define CLL_VARIABLEHANDLER_HPP

#include "BaseDataHandler.hpp"


class VariableHandler : public BaseDataHandler {
private:
    std::string _name;
    VariableType _type;
    Tokenizer::Token _initializerToken;
    ValueArray<VariableType::AUTO> _values;
public:
    explicit VariableHandler(const BaseDataHandler& handler);
    void print() const override;
};


#endif //CLL_VARIABLEHANDLER_HPP
