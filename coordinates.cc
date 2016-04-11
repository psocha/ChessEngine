#include "coordinates.h"

#include <cstdlib>
using namespace std;
	
Square::Square(std::string coordinates) {
	if (coordinates == "-") {
		is_real_square = false;
		return;
	}
	
	is_real_square = true;
	rank = atoi(coordinates[1]);
	file = atoi(coordinates[0] - 'a');
}

string Square::ToString() {
	if (!is_real_square) {
		return "-";
	}
	
	string square_representation;
	square_representation += itoa(file + 'a');
	square_representation += itoa(rank);
	
	return square_representation;
}
	
Move::Move(std::string coordinates) {
	
}
	
string Move::ToString() {
	string move_string = start_square.ToString() + end_square.ToString();
	if (promoted_piece) {
		char piece;
		switch abs(promoted_piece) {
			case QUEEN_W:
				piece = 'q';
				break;
			case ROOK_W:
				piece = 'r';
				break;
			case BISHOP_W:
				piece = 'b';
				break;
			case KNIGHT_W:
				piece = 'n';
				break;
		}
		move_string += piece;
	}
	return move_string;
}
