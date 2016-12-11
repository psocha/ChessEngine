#ifndef AI_UTIL_H__
#define AI_UTIL_H__

#include "../core/position.h"

namespace ai {
  
int MaterialDifference(const core::Position& position, int pawn_value,
                       int knight_value, int bishop_value,
                       int rook_value, int queen_value);

int RandomInt(int min, int max);

}

#endif
