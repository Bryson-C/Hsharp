#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <ctype.h>

#include "Parser/Parser.h"
#include "Tokenizer/Tokenizer.h"
#include "CLL.hpp"
#include "VariableHandler.hpp"
#include "DataStage/DataStage.h"
#include "Parser/ContextParser.h"

int main() {

    std::ofstream output(R"(D:\Languages\CLL\output.c)", std::ios::trunc);

    Parser parser(R"(D:\Languages\CLL\data.lang)", Parser::Settings::RecordNewLine);



    Tokenizer tokenizer(parser);
    DataStage dataStage(tokenizer);
    dataStage.write(output);





    return 0;
}
