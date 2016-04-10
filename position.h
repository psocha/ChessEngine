#ifndef POSITION_H__
#define POSITION_H__

#include <map>

struct CastlesAllowed {
	bool whiteKingside = true;
	bool whiteQueenside = true;
	bool blackKingside = true;
	bool blackQueenside = true;
};

struct Square {
	int rank;
	int file;
};

struct Move {
	Square start;
	Square end;
	
	bool is_castle = false;
	bool is_en_passant = false;
	int promoted_piece = 0;
};

class Position {
public:
    static const int EMPTY = 0;
    
    static const int PAWN_W =   1;
    static const int KNIGHT_W = 2;
    static const int BISHOP_W = 3;
    static const int ROOK_W =   4;
    static const int QUEEN_W =  5;
    static const int KING_W =   6;

    static const int PAWN_B =   -1;
    static const int KNIGHT_B = -2;
    static const int BISHOP_B = -3;
    static const int ROOK_B =   -4;
    static const int QUEEN_B =  -5;
    static const int KING_B =   -6;

    static const int OUT_OF_BOUNDS = 99;

    Position();
    ~Position();

    void Print();

    void LoadFromFen();

private:
    int chessboard[12][12];
	
	std::map<int, char> pieceRepresentations;
	
	void InitializeOutOfBounds();
	void InitializePieceRepresentations();
};

#endif
