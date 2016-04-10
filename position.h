#ifndef POSITION_H__
#define POSITION_H__

#include <map>

const int EMPTY = 0;
    
const int PAWN_W =   1;
const int KNIGHT_W = 2;
const int BISHOP_W = 3;
const int ROOK_W =   4;
const int QUEEN_W =  5;
const int KING_W =   6;

const int PAWN_B =   -1;
const int KNIGHT_B = -2;
const int BISHOP_B = -3;
const int ROOK_B =   -4;
const int QUEEN_B =  -5;
const int KING_B =   -6;

const int OUT_OF_BOUNDS = 99;

struct CastlesAllowed {
	bool whiteKingside = true;
	bool whiteQueenside = true;
	bool blackKingside = true;
	bool blackQueenside = true;
};

class Position {
public:
    Position();
    ~Position();

    void Print();

    void LoadFromFen();

private:
    int chessboard[12][12];
	CastlesAllowed castles_allowed;
	
	std::map<int, char> pieceRepresentations;
	
	void InitializeOutOfBounds();
	void InitializePieceRepresentations();
};

#endif
