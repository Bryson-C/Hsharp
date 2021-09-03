#include "lang_stream/lang_stream.h"



class ttt {
    private:
        char plays[9] = {'.'};
        bool m_hasWon = false;

    public:
        void printBoard() {
            for (int i = 0; i < 9; i++) {
                if (i%3 == 0) printf("\n");
                printf("  %c  ", plays[i]);
            }
        }
        void play(int spot) {
            if (plays[spot] == '.') {
                plays[spot] = 'x';
            }
        }
        void reset() {
            for (int i = 0; i < 9; i++) {
                plays[i] == 0;
            }
        }
        bool hasWon() { return m_hasWon; }
};



int main() {


    std::string path = getLangFile("lang");
    lstream file(path);
    file.getProgram().getInfo();
    file.printVariables();
    
    

    

    return 0;
}