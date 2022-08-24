//
// Created by Owner on 8/20/2022.
//

#include "Generator.hpp"

std::string Generator::writeStatement(Scope& scope, std::vector<Tokenizer::Token> tokens) {
    std::string stmt;
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].token == Tokenizer::MainToken::VARIABLE_CALL) {
            auto variable = scope.variables.getVariable(tokens[i].tokenData).two;
            stmt += variable.name;
            i++;
            if (tokens[i].token == Tokenizer::MainToken::EQUALS) {
                // TODO: Actually Handle Data Given In File
                while (tokens[i].token != Tokenizer::MainToken::SEMICOLON) {
                    if (tokens[i].token == Tokenizer::MainToken::OPEN_BRACKET) { stmt += "{"; i++; continue; }
                    else if (tokens[i].token == Tokenizer::MainToken::CLOSE_BRACKET) { stmt += "}"; i++; continue; }
                    else stmt += tokens[i].tokenData;
                    i++;
                }
            } else {
                fprintf(stderr,"Applying Unknown Operation To Variable OP: '%s'\n", tokens[i].tokenData.c_str());
            }
            stmt += ";\n";
        }
        else if (tokens[i].token == Tokenizer::MainToken::FUNCTION_CALL) {
            auto function = scope.functions.getFunction(tokens[i].tokenData).two;
            stmt += function.name;
            stmt += "(";
            while (tokens[i].token != Tokenizer::MainToken::SEMICOLON) {
                stmt += tokens[i].tokenData;
                i++;
            }
            stmt += ")";
            stmt += ";\n";
        }
        else if (tokens[i].token == Tokenizer::MainToken::NEW_LINE) {
            stmt += "\n";
        }
        else {
            stmt += tokens[i].tokenData;
            i++;
            continue;
        }
    }
    return stmt;
}


// TODO: Write Rest Of Function
std::string Generator::writeFunction(Scope& scope, Function function) {
    std::string fn;
    for (int iter = 0; auto& types : function.argTypes) {
        if (types.size() > 1) fn += "// TODO: Add Support For Multiple Types In To Argument_" + std::to_string(iter) + "\n";
        iter++;
    }
    fn += Tokenizer::typeToString(function.type) + " ";
    fn += function.name + "(";
    for (int i = 0; i < function.argNames.size(); i++) {
        fn += Tokenizer::typeToString(function.argTypes[i][0]) + " ";
        fn += function.argNames[i].tokenData;
        if (i != function.argNames.size()-1) fn += ", ";
    }
    fn += ")";
    if (!function.bodyVariables.variables.empty() || !function.bodyStatements.statements.empty()) {
        //fn += "; // TODO: Implement Function Body Generation\n";
        fn += " {";
        if (!function.bodyVariables.variables.empty())
            for (auto& var : function.bodyVariables.variables)
                fn += writeVariable(scope, var);
        if (!function.bodyStatements.statements.empty())
            for (auto& statement : function.bodyStatements.statements)
                fn += writeStatement(scope, statement);
        fn += "}\n";
    } else {
        fn += "; // TODO: Add Body To Function, For Dead Code Is Sloppy\n";
    }
    return fn;
}

std::string Generator::writeVariable(Scope& scope, Variable variable) {
    std::string var;
    var += Tokenizer::typeToString(variable.type) + " ";
    var += variable.name;
    var += ((variable.initializer.size() > 1) ? "["+std::to_string(variable.initializer.size())+"]" : "");
    if (variable.initializer.empty()) {
        var += ";\n";
        return var;
    }
    else {
        var += " = ";
    }
    var += ((variable.initializer.size() > 1) ? "{" : "");
    for (int iter = 0; auto& i : variable.initializer) {
        var += std::to_string(i);
        if (iter != variable.initializer.size()-1) var += ", ";
        iter++;
    }
    var += ((variable.initializer.size() > 1) ? "};\n" : ";\n");
    return var;
}

Generator::Generator(std::ofstream& outfile, DataStage dataStage) {
    for (auto& include : alwaysInclude)
        outfile << include << "\n";

    bool hasEntry = false;
    for (auto& function : dataStage.globalScope.functions.functions) {
        if (function.name == "main") hasEntry = true;
        outfile << writeFunction(dataStage.globalScope, function);
    }
    if (!hasEntry) std::cerr << "No Entry Point In Program\n";

    for (auto& variable : dataStage.globalScope.variables.variables) {
        outfile << writeVariable(dataStage.globalScope, variable);
    }

    for (auto& statement : dataStage.globalScope.statements.statements) {
        outfile << writeStatement(dataStage.globalScope, statement);
    }
}




