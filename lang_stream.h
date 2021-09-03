#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <optional>

struct var {
    std::string name;
    std::string type;
    std::vector<int> val;

    void print() {
        printf("Name: %s | ", name.c_str());
        if (val.size() > 1) {
            printf("Value: [");
            for (auto i : val)  printf("%i, ", i);
            printf("]\n");
        } else printf("Value: %i\n", val[0]);
    }
};

typedef std::vector<std::string> FileContent;
typedef std::vector<std::string> LineContent;


std::string getLangFile(std::string stem);

bool contains(std::string str, std::string substr);

bool contains(FileContent content, std::string substr);

LineContent getFileContent(std::string path, std::optional<int> lines = 0);

std::vector<var> loadStateVariables(std::string path);

LineContent get_all_keywords_in_line(FileContent content, int line);

std::string findVariableName(std::vector<std::string> content, int i);

void findVariable(std::vector<std::string> content, int& line, std::vector<var>& variables);

void modifyVariable(FileContent content, int& line, std::vector<var>& variables, std::ofstream* stateFile = nullptr);


class program {
    private:
        std::vector<var> variables;
        FileContent content;

        int lines = 0;
        std::string file;
        std::string m_program;
        std::vector<std::string> useProgram; // change this type

    public:
        program();
        program(std::string path);
        void init(std::string path);
        inline FileContent          getContent() { return content; }
        inline int                  getLines() { return lines; }
        inline std::string          getProgram() { return m_program; }
        inline std::vector<var>     getVariables() { return variables; }
        inline void                 addVar(std::string name, std::vector<int> value, std::string type) 
                                          { variables.push_back({name, type, value}); }
        inline void                 getInfo() { printf("File: %s\n\tProgram: %s\n\tLines: %s\n",file.c_str(), m_program.c_str(), lines); }
        void                        initVariables();
};

class lstream {
    private: 
        program m_program;


    public:
        lstream(std::string);
        //void addProgram();
        
        FileContent getContent();
        LineContent getLine(int line);
        void printVariables();

        inline program getProgram() { return m_program; }

};

