#include "ai_util.h"

using core::Position;
using core::Square;
using core::SquareContents;

namespace ai {

double MaterialDifference(const Position& position, double pawn_value,
                          double knight_value, double bishop_value,
                          double rook_value, double queen_value) {
                            
  double difference = 0.0;
  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      Square square(rank, file);
      SquareContents contents = position.ContentsAt(square);
      if (contents == core::EMPTY) continue;
      
      double piece_value = 0.0;
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
          piece_value = 0.0;
      }
      
      if (ColorOfContents(contents) == core::BLACK) {
        piece_value *= -1;
      }
      difference += piece_value;
    }
  }

  return difference;
}

}
