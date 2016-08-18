#ifndef BOARD_H__
#define BOARD_H__

#include "position.h"
#include "movegen.h"

#include <set>
#include <string>

namespace core {

class Board {
public:
  Board();
  ~Board();

  void LoadStartPos();

  void LoadFromFen(std::string position, std::string active_color,
    std::string castle, std::string en_passant, std::string halfmove_clock,
    std::string fullmove_number);

  void LoadMove(std::string move);

  std::set<std::string> GetLegalMoves();

  std::string BestMove();

private:
  Position *position;
  MoveGen *move_generator;
};

}

#endif

