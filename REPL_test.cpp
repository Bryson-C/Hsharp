#include <iostream>
#include <string>
#include <fstream>
#include <vector>


template<typename T>
void p(T t) { std::cout << t; }
template<typename T>
void psp(T t) { std::cout << t << " "; }
template<typename T>
void pln(T t) { std::cout << t << std::endl; }

bool contains(std::string str, std::string substr) {
    return (str.find(substr) != std::string::npos);
}

struct var {
    std::string name, type;
    std::vector<int> val;
};
void pv(var v) {
    p("("); p(v.type); p(") ");
    p(v.name); p(": ");
    if (v.val.size() > 1) { p("["); }
    for (int i = 0; i < v.val.size(); i++) {
        p(v.val[i]);
        if (i < v.val.size()-1) p(", ");
    }
    if (v.val.size() > 1) { p("]"); } p("\n");
}

int main()
{
    std::vector<var> vars;
    std::string inp = " ";
    while (inp != "!q") {
    
        /*
          TODO:
            - Arrays ( [] )
            - Strings ( "" )
            - Make Variable have unique names
        */
        if (contains(inp, "set")) {
            var v;
            int i = inp.find('<')+1;
            while (inp[i] != '>') {
                v.type += inp[i];
                i++;
            } i++;
            while (inp[i] != ':') {
                v.name += inp[i];
                i++;
            } i++;
            std::string buffer; // buffer needs to parse arrays ( ->[] )
            while (inp[i] != ';') {
                buffer += inp[i];
                i++;
            }
            v.val.push_back(std::stoi(buffer));
            vars.push_back(v);
        } else if (inp == "!v") {
            p("Size Of Variables: ");
            pln(vars.size() * sizeof(var));
            for (auto v : vars) {
                pv(v);
            }
        }
    
    
        std::getline(std::cin, inp);    
    }
    
    

    return 0;
}
