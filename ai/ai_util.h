#ifndef AI_UTIL_H__
#define AI_UTIL_H__

#include "../core/position.h"

namespace ai {
  
double MaterialDifference(const core::Position& position, double pawn_value,
                          double knight_value, double bishop_value,
                          double rook_value, double queen_value);

double RandomDouble(double min, double max);

}

#endif
