#include "lang_stream.h"


std::string getLangFile(std::string stem) {
    for (auto i : std::filesystem::directory_iterator(std::filesystem::current_path())) {
        if (i.path().stem().string().find(stem) < i.path().string().size())
            return i.path().string();
    }
}

bool contains(std::string str, std::string substr) {
    if (str.find(substr) < str.size()) return true;
    return false;
} 

bool contains(FileContent content, std::string substr) {
    for (auto i : content) if (contains(i, substr)) return true;
    return false;
}

FileContent getFileContent(std::string path, std::optional<int>lines) {
    FileContent content;
    std::fstream file(path);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            lines.value()++;
            content.push_back(line);
        }
    }
    //printf("Size Of %s: %i\n", path.c_str(), content.size());
    return content;
}

std::vector<var> loadStateVariables(std::string path) {
    FileContent content = getFileContent(path);
    std::vector<var> states{};
    for (int i = 0; i < content.size(); i++) {
        var v;
        int loop = 0;
        while (content[i][loop] != ':') {v.name += content[i][loop]; loop++; }
        loop++;
        while (content[i][loop] != '\n') {v.val[0] += content[i][loop]; loop++; }
        states.push_back(v);
    }
    return states;
}

LineContent get_all_keywords_in_line(FileContent content, int line) {
    LineContent Line;
    std::string charBuffer;
    for (size_t i = 0; i < content[line].size(); i++) {
        if (content[line][i] == ' ' || content[line][i] == ';') { Line.push_back(charBuffer); charBuffer.clear(); continue; }
        charBuffer += content[line][i];
    }
    if (contains(Line, "//")) return {" "};
    return Line;
}

std::string findVariableName(std::vector<std::string> content, int i) {
    std::string varname;
    int k = content[i].find("{")+1;
    while (content[i][k] != '}') { varname += content[i][k]; k++; }
    return varname;
}

void findVariable(std::vector<std::string> content, int& line, std::vector<var>& variables) {
    auto keywords = get_all_keywords_in_line(content, line);
    if (contains(keywords, "set")) {
        var v;
        v.name = findVariableName(content, line);
        if (contains(keywords,"new") && contains(keywords, "int")) {
            v.type = "int";
            std::string val = keywords[keywords.size()-1];
            // cant print val;
        }
        else {
            if (contains(keywords, "\"")) v.type = "string";
            for (auto i : keywords[keywords.size()-1]) {
                v.val.push_back(static_cast<int>(i));
            }
        }
        v.print();
        variables.push_back(v);
    }
}

/*void modifyVariable(FileContent content, int& line, std::vector<var>& variables, std::ofstream* stateFile) {
    auto varname = findVariableName(content, line);
    auto keywords = get_all_keywords_in_line(content, line);
    if (contains(content[line], "++")) {
        for (auto& vars : variables) {
            if (varname == vars.name) vars.val[0] += "+1"; line++;
        }
    }
    if (contains(content[line], "--")) {
        for (auto& vars : variables) {
            if (varname == vars.name) vars.val[0] += "-1"; line++;
        }
    }
    if (contains(keywords, "state") && stateFile != nullptr) {
        if (stateFile->is_open()) *stateFile << varname << ":" << keywords[keywords.size()-1] << std::endl;
    }
}*/

program::program() {};

program::program(std::string path) : file(path) {
    content = getFileContent(path, lines);
}
void program::init(std::string path) {
    content = getFileContent(path, lines);
    file = path;
}
void program::initVariables() {
    for (int i = 0; i < content.size(); i++) {
        findVariable(content, i, variables);
    }
}

lstream::lstream(std::string path) {
    m_program.init(path);
    m_program.initVariables();
}

FileContent lstream::getContent() {
    return m_program.getContent();
}

LineContent lstream::getLine(int line) {
    return get_all_keywords_in_line(m_program.getContent(), line);
}

void lstream::printVariables() {
    for (auto i : m_program.getVariables()) {
        i.print();
    }
}