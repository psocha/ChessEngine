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

}
