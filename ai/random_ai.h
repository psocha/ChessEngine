#ifndef RANDOM_AI_H__
#define RANDOM_AI_H__

#include "chess_ai.h"
#include <string>

namespace ai {

class RandomAI : public ChessAI {
 public:
  RandomAI(core::Board *board);

  std::string BestMove();

};

}

#endif
