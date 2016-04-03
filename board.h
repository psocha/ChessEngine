#ifndef BOARD_H__
#define BOARD_H__

#include <string>
using namespace std;

class Board {
public:  
    Board();
    ~Board();

    void LoadStartPos();

    void LoadFromFen(string position, string active_color, string castle,
                     string en_passant, string halfmove_clock, string fullmove_number);

    void LoadMove(string move);

    string BestMove();
};

#endif

