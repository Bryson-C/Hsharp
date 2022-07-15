
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <time.h> // To Record Time
#include "CLL/FileReader/FileReader.h"
#include "CLL/Generators/GeneratorStatements.h"

struct IR {
    enum class ID {
        VAR_TYPE,
        DIGIT,
        CUSTOM_STRING,
        OPEN_ARRAY,
        CLOSE_ARRAY,
        OPEN_BRACKET,
        CLOSE_BRACKET,
        OPEN_PAREN,
        CLOSE_PAREN,
        OPEN_GENERIC,
        CLOSE_GENERIC,
        END_STATEMENT,
        INITIALIZER,
        COMMA,
        STRAIGHT_BRACKET,
        COMMENT,
        NEW_LINE,
        BLANK,
    };
    static std::string IDToString(IR::ID id) {
        switch(id) {
            case IR::ID::VAR_TYPE: return "VAR_TYPE"; break;
            case IR::ID::CUSTOM_STRING: return "CUSTOM_STRING"; break;
            case IR::ID::DIGIT: return "DIGIT"; break;
            case IR::ID::OPEN_ARRAY: return "OPEN_ARRAY"; break;
            case IR::ID::CLOSE_ARRAY: return "CLOSE_ARRAY"; break;
            case IR::ID::OPEN_BRACKET: return "OPEN_BRACKET"; break;
            case IR::ID::OPEN_PAREN: return "OPEN_PAREN"; break;
            case IR::ID::CLOSE_PAREN: return "CLOSE_PAREN"; break;
            case IR::ID::CLOSE_BRACKET: return "OPEN_CLOSE"; break;
            case IR::ID::OPEN_GENERIC: return "CLOSE_GENERIC"; break;
            case IR::ID::CLOSE_GENERIC: return "OPEN_GENERIC"; break;
            case IR::ID::END_STATEMENT: return "END_STATEMENT"; break;
            case IR::ID::INITIALIZER: return "INITIALIZER"; break;
            case IR::ID::COMMA: return "COMMA"; break;
            case IR::ID::STRAIGHT_BRACKET: return "STRAIGHT_BRACKET"; break;
            case IR::ID::COMMENT: return "COMMENT"; break;
            case IR::ID::NEW_LINE: return "NEW_LINE"; break;
            case IR::ID::BLANK: return "BLANK"; break;
        }
    }
    IR::ID id;
    std::string data;
    void print() { printf("%s: \'%s\'\n", IDToString(id).c_str(), data.c_str()); }
    std::string to_string() { return {IDToString(id) + ": \'" + data + "\'"};}

};


using IRvec = std::vector<IR>;



struct Block {
private:
public:
    IRvec statement;
    uint32_t childCount = 0;
    size_t childSize = 0;
    Block* child = nullptr;


    void allocate(size_t count)
    {
        child = new Block[count];
        childSize = count;
    }
    void free() { delete[] child; }

    void print()
    {
        printf("Block:\n"); for (IR& ir : statement) printf("  | %s\n", ir.to_string().c_str());
        if (child != nullptr && childCount > 0) {
            printf("  Child:\n");
            for (IR& ir : statement) printf("    | %s\n", ir.to_string().c_str());
        }
    }

    void writeToFile(std::ofstream& file)
    {
        file << "Block:\n";
        for (IR& ir : statement) file << "  | " << ir.to_string() << "\n";
        if (childCount > 0 && child != nullptr) {
            for (int cc = 0; cc < childCount; cc++)
            {
                if (!child[cc].statement.empty())
                {
                    file << "  Child:\n";
                    for (int sc = 0; sc < child[cc].statement.size(); sc++)
                        file << "    | " << child[cc].statement[sc].to_string() << "\n";
                }
            }
        }
        file << "\n";
    }
};

// Keywords For The Language
std::map<std::string, IR> InstructionTable {
        {"Int",  {IR::ID::VAR_TYPE, "Int"}},
        {"Void", {IR::ID::VAR_TYPE, "Void"}},
        {"String", {IR::ID::VAR_TYPE, "String"}},
        {";", {IR::ID::END_STATEMENT, ";"}},
        {"=", {IR::ID::INITIALIZER, "="}},
        {",", {IR::ID::COMMA, ","}},
        {"{", {IR::ID::OPEN_BRACKET, "{"}},
        {"}", {IR::ID::CLOSE_BRACKET, "}"}},
        {"[", {IR::ID::OPEN_ARRAY, "["}},
        {"(", {IR::ID::OPEN_PAREN, "("}},
        {")", {IR::ID::CLOSE_PAREN, ")"}},
        {"]", {IR::ID::CLOSE_ARRAY, "]"}},
        {"<", {IR::ID::OPEN_GENERIC, "<"}},
        {"|", {IR::ID::STRAIGHT_BRACKET, "|"}},
        {">", {IR::ID::CLOSE_GENERIC, ">"}},
        {"//",{IR::ID::COMMENT, "//"}},
        {"\n",{IR::ID::NEW_LINE, ""}}, // returns nothings (new lines dont effect the program)
};

namespace CompilerWarnings {
    static const char* errorFormat = "\n-- Error: %s";
    static const char* warningFormat = "\n-- Warning: %s";

    static size_t invalidVarCreationCalled = 0;
    std::string InvalidVarCreation(std::string additionalInfo = "") {
        invalidVarCreationCalled++;
        std::string errCode = {"(Count: " + std::to_string(invalidVarCreationCalled) + ") " + " Invalid Creation Of A Variable"};
        printf(errorFormat, errCode.c_str());
        if (!additionalInfo.empty()) printf(":\n\t%s\n\n", additionalInfo.c_str()); else printf("\n\n");
        return errCode;
    }
    static size_t invalidVarFormatCalled = 0;
    std::string InvalidVarFormat(std::string additionalInfo = "") {
        invalidVarFormatCalled++;
        std::string errCode = {"(Count: " + std::to_string(invalidVarFormatCalled) + ") " + " Invalid Variable Format Found"};
        printf(errorFormat, errCode.c_str());
        if (!additionalInfo.empty()) printf(":\n\t%s\n\n", additionalInfo.c_str()); else printf("\n\n");
        return errCode;
    }
    static size_t missingFeatureCalled = 0;
    std::string MissingFeature(std::string additionalInfo = "") {
        missingFeatureCalled++;
        std::string errCode = {"(Count: " + std::to_string(missingFeatureCalled) + ") " + " Feature Has Yet To Be Implemented"};
        printf(errorFormat, errCode.c_str());
        if (!additionalInfo.empty()) printf(":\n\t%s\n\n", additionalInfo.c_str()); else printf("\n\n");
        return errCode;
    }
};



IR AcquireInstructionFromTable(std::map<std::string, IR> table, std::string index) {
    bool isDigit = true;
    for (char& c : index) { if (!isdigit(c)) {isDigit = false; break;} }
    if (index.empty()) {
        return {IR::ID::BLANK, index};
    }
    else if (isDigit) {
        return {IR::ID::DIGIT, index};
    }
    else if (!table.contains(index)) {
        if (index[0] == '/' && index[1] == '/')
            return {IR::ID::COMMENT, index};
        return {IR::ID::CUSTOM_STRING, index};
    }
    return table[index];
}


bool StateCheck(std::vector<IR::ID> desiredFormat, Block& block) {
    for (int i = 0; i < block.statement.size(); i++) {
        if (desiredFormat[desiredFormat.size()-1-i] != block.statement[block.statement.size()-1-i].id) return false;
    }
    return true;
}

std::vector<Block> InstructionPass(std::ofstream& file, std::vector<FileReader::WordBufferWithPos>& wordBuffer) {
    IRvec ir;
    for (auto& i : wordBuffer) { ir.push_back(AcquireInstructionFromTable(InstructionTable, i)); }

    std::vector<Block> statements;

    WriteStatementCompliantToAlias(file, "#include <stdint.h>\n");

    for (int i = 0; i < ir.size(); i++) {
        if (ir[i].id == IR::ID::VAR_TYPE) {
            Block stmt;

            // Confirmed To Be A Variable Type So It Doesnt Need To Be Checked
            stmt.statement.push_back(ir[i]);
            WriteStatementCompliantToAlias(file, ir[i].data);
            i++;

            // This Is Required To Be A Name
            stmt.statement.push_back(ir[i]);
            WriteStatementCompliantToAlias(file, ir[i].data);
            i++;

            if (stmt.statement[stmt.statement.size()-1].id != IR::ID::CUSTOM_STRING)
                CompilerWarnings::InvalidVarCreation("Name Appears To Be A Pre-Defined Keyword");

            // Required To End Variable Definition Or Initialize The Var
            // Also Where The Variable Is Check If It Is A Function
            if (IR currentIR = ir[i]; currentIR.id != IR::ID::INITIALIZER && currentIR.id != IR::ID::END_STATEMENT) {

                // This Is A Function If An Array Is Found
                if (currentIR.id == IR::ID::OPEN_ARRAY) {
                    i++;
                    stmt.statement.push_back(currentIR);
                    Block parameters;
                    parameters.statement = HereTo<IRvec, IR>(ir, i, [](IR x) { return (x.id == IR::ID::CLOSE_ARRAY); });
                    stmt.statement.push_back(ir[i]);

                    stmt.allocate(10);
                    stmt.child[stmt.childCount++] = parameters;
                    // Since A Parameter Definition Is Required To Have A Opening And Closing Symbol Push Both
                    parameters.statement.insert(parameters.statement.begin(), AcquireInstructionFromTable(InstructionTable, "["));
                    parameters.statement.insert(parameters.statement.end(), AcquireInstructionFromTable(InstructionTable, "]"));

                    for (auto& i : parameters.statement) {
                        switch (i.id) {
                            case IR::ID::OPEN_ARRAY: WriteStatement(file, "("); break;
                            case IR::ID::CLOSE_ARRAY: WriteStatement(file, ")"); break;
                            case IR::ID::VAR_TYPE: WriteStatementCompliantToAlias(file, i.data); break;
                            case IR::ID::CUSTOM_STRING:WriteStatement(file, i.data); break;
                            case IR::ID::STRAIGHT_BRACKET: WriteStatement(file, ","); break;
                            default: break;
                        }
                    }
                    i++;
                    if (ir[i].id == IR::ID::END_STATEMENT) WriteStatement(file,";\n");
                    else if (ir[i].id == IR::ID::OPEN_BRACKET) { WriteStatement(file, "{}\n"); CompilerWarnings::MissingFeature("Function Blocks Are Not Recorded; Hence They Will Not Be Written"); }

                }
                else CompilerWarnings::InvalidVarCreation("Variable Requires An Initializer Or End Statement After A Variable Name");
            }
                // Variable Was Confirmed To Have A Safe Definition
            else if (currentIR.id == IR::ID::END_STATEMENT) {
                stmt.statement.push_back(currentIR);
                if (!StateCheck({IR::ID::VAR_TYPE, IR::ID::CUSTOM_STRING, IR::ID::END_STATEMENT}, stmt)) CompilerWarnings::InvalidVarFormat("Variable Format Suspected To End, However, Doest Not Appear To Conform To The Format");
                WriteStatementCompliantToAlias(file, ";\n");
                statements.push_back(stmt);
                continue;
            }
                // Variable Was Confirmed To Have A Safe Initialization
            else if (currentIR.id == IR::ID::INITIALIZER) {
                stmt.statement.push_back(currentIR);
                if (!StateCheck({IR::ID::VAR_TYPE, IR::ID::CUSTOM_STRING, IR::ID::INITIALIZER}, stmt)) CompilerWarnings::InvalidVarFormat("Variable ");

                i++;
                Block values;
                values.statement = HereTo<IRvec, IR>(ir, i, [](IR x){ return (x.id == IR::ID::END_STATEMENT); });

                stmt.allocate(10);
                stmt.child[stmt.childCount++] = values;

                int commas = 0;
                std::vector<std::string> digits;
                for (auto& i : values.statement) {
                    if (i.id == IR::ID::COMMA) commas++;
                    else digits.push_back(i.data);
                }

                if (digits.size()>1) {
                    WriteStatement(file, "[");
                    WriteStatement(file, std::to_string(commas+1));
                    WriteStatement(file, "]");
                    WriteStatement(file, " = {");
                }
                else WriteStatement(file, "=");

                for (int loop = 0; std::string& i : digits) {
                    WriteStatement(file, i + ((loop < digits.size()-1) ? "," : ""));
                    loop++;
                }

                if (digits.size() > 1) WriteStatement(file, "};\n");
                else WriteStatement(file, ";\n");
            }

            // Give The Statement To The Context Pass
            statements.push_back(stmt);
        }
    }

    return statements;
}




int main() {

    clock_t executionStart = clock();

    std::ofstream outfile(R"(D:\Languages\CommonLanguageLibraries\out.cpp)", std::ios::trunc);
    std::ofstream irfile(R"(D:\Languages\CommonLanguageLibraries\IR.lang)", std::ios::trunc);
    FileReader reader(R"(D:\Languages\CommonLanguageLibraries\data.lang)");

    auto wordBuffer = reader.getWordBuffer();
    auto Instructions = InstructionPass(outfile, wordBuffer);

    for (auto& i : Instructions)
        i.writeToFile(irfile);

    printf("Execution Time: %lims\n", (clock()-executionStart));
}




