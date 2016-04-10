#include "board.h"
#include "position.h"
#include "movegen.h"

Board::Board() {
    position = new Position();
    move_generator = new MoveGen();
}

Board::~Board() {
    delete position;
	delete move_generator;
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
    vector<string> legal_moves = move_generator->AllLegalMoves(position);

}
