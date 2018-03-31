#include "material_positional_ai.h"
#include "ai_util.h"
#include "../core/pieces.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

using core::Color;
using core::WHITE;
using core::Position;
using core::Square;
using core::SquareContents;

namespace ai {

MaterialPositionalAI::MaterialPositionalAI() : MinMaxAI() {
  srand(time(NULL));
}

MaterialPositionalAI::~MaterialPositionalAI() {}

int MaterialPositionalAI::Evaluate(core::Position* position) {
  TotalMaterial total_material = GetTotalMaterial(*position);

  int white_king_adjustment = 0;
  int black_king_adjustment = 0;

  if (total_material.black_king_square.is_real_square) {
    if (total_material.total_white_material < ENDGAME_MATERIAL_THRESHOLD) {
      black_king_adjustment = KING_ENDGAME_TABLE
        [total_material.black_king_square.rank][total_material.black_king_square.file];
    } else {
      black_king_adjustment = KING_MIDDLEGAME_TABLE
        [total_material.black_king_square.rank][total_material.black_king_square.file];
    }
  }

  if (total_material.white_king_square.is_real_square) {
    if (total_material.total_black_material < ENDGAME_MATERIAL_THRESHOLD) {
      white_king_adjustment = KING_ENDGAME_TABLE
        [7 - total_material.white_king_square.rank][total_material.white_king_square.file];
    } else {
      white_king_adjustment = KING_MIDDLEGAME_TABLE
        [7 - total_material.white_king_square.rank][total_material.white_king_square.file];
    }
  }

  total_material.total_white_material += white_king_adjustment;
  total_material.total_black_material += black_king_adjustment;

  return total_material.total_white_material - total_material.total_black_material
         + RandomInt(-4, 4);
}

TotalMaterial MaterialPositionalAI::GetTotalMaterial(const Position& position) {
  TotalMaterial material;

  for (int rank = 0; rank < 8; rank++) {
    for (int file = 0; file < 8; file++) {
      Square square(rank, file);
      SquareContents contents = position.ContentsAt(square);
      if (contents == core::EMPTY) continue;
      Color color = ColorOfContents(contents);

      int table_rank = color == WHITE ? 7 - rank : rank;
      int table_file = file;

      int piece_value = 0;
      switch (GetPieceType(contents)) {
        case core::PAWN:
          piece_value = PAWN_VALUE + PAWN_TABLE[table_rank][table_file];
          break;
        case core::KNIGHT:
          piece_value = KNIGHT_VALUE + KNIGHT_TABLE[table_rank][table_file];
          break;
        case core::BISHOP:
          piece_value = BISHOP_VALUE + BISHOP_TABLE[table_rank][table_file];
          break;
        case core::ROOK:
          piece_value = ROOK_VALUE + ROOK_TABLE[table_rank][table_file];
          break;
        case core::QUEEN:
          piece_value = QUEEN_VALUE + QUEEN_TABLE[table_rank][table_file];
          break;
        case core::KING:
          if (color == WHITE) {
            material.white_king_square = square;
          } else {
            material.black_king_square = square;
          }
          break;
        default:
          break;
      }

      if (color == WHITE) {
        material.total_white_material += piece_value;
      } else {
        material.total_black_material += piece_value;
      }
    }
  }

  return material;
}

}
