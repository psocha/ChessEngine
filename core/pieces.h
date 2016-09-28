#ifndef PIECES_H__
#define PIECES_H__

#include <map>

namespace core {

enum SquareContents {
  EMPTY = 0,

  PAWN_W =   1,
  KNIGHT_W = 2,
  BISHOP_W = 3,
  ROOK_W =   4,
  QUEEN_W =  5,
  KING_W =   6,

  PAWN_B =   -1,
  KNIGHT_B = -2,
  BISHOP_B = -3,
  ROOK_B =   -4,
  QUEEN_B =  -5,
  KING_B =   -6,

  OUT_OF_BOUNDS = 99
};

enum PieceType {
  NULL_PIECE = 0,
  PAWN =   1,
  KNIGHT = 2,
  BISHOP = 3,
  ROOK =   4,
  QUEEN =  5,
  KING =   6
};

enum Color {
  NULL_COLOR,
  WHITE,
  BLACK
};

const std::map<SquareContents, char> PIECE_REPRESENTATIONS =
  {{EMPTY, '-'}, {PAWN_W, 'P'}, {KNIGHT_W, 'N'}, {BISHOP_W, 'B'}, {ROOK_W, 'R'},
  {QUEEN_W, 'Q'}, {KING_W, 'K'}, {PAWN_B, 'p'}, {KNIGHT_B, 'n'}, {BISHOP_B, 'b'}, 
  {ROOK_B, 'r'}, {QUEEN_B, 'q'}, {KING_B, 'k'}, {OUT_OF_BOUNDS, '#'}};

PieceType GetPieceType(SquareContents piece);

char CharFromSquareContents(SquareContents contents);
SquareContents SquareContentsFromChar(char piece);

Color ColorFromChar(char color);

Color ColorOfContents(SquareContents contents);

bool SameColors(Color firist, Color second);
bool OppositeColors(Color first, Color second);

}

#endif
