#ifndef CHESS_AI_H__
#define CHESS_AI_H__

#include "../core/board.h"
#include <string>

namespace ai {

class ChessAI {
 public:
  ChessAI(core::Board *board) : board(board) {} 

  virtual std::string BestMove() = 0;

 protected:
  core::Board *board;
};

}

#endif
