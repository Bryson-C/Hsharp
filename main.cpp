#include <iostream>
#include <vector>
#include <string>
#include <map>


#include "Parser/Parser.hpp"
#include "Tokenizer/Tokenizer.hpp"
#include "CLL.hpp"
#include "Utility/Enviornment.hpp"
#include "Utility/VariableHandler.hpp"
#include "Data/Scope.hpp"





int main(int argc, const char** argv) {

    auto args = getCmdLineArguments(argc, argv);
    for (const auto& i : args) std::cout << "Option: " << i.one << "  |  Value: " << i.two << "\n";
    CompilerOptions compilerOptions(args);

    CompilerDirectory directory(compilerOptions.baseDirectory, "");

    compilerOptions.linkDirectory(directory);

    return 0;




    std::ofstream output(R"(D:\Languages\CLL\output.c)", std::ios::trunc);

    Parser Parser(R"(D:\Languages\CLL\default.lang)");
    Tokenizer Tokenizer(Parser);

    auto tokenGroups = GetTokenGroups(Tokenizer);
    for (auto& i : tokenGroups) i.printGroup();


    auto program = DetermineScope(compilerOptions, tokenGroups);


/*
    std::vector<Variable> variables;
    std::vector<Function> functions;
    for (auto& i : tokenGroups) {

        Variable var;
        Function func;
        using TokenType = Tokenizer::MainToken;

        std::string name;
        bool namedDefinition = true;

        for (int iteration = 0; auto& tok : i.getTokens()) {
            if (tok.token == TokenType::INT_TYPE)
                {
                    var.setType(VariableType::INT32_TYPE);
                    func.setType(VariableType::INT32_TYPE);
                }
            if (tok.token == TokenType::STRING_TYPE)
                {
                    var.setType(VariableType::STRING_TYPE);
                    func.setType(VariableType::STRING_TYPE);
                }
            if (tok.token == TokenType::NAMED) {
                for (auto& alreadyDefined : variables) {
                    if (alreadyDefined.getName() == tok.tokenData) {
                        tok.token = Tokenizer::MainToken::VARIABLE_CALL;
                        namedDefinition = false;
                    }
                }
                for (auto& alreadyDefined : functions) {
                    if (alreadyDefined.getName() == tok.tokenData) {
                        tok.token = Tokenizer::MainToken::FUNCTION_CALL;
                        namedDefinition = false;
                    }
                }
                if (namedDefinition)
                    {
                        var.setName(tok.tokenData);
                        func.setName(tok.tokenData);
                    }
            }
            if (tok.token == TokenType::AUTO_TYPE)
                {
                    var.setType(VariableType::AUTO);
                    func.setType(VariableType::AUTO);
                }

            iteration++;
        }
        Variable argument;
        for (auto& arg : i.getArguments()) {
            if (arg.token == TokenType::INT_TYPE)
                {
                    argument.setType(VariableType::INT32_TYPE);
                }
            else if (arg.token == TokenType::STRING_TYPE)
                {
                    argument.setType(VariableType::STRING_TYPE);
                }
            else if (arg.token == TokenType::NAMED)
                argument.setName(arg.tokenData);
            else if (arg.token == TokenType::COMMA) {
                func.pushArgument(argument);
                argument = Variable();
            }
        }
        if (argument.getType() != VariableType::NONE) {
            func.pushArgument(argument);
            argument = Variable();
        }

        for (int iter = 0; auto &init: i.getInitializer()) {
            if (!i.isFunctionType()) {
                Value val;

                if (init.token == TokenType::INTEGER) val = Value(std::stoi(init.tokenData));
                else if (init.token == TokenType::STRING) val = Value(init.tokenData);

                var.push(val);
            } else {

            }
            iter++;
        }

        if (namedDefinition && !i.isFunctionType()) {
            output << var.generateOutput();
            variables.push_back(var);
        } else if (namedDefinition && i.isFunctionType()) {
            output << func.generateOutput();
            functions.push_back(func);
        }
    }

*/

    std::cout << "-- NEW SCOPE --\n";
    for (auto& var : program.variables) {
        std::cout << "\t" << getVariableTypeAsString(var.getType()) << " " << var.getName() << "\n";
        output << var.generateOutput();
    }
    for (auto& func : program.functions) {
        std::cout << "\t" << "[.FUNCTION.] -> " << getVariableTypeAsString(func.getType()) << " " << func.getName() << "\n";
        output << func.generateOutput();
    }
    std::cout << "-- END SCOPE --\n";


    return 0;
}
