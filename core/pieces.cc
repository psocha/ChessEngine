#include "pieces.h"

#include <cstdlib>

namespace core {

PieceType GetPieceType(SquareContents piece) {
  return (PieceType)abs((int)piece);
}

char CharFromSquareContents(SquareContents contents) {
  return PIECE_REPRESENTATIONS.at(contents);
}

SquareContents SquareContentsFromChar(char piece) {
  std::map<SquareContents, char>::const_iterator it;
  for (it = PIECE_REPRESENTATIONS.begin(); it != PIECE_REPRESENTATIONS.end(); ++it) {
    if (it->second == piece) {
      return it->first;
    }
  }
  return OUT_OF_BOUNDS;
}

Color ColorFromChar(char color) {
  if (color == 'w') return WHITE;
  else if (color == 'b') return BLACK;
  else return NULL_COLOR;
}

Color ColorOfContents(SquareContents contents) {
  if (contents >= PAWN_W && contents <= KING_W) return WHITE;
  if (contents <= PAWN_B && contents >= KING_B) return BLACK;
  return NULL_COLOR;
}

bool SameColors(Color first, Color second) {
  if (first == WHITE && second == WHITE) return true;
  if (first == BLACK && second == BLACK) return true;
  return false;
}

bool OppositeColors(Color first, Color second) {
  if (first == WHITE && second == BLACK) return true;
  if (first == BLACK && second == WHITE) return true;
  return false;
}

}
