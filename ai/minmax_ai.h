#ifndef MIN_MAX_AI_H__
#define MIN_MAX_AI_H__

#include "chess_ai.h"
#include "../core/position.h"

#include <string>

namespace ai {
  
class NoMovesException {};

struct MoveScore {
  int move_index;
  double score;
  
  MoveScore(int index, double score) {
    this->move_index = index;
    this->score = score;
  }
};

class MinMaxAI : public ChessAI {
 public:
  MinMaxAI();

  virtual std::string BestMove(core::Position position);
  
  virtual double Evaluate(core::Position position) = 0;
 private:
  MoveScore MinMax(core::Position position, int depth, double alpha, double beta, int move_index);
 
  core::Color player_color;
};

}

#endif
