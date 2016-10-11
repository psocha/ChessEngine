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
  
  SquareContents moving_piece = ContentsAt(move.start_square);
  chessboard.at(move.start_square.rank + 2).at(move.start_square.file + 2) = EMPTY;
  chessboard.at(move.end_square.rank + 2).at(move.end_square.file + 2) = moving_piece;
  
  if (GetPieceType(moving_piece) == PAWN && move.promoted_piece != NULL_PIECE) {
    chessboard.at(move.end_square.rank + 2).at(move.end_square.file + 2) =
      MakePiece(move.promoted_piece, this->active_color);
  }
  
  if (moving_piece == KING_W) {
    castles_allowed.white_kingside = false;
    castles_allowed.white_queenside = false;
  }
  if (moving_piece == KING_B) {
    castles_allowed.black_kingside = false;
    castles_allowed.black_queenside = false;
  }
  
  if (moving_piece == KING_W && move.ToString() == "e1g1") {
    chessboard[0 + 2][7 + 2] = EMPTY;
    chessboard[0 + 2][5 + 2] = ROOK_W;
  }
  if (moving_piece == KING_W && move.ToString() == "e1c1") {
    chessboard[0 + 2][0 + 2] = EMPTY;
    chessboard[0 + 2][3 + 2] = ROOK_W;
  }
  if (moving_piece == KING_B && move.ToString() == "e8g8") {
    chessboard[7 + 2][7 + 2] = EMPTY;
    chessboard[7 + 2][5 + 2] = ROOK_B;
  }
  if (moving_piece == KING_B && move.ToString() == "e8c8") {
    chessboard[7 + 2][0 + 2] = EMPTY;
    chessboard[7 + 2][3 + 2] = ROOK_B;
  }
  
  if (move.start_square == Square("h1") || move.end_square == Square("h1")) {
    castles_allowed.white_kingside = false;
  }
  if (move.start_square == Square("a1") || move.end_square == Square("a1")) {
    castles_allowed.white_queenside = false;
  }
  if (move.start_square == Square("h8") || move.end_square == Square("h8")) {
    castles_allowed.black_kingside = false;
  }
  if (move.start_square == Square("a8") || move.end_square == Square("a8")) {
    castles_allowed.black_queenside = false;
  }
  
  if (GetPieceType(moving_piece) == PAWN && move.end_square == this->en_passant_square) {
    if (active_color == WHITE) {
      chessboard[2 + en_passant_square.rank - 1][2 + en_passant_square.file] = EMPTY;
    } else {
      chessboard[2 + en_passant_square.rank + 1][2 + en_passant_square.file] = EMPTY;
    }
  }
  
  SetEnPassant("-");
  
  if (moving_piece == PAWN_W && move.start_square.rank == 1 && move.end_square.rank == 3) {
    en_passant_square = Square(move.start_square.rank + 1, move.start_square.file);
  }
  if (moving_piece == PAWN_B && move.start_square.rank == 6 && move.end_square.rank == 4) {
    en_passant_square = Square(move.start_square.rank - 1, move.start_square.file);
  }
  
  if (ColorOfContents(moving_piece) == WHITE) {
    active_color = BLACK;
  } else {
    active_color = WHITE;
  }
}

void Position::SetActiveColor(std::string color_marker) {
  active_color = ColorFromChar(color_marker.at(0));
}

Color Position::GetActiveColor() const {
  return active_color;
}

void Position::SetCastle(std::string castle) {
  castles_allowed.white_kingside = castle.find('K') != string::npos;
  castles_allowed.white_queenside = castle.find('Q') != string::npos;
  castles_allowed.black_kingside = castle.find('k') != string::npos;
  castles_allowed.black_queenside = castle.find('q') != string::npos;
}

CastlesAllowed Position::GetCastle() const {
  return castles_allowed;
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
