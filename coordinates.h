#ifndef COORDINATES_H__
#define COORDINATES_H__

#include <string>

struct Square {
	bool is_real_square = true;
	
	int rank;
	int file;
	
	Square(std::string coordinates);
	std::string ToString();
};

struct Move {
	Square start_square;
	Square end_square;
	
	int moving_piece;
	
	bool is_castle = false;
	bool is_en_passant = false;
	int promoted_piece = 0;
	
	Move(std::string coordinates);
	std::string ToString();
};

#endif
