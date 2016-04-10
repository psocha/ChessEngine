#include "position.h"

#include <iostream>
using namespace std;

Position::Position() {
    InitializeOutOfBounds();
	InitializePieceRepresentations();
}

Position::~Position() {

}

void Position::Print() {
	for (int rank = 11; rank <= 0; rank--) {
		for (int file = 0; file < 12; file++) {
			int piece = chessboard[rank][file];
			char piece_char = pieceRepresentations[piece];
			cout << piece_char;
		}
		cout << endl;
	}
}

void Position::LoadFromFen() {

}

void Position::InitializeOutOfBounds() {
    for (int i = 0; i < 12; i++) {
        if (i < 2 || i > 9) {
            for (int j = 0; j < 12; j++) {
                chessboard[i][j] = OUT_OF_BOUNDS;
            }
        } else {
            chessboard[i][0] = OUT_OF_BOUNDS;
            chessboard[i][1] = OUT_OF_BOUNDS;
            chessboard[i][10] = OUT_OF_BOUNDS;
            chessboard[i][11] = OUT_OF_BOUNDS;
        }
    }
}

void Position::InitializePieceRepresentations() {
	pieceRepresentations[ROOK_B]   = 'r';
	pieceRepresentations[KNIGHT_B] = 'n';
	pieceRepresentations[BISHOP_B] = 'b';
	pieceRepresentations[QUEEN_B]  = 'q';
	pieceRepresentations[KING_B]   = 'k';
	pieceRepresentations[PAWN_B]   = 'p';
	pieceRepresentations[ROOK_W]   = 'R';
	pieceRepresentations[KNIGHT_W] = 'N';
	pieceRepresentations[BISHOP_W] = 'B';
	pieceRepresentations[QUEEN_W]  = 'Q';
	pieceRepresentations[KING_W]   = 'K';
	pieceRepresentations[PAWN_W]   = 'P';
	pieceRepresentations[EMPTY]    = '-';
	pieceRepresentations[OUT_OF_BOUNDS] = '#';
	/*
	pieceRepresentations = { 
		make_pair('r', ROOK_B),
		make_pair('n', KNIGHT_B),
		make_pair('b', BISHOP_B),
		make_pair('q', QUEEN_B),
		make_pair('k', KING_B),
		make_pair('p', PAWN_B),
		make_pair('R', ROOK_W),
		make_pair('N', KNIGHT_W),
		make_pair('B', BISHOP_W),
		make_pair('Q', QUEEN_W),
		make_pair('K', KING_W),
		make_pair('P', PAWN_W),
		make_pair('-', EMPTY),
		make_pair('#', OUT_OF_BOUNDS)
	};
	*/
}
