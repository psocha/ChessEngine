#include "board.h"
#include "position.h"
#include "movegen.h"

#include <cstdlib>
#include <ctime>
using namespace std;

Board::Board() {
    position = new Position();
    move_generator = new MoveGen();
	
	srand(time(NULL));
}

Board::~Board() {
    delete position;
	delete move_generator;
}

void Board::LoadStartPos() {
    LoadFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", "w", "KQkq", "-", "0", "1"); 
}

void Board::LoadFromFen(string fen_position, string active_color, string castle,
        string en_passant, string halfmove_clock, string fullmove_number) {
	position->LoadFromFen(fen_position);
	position->SetActiveColor(active_color);
	position->SetCastle(castle);
	position->SetEnPassant(en_passant);
}

void Board::LoadMove(string move) {

}

string Board::BestMove() {
    vector<Move> legal_moves = move_generator->AllLegalMoves(position);
	int random_index = rand() % legal_moves.size();
	return legal_moves.at(random_index).ToString();
}
