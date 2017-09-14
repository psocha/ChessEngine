#ifndef BOARD_H__
#define BOARD_H__

#include "position.h"
#include "movegen.h"

#include <string>
#include <vector>

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
  void UndoLastMove();

  std::vector<std::string> GetLegalMoves();
  std::vector<std::string> GetPseudolegalMoves();

  Position GetPosition();
  Position* GetPositionRef();

  void Print();

 private:
  Position *position;
};

}

#endif
