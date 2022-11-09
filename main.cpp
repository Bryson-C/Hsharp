#include <iostream>
#include <vector>
#include <string>
#include <map>


#include "Parser/Parser.hpp"
#include "Tokenizer/Tokenizer.hpp"
#include "CLL.hpp"
#include "Utility/Environment.hpp"
#include "Utility/BaseHandlers.hpp"
#include "Data/Scope.hpp"



#define DEBUG 1

// TODO: Add Constant-Correctness To Project
int main(int argc, const char** argv) {


    auto args = getCmdLineArguments(argc, argv);

    std::string languageFolder;
#ifdef DEBUG
    languageFolder = R"(D:\Languages\CLL\Language)";
#endif
/*
    for (const auto& i : args)
        if (std::string value = stripQuotes(i.two); i.one == "dir")
            languageFolder = ((std::filesystem::path(value).is_absolute()) ? std::filesystem::path(value) : std::filesystem::absolute(std::filesystem::path(value))).string();
*/

/*
    CompilerDirectory directory(languageFolder);
    CompilerOptions compilerOptions(directory);
*/
    


    CompilerOptions compilerOptions;






    std::ofstream output(R"(D:\Languages\CLL\Language\output.c)", std::ios::trunc);

    Parser Parser(R"(D:\Languages\CLL\Language\default.lang)");
    Tokenizer Tokenizer(Parser);

    auto tokenGroups = GetTokenGroups(Tokenizer);
    std::vector<BaseDataHandler> baseData(tokenGroups.size());
    for (int i = 0; i < tokenGroups.size(); i++) {
        baseData[i] = BaseDataHandler(tokenGroups[i]);
        if (auto handlerType = baseData[i].getHandledDataType(); handlerType == BaseDataHandler::DataHandlerType::OPERATION) {
            OperationHandler operation(baseData[i]);
        } else if (handlerType == BaseDataHandler::DataHandlerType::FUNCTION) {
            FunctionHandler function(baseData[i]);
        } else if (handlerType == BaseDataHandler::DataHandlerType::VARIABLE) {
            VariableHandler variable(baseData[i]);
        }
        baseData[i].print();
    }






/*
    auto program = DetermineScope(compilerOptions, tokenGroups);

    std::cout << "-- NEW SCOPE --\n";
    for (auto& var : program.variables) {
        std::cout << "\t" << getVariableTypeAsString(var.getType()) << " " << var.getName() << "\n";
        output << var.generateOutput();
    }
    for (auto& func : program.functions) {
        std::cout << "\t" << "[.FUNCTION.] -> " << getVariableTypeAsString(func.getType()) << " " << func.getName() << "\n";
        output << func.generateOutput();
    }
    for (auto& op : program.operations) {
        std::cout << "\t" << "[.OPERATION.] -> " << op.getName();
        for (auto& tok : op.getInitializer()) {
            std::cout << tok.tokenData << " ";
        }
        std::cout << '\n';
        output << op.generateOutput();
    }
    std::cout << "-- END SCOPE --\n";
*/

    return 0;
}
