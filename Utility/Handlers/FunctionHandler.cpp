//
// Created by Owner on 11/8/2022.
//

#include "FunctionHandler.hpp"

FunctionHandler::FunctionHandler(const BaseDataHandler &handler) {
    _args = handler._arguments;
    _name = handler._name;
    _type = handler._type;
    _body = handler._functionBody;
}

void FunctionHandler::print() const {
    std::cout << "[.Function." << getVariableTypeAsString(_type) << ".] '" << _name << "'\n";
    std::cout << "\t{\n";
    for (int i = 0; i < _body.size(); i++) {
        switch (_body[i]._dataType) {
            case BaseDataHandler::DataHandlerType::OPERATION: {
                OperationHandler handler(_body[i]);
                handler.print();
                break;
            }
            case BaseDataHandler::DataHandlerType::VARIABLE: {
                VariableHandler handler(_body[i]);
                handler.print();
                break;
            }
            case BaseDataHandler::DataHandlerType::FUNCTION: {
                FunctionHandler handler(_body[i]);
                handler.print();
                break;
            }
            case BaseDataHandler::DataHandlerType::UNKNOWN:
                std::cerr << "Unknown Or Unhandled Handler Type!\n";
                break;
            default: break;
        }
    }
    std::cout << "\t}\n";
}