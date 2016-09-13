#include "move.h"
#include "position.h"

#include <cctype>
#include <cstdlib>
#include <iostream>
using std::map;
using std::string;
using std::vector;

namespace core {

Position::Position() {
  InitializeOutOfBounds();
  InitializePieceRepresentations();
}

Position::~Position() {}

void Position::Print() {
  for (int rank = 9; rank >= 2; rank--) {
    for (int file = 2; file < 10; file++) {
      int piece = chessboard[rank][file];
      char piece_char = pieceRepresentations[piece];
      std::cout << " " << piece_char << " ";
    }
    std::cout << std::endl;
  }
}

void Position::LoadFromFen(string fen) {
  int rank = 9;
  int file = 2;
  for (unsigned int i = 0; i < fen.length(); i++) {
    char ch = fen.at(i);

    if (ch == '/') {
      rank--;
	  file = 2;
      continue;
    }

    if (isdigit(ch)) {
      int squares_to_skip = ch - '0';
      file += squares_to_skip;
    } else {
      int piece = PieceFromChar(ch);
      chessboard[rank][file] = piece;
      file++;
    }
  }
}

void Position::PerformMove(std::string mv) {
  Move move = Move(mv, *this);
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

Square Position::GetEnPassant() const {
  return en_passant_square;
}

int Position::PieceAt(Square square) const {
  return chessboard.at(square.rank + 2).at(square.file + 2);
}

void Position::InitializeOutOfBounds() {
  for (int i = 0; i < 12; i++) {
    chessboard.push_back(vector<int>(12));
    for (int j = 0; j < 12; j++) {
      chessboard[i][j] = EMPTY;
	}
  }

  for (int i = 0; i < 12; i++) {
    if (i < 2 || i > 9) {
      for (int j = 0; j < 12; j++) {
        chessboard[i][j] = OUT_OF_BOUNDS;
      }
    }
    else {
      chessboard[i][0] = OUT_OF_BOUNDS;
      chessboard[i][1] = OUT_OF_BOUNDS;
      chessboard[i][10] = OUT_OF_BOUNDS;
      chessboard[i][11] = OUT_OF_BOUNDS;
    }
  }
}

void Position::InitializePieceRepresentations() {
  pieceRepresentations[ROOK_B] =   'r';
  pieceRepresentations[KNIGHT_B] = 'n';
  pieceRepresentations[BISHOP_B] = 'b';
  pieceRepresentations[QUEEN_B] =  'q';
  pieceRepresentations[KING_B] =   'k';
  pieceRepresentations[PAWN_B] =   'p';
  pieceRepresentations[ROOK_W] =   'R';
  pieceRepresentations[KNIGHT_W] = 'N';
  pieceRepresentations[BISHOP_W] = 'B';
  pieceRepresentations[QUEEN_W] =  'Q';
  pieceRepresentations[KING_W] =   'K';
  pieceRepresentations[PAWN_W] =   'P';
  pieceRepresentations[EMPTY] =    '-';
  pieceRepresentations[OUT_OF_BOUNDS] = '#';
}

int Position::PieceFromChar(char piece) {
  map<int, char>::iterator it;
  for (it = pieceRepresentations.begin(); it != pieceRepresentations.end(); ++it) {
    if (it->second == piece) {
      return it->first;
    }
  }
  return EMPTY;
}

int PieceType(int piece) {
  return abs(piece);
}

}

