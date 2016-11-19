#ifndef CHESS_AI_H__
#define CHESS_AI_H__

#include "../core/position.h"
#include <string>

namespace ai {

class ChessAI {
 public:
  ChessAI() {}
  virtual ~ChessAI() {}
  
  virtual std::string BestMove(core::Position position) = 0;
};

}

#endif
