#include "board.h"

#include <iostream>
#include <sstream>
#include <string>
using namespace std;

int main() {
    string line;
    cout.setf(ios::unitbuf);

    Board *board = new Board();

    while (getline(cin, line)) {
        if (line == "uci") {
            cout << "id name random" << endl;
            cout << "id author psocha" << endl;
        } else if (line == "isready") {
            cout << "readyok" << endl;
        } else if (line.substr(0, 9) == "position ") {
            istringstream position_line(line);
            string current_word;
            position_line >> current_word;
            position_line >> current_word;
            if (current_word == "startpos") {
                board->LoadStartPos();
            } else {
                   string position, active_color, castle, en_passant, halfmove_clock, fullmove_number;
                   position_line >> position;
                   position_line >> active_color;
                   position_line >> castle;
                   position_line >> en_passant;
                   position_line >> halfmove_clock;
                   position_line >> fullmove_number;

                   board->LoadFromFen(position, active_color, castle, en_passant, halfmove_clock, fullmove_number);
            }

            while (position_line >> current_word) {
                if (current_word == "moves") continue;

                board->LoadMove(current_word);
            }
        } else if (line.substr(0, 3) == "go ") {
            string best_move = board->BestMove();
            cout << "bestmove " << best_move << endl;
        } else if (line == "quit") {
            break;
        }
    }

    delete board;

    return 0;
}

