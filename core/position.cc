#include "move.h"
#include "position.h"

#include <iostream>
using std::map;
using std::string;
using std::vector;

namespace core {

Position::Position() {
  InitializeEmptyBoard();
}

Position::~Position() {}

void Position::Print() {
  for (int rank = 9; rank >= 2; rank--) {
    for (int file = 2; file < 10; file++) {
      SquareContents piece = chessboard[rank][file];
      char piece_char = CharFromSquareContents(piece);
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
      SquareContents piece = SquareContentsFromChar(ch);
      chessboard[rank][file] = piece;
      file++;
    }
  }
}

void Position::PerformMove(std::string mv) {
  Move move = Move(mv, *this);
}

void Position::SetActiveColor(std::string active_color) {
  active_color = ColorFromChar(active_color.at(0));
}

Color Position::GetActiveColor() const {
  return active_color;
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

SquareContents Position::ContentsAt(Square square) const {
  return chessboard.at(square.rank + 2).at(square.file + 2);
}

void Position::InitializeEmptyBoard() {
  for (int i = 0; i < 12; i++) {
    chessboard.push_back(vector<SquareContents>(12));
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

}
