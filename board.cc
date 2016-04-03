#include "board.h"

Board::Board() {

}

Board::~Board() {

}

void Board::LoadStartPos() {
    LoadFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", "w", "KQkq", "-", "0", "1"); 
}

void Board::LoadFromFen(string position, string active_color, string castle,
        string en_passant, string halfmove_clock, string fullmove_number) {

}


void Board::LoadMove(string move) {

}

string Board::BestMove() {
    return "d7d5";
}

