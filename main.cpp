
#include <string>
#include <vector>
#include <ctype.h>

std::string prgm =  "void main();"
                    "int thing = 10;";


std::vector<std::string> getWords(std::string str) {
    std::vector<std::string> words;
    std::string buffer;
    for (auto i : str) {
        if (isalpha(i)) {
            buffer += i;
        }
        if (isdigit(i)) {
            words.emplace_back(buffer);
            words.clear();
            while (isdigit(i)) { buffer += i; }
        }
        if (!isalnum(i)) {
            words.emplace_back(i);
        }
    }
    return words;
}



int main() {

    for (auto i : getWords(prgm)) {
        printf("%s\n", i.c_str());
    }


}
