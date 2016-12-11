#include "ai_util.h"

using core::Position;
using core::Square;
using core::SquareContents;

namespace ai {

int MaterialDifference(const Position& position, int pawn_value,
                       int knight_value, int bishop_value,
                       int rook_value, int queen_value) {
                            
  int difference = 0;
  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      Square square(rank, file);
      SquareContents contents = position.ContentsAt(square);
      if (contents == core::EMPTY) continue;
      
      int piece_value = 0;
      switch (GetPieceType(contents)) {
        case core::PAWN:
          piece_value = pawn_value;
          break;
        case core::KNIGHT:
          piece_value = knight_value;
          break;
        case core::BISHOP:
          piece_value = bishop_value;
          break;
        case core::ROOK:
          piece_value = rook_value;
          break;
        case core::QUEEN:
          piece_value = queen_value;
          break;
        default:
          piece_value = 0;
      }
      
      if (ColorOfContents(contents) == core::BLACK) {
        piece_value *= -1;
      }
      difference += piece_value;
    }
  }

  return difference;
}

int RandomInt(int min, int max) {
  return (max - min) * (rand() / RAND_MAX) + min;
}

}
