#include "position.h"

#include <cctype>
#include <cstdlib>
#include <iostream>
using namespace std;

Position::Position() {
    InitializeOutOfBounds();
	InitializePieceRepresentations();
}

Position::~Position() {}

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

void Position::LoadFromFen(string fen) {
	int rank = 9;
	int file = 2;
	for (unsigned int i = 0; i < fen.length(); i++) {
		char ch = fen.at(i);
		
		if (ch == '/') {
			continue;
		}
		
		if (isdigit(ch)) {
			int squares_to_skip = ch - '0';
			for (int sq = 0; sq < squares_to_skip; sq++) {
				file++;
				if (file > 9) {
					file = 2;
					rank--;
				}
			}
		} else {
			int piece = PieceFromChar(ch);
			chessboard[rank][file] = piece;
			file++;
			if (file > 9) {
				file = 2;
				rank--;
			}
		}
	}
}

void Position::SetActiveColor(std::string active_color) {
	active_color = active_color.at(0);
}
	
void Position::SetCastle(std::string castle) {
	castles_allowed.whiteKingside = castle.find('K') != string::npos;
	castles_allowed.whiteQueenside = castle.find('Q') != string::npos;
	castles_allowed.blackKingside = castle.find('k') != string::npos;
	castles_allowed.whiteQueenside = castle.find('q') != string::npos;
}

void Position::SetEnPassant(std::string en_passant) {
	en_passant_square = Square(en_passant);
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
}

int Position::PieceFromChar(char piece) {
	map<int, char>::iterator it;
	for (it = pieceRepresentations.begin(); it != pieceRepresentations.end(); ++it ) {
		if (it->second == piece) {
			return it->first;
		}
	}
	return EMPTY;
}
