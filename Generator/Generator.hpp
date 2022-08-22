//
// Created by Owner on 8/20/2022.
//

#ifndef CLL_GENERATOR_HPP
#define CLL_GENERATOR_HPP

#include <string>
#include <iostream>

#include "../Tokenizer/Tokenizer.h"
#include "../DataStage/DataStage.h"


class Generator {
public:
    static std::string writeStatement(Scope& scope, std::vector<Tokenizer::Token> tokens);
    static std::string writeFunction(Function function);
    static std::string writeVariable(Variable variable);
private:
    // This Only Excepts C Headers
    std::vector<std::string> alwaysInclude = {};
public:
    Generator(std::ofstream& outfile, DataStage dataStage);
};



#endif //CLL_GENERATOR_HPP
