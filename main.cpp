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



#define DEBUG

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

    std::vector<BaseDataHandler> baseData;
    auto tokenGroups = GetTokenGroups(Tokenizer);
    for (auto& i : tokenGroups) {
        baseData.emplace_back(i);
        //i.printGroup();
    }
    for (auto& i : baseData) {
        i.print();
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
