#ifndef COORDINATES_H__
#define COORDINATES_H__

struct Square {
	int rank;
	int file;
	
	Square(std::string coordinates);
	std::string ToString();
};

struct Move {
	Square start;
	Square end;
	
	int moving_piece;
	
	bool is_castle = false;
	bool is_en_passant = false;
	int promoted_piece = 0;
	
	Move(std::string coordinates);
	std::string ToString();
};

#endif
