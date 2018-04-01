#ifndef MOVE_H__
#define MOVE_H__

#include "position.h"
#include "square.h"
#include <string>

namespace core {

struct Move {
  Square start_square;
  Square end_square;

  PieceType moving_piece;

  bool is_castle = false;
  bool is_capture = false;
  PieceType promoted_piece = NULL_PIECE;

  Move(std::string coordinates, const Position &initial_position);
  Move(Square start, Square end, const Position &initial_position);
  Move(Square start, Square end, const Position &initial_position, PieceType promotion);

  std::string ToString() const;

 private:
  void DescribeFromPosition(const Position &initial_position);
};

bool operator==(const Move& first, const Move& second);
bool operator<(const Move& first, const Move& second);

}

#endif
