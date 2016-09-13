#ifndef MOVE_H__
#define MOVE_H__

#include "position.h"
#include "square.h"
#include <string>

namespace core {

struct Move {
  Square start_square;
  Square end_square;

  int moving_piece;

  bool is_castle = false;
  bool is_en_passant = false;
  int promoted_piece = 0;

  Move(std::string coordinates, const Position &initial_position);
  std::string ToString();
};

}

#endif
