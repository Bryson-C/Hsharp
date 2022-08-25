//
// Created by Owner on 8/12/2022.
//

#include "DataStage.h"

DataStage::DataStage(std::vector<Tokenizer::Token> tokens) { globalScope = run(tokens); }
DataStage::DataStage(Tokenizer tokenizer) { globalScope = run(tokenizer); }
DataStage::~DataStage() {
    for (auto& var : globalScope.variables.variables) {
        //var.print();
    }
    for (auto& func : globalScope.functions.functions) {
        //func.print();
    }
    for (auto& stmt : globalScope.statements.statements) {
        // Print Statement
    }
}



Scope DataStage::run(std::vector<Tokenizer::Token> tokens) {
    bool LOG_UNHANDLED_RESULT = false;
    using TokenType = Tokenizer::MainToken;

    Scope scope;

    for (int i = 0; i < tokens.size(); i++) {
        Variable var;
        Function func;
        if (isAny(tokens[i].token, {TokenType::INT_TYPE})) {
            // Checks If Vector Has
            // 1. A Type (Which It Should)
            // 2. A Name
            // 3. Ending Statement (Either `;` Or A Value)
            //expects(tokens, i, {{TokenType::INT_TYPE}, {TokenType::NAMED}, {TokenType::SEMICOLON, TokenType::EQUALS, TokenType::OPEN_BRACKET}});

            var.type = tokens[i].token;
            func.type = tokens[i].token;
            i++;

            if (tokens[i].token != TokenType::NAMED) std::cerr << "Named String '" << tokens[i].tokenData << "' Does Not Comply With DataStage\n";

            var.name = tokens[i].tokenData;
            func.name = tokens[i].tokenData;
            i++;
            // A Semi-Colon Indicates That The Data Must Be A Uninitialized Variable
            if (tokens[i].token == TokenType::SEMICOLON) {
                var.initializer = {};
                scope.variables.newVariable(var);
                continue;
            }
            // An Equals Means That The Data Is An Initialized Variable
            else if (tokens[i].token == TokenType::EQUALS) {
                bool usesBrackets = false;
                if (tokens[i+1].token == TokenType::OPEN_BRACKET) {
                    usesBrackets = true;
                    i++;
                } else if (tokens[i+1].token == TokenType::NAMED) {
                    i++;
                    usesBrackets = false;

                    auto functionResult = scope.functions.getFunction(tokens[i].tokenData);
                    auto variableResult = scope.variables.getVariable(tokens[i].tokenData);

                    // If It Is A Function
                    if (functionResult.one) {
                        //auto function = functionResult.two;
                        // I Dont Really Know What To Do Here (What To Do When Var Is Initialized With Function?)
                    } else {
                        std::cerr << tokens[i].filePosition.errorString() << "Absolutely No Function Of The Name '" << tokens[i].tokenData << "' Exists\n";
                    }

                }
                i++;

                std::vector<Tokenizer::Token> initializer;
                while ((usesBrackets)?(tokens[i].token != TokenType::CLOSE_BRACKET) : (tokens[i].token != TokenType::SEMICOLON)) {
                    if (tokens[i].token == TokenType::COMMA) {
                        if (!usesBrackets) std::cerr << tokens[i].filePosition.errorString() << "Arrays Need '[' And ']' For Open And Closing\n";
                        i++;
                        continue;
                    }

                    else if (Tokenizer::isOperation(tokens[i+1])) {
                        i++;
                        std::vector<int64_t> left, right;
                        Tokenizer::Token op = tokens[i];

                        if (tokens[i-1].token == TokenType::NAMED)
                            left = scope.variables.getVariable(tokens[i-1].tokenData).two.initializer;
                        else left = {std::stoll(tokens[i-1].tokenData)};

                        if (tokens[i+1].token == TokenType::NAMED)
                            right = scope.variables.getVariable(tokens[i+1].tokenData).two.initializer;
                        else
                            right = {std::stoll(tokens[i+1].tokenData)};


                        OperationStack stack;
                        stack.pushInt(left);
                        stack.pushInt(right);
                        auto opString = OperationStack::opFromString(op.tokenData);
                        auto opResult = stack.pushOp(opString);
                        if (opResult.success) {
                            for (auto& res : opResult.result)
                                push(var.initializer, res);
                        } else {
                            std::cerr << tokens[i].filePosition.errorString() << "Operation Error\n";
                            panic();
                        }
                        i+=2;
                        continue;
                    }
                    if (tokens[i].token == TokenType::INTEGER)
                        push(var.initializer, std::stoll(tokens[i].tokenData));
                    i++;
                }
                scope.variables.newVariable(var);
                continue;
            }

            // A Bracket Indicates That The Data Must Be A Argument Array, Meaning A Function
            else if (tokens[i].token == TokenType::OPEN_BRACKET) {
                i++;
                // Arguments
                std::vector<std::vector<Tokenizer::Token>> argTypes;
                std::vector<Tokenizer::Token> argNames;

                std::vector<Tokenizer::Token> currentArgTypes;

                // Get The Arguments
                while (tokens[i].token != TokenType::CLOSE_BRACKET) {
                    if (isAny(tokens[i].token, {TokenType::INT_TYPE, TokenType::STRING_TYPE})) {
                        currentArgTypes.push_back(tokens[i]);
                    }
                    else if (tokens[i].token == TokenType::OP_OTHER) {
                    }
                    else if (tokens[i].token == TokenType::NAMED) {
                        argTypes.push_back(currentArgTypes);
                        argNames.push_back(tokens[i]);
                        currentArgTypes.clear();
                    }
                    i++;
                }
                // Push The Arguments
                func.argTypes = argTypes;
                func.argNames = argNames;
                i++;
                // Get The Body Of The Function
                std::vector<Tokenizer::Token> funcBody;
                if (tokens[i].token == TokenType::OPEN_BRACE) {
                    i++;
                    while (tokens[i].token != TokenType::CLOSE_BRACE) {
                        funcBody.push_back(tokens[i]);
                        i++;
                    }
                } else if (tokens[i].token == TokenType::BIG_ARROW) {
                    i++;
                    funcBody.push_back({TokenType::RETURN, "return", tokens[i].filePosition});
                    while (tokens[i].token != TokenType::SEMICOLON) {
                        funcBody.push_back(tokens[i]);
                        i++;
                    }
                    funcBody.push_back(tokens[i]);
                }
                if (!funcBody.empty()) {
                    auto insides = run(funcBody);
                    //func.bodyStatements = insides.statements;
                    //func.bodyVariables = insides.variables;
                    printf("{ \n\t%s\n}\n In Function -- %s\n",Generator::WriteScope(insides).c_str(), func.name.c_str());
                }

                scope.functions.newFunction(func);
            }
        }
        else if (tokens[i].token == TokenType::NAMED) {
            // TODO: Found A Variable To Modify, Or Some Function Is Called
            auto function = scope.functions.getFunction(tokens[i].tokenData);
            auto variable = scope.variables.getVariable(tokens[i].tokenData);

            if (variable.one && function.one)
                std::cerr << tokens[i].filePosition.errorString()
                          << "Name Collision Of Function '" << function.two.name
                          << "' And Variable '" << variable.two.name << "'\n";
            else if (function.one) tokens[i].token = TokenType::FUNCTION_CALL;
            else if (variable.one) tokens[i].token = TokenType::VARIABLE_CALL;

            std::vector<Tokenizer::Token> stmt;
            do {
                stmt.push_back(tokens[i]);
                i++;
            } while (tokens[i].token != TokenType::SEMICOLON);
            stmt.push_back(tokens[i]);
            scope.statements.newStatement(stmt);
            i++;
        }
        else {
            if (LOG_UNHANDLED_RESULT)
                fprintf(stderr, "%s Unknown DataStage Case: '%s'\n", tokens[i].filePosition.errorString().c_str(), tokens[i].tokenData.c_str());
        }
    }

    return scope;
}
Scope DataStage::run(Tokenizer tokenizer) {
    return run(tokenizer.getTokens());
}

void DataStage::write(std::ofstream &outfile) {
    outfile << Generator::WriteScope(globalScope);
}


std::string Generator::WriteStatement(Scope& scope, std::vector<Tokenizer::Token> tokens) {
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
std::string Generator::WriteFunction(Scope& scope, Function function) {
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
    if (function.bodyVariables.variables.size() > 0 || function.bodyStatements.statements.size() > 0) fn += " {";
    if (!function.bodyVariables.variables.empty())
        for (auto& var : function.bodyVariables.variables)
            fn += WriteVariable(scope, var);
    if (!function.bodyStatements.statements.empty())
        for (auto& statement : function.bodyStatements.statements)
            fn += WriteStatement(scope, statement);
    if (function.bodyVariables.variables.size() > 0 || function.bodyStatements.statements.size() > 0) fn += "}\n";
    else fn += ";\n";

    return fn;
}

std::string Generator::WriteVariable(Scope& scope, Variable variable) {
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

std::string Generator::WriteScope(Scope &scope) {
    std::string str;
    for (auto& fn : scope.functions.functions) {
        str += WriteFunction(scope, fn);
    }
    for (auto& var : scope.variables.variables) {
        str += WriteVariable(scope, var);
    }
    for (auto& stmt : scope.statements.statements) {
        str += WriteStatement(scope, stmt);
    }
    return str;
}

