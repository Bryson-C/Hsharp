//
// Created by Owner on 8/20/2022.
//

#include "Generator.hpp"

std::string Generator::writeStatement(Scope& scope, std::vector<Tokenizer::Token> tokens) {
    std::string stmt;
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].token == Tokenizer::MainToken::VARIABLE_CALL) {
            auto variableResult = scope.variables.getVariable(tokens[i].tokenData);

            stmt += variableResult.two.name;
            i++;
            if (tokens[i].token == Tokenizer::MainToken::EQUALS)
                stmt += " = ";
            else {
                fprintf(stderr,"%s I Dont Know What To Do Here '%s'\n", tokens[i].filePosition.errorString().c_str(), tokens[i].tokenData.c_str());
            }
            i++;
            if (tokens[i].token == Tokenizer::MainToken::FUNCTION_CALL) goto FUNCTION_RECORD_LABEL;
            else if (tokens[i].token == Tokenizer::MainToken::VARIABLE_CALL) {
                while (tokens[i].token != Tokenizer::MainToken::SEMICOLON) i++;
            } else {
                while (tokens[i].token != Tokenizer::MainToken::SEMICOLON) {
                    stmt += tokens[i].tokenData;
                    i++;
                }
                stmt += ";";
            }
            stmt += "\n";
        } else if (tokens[i].token == Tokenizer::MainToken::FUNCTION_CALL) {
            FUNCTION_RECORD_LABEL:
            auto functionResult = scope.functions.getFunction(tokens[i].tokenData);
            i++;

            stmt += functionResult.two.name;
            while (tokens[i].token != Tokenizer::MainToken::SEMICOLON) {
                if (tokens[i].token == Tokenizer::MainToken::CLOSE_BRACKET) {
                    stmt += ");\n";
                    break;
                } else if (tokens[i].token == Tokenizer::MainToken::OPEN_BRACKET) {
                    stmt += "(";
                    i++;
                    continue;
                } else if (tokens[i].token == Tokenizer::MainToken::COMMA) {
                    stmt += ",";
                    i++;
                    continue;
                } else {
                    stmt += tokens[i].tokenData;
                    i++;
                    continue;
                }
            }

        } else if (tokens[i].token == Tokenizer::MainToken::NEW_LINE) {
            stmt += "\n";
        }
        /*else {
            stmt += tokens[i].tokenData + " ";
            continue;
        }*/
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
    if (!function.body.empty()) {
        //fn += "; // TODO: Implement Function Body Generation\n";
        fn += " {";
        fn += writeStatement(scope, function.body);
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
    for (auto& function : dataStage.scope.functions.functions) {
        if (function.name == "main") hasEntry = true;
        outfile << writeFunction(dataStage.scope, function);
    }
    if (!hasEntry) std::cerr << "No Entry Point In Program\n";

    for (auto& variable : dataStage.scope.variables.variables) {
        outfile << writeVariable(dataStage.scope, variable);
    }

    for (auto& statement : dataStage.scope.statements.statements) {
        outfile << writeStatement(dataStage.scope, statement);
    }
}




